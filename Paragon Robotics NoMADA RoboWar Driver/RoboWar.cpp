/*
    RoboWar.h library for Nomada-e RoboWar motor driver control
    Created by ImagineTitanz December 6, 2020
    Property of Paragon Roboticz
*/

#include "Arduino.h"
#include "RoboWar.h"

RoboWar::RoboWar(int current_pin, int voltaje_pin)
{
  pinMode(current_pin, INPUT);
  pinMode(voltaje_pin, INPUT);
  _current_pin = current_pin;
  _voltaje_pin = voltaje_pin;
}

int RoboWar::checkCurrent() {
  int current_value;
  current_value = analogRead(_current_pin);
  return current_value;
}

int RoboWar::checkVoltaje() {
  int voltaje_value;
  voltaje_value = analogRead(_voltaje_pin);
  return voltaje_value;
}

Motor::Motor(int motor_1_pin, int motor_2_pin, int motor_velocity_pin)
{
  pinMode(motor_1_pin, OUTPUT);
  pinMode(motor_2_pin, OUTPUT);
  pinMode(motor_velocity_pin, OUTPUT);
  _motor_1_pin = motor_1_pin;
  _motor_2_pin = motor_2_pin;
  _motor_velocity_pin = motor_velocity_pin;
}

void Motor::forwardDrive(int velocity)
{
  digitalWrite(_motor_1_pin, HIGH);
  digitalWrite(_motor_2_pin, LOW);
  analogWrite(_motor_velocity_pin, velocity);
}

void Motor::backwardDrive(int velocity)
{
  digitalWrite(_motor_1_pin, LOW);
  digitalWrite(_motor_2_pin, HIGH);
  analogWrite(_motor_velocity_pin, velocity);
}