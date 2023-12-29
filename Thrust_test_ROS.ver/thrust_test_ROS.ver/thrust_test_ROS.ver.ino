#include "HX711.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> //scl = A5 / sda = A4
#include <ros.h>
#include <std_msgs/Float64.h>


ros::NodeHandle nh;

std_msgs::Float64 pwm_msg;
std_msgs::Float64 thrust_msg;

ros::Publisher pwm("pwm", &pwm_msg);
ros::Publisher thrust("thrust", &thrust_msg);

HX711 scale(A1, A0);

Adafruit_PWMServoDriver PWM=Adafruit_PWMServoDriver();

void Run(float duty)
{
  PWM.setPWM(0,0, constrain(map(duty, 0, 100, 1613, 3226), 1613, 3226));
  delay(1500);

  pwm_msg.data = duty;
  thrust_msg.data = 37.037*scale.get_units()*9.81/1000;
  
  pwm.publish( &pwm_msg );
  thrust.publish( &thrust_msg );
  nh.spinOnce();
  
}
void setup() 
{
  PWM.begin();
  PWM.setPWMFreq(390); //->400hz로 측정됨 = 2.5ms 즉 1당 0.620us
  scale.set_scale(2280.f);
  scale.tare();
  
  nh.initNode();
  nh.advertise(pwm);
  nh.advertise(thrust);

  for(int i=0;i<100;i = i + 5)
  {
    Run(i);
  }
  
}

void loop()
{
  PWM.setPWM(0,0, 1613);
}
