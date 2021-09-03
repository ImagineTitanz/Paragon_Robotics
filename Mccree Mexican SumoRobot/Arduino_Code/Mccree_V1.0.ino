#include <Arduino.h>
#include <RoboWar.h>

RoboWar RoboWarJapanSumo(3, 4);
Motor MotorA(12, 11, 10);
const int max_speed = 254;


void setup() {
  Serial.begin(9600);
  Serial.println("Mexican SumoRobot 'Drongo'");
  Serial.println("Start up values:");
  Serial.println(RoboWarJapanSumo.checkCurrent());
  Serial.println(RoboWarJapanSumo.checkVoltaje());
}

void loop() {
  MotorA.forwardDrive(254);
}