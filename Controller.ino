#include <SPI.h>
#include "RF24.h"

RF24 radio(6,8);

byte addresses[][6] = {"1Node","2Node"};

void setup() {
  Serial.begin(115200);
  Serial.println(F("This is the REMOTE CONTROL script"));
  
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
//Package lastPackage;
//bool sent = false;

//bool isPackageDifferent(int room) {
//  bool x = p.x < lastPackage.x + room && p.x > lastPackage.x - room;
//  bool y = p.y < lastPackage.y + room && p.y > lastPackage.y - room;
//  return !(x || y);
//}

void loop() {
  float xVal1 = analogRead(0);
  float yVal1 = analogRead(1);
  
  p.x = xVal1;
  p.y = yVal1;

  Serial.print(p.x);
  Serial.print(" || ");
  Serial.println(p.y);

//  Serial.print(" ||| ");
//
//  Serial.print(lastPackage.x);
//  Serial.print(" || ");
//  Serial.print(lastPackage.y);
//  Serial.print(" ");
//
//  Serial.print(" ");
//  Serial.println(isPackageDifferent(20));

//  if (isPackageDifferent(20)) {
    radio.stopListening();
    Serial.println(F("Now sending"));
  
    unsigned long start_time = micros();
    bool worked = radio.write( &p, sizeof(p));
//    lastPackage = p;
        
    radio.startListening();
    
    unsigned long started_waiting_at = micros();
    boolean timeout = false;
    
    while (!radio.available()) {
      if (micros() - started_waiting_at > 200000 ){
        timeout = true;
        break;
      }      
    }
        
    if (timeout) {
      Serial.println(F("Failed, response timed out."));
//      sent = false;
    } else {
      unsigned long got_time;
      radio.read( &got_time, sizeof(unsigned long) );
      unsigned long end_time = micros();
      
      // Spew it
      Serial.print(F("Sent "));
      Serial.print(start_time);
      Serial.print(F(", Got response "));
      Serial.print(got_time);
      Serial.print(F(", Round-trip delay "));
      Serial.print(end_time-start_time);
      Serial.println(F(" microseconds"));
//      sent = true;
    }
//  }
  
  

} // Loop
