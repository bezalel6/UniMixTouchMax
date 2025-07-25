#pragma once

#include <Arduino.h>
#include <MessageProtocol.h>

namespace Messaging {
namespace Config {

// =============================================================================
// DEVICE CONFIGURATION
// =============================================================================

extern const char* DEVICE_ID;
extern const char* DEVICE_TYPE;

// =============================================================================
// NEW ENUM-BASED MESSAGE TYPE CONFIGURATION
// =============================================================================

// External message types (cross-transport boundaries)
extern const MessageProtocol::ExternalMessageType EXT_MSG_GET_STATUS;
extern const MessageProtocol::ExternalMessageType EXT_MSG_STATUS_UPDATE;
extern const MessageProtocol::ExternalMessageType EXT_MSG_STATUS_MESSAGE;
extern const MessageProtocol::ExternalMessageType EXT_MSG_GET_ASSETS;
extern const MessageProtocol::ExternalMessageType EXT_MSG_ASSET_RESPONSE;
extern const MessageProtocol::ExternalMessageType EXT_MSG_SESSION_UPDATE;

// Internal message types (ESP32 internal communication)
extern const MessageProtocol::InternalMessageType INT_MSG_WIFI_STATUS;
extern const MessageProtocol::InternalMessageType INT_MSG_NETWORK_INFO;
extern const MessageProtocol::InternalMessageType INT_MSG_UI_UPDATE;
extern const MessageProtocol::InternalMessageType INT_MSG_AUDIO_STATE_UPDATE;
extern const MessageProtocol::InternalMessageType INT_MSG_AUDIO_UI_REFRESH;
extern const MessageProtocol::InternalMessageType INT_MSG_SD_STATUS;
extern const MessageProtocol::InternalMessageType INT_MSG_ASSET_RESPONSE;

// Legacy string-based message types have been removed.
// Use the new enum-based constants (EXT_MSG_* and INT_MSG_*) instead.

// =============================================================================
// REASON CODES
// =============================================================================

extern const char* REASON_UPDATE_RESPONSE;
extern const char* REASON_STATUS_REQUEST;

// =============================================================================
// TIMING CONFIGURATION
// =============================================================================

extern const unsigned long ACTIVITY_TIMEOUT_MS;
extern const unsigned long MESSAGE_LOG_TRUNCATE_LENGTH;

// =============================================================================
// TRANSPORT NAMES
// =============================================================================

// Network transports not currently available
extern const char* TRANSPORT_NAME_SERIAL;

String generateRequestId();
String getDeviceId();
}  // namespace Config
}  // namespace Messaging
