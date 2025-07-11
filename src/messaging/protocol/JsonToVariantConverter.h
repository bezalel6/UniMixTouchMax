#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <esp_log.h>
#include <StringAbstraction.h>
#include "MessageShapes.h"

namespace Messaging {

// =============================================================================
// JSON TO VARIANT CONVERSION UTILITIES
// =============================================================================

/**
 * Result of JSON parsing operation
 */
struct JsonParseResult {
    bool success = false;
    string error;
    MessageVariantMap data;

    static JsonParseResult createSuccess(const MessageVariantMap& variantData) {
        return {true, STRING_EMPTY, variantData};
    }

    static JsonParseResult createError(const string& err) {
        return {false, err, {}};
    }

    bool isValid() const { return success; }
    const string& getError() const { return error; }
    const MessageVariantMap& getData() const { return data; }
};

/**
 * JSON to Variant converter class
 */
class JsonToVariantConverter {
   private:
    static constexpr const char* TAG = "JsonToVariantConverter";

    /**
     * Convert JsonVariant to MessageFieldValue
     */
    static MessageFieldValue convertJsonValue(JsonVariantConst value) {
        if (value.isNull()) {
            return std::monostate{};
        }

        if (value.is<bool>()) {
            return value.as<bool>();
        }

        if (value.is<int>()) {
            return value.as<int>();
        }

        if (value.is<float>()) {
            return value.as<float>();
        }

        if (value.is<double>()) {
            return value.as<double>();
        }

        if (value.is<const char*>()) {
            return STRING_FROM_CSTR(value.as<const char*>());
        }

        if (value.is<JsonArrayConst>()) {
            auto array = value.as<JsonArrayConst>();
            std::vector<string> stringArray;

            for (JsonVariantConst element : array) {
                if (element.is<const char*>()) {
                    stringArray.push_back(STRING_FROM_CSTR(element.as<const char*>()));
                } else {
                    // Convert non-string array elements to string
                    stringArray.push_back(STRING_FROM_CSTR(element.as<string>().c_str()));
                }
            }

            return stringArray;
        }

        if (value.is<JsonObjectConst>()) {
            auto object = value.as<JsonObjectConst>();
            std::unordered_map<string, string> stringMap;

            for (JsonPairConst pair : object) {
                string key = STRING_FROM_CSTR(pair.key().c_str());
                string val;

                if (pair.value().is<const char*>()) {
                    val = STRING_FROM_CSTR(pair.value().as<const char*>());
                } else {
                    // Convert non-string object values to string
                    val = STRING_FROM_CSTR(pair.value().as<string>().c_str());
                }

                stringMap[key] = val;
            }

            return stringMap;
        }

        // Fallback: convert to string
        return STRING_FROM_CSTR(value.as<string>().c_str());
    }

    /**
     * Convert JsonObject to MessageVariantMap
     */
    static MessageVariantMap convertJsonObject(JsonObjectConst object) {
        MessageVariantMap variantMap;

        for (JsonPairConst pair : object) {
            string key = STRING_FROM_CSTR(pair.key().c_str());
            variantMap[key] = convertJsonValue(pair.value());
        }

        return variantMap;
    }

   public:
    /**
     * Convert JSON string to MessageVariantMap
     */
    static JsonParseResult parseJsonString(const string& jsonString) {
        if (STRING_IS_EMPTY(jsonString)) {
            return JsonParseResult::createError(STRING_FROM_LITERAL("Empty JSON string"));
        }

        // Use JsonDocument for parsing
        JsonDocument doc;

        DeserializationError error = deserializeJson(doc, STRING_C_STR(jsonString));

        if (error) {
            string errorMsg = STRING_FROM_LITERAL("JSON parsing failed: ");
            errorMsg += STRING_FROM_CSTR(error.c_str());
            ESP_LOGE(TAG, "%s", STRING_C_STR(errorMsg));
            return JsonParseResult::createError(errorMsg);
        }

        if (!doc.is<JsonObject>()) {
            return JsonParseResult::createError(STRING_FROM_LITERAL("JSON root is not an object"));
        }

        try {
            MessageVariantMap variantMap = convertJsonObject(doc.as<JsonObject>());
            ESP_LOGD(TAG, "Successfully converted JSON to variant map with %zu fields", variantMap.size());
            return JsonParseResult::createSuccess(variantMap);
        } catch (const std::exception& e) {
            string errorMsg = STRING_FROM_LITERAL("Conversion failed: ");
            errorMsg += STRING_FROM_CSTR(e.what());
            ESP_LOGE(TAG, "%s", STRING_C_STR(errorMsg));
            return JsonParseResult::createError(errorMsg);
        }
    }

