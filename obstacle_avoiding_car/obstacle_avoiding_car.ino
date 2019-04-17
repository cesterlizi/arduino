//ARDUINO OBSTACLE AVOIDING CAR//
// Before uploading the code you have to install the necessary library//
//NewPing Library https://github.com/eliteio/Arduino_New_Ping.git //
//Servo Library https://github.com/arduino-libraries/Servo.git //

#include <Servo.h>

#define TRIG_PIN 6
#define ECHO_PIN 7
#define MAX_DISTANCE 200
#define MAX_SPEED_OFFSET 20


/*
 * Moverse
 * 
 * 
 */
// servo
int PINSERVO = 5;
int PULSOMIN = 1500;
int PULSOMAX = 7500;

Servo myservo;

boolean goesForward = false;
int distance = 100;
int distanceR = 0;
int distanceL = 0;

// motors servo
Servo servo_motor1;
Servo servo_motor2;

int speedSet = 0;

int SERVO_MOTOR1 = 9;
int SERVO_MOTOR2 = 10;
int SPEED = 7;
int MAX_SPEED = 7;


// STATUS
const int LAH = 0;
const int MB = 1;
const int MS1 = 2;
const int MF = 3;
const int MS2 = 4;
const int LR = 5;
const int LL = 6;
const int MR = 7;
const int ML = 8;
const int TR = 9;
const int TL = 10;
const int LF = 11;
const int READ_RIGHT = 12;
const int READ_LEFT = 13;

int status = LAH;
boolean wait_mode = false;
long start = 0;


// INTERVALS
const int interval_lah = 100;
const int interval_mb = 500;
const int interval_mf = 50;
const int interval_ms1 = 100;
const int interval_ms2 = 200;
const int interval_look = 1000;
const int interval_turn = 700;

/*
 * 
 * 
 * 
 */

 
/*
 * 
 * Sonido
 * 
 */

const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;

const int silence = 0;
 
const int buzzerPin = 11;
const int ledPin1 = 12;
const int ledPin2 = 13;

const int notes[] = {
  //first
  a,a,a,f,cH,a,f,cH,a,
  silence,
  eH,eH,eH,fH,cH,gS,f,cH,a,
  silence,

  //second
  aH,a,a,aH,gSH,gH,fSH,fH,fSH,
  silence,
  aS,dSH,dH,cSH,cH,b,cH,
  silence,
  
  // variant1
  f, gS, f, a, cH, a, cH, eH,
  silence,
  
  //second
  aH,a,a,aH,gSH,gH,fSH,fH,fSH,
  silence,
  aS,dSH,dH,cSH,cH,b,cH,
  silence,

  //variant2
  f,gS,f,cH,a,f,cH,a,
  silence
  
};
const int intervals[] = {
  //first
  500,500,500,350,150,500,350,150,650,
  500,
  500,500,500,350,150,500,350,150,650,
  500,

  //second
  500,300,150,500,325,175,125,125,250,
  325,
  250,500,325,175,125,125,250,
  350,
  
  //variant1
  250, 500, 350, 125, 500, 375, 125, 650,
  500,

  //second
  500,300,150,500,325,175,125,125,250,
  325,
  250,500,325,175,125,125,250,
  350,
  
  //variant2
  250,500,375,125,500,375,125,650,
  650
};


int toBeep = 0;

boolean sound_wait_mode = false;
long sound_start = 0;

/*
 * 
 * 
 * 
 */

 
void setup()
{
  
  Serial.begin(9600);
  //Setup pin modes
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  
  // motors servo
  servo_motor1.attach(SERVO_MOTOR1, PULSOMIN, PULSOMAX);
  servo_motor2.attach(SERVO_MOTOR2, PULSOMIN, PULSOMAX);

pinMode(TRIG_PIN, OUTPUT);
pinMode(ECHO_PIN, INPUT);
  // distance servo
  myservo.attach(PINSERVO, PULSOMIN, PULSOMAX);
  myservo.write(90);
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}

void loop()
{
  beep(notes[toBeep], intervals[toBeep]);
  andar();
}

