#include <RoboWar.h>

//Comentado por repetición de pines con sensores
//RoboWar RoboWarJapanSumo(A1, A2); // Corriente y voltaje
Motor MotorA(12, 8, 11); // Pines de control y PWM
Motor MotorB(7, 2, 10);  // Pines de control y PWM

bool cincosegundos = false;


int side_right_sensor = A1;
int side_left_sensor = A2;
int front_center_sensor = A3;
int front_left_sensor = A4;
int front_right_sensor = A5;

int lineR = 0;
int lineL = 1; // Sensores de piso

unsigned int right_measurement;
unsigned int left_measurement;
unsigned int acute_measurement;
unsigned int center_measurement;
unsigned int obtuse_measurement;

int dip1 = 6, dip2 = 9, dip3 = 5, dip4 = 3; // Dip switch
int vel;
int lectura1,lectura2,lectura3,lectura4;

int buzzer = 4;

int arrancador = A0;
int lectura = 2;

int led = 13;

bool inicio = false;
bool estado = false;

int contador = 0;

void setup() {

pinMode(dip1,INPUT);
pinMode(dip2,INPUT);
pinMode(dip3,INPUT);
pinMode(dip4,INPUT); 
pinMode(lineR,INPUT);
pinMode(lineL,INPUT); 

pinMode(buzzer,OUTPUT);
pinMode(led,OUTPUT);

lectura1 = digitalRead(dip1);
lectura2 = digitalRead(dip2);
lectura3 = digitalRead(dip3);
lectura4 = digitalRead(dip4);

if(lectura1 == LOW && lectura2 == HIGH && lectura3 == HIGH && lectura4 == HIGH){

  vel = 50;
}

if(lectura1 == HIGH && lectura2 == LOW && lectura3 == HIGH && lectura4 == HIGH){

  vel = 80;
}

if(lectura1 == HIGH && lectura2 == HIGH && lectura3 == LOW && lectura4 == HIGH){

  vel = 100;
}

if(lectura1 == HIGH && lectura2 == HIGH && lectura3 == HIGH && lectura4 == LOW){

  vel = 120;
}




}

void loop() {


  
 lectura = digitalRead(arrancador);

 if(lectura == HIGH){

  while(cincosegundos == false){

    delay(5000);
    cincosegundos = true;
  }
  
  readSensorData();
  

 if(lectura == HIGH && center_measurement < 20 ){ // Cambiar a 50
  
  MotorA.forwardDrive(vel);
  MotorB.forwardDrive(vel);
  digitalWrite(led,HIGH);
  lectura = digitalRead(arrancador);
 }

 else if(lectura == HIGH && right_measurement < 20){ // Cambiar a 50

  MotorA.backwardDrive(vel);
  MotorB.forwardDrive(vel);
  digitalWrite(led,HIGH);
  lectura = digitalRead(arrancador);
 }

 
 else if(lectura == HIGH && left_measurement < 20){ // Cambiar a 50

  MotorA.forwardDrive(vel);
  MotorB.backwardDrive(vel);
  digitalWrite(led,HIGH);
  lectura = digitalRead(arrancador);
 }

 else{
  digitalWrite(led,LOW);
  //search();
  
 }
 
 }

 else{
  
  digitalWrite(led,LOW);
  MotorA.forwardDrive(0);
  MotorB.forwardDrive(0);
  
 }
 
 

}

void readSensorData()
{
  
  center_measurement = 187754 * pow(analogRead(front_center_sensor), -1.51);
  delay(10);
  acute_measurement = 187754 * pow(analogRead(front_left_sensor), -1.51);
  delay(10);
  obtuse_measurement = 187754 * pow(analogRead(front_right_sensor), -1.51);
  delay(10);
  right_measurement = 17569.7 * pow(analogRead(side_right_sensor), -1.2062);
  delay(10);
  left_measurement = 17569.7 * pow(analogRead(side_left_sensor), -1.2062);
  delay(10);
  
}

void search(){
  
 if(contador < 5){

  MotorA.forwardDrive(vel);
  MotorB.backwardDrive(vel);
  delay(500);
  MotorA.forwardDrive(0);
  MotorB.forwardDrive(0);
  contador++;
  
 }

 else if(contador >=5){
  
 MotorA.forwardDrive(vel);
 MotorB.forwardDrive(vel);
 delay(500);
 MotorA.forwardDrive(0);
 MotorB.forwardDrive(0);
 contador = 0;
 }

  
  
}
