#include <Servo.h>
int ena = 36;
int in1 = 35;
int in2 = 34;
int enb = 31;
int in3 = 33;
int in4 = 32;
Servo servo1;
Servo servo2;
void setup() {
  Serial.begin(115200);
  Servo1.attache(29);
  Servo2.attache(30);
  // put your setup code here, to run once:
pinMode(ena,OUTPUT);
pinMode(in1,OUTPUT);
pinMode(in2,OUTPUT);
pinMode(enb,OUTPUT);
pinMode(in3,OUTPUT);
pinMode(in4,OUTPUT);
pinMode(30,OUTPUT);
}
void vpered(){
digitalWrite(in1,1);  
digitalWrite(in2,0);
analogWrite(ena,150);
digitalWrite(in3,0);  
digitalWrite(in4,1);
analogWrite(enb,150);
}
void Back(){
digitalWrite(in1,0);  
digitalWrite(in2,1);
analogWrite(ena,150);
digitalWrite(in3,1);  
digitalWrite(in4,0);
analogWrite(enb,150);
}
void stop(){
digitalWrite(in1,0);  
digitalWrite(in2,0);
analogWrite(ena,0);
digitalWrite(in3,0);  
digitalWrite(in4,0);
analogWrite(enb,0);
}
void vpravo(){
digitalWrite(in1,1);  
digitalWrite(in2,0);
analogWrite(ena,150);
digitalWrite(in3,1);  
digitalWrite(in4,0);
analogWrite(enb,150);
}
void vlevo(){
digitalWrite(in1,0);  
digitalWrite(in2,1);
analogWrite(ena,150);
digitalWrite(in3,0);  
digitalWrite(in4,1);
analogWrite(enb,150);
}

void loop() {
  if (Serial.available()) {
    if (Serial.read()=='FORWARD'){
      vpered();
    }
    if (Serial.read()=='STOP'){
      stop();
    }
    if (Serial.read()=='RIGHT'){
      vpravo();
    }
    if (Serial.read()=='LEFT'){
      vlevo();
    }
    if (Serial.read()=='GRAB'){
      Servo1.write(10);
      delay(500);
      Servo2.write(100);

    }
    if (Serial.read()=='DROP'){
      Servo1.write(120);
      delay(500);
      Servo2.write(10);
    }
    if (Serial.read()=='BACK'){
      Back();
    }
    if (Serial.read()=='SCAN'){
      vlevo();
    }
    delay(50);
  }

}