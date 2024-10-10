#ifndef MOTORS_H
#define MOTORS_H

extern int maxSpeed;
extern int curr_RightSpeed;
extern int curr_LeftSpeed;
extern float target_angle;
extern int counter1 ;
extern int counter2;

void initializeMotors();
void moveForward();
void turnRight();
void turnLeft();
void stop();

void change_RightSpeed(int change);
void change_LeftSpeed(int change);

#endif  // MOTORS_H