#include <Wire.h>
#include "SparkFun_OPT4048.h"
#include "SparkFun_Qwiic_OLED.h"

SparkFun_OPT4048 myColor;
QwiicMicroOLED myOLED;

//#ifdef
//#define maxArduinoI2CBuffer 32
//#endif

int led = LED_BUILTIN;

void setup()
{
    Serial.begin(115200); // Default config settings
    //Serial1.begin(115200, SERIAL_8N1); // Default config settings - ESP doesn't like this.
    //Serial2.begin(115200, SERIAL_8N1); // Default config settings
    Wire.begin();

    while(!Serial) // Circle the void until the monitor opens
        ;

    Serial.println("I2C Sketch test.");

    if (!myColor.begin()) {
        Serial.println("OPT4048 not detected- check wiring or that your I2C address is correct!");
        while (1) 
            ;
    }
    if (!myOLED.begin()) {
        Serial.println("Could not connect to the OLED - check wiring.");
        while (1) 
            ;
    }

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    String hello = "hello";

    //Wire.setClock(10000); //Low mode
    //Wire.setClock(4000000); //Fast mode
    //Wire.setClock(1000000); //Fast mode plus
    //Wire.setClock(3400000); //High speed mode

    myColor.setBasicSetup();

    int x0 = (myOLED.getWidth() - myOLED.getStringWidth(hello)) / 2;

    // starting y position - screen height minus string height / 2 
    int y0 = (myOLED.getHeight() - myOLED.getStringHeight(hello)) / 2;

    // Draw the text - color of black (0)
    myOLED.text(x0, y0, hello, 0);

    // There's nothing on the screen yet - Now send the graphics to the device
    myOLED.display();
}

void loop()
{
    Serial.print("CIEx: ");
    Serial.print(myColor.getCIEx());
    Serial.print(" CIEy: ");
    Serial.println(myColor.getCIEy());

    delay(200);
}