void beep(int note, int duration)
{
  if(!sound_wait_mode){
    //Play tone on buzzerPin
    Serial.println(note);
    if(note != silence){
      tone(buzzerPin, note);
    }
    sound_wait_mode = true;
    sound_start = millis();
  }
  long now = millis();
  if (now - sound_start > duration)
  {
    //Stop tone on buzzerPin
    noTone(buzzerPin);
    sound_wait_mode = false;
    if(toBeep == 74){
      toBeep = 0;
    } else {
      toBeep +=1;
    }
    return;
  }
}

void andar(){
  switch (status)
  {
  case LAH:
    execute(&lookAhead, interval_lah);
    break;
  case MS1:
    execute(&moveStop, interval_ms1);
    break;
  case MB:
    execute(&moveBackward, interval_mb);
    break;
  case MS2:
    execute(&moveStop, interval_ms2);
    break;
  case LR:
    execute(&lookRight, interval_look);
    break;
  case LL:
    execute(&lookLeft, interval_look);
    break;
  case LF:
    execute(&moveViewerFront, interval_look);
    break;
  case TR:
    execute(&turnRight, interval_turn);
    break;
  case TL:
    execute(&turnLeft, interval_turn);
    break;
  case MF:
    execute(&moveForward, interval_mf);
    break;
  case READ_RIGHT:
    execute(&readRight, 100);
    break;
  case READ_LEFT:
    execute(&readLeft, 100);
    break;
  default:
    break;
  }
}

int changeStatus(int status)
{
  switch (status)
  {
  case MS1:
    return MB;
  case MB:
    return MS2;
  case MS2:
    return LR;
  case MF:
    return LAH;
  case LR:
    return READ_RIGHT;
  case READ_RIGHT:
    return LL;
  case LL:
    return READ_LEFT;
  case READ_LEFT:
    return LF;
  case LF:
    if(distanceR>=distanceL){
      return TR;
    }
    return TL;
  case TR:
  case TL:
  return LAH;
  case LAH:
    if(distance < 15){
      return MS1;
    }
    distance = 0;
    return MF;
  default:
    break;
  }
}

void execute(void (*func) (void), long interval)
{
  if (!wait_mode)
  {
    func();
    wait_mode = true;
    start = millis();
    return;
  }
  long now = millis();
  if (now - start > interval)
  {
    status = changeStatus(status);
    wait_mode = false;
  }
}

void lookAhead(){
  distanceL = 0;
  distanceR = 0;
  distance = readPing();
}

void lookRight()
{
  execute(&moveViewerRight, 1000);
}

void readRight(){
  distanceR = readPing();
}

void readLeft(){
  distanceL = readPing();
}

void lookLeft()
{
  execute(&moveViewerLeft, 2000);
}

void moveViewerRight(){
  myservo.write(0);
}

void moveViewerLeft(){
  myservo.write(180);
}

void moveViewerFront(){
  myservo.write(90);
}

int readPing()
{

  unsigned long duration;
  int dist;

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 5700);  // Timeout if distance > 100 cm

  dist = duration / 57;  // Divide by round-trip microseconds per cm to get cm

  if (dist >= MAX_DISTANCE || dist <= 0) {
    //Serial.println("Out of range");
    return MAX_DISTANCE;
  }

  Serial.print("Distance: ");
  Serial.println(dist);

  return dist;
}

void moveStop()
{
  servo_motor1.write(90);
  servo_motor2.write(90);
}

void moveForward()
{
  if (!goesForward)
  {
    goesForward = true;
    for (int speedSet = 2; speedSet < MAX_SPEED; speedSet += 1) // slowly bring the speed up to avoid loading down the batteries too quickly
    {
      servo_motor1.write(90 + speedSet);
      servo_motor2.write(90 - speedSet);
      delay(5);
    }
  }
}
void moveBackward()
{
  goesForward = false;
  for (int speedSet = 2; speedSet < MAX_SPEED; speedSet += 1) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    servo_motor1.write(90 - speedSet);
    servo_motor2.write(90 + speedSet);
    delay(5);
  }
}

void turnRight()
{
  servo_motor1.write(90 + SPEED);
  servo_motor2.write(90 + SPEED);
}

void turnLeft()
{
  servo_motor1.write(90 - SPEED);
  servo_motor2.write(90 - SPEED);
}
