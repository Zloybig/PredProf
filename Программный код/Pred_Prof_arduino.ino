int ena = 36;
int in1 = 35;
int in2 = 34;
int enb = 31;
int in3 = 33;
int in4 = 32;
#include <Servo.h>
Servo servo;
void setup() {
  servo.attach(29);
  servo.write(200);
  Serial.begin(9600);
  Serial2.begin(115200);
  // put your setup code here, to run once:
pinMode(ena,OUTPUT);
pinMode(in1,OUTPUT);
pinMode(in2,OUTPUT);
pinMode(enb,OUTPUT);
pinMode(in3,OUTPUT);
pinMode(in4,OUTPUT);
pinMode(29,OUTPUT);
pinMode(6,OUTPUT);
pinMode(7,OUTPUT);
}
void vpered(){
digitalWrite(in1,1);  
digitalWrite(in2,0);
analogWrite(ena,250);
digitalWrite(in3,0);  
digitalWrite(in4,1);
analogWrite(enb,250);
}
void stopp(){
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
analogWrite(ena,250);
digitalWrite(in3,1);  
digitalWrite(in4,0);
analogWrite(enb,250);
}
void vlevo(){
digitalWrite(in1,0);  
digitalWrite(in2,1);
analogWrite(ena,250);
digitalWrite(in3,0);  
digitalWrite(in4,1);
analogWrite(enb,250);
}
void n(){
digitalWrite(in1,0);  
digitalWrite(in2,1);
analogWrite(ena,250);
digitalWrite(in3,1);  
digitalWrite(in4, 0);
analogWrite(enb,250);
}

void loop() {
  if (Serial2.available()) {
    
    byte a = Serial2.read();
    Serial.write(a);
    if (a=='f'){
      vpered();
    }
    if (a=='s'){
      n();
    }
    if (a=='r'){
      vpravo();
    }
    if (a=='l'){
      vlevo();
    }
    if(a=='S'){
      stopp();
    }
    if (a=='v'){
      servo.write(120);
    }
    if (a=='n'){
      servo.write(200);
    }

    if (a=='k'){
      digitalWrite(6,HIGH);
      digitalWrite(7,LOW);
      delay(1500);
      digitalWrite(6,LOW);
      digitalWrite(7,LOW);
    }
    if (a=='z'){
      digitalWrite(6,LOW);
      digitalWrite(7,HIGH);
      delay(1500);
      digitalWrite(6,LOW);
      digitalWrite(7,LOW);
    }
    delay(50);
  }

}
