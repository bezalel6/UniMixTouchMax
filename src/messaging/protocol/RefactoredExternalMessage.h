#pragma once

#include <Arduino.h>
#include <variant>
#include <memory>
#include <esp_log.h>
#include <StringAbstraction.h>
#include <MessageProtocol.h>
#include "MessageShapes.h"
#include "JsonToVariantConverter.h"
#include "MessageShapeDefinitions.h"

namespace Messaging {

// =============================================================================
// REFACTORED EXTERNAL MESSAGE CLASS
// =============================================================================

/**
 * REFACTORED EXTERNAL MESSAGE - No more JsonDocument dependency!
 * 
 * Features:
 * - Type-safe variant-based storage
 * - Automatic validation and parsing
 * - Generic access to typed message data
 * - Compile-time message shape verification
 * - Zero-copy message handling where possible
 */
class RefactoredExternalMessage {
private:
    static constexpr const char* TAG = "RefactoredExternalMessage";
    
    MessageProtocol::ExternalMessageType messageType_;
    string requestId_;
    string deviceId_;
    string originatingDeviceId_;
    unsigned long timestamp_;
    bool validated_;
    
    // Variant-based storage - replaces JsonDocument
    MessageVariantMap variantData_;
    
    // Cached parsed message shape (only allocated when needed)
    mutable std::unique_ptr<MessageShapeHandler> cachedHandler_;
    
public:
    // =============================================================================
    // CONSTRUCTORS AND BASIC PROPERTIES
    // =============================================================================
    
    RefactoredExternalMessage() 
        : messageType_(MessageProtocol::ExternalMessageType::INVALID)
        , timestamp_(millis())
        , validated_(false) {}
    
    RefactoredExternalMessage(MessageProtocol::ExternalMessageType type, 
                             const string& reqId = STRING_EMPTY, 
                             const string& devId = STRING_EMPTY)
        : messageType_(type)
        , requestId_(reqId)
        , deviceId_(devId)
        , timestamp_(millis())
        , validated_(false) {}
    
    // Move constructor for efficient message handling
    RefactoredExternalMessage(RefactoredExternalMessage&& other) noexcept 
        : messageType_(other.messageType_)
        , requestId_(std::move(other.requestId_))
        , deviceId_(std::move(other.deviceId_))
        , originatingDeviceId_(std::move(other.originatingDeviceId_))
        , timestamp_(other.timestamp_)
        , validated_(other.validated_)
        , variantData_(std::move(other.variantData_))
        , cachedHandler_(std::move(other.cachedHandler_)) {}
    
    // Move assignment operator
    RefactoredExternalMessage& operator=(RefactoredExternalMessage&& other) noexcept {
        if (this != &other) {
            messageType_ = other.messageType_;
            requestId_ = std::move(other.requestId_);
            deviceId_ = std::move(other.deviceId_);
            originatingDeviceId_ = std::move(other.originatingDeviceId_);
            timestamp_ = other.timestamp_;
            validated_ = other.validated_;
            variantData_ = std::move(other.variantData_);
            cachedHandler_ = std::move(other.cachedHandler_);
        }
        return *this;
    }
    
    // Basic property accessors
    MessageProtocol::ExternalMessageType getMessageType() const { return messageType_; }
    const string& getRequestId() const { return requestId_; }
    const string& getDeviceId() const { return deviceId_; }
    const string& getOriginatingDeviceId() const { return originatingDeviceId_; }
    unsigned long getTimestamp() const { return timestamp_; }
    bool isValidated() const { return validated_; }
    
    // =============================================================================
    // STATIC FACTORY METHODS - Create from JSON
    // =============================================================================
    
