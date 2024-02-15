// For new UUID generation, use https://www.uuidgenerator.net/

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Wire.h>
#include "SparkFun_ENS160.h"

//BLE server name
#define bleServerName "AESOP_ESP32C3"

SparkFun_ENS160 myENS;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;
unsigned int CO2;


bool deviceConnected = false;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID "91bad492-b950-4226-aa2b-4ede9fa42f59"

// ENS CO2 Characteristic and Descriptor
BLECharacteristic ensCO2ConcentrationCharacteristic("f78ebbff-c8b7-4107-93de-889a6a06d408", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor ensCO2Descriptor(BLEUUID((uint16_t)0x2902));

//Setup callbacks onConnect and onDisconnect
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) 
    {
        deviceConnected = true;
    };
    void onDisconnect(BLEServer* pServer) 
    {
        deviceConnected = false;
    }
};

void setup() {

    Wire.begin();

    // Start serial communication 
    Serial.begin(115200);
    
    // Begin sensor
    if(!myENS.begin())
    {
        Serial.println("Could not communicate with the ENS160.");
        while(1)
            ;
    }

    if(myENS.setOperatingMode(SFE_ENS160_RESET))
        Serial.println("ENS160 reset.");

    delay(100);

    myENS.setOperatingMode(SFE_ENS160_STANDARD);

    // Create the BLE Device
    BLEDevice::init(bleServerName);

    // Create the BLE Server
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService *ensService = pServer->createService(SERVICE_UUID);

    // Create BLE Characteristics and Create a BLE Descriptor
    // ENS CO2 Concentration Characteristic.
    ensService->addCharacteristic(&ensCO2ConcentrationCharacteristic);
    ensCO2Descriptor.setValue("CO2 Concentration");
    ensCO2ConcentrationCharacteristic.addDescriptor(&ensCO2Descriptor);

    // Start the service
    ensService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pServer->getAdvertising()->start();
    Serial.println("Waiting a client connection to notify...");
}

void loop() {

  if (deviceConnected) {
    if ((millis() - lastTime) > timerDelay) {

        //Get CO2 reading from ENS
        CO2 = myENS.getECO2();
        static char charCO2[10];
        itoa(CO2, charCO2, 10);

        // Set the charactersistic's value to the CO2 reading
        ensCO2ConcentrationCharacteristic.setValue(charCO2);
        ensCO2ConcentrationCharacteristic.notify();
        Serial.print("CO2: ");
        Serial.print(CO2);
        Serial.println("ppm");

        lastTime = millis();
    }
  }
}

