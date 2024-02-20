// For new UUID generation, use https://www.uuidgenerator.net/

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Wire.h>
#include "SparkFun_STTS22H.h"

SparkFun_STTS22H mySTTS; 


//BLE server name
#define bleServerName "AESOP_ESP32C3"
#define SERVICE_UUID "91bad492-b950-4226-aa2b-4ede9fa42f59"

// Timer variables
unsigned int temperature;
float tempF; 
static char charTemperature[6];

bool deviceConnected = false;
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;


// STTS22H temperature Characteristic and Descriptor
BLECharacteristic sttTempCharacteristic("f78ebbff-c8b7-4107-93de-889a6a06d408", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor sttTempDescriptor(BLEUUID((uint16_t)0x2902));

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
	if( !mySTTS.begin() )
	{
		Serial.println("Did not begin.");
		while(1);
	}

	mySTTS.setDataRate(STTS22H_POWER_DOWN);
	// Enables incrementing register behavior for the IC.
	// It is not enabled by default as the datsheet states and
	// is vital for reading the two temperature registers.
	mySTTS.enableAutoIncrement();

    // Create the BLE Device
    BLEDevice::init(bleServerName);

    // Create the BLE Server
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService *stts22hService = pServer->createService(SERVICE_UUID);

    // Create BLE Characteristics and Create a BLE Descriptor
    // STTS22H temperature Concentration Characteristic.
    stts22hService->addCharacteristic(&sttTempCharacteristic);
    sttTempDescriptor.setValue("Temperatue Data.");
    sttTempCharacteristic.addDescriptor(&sttTempDescriptor);

    // Start the service
    stts22hService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pServer->getAdvertising()->start();
    Serial.println("Waiting a client connection to notify...");
}

void loop() {

  if (deviceConnected) {
    if ((millis() - lastTime) > timerDelay) {
        mySTTS.setDataRate(STTS22H_ONE_SHOT); 
        delay(1);
		mySTTS.getTemperatureF(&tempF);

		// Temperature in different units can be retrieved
		// using the following functions.

		//mySTTS.getTemperatureC(&tempF);
		//mySTTS.getTemperatureK(&tempF);
        dtostrf(tempF, 6, 2, charTemperature);

        // Set the charactersistic's value to the temperature reading
        sttTempCharacteristic.setValue(charTemperature);
        sttTempCharacteristic.notify();
		Serial.print("Temp: "); 
		Serial.print(tempF);
		Serial.println("F"); 

        lastTime = millis();
    }
  }
}

