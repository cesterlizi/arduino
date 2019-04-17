#include <Arduino_FreeRTOS.h>
#include <Servo.h> 

// define tasks
void TaskBlink( void *pvParameters );
void TaskStarWars( void *pvParameters );
void TaskCar( void *pvParameters );
void TaskIR( void *pvParameters );

// the setup function runs once when you press reset or power the board
void setup() {
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  // Now set up two tasks to run independently.
  xTaskCreate(
    TaskBlink
    ,  (const portCHAR *)"Blink"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskStarWars
    ,  (const portCHAR *) "StartWars"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

  xTaskCreate(
    TaskCar
    ,  (const portCHAR *) "Car"
    ,  128  // Stack size
    ,  NULL
    ,  2  // Priority
    ,  NULL );

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

#define LASER_PIN 2
boolean blaster = false;

void TaskBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, LEONARDO, MEGA, and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN takes care 
  of use the correct LED pin whatever is the board used.
  
  The MICRO does not have a LED_BUILTIN available. For the MICRO board please substitute
  the LED_BUILTIN definition with either LED_BUILTIN_RX or LED_BUILTIN_TX.
  e.g. pinMode(LED_BUILTIN_RX, OUTPUT); etc.
  
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
  
  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  modified 2 Sep 2016
  by Arturo Guadalupi
*/

  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(LASER_PIN, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    if(blaster) {
      spaceGun(300);
      digitalWrite(LASER_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for one second
      digitalWrite(LASER_PIN, LOW);    // turn the LED off by making the voltage LOW
      vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for one second
    } else {
       vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for one second
    }
  }
}


/* 
 *  Star Wars Task
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
 
const int buzzerPin = 11; 
boolean music = true;


void beep(int note, int duration)
{
  if(music) {
    //Play tone on buzzerPin
    tone(buzzerPin, note);
    vTaskDelay( duration / portTICK_PERIOD_MS );
   
    //Stop tone on buzzerPin
    noTone(buzzerPin);
   
    vTaskDelay( 50 / portTICK_PERIOD_MS );
  }
}

void spaceGun(int maximun) {
  for(int i=0; i < maximun; i++) {
    digitalWrite(buzzerPin,HIGH);
    delayMicroseconds(i);
    digitalWrite(buzzerPin,LOW);
    delayMicroseconds(i);
            
  }  
}
 
void firstSection()
{
  beep(a, 500);
  beep(a, 500);    
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);  
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  vTaskDelay( 500 / portTICK_PERIOD_MS );
 
  beep(eH, 500);
  beep(eH, 500);
  beep(eH, 500);  
  beep(fH, 350);
  beep(cH, 150);
  beep(gS, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  vTaskDelay( 500 / portTICK_PERIOD_MS );
}
 
void secondSection()
{
  beep(aH, 500);
  beep(a, 300);
  beep(a, 150);
  beep(aH, 500);
  beep(gSH, 325);
  beep(gH, 175);
  beep(fSH, 125);
  beep(fH, 125);    
  beep(fSH, 250);
 
  vTaskDelay( 325 / portTICK_PERIOD_MS );
 
  beep(aS, 250);
  beep(dSH, 500);
  beep(dH, 325);  
  beep(cSH, 175);  
  beep(cH, 125);  
  beep(b, 125);  
  beep(cH, 250);  
 
  vTaskDelay( 350 / portTICK_PERIOD_MS );;
}

void TaskStarWars(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;)
  {
    //Play first section
    firstSection();
   
    //Play second section
    secondSection();
   
    //Variant 1
    beep(f, 250);  
    beep(gS, 500);  
    beep(f, 350);  
    beep(a, 125);
    beep(cH, 500);
    beep(a, 375);  
    beep(cH, 125);
    beep(eH, 650);
   
    vTaskDelay( 500 / portTICK_PERIOD_MS );
   
    //Repeat second section
    secondSection();
   
    //Variant 2
    beep(f, 250);  
    beep(gS, 500);  
    beep(f, 375);  
    beep(cH, 125);
    beep(a, 500);  
    beep(f, 375);  
    beep(cH, 125);
    beep(a, 650);  
   
    vTaskDelay( 650 / portTICK_PERIOD_MS );
  }
}


/*
 * Car Task
 */


#define TRIG_PIN 6 
#define ECHO_PIN 7 
#define MAX_DISTANCE 200 
#define MAX_SPEED_OFFSET 20

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
int SPEED = 5;
int MAX_SPEED = 5; 

int readPing() { 
  unsigned long duration;
  int distance;

  digitalWrite(TRIG_PIN, HIGH);
  vTaskDelay( 1 / portTICK_PERIOD_MS );
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 5700);  // Timeout if distance > 100 cm

  distance = duration / 57;  // Divide by round-trip microseconds per cm to get cm

  if (distance >= MAX_DISTANCE || distance <= 0) {
    Serial.println("Out of range");
    return MAX_DISTANCE;
  }

  Serial.print("Distance: ");
  Serial.println(distance);

  return distance;

}
  
int lookRight()
{
    myservo.write(0); 
    vTaskDelay( 500 / portTICK_PERIOD_MS );
    int distance = readPing();
    vTaskDelay( 100 / portTICK_PERIOD_MS );
    myservo.write(90); 
    return distance;
}

int lookLeft()
{
    myservo.write(180); 
    vTaskDelay( 500 / portTICK_PERIOD_MS );
    int distance = readPing();
    vTaskDelay( 100 / portTICK_PERIOD_MS );
    myservo.write(90); 
    return distance;
    vTaskDelay( 100 / portTICK_PERIOD_MS );
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
    vTaskDelay( 5 / portTICK_PERIOD_MS );
   }
  }
}

