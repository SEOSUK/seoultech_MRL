#include <Servo.h>
#define CLK 2    // 2번핀을 CLK로 지정
#define DT 3    // 3번핀을 DT로 지정
#define SW 4     // 4번핀을 스위치핀으로 지정

int counter = 0;           // 회전 카운터 측정용 변수
int currentStateCLK;       // CLK의 현재 신호상태 저장용 변수
int lastStateCLK;          // 직전 CLK의 신호상태 저장용 변수 
String currentDir ="";      // 현재 회전 방향 출력용 문자열 저장 변수
unsigned long lastButtonPress = 0;     // 버튼 눌림 상태 확인용 변수
Servo myservo;
void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  Serial.begin(2000000);
   myservo.attach(5);
  
  // CLK핀의 현재 상태 확인
  myservo.write(30);
  delay(1000);
  lastStateCLK = digitalRead(CLK);
}
double i = 0;
double i_past =0;

void loop() 
{
int st=50;
    
  // CLK핀의 상태를 확인
  currentStateCLK = digitalRead(CLK);

  // CLK핀의 신호가 바뀌었고(즉, 로터리엔코더의 회전이 발생했했고), 그 상태가 HIGH이면(최소 회전단위의 회전이 발생했다면) 
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

    // DT핀의 신호를 확인해서 엔코더의 회전 방향을 확인함.

    if (digitalRead(DT) != currentStateCLK) {  // 신호가 다르다면 시계방향 회전
      counter ++;                           // 카운팅 용 숫자 1 증가
      currentDir ="시계방향 회전";
    } else {                                 // 신호가 같다면 반시계방향 회전
      counter --;                         // 카운팅 용 숫자 1 감소
      currentDir ="반시계 방향 회전";
    }
   
  }

  // 현재의 CLK상태를 저장
  lastStateCLK = currentStateCLK;
 Serial.println(counter);           // 회전 카운팅 출력

 double e = 0 - counter;
 double pos = 0.6*e + 0.01*(i_past+e*0.00001);

  myservo.write(constrain(st+pos,0,120));
  i_past = i_past+e*0.00001;
  delay(1);
  

    
}
