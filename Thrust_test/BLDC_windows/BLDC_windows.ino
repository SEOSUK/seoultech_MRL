//#include "HX711.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> //scl = A5 / sda = A4

//HX711 scale(A1, A0);
Adafruit_PWMServoDriver pwm=Adafruit_PWMServoDriver();

float duty = 0, duty_past = 0; // 0~100 [%]
float motor_speed=0;
float thrust=0;



void setup() 
{
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(2000); //->400hz로 측정됨 = 2.5ms 즉 1당 0.620us
//  scale.set_scale(2280.f);
//  scale.tare();
}

void loop() 
{
    duty = Serial.parseInt();
    if(duty == 0) duty = duty_past;
    //duty = Serial.parseInt();
    //Serial.println(duty);
    //motor_speed = constrain(map(duty, 0, 100, 1613, 3226), 1613, 3226); //1000~2000ms 로 제한을 걸고 duty를 mapping 하여 변환함
    pwm.setPWM(0,0,duty);
    pwm.setPWM(3,0,duty);
    duty_past = duty;
    delay(100);
  
//  thrust = 30.82399*scale.get_units();
  Serial.println(thrust);
  digitalWrite(3, HIGH);
}
