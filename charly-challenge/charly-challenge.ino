//ARDUINO OBSTACLE AVOIDING CAR//
// Before uploading the code you have to install the necessary library//
//NewPing Library https://github.com/eliteio/Arduino_New_Ping.git //
//Servo Library https://github.com/arduino-libraries/Servo.git //

#include <Servo.h>

#define TRIG_PIN 6
#define ECHO_PIN 7
#define MAX_DISTANCE 200
#define MAX_SPEED_OFFSET 20

// NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

// servo
int PINSERVO = 5;
int PULSOMIN = 1500;
int PULSOMAX = 7500;

Servo myservo;

bool goesForward = false;
int distance = 100;


// motors servo
Servo servo_motor1;
Servo servo_motor2;

int SERVO_MOTOR1 = 9;
int SERVO_MOTOR2 = 10;
int SPEED = 7;
int MAX_SPEED = 7;
int speedSet = 2;

int step = 0;
bool ran = false;
bool isLooking = false;
bool isTurning = false;
bool isStuck = false;
bool isMoving = false;

int distanceR = 0;
int distanceL = 0;

//// SOUNDS

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
const int ledPin1 = 12;
const int ledPin2 = 13;
int ledAux = 0;

int counter = 0;

bool isToneOn = false;
bool isBlasting = false;
bool boop = false;

unsigned long previousRunMillis = 0;
unsigned long previousRunDurationMillis = 0;
unsigned long previousToneMillis = 0;
unsigned long previousToneDurationMillis = 0;
unsigned long previousBlastDurationMillis = 0;

int imperialMarch1_1[9][2] = {
  {a, 500},
  {a, 500},
  {a, 500},
  {f, 350},
  {cH, 150},
  {a, 500},
  {f, 350},
  {cH, 150},
  {a, 650}
};

int imperialMarch1_2[9][2] = {
  {eH, 500},
  {eH, 500},
  {eH, 500},
  {fH, 350},
  {cH, 150},
  {gS, 500},
  {f, 350},
  {cH, 150},
  {a, 650}
};

int imperialMarch2_1[9][2] = {
  {aH, 500},
  {a, 300},
  {a, 150},
  {aH, 500},
  {gSH, 325},
  {gH, 175},
  {fSH, 125},
  {fH, 125},
  {fSH, 250}
};

int imperialMarch2_2[9][2] = {
  {aS, 250},
  {dSH, 500},
  {dH, 325},
  {cSH, 175},
  {cH, 125},
  {b, 125},
  {cH, 250},
  {0,0},
  {0,0}
};

int imperialMarchVar_1[9][2] = {
  {f, 250},
  {gS, 500},
  {f, 350},
  {a, 125},
  {cH, 500},
  {a, 375},
  {cH, 125},
  {eH, 650}
};

int imperialMarchVar_2[9][2] = {
  {f, 250},
  {gS, 500},
  {f, 375},
  {cH, 125},
  {a, 500},
  {f, 375},
  {cH, 125},
  {a, 650}
};

typedef int (*imperialMarchArrangement)[2];

imperialMarchArrangement arr[] = {
  imperialMarch1_1,
  imperialMarch1_2,
  imperialMarch2_1,
  imperialMarch2_2,
  imperialMarchVar_1,
  imperialMarch2_1,
  imperialMarch2_2,
  imperialMarchVar_2
};

int delays[] = {
  0,
  500,
  500,
  325,
  350,
  500,
  325,
  350
};

int part = 0;
int note = 0;

// Laser
int laserPin = 2;
int i = 0;

