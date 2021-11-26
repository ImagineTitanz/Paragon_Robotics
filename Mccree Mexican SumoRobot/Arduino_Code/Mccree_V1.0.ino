#include <RoboWar.h>

/*Comentado por repetición de pines con sensores
RoboWar RoboWarJapanSumo(A1, A2); // Corriente y voltaje*/
Motor MotorA(12, 8, 11); // Pines de control y PWM
Motor MotorB(7, 2, 10);  // Pines de control y PWM

const int max_speed = 254;
const int min_speed = 10;
const int safe_speed = 30; // elegir empiricamente para pruebas
const int yellow_speed = 50; // elegir empiricamente
const int red_speed = 200; // elegir empiricamente
const int stops = 0;

unsigned int robo_war_current;
unsigned int robo_war_voltaje;

/*ver diagrama de conexiones Milanote*/
int front_center_sensor = A3;
int front_left_sensor = A2;
int front_right_sensor = A1;
int side_right_sensor = A5;
int side_left_sensor = A0;

/*medición de sensores en cm*/
unsigned int right_measurement;
unsigned int left_measurement;
unsigned int acute_measurement;
unsigned int center_measurement;
unsigned int obtuse_measurement;

/*lineas de inicio de cercanía al robot de cada zona por color*/
const int yellow_radar_line = 80;
const int red_radar_line = 40;
/*indicadores de zona de ubicación del robot oponente*/
bool yellow_zone = false;
bool red_zone = false;

/*definición del dip switch para rutinas de iniciacion*/
int dip_size;
int dip_switch_pins[] = {6, 9, 5, 3};
bool dip_switch_position[] = {}; //posición del dip al iniciar la match

int programmed_routines = 2;
bool routine_1_attack[4] = {1, 1, 0, 0};
bool routine_2_defend[4] = {0, 0, 1, 1};

/*Inicio de cosas de visual studio, tu borras esto ivan*/
int dipSwitchRoutine();
void wiggle();
void yellowZoneAction();
void redZoneAction();
void readSensorData();
/*Final de cosas de visual studio, tu borras esto ivan*/

bool routine_ready = false;
int starting_routine_to_play;
bool routine_played = false;

void setup()
{
  dip_size = sizeof(dip_switch_pins);
  while (routine_ready == false)
  {
    for (int i = 0; i < dip_size; i++)
    {
      dip_switch_position[i] = digitalRead(dip_switch_pins[i]);
    }
    routine_ready = true;
  }
  starting_routine_to_play = dipSwitchRoutine();
}

void loop()
{
  while (routine_played == false)
  {
    switch (starting_routine_to_play)
    {
    case 1:
      /* codigo de rutinas cortas de inicio 1 */
      routine_played = true;
      break;
    case 2:
      /* codigo de rutinas cortas de inicio 2... */
      routine_played = true;
      break;
    default:
      routine_played = true;
      break;
    }
  }
  while (yellow_zone == false)
  {
    wiggle();
  }
  while (red_zone == false)
  {
    yellowZoneAction();
  }
  redZoneAction();
}


void wiggle()
{
  float microseconds;
  // Set up Timer 1 to count every single clock cycle
  TCCR1A = 0;
  TCCR1B = bit(CS10);
  TCNT1 = 0;
  while (microseconds < 500000 || yellow_zone == true)
  {
    MotorA.forwardDrive(safe_speed);
    MotorB.backwardDrive(safe_speed);
    readSensorData();
    unsigned int value = TCNT1; // value of clock cycles
    microseconds = ((float)(value - 1) / 16); // transforming clock cycles in microseconds
  }
  TCCR1A = 0;
  TCCR1B = bit(CS10);
  TCNT1 = 0;
  while (microseconds < 500000 || yellow_zone == true)
  {
    MotorB.forwardDrive(safe_speed);
    MotorA.backwardDrive(safe_speed);
    readSensorData();
    unsigned int value = TCNT1;
    microseconds = ((float)(value - 1) / 16);
  }
}

