#include <Arduino.h>
#include "motors.h"
#include "sensors.h"


#define ENA 5  // Rigth motor
#define in1 10
#define in2 9

#define ENB 6  // Left motor
#define in3 12
#define in4 11

// start encoder   Pins //
// #define encoder1 7
#define encoder 8
// end encoder   Pins //


// extern float AngleZ;
float target_angle = 0;
// float gyroRatio =1.0 ;


int max_RightSpeed = 56;
int max_LeftSpeed = 60;
int curr_RightSpeed = 47;
int curr_LeftSpeed = 52;
int turn_RightSpeed = 40;
int turn_LeftSpeed = 40;

//// encoder  ////// 
int counter = 0;
int state = 0;
int last_state = 0;


float last_error = 0;
float kp = .4; // .4
float kd = 1;  // 1
float error = 0; 
int change = 0;


void initializeMotors(){
  pinMode(ENA , OUTPUT);
  pinMode(ENB , OUTPUT);

  pinMode(in1 , OUTPUT);
  pinMode(in2 , OUTPUT);
  pinMode(in3 , OUTPUT);
  pinMode(in4 , OUTPUT);

  pinMode(encoder , INPUT);

}

void moveForward(){
  if(wallFront()){
    stop();
    return;
  }
	// to push motors
  digitalWrite(13 , HIGH);
  // delay(10);
  analogWrite(ENA , 90);       // Right motor
  digitalWrite(in1 , HIGH);
  digitalWrite(in2 , LOW);

  analogWrite(ENB , 110);       //Left motor
  digitalWrite(in3 , HIGH);
  digitalWrite(in4 , LOW);
  delay(10);

  // analogWrite(ENA , curr_RightSpeed); 
  // analogWrite(ENB , curr_LeftSpeed); 

  while( counter < 60){
      if(digitalRead(encoder) == 1){
          state = 1;
      }
      else{
          state = 0;
      }

      counter += state ^ last_state;
      last_state = state;

      // Serial.print("counter : ");
      // Serial.println(counter);

        ReadGyro();
        error = target_angle - AngleZ;

        change = kp * error + kd * (error - last_error);


        change_RightSpeed(change);
        change_LeftSpeed(-change);
        last_error = error;
            if(wallFront()){
              break;
            }
            digitalWrite(13 , LOW);
      }
      
      counter=0;
      // stop(); 
}
void stop(){
  analogWrite(ENA , 0); 
  analogWrite(ENB , 0); 

}
void turnRight(){
	// start motors
  analogWrite(ENA , 100);       // Right motor
  digitalWrite(in1 , LOW);
  digitalWrite(in2 , HIGH);

  analogWrite(ENB , 100);       //Left motor
  digitalWrite(in3 , HIGH);
  digitalWrite(in4 , LOW);

  delay(10);

  target_angle -=88.0;

  while(abs(target_angle - AngleZ)  > 16){
    analogWrite(ENA , turn_RightSpeed);       // Right motor
    analogWrite(ENB , turn_LeftSpeed);       //Left motor
      ReadGyro();
      
  }
  stop();
}

void turnLeft(){
	// start motors
  analogWrite(ENA , 100);       // Right motor
  digitalWrite(in1 , HIGH);
  digitalWrite(in2 , LOW);

  analogWrite(ENB , 100);       // Left motor
  digitalWrite(in3 , LOW);
  digitalWrite(in4 , HIGH);

  delay(10);

  target_angle +=88.0;

    while(abs(target_angle - AngleZ)  > 16){
    analogWrite(ENA , turn_RightSpeed);       // Right motor
    analogWrite(ENB , turn_LeftSpeed);       //Left motor
      ReadGyro();
  }

  stop();
}

void change_RightSpeed(int change){
  curr_RightSpeed += change;
  curr_RightSpeed = max(curr_RightSpeed , 47 );
  curr_RightSpeed = min(curr_RightSpeed , max_RightSpeed );
  analogWrite(ENA , curr_RightSpeed); 
}

void change_LeftSpeed(int change){
  curr_LeftSpeed += change;
  curr_LeftSpeed = max(curr_LeftSpeed , 52 );
  curr_LeftSpeed = min(curr_LeftSpeed , max_LeftSpeed );
  analogWrite(ENB , curr_LeftSpeed); 
}

