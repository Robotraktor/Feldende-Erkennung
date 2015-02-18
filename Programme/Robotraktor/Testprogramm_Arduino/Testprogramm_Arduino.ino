#define motorLinks 6
#define motorRechts 5
#define sensorLinks 10
#define sensorRechts 11
#define trig 8
#define echo 2

#include <Servo.h>

volatile boolean kurveRechts;
volatile boolean kurveLinks;
volatile boolean anfang;
volatile boolean feldende;

Servo rechts;
Servo links;

void setup()
{
  Serial.begin(9600);
  attachInterrupt(0, ISRendeErkennen, LOW);
  rechts.attach(motorRechts);
  links.attach(motorLinks);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  anfang = true;
  feldende = false;
  kurveRechts = false;
  kurveLinks = false;
  stoppen();
}

void loop()
{ 
  /*for(int i = 80; i <= 83; i++)
  {
    rechts.write(i);
    links.write(180);
    delay(620);
  }
  stoppen();
  delay(2000);*/
  
  if(feldende)
  {
    if(kurveRechts && !kurveLinks)
    {
     rechtsKurveFahren(); 
     delay(1000);
    }else if(kurveLinks && !kurveRechts)
    {
      linksKurveFahren();
      delay(1000);
    }else
    {
     stoppen(); 
     delay(5000);
    }
    feldende = false;
  }else
  {
   fahren(); 
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
}

void rechtsKurveFahren()
{
    rechts.write(89);
    links.write(180); 
}

void ISRendeErkennen()
{
  feldende = true;
  if(anfang)
  {
    if (analogRead(A2) == LOW)
    {
      kurveLinks = false;
      kurveRechts = true;
    }
    else if(analogRead(A0) == LOW)
    {
      kurveRechts = false;
      kurveLinks = true;
    }
    else if(analogRead(A2) == LOW && analogRead(A0) == LOW)
    {
      kurveRechts = false;
      kurveLinks = false;
    }else
    {
      kurveLinks = false;
      kurveRechts = true;
    }
    anfang = false; 
  }else
   {
     if (analogRead(A2) == LOW || analogRead(A0) == LOW)
     {
       kurveRechts = false;
       kurveLinks = false;
     }else
      {
       kurveRechts = !kurveRechts;
       kurveLinks = !kurveLinks;
      }
   }
  
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

