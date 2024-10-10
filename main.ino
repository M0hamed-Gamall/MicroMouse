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

  moveForward();



  // Serial.print("Yaw (Z-axis): ");
  // Serial.println(AngleZ );

//  if(!wallFront())
//  {
//   moveForward();
//  }else if(wallFront() && wallRight() && wallLeft()){
//   turnRight();
//   turnRight();
//  }else if(wallFront() && wallRight()){
//   turnLeft();
//  }else if(wallFront() && wallLeft()){
//   turnRight();
//  }
//  else if(wallFront()){
//   turnRight();
//  }


 
}


