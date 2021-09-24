#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 30
#define LED_DATA_PIN 3
#define SENSOR_LEFT_TRIGGER_PIN 7
#define SENSOR_LEFT_ECHO_PIN 6


CRGB ledsLeftSide[NUM_LEDS];
int index = 0;

void mapDistanceToColor(int color[], long distanceInCm){
  if(distanceInCm <= 10){
    color[0] = 255; color[1] = 0; color[2] = 0;
  }
  if(distanceInCm > 10 && distanceInCm <= 15) {
    color[0] = 255; color[1] = 191; color[2] = 0;
  }
  if(distanceInCm > 15 && distanceInCm <= 18) {
    color[0] = 255; color[1] = 255; color[2] = 0;
  }
  if(distanceInCm > 18 && distanceInCm <= 20) {
    color[0] = 191; color[1] = 255; color[2] = 0;
  }
  if(distanceInCm > 20) {
    color[0] = 0; color[1] = 255; color[2] = 0;
  }
}

long getDistanceInCm(uint8_t trigger_pin, uint8_t echo_pin)
{
  digitalWrite(trigger_pin, LOW); 
  delay(5); 
  digitalWrite(trigger_pin, HIGH); 
  delay(10);
  digitalWrite(trigger_pin, LOW);
  long time = pulseIn(echo_pin, HIGH); 
return (time/2) * 0.03432; 
}

void setColorByDistance(CRGB leds[], long distance){
  for(int i = 0; i < NUM_LEDS; i++ ){
    int color[3];
    mapDistanceToColor(color, distance);
    leds[i].setRGB(color[0],color[1],color[2]);
  }
  FastLED.show();
}

void setup() {
  FastLED.addLeds<WS2812B, LED_DATA_PIN ,GRB>(ledsLeftSide, NUM_LEDS);

  pinMode(SENSOR_LEFT_TRIGGER_PIN, OUTPUT);
  pinMode(SENSOR_LEFT_ECHO_PIN, INPUT); 
}

void loop() {
  long distanceLeft = getDistanceInCm(SENSOR_LEFT_TRIGGER_PIN, SENSOR_LEFT_ECHO_PIN);
  setColorByDistance(ledsLeftSide, distanceLeft);
  delay(200);
}
