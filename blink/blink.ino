#include "USB.h"
int LED = 10;

void setup()
{
    USBSerial.PID(0x0035);
    USBSerial.VID(0x1B4F);
    Serial.begin(115200);
    Serial.print("Hello You.");

    pinMode(LED, OUTPUT);
}

void loop()
{
    
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
    delay(1000);

}
