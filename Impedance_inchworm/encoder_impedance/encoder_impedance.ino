#include <Servo.h>
#define CLK 2    // 2번핀을 CLK로 지정
#define DT 3    // 3번핀을 DT로 지정

#define Kp 10    // 3번핀을 DT로 지정
#define Kd 3    // 3번핀을 DT로 지정

int counter = 0;           // 회전 카운터 측정용 변수
int counter_past = 0;           // 회전 카운터 측정용 변수
int currentStateCLK;       // CLK의 현재 신호상태 저장용 변수
int lastStateCLK;          // 직전 CLK의 신호상태 저장용 변수 

Servo myservo;

void Read_encoder()
{
  currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1) counter = digitalRead(DT) != currentStateCLK ? counter+1 : counter-1;
  lastStateCLK = currentStateCLK;
  delay(1);
  
}


void setup() 
{
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  Serial.begin(2000000);
  
  myservo.attach(5);
  myservo.write(60);
  delay(1000);
  lastStateCLK = digitalRead(CLK);
}

double now_angle = 60;
double past_angle = 60;

void loop() 
{
  now_angle = 60+Kp*counter+Kd*(counter-counter_past);
  myservo.write(constrain(now_angle,20,100));
  
  Read_encoder();
  
  Serial.print(counter);
  Serial.print(",");
  Serial.println(now_angle);
  
  counter_past = counter;
  

}
