#include "RefactoredExternalMessage.h"
#include "MessageShapeDefinitions.h"
#include <esp_log.h>

namespace Messaging {

// =============================================================================
// EXAMPLE USAGE OF REFACTORED MESSAGING SYSTEM
// =============================================================================

class RefactoringExampleUsage {
private:
    static constexpr const char* TAG = "RefactoringExample";
    
public:
    /**
     * Example: Process an audio status response message
     */
    static void processAudioStatusResponse(const string& jsonPayload) {
        ESP_LOGI(TAG, "Processing audio status response...");
        
        // STEP 1: Parse JSON to RefactoredExternalMessage
        auto parseResult = RefactoredExternalMessage::fromJsonString(jsonPayload);
        
        if (!parseResult.isValid()) {
            ESP_LOGE(TAG, "Failed to parse message: %s", STRING_C_STR(parseResult.getError()));
            return;
        }
        
        RefactoredExternalMessage message = parseResult.getValue();
        
        // STEP 2: Get type-safe access to message data
        auto audioDataResult = message.getTypedData<AudioStatusResponseShape>();
        
        if (!audioDataResult.isValid()) {
            ESP_LOGE(TAG, "Failed to get audio data: %s", STRING_C_STR(audioDataResult.getError()));
            return;
        }
        
        AudioStatusResponseShape audioData = audioDataResult.getValue();
        
        // STEP 3: Use the strongly-typed data
        ESP_LOGI(TAG, "Audio Status Response:");
        ESP_LOGI(TAG, "  Device ID: %s", STRING_C_STR(audioData.deviceId));
        ESP_LOGI(TAG, "  Request ID: %s", STRING_C_STR(audioData.requestId));
        ESP_LOGI(TAG, "  Has Default Device: %s", audioData.hasDefaultDevice ? "true" : "false");
        ESP_LOGI(TAG, "  Active Sessions: %d", audioData.activeSessionCount);
        
        if (audioData.hasDefaultDevice) {
            ESP_LOGI(TAG, "  Default Device: %s", STRING_C_STR(audioData.defaultDeviceName));
            ESP_LOGI(TAG, "  Default Volume: %.1f", audioData.defaultDeviceVolume);
            ESP_LOGI(TAG, "  Default Muted: %s", audioData.defaultDeviceIsMuted ? "true" : "false");
        }
        
        // STEP 4: Process the data (your business logic here)
        handleAudioStatus(audioData);
    }
    
    /**
     * Example: Process an asset response message
     */
    static void processAssetResponse(const string& jsonPayload) {
        ESP_LOGI(TAG, "Processing asset response...");
        
        // Parse and get typed data in one go
        auto parseResult = RefactoredExternalMessage::fromJsonString(jsonPayload);
        if (!parseResult.isValid()) {
            ESP_LOGE(TAG, "Parse failed: %s", STRING_C_STR(parseResult.getError()));
            return;
        }
        
        auto assetDataResult = parseResult.getValue().getTypedData<AssetResponseShape>();
        if (!assetDataResult.isValid()) {
            ESP_LOGE(TAG, "Type conversion failed: %s", STRING_C_STR(assetDataResult.getError()));
            return;
        }
        
        AssetResponseShape assetData = assetDataResult.getValue();
        
        ESP_LOGI(TAG, "Asset Response:");
        ESP_LOGI(TAG, "  Process: %s", STRING_C_STR(assetData.processName));
        ESP_LOGI(TAG, "  Success: %s", assetData.success ? "true" : "false");
        
        if (assetData.success) {
            ESP_LOGI(TAG, "  Dimensions: %dx%d", assetData.width, assetData.height);
            ESP_LOGI(TAG, "  Format: %s", STRING_C_STR(assetData.format));
            ESP_LOGI(TAG, "  Data size: %zu bytes", STRING_LENGTH(assetData.assetDataBase64));
        } else {
            ESP_LOGE(TAG, "  Error: %s", STRING_C_STR(assetData.errorMessage));
        }
        
        handleAssetResponse(assetData);
    }
    
    /**
     * Example: Generic message handler using type checking
     */
    static void processGenericMessage(const string& jsonPayload) {
        ESP_LOGI(TAG, "Processing generic message...");
        
        auto parseResult = RefactoredExternalMessage::fromJsonString(jsonPayload);
        if (!parseResult.isValid()) {
            ESP_LOGE(TAG, "Parse failed: %s", STRING_C_STR(parseResult.getError()));
            return;
        }
        
        RefactoredExternalMessage message = parseResult.getValue();
        
        // Route based on message type
        switch (message.getMessageType()) {
            case MessageProtocol::ExternalMessageType::STATUS_RESPONSE:
                if (message.canAccessAs<AudioStatusResponseShape>()) {
                    auto audioData = message.getTypedData<AudioStatusResponseShape>().getValue();
                    handleAudioStatus(audioData);
                }
                break;
                
            case MessageProtocol::ExternalMessageType::ASSET_RESPONSE:
                if (message.canAccessAs<AssetResponseShape>()) {
                    auto assetData = message.getTypedData<AssetResponseShape>().getValue();
                    handleAssetResponse(assetData);
                }
                break;
                
            case MessageProtocol::ExternalMessageType::STATUS_REQUEST:
                if (message.canAccessAs<StatusRequestShape>()) {
                    auto requestData = message.getTypedData<StatusRequestShape>().getValue();
                    handleStatusRequest(requestData);
                }
                break;
                
            case MessageProtocol::ExternalMessageType::ASSET_REQUEST:
                if (message.canAccessAs<AssetRequestShape>()) {
                    auto requestData = message.getTypedData<AssetRequestShape>().getValue();
                    handleAssetRequest(requestData);
                }
                break;
                
            default:
                ESP_LOGW(TAG, "Unknown message type: %d", static_cast<int>(message.getMessageType()));
                break;
        }
    }
    
