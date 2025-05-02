#include <Arduino.h>
#include <Ieee802154NetworkShared.h>

void setup() {
  Ieee802154NetworkShared::MessageV1 message;
  Ieee802154NetworkShared::DiscoveryRequestV1 discovery_request;
  Ieee802154NetworkShared::DiscoveryResponseV1 discovery_response;
  Ieee802154NetworkShared::PendingTimestampResponseV1 pending_timestamp_response;
  Ieee802154NetworkShared::PendingPayloadResponseV1 pending_payload_response;
  Ieee802154NetworkShared::PendingFirmwareWifiCredentialsResponseV1 pending_firmware_wifi_credentials_response;
  Ieee802154NetworkShared::PendingFirmwareChecksumResponseV1 pending_firmware_checksum_response;
  Ieee802154NetworkShared::PendingFirmwareUrlResponseV1 pending_firmware_url_response;
}

void loop() {}
