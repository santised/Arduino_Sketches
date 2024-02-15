
#include <Wire.h>
#include "SparkFun_Qwiic_OLED.h"
#include "res/qw_fnt_5x7.h"
#include "res/qw_fnt_8x16.h"
#include "res/qw_fnt_31x48.h"

QwiicNarrowOLED myOLED;

int led = LED_BUILTIN;
String usrInput = "";
int centerX; 
int centerY; 

void setup()
{
    Serial.begin(115200); // Default config settings

    Wire.begin();

    Serial.println("I2C Sketch test.");

    if (!myOLED.begin()) {
        Serial.println("Could not connect to the OLED - check wiring.");
        while (1) 
            ;
    }

    String hello = "hello";

    myOLED.setFont(QW_FONT_8X16);

    int x0 = (myOLED.getWidth() - myOLED.getStringWidth(hello)) / 2;
    int y0 = (myOLED.getHeight() - myOLED.getStringHeight(hello)) / 2;

    myOLED.text(x0, y0, hello);

    // There's nothing on the screen yet - Now send the graphics to the device
    myOLED.display();
}

void loop()
{
    if(Serial.available() > 0)
    {
        usrInput = Serial.readStringUntil('\r');
        myOLED.erase();
        centerX = (myOLED.getWidth() - myOLED.getStringWidth(usrInput)) / 2;
        centerY = (myOLED.getHeight() - myOLED.getStringHeight(usrInput)) / 2;
        myOLED.text(centerX, centerY, usrInput);
        myOLED.display();
    }
}