void setup()
{
  Serial.begin(9600);
  
  // motors servo
  servo_motor1.attach(SERVO_MOTOR1, PULSOMIN, PULSOMAX);
  servo_motor2.attach(SERVO_MOTOR2, PULSOMIN, PULSOMAX);

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

  // Setup pin modes
  pinMode(buzzerPin, OUTPUT);
  pinMode(laserPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  previousRunMillis = millis();
  previousRunDurationMillis = millis();
  previousToneMillis = millis();
  previousToneDurationMillis = millis();
  previousBlastDurationMillis = millis();
}

void loop()
{
  run();
  play();
}

void run() {
  unsigned long currentMillis = millis();

  if (distance <= 15) {
    switch (step) {
      case 0:
        isStuck = true;
        moveStop();
        step++;
        previousRunMillis = currentMillis;
        break;
      case 1:
        if (currentMillis - previousRunMillis >= 100) {
          ran = moveBackward(currentMillis);
        }
        break;
      case 2:
        if (currentMillis - previousRunMillis >= 700) {
          moveStop();
          step++;
          previousRunMillis = currentMillis;
        }
        break;
      case 3:
        if (currentMillis - previousRunMillis >= 200) {
          ran = lookRight(currentMillis);
        }
        break;
      case 4:
        if (currentMillis - previousRunMillis >= 200) {
          ran = lookLeft(currentMillis);
        }
        break;
      case 5:
        if (currentMillis - previousRunMillis >= 200) {
          if (distanceR >= distanceL) {
            ran = turnRight(currentMillis);
          }
          else {
            ran = turnLeft(currentMillis);
          }
          if (ran) {
            moveStop();
          }
        }
        break;
      case 6:
        step = 0;
        isStuck = false;
        break;
    }
    if (ran) {
      ran = false;
      step++;
      previousRunMillis = currentMillis;
      previousRunDurationMillis = currentMillis;
    }
    spaceGun(300,currentMillis);
  }
  if (!isStuck) {
    distance = readPing();
    moveForward(currentMillis);
  }
}

void play() {
  unsigned long currentMillis = millis();
  
  if (part == 8) {
    if (currentMillis - previousToneMillis >= 650) {
      previousToneMillis = currentMillis;
      part = 0;
    }
  } else if (currentMillis - previousToneMillis >= delays[part]) {
    boop = beep(arr[part][note][0], arr[part][note][1], currentMillis);
    if (boop) {
      note++;
    }
    if (note == 9) {
      previousToneMillis = currentMillis;
      note = 0;
      part++;
    }
  }
}

bool lookRight(unsigned long currentMillis)
{
  if (!isLooking) {
    previousRunDurationMillis = currentMillis;
    myservo.write(0);
    isLooking = true;
  } else if (currentMillis - previousRunDurationMillis > 670) {
    myservo.write(90);
    isLooking = false;
  } else if (currentMillis - previousRunDurationMillis > 570) {
    distanceR = readPing();
  }
  return !isLooking;
}

bool lookLeft(unsigned long currentMillis)
{
  if (!isLooking) {
    previousRunDurationMillis = currentMillis;
    myservo.write(180);
    isLooking = true;
  } else if (currentMillis - previousRunDurationMillis > 670) {
    myservo.write(90);
    isLooking = false;
  } else if (currentMillis - previousRunDurationMillis > 570) {
    distanceL = readPing();
  } 
  return !isLooking;
}

int readPing() {
  unsigned long duration;
  int distance;
  
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(5);
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

void moveStop() {
  servo_motor1.write(90);
  servo_motor2.write(90);
}

void moveForward(unsigned long currentMillis) {
  if (!goesForward)
  {
    if (!isMoving) {
      previousRunDurationMillis = currentMillis;
      isMoving = true;
    } else if (speedSet < MAX_SPEED) {
      if (currentMillis - previousRunDurationMillis >= 5) {
        servo_motor1.write(90 + speedSet);
        servo_motor2.write(90 - speedSet);
        speedSet += 1;
        previousRunDurationMillis = currentMillis;
      }
    } else {
      speedSet = 2;
      goesForward = true;
      isMoving = false;
    }
  }
}

bool moveBackward(unsigned long currentMillis) {
  goesForward = false;
  if (!isMoving) {
    previousRunDurationMillis = currentMillis;
    isMoving = true;
  } else if (speedSet < MAX_SPEED) {
    if (currentMillis - previousRunDurationMillis >= 5) {
      servo_motor1.write(90 - speedSet);
      servo_motor2.write(90 + speedSet);
      speedSet += 1;
      previousRunDurationMillis = currentMillis;
    }
  } else {
    speedSet = 2;
    isMoving = false;
  }
  return !isMoving;
}

bool turnRight(unsigned long currentMillis) {
  if (!isTurning) {
    previousRunDurationMillis = currentMillis;
    servo_motor1.write(90 + SPEED);
    servo_motor2.write(90 + SPEED);
    isTurning = true;
  } else if (currentMillis - previousRunDurationMillis >= 700) {
    isTurning = false;
  }
  return !isTurning;
}

bool turnLeft(unsigned long currentMillis) {
  if (!isTurning) {
    previousRunDurationMillis = currentMillis;
    servo_motor1.write(90 - SPEED);
    servo_motor2.write(90 - SPEED);
    isTurning = true;
  } else if (currentMillis - previousRunDurationMillis >= 700) {
    isTurning = false;
  }
  return !isTurning;
}

bool beep(int note, int duration, unsigned long currentMillis)
{
  //Play tone on buzzerPin
  if (!isToneOn) {
    tone(buzzerPin, note);
    isToneOn = true;
    //Play different LED depending on value of 'counter'
    if (counter % 2 == 0) {
      ledAux = ledPin1;
    } else {
      ledAux = ledPin2;
    }
    digitalWrite(ledAux, HIGH);
    previousToneDurationMillis = currentMillis;
  } else if (currentMillis - previousToneDurationMillis >= duration) {
    digitalWrite(ledAux, LOW);
    //Stop tone on buzzerPin
    noTone(buzzerPin);
    isToneOn = false;
    counter++;
    return true;
  }
  return false;
}

void spaceGun(int blastMax, unsigned long currentMillis) {
  if ((!isBlasting) && (currentMillis - previousBlastDurationMillis > blastMax)) {
    isBlasting = true;
    i = 0;
    previousBlastDurationMillis = currentMillis;
    digitalWrite(laserPin,HIGH);
  } else if (currentMillis - previousBlastDurationMillis < blastMax) {
    digitalWrite(buzzerPin,HIGH);
    delayMicroseconds(i);
    digitalWrite(buzzerPin,LOW);
    delayMicroseconds(i);     
    i++;
  } else {
    previousBlastDurationMillis = currentMillis;
    isBlasting = false;
    digitalWrite(laserPin,LOW);
  }
}
