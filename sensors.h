#ifndef SENSOR_H
#define SENSOR_H

extern float AngleZ;

void initializeSensors();
void startgyro();
void ReadGyro();

bool wallFront();
bool wallRight();
bool wallLeft();






#endif  // SENSOR_H