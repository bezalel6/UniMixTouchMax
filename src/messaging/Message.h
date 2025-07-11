#pragma once

#include <Arduino.h>
#include <functional>
#include <unordered_map>
#include <memory>
#include <esp_log.h>

namespace Messaging {

/**
 * BRUTALLY SIMPLE MESSAGE SYSTEM
 * No abstractions. No variants. No shapes. Just data.
 */
struct Message {
    // Only the message types we ACTUALLY use
    enum Type {
        INVALID = 0,
        
        // Audio messages
        AUDIO_STATUS,      // Status of all audio devices
        VOLUME_CHANGE,     // Change volume for a process
        MUTE_TOGGLE,       // Mute/unmute a process
        
        // Asset messages  
        ASSET_REQUEST,     // Request logo/asset for process
        ASSET_RESPONSE,    // Response with asset data
        
        // Control messages
        GET_STATUS,        // Request current status
        SET_VOLUME,        // Set volume command
        SET_DEFAULT_DEVICE // Set default audio device
    };
    
    // Core fields every message has
    Type type = INVALID;
    String deviceId;
    String requestId;
    uint32_t timestamp = 0;
    
    // Simple payload data - no complex variants!
    struct AudioData {
        char processName[64] = {0};
        int volume = 0;
        bool isMuted = false;
        bool hasDefaultDevice = false;
        char defaultDeviceName[64] = {0};
        int defaultVolume = 0;
        bool defaultIsMuted = false;
        int activeSessionCount = 0;
    };
    
    struct AssetData {
        char processName[64] = {0};
        bool success = false;
        char errorMessage[128] = {0};
        char assetDataBase64[2048] = {0};  // Base64 encoded asset
        int width = 0;
        int height = 0;
        char format[16] = {0};
    };
    
    struct VolumeData {
        char processName[64] = {0};
        int volume = 0;
        char target[64] = {0};  // "default" or specific device
    };
    
    // Tagged union for payload
    union {
        AudioData audio;
        AssetData asset;
        VolumeData volume;
    } data;
    
    // Constructors for common messages
    static Message createStatusRequest(const String& deviceId);
    static Message createAssetRequest(const String& processName, const String& deviceId);
    static Message createVolumeChange(const String& processName, int volume, const String& deviceId);
    static Message createAudioStatus(const AudioData& audioData, const String& deviceId);
    static Message createAssetResponse(const AssetData& assetData, const String& requestId, const String& deviceId);
    
    // Direct JSON serialization (no shapes, no macros)
    String toJson() const;
    static Message fromJson(const String& json);
    
    // Utility
    const char* typeToString() const;
    static Type stringToType(const String& str);
    bool isValid() const { return type != INVALID; }
};

/**
 * SIMPLE MESSAGE ROUTER
 * No MessageCore complexity. Just route messages to handlers.
 */
class MessageRouter {
private:
    std::unordered_map<Message::Type, std::vector<std::function<void(const Message&)>>> handlers;
    static MessageRouter* instance;
    
public:
    static MessageRouter& getInstance() {
        if (!instance) {
            instance = new MessageRouter();
        }
        return *instance;
    }
    
    // Subscribe to message type
    void subscribe(Message::Type type, std::function<void(const Message&)> handler) {
        handlers[type].push_back(handler);
    }
    
    // Route incoming message to handlers
    void route(const Message& msg) {
        if (!msg.isValid()) {
            ESP_LOGW("Router", "Invalid message type");
            return;
        }
        
        auto it = handlers.find(msg.type);
        if (it != handlers.end()) {
            for (auto& handler : it->second) {
                handler(msg);
            }
        }
    }
    
    // Send message out via serial
    void send(const Message& msg);
    
    // Get handler count for status
    size_t getHandlerCount() const {
        size_t count = 0;
        for (const auto& [type, handlers] : handlers) {
            count += handlers.size();
        }
        return count;
    }
};

// Global convenience functions
inline void sendMessage(const Message& msg) {
    MessageRouter::getInstance().send(msg);
}

inline void subscribe(Message::Type type, std::function<void(const Message&)> handler) {
    MessageRouter::getInstance().subscribe(type, handler);
}

}  // namespace Messaging