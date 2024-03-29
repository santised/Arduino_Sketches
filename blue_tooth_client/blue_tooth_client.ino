#include "BLEDevice.h"
#include <Wire.h>
#include "SparkFun_Qwiic_OLED.h"
#include "res/qw_fnt_8x16.h"

QwiicNarrowOLED myOLED;
String strCO2 = "CO2: ";
String readings = "";

//BLE Server name (the other ESP32 name running the server sketch)
#define bleServerName "AESOP_ESP32C3"

/* UUID's of the service, characteristic that we want to read*/
// BLE Service
static BLEUUID ensServiceUUID("91bad492-b950-4226-aa2b-4ede9fa42f59");

// BLE Characteristics #ifdef temperatureCelsius
static BLEUUID ensCO2CharacteristicUUID("f78ebbff-c8b7-4107-93de-889a6a06d408");

//Flags stating if should begin connecting and if the connection is up
static boolean doConnect = false;
static boolean connected = false;

//Address of the peripheral device. Address will be found during scanning...
static BLEAddress *pServerAddress;
 
//Characteristicd that we want to read
static BLERemoteCharacteristic* co2Characteristic;

//Activate notify
const uint8_t notificationOn[] = {0x1, 0x0};
const uint8_t notificationOff[] = {0x0, 0x0};

//Variables to store temperature and humidity
char* co2Char;

//Flags to check whether new temperature and humidity readings are available
boolean newCO2 = false;

//Connect to the BLE Server that has the name, Service, and Characteristics
bool connectToServer(BLEAddress pAddress) {
   BLEClient* pClient = BLEDevice::createClient();
 
  // Connect to the remove BLE Server.
  pClient->connect(pAddress);
  Serial.println(" - Connected to server");
 
  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(ensServiceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(ensServiceUUID.toString().c_str());
    return (false);
  }
 
  // Obtain a reference to the characteristics in the service of the remote BLE server.
  co2Characteristic = pRemoteService->getCharacteristic(ensCO2CharacteristicUUID);

  if (co2Characteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID");
    return false;
  }
  Serial.println(" - Found our characteristics");
 
  //Assign callback functions for the Characteristics
  co2Characteristic->registerForNotify(co2NotifyCallback);
  return true;
}

//Callback function that gets called, when another device's advertisement has been received
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getName() == bleServerName) { //Check if the name of the advertiser matches
      advertisedDevice.getScan()->stop(); //Scan can be stopped, we found what we are looking for
      pServerAddress = new BLEAddress(advertisedDevice.getAddress()); //Address of advertiser is the one we need
      doConnect = true; //Set indicator, stating that we are ready to connect
      Serial.println("Device found. Connecting!");
    }
  }
};
 
//When the BLE Server sends a new temperature reading with the notify property
static void co2NotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, 
                                        uint8_t* pData, size_t length, bool isNotify) {
  //store temperature value
  co2Char = (char*)pData;
  newCO2 = true;
}

//function that prints the latest sensor readings in the OLED myOLED
void printReadings()
{

    readings = strCO2 + co2Char + "ppm";
    myOLED.erase();
    int x0 = (myOLED.getWidth() - myOLED.getStringWidth(readings)) / 2;
    int y0 = (myOLED.getHeight() - myOLED.getStringHeight(readings)) / 2;
    myOLED.text(x0, y0, readings);
    myOLED.display();

    Serial.print("CO2:");
    Serial.print(co2Char);
    Serial.println("ppm");
}

void setup() {

    Wire.begin();
    //Start serial communication
    Serial.begin(115200);
    Serial.println("Starting Arduino BLE Client application...");

    if(myOLED.begin() == false) 
    {
        Serial.println("Unable to communicate with display");
        while (1)
            ;
    }

    Serial.println("Communication established with display");

    myOLED.setFont(QW_FONT_8X16);

    String hello = "Hello You."; // our message
    // Center the text.
    // starting x and y position - screen width/heigth minus string width/height  / 2
    int x0 = (myOLED.getWidth() - myOLED.getStringWidth(hello)) / 2;
    int y0 = (myOLED.getHeight() - myOLED.getStringHeight(hello)) / 2;
    myOLED.text(x0, y0, hello);
    myOLED.display();

    //Init BLE device
    BLEDevice::init("");

    // Retrieve a Scanner and set the callback we want to use to be informed when we
    // have detected a new device.  Specify that we want active scanning and start the
    // scan to run for 30 seconds.
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->start(30);
}

void loop() {
    // If the flag "doConnect" is true then we have scanned for and found the desired
    // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
    // connected we set the connected flag to be true.
    if (doConnect == true) {
        if (connectToServer(*pServerAddress)) 
        {
          Serial.println("We are now connected to the BLE Server.");
          //Activate the Notify property of each Characteristic
          co2Characteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
          connected = true;
        } 

        else 
        {
          Serial.println("We have failed to connect to the server; Restart your device to scan for nearby BLE server again.");
        }
        doConnect = false;
    }

    //if new temperature readings are available, print in the OLED
    if(newCO2)
    {
        newCO2 = false;
        printReadings();
    }

    delay(1000); // Delay a second between loops.
}
