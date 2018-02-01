#include "SensorsDRV.h"
#include <Arduino.h>

Ultrasonic::Ultrasonic(uint8_t Trig_Pin, uint8_t Echo_Pin)
{
  _Trig_Pin = Trig_Pin;
  _Echo_Pin = Echo_Pin;
  Distance = 0;
  Time = 0;
  pinMode(_Echo_Pin, INPUT);
  pinMode(_Trig_Pin, OUTPUT);
}

float Ultrasonic::Detect()  //Note the distance is in CM
{
  digitalWrite(_Trig_Pin, LOW); 
  delayMicroseconds(5);
  digitalWrite(_Trig_Pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_Trig_Pin, LOW);   
  Time = pulseIn(_Echo_Pin, HIGH);
  //if (Distance == 0)Distance = 5000;
  Distance = (Time/2)/29.1;
  Serial.println(Distance);
  delay(100);
  return Distance;
}



