#define motorLinks 6
#define motorRechts 5
#define sensorLinks 10
#define sensorRechts 11
#define trig 8
#define echo 7

#include <Servo.h>

boolean kurveRechts = true;
boolean kurveLinks = false;
volatile boolean feldende = false;

Servo rechts;
Servo links;

void setup()
{
  Serial.begin(9600);
  //attachInterrupt(0, ISRendeErkennen, LOW);
  rechts.attach(motorRechts);
  links.attach(motorLinks);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  stoppen();
}

void loop()
{ 
  if(entfernungMessen() < 20)
  {
    stoppen();
    delay(1000);    
  }
  delay(100);
  
  if(analogRead(A4) >= 400)
  {
    fahren();
  }
  else if(analogRead(A4) <= 400 && (analogRead(A2) <= 400))
  {
    stoppen();
    delay(500);
    rechtsKurveFahren();
    delay(2000);
  }
  else if(analogRead(A4) <= 400 && (analogRead(A0) <= 400))
  {
    stoppen();
    delay(500);
    linksKurveFahren();
    delay(2000);
  }
  else if(analogRead(A4) <= 400 && kurveRechts)
  {
    stoppen();
    delay(500);
    rechtsKurveFahren();
    delay(2000);
  }
  else if(analogRead(A4) <= 400 && kurveLinks)
  {
    stoppen();
    delay(500);
    linksKurveFahren();
    delay(2000);
  }
  else
  {
    stoppen();
    delay(5000);
  }
  
}

void fahren()
{
  rechts.write(0);
  links.write(180);
  
}

void stoppen()
{
  rechts.write(89);
  links.write(99);
}

//Umdrehungen im Verhältnis
//  r : (r+x) ... r=Innenradius; x=Abstand von Innnen- zu Außenradius
void linksKurveFahren()
{
    rechts.write(0);
    links.write(99);
    
    kurveLinks = !kurveLinks;
    kurveRechts = !kurveRechts;
}

void rechtsKurveFahren()
{
    rechts.write(89);
    links.write(180); 
    
    kurveRechts = !kurveRechts;
    kurveLinks = !kurveLinks;
}

void ISRendeErkennen()
{
  feldende = true;
}


double entfernungMessen()
{
  double duration;
  
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  
  return duration/29/2;
  
}