void moveBackward() {
   goesForward=false;  
  for (int speedSet = 2; speedSet < MAX_SPEED; speedSet +=1) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    servo_motor1.write(90-speedSet);
    servo_motor2.write(90+speedSet);
    vTaskDelay( 5 / portTICK_PERIOD_MS );
  }
}  

void turnRight() {
  servo_motor1.write(90+SPEED);
  servo_motor2.write(90+SPEED);  
  vTaskDelay( 700 / portTICK_PERIOD_MS );   
} 
 
void turnLeft() {
  servo_motor1.write(90-SPEED);
  servo_motor2.write(90-SPEED);
  vTaskDelay( 700 / portTICK_PERIOD_MS );
} 

void TaskCar(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  // motors servo
  servo_motor1.attach(SERVO_MOTOR1,PULSOMIN,PULSOMAX);
  servo_motor2.attach(SERVO_MOTOR2,PULSOMIN,PULSOMAX);

  // distance servo
  myservo.attach(PINSERVO,PULSOMIN,PULSOMAX);
  myservo.write(90); 


  // distance
   pinMode(TRIG_PIN, OUTPUT);
   pinMode(ECHO_PIN, INPUT);

  int distanceR = 0;
  int distanceL =  0;
  int distance = 0;

  for (;;) // A Task shall never return or exit.
  {
     distance = readPing();
     
     if(distance<=15)
     {
      blaster = true;
      music = false;
      moveStop();
      vTaskDelay( 100 / portTICK_PERIOD_MS );
      moveBackward();
      vTaskDelay( 700 / portTICK_PERIOD_MS );
      moveStop();
      vTaskDelay( 200 / portTICK_PERIOD_MS );
      distanceR = lookRight();
      vTaskDelay( 200 / portTICK_PERIOD_MS );
      distanceL = lookLeft();
      vTaskDelay( 200 / portTICK_PERIOD_MS );;
    
      if(distanceR>=distanceL)
      {
        turnRight();
        moveStop();
      }else
      {
        turnLeft();
        moveStop();
      }
     }else {
      blaster = false;
      music = true;
      moveForward();
     }

     vTaskDelay( 40 / portTICK_PERIOD_MS );
  }
}


/*
 * Task IR
 */
void TaskIR(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;) // A Task shall never return or exit.
  {
     

     vTaskDelay( 40 / portTICK_PERIOD_MS );
  }
}
