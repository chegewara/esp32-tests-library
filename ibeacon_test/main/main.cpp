/*
 * ESP32 iBeacon
 *
 * WIP: still trying to understand what the various ibeacon fields mean
 *
 * Author: Anthony Elder
 * License: Apache License v2
 */
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
const BLEUUID MY_UUID = BLEUUID("26410a28-2a4c-4d57-ae2f-67a4d96b1f83");

extern "C" {
	void app_main(void);
}

//static esp_ble_adv_params_t ble_adv_params;


void app_main(void)
{

  BLEDevice::init("MyESP32");
  BLEServer *pServer = BLEDevice::createServer();

  BLEBeacon myBeacon;
    myBeacon.setManufacturerId(0x4c00);
    myBeacon.setMajor(5);
    myBeacon.setMinor(88);
    myBeacon.setProximityUUID(MY_UUID);
    myBeacon.setSignalPower(0xc5);

    BLEAdvertisementData advertisementData;
    advertisementData.setFlags(ESP_BLE_ADV_FLAG_LIMIT_DISC | ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT);
    advertisementData.setManufacturerData(myBeacon.getData());

    BLEAdvertising* pAdvertising = pServer->getAdvertising();
    pAdvertising->setAdvertisementData(advertisementData);
    pAdvertising->setScanResponseData(advertisementData);
    pAdvertising->start();
}
