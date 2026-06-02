#include <ESP32Servo.h>

Servo testServo;

void setup() {
  testServo.attach(18); 
}

void loop() {
  testServo.write(40);
  delay(2000);

  testServo.write(100);
  delay(2000);

  testServo.write(40);
  delay(2000);
}