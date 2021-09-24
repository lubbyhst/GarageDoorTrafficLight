#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 30
#define LED_DATA_PIN 3
#define SENSOR_LEFT_TRIGGER_PIN 7
#define SENSOR_LEFT_ECHO_PIN 6
#define SENSOR_MAX_DIFF 3l
#define STANDBY_DELAY 500
#define RUNNING_DELAY 50
#define STANDBY_TIMEOUT_IN_SECONDS 5

CRGB ledsLeftSide[NUM_LEDS];
int noChangeCounter = 0;
int index = 0;
int actualColor[3];
long distanceLeftLast = 0;

void mapDistanceToColor(int color[], long distanceInCm){
  if(distanceInCm <= 10){
    color[0] = 255; color[1] = 0; color[2] = 0;
  }
  if(distanceInCm > 10 && distanceInCm <= 30) {
    color[0] = 255; color[1] = (255/20*(distanceInCm-10)); color[2] = 0;
  }
  if(distanceInCm > 30 && distanceInCm <= 50) {
    color[0] = (255 - (255/20*(distanceInCm-10))); color[1] = 255; color[2] = 0;
  }
  if(distanceInCm > 50) {
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
  int color[3];
  mapDistanceToColor(color, distance);
  for(int i = 0; i < NUM_LEDS; i++ ){
    leds[i].setRGB(color[0],color[1],color[2]);
  }
  FastLED.show();
  memcpy(color, actualColor, sizeof(actualColor)); 
}

void setup() {
  //Serial.begin(9600);

  FastLED.addLeds<WS2812B, LED_DATA_PIN ,GRB>(ledsLeftSide, NUM_LEDS);

  pinMode(SENSOR_LEFT_TRIGGER_PIN, OUTPUT);
  pinMode(SENSOR_LEFT_ECHO_PIN, INPUT); 
}

void loop() {
  long distanceLeftCurrent = getDistanceInCm(SENSOR_LEFT_TRIGGER_PIN, SENSOR_LEFT_ECHO_PIN);
  //Serial.print("Current distance left: "); Serial.println(distanceLeftCurrent);
  //Serial.print("Last distance left: "); Serial.println(distanceLeftLast);
  if( -SENSOR_MAX_DIFF <= (distanceLeftLast-distanceLeftCurrent) && (distanceLeftLast-distanceLeftCurrent) <= SENSOR_MAX_DIFF){
    noChangeCounter++;
  } else {
    noChangeCounter = 0;
    setColorByDistance(ledsLeftSide, distanceLeftCurrent);
    distanceLeftLast = distanceLeftCurrent;
  }
  if(noChangeCounter > STANDBY_TIMEOUT_IN_SECONDS * 1000 / RUNNING_DELAY){
    FastLED.clear();
    FastLED.show();
    delay(STANDBY_DELAY);// slow down in standby
  }
  delay(RUNNING_DELAY);
}