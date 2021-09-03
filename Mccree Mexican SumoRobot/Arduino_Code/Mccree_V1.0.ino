#include <Arduino.h>
#include <RoboWar.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#define MODE 2

RoboWar RoboWarJapanSumo(3, 4);
Motor MotorA(12, 11, 10);
const int max_speed = 254;
unsigned int robo_war_current;
unsigned int robo_war_voltaje;

SoftwareSerial nextionSerial(3, 5);
void readNextionSerialData();
void writeIntIntoEEPROM(int address, int number);
int readIntFromEEPROM(int address);
void runSelectedRoutine(int routine_to_play);
bool routine_ready = false;
int routine_to_play;

bool start_signal = false;

void setup()
{
  nextionSerial.begin(9600);
  if (MODE == 0)
  {
    Serial.begin(9600);
    Serial.println("Mexican SumoRobot 'Drongo'");
    Serial.println("Start up values:");
    Serial.println(RoboWarJapanSumo.checkCurrent());
    Serial.println(RoboWarJapanSumo.checkVoltaje());
  }
}

void loop()
{
  while (routine_ready == false)
  {
    readNextionSerialData();
    if (routine_ready == true)
    {
      routine_to_play = readIntFromEEPROM(45);
      nextionSerial.println(routine_to_play); //Poner en verde pantalla nextion
    }
  }
  while (start_signal == false)
  {
    if (digitalRead(4) == HIGH) //Colocar pin del arrancador
    {
      start_signal = true;
    }
  }

  runSelectedRoutine(routine_to_play);

}

void runSelectedRoutine(int routine_to_play)
{
}

void measureWastedTime()
{
  // Set up Timer 1 to count every single clock cycle
  TCCR1A = 0;
  TCCR1B = bit(CS10);
  TCNT1 = 0;

  /* Code under test */

  /* End of code under test */

  // Print the result
  unsigned int value = TCNT1;
  Serial.print("Cycles: ");
  Serial.println(value - 1);
  Serial.print("Microseconds: ");
  Serial.println((float)(value - 1) / 16);
}

void readNextionSerialData()
{
  String data_from_nextion = "";
  if (nextionSerial.available() > 0)
  {
    delay(30);
    while (nextionSerial.available())
    {
      data_from_nextion += char(nextionSerial.read());
    }
    if (data_from_nextion.substring(0, 8) == "routine:")
    {
      int new_routine = data_from_nextion.substring(8).toInt();
      writeIntIntoEEPROM(45, new_routine);
      routine_ready = true;
    }
    delay(20);
  }
}

void writeIntIntoEEPROM(int address, int number)
{
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}

int readIntFromEEPROM(int address)
{
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}
