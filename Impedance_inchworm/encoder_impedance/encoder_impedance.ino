
int count = 0;
void setup() 
{
 Serial.begin(9600);
 pinMode(36, INPUT_PULLUP);
 attachInterrupt(digitalPinToInterrupt(36), COUNT, RISING);
}

void loop() 
{
  if(digitalRead(36)==HIGH)
  {
    Serial.println("1");
  }
  Serial.println(count);
  delay(100);
}

void COUNT()
{
  count++; 
}
