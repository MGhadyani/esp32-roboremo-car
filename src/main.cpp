#include <Arduino.h>
#include <roboremo.h>
#include "FastLED.h"

roboremo roboremo_main;
#define NUM_LEDS 4
#define DATA_PIN 12

CRGB leds[NUM_LEDS];

#define motor_speed_1 2
#define motor_speed_2 5
#define motor_dir_1 4
#define motor_dir_2 18
#define led_left 15
#define BATTERY_PIN 35
#define LOW_BATTERY_THRESHOLD 3.3 // 3.3 voltage
#define led_battery 13

const int pwmChannel_1 = 0;
const int pwmChannel_2 = 1;
const int pwmFreq = 5000;
const int pwmResolution = 8; // 8-bit resolution (0-255)

// void setup()
// {
//   Serial.begin(115200);

//   roboremo_main.begin();

//   ledcSetup(pwmChannel_1, pwmFreq, pwmResolution);
//   ledcSetup(pwmChannel_2, pwmFreq, pwmResolution);
//   ledcAttachPin(motor_speed_1, pwmChannel_1);
//   ledcAttachPin(motor_speed_2, pwmChannel_2);
//   pinMode(motor_dir_1, OUTPUT);
//   pinMode(motor_dir_2, OUTPUT);
//   digitalWrite(motor_dir_1, LOW);
//   digitalWrite(motor_dir_2, LOW);
//   pinMode(led_left, OUTPUT);
//   digitalWrite(led_left, LOW);

//   ledcWrite(pwmChannel_1, 0);
//   ledcWrite(pwmChannel_2, 0);

//   pinMode(led_battery, OUTPUT);
//   digitalWrite(led_battery, LOW);
// }
// /**/
void setup()
{
  Serial.begin(115200);
  roboremo_main.begin();

  ledcSetup(pwmChannel_1, pwmFreq, pwmResolution);
  ledcSetup(pwmChannel_2, pwmFreq, pwmResolution);
  ledcAttachPin(motor_speed_1, pwmChannel_1);
  ledcAttachPin(motor_speed_2, pwmChannel_2);
  pinMode(motor_dir_1, OUTPUT);
  pinMode(motor_dir_2, OUTPUT);
  digitalWrite(motor_dir_1, LOW);
  digitalWrite(motor_dir_2, LOW);
  pinMode(led_left, OUTPUT);
  digitalWrite(led_left, LOW);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  //   // Configure PWM Pins
  //   ledcSetup(0, freq, resolution);
  //   ledcSetup(1, freq, resolution);
  //   ledcAttachPin(enable1Pin, 0);
  //   ledcAttachPin(enable2Pin, 1);

  // Initialize PWM with 0 duty cycle
  ledcWrite(pwmChannel_1, 0);
  ledcWrite(pwmChannel_2, 0);

  pinMode(led_battery, OUTPUT);
  digitalWrite(led_battery, LOW);

}

void loop()
{
  roboremo_main.handel();

  if (roboremo_main.sliderValue[1] > 0)
  {
    digitalWrite(motor_dir_1, LOW);
    leds[2] = CRGB(0, abs(roboremo_main.sliderValue[1]), 0);
    leds[3] = CRGB(0, abs(roboremo_main.sliderValue[1]), 0);
  
    FastLED.show();
    ledcWrite(pwmChannel_1, roboremo_main.sliderValue[1]);
    // Serial.println(roboremo_main.sliderValue[1]);
  }
  else
  {
    digitalWrite(motor_dir_1, HIGH);
    leds[2] = CRGB(abs(roboremo_main.sliderValue[1]), 0, 0);
    leds[3] = CRGB(abs(roboremo_main.sliderValue[1]), 0, 0);
  
    FastLED.show();
    ledcWrite(pwmChannel_1, 255 - abs(roboremo_main.sliderValue[1]));
  }

  if (roboremo_main.sliderValue[2] > 0)
  {
    digitalWrite(motor_dir_2, LOW);
    ledcWrite(pwmChannel_2, roboremo_main.sliderValue[2]);
    // Serial.println(roboremo_main.sliderValue[1]);
    leds[0] = CRGB(0, roboremo_main.sliderValue[2], 0);
    leds[1] = CRGB(0, roboremo_main.sliderValue[2], 0);

    FastLED.show();
    
  }
  else
  {
    digitalWrite(motor_dir_2, HIGH);
    leds[0] = CRGB(abs(roboremo_main.sliderValue[2]), 0, 0);
    leds[1] = CRGB(abs(roboremo_main.sliderValue[2]), 0, 0);

    FastLED.show();
    ledcWrite(pwmChannel_2, 255 - abs(roboremo_main.sliderValue[2]));
  }
  digitalWrite(led_left, roboremo_main.buttonValue[1]);

  if (roboremo_main.batteryPercentage <= 15)
  {
    digitalWrite(led_battery, HIGH);
  }
  else
  {
    digitalWrite(led_battery, LOW);
  }
}
