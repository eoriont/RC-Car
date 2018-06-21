#include <SPI.h>
#include "RF24.h"

bool radioNumber = 1;

RF24 radio(7,8);

byte addresses[][6] = {"1Node","2Node"};

bool role = 1;

void setup() {
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
  
  radio.begin();

  radio.setPALevel(RF24_PA_LOW);
  
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);
  
  radio.startListening();
}


typedef struct {
  float x = 0;
  float y = 0;
} Package;
Package p;

void loop() {
  float xVal1 = analogRead(0);
  float yVal1 = analogRead(1);
  
  p.x = xVal1;
  p.y = yVal1;
    
    radio.stopListening();
    
    
    Serial.println(F("Now sending"));

    unsigned long start_time = micros();
     bool worked = radio.write( &p, sizeof(p));
        
    radio.startListening();
    
    unsigned long started_waiting_at = micros();
    boolean timeout = false;
    
    while ( ! radio.available() ){
      if (micros() - started_waiting_at > 200000 ){
          timeout = true;
          break;
      }      
    }
        
    if (timeout) {
        Serial.println(F("Failed, response timed out."));
        digitalWrite(4, LOW);
    } else {
        unsigned long got_time;
        radio.read( &got_time, sizeof(unsigned long) );
        unsigned long end_time = micros();
        Serial.print(F("Sent "));
        Serial.print(start_time);
        Serial.print(F(", Got response "));
        Serial.print(got_time);
        Serial.print(F(", Round-trip delay "));
        Serial.print(end_time-start_time);
        Serial.println(F(" microseconds"));

        digitalWrite(4, HIGH);
    }
}

