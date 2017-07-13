#include <Stepper.h>
 
// change this to the number of steps on your motor
#define STEPS 200
 
// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
int movespeed = 1;

Stepper stepper(STEPS, 4, 5, 6, 7);
Stepper stepper2(STEPS, 8, 9, 10, 12);
 
 
void setup() {
  Serial.begin(9600);
  Serial.println("Stepper test!");
  // set the speed of the motor to 30 RPMs
  stepper.setSpeed(movespeed);
  stepper2.setSpeed(movespeed);
}
 
void loop() {
  Serial.println("Forward");
  stepper.step(STEPS);
  stepper2.step(STEPS);
  Serial.println("Backward");
  stepper2.step(-STEPS);
  stepper.step(-STEPS);
}
