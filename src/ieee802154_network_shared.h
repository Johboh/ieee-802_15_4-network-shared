#pragma once

#include <cstdint>

#define MESSAGE_ID_HEADER 0x03

#define MESSAGE_ID_DISCOVERY_REQUEST_V1 0x20
#define MESSAGE_ID_DISCOVERY_RESPONSE_V1 0x21

#define MESSAGE_ID_PENDING_TIMESTAMP_RESPONSE_V1 0x30
#define MESSAGE_ID_PENDING_PAYLOAD_RESPONSE_V1 0x31

#define MESSAGE_ID_PENDING_FIRMWARE_WIFI_CREDENTIALS_RESPONSE_V1 0x40
#define MESSAGE_ID_PENDING_FIRMWARE_CHECKSUM_RESPONSE_V1 0x41
#define MESSAGE_ID_PENDING_FIRMWARE_URL_RESPONSE_V1 0x42

// Handle all structures like protobuf, e.g. cannot remove fields and new fields should be added last.
namespace Ieee802154NetworkShared {

/**
 * Regular payload message send by node to host.
 */
struct __attribute__((packed)) MessageV1 {
  uint8_t id = MESSAGE_ID_HEADER;
  uint16_t attempt = 0;
  uint8_t payload[]; // We know the size as we know the size from the decrypted outer message.
};

/**
 * Sent by nodes to perform host discovery.
 */
struct DiscoveryRequestV1 {
  uint8_t id = MESSAGE_ID_DISCOVERY_REQUEST_V1;
};

/**
 * Sent by host to confirm host discovery.
 */
struct DiscoveryResponseV1 {
  uint8_t id = MESSAGE_ID_DISCOVERY_RESPONSE_V1;
  uint8_t channel; // 802.15.4 channel to use when sending messages. Must be a valid 802.15.4 channel between 11 and 26.
};

/**
 * Sent by host to node for time syncronization.
 */
struct PendingTimestampResponseV1 {
  uint8_t id = MESSAGE_ID_PENDING_TIMESTAMP_RESPONSE_V1;
  uint64_t timestamp; // unix timestamp in seconds, UTC.
};

/**
 * Sent by host when node has been informed about pending messages.
 * This message allows for the host to provide additional application specific payload.
 */
struct PendingPayloadResponseV1 {
  uint8_t id = MESSAGE_ID_PENDING_PAYLOAD_RESPONSE_V1;
  uint8_t payload[]; // We know the size as we know the size from the decrypted outer message. Max size 79 bytes.
};

/**
 * Sent by host when node has been informed about pending messages.
 * This is part 1/3 of the firmware update process. We cannot fit everyting in one message.
 * This can tecnically be omitted if the node already knows about the wifi credentials.
 */
struct PendingFirmwareWifiCredentialsResponseV1 {
  uint8_t id = MESSAGE_ID_PENDING_FIRMWARE_WIFI_CREDENTIALS_RESPONSE_V1;
  char wifi_ssid[32];     // WiFi SSID that node should connect to.
  char wifi_password[32]; // WiFi password that the node should connect to.
  uint32_t identifier;    // Set by host. All firmware update will have the same ID, so node should verify that they are
                          // identical across pending firmware responses.
};

/**
 * Sent by host when node has been informed about pending messages.
 * This is part 2/3 of the firmware update process. We cannot fit everyting in one message.
 */
struct PendingFirmwareChecksumResponseV1 {
  uint8_t id = MESSAGE_ID_PENDING_FIRMWARE_CHECKSUM_RESPONSE_V1;
  char md5[32];        // MD5 hash of firmware. Does not include trailing \0
  uint32_t identifier; // Set by host. All firmware update will have the same ID, so node should verify that they are
  // identical across pending firmware responses.
};

/**
 * Sent by host when node has been informed about pending messages.
 * This is part 3/3 of the firmware update process. We cannot fit everyting in one message.
 * After this message, firmware update will proceed, given that wifi credentials have been proided earlier.
 */
struct PendingFirmwareResponseV1 {
  uint8_t id = MESSAGE_ID_PENDING_FIRMWARE_URL_RESPONSE_V1;
  char url[75];        // url where to find firmware binary. Note the max file path.
  uint32_t identifier; // Set by host. All firmware update will have the same ID, so node should verify that they are
  // identical across pending firmware responses.
};

}; // namespace Ieee802154NetworkShared