    /**
     * Example: Migration from old system to new system
     */
    static void migrationExample_OldVsNew(const string& jsonPayload) {
        ESP_LOGI(TAG, "Migration example: Old vs New approach");
        
        // OLD WAY (would use ExternalMessage with JsonDocument)
        ESP_LOGI(TAG, "=== OLD WAY (conceptual) ===");
        /*
        ExternalMessage oldMessage;
        // ... parsing logic ...
        string deviceId = oldMessage.getString("deviceId");
        bool hasDevice = oldMessage.getBool("hasDefaultDevice");
        float volume = oldMessage.getFloat("defaultDeviceVolume");
        // ... lots of manual field extraction ...
        // ... no type safety, easy to make mistakes ...
        */
        
        // NEW WAY (using RefactoredExternalMessage)
        ESP_LOGI(TAG, "=== NEW WAY ===");
        
        auto parseResult = RefactoredExternalMessage::fromJsonString(jsonPayload);
        if (!parseResult.isValid()) {
            ESP_LOGE(TAG, "Parse failed: %s", STRING_C_STR(parseResult.getError()));
            return;
        }
        
        RefactoredExternalMessage message = parseResult.getValue();
        
        // Type-safe access with compile-time verification
        auto audioDataResult = message.getTypedData<AudioStatusResponseShape>();
        if (audioDataResult.isValid()) {
            AudioStatusResponseShape audioData = audioDataResult.getValue();
            
            // All fields are strongly typed and validated
            ESP_LOGI(TAG, "Device: %s", STRING_C_STR(audioData.deviceId));
            ESP_LOGI(TAG, "Volume: %.1f", audioData.defaultDeviceVolume);
            ESP_LOGI(TAG, "Sessions: %d", audioData.activeSessionCount);
            
            // No more manual field extraction or type conversion errors!
        }
        
        // Backward compatibility is still available if needed
        ESP_LOGI(TAG, "=== BACKWARD COMPATIBILITY ===");
        string legacyDeviceId = message.getString("deviceId");
        bool legacyHasDevice = message.getBool("hasDefaultDevice");
        ESP_LOGI(TAG, "Legacy access: %s, %s", 
                 STRING_C_STR(legacyDeviceId), 
                 legacyHasDevice ? "true" : "false");
    }
    
    /**
     * Example: Performance comparison
     */
    static void performanceExample(const string& jsonPayload) {
        ESP_LOGI(TAG, "Performance example");
        
        unsigned long startTime = micros();
        
        // Parse once
        auto parseResult = RefactoredExternalMessage::fromJsonString(jsonPayload);
        if (!parseResult.isValid()) {
            return;
        }
        
        RefactoredExternalMessage message = parseResult.getValue();
        
        // Multiple type-safe accesses are now very fast (no JSON re-parsing)
        auto audioData1 = message.getTypedData<AudioStatusResponseShape>();
        auto audioData2 = message.getTypedData<AudioStatusResponseShape>();
        auto audioData3 = message.getTypedData<AudioStatusResponseShape>();
        
        unsigned long endTime = micros();
        
        ESP_LOGI(TAG, "Parse + 3 typed accesses took: %lu microseconds", endTime - startTime);
        
        // Old system would have to re-parse JSON fields every time!
    }
    
private:
    // Mock handlers for the examples
    static void handleAudioStatus(const AudioStatusResponseShape& audioData) {
        ESP_LOGD(TAG, "Handling audio status for device: %s", STRING_C_STR(audioData.deviceId));
        // Your audio processing logic here
    }
    
    static void handleAssetResponse(const AssetResponseShape& assetData) {
        ESP_LOGD(TAG, "Handling asset response for process: %s", STRING_C_STR(assetData.processName));
        // Your asset processing logic here
    }
    
    static void handleStatusRequest(const StatusRequestShape& requestData) {
        ESP_LOGD(TAG, "Handling status request from device: %s", STRING_C_STR(requestData.deviceId));
        // Your status request logic here
    }
    
    static void handleAssetRequest(const AssetRequestShape& requestData) {
        ESP_LOGD(TAG, "Handling asset request for process: %s", STRING_C_STR(requestData.processName));
        // Your asset request logic here
    }
};

// =============================================================================
// INITIALIZATION EXAMPLE
// =============================================================================

/**
 * Example initialization function that should be called at startup
 */
void initializeRefactoredMessaging() {
    ESP_LOGI("RefactoredMessaging", "Initializing refactored messaging system...");
    
    // Register all message shapes
    registerAllMessageShapes();
    
    // Test with some sample data
    string sampleJsonAudio = R"({
        "messageType": "STATUS_RESPONSE",
        "deviceId": "ESP32-AudioMixer-001",
        "requestId": "req-12345",
        "timestamp": 1234567890,
        "hasDefaultDevice": true,
        "defaultDeviceName": "Speakers",
        "defaultDeviceVolume": 75.5,
        "defaultDeviceIsMuted": false,
        "activeSessionCount": 3,
        "reason": "Periodic update"
    })";
    
    RefactoringExampleUsage::processAudioStatusResponse(sampleJsonAudio);
    
    ESP_LOGI("RefactoredMessaging", "Refactored messaging system initialized successfully!");
}

} // namespace Messaging