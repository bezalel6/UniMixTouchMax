#include "MessageData.h"
#include <esp_log.h>

namespace Messaging {

// =============================================================================
// EXTERNAL MESSAGE METHODS
// =============================================================================

bool ExternalMessage::isSelfOriginated() const {
    return deviceId == Config::getDeviceId() ||
           originatingDeviceId == Config::getDeviceId();
}

bool ExternalMessage::requiresResponse() const {
    switch (messageType) {
        case MessageProtocol::ExternalMessageType::GET_STATUS:
        case MessageProtocol::ExternalMessageType::GET_ASSETS:
            return true;
        default:
            return false;
    }
}

// =============================================================================
// INTERNAL MESSAGE CONSTRUCTORS
// =============================================================================

InternalMessage::InternalMessage() {
    messageType = MessageProtocol::InternalMessageType::INVALID;
    timestamp = millis();
    priority = static_cast<uint8_t>(MessageProtocol::getInternalMessagePriority(messageType));
}

InternalMessage::InternalMessage(MessageProtocol::InternalMessageType type, const void* payload, size_t size)
    : messageType(type), dataSize(size) {
    timestamp = millis();
    priority = static_cast<uint8_t>(MessageProtocol::getInternalMessagePriority(type));

    if (payload && size > 0) {
        data.reset(new uint8_t[size]);
        memcpy(data.get(), payload, size);
    }
}

// =============================================================================
// ASSET RESPONSE DATA CONSTRUCTOR
// =============================================================================

AssetResponseData::AssetResponseData(const ExternalMessage& external) {
    requestId = external.requestId;
    deviceId = external.deviceId;
    timestamp = external.timestamp;

    // Extract asset-specific fields from parsed data
    processName = external.get<String>("processName", "");
    success = external.get<bool>("success", false);
    errorMessage = external.get<String>("errorMessage", "");
    assetDataBase64 = external.get<String>("assetData", "");

    if (external.hasField("metadata")) {
        auto metadata = external.parsedData["metadata"];
        if (!metadata.isNull()) {
            width = metadata["width"].as<int>();
            height = metadata["height"].as<int>();
            format = metadata["format"].as<String>();
        }
    }
}

// =============================================================================
// MESSAGE FACTORY IMPLEMENTATIONS
// =============================================================================

ExternalMessage MessageFactory::createStatusRequest(const String& deviceId) {
    String devId = deviceId.isEmpty() ? Config::getDeviceId() : deviceId;
    ExternalMessage message(MessageProtocol::ExternalMessageType::GET_STATUS,
                           Config::generateRequestId(), devId);
    return message;
}

ExternalMessage MessageFactory::createAssetRequest(const String& processName, const String& deviceId) {
    String devId = deviceId.isEmpty() ? Config::getDeviceId() : deviceId;
    ExternalMessage message(MessageProtocol::ExternalMessageType::GET_ASSETS,
                           Config::generateRequestId(), devId);

    // Add process name to parsed data
    message.parsedData["processName"] = processName;
    return message;
}

InternalMessage MessageFactory::createAudioVolumeMessage(const String& processName, int volume) {
    struct AudioVolumeData {
        char processName[64];
        int volume;
    };

    AudioVolumeData data;
    strncpy(data.processName, processName.c_str(), sizeof(data.processName) - 1);
    data.processName[sizeof(data.processName) - 1] = '\0';
    data.volume = volume;

    return InternalMessage(MessageProtocol::InternalMessageType::AUDIO_STATE_UPDATE, &data, sizeof(data));
}

InternalMessage MessageFactory::createUIUpdateMessage(const String& component, const String& data) {
    struct UIUpdateData {
        char component[32];
        char data[128];
    };

    UIUpdateData updateData;
    strncpy(updateData.component, component.c_str(), sizeof(updateData.component) - 1);
    updateData.component[sizeof(updateData.component) - 1] = '\0';
    strncpy(updateData.data, data.c_str(), sizeof(updateData.data) - 1);
    updateData.data[sizeof(updateData.data) - 1] = '\0';

    return InternalMessage(MessageProtocol::InternalMessageType::UI_UPDATE, &updateData, sizeof(updateData));
}

InternalMessage MessageFactory::createSystemStatusMessage(const String& status) {
    struct SystemStatusData {
        char status[64];
    };

    SystemStatusData data;
    strncpy(data.status, status.c_str(), sizeof(data.status) - 1);
    data.status[sizeof(data.status) - 1] = '\0';

    return InternalMessage(MessageProtocol::InternalMessageType::SYSTEM_STATUS, &data, sizeof(data));
}

InternalMessage MessageFactory::createWifiStatusMessage(const String& status, bool connected) {
    struct WifiStatusData {
        char status[32];
        bool connected;
    };

    WifiStatusData data;
    strncpy(data.status, status.c_str(), sizeof(data.status) - 1);
    data.status[sizeof(data.status) - 1] = '\0';
    data.connected = connected;

    return InternalMessage(MessageProtocol::InternalMessageType::WIFI_STATUS, &data, sizeof(data));
}

InternalMessage MessageFactory::createNetworkInfoMessage(const String& ssid, const String& ip) {
    struct NetworkInfoData {
        char ssid[64];
        char ip[16];
    };

    NetworkInfoData data;
    strncpy(data.ssid, ssid.c_str(), sizeof(data.ssid) - 1);
    data.ssid[sizeof(data.ssid) - 1] = '\0';
    strncpy(data.ip, ip.c_str(), sizeof(data.ip) - 1);
    data.ip[sizeof(data.ip) - 1] = '\0';

    return InternalMessage(MessageProtocol::InternalMessageType::NETWORK_INFO, &data, sizeof(data));
}

InternalMessage MessageFactory::createSDStatusMessage(const String& status, bool mounted) {
    struct SDStatusData {
        char status[32];
        bool mounted;
    };

    SDStatusData data;
    strncpy(data.status, status.c_str(), sizeof(data.status) - 1);
    data.status[sizeof(data.status) - 1] = '\0';
    data.mounted = mounted;

    return InternalMessage(MessageProtocol::InternalMessageType::SD_STATUS, &data, sizeof(data));
}

InternalMessage MessageFactory::createAudioDeviceChangeMessage(const String& deviceName) {
    struct AudioDeviceChangeData {
        char deviceName[64];
    };

    AudioDeviceChangeData data;
    strncpy(data.deviceName, deviceName.c_str(), sizeof(data.deviceName) - 1);
    data.deviceName[sizeof(data.deviceName) - 1] = '\0';

    return InternalMessage(MessageProtocol::InternalMessageType::AUDIO_DEVICE_CHANGE, &data, sizeof(data));
}

InternalMessage MessageFactory::createCoreToCoreSyncMessage(uint8_t fromCore, uint8_t toCore) {
    struct CoreSyncData {
        uint8_t fromCore;
        uint8_t toCore;
    };

    CoreSyncData data;
    data.fromCore = fromCore;
    data.toCore = toCore;

    return InternalMessage(MessageProtocol::InternalMessageType::CORE_SYNC, &data, sizeof(data));
}

InternalMessage MessageFactory::createDebugUILogMessage(const String& logMessage) {
    struct DebugLogData {
        char logMessage[256];
    };

    DebugLogData data;
    strncpy(data.logMessage, logMessage.c_str(), sizeof(data.logMessage) - 1);
    data.logMessage[sizeof(data.logMessage) - 1] = '\0';

    return InternalMessage(MessageProtocol::InternalMessageType::DEBUG_LOG, &data, sizeof(data));
}

// =============================================================================
// MESSAGE PARSER IMPLEMENTATIONS
// =============================================================================

ParseResult<MessageProtocol::ExternalMessageType> MessageParser::parseExternalMessageType(const String& jsonPayload) {
    const char* TAG = "MessageParser";

    if (jsonPayload.isEmpty()) {
        return ParseResult<MessageProtocol::ExternalMessageType>::createError("Empty JSON payload");
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, jsonPayload);

    if (error) {
        String errorMsg = "JSON deserialization failed: " + String(error.c_str());
        ESP_LOGW(TAG, "%s", errorMsg.c_str());
        return ParseResult<MessageProtocol::ExternalMessageType>::createError(errorMsg);
    }

    if (!doc.containsKey(MessageProtocol::JsonFields::MESSAGE_TYPE)) {
        return ParseResult<MessageProtocol::ExternalMessageType>::createError("Missing messageType field");
    }

    int typeValue = doc[MessageProtocol::JsonFields::MESSAGE_TYPE].as<int>();
    auto messageType = static_cast<MessageProtocol::ExternalMessageType>(typeValue);

    if (!MessageProtocol::isValidExternalMessageType(messageType)) {
        return ParseResult<MessageProtocol::ExternalMessageType>::createError("Invalid messageType value");
    }

    return ParseResult<MessageProtocol::ExternalMessageType>::createSuccess(messageType);
}

ParseResult<ExternalMessage> MessageParser::parseExternalMessage(const String& jsonPayload) {
    const char* TAG = "MessageParser";
    using namespace MessageProtocol::JsonFields;

    if (jsonPayload.isEmpty()) {
        return ParseResult<ExternalMessage>::createError("Empty JSON payload");
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, jsonPayload);

    if (error) {
        String errorMsg = "JSON deserialization failed: " + String(error.c_str());
        ESP_LOGW(TAG, "%s", errorMsg.c_str());
        return ParseResult<ExternalMessage>::createError(errorMsg);
    }

    // Parse message type
    if (!doc.containsKey(MESSAGE_TYPE)) {
        return ParseResult<ExternalMessage>::createError("Missing messageType field");
    }

    int typeValue = doc[MESSAGE_TYPE].as<int>();
    auto messageType = static_cast<MessageProtocol::ExternalMessageType>(typeValue);

    if (!MessageProtocol::isValidExternalMessageType(messageType)) {
        return ParseResult<ExternalMessage>::createError("Invalid messageType value");
    }

    // Parse other fields
    String requestId = doc[REQUEST_ID].as<String>();
    String deviceId = doc[DEVICE_ID].as<String>();
    String originatingDeviceId = doc[ORIGINATING_DEVICE_ID].as<String>();
    unsigned long timestamp = doc[TIMESTAMP].as<unsigned long>();

    if (timestamp == 0) {
        timestamp = millis();
    }

    // Create message
    ExternalMessage message(messageType, requestId, deviceId);
    message.originatingDeviceId = originatingDeviceId;
    message.timestamp = timestamp;
    message.validated = true;

    // Copy parsed data
    message.parsedData.set(doc.as<JsonObject>());

    ESP_LOGD(TAG, "Successfully parsed external message: type=%d, deviceId=%s",
             static_cast<int>(messageType), deviceId.c_str());

    return ParseResult<ExternalMessage>::createSuccess(message);
}

bool MessageParser::shouldIgnoreMessage(const ExternalMessage& message, const String& myDeviceId) {
    // Ignore invalid messages
    if (message.messageType == MessageProtocol::ExternalMessageType::INVALID) {
        return true;
    }

    // Ignore self-originated messages
    if (message.deviceId == myDeviceId) {
        return true;
    }

    // Ignore messages from our own device ID in originatingDeviceId
    if (!message.originatingDeviceId.isEmpty() && message.originatingDeviceId == myDeviceId) {
        return true;
    }

    return false;
}

ParseResult<AudioStatusData> MessageParser::parseAudioStatusData(const ExternalMessage& message) {
    const char* TAG = "MessageParser";
    using namespace MessageProtocol::JsonFields;

    AudioStatusData data;

    if (message.messageType != MessageProtocol::ExternalMessageType::STATUS_UPDATE &&
        message.messageType != MessageProtocol::ExternalMessageType::STATUS_MESSAGE) {
        return ParseResult<AudioStatusData>("Invalid message type for audio status");
    }

    try {
        // Extract sessions
        if (message.parsedData.containsKey(SESSIONS)) {
            JsonArray sessionsArray = message.parsedData[SESSIONS].as<JsonArray>();
            for (JsonVariant sessionVariant : sessionsArray) {
                JsonObject sessionObj = sessionVariant.as<JsonObject>();

                SessionStatusData session;
                session.processId = sessionObj[PROCESS_ID].as<int>();
                session.processName = sessionObj[PROCESS_NAME].as<String>();
                session.displayName = sessionObj[DISPLAY_NAME].as<String>();
                session.volume = sessionObj[VOLUME].as<float>();
                session.isMuted = sessionObj[IS_MUTED].as<bool>();
                session.state = sessionObj[STATE].as<String>();

                data.sessions.push_back(session);
            }
        }

        // Extract default device
        if (message.parsedData.containsKey(DEFAULT_DEVICE)) {
            JsonObject defaultObj = message.parsedData[DEFAULT_DEVICE].as<JsonObject>();

            data.defaultDevice.friendlyName = defaultObj[FRIENDLY_NAME].as<String>();
            data.defaultDevice.volume = defaultObj[VOLUME].as<float>();
            data.defaultDevice.isMuted = defaultObj[IS_MUTED].as<bool>();
            data.defaultDevice.dataFlow = defaultObj[DATA_FLOW].as<String>();
            data.defaultDevice.deviceRole = defaultObj[DEVICE_ROLE].as<String>();
            data.hasDefaultDevice = true;
        }

        // Extract metadata
        data.timestamp = message.timestamp;
        data.reason = message.parsedData[REASON].as<String>();
        data.originatingDeviceId = message.parsedData[ORIGINATING_DEVICE_ID].as<String>();
        data.originatingRequestId = message.parsedData[ORIGINATING_REQUEST_ID].as<String>();
        data.activeSessionCount = message.parsedData[ACTIVE_SESSION_COUNT].as<int>();

    } catch (const std::exception& e) {
        String errorMsg = "Error parsing audio status data: " + String(e.what());
        ESP_LOGE(TAG, "%s", errorMsg.c_str());
        return ParseResult<AudioStatusData>(errorMsg);
    }

    return ParseResult<AudioStatusData>(data);
}

ParseResult<AssetResponseData> MessageParser::parseAssetResponseData(const ExternalMessage& message) {
    const char* TAG = "MessageParser";

    if (message.messageType != MessageProtocol::ExternalMessageType::ASSET_RESPONSE) {
        return ParseResult<AssetResponseData>("Invalid message type for asset response");
    }

    try {
        AssetResponseData data(message);
        return ParseResult<AssetResponseData>(data);
    } catch (const std::exception& e) {
        String errorMsg = "Error parsing asset response data: " + String(e.what());
        ESP_LOGE(TAG, "%s", errorMsg.c_str());
        return ParseResult<AssetResponseData>(errorMsg);
    }
}

// =============================================================================
// MESSAGE SERIALIZER IMPLEMENTATIONS
// =============================================================================

ParseResult<String> MessageSerializer::serializeExternalMessage(const ExternalMessage& message) {
    const char* TAG = "MessageSerializer";

    try {
        DynamicJsonDocument doc(2048);

        // Core fields
        doc[MessageProtocol::JsonFields::MESSAGE_TYPE] = static_cast<int>(message.messageType);
        doc[MessageProtocol::JsonFields::REQUEST_ID] = message.requestId;
        doc[MessageProtocol::JsonFields::DEVICE_ID] = message.deviceId;
        doc[MessageProtocol::JsonFields::TIMESTAMP] = message.timestamp;

        if (!message.originatingDeviceId.isEmpty()) {
            doc[MessageProtocol::JsonFields::ORIGINATING_DEVICE_ID] = message.originatingDeviceId;
        }

        // Add additional fields from parsedData
        if (!message.parsedData.isNull()) {
            for (JsonPair kv : message.parsedData.as<JsonObject>()) {
                const char* key = kv.key().c_str();

                // Skip core fields to avoid duplication
                if (strcmp(key, MessageProtocol::JsonFields::MESSAGE_TYPE) != 0 &&
                    strcmp(key, MessageProtocol::JsonFields::REQUEST_ID) != 0 &&
                    strcmp(key, MessageProtocol::JsonFields::DEVICE_ID) != 0 &&
                    strcmp(key, MessageProtocol::JsonFields::TIMESTAMP) != 0 &&
                    strcmp(key, MessageProtocol::JsonFields::ORIGINATING_DEVICE_ID) != 0) {
                    doc[key] = kv.value();
                }
            }
        }

        String jsonString;
        serializeJson(doc, jsonString);

        return ParseResult<String>(jsonString);

    } catch (const std::exception& e) {
        String errorMsg = "Error serializing external message: " + String(e.what());
        ESP_LOGE(TAG, "%s", errorMsg.c_str());
        return ParseResult<String>(errorMsg);
    }
}

ParseResult<String> MessageSerializer::serializeInternalMessage(const InternalMessage& message) {
    const char* TAG = "MessageSerializer";

    try {
        DynamicJsonDocument doc(512);

        doc["messageType"] = static_cast<int>(message.messageType);
        doc["timestamp"] = message.timestamp;
        doc["priority"] = message.priority;
        doc["dataSize"] = message.dataSize;
        doc["requiresResponse"] = message.requiresResponse;

        String jsonString;
        serializeJson(doc, jsonString);

        return ParseResult<String>(jsonString);

    } catch (const std::exception& e) {
        String errorMsg = "Error serializing internal message: " + String(e.what());
        ESP_LOGE(TAG, "%s", errorMsg.c_str());
        return ParseResult<String>(errorMsg);
    }
}

ParseResult<String> MessageSerializer::createStatusResponse(const AudioStatusData& data) {
    const char* TAG = "MessageSerializer";
    using namespace MessageProtocol::JsonFields;

    try {
        DynamicJsonDocument doc(4096);

        doc[MESSAGE_TYPE] = static_cast<int>(MessageProtocol::ExternalMessageType::STATUS_MESSAGE);
        doc[DEVICE_ID] = Config::getDeviceId();
        doc[TIMESTAMP] = data.timestamp;
        doc[ACTIVE_SESSION_COUNT] = data.activeSessionCount;

        if (!data.reason.isEmpty()) {
            doc[REASON] = data.reason;
        }

        if (!data.originatingDeviceId.isEmpty()) {
            doc[ORIGINATING_DEVICE_ID] = data.originatingDeviceId;
        }

        if (!data.originatingRequestId.isEmpty()) {
            doc[ORIGINATING_REQUEST_ID] = data.originatingRequestId;
        }

        // Serialize sessions
        JsonArray sessionsArray = doc.createNestedArray(SESSIONS);
        for (const auto& session : data.sessions) {
            JsonObject sessionObj = sessionsArray.createNestedObject();
            sessionObj[PROCESS_ID] = session.processId;
            sessionObj[PROCESS_NAME] = session.processName;
            sessionObj[DISPLAY_NAME] = session.displayName;
            sessionObj[VOLUME] = session.volume;
            sessionObj[IS_MUTED] = session.isMuted;
            sessionObj[STATE] = session.state;
        }

        // Serialize default device
        if (data.hasDefaultDevice) {
            JsonObject defaultObj = doc.createNestedObject(DEFAULT_DEVICE);
            defaultObj[FRIENDLY_NAME] = data.defaultDevice.friendlyName;
            defaultObj[VOLUME] = data.defaultDevice.volume;
            defaultObj[IS_MUTED] = data.defaultDevice.isMuted;
            defaultObj[DATA_FLOW] = data.defaultDevice.dataFlow;
            defaultObj[DEVICE_ROLE] = data.defaultDevice.deviceRole;
        }

        String jsonString;
        serializeJson(doc, jsonString);

        return ParseResult<String>(jsonString);

    } catch (const std::exception& e) {
        String errorMsg = "Error creating status response: " + String(e.what());
        ESP_LOGE(TAG, "%s", errorMsg.c_str());
        return ParseResult<String>(errorMsg);
    }
}

ParseResult<String> MessageSerializer::createAssetRequest(const String& processName, const String& deviceId) {
    const char* TAG = "MessageSerializer";

    try {
        DynamicJsonDocument doc(512);

        String devId = deviceId.isEmpty() ? Config::getDeviceId() : deviceId;

        doc[MessageProtocol::JsonFields::MESSAGE_TYPE] = static_cast<int>(MessageProtocol::ExternalMessageType::GET_ASSETS);
        doc[MessageProtocol::JsonFields::REQUEST_ID] = Config::generateRequestId();
        doc[MessageProtocol::JsonFields::DEVICE_ID] = devId;
        doc[MessageProtocol::JsonFields::TIMESTAMP] = millis();
        doc["processName"] = processName;

        String jsonString;
        serializeJson(doc, jsonString);

        return ParseResult<String>(jsonString);

    } catch (const std::exception& e) {
        String errorMsg = "Error creating asset request: " + String(e.what());
        ESP_LOGE(TAG, "%s", errorMsg.c_str());
        return ParseResult<String>(errorMsg);
    }
}

// =============================================================================
// MESSAGE CONVERTER IMPLEMENTATIONS
// =============================================================================

std::vector<InternalMessage> MessageConverter::externalToInternal(const ExternalMessage& external) {
    std::vector<InternalMessage> internalMessages;

    switch (external.messageType) {
        case MessageProtocol::ExternalMessageType::STATUS_UPDATE:
        case MessageProtocol::ExternalMessageType::STATUS_MESSAGE: {
            auto parseResult = MessageParser::parseAudioStatusData(external);
            if (parseResult.isValid()) {
                InternalMessage msg(MessageProtocol::InternalMessageType::AUDIO_STATE_UPDATE,
                                   &parseResult.getValue(), sizeof(AudioStatusData));
                internalMessages.push_back(msg);
            }
            break;
        }

        case MessageProtocol::ExternalMessageType::ASSET_RESPONSE: {
            auto parseResult = MessageParser::parseAssetResponseData(external);
            if (parseResult.isValid()) {
                InternalMessage msg(MessageProtocol::InternalMessageType::ASSET_RESPONSE,
                                   &parseResult.getValue(), sizeof(AssetResponseData));
                internalMessages.push_back(msg);
            }
            break;
        }

        default:
            // For other message types, create generic internal message
            break;
    }

    return internalMessages;
}

ExternalMessage MessageConverter::internalToExternal(const InternalMessage& internal) {
    ExternalMessage external;

    switch (internal.messageType) {
        case MessageProtocol::InternalMessageType::AUDIO_STATE_UPDATE: {
            external.messageType = MessageProtocol::ExternalMessageType::STATUS_MESSAGE;
            external.requestId = Config::generateRequestId();
            external.deviceId = Config::getDeviceId();
            external.timestamp = internal.timestamp;
            break;
        }

        default:
            external.messageType = MessageProtocol::ExternalMessageType::INVALID;
            break;
    }

    return external;
}

} // namespace Messaging
