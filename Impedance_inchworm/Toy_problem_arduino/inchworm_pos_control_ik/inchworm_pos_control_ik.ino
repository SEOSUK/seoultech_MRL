//         L
//   <----------->
// 
//  {2}----link2----{3}----link3----{4}      ^
//   |                               |       |
// link1                           link4     | l
//   |                               |       |  
//  {1}=base                        {5}=tool | 
  //
//                           zero point(x0, y0) x0 = 2L / y0 = 0



//                               /
//                              /    45 deg
// set point ------------------/-  -  -  -  -  -
//                     135 deg 


#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> //scl = A5 / sda = A4

Adafruit_PWMServoDriver pwm=Adafruit_PWMServoDriver();

#define L 0.1        // [m]
#define l 0.07       // [m]
#define deg_min_pwm 900  //pca9685 unit of pwm when 0 deg
#define deg_mid_pwm 2250 //pca9685 unit of pwm when 90 deg
#define deg_max_pwm 3599 //pca9685 unit of pwm when 180 deg
#define deg_set 30       // servo angle set point [deg]
#define x0 L  // [m]
#define y0 0         // [m]

#define theta1_pin 0 
#define theta2_pin 1
#define theta3_pin 2


double theta1 = deg_set*(PI/180); double theta2 = (180-2*deg_set)*(PI/180); double theta3 = deg_set*(PI/180); //[rad]
double x = x0; double y = y0;

int command = 0;


void setup() 
{
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(300);
  
}

void ik(double x, double y)
{
    theta2 = acos((x*x+y*y-2*L*L)/(2*L*L));
    theta1 = atan2(y,x) - (asin(L*sin(theta2)/sqrt(x*x+y*y)));
    theta3 = -(theta1 + theta2);

    theta1 = PI/2 + theta1;
    theta3 = PI/2 + theta3;
    
    Serial.print(theta1*180/PI); 
    Serial.print(","); 
    Serial.print(theta2*180/PI); 
    Serial.print(","); 
    Serial.println(theta3*180/PI); 
}

void move_servo()
{

  int theta1_pca_unit = constrain(900+(theta1*180/PI)*(2250-900)/90, 900, 2500);
  int theta2_pca_unit = constrain(900+(theta2*180/PI)*(2250-900)/90, 900, 2500);
  int theta3_pca_unit = constrain(900+(theta3*180/PI)*(2250-900)/90, 900, 2500);

  pwm.setPWM(theta1_pin, 0, theta1_pca_unit);
  pwm.setPWM(theta2_pin, 0, theta2_pca_unit);
  pwm.setPWM(theta3_pin, 0, theta3_pca_unit);
  delay(30);
}



void loop() 
{
  
  int buf = Serial.parseInt();
  command = buf == 0 ? command : buf;

  switch(command)
  {
    case 1:
    
      for(double dy=0;dy>-0.1;dy-=0.001)
      {
        x=x0;
        y = y0+dy;
        ik(x, y);
        move_servo();
      }    
      
      for(double dx=0;dx<0.07;dx+=0.001)
      {
        x = x0+dx;
        ik(x, y);
        move_servo();
      }

      for(double dy=-0.1;dy<0.1;dy+=0.001)
      {
        y = y0+dy;
        ik(x, y);
        move_servo();
      }
      for(double dx=0.07;dx>0;dx-=0.001)
      {
        x = x0+dx;
        ik(x, y);
        move_servo();
      }
      for(double dy=0.1;dy>0;dy-=0.001)
      {
        y = y0+dy;
        ik(x, y);
        move_servo();
      }  
      
      break;
      
    case 2:
      theta1 = 0; theta2 = 0; theta3 = 0; //[rad]
      move_servo();
      break;
      
    case 3:
        theta1 = deg_set*(PI/180); theta2 = (180-2*deg_set)*(PI/180); theta3 = deg_set*(PI/180); //[rad]
        move_servo();
        command = 0;
      break;
    default:
      
      break; 
  }
  command = 0;
}
