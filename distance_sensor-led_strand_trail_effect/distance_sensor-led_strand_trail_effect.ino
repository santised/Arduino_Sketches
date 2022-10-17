/*
	Using a VL43LCD, I'm taking a distance value and mapping it, using the map function, 
	to a number within the number of LEDs on a strand of APA102 LEDs. 
*/

#include <ComponentObject.h>
#include <RangeSensor.h>
#include <SparkFun_VL53L1X.h>
#include <vl53l1x_class.h>
#include <vl53l1_error_codes.h>
#include <Wire.h>
#include "SparkFun_VL53L1X.h" //Click here to get the library: http://librarymanager/All#SparkFun_VL53L1X
#include <FastLED.h>

// 29 and 11 are Teensy MicroMod SPI pin numbering
#define NUM_LEDS 29
#define DATA_PIN 11
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];
int led = 0;
int oldLed = 0;

SFEVL53L1X distanceSensor;
int distance = 0;

void setup(void)
{
  Wire.begin();

  Serial.begin(115200);
  Serial.println("VL53L1X Qwiic Test");

  if (distanceSensor.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor failed to begin. Please check wiring. Freezing...");
    while (1) ;
  }
  Serial.println("Sensor online!");

	FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);  // GRB ordering is typical
}

void loop(void)
{

  distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor.checkForDataReady())
  {
    delay(1);
  }

  distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor

	// 600 being "far"
	// 50 being "close"
	// 29 LEDs
	oldLed = led; // Reassign the LED value to make space for a new value. 
	led = map(distance, 50, 600, 1, 29); // Retreive new value.
	
	// If it's a new value (different from old), then turn off the old and turn on the new. 
	if( led != oldLed ) 
	{
		leds[oldLed] = CRGB::Black; // old off

		FastLED.show();

		leds[led].b = random(50, 100); // new on, trying something fun here
		leds[led].g = random(50, 100); // Spoiler alert - not so fun.

		FastLED.show(); 
	}

  distanceSensor.clearInterrupt();
  distanceSensor.stopRanging();

  Serial.print("Distance(mm): ");
  Serial.print(distance);
  Serial.println();

}
