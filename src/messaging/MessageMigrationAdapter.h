#pragma once

#include "Message.h"
#include "protocol/MessageData.h"
#include "protocol/MessageConfig.h"
#include <esp_log.h>

/**
 * TEMPORARY MIGRATION ADAPTER
 * Helps transition from old message system to new one
 * Will be deleted once migration is complete
 */

namespace Messaging {

// Adapter to make new system work with old API temporarily
class MessageAPI {
public:
    // Subscribe to messages (maps old internal types to new message types)
    static void subscribeToInternal(MessageProtocol::InternalMessageType oldType,
                                  std::function<void(const InternalMessage&)> callback) {
        // Map old internal types to new message types
        Message::Type newType = Message::INVALID;
        
        switch (oldType) {
            case MessageProtocol::InternalMessageType::AUDIO_STATE_UPDATE:
                newType = Message::AUDIO_STATUS;
                break;
            case MessageProtocol::InternalMessageType::ASSET_RESPONSE:
                newType = Message::ASSET_RESPONSE;
                break;
            default:
                ESP_LOGW("Adapter", "Unmapped internal type: %d", (int)oldType);
                return;
        }
        
        // Subscribe with adapter
        MessageRouter::getInstance().subscribe(newType, 
            [callback, oldType](const Message& msg) {
                // Create fake InternalMessage for compatibility
                InternalMessage internal;
                internal.messageType = oldType;
                internal.timestamp = msg.timestamp;
                
                // Convert Message data to AudioStatusData for compatibility
                if (msg.type == Message::AUDIO_STATUS) {
                    AudioStatusData data;
                    data.hasDefaultDevice = msg.data.audio.hasDefaultDevice;
                    data.defaultDevice.friendlyName = msg.data.audio.defaultDeviceName;
                    data.defaultDevice.volume = msg.data.audio.defaultVolume;
                    data.defaultDevice.isMuted = msg.data.audio.defaultIsMuted;
                    data.activeSessionCount = msg.data.audio.activeSessionCount;
                    data.timestamp = msg.timestamp;
                    data.originatingDeviceId = msg.deviceId;
                    data.originatingRequestId = msg.requestId;
                    
                    // Add the current process as a session
                    if (strlen(msg.data.audio.processName) > 0) {
                        SessionStatusData session;
                        session.processName = msg.data.audio.processName;
                        session.displayName = msg.data.audio.processName;
                        session.volume = msg.data.audio.volume;
                        session.isMuted = msg.data.audio.isMuted;
                        data.sessions.push_back(session);
                    }
                    
                    internal.setTypedData(data);
                }
                // Add AssetResponseData conversion if needed
                
                callback(internal);
            });
    }
    
    // Request audio status
    static bool requestAudioStatus() {
        Message msg = Message::createStatusRequest(Config::getDeviceId());
        MessageRouter::getInstance().send(msg);
        return true;
    }
    
    // Create status response
    static ParseResult<String> createStatusResponse(const AudioStatusData& data) {
        Message::AudioData audio;
        
        // Copy data
        strncpy(audio.defaultDeviceName, data.defaultDevice.friendlyName.c_str(), 
                sizeof(audio.defaultDeviceName) - 1);
        audio.hasDefaultDevice = data.hasDefaultDevice;
        audio.defaultVolume = data.defaultDevice.volume;
        audio.defaultIsMuted = data.defaultDevice.isMuted;
        audio.activeSessionCount = data.activeSessionCount;
        
        // If we have sessions, use the first one as the current
        if (!data.sessions.empty()) {
            strncpy(audio.processName, data.sessions[0].processName.c_str(),
                    sizeof(audio.processName) - 1);
            audio.volume = data.sessions[0].volume;
            audio.isMuted = data.sessions[0].isMuted;
        }
        
        Message msg = Message::createAudioStatus(audio, data.originatingDeviceId);
        return ParseResult<String>::createSuccess(msg.toJson());
    }
    
    // Parse external message (for compatibility)
    static ParseResult<ExternalMessage> parseExternalMessage(const String& json) {
        Message msg = Message::fromJson(json);
        
        // Create fake ExternalMessage
        ExternalMessage ext;
        ext.messageType = MessageProtocol::ExternalMessageType::STATUS_MESSAGE;
        ext.deviceId = msg.deviceId;
        ext.requestId = msg.requestId;
        ext.timestamp = msg.timestamp;
        ext.validated = true;
        
        return ParseResult<ExternalMessage>::createSuccess(ext);
    }
    
    // Publish external (compatibility)
    static bool publishExternal(const ExternalMessage& msg) {
        // In new system, we just send directly
        Message newMsg = Message::createStatusRequest(msg.deviceId);
        MessageRouter::getInstance().send(newMsg);
        return true;
    }
    
    // Check health
    static bool isHealthy() {
        return true;  // Simplified system is always healthy!
    }
    
    // Unsubscribe (no-op for now)
    static void unsubscribeFromInternal(MessageProtocol::InternalMessageType type) {
        // No-op - will be cleaned up when we remove this adapter
    }
};



}  // namespace Messaging