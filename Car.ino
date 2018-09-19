#include <SPI.h>
#include "RF24.h"


RF24 radio(7,8) ;
byte addresses[][6] = {"1Node","2Node"};

int leftEnable = 3;
int rightEnable = 5;
int leftPhase = 2;
int rightPhase = 4;
int mode = 6;


int go = 255;
int turn = 50;

void setup() {
  Serial.begin(115200);
  Serial.println(F("This is the CAR RECIEVER script."));
  
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  radio.startListening();

  pinMode(leftEnable, OUTPUT);
  pinMode(rightEnable, OUTPUT);
  pinMode(leftPhase, OUTPUT);
  pinMode(rightPhase, OUTPUT);
  pinMode(mode, OUTPUT);
}

typedef struct {
  float x = 0;
  float y = 0;
} Package;
Package p;

void loop() {
  unsigned long got_time;
  if (radio.available()) {
    while (radio.available()) {
      radio.read(&p, sizeof(p));
    }
    radio.stopListening();
    radio.write(&got_time, sizeof(unsigned long));  
    radio.startListening();
  }


  char y;
  if (p.y > 600) {
    y = 'F';
  } else if (p.y < 400) {
    y = 'B';
  } else {
    y = 'S';
  }

  char x;
  if (p.x > 600) {
    x = 'R';
  } else if (p.x < 400) {
    x = 'L';
  } else {
    x = 'S';
  }
  
  Serial.print(y);
  Serial.print(" ");
  Serial.print(p.y);
  Serial.print(" ");
  Serial.print(x);
  Serial.print(" ");
  Serial.println(p.x);
  move(x, y);
}

void move(char x, char y) {
  digitalWrite(mode, HIGH);
  if (y == 'F') {
    if (x == 'L') {
      turnWheels(1, 1, turn, go);
    } else if (x == 'S') {
      turnWheels(1, 1, go, go);
    } else if (x == 'R') {
      turnWheels(1, 1, go, turn);
    }
  } else if (y == 'S') {
    if (x == 'L') {
      turnWheels(0, 1, turn, turn);
    } else if (x == 'R') {
      turnWheels(1, 0, turn, turn);
    }
  } else if (y == 'B') {
    if (x == 'L') {
      turnWheels(0, 0, turn, go);
    } else if (x == 'S') {
      turnWheels(0, 0, go, go);
    } else if (x == 'R') {
      turnWheels(0, 0, go, turn);
    }
  }
}

void turnWheels (bool mode1, bool mode2, int speed1, int speed2) {
  if (mode1) digitalWrite(leftPhase, HIGH);
  else digitalWrite(leftPhase, LOW);
  
  if (mode2) digitalWrite(rightPhase, HIGH);
  else digitalWrite(rightPhase, LOW);
  
  analogWrite(leftEnable, speed1);
  analogWrite(rightEnable, speed2);
}
