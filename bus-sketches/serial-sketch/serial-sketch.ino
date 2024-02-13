#include <Arduino.h>
#include "USB.h"
int led = LED_BUILTIN;

void setup()
{
    Serial.begin(115200); // Default config settings
    //Serial1.begin(115200, SERIAL_8N1); // Default config settings
    //Serial2.begin(115200, SERIAL_8N1); // Default config settings

    while(!Serial) // Circle the void until the monitor opens
        ;

    Serial.println("Serial test sketch.");

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);

}

void loop()
{
    Serial.println("HIGH");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    Serial.println("LOW");
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
}

//void serialWatcher()
//{
//    while(Serial.avialable())
//        Serial1.print(Serial.read())
//}
