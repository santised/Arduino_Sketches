#include <Wire.h>

const int oledAddrOpen = 0x3C; 
const int oledAddrClosed = 0x3D; 
int led = LED_BUILTIN;

enum registerMap
{
  controlByte = 0x80,
  randomValue = 0x1,
  panelID = 0xE1,  
  driverID = 0xE2, //0x60 is the answer
}; 

void setup()
{
  Serial.begin(115200); // Default config settings
  //Serial1.begin(115200, SERIAL_8N1); // Default config settings - ESP doesn't like this.
  //Serial2.begin(115200, SERIAL_8N1); // Default config settings
  Wire.begin();

  while(!Serial) // Circle the void until the monitor opens
      ;

  Serial.println("I2C Sketch test.");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
    
  //Wire.setClock(10000); //Low mode
  //Wire.setClock(4000000); //Fast mode
  //Wire.setClock(1000000); //Fast mode plus
  //Wire.setClock(3400000); //High speed mode
  Wire.beginTransmission(oledAddrOpen);
  int temp = Wire.endTransmission(oledAddrOpen);
  Serial.println("Result (want '0'): ");
  Serial.println(temp); 

  while (1)
  {
    Wire.beginTransmission(oledAddrOpen);
    Wire.write(controlByte);
    Wire.write(driverID);
    Wire.endTransmission();
    Wire.requestFrom(oledAddrOpen, 1); //just read?`


    while(Wire.available())
      Serial.println(Wire.read(), HEX);

    Wire.endTransmission();
    delay(1000);
  }

}

void loop()
{
    delay(1000);
}
