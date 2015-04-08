#include <Servo.h>

Servo l;
Servo r;

void setup(){
  r.attach(10);
  l.attach(6); 
  Serial.begin(9600);
  pinMode(3, INPUT);
}

void loop(){
  //fahren();
  /*r.write(180); //vorwärtsfahren rechts (rückwarts: 0)
  l.write(0); //vorwärtsfahren links (rückwarts: 180)
  delay(500);
  r.write(89); //stehen bleiben
  l.write(89); //stehen bleiben
  delay(2000);*/
  
  r.write(180);
  l.write(99);
  delay(500);
  
  r.write(89);
  l.write(99);
  delay(500);
  
  r.write(89);
  l.write(0);
  delay(500);
  
  r.write(89);
  l.write(99);
  delay(500);
  }

