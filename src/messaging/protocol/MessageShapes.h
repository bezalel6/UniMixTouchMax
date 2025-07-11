#pragma once

#include <Arduino.h>
#include <variant>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include <esp_log.h>
#include <StringAbstraction.h>
#include <MessageProtocol.h>
#include <float.h>
namespace Messaging {

// =============================================================================
// VARIANT TYPE SYSTEM FOR MESSAGE DATA
// =============================================================================

/**
 * Variant type for message field values
 * Supports all basic types that can come from JSON
 */
using MessageFieldValue = std::variant<
    std::monostate,  // null/empty
    bool,
    int,
    float,
    double,
    string,
    std::vector<string>,
    std::unordered_map<string, string>>;

/**
 * Message data container - replaces JsonDocument
 */
using MessageVariantMap = std::unordered_map<string, MessageFieldValue>;

// =============================================================================
// VALIDATION RESULT SYSTEM
// =============================================================================

struct ValidationResult {
    bool success = false;
    string error;

    static ValidationResult createSuccess() {
        return {true, STRING_EMPTY};
    }

    static ValidationResult createError(const string& err) {
        return {false, err};
    }

    bool isValid() const { return success; }
    const string& getError() const { return error; }
};

// =============================================================================
// FIELD VALIDATION UTILITIES
// =============================================================================

namespace FieldValidator {

/**
 * Validate string field with optional constraints
 */
inline ValidationResult validateString(const MessageFieldValue& value,
                                       const string& fieldName,
                                       bool required = false,
                                       size_t maxLength = 0) {
    if (std::holds_alternative<std::monostate>(value)) {
        if (required) {
            return ValidationResult::createError(fieldName + STRING_FROM_LITERAL(" is required"));
        }
        return ValidationResult::createSuccess();
    }

    if (!std::holds_alternative<string>(value)) {
        return ValidationResult::createError(fieldName + STRING_FROM_LITERAL(" must be a string"));
    }

    const auto& str = std::get<string>(value);
    if (maxLength > 0 && STRING_LENGTH(str) > maxLength) {
        return ValidationResult::createError(fieldName + STRING_FROM_LITERAL(" exceeds maximum length"));
    }

    return ValidationResult::createSuccess();
}

/**
 * Validate boolean field
 */
inline ValidationResult validateBool(const MessageFieldValue& value,
                                     const string& fieldName,
                                     bool required = false) {
    if (std::holds_alternative<std::monostate>(value)) {
        if (required) {
            return ValidationResult::createError(fieldName + STRING_FROM_LITERAL(" is required"));
        }
        return ValidationResult::createSuccess();
    }

    if (!std::holds_alternative<bool>(value)) {
        return ValidationResult::createError(fieldName + STRING_FROM_LITERAL(" must be a boolean"));
    }

    return ValidationResult::createSuccess();
}

/**
 * Validate integer field with optional range
 */
inline ValidationResult validateInt(const MessageFieldValue& value,
                                    const string& fieldName,
                                    bool required = false,
                                    int minValue = INT_MIN,
                                    int maxValue = INT_MAX) {
    if (std::holds_alternative<std::monostate>(value)) {
        if (required) {
            return ValidationResult::createError(fieldName + STRING_FROM_LITERAL(" is required"));
        }
        return ValidationResult::createSuccess();
    }

    if (!std::holds_alternative<int>(value)) {
        return ValidationResult::createError(fieldName + STRING_FROM_LITERAL(" must be an integer"));
    }

    const auto& intVal = std::get<int>(value);
    if (intVal < minValue || intVal > maxValue) {
        return ValidationResult::createError(fieldName + STRING_FROM_LITERAL(" out of range"));
    }

    return ValidationResult::createSuccess();
}

/**
 * Validate float field with optional range
 */
inline ValidationResult validateFloat(const MessageFieldValue& value,
                                      const string& fieldName,
                                      bool required = false,
                                      float minValue = -FLT_MAX,
                                      float maxValue = FLT_MAX) {
    if (std::holds_alternative<std::monostate>(value)) {
        if (required) {
            return ValidationResult::createError(fieldName + STRING_FROM_LITERAL(" is required"));
        }
        return ValidationResult::createSuccess();
    }

    if (!std::holds_alternative<float>(value)) {
        return ValidationResult::createError(fieldName + STRING_FROM_LITERAL(" must be a float"));
    }

    const auto& floatVal = std::get<float>(value);
    if (floatVal < minValue || floatVal > maxValue) {
        return ValidationResult::createError(fieldName + STRING_FROM_LITERAL(" out of range"));
    }

    return ValidationResult::createSuccess();
}

}  // namespace FieldValidator

// =============================================================================
// MACRO SYSTEM FOR MESSAGE SHAPE DEFINITIONS
// =============================================================================

/**
 * Begin message shape definition
 */
#define MESSAGE_SHAPE_BEGIN(ShapeName, MessageType)                                                                             \
    struct ShapeName {                                                                                                          \
        static constexpr MessageProtocol::ExternalMessageType MESSAGE_TYPE = MessageProtocol::ExternalMessageType::MessageType; \
        static constexpr const char* SHAPE_NAME = #ShapeName;                                                                   \
                                                                                                                                \
        /* Validation function declaration */                                                                                   \
        static ValidationResult validate(const MessageVariantMap& data);                                                        \
                                                                                                                                \
        /* Creation function declaration */                                                                                     \
        static ShapeName create(const MessageVariantMap& data);                                                                 \
                                                                                                                                \
        /* Serialization function declarations */                                                                               \
        MessageVariantMap serialize() const;                                                                                    \
        string toJsonString() const;                                                                                            \
                                                                                                                                \
        /* Direct creation from JSON */                                                                                         \
        static ParseResult<ShapeName> fromJsonString(const string& json);                                                       \
                                                                                                                                \
        /* Default constructor */                                                                                               \
        ShapeName() = default;                                                                                                  \
                                                                                                                                \
        /* Fields start here */

/**
 * Define a required string field
 */
#define REQUIRED_STRING_FIELD(fieldName, maxLen)             \
    string fieldName;                                        \
    static constexpr size_t fieldName##_MAX_LENGTH = maxLen; \
    static constexpr bool fieldName##_REQUIRED = true;

/**
 * Define an optional string field
 */
#define OPTIONAL_STRING_FIELD(fieldName, maxLen)             \
    string fieldName;                                        \
    static constexpr size_t fieldName##_MAX_LENGTH = maxLen; \
    static constexpr bool fieldName##_REQUIRED = false;

/**
 * Define a required boolean field
 */
#define REQUIRED_BOOL_FIELD(fieldName) \
    bool fieldName = false;            \
    static constexpr bool fieldName##_REQUIRED = true;

/**
 * Define an optional boolean field
 */
#define OPTIONAL_BOOL_FIELD(fieldName) \
    bool fieldName = false;            \
    static constexpr bool fieldName##_REQUIRED = false;

/**
 * Define a required integer field
 */
#define REQUIRED_INT_FIELD(fieldName, minVal, maxVal)    \
    int fieldName = 0;                                   \
    static constexpr int fieldName##_MIN_VALUE = minVal; \
    static constexpr int fieldName##_MAX_VALUE = maxVal; \
    static constexpr bool fieldName##_REQUIRED = true;

/**
 * Define an optional integer field
 */
#define OPTIONAL_INT_FIELD(fieldName, minVal, maxVal)    \
    int fieldName = 0;                                   \
    static constexpr int fieldName##_MIN_VALUE = minVal; \
    static constexpr int fieldName##_MAX_VALUE = maxVal; \
    static constexpr bool fieldName##_REQUIRED = false;

/**
 * Define a required float field
 */
#define REQUIRED_FLOAT_FIELD(fieldName, minVal, maxVal)    \
    float fieldName = 0.0f;                                \
    static constexpr float fieldName##_MIN_VALUE = minVal; \
    static constexpr float fieldName##_MAX_VALUE = maxVal; \
    static constexpr bool fieldName##_REQUIRED = true;

/**
 * Define an optional float field
 */
#define OPTIONAL_FLOAT_FIELD(fieldName, minVal, maxVal)    \
    float fieldName = 0.0f;                                \
    static constexpr float fieldName##_MIN_VALUE = minVal; \
    static constexpr float fieldName##_MAX_VALUE = maxVal; \
    static constexpr bool fieldName##_REQUIRED = false;

/**
 * Begin validation implementation
 */
#define MESSAGE_SHAPE_VALIDATION_BEGIN(ShapeName)                         \
    ValidationResult ShapeName::validate(const MessageVariantMap& data) { \
        ValidationResult result;

/**
 * Validate string field
 */
#define VALIDATE_STRING_FIELD(fieldName)                                                                                                    \
    if (auto it = data.find(STRING_FROM_LITERAL(#fieldName)); it != data.end()) {                                                           \
        result = FieldValidator::validateString(it->second, STRING_FROM_LITERAL(#fieldName), fieldName##_REQUIRED, fieldName##_MAX_LENGTH); \
        if (!result.isValid()) return result;                                                                                               \
    } else if (fieldName##_REQUIRED) {                                                                                                      \
        return ValidationResult::createError(STRING_FROM_LITERAL(#fieldName " is required"));                                               \
    }

/**
 * Validate boolean field
 */
#define VALIDATE_BOOL_FIELD(fieldName)                                                                            \
    if (auto it = data.find(STRING_FROM_LITERAL(#fieldName)); it != data.end()) {                                 \
        result = FieldValidator::validateBool(it->second, STRING_FROM_LITERAL(#fieldName), fieldName##_REQUIRED); \
        if (!result.isValid()) return result;                                                                     \
    } else if (fieldName##_REQUIRED) {                                                                            \
        return ValidationResult::createError(STRING_FROM_LITERAL(#fieldName " is required"));                     \
    }

/**
 * Validate integer field
 */
#define VALIDATE_INT_FIELD(fieldName)                                                                                                                          \
    if (auto it = data.find(STRING_FROM_LITERAL(#fieldName)); it != data.end()) {                                                                              \
        result = FieldValidator::validateInt(it->second, STRING_FROM_LITERAL(#fieldName), fieldName##_REQUIRED, fieldName##_MIN_VALUE, fieldName##_MAX_VALUE); \
        if (!result.isValid()) return result;                                                                                                                  \
    } else if (fieldName##_REQUIRED) {                                                                                                                         \
        return ValidationResult::createError(STRING_FROM_LITERAL(#fieldName " is required"));                                                                  \
    }

/**
 * Validate float field
 */
#define VALIDATE_FLOAT_FIELD(fieldName)                                                                                                                          \
    if (auto it = data.find(STRING_FROM_LITERAL(#fieldName)); it != data.end()) {                                                                                \
        result = FieldValidator::validateFloat(it->second, STRING_FROM_LITERAL(#fieldName), fieldName##_REQUIRED, fieldName##_MIN_VALUE, fieldName##_MAX_VALUE); \
        if (!result.isValid()) return result;                                                                                                                    \
    } else if (fieldName##_REQUIRED) {                                                                                                                           \
        return ValidationResult::createError(STRING_FROM_LITERAL(#fieldName " is required"));                                                                    \
    }

/**
 * End validation implementation
 */
#define MESSAGE_SHAPE_VALIDATION_END()        \
    return ValidationResult::createSuccess(); \
    }

/**
 * Begin creation implementation
 */
#define MESSAGE_SHAPE_CREATION_BEGIN(ShapeName)                  \
    ShapeName ShapeName::create(const MessageVariantMap& data) { \
        ShapeName shape;

/**
 * Assign string field
 */
#define ASSIGN_STRING_FIELD(fieldName)                                                                                          \
    if (auto it = data.find(STRING_FROM_LITERAL(#fieldName)); it != data.end() && std::holds_alternative<string>(it->second)) { \
        shape.fieldName = std::get<string>(it->second);                                                                         \
    }

/**
 * Assign boolean field
 */
#define ASSIGN_BOOL_FIELD(fieldName)                                                                                          \
    if (auto it = data.find(STRING_FROM_LITERAL(#fieldName)); it != data.end() && std::holds_alternative<bool>(it->second)) { \
        shape.fieldName = std::get<bool>(it->second);                                                                         \
    }

/**
 * Assign integer field
 */
#define ASSIGN_INT_FIELD(fieldName)                                                                                          \
    if (auto it = data.find(STRING_FROM_LITERAL(#fieldName)); it != data.end() && std::holds_alternative<int>(it->second)) { \
        shape.fieldName = std::get<int>(it->second);                                                                         \
    }

/**
 * Assign float field
 */
#define ASSIGN_FLOAT_FIELD(fieldName)                                                                                          \
    if (auto it = data.find(STRING_FROM_LITERAL(#fieldName)); it != data.end() && std::holds_alternative<float>(it->second)) { \
        shape.fieldName = std::get<float>(it->second);                                                                         \
    }

/**
 * End creation implementation
 */
#define MESSAGE_SHAPE_CREATION_END() \
    return shape;                    \
    }

/**
 * Begin serialization implementation
 */
#define MESSAGE_SHAPE_SERIALIZATION_BEGIN(ShapeName) \
    MessageVariantMap ShapeName::serialize() const { \
        MessageVariantMap data;

/**
 * Serialize string field
 */
#define SERIALIZE_STRING_FIELD(fieldName)                  \
    if (!STRING_IS_EMPTY(fieldName)) {                     \
        data[STRING_FROM_LITERAL(#fieldName)] = fieldName; \
    }

/**
 * Serialize boolean field
 */
#define SERIALIZE_BOOL_FIELD(fieldName) \
    data[STRING_FROM_LITERAL(#fieldName)] = fieldName;

/**
 * Serialize integer field
 */
#define SERIALIZE_INT_FIELD(fieldName) \
    data[STRING_FROM_LITERAL(#fieldName)] = fieldName;

/**
 * Serialize float field
 */
#define SERIALIZE_FLOAT_FIELD(fieldName) \
    data[STRING_FROM_LITERAL(#fieldName)] = fieldName;

/**
 * End serialization implementation
 */
#define MESSAGE_SHAPE_SERIALIZATION_END() \
    return data;                          \
    }

/**
 * Begin JSON string generation implementation
 */
#define MESSAGE_SHAPE_JSON_BEGIN(ShapeName)                                 \
    string ShapeName::toJsonString() const {                                \
        string json = STRING_FROM_LITERAL("{");                             \
        json += STRING_FROM_LITERAL("\"messageType\":\"");                  \
        json += MessageProtocol::externalMessageTypeToString(MESSAGE_TYPE); \
        json += STRING_FROM_LITERAL("\",");

/**
 * Generate JSON for string field
 */
#define JSON_STRING_FIELD(fieldName)                          \
    if (!STRING_IS_EMPTY(fieldName)) {                        \
        json += STRING_FROM_LITERAL("\"" #fieldName "\":\""); \
        json += fieldName;                                    \
        json += STRING_FROM_LITERAL("\",");                   \
    }

/**
 * Generate JSON for boolean field
 */
#define JSON_BOOL_FIELD(fieldName)                                                  \
    json += STRING_FROM_LITERAL("\"" #fieldName "\":");                             \
    json += fieldName ? STRING_FROM_LITERAL("true") : STRING_FROM_LITERAL("false"); \
    json += STRING_FROM_LITERAL(",");

/**
 * Generate JSON for integer field
 */
#define JSON_INT_FIELD(fieldName)                           \
    json += STRING_FROM_LITERAL("\"" #fieldName "\":");     \
    json += STRING_FROM_STD_STR(std::to_string(fieldName)); \
    json += STRING_FROM_LITERAL(",");

/**
 * Generate JSON for float field
 */
#define JSON_FLOAT_FIELD(fieldName)                                \
    json += STRING_FROM_LITERAL("\"" #fieldName "\":");            \
    char floatBuffer[32];                                          \
    snprintf(floatBuffer, sizeof(floatBuffer), "%.2f", fieldName); \
    json += floatBuffer;                                           \
    json += STRING_FROM_LITERAL(",");

/**
 * End JSON string generation implementation
 */
#define MESSAGE_SHAPE_JSON_END()                                                     \
    /* Remove trailing comma */                                                      \
    if (STRING_LENGTH(json) > 0 && json[STRING_LENGTH(json) - 1] == ',') {           \
        json = string(json, 0, STRING_LENGTH(json) - 1);                             \
    }                                                                                \
    json += STRING_FROM_LITERAL("}");                                                \
    return json;                                                                     \
    }                                                                                \
                                                                                     \
    ParseResult<ShapeName> ShapeName::fromJsonString(const string& jsonStr) {        \
        auto parseResult = JsonToVariantConverter::parseJsonString(jsonStr);         \
        if (!parseResult.isValid()) {                                                \
            return ParseResult<ShapeName>::createError(parseResult.getError());      \
        }                                                                            \
                                                                                     \
        auto validation = validate(parseResult.getData());                           \
        if (!validation.isValid()) {                                                 \
            return ParseResult<ShapeName>::createError(validation.getError());       \
        }                                                                            \
                                                                                     \
        return ParseResult<ShapeName>::createSuccess(create(parseResult.getData())); \
    }

/**
 * End message shape definition
 */
#define MESSAGE_SHAPE_END() \
    }                       \
    ;

// =============================================================================
// MESSAGE SHAPE REGISTRY
// =============================================================================

/**
 * Base class for message shape handlers
 */
class MessageShapeHandler {
   public:
    virtual ~MessageShapeHandler() = default;
    virtual ValidationResult validate(const MessageVariantMap& data) = 0;
    virtual MessageVariantMap createFromData(const MessageVariantMap& data) = 0;
    virtual MessageProtocol::ExternalMessageType getMessageType() const = 0;
    virtual const char* getShapeName() const = 0;
};

/**
 * Template implementation for specific message shapes
 */
template <typename ShapeType>
class TypedMessageShapeHandler : public MessageShapeHandler {
   public:
    ValidationResult validate(const MessageVariantMap& data) override {
        return ShapeType::validate(data);
    }

    MessageVariantMap createFromData(const MessageVariantMap& data) override {
        auto shape = ShapeType::create(data);
        return shape.serialize();
    }

    MessageProtocol::ExternalMessageType getMessageType() const override {
        return ShapeType::MESSAGE_TYPE;
    }

    const char* getShapeName() const override {
        return ShapeType::SHAPE_NAME;
    }
};

/**
 * Registry for message shape handlers
 */
class MessageShapeRegistry {
   private:
    std::unordered_map<MessageProtocol::ExternalMessageType, std::unique_ptr<MessageShapeHandler>> handlers;

   public:
    template <typename ShapeType>
    void registerShape() {
        handlers[ShapeType::MESSAGE_TYPE] = std::make_unique<TypedMessageShapeHandler<ShapeType>>();
    }

    MessageShapeHandler* getHandler(MessageProtocol::ExternalMessageType type) {
        auto it = handlers.find(type);
        return (it != handlers.end()) ? it->second.get() : nullptr;
    }

    ValidationResult validateMessage(MessageProtocol::ExternalMessageType type, const MessageVariantMap& data) {
        auto handler = getHandler(type);
        if (!handler) {
            return ValidationResult::createError(STRING_FROM_LITERAL("Unknown message type"));
        }
        return handler->validate(data);
    }

    // Singleton access
    static MessageShapeRegistry& instance() {
        static MessageShapeRegistry registry;
        return registry;
    }
};

// =============================================================================
// CONVENIENCE MACROS FOR COMPLETE MESSAGE SHAPE DEFINITIONS
// =============================================================================

/**
 * Complete message shape definition with validation, creation, and serialization
 */
#define DEFINE_MESSAGE_SHAPE(ShapeName, MessageType, ...) \
    MESSAGE_SHAPE_BEGIN(ShapeName, MessageType)           \
    __VA_ARGS__                                           \
    MESSAGE_SHAPE_END()

/**
 * Implement all required methods for a message shape with JSON generation
 */
#define IMPLEMENT_MESSAGE_SHAPE(ShapeName, ValidationFields, CreationFields, SerializationFields, JsonFields) \
    MESSAGE_SHAPE_VALIDATION_BEGIN(ShapeName)                                                                 \
    ValidationFields                                                                                          \
    MESSAGE_SHAPE_VALIDATION_END()                                                                            \
                                                                                                              \
        MESSAGE_SHAPE_CREATION_BEGIN(ShapeName)                                                               \
            CreationFields                                                                                    \
            MESSAGE_SHAPE_CREATION_END()                                                                      \
                                                                                                              \
                MESSAGE_SHAPE_SERIALIZATION_BEGIN(ShapeName)                                                  \
                    SerializationFields                                                                       \
                    MESSAGE_SHAPE_SERIALIZATION_END()                                                         \
                                                                                                              \
                        MESSAGE_SHAPE_JSON_BEGIN(ShapeName)                                                   \
                            JsonFields                                                                        \
                            MESSAGE_SHAPE_JSON_END();

}  // namespace Messaging
