#include <SPI.h>
#include "RF24.h"

bool radioNumber = 0;

int aEnable = 3;
int bEnable = 5;
int aPhase = 2;
int bPhase = 4;
int mode = 6;

int go = 255;
int turn = 50;

RF24 radio(7,8) ;

byte addresses[][6] = {"1Node","2Node"};

bool role = 0;

void setup() {
  Serial.begin(115200);
  Serial.println(F("This is the CAR RECIEVER script."));
  
  radio.begin();

  radio.setPALevel(RF24_PA_MAX);
  
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  
  // Start the radio listening for data
  radio.startListening();

  pinMode(aEnable, OUTPUT);
  pinMode(bEnable, OUTPUT);
  pinMode(aPhase, OUTPUT);
  pinMode(bPhase, OUTPUT);
  pinMode(mode, OUTPUT);
}

typedef struct {
  float x = 0;
  float y = 0;
} Package;
Package p;

void loop() {
  digitalWrite(mode, HIGH);
  unsigned long got_time;
  
  if( radio.available()){
    
    while (radio.available()) {
      radio.read(&p, sizeof(p));
    }
   
    radio.stopListening();
    radio.write(&got_time, sizeof(unsigned long));  
    radio.startListening();
    Serial.print(F("Got class x: "));
    Serial.print(p.x);  
    Serial.print(" || y: ");
    Serial.println(p.y);

    if (p.y > 700) { //forward
      digitalWrite(aPhase, HIGH);
      digitalWrite(bPhase, HIGH);
      if (p.x > 700) { //left
        analogWrite(aEnable, turn);
        analogWrite(bEnable, go);
      } else if (p.x < 300) { //right
        analogWrite(aEnable, go);
        analogWrite(bEnable, turn);
      }
    } else if (p.y < 300) { //backward
      digitalWrite(aPhase, LOW);
      digitalWrite(bPhase, LOW);
      if (p.x > 700) { //left
        analogWrite(aEnable, turn);
        analogWrite(bEnable, go);
      } else if (p.x < 300) { //right
        analogWrite(aEnable, go);
        analogWrite(bEnable, turn);
      }
    } else { //stationary
      
      if (p.x > 700) { //left
        digitalWrite(aPhase, LOW);
        digitalWrite(bPhase, HIGH);
        analogWrite(aEnable, turn);
        analogWrite(bEnable, turn);
      } else if (p.x < 300) { //right
        digitalWrite(aPhase, HIGH);
        digitalWrite(bPhase, LOW);
        analogWrite(aEnable, turn);
        analogWrite(bEnable, turn);
      }
    }
 }
  
} 
