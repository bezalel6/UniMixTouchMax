# Messaging System - Quick Start Guide

## 🚀 Overview

The messaging system provides type-safe, validated message handling with compile-time verification and zero `JsonDocument` dependencies.

### Key Features
- ✅ **Type-safe access**: `message.getTypedData<AudioStatusResponseShape>()`
- ✅ **Automatic validation**: Detailed error messages for invalid data
- ✅ **Performance**: Parse once, access many times (no JSON re-parsing)
- ✅ **Compile-time safety**: Impossible to access wrong message fields
- ✅ **Zero overhead**: No JsonDocument, minimal memory usage

## 📝 How to Use

### 1. Define Message Shapes (Already Done)

```cpp
// Example: Audio Status Response
DEFINE_MESSAGE_SHAPE(AudioStatusResponseShape, STATUS_RESPONSE,
    REQUIRED_STRING_FIELD(deviceId, 64)
    REQUIRED_STRING_FIELD(requestId, 64)
    REQUIRED_BOOL_FIELD(hasDefaultDevice)
    OPTIONAL_FLOAT_FIELD(defaultDeviceVolume, 0.0f, 100.0f)
    REQUIRED_INT_FIELD(activeSessionCount, 0, 1000)
    REQUIRED_INT_FIELD(timestamp, 0, INT_MAX)
)
```

### 2. Parse JSON Messages

```cpp
// Parse JSON with automatic validation
auto parseResult = ExternalMessage::fromJsonString(jsonPayload);
if (parseResult.isValid()) {
    ExternalMessage message = parseResult.getValue();
    // Message is automatically validated!
}
```

### 3. Access Message Data (Type-Safe)

```cpp
// Get strongly-typed data
auto audioDataResult = message.getTypedData<AudioStatusResponseShape>();
if (audioDataResult.isValid()) {
    AudioStatusResponseShape audioData = audioDataResult.getValue();
    
    // All fields are now strongly typed and validated
    string deviceId = audioData.deviceId;           // No getString() needed
    bool hasDevice = audioData.hasDefaultDevice;    // No getBool() needed
    float volume = audioData.defaultDeviceVolume;   // No getFloat() needed
    int sessionCount = audioData.activeSessionCount; // No getInt() needed
}
```

### 4. Handle Different Message Types

```cpp
void handleMessage(const string& jsonPayload) {
    auto parseResult = ExternalMessage::fromJsonString(jsonPayload);
    if (!parseResult.isValid()) {
        ESP_LOGE("MessageHandler", "Parse failed: %s", STRING_C_STR(parseResult.getError()));
        return;
    }
    
    ExternalMessage message = parseResult.getValue();
    
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
            
        // ... other message types
    }
}
```



## 📊 Available Message Shapes

### AudioStatusResponseShape
```cpp
auto audioData = message.getTypedData<AudioStatusResponseShape>();
// Fields: deviceId, requestId, reason, hasDefaultDevice, defaultDeviceName, 
//         defaultDeviceVolume, defaultDeviceIsMuted, activeSessionCount, timestamp
```

### AssetResponseShape
```cpp
auto assetData = message.getTypedData<AssetResponseShape>();
// Fields: deviceId, requestId, processName, success, errorMessage, 
//         assetDataBase64, width, height, format, timestamp
```

### StatusRequestShape
```cpp
auto requestData = message.getTypedData<StatusRequestShape>();
// Fields: deviceId, requestId, timestamp
```

### AssetRequestShape
```cpp
auto requestData = message.getTypedData<AssetRequestShape>();
// Fields: deviceId, requestId, processName, timestamp
```

## 🎯 Usage Guidelines

### Essential Steps
1. Initialize the system: `initializeMessagingSystem()`
2. Parse messages: `ExternalMessage::fromJsonString()`
3. Access data: `getTypedData<MessageShapeType>()`

### Best Practices
- Always check `parseResult.isValid()` before using data
- Use appropriate message shape types for type safety
- Handle all message types in switch statements
- Log errors for debugging

## 🔧 Adding New Message Types

### 1. Define the Shape
```cpp
DEFINE_MESSAGE_SHAPE(MyNewMessageShape, MY_NEW_MESSAGE_TYPE,
    REQUIRED_STRING_FIELD(customField, 128)
    OPTIONAL_INT_FIELD(customNumber, 0, 9999)
)
```

### 2. Implement the Methods
```cpp
IMPLEMENT_MESSAGE_SHAPE(MyNewMessageShape,
    VALIDATE_STRING_FIELD(customField)
    VALIDATE_INT_FIELD(customNumber),
    
    ASSIGN_STRING_FIELD(customField)
    ASSIGN_INT_FIELD(customNumber),
    
    SERIALIZE_STRING_FIELD(customField)
    SERIALIZE_INT_FIELD(customNumber)
)
```

### 3. Register the Shape
```cpp
// In MessageShapeDefinitions.h
registry.registerShape<MyNewMessageShape>();
```

### 4. Use It
```cpp
auto myData = message.getTypedData<MyNewMessageShape>();
if (myData.isValid()) {
    // Use myData.getValue().customField, etc.
}
```

## 📚 Key Files

- `MessageShapes.h` - Macro system and variant types
- `JsonToVariantConverter.h` - JSON conversion utilities
- `MessageShapeDefinitions.h` - Concrete message shapes
- `RefactoredExternalMessage.h` - Type-safe message class
- `RefactoringExampleUsage.cpp` - Usage examples

## 🎉 Benefits Achieved

### Developer Experience
- **Type Safety**: Compile-time verification prevents runtime errors
- **Intellisense**: Better IDE support with strongly-typed fields
- **Maintainability**: Single source of truth for message definitions
- **Debugging**: Clear error messages for validation failures

### Performance
- **Memory**: No JsonDocument storage overhead
- **CPU**: Parse once, access many times (no JSON re-parsing)
- **Validation**: Centralized validation with detailed error reporting

### Code Quality
- **Less Code**: Macro-generated boilerplate
- **Consistency**: Uniform message handling
- **Extensibility**: Easy to add new message types

The messaging system provides complete type safety with zero overhead!