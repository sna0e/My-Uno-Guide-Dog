const byte s0_pin = 8;
const byte s1_pin = 44;
const byte s2_pin = 45;
const byte s3_pin = 46;
const byte out_pin = 9;

void setup() {
  Serial.begin(9600);
  Serial.println("MicroFun_Kit");
  Serial.println("MFK_052");
  Serial.println("START..");

  pinMode(s0_pin, OUTPUT);
  pinMode(s1_pin, OUTPUT);
  pinMode(s2_pin, OUTPUT);
  pinMode(s3_pin, OUTPUT);
  
  pinMode(out_pin, INPUT);

  digitalWrite(s0_pin, HIGH);
  digitalWrite(s1_pin, LOW);
}

void loop() {

  //Red Color Read
  digitalWrite(s2_pin, LOW);
  digitalWrite(s3_pin, LOW);
  int RED_Color = pulseIn(out_pin, LOW);
  RED_Color = map(RED_Color, 72, 25, 0, 255);
  RED_Color = constrain(RED_Color, 255, 0);
  delay(500);

  //Green Color Read
  digitalWrite(s2_pin, HIGH);
  digitalWrite(s3_pin, HIGH);
  int GREEN_Color = pulseIn(out_pin, LOW);
  GREEN_Color = map(GREEN_Color, 90, 30, 0, 255);
  GREEN_Color = constrain(GREEN_Color, 255, 0);
  delay(500);

  //Blue Color Read
  digitalWrite(s2_pin, HIGH);
  digitalWrite(s3_pin, LOW);
  int BLUE_Color = pulseIn(out_pin, LOW);
  BLUE_Color = map(BLUE_Color, 70, 25, 0, 255);
  BLUE_Color = constrain(BLUE_Color, 255, 0);
  delay(500);
}
