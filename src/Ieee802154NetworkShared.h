#pragma once

#include <cstdint>

// Handle all structures like protobuf, e.g. cannot remove fields and new fields should be added last.
namespace Ieee802154NetworkShared {

constexpr uint8_t MESSAGE_ID_MESSAGE = 0x03;

constexpr uint8_t MESSAGE_ID_DISCOVERY_REQUEST_V1 = 0x20;
constexpr uint8_t MESSAGE_ID_DISCOVERY_RESPONSE_V1 = 0x21;

constexpr uint8_t MESSAGE_ID_PENDING_TIMESTAMP_RESPONSE_V1 = 0x30;
constexpr uint8_t MESSAGE_ID_PENDING_PAYLOAD_RESPONSE_V1 = 0x31;

// Firmware update when using WiFi.
constexpr uint8_t MESSAGE_ID_PENDING_FIRMWARE_WIFI_CREDENTIALS_RESPONSE_V1 = 0x40;
constexpr uint8_t MESSAGE_ID_PENDING_FIRMWARE_CHECKSUM_RESPONSE_V1 = 0x41;
constexpr uint8_t MESSAGE_ID_PENDING_FIRMWARE_URL_RESPONSE_V1 = 0x42;
// Firmware update when using 802.15.4.
constexpr uint8_t MESSAGE_ID_PENDING_FIRMWARE_BEGIN_RESPONSE_V1 = 0x50;
constexpr uint8_t MESSAGE_ID_PENDING_FIRMWARE_DATA_RESPONSE_V1 = 0x51;
constexpr uint8_t MESSAGE_ID_PENDING_FIRMWARE_END_RESPONSE_V1 = 0x52;

/**
 * Regular payload message send by node to host.
 */
struct __attribute__((packed)) MessageV1 {
  uint8_t id = MESSAGE_ID_MESSAGE;
  uint32_t firmware_version;
  uint8_t payload[]; // We know the size as we know the size from the decrypted outer message. Maxium size is 74 bytes.
};

/**
 * Sent by nodes to perform host discovery.
 */
struct __attribute__((packed)) DiscoveryRequestV1 {
  uint8_t id = MESSAGE_ID_DISCOVERY_REQUEST_V1;
};

/**
 * Sent by host to confirm host discovery.
 */
struct __attribute__((packed)) DiscoveryResponseV1 {
  uint8_t id = MESSAGE_ID_DISCOVERY_RESPONSE_V1;
  uint8_t channel; // 802.15.4 channel to use when sending messages. Must be a valid 802.15.4 channel between 11 and 26.
};

/**
 * Sent by host to node for time syncronization.
 */
struct __attribute__((packed)) PendingTimestampResponseV1 {
  uint8_t id = MESSAGE_ID_PENDING_TIMESTAMP_RESPONSE_V1;
  uint64_t timestamp; // unix timestamp in seconds, UTC.
};

/**
 * Sent by host when node has been informed about pending messages.
 * This message allows for the host to provide additional application specific payload.
 */
struct __attribute__((packed)) PendingPayloadResponseV1 {
  uint8_t id = MESSAGE_ID_PENDING_PAYLOAD_RESPONSE_V1;
  uint8_t payload[]; // We know the size as we know the size from the decrypted outer message. Max size 78 bytes.
};

/**
 * Sent by host when node has been informed about pending messages.
 * This is part 1/3 of the firmware update process when uisng WiFi. We cannot fit everyting in one message.
 * This can tecnically be omitted if the node already knows about the wifi credentials.
 */
struct __attribute__((packed)) PendingFirmwareWifiCredentialsResponseV1 {
  uint8_t id = MESSAGE_ID_PENDING_FIRMWARE_WIFI_CREDENTIALS_RESPONSE_V1;
  char wifi_ssid[32];     // WiFi SSID that node should connect to.
  char wifi_password[32]; // WiFi password that the node should connect to.
  uint32_t identifier;    // Set by host. All firmware update will have the same ID, so node should verify that they are
                          // identical across pending firmware responses.
};

/**
 * Sent by host when node has been informed about pending messages.
 * This is part 2/3 of the firmware update process when uisng WiFi. We cannot fit everyting in one message.
 */
struct __attribute__((packed)) PendingFirmwareChecksumResponseV1 {
  uint8_t id = MESSAGE_ID_PENDING_FIRMWARE_CHECKSUM_RESPONSE_V1;
  char md5[32];        // MD5 hash of firmware. Does not include trailing \0
  uint32_t identifier; // Set by host. All firmware update will have the same ID, so node should verify that they are
  // identical across pending firmware responses.
};

/**
 * Sent by host when node has been informed about pending messages.
 * This is part 3/3 of the firmware update process when uisng WiFi. We cannot fit everyting in one message.
 * After this message, firmware update will proceed, given that wifi credentials have been proided earlier.
 */
struct __attribute__((packed)) PendingFirmwareUrlResponseV1 {
  uint8_t id = MESSAGE_ID_PENDING_FIRMWARE_URL_RESPONSE_V1;
  char url[74];        // url where to find firmware binary. Note the max file path.
  uint32_t identifier; // Set by host. All firmware update will have the same ID, so node should verify that they are
  // identical across pending firmware responses.
};

/**
 * Sent by host when node has been informed about pending messages.
 * This is for the firmware update process when using 802.15.4. Part 1 to indicate a begin/start.
 */
struct __attribute__((packed)) PendingFirmwareBeginResponseV1 {
  uint8_t id = MESSAGE_ID_PENDING_FIRMWARE_BEGIN_RESPONSE_V1;
  uint32_t identifier; // Set by host. All firmware update will have the same ID, so node should verify that they are
  // identical across pending firmware responses.
  uint64_t size; // Size of firmware, in bytes.
};

/**
 * Sent by host when node has been informed about pending messages.
 * This is for the firmware update process when using 802.15.4. Part 2, repeating. Host will continue to send acked
 * messages until all data has been sent.
 */
struct __attribute__((packed)) PendingFirmwareDataResponseV1 {
  uint8_t id = MESSAGE_ID_PENDING_FIRMWARE_DATA_RESPONSE_V1;
  uint32_t identifier; // Set by host. All firmware update will have the same ID, so node should verify that they are
                       // identical across pending firmware responses.
  uint8_t payload[];   // We know the size as we know the size from the decrypted outer message. Max size 73 bytes.
};

/**
 * Sent by host when node has been informed about pending messages.
 * Host must send checksum first.
 * This is for the firmware update process when using 802.15.4. Part 3 to indicate done/end/complete.
 */
struct __attribute__((packed)) PendingFirmwareEndResponseV1 {
  uint8_t id = MESSAGE_ID_PENDING_FIRMWARE_END_RESPONSE_V1;
  uint32_t identifier; // Set by host. All firmware update will have the same ID, so node should verify that they are
                       // identical across pending firmware responses.
  char md5[32];        // MD5 hash of firmware. Does not include trailing \0
};

}; // namespace Ieee802154NetworkShared
