/*
  Code for a simple Nighlight that turns turns on and off with a button press,
  and dims (or brightens) when you hold a second button down.
  Author: Elias Santistevan
  SparkFun Electronics
  6/2019
*/

#include <FastLED.h>

#define NUM_LEDS 60
#define DATA_PIN 10
#define CLOCK_PIN 12

CRGB leds[NUM_LEDS]; 

// Buttons
int dimButton = 6; 
int capButton = 9; 

// state keeps track it the light is on or off, and high keeps track of the
// limits of illumination of the LED
bool state = false; 
bool high = false; 

// default brightness
int brightness = 10; 

void setup() {

  // Don't forget SAMD21 boards use SerialUSB, not Serial 
  SerialUSB.begin(115200); 
  SerialUSB.println("We up!"); 

  // Capacitive Touch Button
  pinMode(capButton, INPUT); 
  pinMode(dimButton, INPUT_PULLUP); 

  // Notice our APA102s have a BGR order not RGB
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS); 
  FastLED.setBrightness(brightness);
  

}

void loop() {
 
  // If the dimButton is pressed and the "state" (light) is off, then turn it on. 
  if((digitalRead(capButton) == HIGH) && (state == false)){
    delay(300); // Debounce
    state = true; // They are now on.
    for(int i = 0; i < NUM_LEDS; i++){
      leds[i] = CHSV(32, 255, 255); // Warm orange. 
      FastLED.show(); 
      delay(5); // Just enough to look awesome when they turn on.
    }
  }

  // If the dimButton is pressed and the "state" (light) is on, then turn it off. 
  else if((digitalRead(capButton) == HIGH) && (state == true)){
    delay(300); // Debounce
    state = false; // They are now off.
    for(int i = 0; i < NUM_LEDS; i++){
      leds[i] = CRGB::Black;
      FastLED.show(); 
      delay(2);
    }
  }

  // If you keep down the dimButton (onboard D6 button) then the light will start
  // dimming.
  while((digitalRead(dimButton) == LOW) && (high == false)){
      FastLED.setBrightness(brightness += 2); // Don't dim too slowly.
      FastLED.show();
      SerialUSB.println(brightness); 

      if(brightness >= 75) // Max brightness
        high = true; //Jump out of this loop.

      delay(50);
  }

  while((digitalRead(dimButton) == LOW) && (high == true)){
      FastLED.setBrightness(brightness -= 2);
      FastLED.show();
      SerialUSB.println(brightness); 

      if(brightness <= 5)
        high = false; 

      delay(50);
  }

}

