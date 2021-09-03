/*
    RoboWar.h library for Nomada-e RoboWar motor driver
    Created by ImagineTitanz December 6, 2020
    Property of Paragon Roboticz
*/

#ifndef RoboWar_h
#define RoboWar_h

#include <Arduino.h>

class RoboWar
{
private:
  int _current_pin;
  int _voltaje_pin;

public:
  RoboWar(int current_pin, int voltaje_pin);
  int checkCurrent();
  int checkVoltaje();
};

class Motor
{
public:
  Motor(int motor_1_pin, int motor_2_pin, int motor_velocity_pin);
  void forwardDrive(int velocity);
  void backwardDrive(int velocity);

private:
  int _motor_1_pin;
  int _motor_2_pin;
  int _motor_velocity_pin;
};

#endif