#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySerial(10, 11);//(4,2)

byte voice_recogn = 0;

SoftwareSerial mySoftwareSerial(12, 13); // RX, TX // 검정색 = 9 => 12, 보라색 = 3 =>13
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(9600);//음성인식
  mySerial.begin(9600);//음성인식
  //Serial.begin(115200);
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
   
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(20);  //Set volume value. From 0 to 30
  //myDFPlayer.play(1);  //Play the first mp3



  // Serial.begin(9600); // 통신 속도 9600bps로 PC와 시리얼 통신 시작
  //mySerial.begin(9600); // 통신 속도 9600bps로 모듈과 시리얼 통신 시작

  Serial.println("wait settings are in progress");
  delay(1000);
  mySerial.write(0xAA); // compact mode 사용
  mySerial.write(0x37);
  delay(1000);
  mySerial.write(0xAA); // 그룹1 음성 명령어 imported
  mySerial.write(0x21);
  
  Serial.println("The settings are complete"); 
}

void loop()
{
  
  while(mySerial.available()&&myDFPlayer.available())
  {
    Serial.println("voice input");
    voice_recogn=mySerial.read();

   switch(voice_recogn)
   {
     case 0x11:
      myDFPlayer.play(1);
     //Serial.println("voice recogn success : Start");
      break;
     
     case 0x12:
      myDFPlayer.play(2);
      //Serial.println("voice recogn success : Bathroom");
      break;

     case 0x13:
      myDFPlayer.play(3);
      //Serial.println("voice recogn success : Bedroom");
      break;
     
     case 0x14:
      myDFPlayer.play(4);
      //Serial.println("voice recogn success : Livingroom");
      break; 
          
     case 0x15:
      myDFPlayer.play(5);
      //Serial.println("voice recogn success : Kitchen");
      break;
  }
 }
}
  
  /*
  static unsigned long timer = millis();
  
  if (millis() - timer > 3000) {
    timer = millis();
    myDFPlayer.next();  //Play next mp3 every 3 second.
  }
  
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}
*/
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
