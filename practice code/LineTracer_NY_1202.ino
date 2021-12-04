#include "Arduino.h"

#define IN1 4//5 
#define IN2 5//6
#define IN3 6//10
#define IN4 7//11
int speed = 50;


//int sensor = 9;
int leftLineSensor=3;//12
int rightLineSensor=2;//13

void forward(){
  digitalWrite(IN1, 1);
  digitalWrite(IN2, 0);
  digitalWrite(IN3, 1);
  digitalWrite(IN4, 0);
}

void back(){
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 1);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 1);
}

void left(){
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 0);
  digitalWrite(IN3, 1);
  digitalWrite(IN4, 0);
}

void right(){
  digitalWrite(IN1, 1);
  digitalWrite(IN2, 0);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 0);
}

void stop(){
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 0);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 0);
}

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(leftLineSensor, INPUT);
  pinMode(rightLineSensor, INPUT);
  analogWrite(IN1, 1);
  analogWrite(IN2, 1);
  analogWrite(IN3, 1);
  analogWrite(IN4, 1);
  //pinMode(IN1,OUTPUT);
  //pinMode(IN2,OUTPUT);
  //pinMode(IN3,OUTPUT); 
  //pinMode(IN4,OUTPUT);
}
 void loop() {
  if (digitalRead(leftLineSensor) && digitalRead(rightLineSensor)) {
    forward();
  }

  else if (!digitalRead(leftLineSensor) && digitalRead(rightLineSensor)) {
    left();
  }

  else if (digitalRead(leftLineSensor) && !digitalRead(rightLineSensor)) {
    right();
  }
  else if (!digitalRead(leftLineSensor) && !digitalRead(rightLineSensor)) {
    stop();
  }


}
