#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include "sensors.h"

#define F_sensor 3
#define R_sensor 4
#define L_sensor 2
 
void initializeSensors(){
  //  start ultrasonic Pins Mode //
  pinMode(F_sensor , INPUT);
  pinMode(R_sensor , INPUT);
  pinMode(L_sensor , INPUT);

  // //  end  ultrasonic Pins Mode //

}


//////////////////////// start mpu6050    ////////////
// Define MPU object
Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;

float AngleZ = 0, ZError = 0;

/* Integration variables */
float LastTime = 0, Time, dt, Area;

void startgyro(){
    // Try to initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);

  // Calibrate gyro for z-axis drift error
    for (int i = 0; i < 200; ++i) {
    mpu.getEvent(&a, &g, &temp);
    ZError += g.gyro.z;
    }
  ZError /= 200.0;

  // Initialize timing
  LastTime = millis();
}


void ReadGyro() {
  mpu.getEvent(&a, &g, &temp);
  Time = millis();
  dt = (Time - LastTime) / 1000.0;  // Convert to seconds
  Area = (g.gyro.z - ZError) * dt * 180 / PI;  // Convert radians/sec to degrees
  if (fabs(Area) >.01) {
    AngleZ += Area;  // Accumulate the yaw angle
  }
  LastTime = Time;

  // delay(10);
}
//////////////////////// end mpu6050    ////////////

///////// Start IR ////////////

bool wallFront(){
  if(!digitalRead(F_sensor)){
    return true;
  }
  return false;
}

bool wallRight(){
  if(!digitalRead(R_sensor)){
    return true;
  }
  return false;
}

bool wallLeft(){
  if(!digitalRead(L_sensor)){
    return true;
  }
  return false;
}
//////// End IR   /////////////

