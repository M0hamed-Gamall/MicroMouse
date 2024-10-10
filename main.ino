#include "motors.h"
#include "sensors.h"
#include "algorithm.h"

void setup(){
  Serial.begin(9600);
  initializeMotors();
  initializeSensors();
  startgyro();
  ReadGyro();

  pinMode(13 , OUTPUT);
}

void loop(){

  set_walls();
  floodfill();
  move();
  reintialize_vis();

 
}


