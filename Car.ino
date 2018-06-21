#include <SPI.h>
#include "RF24.h"

bool radioNumber = 0;

int aEnable = 3;
int aPhase = 2;
int bEnable = 5;
int bPhase = 4;
int mode = 6;

RF24 radio(7,8) ;

byte addresses[][6] = {"1Node","2Node"};

bool role = 0;

void setup() {
  Serial.begin(115200);
  Serial.println(F("This is the CAR RECIEVER script."));
  
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  radio.startListening();

  pinMode(aEnable, OUTPUT);
  pinMode(aPhase, OUTPUT);
  pinMode(bEnable, OUTPUT);
  pinMode(bPhase, OUTPUT);
  pinMode(mode, OUTPUT);
}

typedef struct {
  float x = 0;
  float y = 0;
} Package;
Package p;

void loop() {

/****************** Pong Back Role ***************************/

  unsigned long got_time;
  
  
  if( radio.available()){
    while (radio.available()) {
      radio.read( &p, sizeof(p) );
    }
   
    radio.stopListening();
    radio.write( &got_time, sizeof(unsigned long) );
    radio.startListening();
    Serial.print(F("Got class x: "));
    Serial.print(p.x);  
    Serial.print(" || y: ");
    Serial.println(p.y);

    if (p.y > 650) { //forward
      digitalWrite(aPhase, HIGH);
      digitalWrite(bPhase, HIGH);
      if (p.x > 650) { // right
        analogWrite(aEnable, 255);
        analogWrite(bEnable, 0);
      } else if (p.x < 350) { //left
        analogWrite(aEnable, 0);
        analogWrite(bEnable, 255);
      } else {
        analogWrite(aEnable, 255);
        analogWrite(bEnable, 255);
      }
    } else if (p.y < 350) {
      digitalWrite(aPhase, LOW);
      digitalWrite(bPhase, LOW);
      if (p.x > 650) { // right
        analogWrite(aEnable, 255);
        analogWrite(bEnable, 0);
      } else if (p.x < 350) { //left
        analogWrite(aEnable, 0);
        analogWrite(bEnable, 255);
      } else {
        analogWrite(aEnable, 255);
        analogWrite(bEnable, 255);
      }
    } else {
      
      if (p.x > 650) { // right
        digitalWrite(aPhase, HIGH);
        digitalWrite(bPhase, LOW);
        analogWrite(aEnable, 255);
        analogWrite(bEnable, 255);
      } else if (p.x < 350) { //left
        digitalWrite(aPhase, LOW);
        digitalWrite(bPhase, HIGH);
        analogWrite(aEnable, 255);
        analogWrite(bEnable, 255);
      } else {
        analogWrite(aEnable, 0);
        analogWrite(bEnable, 0);
      }
    }
 }
  digitalWrite(mode, HIGH);
} 