    /**
     * Create RefactoredExternalMessage from JSON string
     * This replaces the old JsonDocument-based parsing
     */
    static ParseResult<RefactoredExternalMessage> fromJsonString(const string& jsonString) {
        // Step 1: Parse JSON to variant map
        auto parseResult = JsonToVariantConverter::parseJsonString(jsonString);
        if (!parseResult.isValid()) {
            ESP_LOGE(TAG, "JSON parsing failed: %s", STRING_C_STR(parseResult.getError()));
            return ParseResult<RefactoredExternalMessage>::createError(parseResult.getError());
        }
        
        // Step 2: Extract message type
        auto messageType = JsonToVariantConverter::extractMessageType(parseResult.getData());
        if (messageType == MessageProtocol::ExternalMessageType::INVALID) {
            return ParseResult<RefactoredExternalMessage>::createError(
                STRING_FROM_LITERAL("Invalid or missing message type"));
        }
        
        // Step 3: Validate common fields
        auto commonValidation = JsonToVariantConverter::validateCommonFields(parseResult.getData());
        if (!commonValidation.isValid()) {
            ESP_LOGE(TAG, "Common field validation failed: %s", STRING_C_STR(commonValidation.getError()));
            return ParseResult<RefactoredExternalMessage>::createError(commonValidation.getError());
        }
        
        // Step 4: Type-specific validation using message shape registry
        auto& registry = MessageShapeRegistry::instance();
        auto validation = registry.validateMessage(messageType, parseResult.getData());
        if (!validation.isValid()) {
            ESP_LOGE(TAG, "Message shape validation failed: %s", STRING_C_STR(validation.getError()));
            return ParseResult<RefactoredExternalMessage>::createError(validation.getError());
        }
        
        // Step 5: Create the message
        RefactoredExternalMessage message;
        message.messageType_ = messageType;
        message.variantData_ = parseResult.getData();
        message.validated_ = true;
        message.timestamp_ = millis();
        
        // Extract common fields from variant data
        message.extractCommonFields();
        
        ESP_LOGD(TAG, "Successfully created RefactoredExternalMessage of type %d", 
                 static_cast<int>(messageType));
        
        return ParseResult<RefactoredExternalMessage>::createSuccess(std::move(message));
    }
    
    // =============================================================================
    // GENERIC TYPE-SAFE ACCESS METHODS
    // =============================================================================
    
    /**
     * Get typed message data - This is the key feature!
     * Usage: auto audioData = message.getTypedData<AudioStatusResponseShape>();
     */
    template<typename ShapeType>
    ParseResult<ShapeType> getTypedData() const {
        // Compile-time verification that the requested type matches message type
        if constexpr (std::is_same_v<ShapeType, AudioStatusResponseShape>) {
            if (messageType_ != MessageProtocol::ExternalMessageType::STATUS_RESPONSE) {
                return ParseResult<ShapeType>::createError(
                    STRING_FROM_LITERAL("Message type mismatch: expected STATUS_RESPONSE"));
            }
        } else if constexpr (std::is_same_v<ShapeType, AssetResponseShape>) {
            if (messageType_ != MessageProtocol::ExternalMessageType::ASSET_RESPONSE) {
                return ParseResult<ShapeType>::createError(
                    STRING_FROM_LITERAL("Message type mismatch: expected ASSET_RESPONSE"));
            }
        } else if constexpr (std::is_same_v<ShapeType, StatusRequestShape>) {
            if (messageType_ != MessageProtocol::ExternalMessageType::STATUS_REQUEST) {
                return ParseResult<ShapeType>::createError(
                    STRING_FROM_LITERAL("Message type mismatch: expected STATUS_REQUEST"));
            }
        } else if constexpr (std::is_same_v<ShapeType, AssetRequestShape>) {
            if (messageType_ != MessageProtocol::ExternalMessageType::ASSET_REQUEST) {
                return ParseResult<ShapeType>::createError(
                    STRING_FROM_LITERAL("Message type mismatch: expected ASSET_REQUEST"));
            }
        }
        
        // Create the typed data from variant map
        try {
            ShapeType typedData = ShapeType::create(variantData_);
            return ParseResult<ShapeType>::createSuccess(typedData);
        } catch (const std::exception& e) {
            string error = STRING_FROM_LITERAL("Failed to create typed data: ");
            error += STRING_FROM_CSTR(e.what());
            return ParseResult<ShapeType>::createError(error);
        }
    }
    
    /**
     * Check if message can be accessed as a specific type
     */
    template<typename ShapeType>
    bool canAccessAs() const {
        auto result = getTypedData<ShapeType>();
        return result.isValid();
    }
    
    // =============================================================================
    // BACKWARD COMPATIBILITY METHODS
    // =============================================================================
    
    /**
     * Legacy string accessor - for backward compatibility
     * NOTE: This should be phased out in favor of getTypedData<>()
     */
    string getString(const string& field, const string& defaultValue = STRING_EMPTY) const {
        auto it = variantData_.find(field);
        if (it == variantData_.end()) {
            return defaultValue;
        }
        
        if (std::holds_alternative<string>(it->second)) {
            return std::get<string>(it->second);
        }
        
        return defaultValue;
    }
    
