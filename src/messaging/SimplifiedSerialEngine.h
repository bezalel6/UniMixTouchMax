#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <driver/uart.h>
#include "Message.h"
#include "protocol/BinaryProtocol.h"

namespace Messaging {

/**
 * SIMPLE SERIAL ENGINE
 * No transport abstraction. Just send and receive messages.
 */
class SerialEngine {
private:
    static SerialEngine* instance;
    
    // UART configuration
    static const int UART_NUM = UART_NUM_0;
    static const int UART_BAUD_RATE = 3000000;  // 3Mbps
    static const size_t RX_BUFFER_SIZE = 4096;
    static const size_t TX_BUFFER_SIZE = 2048;
    
    // Task configuration  
    TaskHandle_t rxTaskHandle = nullptr;
    bool running = false;
    
    // Binary protocol for framing
    BinaryProtocol::BinaryProtocolFramer framer;
    
    // Receive buffer
    char rxBuffer[RX_BUFFER_SIZE];
    size_t rxBufferPos = 0;
    
    // Statistics
    struct Stats {
        uint32_t messagesReceived = 0;
        uint32_t messagesSent = 0;
        uint32_t parseErrors = 0;
        uint32_t framingErrors = 0;
    } stats;
    
    SerialEngine() = default;
    
public:
    static SerialEngine& getInstance() {
        if (!instance) {
            instance = new SerialEngine();
        }
        return *instance;
    }
    
    // Initialize and start the serial engine
    bool init() {
        // Configure UART
        uart_config_t uart_config = {
            .baud_rate = UART_BAUD_RATE,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .rx_flow_ctrl_thresh = 122,
            .source_clk = UART_SCLK_APB,
        };
        
        ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));
        ESP_ERROR_CHECK(uart_set_pin(UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, 
                                     UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
        ESP_ERROR_CHECK(uart_driver_install(UART_NUM, RX_BUFFER_SIZE, TX_BUFFER_SIZE, 0, NULL, 0));
        
        // Start receive task
        xTaskCreatePinnedToCore(
            rxTaskWrapper, 
            "SerialRx",
            4096,
            this,
            5,  // Priority
            &rxTaskHandle,
            1   // Core 1
        );
        
        running = true;
        ESP_LOGI("Serial", "Serial engine initialized");
        return true;
    }
    
    // Send a message
    void send(const Message& msg) {
        if (!running) {
            ESP_LOGW("Serial", "Serial engine not running");
            return;
        }
        
        String json = msg.toJson();
        sendRaw(json);
        stats.messagesSent++;
    }
    
    // Send raw string (for compatibility)
    void sendRaw(const String& data) {
        if (!running) return;
        
        // Frame and send
        uint8_t frameBuffer[TX_BUFFER_SIZE];
        size_t frameSize = framer.frameMessage(
            reinterpret_cast<const uint8_t*>(data.c_str()), 
            data.length(), 
            frameBuffer, 
            sizeof(frameBuffer)
        );
        
        if (frameSize > 0) {
            uart_write_bytes(UART_NUM, frameBuffer, frameSize);
        } else {
            ESP_LOGW("Serial", "Failed to frame message");
        }
    }
    
    // Get statistics
    const Stats& getStats() const { return stats; }
    
    // Stop the engine
    void stop() {
        running = false;
        if (rxTaskHandle) {
            vTaskDelete(rxTaskHandle);
            rxTaskHandle = nullptr;
        }
        uart_driver_delete(UART_NUM);
    }
    
private:
    // Task wrapper
    static void rxTaskWrapper(void* param) {
        static_cast<SerialEngine*>(param)->rxTask();
    }
    
    // Receive task
    void rxTask() {
        uint8_t data[256];
        
        while (running) {
            int len = uart_read_bytes(UART_NUM, data, sizeof(data), 10 / portTICK_PERIOD_MS);
            
            if (len > 0) {
                processIncomingData(data, len);
            }
        }
    }
    
    // Process incoming data
    void processIncomingData(const uint8_t* data, size_t length) {
        for (size_t i = 0; i < length; i++) {
            auto result = framer.processByte(data[i]);
            
            if (result == BinaryProtocol::ProcessResult::MESSAGE_COMPLETE) {
                // Get the complete message
                size_t messageLength = 0;
                const uint8_t* messageData = framer.getMessage(messageLength);
                
                if (messageData && messageLength > 0) {
                    // Null-terminate for JSON parsing
                    String jsonStr;
                    jsonStr.reserve(messageLength + 1);
                    jsonStr.concat(reinterpret_cast<const char*>(messageData), messageLength);
                    
                    // Parse and route the message
                    Message msg = Message::fromJson(jsonStr);
                    if (msg.isValid()) {
                        stats.messagesReceived++;
                        MessageRouter::getInstance().route(msg);
                    } else {
                        stats.parseErrors++;
                        ESP_LOGW("Serial", "Failed to parse message: %.100s", jsonStr.c_str());
                    }
                }
                
                // Reset for next message
                framer.reset();
            } else if (result == BinaryProtocol::ProcessResult::FRAMING_ERROR) {
                stats.framingErrors++;
                framer.reset();
            }
        }
    }
};

// Static member
SerialEngine* SerialEngine::instance = nullptr;

// Global instance accessor for compatibility
inline SerialEngine& getSerialEngine() {
    return SerialEngine::getInstance();
}

}  // namespace Messaging