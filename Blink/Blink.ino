#include <FastLED.h>

#define NUM_LEDS 70

#define DATA_PIN 11 
#define CLOCK_PIN 13

CRGB ledStrip[NUM_LEDS];
uint8_t lastLed = NUM_LEDS;

void setup() { 
    FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(ledStrip, NUM_LEDS);  // BGR ordering is typical
    Serial.begin(115200);
}

void loop() { 
  // Turn the LED on, then pause
  
  lastLed = NUM_LEDS; 
  for (size_t led = 0; led < NUM_LEDS; led++){

    ledStrip[led] = CRGB::Red;
    ledStrip[lastLed - 1] = CRGB::Blue;
    FastLED.show();
    delay(5);
    ledStrip[led] = CRGB::Black;
    ledStrip[lastLed - 1] = CRGB::Black;
    delay(5);
    lastLed--;
  }
  
}
