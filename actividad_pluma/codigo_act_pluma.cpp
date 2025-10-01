// C++ code
#include <Servo.h>
const int trigg = 6;
const int echo = 7;
const int led1 = 12;
const int led2 = 11;

Servo pluma;

long duracion;
int distancia;

void setup()
{
  pinMode(trigg, OUTPUT);
  pinMode(echo, INPUT);
  pluma.attach(3);
  pluma.write(0);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  Serial.begin(9600);
  
}

void loop()
{
  digitalWrite(trigg, LOW);
  delayMicroseconds(2);
  digitalWrite(trigg, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigg, LOW);
  
  duracion = pulseIn(echo, HIGH);
  distancia = duracion * 0.0175;

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  if (distancia <= 30) {
    pluma.write(90);
    digitalWrite(led2, HIGH);
    digitalWrite(led1, LOW);
    delay(5000);
  } else {
    pluma.write(0);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
  }
}
