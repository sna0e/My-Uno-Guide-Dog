#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// 라인 트레이서 //
#define IN1 4//5 
#define IN2 5//6
#define IN3 6//10
#define IN4 7//11
int speed = 50;

int leftLineSensor=3;//12
int rightLineSensor=2;//13

// RGB 컬러 센서 //
const byte s0_pin = 8;
const byte s1_pin = 44;
const byte s2_pin = 45;
const byte s3_pin = 46;
const byte out_pin = 9;

// 음성인식 출력 //
SoftwareSerial mySerial(10, 11);//(4,2)
byte voice_recogn = 0;
byte voice_recogn1 = 0;

SoftwareSerial mySoftwareSerial(12, 13); // RX, TX // 검정색 = 9 => 12, 보라색 = 3 => 13
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
    Serial.println(F("Time Out!"));
    break;
    
    case WrongStack:
    Serial.println(F("Stack Wrong!"));
    break;
    
    case DFPlayerCardInserted:
    Serial.println(F("Card Inserted!"));
    break;
    
    case DFPlayerCardRemoved:
    Serial.println(F("Card Removed!"));
    break;
    
    case DFPlayerCardOnline:
    Serial.println(F("Card Online!"));
    break;
    
    case DFPlayerUSBInserted:
    Serial.println("USB Inserted!");
    break;
    
    case DFPlayerUSBRemoved:
    Serial.println("USB Removed!");
    break;
    
    case DFPlayerPlayFinished:
    Serial.print(F("Number:"));
    Serial.print(value);
    Serial.println(F(" Play Finished!"));
    break;
    
    case DFPlayerError:
    Serial.print(F("DFPlayerError:"));
    switch (value) {
      case Busy:
      Serial.println(F("Card not found"));
      break;
      
      case Sleeping:
      Serial.println(F("Sleeping"));
      break;
      
      case SerialWrongStack:
      Serial.println(F("Get Wrong Stack"));
      break;
      
      case CheckSumNotMatch:
      Serial.println(F("Check Sum Not Match"));
      break;
      
      case FileIndexOut:
      Serial.println(F("File Index Out of Bound"));
      break;
      
      case FileMismatch:
      Serial.println(F("Cannot Find File"));
      break;
      
      case Advertise:
      Serial.println(F("In Advertise"));
      break;
      
      default:
      break;
    }
    break;
    
    default:
    break;
  }
}

void navigation()
{
  if (digitalRead(leftLineSensor) && digitalRead(rightLineSensor)) {
    //forward();
    digitalWrite(IN1, 1);
    digitalWrite(IN2, 0);
    digitalWrite(IN3, 1);
    digitalWrite(IN4, 0);
  }

  else if (!digitalRead(leftLineSensor) && digitalRead(rightLineSensor)) {
    //left();
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 0);
    digitalWrite(IN3, 1);
    digitalWrite(IN4, 0);
  }

  else if (digitalRead(leftLineSensor) && !digitalRead(rightLineSensor)) {
    //right();
    digitalWrite(IN1, 1);
    digitalWrite(IN2, 0);
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 0);
  }
  else if (!digitalRead(leftLineSensor) && !digitalRead(rightLineSensor)) {
    //stop();
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 0);
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 0);
  }
}

void turn()
{
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 1);
  digitalWrite(IN3, 1);
  digitalWrite(IN4, 0);
}

void setup()
{
  Serial.begin(9600);
  
  // 음성인식 출력 //
  mySoftwareSerial.begin(9600);
  mySerial.begin(9600);
  myDFPlayer.volume(20);

  delay(1000);
  mySerial.write(0xAA); // compact mode 사용
  mySerial.write(0x37);
  delay(1000);
  mySerial.write(0xAA); // 그룹1 음성 명령어 imported
  mySerial.write(0x21);
  
  Serial.println("The settings are complete");

  // RGB 컬러 센서 //
  pinMode(s0_pin, OUTPUT);
  pinMode(s1_pin, OUTPUT);
  pinMode(s2_pin, OUTPUT);
  pinMode(s3_pin, OUTPUT);
  pinMode(out_pin, INPUT);

  digitalWrite(s0_pin, HIGH);
  digitalWrite(s1_pin, LOW);

  // 라인 트레이서 //
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

void loop()
{
  //Red Color Read
  digitalWrite(s2_pin, LOW);
  digitalWrite(s3_pin, LOW);
  int RED_Color = pulseIn(out_pin, LOW);
  RED_Color = map(RED_Color, 72, 25, 0, 255);
  RED_Color = constrain(RED_Color, 255, 0);

  //Green Color Read
  digitalWrite(s2_pin, HIGH);
  digitalWrite(s3_pin, HIGH);
  int GREEN_Color = pulseIn(out_pin, LOW);
  GREEN_Color = map(GREEN_Color, 90, 30, 0, 255);
  GREEN_Color = constrain(GREEN_Color, 255, 0);

  //Blue Color Read
  digitalWrite(s2_pin, HIGH);
  digitalWrite(s3_pin, LOW);
  int BLUE_Color = pulseIn(out_pin, LOW);
  BLUE_Color = map(BLUE_Color, 70, 25, 0, 255);
  BLUE_Color = constrain(BLUE_Color, 255, 0);
  
  while(mySerial.available()&&myDFPlayer.available())
  {
    //Serial.println("voice input");
    voice_recogn=mySerial.read(); // 시작 입력 받을 준비

    if(voice_recogn == 0x11)
    {
      myDFPlayer.play(1); // 시작 안내 메시지 & 장소 입력 요청

      voice_recogn1=mySerial.read(); // 장소 입력 받을 준비
      switch(voice_recogn1)
      {
        case 0x12:  // "화장실 (검정)"
        myDFPlayer.play(2); // 화장실로 안내 시작 메시지
        // 색상 설정 코드 (검정)
        navigation();
        myDFPlayer.play(4); // 도착 안내 메시지
        delay(3000);
        turn();
        break;

        case 0x13:  // "안방 (파랑)"
        myDFPlayer.play(3); // 안방으로 안내 시작 메시지
        // 색상 설정 코드 (파랑)
        navigation();
        myDFPlayer.play(4); // 도착 안내 메시지
        delay(3000);
        turn();
        break;
      }
    }

    /*
    if(voice_recogn == 0x14)  // 돌아가자고 할 때?
    {
      myDFPlayer.play(5);
      navigation();
      myDFPlayer.play(4);
      turn();
    }
    */
  }
}
