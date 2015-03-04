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
  rechts.write(89); //Durch Probieren kamen wir auf diese Werte
  links.write(99);  //Angegeben sind eigentlich für Stillstand 90
}

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

//Interrupt-Routine
void ISRendeErkennen()
{
  feldende = true; //Erkennung des Feldendes
  if(anfang)
  {
    //Wenn der Sensor links ein Signal bekommt, soll der Roboter eine Rechtskurve fahren
    // weil durch den Interrupt vorne das Feldende schon gegeben ist
    if (analogRead(A2) == LOW) //A2 entspricht Sensor links
    {
      kurveLinks = false;
      kurveRechts = true;
    }
    //Gleich wie oben, nur mit rechtem Sensor und Linkskurve
    else if(analogRead(A0) == LOW) //A0 entspricht Sensor rechts
    {
      kurveRechts = false;
      kurveLinks = true;
    }
    //Wenn Sensor rechts und links ein Signal bekommen, dann soll er stehen bleiben
    else if(analogRead(A2) == LOW && analogRead(A0) == LOW)
    {
      kurveRechts = false;
      kurveLinks = false;
     
    }else //Default-Wert ist eine Rechtskurve 
    {
      kurveLinks = false;
      kurveRechts = true;
    }
    anfang = false; //Weil dies nur beim ersten Interrupt passieren soll
  }else //Für alle anderen Fälle (ausgenommen der erste Interrupt)
   {
     if (analogRead(A2) == LOW || analogRead(A0) == LOW) //Ende des gesamten Feldes
     {
       kurveRechts = false;
       kurveLinks = false;
     }else //Abwechselndes Kurvenfahren
      {
       kurveRechts = !kurveRechts;
       kurveLinks = !kurveLinks;
      }
   }
  
}

//Ultraschallsensor
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