void readSensorData()
{
  center_measurement = 187754 * pow(analogRead(front_center_sensor), -1.51);
  acute_measurement = 187754 * pow(analogRead(front_left_sensor), -1.51);
  obtuse_measurement = 187754 * pow(analogRead(front_right_sensor), -1.51);
  right_measurement = 17569.7 * pow(analogRead(side_right_sensor), -1.2062);
  left_measurement = 17569.7 * pow(analogRead(side_left_sensor), -1.2062);

  /*green range*/
  if (center_measurement > yellow_radar_line &&
      acute_measurement > yellow_radar_line &&
      obtuse_measurement > yellow_radar_line &&
      right_measurement > yellow_radar_line &&
      left_measurement > yellow_radar_line)
  {
    yellow_zone = false;
    red_zone = false;
  }

  /*yellow range*/
  if (center_measurement < yellow_radar_line ||
      acute_measurement < yellow_radar_line ||
      obtuse_measurement < yellow_radar_line ||
      right_measurement < yellow_radar_line ||
      left_measurement < yellow_radar_line)
  {
    yellow_zone = true;
    red_zone = false;
  }

  /*red range*/
  if (center_measurement < red_radar_line ||
      acute_measurement < red_radar_line ||
      obtuse_measurement < red_radar_line ||
      right_measurement < red_radar_line ||
      left_measurement < red_radar_line)
  {
    red_zone = true;
  }
}

void yellowZoneAction()
{
  readSensorData();
  if (center_measurement < yellow_radar_line) //avanzamos buscando zona roja
  {
    MotorA.forwardDrive(safe_speed);
    MotorB.forwardDrive(safe_speed);
  }
  if (acute_measurement < yellow_radar_line) //volteamos detecha
  {
    MotorA.forwardDrive(safe_speed);
    MotorB.backwardDrive(safe_speed);
  }
  if (right_measurement < yellow_radar_line) //volteamos derecha más rapido (elegir suma empiricamente)
  {
    MotorA.forwardDrive(safe_speed + 20);
    MotorB.backwardDrive(safe_speed + 20);
  }
  if (obtuse_measurement < yellow_radar_line) //volteamos izquierda
  {
    MotorA.backwardDrive(safe_speed);
    MotorB.forwardDrive(safe_speed);
  }
  if (left_measurement < yellow_radar_line) //volteamos izquierda más rapidamente (elegir suma empiricamente)
  {
    MotorA.backwardDrive(safe_speed + 20);
    MotorB.forwardDrive(safe_speed + 20);
  }
}
void redZoneAction()
{
  readSensorData();
  if (center_measurement < red_radar_line &&
      acute_measurement < red_radar_line &&
      obtuse_measurement < red_radar_line) //solo si están los tres sensores frontales en zona roja => max_speed
  {
    MotorA.forwardDrive(safe_speed + 30);
    MotorB.forwardDrive(safe_speed + 30);
  }
  else if (center_measurement < red_radar_line) //avanzamos buscando conectar con robot
  {
    MotorA.forwardDrive(safe_speed);
    MotorB.forwardDrive(safe_speed);
  }
  else if (acute_measurement < red_radar_line) //volteamos detecha
  {
    MotorA.forwardDrive(safe_speed);
    MotorB.backwardDrive(safe_speed);
  }
  else if (right_measurement < red_radar_line) //volteamos derecha más rapido (elegir suma empiricamente)
  {
    MotorA.forwardDrive(safe_speed + 20);
    MotorB.backwardDrive(safe_speed + 20);
  }
  else if (obtuse_measurement < red_radar_line) //volteamos izquierda
  {
    MotorA.backwardDrive(safe_speed);
    MotorB.forwardDrive(safe_speed);
  }
  else if (left_measurement < red_radar_line) //volteamos izquierda más rapidamente (elegir suma empiricamente)
  {
    MotorA.backwardDrive(safe_speed + 20);
    MotorB.forwardDrive(safe_speed + 20);
  }
}

int dipSwitchRoutine()
{
  if (dip_switch_position[0] == routine_1_attack[0] &&
      dip_switch_position[1] == routine_1_attack[1] &&
      dip_switch_position[2] == routine_1_attack[2] &&
      dip_switch_position[3] == routine_1_attack[3])
  {
    return 1;
  }
  if (dip_switch_position[0] == routine_2_defend[0] &&
      dip_switch_position[1] == routine_2_defend[1] &&
      dip_switch_position[2] == routine_2_defend[2] &&
      dip_switch_position[3] == routine_2_defend[3])
  {
    return 2;
  }
}
