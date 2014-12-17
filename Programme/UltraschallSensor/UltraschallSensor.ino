#define trig 8
#define echo 7

void setup()
{
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  
}

void loop()
{
  long cm = entfernungMessen();
  if(cm <= 100)
  {
    Serial.print(cm);
    Serial.println("cm");
  }
  else Serial.println("Kein Hindernis");
  delay(1000); 
} 

long entfernungMessen()
{
  long duration;
  
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  
  return microsecondsToCentimeters(duration);
  
}


long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
