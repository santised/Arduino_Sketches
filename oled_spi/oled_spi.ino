#include <SPI.h>

const int black = 0x00; 
const int white = 0x01; 
const int solid = 0xFF; 
const int reset = A0;
const int height = 128;
const int width = 128;
const int fullPixelWidth = 16;
// Four wire SPI includes D/C "deeCee" which indicates command or ram data being sent over data line
const int reset = A0;
// LOW == command, HIGH == ram data
const int deeCee = A1;
const int cs = 10;
/*
 * Clock is idle HIGH
 * CPOL = 1
 * Data activates on first clock edge after CS activates
 * CPHA = 1
 * CPHA + CPOL = SPI_MODE3
*/
SPISettings oledSettings(14000000, MSBFIRST, SPI_MODE3);

void setup()
{
  pinmode(reset, OUTPUT);
  pinmode(deeCee, OUTPUT);
  pinmode(cs, OUTPUT);

  SPI.begin();

}

void loop()
{
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
