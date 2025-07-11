#pragma once

#include "MessageShapes.h"
#include "JsonToVariantConverter.h"

namespace Messaging {

// =============================================================================
// AUDIO STATUS RESPONSE MESSAGE SHAPE
// =============================================================================

DEFINE_MESSAGE_SHAPE(AudioStatusResponseShape, STATUS_MESSAGE,
                     REQUIRED_STRING_FIELD(deviceId, 64)
                         REQUIRED_STRING_FIELD(requestId, 64)
                             OPTIONAL_STRING_FIELD(reason, 128)
                                 REQUIRED_BOOL_FIELD(hasDefaultDevice)
                                     OPTIONAL_STRING_FIELD(defaultDeviceName, 128)
                                         OPTIONAL_FLOAT_FIELD(defaultDeviceVolume, 0.0f, 100.0f)
                                             OPTIONAL_BOOL_FIELD(defaultDeviceIsMuted)
                                                 OPTIONAL_STRING_FIELD(defaultDeviceDataFlow, 32)
                                                     OPTIONAL_STRING_FIELD(defaultDeviceRole, 32)
                                                         REQUIRED_INT_FIELD(activeSessionCount, 0, 1000)
                                                             REQUIRED_INT_FIELD(timestamp, 0, INT_MAX))

// Implementation for AudioStatusResponseShape
IMPLEMENT_MESSAGE_SHAPE(AudioStatusResponseShape,
                        // Validation fields
                        VALIDATE_STRING_FIELD(deviceId)
                            VALIDATE_STRING_FIELD(requestId)
                                VALIDATE_STRING_FIELD(reason)
                                    VALIDATE_BOOL_FIELD(hasDefaultDevice)
                                        VALIDATE_STRING_FIELD(defaultDeviceName)
                                            VALIDATE_FLOAT_FIELD(defaultDeviceVolume)
                                                VALIDATE_BOOL_FIELD(defaultDeviceIsMuted)
                                                    VALIDATE_STRING_FIELD(defaultDeviceDataFlow)
                                                        VALIDATE_STRING_FIELD(defaultDeviceRole)
                                                            VALIDATE_INT_FIELD(activeSessionCount)
                                                                VALIDATE_INT_FIELD(timestamp),

                        // Creation fields
                        ASSIGN_STRING_FIELD(deviceId)
                            ASSIGN_STRING_FIELD(requestId)
                                ASSIGN_STRING_FIELD(reason)
                                    ASSIGN_BOOL_FIELD(hasDefaultDevice)
                                        ASSIGN_STRING_FIELD(defaultDeviceName)
                                            ASSIGN_FLOAT_FIELD(defaultDeviceVolume)
                                                ASSIGN_BOOL_FIELD(defaultDeviceIsMuted)
                                                    ASSIGN_STRING_FIELD(defaultDeviceDataFlow)
                                                        ASSIGN_STRING_FIELD(defaultDeviceRole)
                                                            ASSIGN_INT_FIELD(activeSessionCount)
                                                                ASSIGN_INT_FIELD(timestamp),

                        // Serialization fields
                        SERIALIZE_STRING_FIELD(deviceId)
                            SERIALIZE_STRING_FIELD(requestId)
                                SERIALIZE_STRING_FIELD(reason)
                                    SERIALIZE_BOOL_FIELD(hasDefaultDevice)
                                        SERIALIZE_STRING_FIELD(defaultDeviceName)
                                            SERIALIZE_FLOAT_FIELD(defaultDeviceVolume)
                                                SERIALIZE_BOOL_FIELD(defaultDeviceIsMuted)
                                                    SERIALIZE_STRING_FIELD(defaultDeviceDataFlow)
                                                        SERIALIZE_STRING_FIELD(defaultDeviceRole)
                                                            SERIALIZE_INT_FIELD(activeSessionCount)
                                                                SERIALIZE_INT_FIELD(timestamp),

                        // JSON generation fields
                        JSON_STRING_FIELD(deviceId)
                            JSON_STRING_FIELD(requestId)
                                JSON_STRING_FIELD(reason)
                                    JSON_BOOL_FIELD(hasDefaultDevice)
                                        JSON_STRING_FIELD(defaultDeviceName)
                                            JSON_FLOAT_FIELD(defaultDeviceVolume)
                                                JSON_BOOL_FIELD(defaultDeviceIsMuted)
                                                    JSON_STRING_FIELD(defaultDeviceDataFlow)
                                                        JSON_STRING_FIELD(defaultDeviceRole)
                                                            JSON_INT_FIELD(activeSessionCount)
                                                                JSON_INT_FIELD(timestamp))

// =============================================================================
// ASSET RESPONSE MESSAGE SHAPE
// =============================================================================

DEFINE_MESSAGE_SHAPE(AssetResponseShape, ASSET_RESPONSE,
                     REQUIRED_STRING_FIELD(deviceId, 64)
                         REQUIRED_STRING_FIELD(requestId, 64)
                             REQUIRED_STRING_FIELD(processName, 128)
                                 REQUIRED_BOOL_FIELD(success)
                                     OPTIONAL_STRING_FIELD(errorMessage, 256)
                                         OPTIONAL_STRING_FIELD(assetDataBase64, 8192)  // Base64 encoded asset data
                     OPTIONAL_INT_FIELD(width, 0, 4096)
                         OPTIONAL_INT_FIELD(height, 0, 4096)
                             OPTIONAL_STRING_FIELD(format, 32)
                                 REQUIRED_INT_FIELD(timestamp, 0, INT_MAX))

// Implementation for AssetResponseShape
IMPLEMENT_MESSAGE_SHAPE(AssetResponseShape,
                        // Validation fields
                        VALIDATE_STRING_FIELD(deviceId)
                            VALIDATE_STRING_FIELD(requestId)
                                VALIDATE_STRING_FIELD(processName)
                                    VALIDATE_BOOL_FIELD(success)
                                        VALIDATE_STRING_FIELD(errorMessage)
                                            VALIDATE_STRING_FIELD(assetDataBase64)
                                                VALIDATE_INT_FIELD(width)
                                                    VALIDATE_INT_FIELD(height)
                                                        VALIDATE_STRING_FIELD(format)
                                                            VALIDATE_INT_FIELD(timestamp),

                        // Creation fields
                        ASSIGN_STRING_FIELD(deviceId)
                            ASSIGN_STRING_FIELD(requestId)
                                ASSIGN_STRING_FIELD(processName)
                                    ASSIGN_BOOL_FIELD(success)
                                        ASSIGN_STRING_FIELD(errorMessage)
                                            ASSIGN_STRING_FIELD(assetDataBase64)
                                                ASSIGN_INT_FIELD(width)
                                                    ASSIGN_INT_FIELD(height)
                                                        ASSIGN_STRING_FIELD(format)
                                                            ASSIGN_INT_FIELD(timestamp),

                        // Serialization fields
                        SERIALIZE_STRING_FIELD(deviceId)
                            SERIALIZE_STRING_FIELD(requestId)
                                SERIALIZE_STRING_FIELD(processName)
                                    SERIALIZE_BOOL_FIELD(success)
                                        SERIALIZE_STRING_FIELD(errorMessage)
                                            SERIALIZE_STRING_FIELD(assetDataBase64)
                                                SERIALIZE_INT_FIELD(width)
                                                    SERIALIZE_INT_FIELD(height)
                                                        SERIALIZE_STRING_FIELD(format)
                                                            SERIALIZE_INT_FIELD(timestamp),

                        // JSON generation fields
                        JSON_STRING_FIELD(deviceId)
                            JSON_STRING_FIELD(requestId)
                                JSON_STRING_FIELD(processName)
                                    JSON_BOOL_FIELD(success)
                                        JSON_STRING_FIELD(errorMessage)
                                            JSON_STRING_FIELD(assetDataBase64)
                                                JSON_INT_FIELD(width)
                                                    JSON_INT_FIELD(height)
                                                        JSON_STRING_FIELD(format)
                                                            JSON_INT_FIELD(timestamp))

// =============================================================================
// STATUS REQUEST MESSAGE SHAPE
// =============================================================================

DEFINE_MESSAGE_SHAPE(StatusRequestShape, GET_STATUS,
                     REQUIRED_STRING_FIELD(deviceId, 64)
                         REQUIRED_STRING_FIELD(requestId, 64)
                             REQUIRED_INT_FIELD(timestamp, 0, INT_MAX))

// Implementation for StatusRequestShape
IMPLEMENT_MESSAGE_SHAPE(StatusRequestShape,
                        // Validation fields
                        VALIDATE_STRING_FIELD(deviceId)
                            VALIDATE_STRING_FIELD(requestId)
                                VALIDATE_INT_FIELD(timestamp),

                        // Creation fields
                        ASSIGN_STRING_FIELD(deviceId)
                            ASSIGN_STRING_FIELD(requestId)
                                ASSIGN_INT_FIELD(timestamp),

                        // Serialization fields
                        SERIALIZE_STRING_FIELD(deviceId)
                            SERIALIZE_STRING_FIELD(requestId)
                                SERIALIZE_INT_FIELD(timestamp),

                        // JSON generation fields
                        JSON_STRING_FIELD(deviceId)
                            JSON_STRING_FIELD(requestId)
                                JSON_INT_FIELD(timestamp))

// =============================================================================
// ASSET REQUEST MESSAGE SHAPE
// =============================================================================

DEFINE_MESSAGE_SHAPE(AssetRequestShape, GET_ASSETS,
                     REQUIRED_STRING_FIELD(deviceId, 64)
                         REQUIRED_STRING_FIELD(requestId, 64)
                             REQUIRED_STRING_FIELD(processName, 128)
                                 REQUIRED_INT_FIELD(timestamp, 0, INT_MAX))

// Implementation for AssetRequestShape
IMPLEMENT_MESSAGE_SHAPE(AssetRequestShape,
                        // Validation fields
                        VALIDATE_STRING_FIELD(deviceId)
                            VALIDATE_STRING_FIELD(requestId)
                                VALIDATE_STRING_FIELD(processName)
                                    VALIDATE_INT_FIELD(timestamp),

                        // Creation fields
                        ASSIGN_STRING_FIELD(deviceId)
                            ASSIGN_STRING_FIELD(requestId)
                                ASSIGN_STRING_FIELD(processName)
                                    ASSIGN_INT_FIELD(timestamp),

                        // Serialization fields
                        SERIALIZE_STRING_FIELD(deviceId)
                            SERIALIZE_STRING_FIELD(requestId)
                                SERIALIZE_STRING_FIELD(processName)
                                    SERIALIZE_INT_FIELD(timestamp),

                        // JSON generation fields
                        JSON_STRING_FIELD(deviceId)
                            JSON_STRING_FIELD(requestId)
                                JSON_STRING_FIELD(processName)
                                    JSON_INT_FIELD(timestamp))

// =============================================================================
// MESSAGE SHAPE REGISTRATION
// =============================================================================

/**
 * Register all message shapes with the registry
 */
inline void registerAllMessageShapes() {
    auto& registry = MessageShapeRegistry::instance();

    registry.registerShape<AudioStatusResponseShape>();
    registry.registerShape<AssetResponseShape>();
    registry.registerShape<StatusRequestShape>();
    registry.registerShape<AssetRequestShape>();

    ESP_LOGI("MessageShapes", "Registered %d message shapes", 4);
}

/**
 * Initialize the messaging system
 * Call this once during startup to set up the system
 */
inline void initializeMessagingSystem() {
    ESP_LOGI("Messaging", "Initializing messaging system...");

    // Register all message shapes
    registerAllMessageShapes();

    ESP_LOGI("Messaging", "✅ Messaging system initialized successfully!");
    ESP_LOGI("Messaging", "🚀 Type-safe messaging is now available!");
    ESP_LOGI("Messaging", "💡 Use ExternalMessage::fromJsonString() for parsing");
    ESP_LOGI("Messaging", "📊 Use getTypedData<MessageShapeType>() for type-safe access");
}

}  // namespace Messaging
