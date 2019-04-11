//ARDUINO OBSTACLE AVOIDING CAR//
// Before uploading the code you have to install the necessary library//
//NewPing Library https://github.com/eliteio/Arduino_New_Ping.git // 
//Servo Library https://github.com/arduino-libraries/Servo.git //

#include <NewPing.h>
#include <Servo.h> 

#define TRIG_PIN 6 
#define ECHO_PIN 7 
#define MAX_DISTANCE 200 
#define MAX_SPEED_OFFSET 20

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 

// servo
int PINSERVO=5;
int PULSOMIN = 1500;
int PULSOMAX = 7500;

Servo myservo;   

boolean goesForward=false;
int distance = 100;


// motors servo
Servo servo_motor1;
Servo servo_motor2;

int SERVO_MOTOR1 = 9;
int SERVO_MOTOR2 = 10;
int SPEED = 7;
int MAX_SPEED = 7;



void setup() {
  // motors servo
  servo_motor1.attach(SERVO_MOTOR1,PULSOMIN,PULSOMAX);
  servo_motor2.attach(SERVO_MOTOR2,PULSOMIN,PULSOMAX);


  // distance servo
  myservo.attach(PINSERVO,PULSOMIN,PULSOMAX);
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

void loop() {
 int distanceR = 0;
 int distanceL =  0;
 delay(40);
 
 if(distance<=15)
 {
  moveStop();
  delay(100);
  moveBackward();
  delay(700);
  moveStop();
  delay(200);
  distanceR = lookRight();
  delay(200);
  distanceL = lookLeft();
  delay(200);

  if(distanceR>=distanceL)
  {
    turnRight();
    moveStop();
  }else
  {
    turnLeft();
    moveStop();
  }
 }else
 {
  moveForward();
 }
 distance = readPing();
}

int lookRight()
{
    myservo.write(0); 
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(90); 
    return distance;
}

int lookLeft()
{
    myservo.write(180); 
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(90); 
    return distance;
    delay(100);
}

int readPing() { 
  delay(70);
  int cm = sonar.ping_cm();
  if(cm==0)
  {
    cm = 250;
  }
  return cm;
}

void moveStop() {
  servo_motor1.write(90);
  servo_motor2.write(90);
  } 
  
void moveForward() {

 if(!goesForward)
  {
   goesForward=true;
   for (int speedSet = 2; speedSet < MAX_SPEED; speedSet +=1) // slowly bring the speed up to avoid loading down the batteries too quickly
   {
    servo_motor1.write(90+speedSet);
    servo_motor2.write(90-speedSet);   
    delay(5);
   }
  }
}

void moveBackward() {
   goesForward=false;  
  for (int speedSet = 2; speedSet < MAX_SPEED; speedSet +=1) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    servo_motor1.write(90-speedSet);
    servo_motor2.write(90+speedSet);
    delay(5);
  }
}  

void turnRight() {
  servo_motor1.write(90+SPEED);
  servo_motor2.write(90+SPEED);  
  delay(700);   
} 
 
void turnLeft() {
  servo_motor1.write(90-SPEED);
  servo_motor2.write(90-SPEED);
  delay(700);
}  
