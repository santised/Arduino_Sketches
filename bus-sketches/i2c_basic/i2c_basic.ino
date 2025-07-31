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
const int black = 0x00; 
const int white = 0x01; 
const int solid = 0xFF; 
const int reset = A0;
const int height = 128;
const int width = 128;
const int fullPixelWidth = 16;

int count = 0; 


void setup()
{
  Serial.begin(115200); // Default config settings
  //Serial1.begin(115200, SERIAL_8N1); // Default config settings - ESP doesn't like this.
  //Serial2.begin(115200, SERIAL_8N1); // Default config settings
  Wire.begin();

  //while(!Serial)
  //  ;

  Serial.println("I2C Sketch test.");

  pinMode(reset, OUTPUT);
  digitalWrite(reset, LOW); 
  delay(10);
  digitalWrite(reset, HIGH); 

  //Wire.setClock(10000); //Low mode
  Wire.setClock(4000000); //Fast mode
  //Wire.setClock(10000000); //Fast mode plus
  //Wire.setClock(3400000); //High speed mode
  
  //getDriverID();
  manufactureSetup();
  writeCommandParameter(0xB0, 0x00);
  writeCommand(0x00);
  writeCommand(0x10);
}

void loop()
{
  //pixel is 8 bits wide so 8 * 16 = 128
  //for(int k = 0; k <= solid; k++)
  //{
  //  for(int j = 0; j < height; j ++)
  //  {
  //    for(int i = 0; i < 16; i++)
  //    {
  //      drawPixel(k);
  //    }
  //  }
  //}
  //delay(2000);
  for(int j = 0; j < height; j ++)
  {
    for(int i = 0; i < fullPixelWidth; i++)
    {
      drawPixel(solid);
    }
  }
  for(int j = 0; j < height; j ++)
  {
    for(int i = 0; i < fullPixelWidth; i++)
    {
      drawPixel(black);
    }
  }

}
