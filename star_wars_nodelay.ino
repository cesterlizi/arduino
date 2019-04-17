#include <Servo.h>
#define TRIG_PIN 6 
#define ECHO_PIN 7 
#define MAX_DISTANCE 200 
#define MAX_SPEED_OFFSET 20


int notes[] = {440, 440, 440, 349, 523, 440, 349, 523, 440, 0, 659, 659, 659, 698, 523, 415, 349, 523, 440, 0, 880, 440, 440, 880, 830, 784, 740, 698, 740, 0, 455, 622, 587, 554, 523, 466, 523, 0, 349, 415, 349, 440, 523, 440, 523, 659, 0, 880, 440, 440, 880, 830, 784, 740, 698, 740, 0, 455, 622, 587, 554, 523, 466, 523, 0, 349, 415, 349, 523, 440, 349, 523, 440, 2};
int timeNotes[] = {500, 500, 500, 350, 150, 500, 350, 150, 650, 500, 500, 500, 500, 350, 150, 500, 350, 150, 650, 500, 500, 300, 150, 500, 325, 175, 125, 125, 250, 325, 250, 500, 325, 175, 125, 125, 250, 350, 250, 500, 350, 125, 500, 375, 125, 650, 500, 500, 300, 150, 500, 325, 175, 125, 125, 250, 325, 250, 500, 325, 175, 125, 125, 250, 350, 250, 500, 375, 125, 500, 375, 125, 650, 650};
int noteCounter = 0;
unsigned long previousMillis = 0;
unsigned long prevMovementMillis = 0;
const int buzzerPin = 11;

// servo
int PINSERVO=5;
int PULSOMIN = 1500;
int PULSOMAX = 7500;
int READING_DISTANCE = 1;
const int MOVING_FORWARD = 2;
const int TURNING_RIGHT = 3;
const int TURNING_LEFT = 4;
const int MOVING_BACK = 5;
const int READING_LEFT = 6;
const int READING_RIGHT = 7;
const int BACKWARD_DELAY = 700;
const int TURNING_DELAY = 700;

Servo servo;   

boolean goesForward=false;
int distance = 100;
int pos;
int state;
int  updateInterval;      // interval between updates
int increment;
unsigned long lastUpdate; // last update of position


// motors servo
Servo servo_motor1;
Servo servo_motor2;

int SERVO_MOTOR1 = 9;
int SERVO_MOTOR2 = 10;
int SPEED = 7;
int MAX_SPEED = 7;

void setup()
{
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  //Setup pin modes
  pinMode(buzzerPin, OUTPUT);
  // motors servo
  servo_motor1.attach(SERVO_MOTOR1,PULSOMIN,PULSOMAX);
  servo_motor2.attach(SERVO_MOTOR2,PULSOMIN,PULSOMAX);


  // distance servo
  servo.attach(PINSERVO,PULSOMIN,PULSOMAX);
  servo.write(90);
  updateInterval = 1;
  increment = 1;
  Serial.begin(9600);
  state = MOVING_FORWARD;
  
}

void loop()
{
  unsigned long currentMillis = millis();
  int distanceR = 0;
  int distanceL =  0;

  if (previousMillis == 0) {
    tone(buzzerPin, notes[noteCounter]);
    previousMillis = currentMillis;
  } else if (currentMillis - previousMillis >= timeNotes[noteCounter]) {
    if (notes[noteCounter] == 2) {
      noteCounter = 0;
      noTone(buzzerPin);
      tone(buzzerPin, 0); 
      previousMillis = currentMillis;
    } else {
      noteCounter++;
      previousMillis = currentMillis;
      noTone(buzzerPin);
      tone(buzzerPin, notes[noteCounter]); 
    } 
    
  }
  distance = readPing(); 
  switch(state) {
    case MOVING_FORWARD:
      if(distance > 15) {
          servo_motor1.write(97);
          servo_motor2.write(83); 
        } else {
          servo_motor1.write(90);
          servo_motor2.write(90);
          state = MOVING_BACK;
        }
        prevMovementMillis = currentMillis;
      break;
    case TURNING_RIGHT:
      if(currentMillis - prevMovementMillis >= TURNING_DELAY)
      {
        servo_motor1.write(90);
        servo_motor2.write(90);
        state = MOVING_FORWARD;
      } else {
        servo_motor1.write(97);
        servo_motor2.write(97); 
      }
      prevMovementMillis = currentMillis;
      break;
    case TURNING_LEFT:
      if(currentMillis - prevMovementMillis >= TURNING_DELAY)
      {
        servo_motor1.write(90);
        servo_motor2.write(90);
        state = MOVING_FORWARD;
      } else {
        servo_motor1.write(83);
        servo_motor2.write(83); 
      }
      prevMovementMillis = currentMillis;
      break;
    case MOVING_BACK:
      if(currentMillis - prevMovementMillis >= BACKWARD_DELAY)
      {
        servo_motor1.write(90);
        servo_motor2.write(90);
        state = READING_RIGHT;
      } else {
        servo_motor1.write(83);
        servo_motor2.write(97);
      }
      prevMovementMillis = currentMillis;
      break;
    case READING_RIGHT:
      increment = -increment;
      if((currentMillis - prevMovementMillis) > updateInterval)  // time to update
        {
          pos += increment;
          servo.write(pos);
          Serial.println(pos);
          if (pos <= 0) // end of sweep
          {
            distanceR = readPing();
            increment = -increment;
          }
          if (pos == 90) {
            state = READING_LEFT;
          }
        }
        prevMovementMillis = currentMillis;
      break;
    case READING_LEFT:
      if((currentMillis - prevMovementMillis) > updateInterval)  // time to update
        {
          lastUpdate = millis();
          pos += increment;
          servo.write(pos);
          Serial.println(pos);
          if (pos >= 180) // end of sweep
          {
            distanceL = readPing();
            increment = -increment;
          }
          if (pos == 90) {
            increment = -increment;
            if(distanceR>=distanceL)
            {
              state = TURNING_RIGHT;
            }else
            {
              state = TURNING_LEFT;
            }
          }
        }
        prevMovementMillis = currentMillis;
      break;
    default:
      break;
  }
  
}

int readPing() { 
  unsigned long duration;
  int distance;

  digitalWrite(TRIG_PIN, HIGH);
  delay(0.5);
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
