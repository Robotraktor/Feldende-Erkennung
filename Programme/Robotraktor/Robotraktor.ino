#define motorLinks 6
#define motorRechts 5
#define sensorLinks 10
#define sensorRechts 11
#define trig 8
#define echo 7

int l;
int r;
int v;

#include <Servo.h>

volatile boolean kurve = true; 
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
  /*if(entfernungMessen() <= 10)
  {
    stoppen();
    delay(2000);
  }*/
  
  
  /*if(feldende)
  {
    stoppen();
    delay(500);
    kurveFahren(kurve);
    delay(2000);
    feldende = false;
  }
  */
  
  //fahren();
 /*
  Serial.print(entfernungMessen());
  Serial.print("\t");
  if(entfernungMessen() <= 10)
  {
    stoppen();
  }else{
  fahren();}
 */
   

   if(analogRead(A4) >= 400)
  {
    fahren();
  } 
  else if(analogRead(A4) <= 400 && (analogRead(A2) <= 400 || kurve))
  {
    rechtsKurveFahren();
    delay(2000);
  }
  else if(analogRead(A4) <= 400 && (analogRead(A0) <= 400 || !kurve))
  {
    linksKurveFahren();
    delay(2000);
  }
  else
  {
    stoppen();
    delay(5000);
  }
  
   
   /*
   if (analogRead(A4) <= 400) //Sensor vorne
  {
    stoppen();
    delay(5000);
  }   
  else if (analogRead(A0) <= 400) //Rechts
  {
    linksKurveFahren();
    delay(1000);
    fahren();
  }
    
 else if (analogRead(A2) <= 400) //Links
  {
    rechtsKurveFahren();
    delay(2000);
  } 
  else {
    fahren();
  }
  
  */

  


  
  
  
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

void kurveFahren(boolean isKurve)
{
    if(isKurve /*|| analogRead(sensorRechts) == 0*/)
    {
      linksKurveFahren();
    }else if(!isKurve /*|| analogRead(sensorLinks) == 0*/)
    {
      rechtsKurveFahren();
    }else
    {
      Serial.println("FEHLER");
      stoppen();
    }
}

//Umdrehungen im Verhältnis
//  r : (r+x) ... r=Innenradius; x=Abstand von Innnen- zu Außenradius
void linksKurveFahren()
{
    rechts.write(0);
    links.write(99);
    
    kurve = true;
}

void rechtsKurveFahren()
{
    rechts.write(89);
    links.write(180); 
    
    kurve = false;
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


int SensorErkennungLinks()
{
  l = analogRead(A2);
}

int SensorErkennungRechts()
{
   r = analogRead(A0);
}
int SensorErkennungVorne()
{
   v = analogRead(A4);
}

