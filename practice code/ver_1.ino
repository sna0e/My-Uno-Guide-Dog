#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "VoiceRecognitionV3.h"
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7
int speed = 50;
const byte s0_pin=8;
const byte s1_pin=44;
const byte s2_pin=45;
const byte s3_pin=46;
const byte out_pin=9;
int leftLineSensor=3;
int rightLineSensor=2;


//음성인식에 필요
SoftwareSerial mySerial(10,11);
byte voice_recogn=0;

SoftwareSerial mySoftwareSerial(12, 13); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void Run(){
  
if(digitalRead(leftLineSensor)==LOW)&&digitalRead(rightSensor)==HIGH)){ //RIGHT
    digitalWrite(IN1, speed);
    digitalWrite(IN2, 0);
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 0);
   }
   else if(digitalRead(leftLineSensor)==HIGH)&&digitalRead(rightSensor)==LOW)){ //LEFT
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 0);
    digitalWrite(IN3, speed);
    digitalWrite(IN4, 0);
   }
   else if(digitalRead(leftLineSensor)==HIGH)&&digitalRead(rightSensor)==HIGH)){ //FORWARD
    digitalWrite(IN1, speed);
    digitalWrite(IN2, 0);
    digitalWrite(IN3, speed);
    digitalWrite(IN4, 0);
   }
   else if(digitalRead(leftLineSensor)==LOW)&&digitalRead(rightSensor)==LOW)){ //STOP
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 0);
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 0);
   }
}

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

void setup() {
  // put your setup code here, to run once:
  //적외선 감지 센서 set
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(leftLineSensor, INPUT);
  pinMode(rightLineSensor, INPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT); 
  pinMode(IN4,OUTPUT);

  //MP3 모듈 센서 set
  mySoftwareSerial.begin(9600);
  Serial.begin(9600);//음성인식
  mySerial.begin(9600);//음성인식

  //if(!myDFPlayer.begin(mySoftwareSerial)){}
  myDFPlayer.volume(15);

  //음성인식 모듈 set
  //pinMode(relay, OUTPUT);
  delay(1000);
  mySerial.write(0XAA);
  mySerial.write(0x37);
  delay(1000);
  mySerial.write(0xAA);
  mySerial.write(0x21);

  Serial.println("The settings are complete");

  //RGB센서 set
  pinMode(s0_pin, OUTPUT);
  pinMode(s1_pin, OUTPUT);
  pinMode(s2_pin, OUTPUT);
  pinMode(s3_pin, OUTPUT);
  pinMode(out_pin, INPUT);

  digitalWrite(s0_pin, HIGH);
  digitalWrite(s1_pin, LOW);
}

void loop() {
  digitalWrite(s2_pin, LOW);//안방에 해당하는 테이프 색상으로 길안내
  digitalWrite(s3_pin, LOW);
  int RED_Color = pulseIn(out_pin, LOW);
  RED_Color=map(RED_Color, 72, 25, 0, 255);
  RED_Color=constrain(RED_Color, 255, 0);

  digitalWrite(s2_pin, HIGH);
  digitalWrite(s3_pin, HIGH);]
  int GREEN_Color=pulseIn(out_pin, LOW);
  GREEN_Color=map(GREEN_Color, 90,30, 0, 255);
  GREEN_Color=constrain(GREEN_Color, 255, 0);

  digitalWrite(s2_pin, HIGH);
  digitalWrite(s3_pin, LOW);
  int BLUE_Color=pulseIn(out_pin, LOW);
  BLUE_Color=map(BLUE_Color, 70, 25, 0, 255);
  BLUE_Color=constrain(BLUE_Color, 255, 0);
  
  //switch(state){
  //case 0:  //음성인식으로 목적지 입력받는 단계
    while(mySerial.available()&&myDFPlayer.available())
    {
      voice_recogn=mySerial.read();
      
     switch(voice_recogn)
     {
      case 0x11://시작
        myDFPlayer.play(1);//시작 안내 메시지& 장소 입력 요청
        break;
        
      case 0x12://화장실(검정색)
        myDFPlayer.play(2);//화장실로 안내 메시지
          Run();
        myDFPlayer.play(4);//도착 안내 메시지//근데 이렇게 하면 Run이 다 제대로 끝난 후에 도착안내 메시지가 나올지..
        digitalWrite(IN1, 0);//차체를 180도 돌리는 코드
        digitalWrite(IN2, speed);
        digitalWrite(IN3, speed);
        digitalWrite(IN4, 0);
        break;
        
      case 0x13://안방 (빨간색)
        myDFPlayer.play(3);//안방으로 안내 메시지
        (s2_pin, LOW);//안방에 해당하는 테이프 색상으로 길안내
        if(RED_Color==0 && GREEN_Color==255 && BLUE_Color==0){
          Run();
        }
        myDFPlayer.play(4);//도착 안내 메시지
        digitalWrite(IN1, 0);
        digitalWrite(IN2, speed);
        digitalWrite(IN3, speed);
        digitalWrite(IN4, 0);
        break;

      default:
      Serial.println("음성을 인식하지 못하였습니다.");
      break;
     }
}
}
