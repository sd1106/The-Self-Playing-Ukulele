#include <Stepper.h>
#include <ESP32Servo.h>

Stepper motorG(2048, 7, 5, 6, 4);
Stepper motorC(2048, 10, 15, 16, 17);
Stepper motorE(2048, 38, 39, 40, 41);
Stepper motorA(2048, 45, 46, 47, 48);

bool strumState = false;
bool once = true; 

Servo pressG;
Servo strumG;
Servo pressC;
Servo strumC;
Servo pressE;
Servo strumE;
Servo pressA;
Servo strumA;

void setup() {
  motorG.setSpeed(15);
  motorC.setSpeed(15);
  motorE.setSpeed(15);
  motorA.setSpeed(15);

  pressG.attach(1);
  strumG.attach(2);
  
  pressC.attach(8);
  strumC.attach(9);
  pressE.attach(18);
  strumE.attach(21);
  pressA.attach(42);
  strumA.attach(3);

  pressG.write(0);
  strumG.write(0);
  pressC.write(0);
  strumC.write(0);
  pressE.write(0);
  strumE.write(0);
  pressA.write(0);
  strumA.write(0);
  delay(1000);
  strumC.write(60);
}
void loop() {
    if(once)
    {
        riptide();
        once = false;
    }
}

void riptide(){
    Aminor();
    strummingRhythm();
    pressG.write(0);
    delay(300);
    
    G();
    strummingRhythm();
    pressC.write(0);
    pressE.write(60);
    pressA.write(60);
    delay(300);
    
    C();
    strummingRhythm();
    pressA.write(60);
    delay(300);
}

void strummingRhythm(){
    strumAll();
    delay(1000);
    strumAll();
    delay(1000);

    strumAll();
    delay(500);
    strumAll();
    delay(500);
    strumAll();
    delay(500);
}

void strumAll(){
    int targetAngle;
    if(strumState == false)
    {
        targetAngle = 90;
        strumState = true;
    }
    else
    {
        targetAngle = 0;
        strumState = false;
    }
    strumG.write(targetAngle);
    strumC.write(targetAngle);
    strumE.write(targetAngle);
    strumA.write(targetAngle);
}

void Aminor(){
    motorG.step(-6000);
    pressG.write(60);
    delay(300);
}

void G(){
    motorC.step(-6000);
    motorE.step(5000);
    motorA.step(6000);
    pressC.write(120);
    pressE.write(0);
    pressA.write(0);
    delay(300);
}

void GOpposite(){
    motorC.step(-6000);
    motorE.step(-5000);
    motorA.step(-6000);
    pressC.write(0);
    pressE.write(60);
    pressA.write(60);
    delay(300); 
}

void C(){
    motorA.step(-1000);
    pressA.write(0);
    delay(300);
}