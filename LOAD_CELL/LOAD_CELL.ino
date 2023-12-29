#include "HX711.h"
#define factor -7050.0
#define DOUT 3
#define CLK 2
HX711 scale(DOUT, CLK);

void setup() 
{
  Serial.begin(9600);
  scale.set_scale(factor);
  scale.tare();
}

void loop() 
{
  Serial.println(scale.get_units()*-97.23787364, 5); //g
}