    /**
     * Convert MessageVariantMap back to JSON string
     */
    static string variantMapToJsonString(const MessageVariantMap& variantMap) {
        JsonDocument doc;

        for (const auto& [key, value] : variantMap) {
            std::visit([&](const auto& val) {
                using T = std::decay_t<decltype(val)>;

                if constexpr (std::is_same_v<T, std::monostate>) {
                    doc[STRING_C_STR(key)] = nullptr;
                } else if constexpr (std::is_same_v<T, bool>) {
                    doc[STRING_C_STR(key)] = val;
                } else if constexpr (std::is_same_v<T, int>) {
                    doc[STRING_C_STR(key)] = val;
                } else if constexpr (std::is_same_v<T, float>) {
                    doc[STRING_C_STR(key)] = val;
                } else if constexpr (std::is_same_v<T, double>) {
                    doc[STRING_C_STR(key)] = val;
                } else if constexpr (std::is_same_v<T, string>) {
                    doc[STRING_C_STR(key)] = STRING_C_STR(val);
                } else if constexpr (std::is_same_v<T, std::vector<string>>) {
                    JsonArray array = doc[STRING_C_STR(key)].to<JsonArray>();
                    for (const auto& item : val) {
                        array.add(STRING_C_STR(item));
                    }
                } else if constexpr (std::is_same_v<T, std::unordered_map<string, string>>) {
                    JsonObject object = doc[STRING_C_STR(key)].to<JsonObject>();
                    for (const auto& [objKey, objVal] : val) {
                        object[STRING_C_STR(objKey)] = STRING_C_STR(objVal);
                    }
                }
            },
                       value);
        }

        string jsonString;
        serializeJson(doc, jsonString);
        return jsonString;
    }

    /**
     * Extract message type from variant map
     */
    static MessageProtocol::ExternalMessageType extractMessageType(const MessageVariantMap& variantMap) {
        static const string MESSAGE_TYPE_KEY = STRING_FROM_LITERAL("messageType");

        auto it = variantMap.find(MESSAGE_TYPE_KEY);
        if (it == variantMap.end()) {
            ESP_LOGW(TAG, "No messageType field found in variant map");
            return MessageProtocol::ExternalMessageType::INVALID;
        }

        if (!std::holds_alternative<string>(it->second)) {
            ESP_LOGW(TAG, "messageType field is not a string");
            return MessageProtocol::ExternalMessageType::INVALID;
        }

        const string& messageTypeStr = std::get<string>(it->second);

        // Convert string to enum (you'll need to implement this based on your MessageProtocol)
        // This is a placeholder implementation
        if (messageTypeStr == STRING_FROM_LITERAL("STATUS_MESSAGE")) {
            return MessageProtocol::ExternalMessageType::STATUS_MESSAGE;
        } else if (messageTypeStr == STRING_FROM_LITERAL("ASSET_RESPONSE")) {
            return MessageProtocol::ExternalMessageType::ASSET_RESPONSE;
        } else if (messageTypeStr == STRING_FROM_LITERAL("GET_STATUS")) {
            return MessageProtocol::ExternalMessageType::GET_STATUS;
        } else if (messageTypeStr == STRING_FROM_LITERAL("GET_ASSETS")) {
            return MessageProtocol::ExternalMessageType::GET_ASSETS;
        }

        ESP_LOGW(TAG, "Unknown message type: %s", STRING_C_STR(messageTypeStr));
        return MessageProtocol::ExternalMessageType::INVALID;
    }

    /**
     * Validate common message fields
     */
    static ValidationResult validateCommonFields(const MessageVariantMap& variantMap) {
        // Check for required common fields
        static const string REQUIRED_FIELDS[] = {
            STRING_FROM_LITERAL("messageType"),
            STRING_FROM_LITERAL("timestamp"),
            STRING_FROM_LITERAL("deviceId")};

        for (const auto& field : REQUIRED_FIELDS) {
            if (variantMap.find(field) == variantMap.end()) {
                return ValidationResult::createError(field + STRING_FROM_LITERAL(" is required"));
            }
        }

        return ValidationResult::createSuccess();
    }

    /**
     * Debug utility to print variant map contents
     */
    static void debugPrintVariantMap(const MessageVariantMap& variantMap) {
        ESP_LOGD(TAG, "Variant map contents (%zu fields):", variantMap.size());

        for (const auto& [key, value] : variantMap) {
            std::visit([&](const auto& val) {
                using T = std::decay_t<decltype(val)>;

                if constexpr (std::is_same_v<T, std::monostate>) {
                    ESP_LOGD(TAG, "  %s: null", STRING_C_STR(key));
                } else if constexpr (std::is_same_v<T, bool>) {
                    ESP_LOGD(TAG, "  %s: %s", STRING_C_STR(key), val ? "true" : "false");
                } else if constexpr (std::is_same_v<T, int>) {
                    ESP_LOGD(TAG, "  %s: %d", STRING_C_STR(key), val);
                } else if constexpr (std::is_same_v<T, float>) {
                    ESP_LOGD(TAG, "  %s: %.2f", STRING_C_STR(key), val);
                } else if constexpr (std::is_same_v<T, double>) {
                    ESP_LOGD(TAG, "  %s: %.2f", STRING_C_STR(key), val);
                } else if constexpr (std::is_same_v<T, string>) {
                    ESP_LOGD(TAG, "  %s: \"%s\"", STRING_C_STR(key), STRING_C_STR(val));
                } else if constexpr (std::is_same_v<T, std::vector<string>>) {
                    ESP_LOGD(TAG, "  %s: [array with %zu items]", STRING_C_STR(key), val.size());
                } else if constexpr (std::is_same_v<T, std::unordered_map<string, string>>) {
                    ESP_LOGD(TAG, "  %s: {object with %zu fields}", STRING_C_STR(key), val.size());
                }
            },
                       value);
        }
    }
};

}  // namespace Messaging
