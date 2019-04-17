int buzzer = 11;

void setup() {
   pinMode(buzzer,OUTPUT);

}

void loop() {
  spaceGun(300);
  delay(1);

}


void spaceGun(int maximun) {
  for(int i=0; i < maximun; i++) {
    digitalWrite(buzzer,HIGH);
    delayMicroseconds(i);
    digitalWrite(buzzer,LOW);
    delayMicroseconds(i);
            
  }  
}
