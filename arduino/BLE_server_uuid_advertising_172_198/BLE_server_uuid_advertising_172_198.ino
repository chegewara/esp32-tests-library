/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 4fafc201-1fb5-459e-8fcc-c5c9c331914b
   And has a characteristic of: beb5483e-36e1-4688-b7f5-ea07361b26a8

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   A connect hander associated with the server starts a background task that performs notification
   every couple of seconds.
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
//extern "C"{ 
 // #include "gap_api.h" 
//  #include "gattdefs.h" 
//  }
BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
uint8_t value = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "2ae78970-7d44-44bb-b097-26183f402400"
#define CHARACTERISTIC_UUID1 "2ae78970-7d44-44bb-b097-26183f402401"
#define CHARACTERISTIC_UUID2 "2ae78970-7d44-44bb-b097-26183f402402"
#define CHARACTERISTIC_UUID3 "2ae78970-7d44-44bb-b097-26183f402403"
#define CHARACTERISTIC_UUID4 "2ae78970-7d44-44bb-b097-26183f402404"
#define CHARACTERISTIC_UUID5 "2ae78970-7d44-44bb-b097-26183f402405"


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};



void setup() {
  Serial.begin(115200);
//tGAP_BLE_ATTR_VALUE attr;
//attr.icon = 0x485;
  // Create the BLE Device
  BLEDevice::init("IoT-DK-SFL");

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID), 20);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID1,
                      BLECharacteristic::PROPERTY_NOTIFY 
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID2,
                      BLECharacteristic::PROPERTY_NOTIFY 
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID3,
                      BLECharacteristic::PROPERTY_NOTIFY 
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID4,
                      BLECharacteristic::PROPERTY_NOTIFY 
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID5,
                      BLECharacteristic::PROPERTY_NOTIFY 
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();
//GAP_BleAttrDBUpdate(GATT_UUID_GAP_ICON, &attr);
  // Start advertising
  pServer->getAdvertising()->addServiceUUID(BLEUUID((uint16_t)0x2ae7));
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {

  if (deviceConnected) {
    Serial.printf("*** NOTIFY: %d ***\n", value);
    pCharacteristic->setValue(&value, 1);
    pCharacteristic->notify();
    //pCharacteristic->indicate();
    value++;
  }
  delay(1000);
}
