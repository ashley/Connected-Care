#include <lib1.h>
#include <spark_wiring_i2c.h>

#define Addr 0x40

unsigned long lastTime = 0UL;
char publishString[40];
bool fear[200];
float cTemp = 0.0;

int led = D6;
int buz = D7;
int vib = D2;
int but = D3;
int lse = A4;
int temp = A6;

int photoValue;
int buttonValue;

int photoresistor = A0;
int power = A5;
bool clicked = false;


void setup()
{
   Serial.begin(9600);

   pinMode(led, OUTPUT);
   pinMode(buz, OUTPUT);
   pinMode(vib, OUTPUT);
   pinMode(power, OUTPUT);
   pinMode(lse, INPUT);
   pinMode(photoresistor, INPUT);
   pinMode(but, INPUT_PULLUP);

   digitalWrite(power,HIGH);

   Particle.variable("photoValue", &photoValue, INT);
   Particle.variable("buttonValue", &buttonValue, INT);
   Particle.function("led",ledToggle);
   digitalWrite(led, LOW);
   setupWires()
}

void loop()
{
    photoValue = analogRead(photoresistor);
    buttonValue = digitalRead(but);
    loopWires();

    unsigned long now = millis();
    //Every 5 seconds publish uptime
    if (now-lastTime>5000UL && !clicked) {
        lastTime = now;
        // now is in milliseconds
        unsigned nowSec = now/1000UL;
        if (clicked){
        	sprintf(publishString,"Clicked, %u and it's %f",nowSec, cTemp);

        }
        else{
        	sprintf(publishString,"Not Clicked and it's %f",nowSec, cTemp);
        }
        Spark.publish("publishString",publishString);
    }

    if( buttonValue == LOW )
    {
      clicked = true;
      Particle.publish("pushed");
      digitalWrite( led, HIGH);
      analogWrite(buz, 90);
    }
    else{
      clicked = false;
    }
}

void setupWires(){
  Wire.begin();
  Serial.begin(9600);
  Serial.beginTransmission(Addr);
  Wire.write(0x02);
  Wire.write(0x15);
  Wire.write(0x40);
  Wire.endTransmission();
}

void storeData(){
}

bool checkData(){
  return true;
}

int ledToggle(String command) {

    if (command=="on") {
        digitalWrite(led,HIGH);
        return 1;
    }
    else if (command=="off") {
        digitalWrite(led,LOW);
        return 0;
    }
    else {
        return -1;
    }
}

float loopWires(){
  unsigned data[2];
  Wire.beginTransmission(Addr);
  Wire.write(0x03);
  Wire.endTransmission();
  Wire.requestFrom(Addr, 2);

  if(Wire.available() == 2){
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  int temp (((data[0] *256) + (data[1] & 0xFC)) / 4);
  if(temp > 8191){
    temp -= 16384;
  }
  cTemp = temp * 0.03125;
}
