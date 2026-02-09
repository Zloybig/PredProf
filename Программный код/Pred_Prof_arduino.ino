#include <Servo.h>


const int ENA = 36; const int IN1 = 35; const int IN2 = 34;
const int IN3 = 33; const int IN4 = 32; const int ENB = 31;
Servo claw;
Servo lifter; 
const int CLAW_OPEN = 90;   const int CLAW_CLOSE = 160;
const int LIFT_DOWN = 170;  const int LIFT_UP = 20;

void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);
  
  claw.attach(30);   claw.write(CLAW_OPEN);
  lifter.attach(29); lifter.write(LIFT_UP);
  stopMotors();
}

void drive(int pwrL, int pwrR) {
  digitalWrite(IN1, pwrL >= 0 ? HIGH : LOW);
  digitalWrite(IN2, pwrL >= 0 ? LOW : HIGH);
  analogWrite(ENA, abs(pwrL));
  
  digitalWrite(IN3, pwrR >= 0 ? HIGH : LOW);
  digitalWrite(IN4, pwrR >= 0 ? LOW : HIGH);
  analogWrite(ENB, abs(pwrR));
}

void stopMotors() { drive(0, 0); }

void loop() {
  if (Serial.available() > 0) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "FORWARD") drive(150, 150);
    else if (cmd == "LEFT")    drive(-130, 130);
    else if (cmd == "RIGHT")   drive(130, -130);
    else if (cmd == "SCAN")    drive(110, -110);
    else if (cmd == "STOP")    stopMotors();
    else if (cmd == "PICK") {
      stopMotors();
      lifter.write(LIFT_DOWN); delay(800);
      claw.write(CLAW_CLOSE);  delay(800);
      lifter.write(LIFT_UP);   delay(800);
    }
    else if (cmd == "DROP") {
      stopMotors();
      lifter.write(LIFT_DOWN); delay(800);
      claw.write(CLAW_OPEN);   delay(800);
      lifter.write(LIFT_UP);   delay(800);
    }
  }
}