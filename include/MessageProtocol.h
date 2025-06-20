#ifndef MESSAGE_PROTOCOL_H
#define MESSAGE_PROTOCOL_H
#include "Arduino.h"
namespace Messaging::Protocol {

// Serial Protocol - Simplified (no prefixes needed)

// Message Types (simplified protocol)
inline constexpr const char *MESSAGE_STATUS_UPDATE = "StatusUpdate";
inline constexpr const char *MESSAGE_GET_STATUS = "GetStatus";

// Reason constants for status messages
inline constexpr const char *REASON_SERVICE_STARTUP = "ServiceStartup";
inline constexpr const char *REASON_PERIODIC_UPDATE = "PeriodicUpdate";
inline constexpr const char *REASON_SESSION_CHANGE = "SessionChange";
inline constexpr const char *REASON_STATUS_REQUEST = "StatusRequest";
inline constexpr const char *REASON_UPDATE_RESPONSE = "UpdateResponse";

// Configuration Constants
inline constexpr int MAX_TOPIC_LENGTH = 128; // Maximum topic name length
inline constexpr int MAX_PAYLOAD_LENGTH =
    2048;                               // Maximum payload size to match server
inline constexpr int MAX_HANDLERS = 10; // Maximum number of message handlers
inline constexpr int MAX_IDENTIFIER_LENGTH =
    64; // Maximum handler identifier length

// Serial Protocol Constants
inline constexpr char SERIAL_TERMINATOR = '\n'; // Message terminator character
inline constexpr int SERIAL_TIMEOUT_MS = 1000;  // Serial communication timeout

// Serial Interface Configuration
inline constexpr int SERIAL_BAUD_RATE =
    115200; // Serial baud rate to match server


inline constexpr const char *MY_DEVICE_ID = "ESP32S3-CONTROL-CENTER";

// Request ID generation helper
inline String generateRequestId() {
  return String("esp32_") + String(millis());
}

} // namespace Messaging::Protocol

#endif // MESSAGE_PROTOCOL_H