#include "MessageData.h"
#include <esp_log.h>
#include <ArduinoJson.h>

namespace Messaging {

// =============================================================================
// SAFE JSON PARSING UTILITIES
// =============================================================================

/**
 * Safely extract a value from a JSON object with fallback
 * Handles missing fields, null values, and type mismatches
 */
template <typename T>
T safeGetJsonValue(const JsonObject &obj, const char *field, const T &fallback) {
    JsonVariant variant = obj[field];
    if (variant.isNull() || !variant.is<T>()) {
        return fallback;
    }
    return variant.as<T>();
}

/**
 * Overload for JsonDocument (treats as JsonObject)
 */
template <typename T>
T safeGetJsonValue(const JsonDocument &doc, const char *field, const T &fallback) {
    JsonVariantConst variant = doc[field];
    if (variant.isNull() || !variant.is<T>()) {
        return fallback;
    }
    return variant.as<T>();
}

/**
 * Specialized version for String type to handle null and empty cases
 */
template <>
String safeGetJsonValue<String>(const JsonObject &obj, const char *field, const String &fallback) {
    JsonVariant variant = obj[field];
    if (variant.isNull()) {
        return fallback;
    }

    // Handle both string and numeric types that can be converted to string
    if (variant.is<const char *>() || variant.is<String>()) {
        return variant.as<String>();
    } else if (variant.is<int>() || variant.is<long>() || variant.is<double>()) {
        return variant.as<String>();
    }

    return fallback;
}

/**
 * String specialization for JsonDocument
 */
template <>
String safeGetJsonValue<String>(const JsonDocument &doc, const char *field, const String &fallback) {
    JsonVariantConst variant = doc[field];
    if (variant.isNull()) {
        return fallback;
    }

    // Handle both string and numeric types that can be converted to string
    if (variant.is<const char *>() || variant.is<String>()) {
        return variant.as<String>();
    } else if (variant.is<int>() || variant.is<long>() || variant.is<double>()) {
        return variant.as<String>();
    }

    return fallback;
}

/**
 * Convenience macro for safe JSON field extraction
 */
#define SAFE_GET_JSON(obj, field, type, fallback) \
    safeGetJsonValue<type>(obj, MessageProtocol::JsonFields::field, fallback)

// =============================================================================
// EXTERNAL MESSAGE METHODS (These are now implemented in ExternalMessage.h)
// =============================================================================
// Removed duplicate implementations - they are already defined in ExternalMessage.h

// =============================================================================
// INTERNAL MESSAGE CONSTRUCTORS
// =============================================================================

InternalMessage::InternalMessage() {
    messageType = MessageProtocol::InternalMessageType::INVALID;
    timestamp = millis();
    priority = static_cast<uint8_t>(
        MessageProtocol::getInternalMessagePriority(messageType));
}

InternalMessage::InternalMessage(MessageProtocol::InternalMessageType type,
                                 const void *payload, size_t size)
    : messageType(type), dataSize(size) {
    timestamp = millis();
    priority =
        static_cast<uint8_t>(MessageProtocol::getInternalMessagePriority(type));

    if (payload && size > 0) {
        data.reset(new uint8_t[size]);
        memcpy(data.get(), payload, size);
    }
}

// =============================================================================
// ASSET RESPONSE DATA CONSTRUCTOR
// =============================================================================

AssetResponseData::AssetResponseData(const ExternalMessage &external) {
    requestId = external.getRequestId();
    deviceId = external.getDeviceId();
    timestamp = external.getTimestamp();

    // Extract asset-specific fields from variant data
    const auto &variantData = external.getVariantData();

    auto processNameIt = variantData.find(STRING_FROM_LITERAL("processName"));
    if (processNameIt != variantData.end() && std::holds_alternative<string>(processNameIt->second)) {
        processName = std::get<string>(processNameIt->second);
    }

    auto successIt = variantData.find(STRING_FROM_LITERAL("success"));
    if (successIt != variantData.end() && std::holds_alternative<bool>(successIt->second)) {
        success = std::get<bool>(successIt->second);
    }

    auto errorMessageIt = variantData.find(STRING_FROM_LITERAL("errorMessage"));
    if (errorMessageIt != variantData.end() && std::holds_alternative<string>(errorMessageIt->second)) {
        errorMessage = std::get<string>(errorMessageIt->second);
    }

    auto assetDataIt = variantData.find(STRING_FROM_LITERAL("assetData"));
    if (assetDataIt != variantData.end() && std::holds_alternative<string>(assetDataIt->second)) {
        assetDataBase64 = std::get<string>(assetDataIt->second);
    }

    // Extract width, height, format from metadata if present
    auto metadataIt = variantData.find(STRING_FROM_LITERAL("metadata"));
    if (metadataIt != variantData.end() && std::holds_alternative<std::unordered_map<string, string>>(metadataIt->second)) {
        const auto &metadata = std::get<std::unordered_map<string, string>>(metadataIt->second);

        auto widthIt = metadata.find(STRING_FROM_LITERAL("width"));
        if (widthIt != metadata.end()) {
            width = std::stoi(STRING_C_STR(widthIt->second));
        }

        auto heightIt = metadata.find(STRING_FROM_LITERAL("height"));
        if (heightIt != metadata.end()) {
            height = std::stoi(STRING_C_STR(heightIt->second));
        }

        auto formatIt = metadata.find(STRING_FROM_LITERAL("format"));
        if (formatIt != metadata.end()) {
            format = formatIt->second;
        }
    }
}
// =============================================================================
// MESSAGE FACTORY IMPLEMENTATIONS - EXTERNAL MESSAGES ONLY
// =============================================================================
// Internal message factories are now generated by macros in MessageData.h

// MessageFactory methods are now implemented as static methods in ExternalMessage.h
// These duplicate implementations are removed to avoid redefinition errors

// =============================================================================
// MESSAGE PARSER IMPLEMENTATIONS
// =============================================================================
// These implementations are now in MessageData.h as inline functions
// Removed to avoid redefinition errors

// =============================================================================
// MESSAGE SERIALIZER IMPLEMENTATIONS
// =============================================================================
// These implementations are now in MessageData.h as inline functions
// Removed to avoid redefinition errors

// =============================================================================
// MESSAGE CONVERTER IMPLEMENTATIONS
// =============================================================================

// =============================================================================
// MESSAGE CONVERTER IMPLEMENTATIONS
// =============================================================================
// These implementations are now in MessageData.h as inline functions
// Removed to avoid redefinition errors

}  // namespace Messaging
