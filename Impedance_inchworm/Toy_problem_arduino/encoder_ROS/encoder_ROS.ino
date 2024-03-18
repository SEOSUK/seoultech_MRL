//1. arduino upload 
//2. roscore 
//3. rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=57600
//4. rostopic echo /angle_deg

//Encoder1 pin map
//pin 4  VCC  (line black )-> vcc(5V)
//pin 6  GND  (line white )-> Gnd
//pin 8  A+   (line gray  )-> A_plus1 pin
//pin 9  B+   (line purple)-> B_plus1 pin
//pin 10 A-   (line blue  )-> Gnd
//pin 11 B-   (line green )-> Gnd

//Encoder2 pin map
//pin 4  VCC  (line black )-> vcc(5V)
//pin 6  GND  (line white )-> Gnd
//pin 8  A+   (line gray  )-> A_plus2 pin
//pin 9  B+   (line purple)-> B_plus2 pin
//pin 10 A-   (line blue  )-> Gnd
//pin 11 B-   (line green )-> Gnd

#include <ros.h>
#include <std_msgs/Float64.h>

ros::NodeHandle nh;
std_msgs::Float64 angle_deg;
ros::Publisher p("angle_deg", &angle_deg);

#define A_plus1 3  
#define B_plus1 2 
#define A_plus2 5  
#define B_plus2 4 
#define gear_ratio 1 // 40(big_gear) / 13(small_gear) 
#define counter_to_deg 0.703125 // 360(deg) / 512(PPR)

int counter1 = 0; int counter_past1 = 0;        
int state_rotation1; int state_rotation_past1;  

int counter2 = 0; int counter_past2 = 0;        
int state_rotation2; int state_rotation_past2;  
  
float angle1 = 0.0; float angle2 = 0.0;

void Read_encoder1()
{
  state_rotation1 = digitalRead(A_plus1);
  if (state_rotation1 != state_rotation_past1 && state_rotation1 == 1) counter1 = digitalRead(B_plus1) != state_rotation1 ? counter1+1 : counter1-1;
  state_rotation_past1 = state_rotation1; 
  counter_past1 = counter1;
  angle1 = (counter1/gear_ratio)*counter_to_deg;

}

void Read_encoder2()
{
  state_rotation2= digitalRead(A_plus2);
  if (state_rotation2 != state_rotation_past2 && state_rotation2 == 1) counter2 = digitalRead(B_plus2) != state_rotation2 ? counter2+1 : counter2-1;
  state_rotation_past2 = state_rotation2; 
  counter_past2 = counter2;
  angle2 = (counter2/gear_ratio)*counter_to_deg;

}


void setup() 
{
  pinMode(A_plus1, INPUT_PULLUP);
  pinMode(B_plus1, INPUT_PULLUP);
  pinMode(A_plus2, INPUT_PULLUP);
  pinMode(B_plus2, INPUT_PULLUP);
  
  state_rotation_past1 = digitalRead(A_plus1);
  state_rotation_past2 = digitalRead(A_plus2);
  
  Serial.begin(2000000);
  attachInterrupt(digitalPinToInterrupt(A_plus1), Read_encoder1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(A_plus2), Read_encoder2, CHANGE);
//  nh.initNode();
//  nh.advertise(p);
}

void loop() 
{
//  angle_deg.data = angle;
//  p.publish( &angle_deg );
//  nh.spinOnce();
  
}
