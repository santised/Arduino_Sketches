/*
  ePaper Display with SRAM and micro SD
  By: Ciara Jekel
  SparkFun Electronics
  Date: August 13th, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14892

  This example loads a bitmap from the SD card onto the display.
  Bitmap image is 24 bit with same dimensions as display and saved on SD card as "img.bmp"

  Hardware Connections:
  BUSY   9
  RST    8
  MISO  12
  MOSI  11
  SCK   13
  SDCS   7
  SRCS   6
  DCS    5
  D/C    4
  GND    GND
  5V     Logic Level (if using 5V logic (e.g. Arduino) connect to 5V. if using 3.3V logic (e.g. Teensy) connect to 3.3V)
  SD in SD card slot
*/

//Click here to get the library: http://librarymanager/All#SparkFun_ePaper
#include "SparkFun_ePaper_154.h"
#include "SparkFun_ePaper_420.h"
#include "SparkFun_ePaper_750.h"

//You must also have the SparkFun HyperDisplay library.
//Click here to get the library: http://librarymanager/All#SparkFun_HyperDisplay
#include "hyperdisplay.h"


#include <SPI.h>
#include <SD.h>

const byte busyPin = 9;
const byte resetPin = 8;
const byte sdCSPin = 7;
const byte srCSPin = 6;
const byte dCSPin = 5;
const byte dcPin = 4;
uint8_t i = 1; 
unsigned long reset = 0; //This number can get to 4 Billion (2^32)-1 

//uint32_t * where = &reset; & == 'address-of' operator, 'where' is our pointer
//int value = *where = 'dereference' or the 'value pointed to' by 'where'
char * imageArray[] = {"img.bmp", "img2.bmp", "img3.bmp", "img4.bmp"};
//Uncomment your display size
//EPAPER_154 myEPaper;
EPAPER_420 myEPaper;
//EPAPER_750 myEPaper;

void setup() {
  Serial.begin(9600);

  if (!myEPaper.begin(busyPin, resetPin, sdCSPin, srCSPin, dCSPin, dcPin))
    Serial.println("No SD Card Detected");
  //Load first image on startup
  Serial.print("Loading image: ");
  Serial.println(imageArray[i]); 
  myEPaper.bmpFromSD(imageArray[i], 120, 100);
  //Assigning reset after first load of image.  
  reset = millis();
}

void loop() {

  if( millis() - reset >= 45000 ){ //45 seconds
    //Reset is set to current time only after condition is true.
    reset = millis(); 
    if( i == 3 )//only 3 images
      i = 0; 
    else 
      i++; 
    Serial.print("Loading image: ");
    Serial.println(imageArray[i]); 
    //Load new image
    myEPaper.bmpFromSD(imageArray[i], 120, 100);
  }

}

