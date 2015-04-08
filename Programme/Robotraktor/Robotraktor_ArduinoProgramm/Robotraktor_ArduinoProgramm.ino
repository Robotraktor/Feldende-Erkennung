#define motorLinks 6
#define motorRechts 5
#define sensorLinks 10
#define sensorRechts 11
#define trig 8
#define echo 7

#include <Servo.h>

volatile boolean kurveRechts;
volatile boolean kurveLinks;
volatile boolean anfang;
volatile boolean feldende;
volatile char letzteKurve;

Servo rechts;
Servo links;

volatile unsigned long alteZeit=0, entprellZeit = 1000;

void setup()
{
  Serial.begin(9600);
  attachInterrupt(0, ISRendeErkennen, FALLING);
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
  if(!feldende)
  {
    //Der Roboter soll stoppen, wenn ein Hindernis zwischen 1 und 10cm steht
    if(entfernungMessen()<=10 && entfernungMessen() >= 1) // 1cm weil: Wenn kein Hindernis vorhanden ist, dann beträgt entfernungMessen einen Wert zwischen 0 und 1
    {
      stoppen();
      delay(1000);
    }else
    {
    fahren();
    }
  }else  
  {
    if(kurveRechts && !kurveLinks)
    {
     //Serial.println("Rechtskurve");
     rechtsKurveFahren(); 
     delay(1000);
    }else if(kurveLinks && !kurveRechts)
    {
      //Serial.println("Linkskurve");
      linksKurveFahren();
      delay(1000);
    }else
    {
     //Serial.println("Stoppen");
     stoppen(); 
     delay(5000);
    }
    feldende = false;
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
  for(int i = 110; i >= 105; i--)
  {
    rechts.write(0);
    links.write(i);
    delay(500);
  }
}

void rechtsKurveFahren()
{
  for(int i = 80; i <= 83; i++)
  {
    rechts.write(i);
    links.write(180);
    delay(500);
  }
}

//Interrupt-Routine
void ISRendeErkennen()
{
  if((millis() - alteZeit) > entprellZeit) //Entprellen des Sensors
  {
  feldende = true; //Erkennung des Feldendes
  if(anfang)
  {
    //Wenn der Sensor links ein Signal bekommt, soll der Roboter eine Rechtskurve fahren
    // weil durch den Interrupt vorne das Feldende schon gegeben ist
    if (analogRead(A2) <= 200) //A2 entspricht Sensor links
    {
      kurveLinks = false;
      kurveRechts = true;
      letzteKurve = 'r';
    }
    //Gleich wie oben, nur mit rechtem Sensor und Linkskurve
    else if(analogRead(A0) <= 200) //A0 entspricht Sensor rechts
    {
      kurveRechts = false;
      kurveLinks = true;
      letzteKurve = 'l';
    }
    //Wenn Sensor rechts und links ein Signal bekommen, dann soll er stehen bleiben
    else if(analogRead(A2) <= 200 && analogRead(A0) <= 200)
    {
      kurveRechts = false;
      kurveLinks = false;
     
    }else //Default-Wert ist eine Rechtskurve 
    {
      kurveLinks = false;
      kurveRechts = true;
      letzteKurve = 'r';
    }
    anfang = false; //Weil dies nur beim ersten Interrupt passieren soll
  }else //Für alle anderen Fälle (ausgenommen der erste Interrupt)
   {
     if (analogRead(A2) >= 600 && analogRead(A0) >= 600) //Abwechselndes Kurvenfahren
     {
       //kurveRechts = !kurveRechts;
       //kurveLinks = !kurveLinks;
       
       if(letzteKurve == 'r')
       {
         kurveLinks = true;
         kurveRechts = false;
         letzteKurve = 'l';
       }else if(letzteKurve == 'l')
       {
         kurveLinks = false;
         kurveRechts = true;
         letzteKurve = 'r';
       }
     }else //Ende des Feldes
      {
       kurveRechts = false;
       kurveLinks = false;
      }
   }
   alteZeit = millis();
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