    /**
     * Legacy boolean accessor - for backward compatibility
     */
    bool getBool(const string& field, bool defaultValue = false) const {
        auto it = variantData_.find(field);
        if (it == variantData_.end()) {
            return defaultValue;
        }
        
        if (std::holds_alternative<bool>(it->second)) {
            return std::get<bool>(it->second);
        }
        
        return defaultValue;
    }
    
    /**
     * Legacy integer accessor - for backward compatibility
     */
    int getInt(const string& field, int defaultValue = 0) const {
        auto it = variantData_.find(field);
        if (it == variantData_.end()) {
            return defaultValue;
        }
        
        if (std::holds_alternative<int>(it->second)) {
            return std::get<int>(it->second);
        }
        
        return defaultValue;
    }
    
    /**
     * Legacy float accessor - for backward compatibility
     */
    float getFloat(const string& field, float defaultValue = 0.0f) const {
        auto it = variantData_.find(field);
        if (it == variantData_.end()) {
            return defaultValue;
        }
        
        if (std::holds_alternative<float>(it->second)) {
            return std::get<float>(it->second);
        }
        
        return defaultValue;
    }
    
    /**
     * Check if field exists
     */
    bool hasField(const string& field) const {
        return variantData_.find(field) != variantData_.end();
    }
    
    // =============================================================================
    // VALIDATION AND SECURITY METHODS
    // =============================================================================
    
    bool isSelfOriginated() const {
        return deviceId_ == originatingDeviceId_;
    }
    
    bool requiresResponse() const {
        return MessageProtocol::requiresResponse(messageType_);
    }
    
    MessageProtocol::ExternalMessageCategory getCategory() const {
        return MessageProtocol::getExternalMessageCategory(messageType_);
    }
    
    MessageProtocol::MessagePriority getPriority() const {
        return MessageProtocol::getExternalMessagePriority(messageType_);
    }
    
    // =============================================================================
    // SERIALIZATION METHODS
    // =============================================================================
    
    /**
     * Convert back to JSON string for transmission
     */
    string toJsonString() const {
        return JsonToVariantConverter::variantMapToJsonString(variantData_);
    }
    
    /**
     * Get the raw variant data (for debugging or advanced use)
     */
    const MessageVariantMap& getVariantData() const {
        return variantData_;
    }
    
    // =============================================================================
    // DEBUG AND UTILITY METHODS
    // =============================================================================
    
    /**
     * Debug print message contents
     */
    void debugPrint() const {
        ESP_LOGD(TAG, "RefactoredExternalMessage:");
        ESP_LOGD(TAG, "  Type: %d", static_cast<int>(messageType_));
        ESP_LOGD(TAG, "  Request ID: %s", STRING_C_STR(requestId_));
        ESP_LOGD(TAG, "  Device ID: %s", STRING_C_STR(deviceId_));
        ESP_LOGD(TAG, "  Validated: %s", validated_ ? "true" : "false");
        ESP_LOGD(TAG, "  Variant data:");
        
        JsonToVariantConverter::debugPrintVariantMap(variantData_);
    }
    
private:
    /**
     * Extract common fields from variant data
     */
    void extractCommonFields() {
        auto extractString = [&](const string& key, string& target) {
            auto it = variantData_.find(key);
            if (it != variantData_.end() && std::holds_alternative<string>(it->second)) {
                target = std::get<string>(it->second);
            }
        };
        
        auto extractInt = [&](const string& key, unsigned long& target) {
            auto it = variantData_.find(key);
            if (it != variantData_.end() && std::holds_alternative<int>(it->second)) {
                target = static_cast<unsigned long>(std::get<int>(it->second));
            }
        };
        
        extractString(STRING_FROM_LITERAL("requestId"), requestId_);
        extractString(STRING_FROM_LITERAL("deviceId"), deviceId_);
        extractString(STRING_FROM_LITERAL("originatingDeviceId"), originatingDeviceId_);
        extractInt(STRING_FROM_LITERAL("timestamp"), timestamp_);
    }
};

// =============================================================================
// TYPE ALIASES FOR CONVENIENCE
// =============================================================================

using RefactoredExtMsg = RefactoredExternalMessage;

} // namespace Messaging