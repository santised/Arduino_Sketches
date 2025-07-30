/*
  0b10000000 - "1" is the C0 bit

  C0 == "0" indicates sending last control byte, only data bytes follow
  * If you only send one control byte then it should be zero
  C0 == "1" indicates the next two bytes are data byte and another control byte.....

  0b01000000 - "1" is the D/C bit

  D/C == "1" indicates the databyte is for RAM operation
  D/C == "0" indicates the databyte is for command operation
*/
#include <Wire.h>

const int oledAddrOpen = 0x3C; 
const int oledAddrClosed = 0x3D; 
int reset = A0;


void setup()
{
  Serial.begin(115200); // Default config settings
  //Serial1.begin(115200, SERIAL_8N1); // Default config settings - ESP doesn't like this.
  //Serial2.begin(115200, SERIAL_8N1); // Default config settings
  Wire.begin();

  Serial.println("I2C Sketch test.");

  pinMode(reset, OUTPUT);
  digitalWrite(reset, LOW); 
  delay(10);
  digitalWrite(reset, HIGH); 

  //Wire.setClock(10000); //Low mode
  //Wire.setClock(4000000); //Fast mode
  //Wire.setClock(1000000); //Fast mode plus
  //Wire.setClock(3400000); //High speed mode
  Wire.beginTransmission(oledAddrOpen);
  int temp = Wire.endTransmission(oledAddrOpen);
  Serial.println("Result (want '0'): ");
  Serial.println(temp); 

  // Sanity Check.
  //getDriverID();
  manufactureSetup();
  //emptySquare();

  Serial.println("Write finished, Looping.");
  //writeCommand(commandDisableEntireDisplay);
  Serial.print("Checking if display is on: ");
  getDriverID();
}

void loop()
{
  for( int row = 0; row < 0x3F; row++)
  {
    writeCommandParameter(0xB0, row);
    writeCommandParameter(0x00, 0x00);
    writeCommandParameter(0x10, 0x10);
    delay(10);
      for(int i = 0; i < 0x0F; i++)
      {
        fillSquare(i);
        delay(100);
      }
    }
  delay(100);
}
