//#include <lib1.h>
#include <application.h>
#include <spark_wiring_i2c.h>

#define Addr 0x40

unsigned long lastTime = 0UL;
char publishString[40];
bool fear[200];
float cTemperature = 0.0;
double cTemp = 0.0, fTemp = 0.0;
float pulse_SPOOF = 120;
int danger_state = 1;

int led = D6;
int buz = A7;
int vib = D7;
int but = D3;
int lse = A4;
int temp = D1;

int photoValue;
int buttonValue;

int photoresistor = A0;
int power = A5;
bool clicked = false;


void setup()
{
    Particle.function("led",ledToggle);
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
//   setupWires();

    // Set variable
    Particle.variable("i2cdevice", "TMP007");
    Particle.variable("cTemp", cTemp);

    // Initialise I2C communication as MASTER
    Wire.begin();
    // Initialise serial communication, set baud rate = 9600
    Serial.begin(9600);

    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select configuration register
    Wire.write(0x02);
    // Send config command
    Wire.write(0x15);
    Wire.write(0x40);
    // Stop I2C Transmission
    Wire.endTransmission();
    delay(300);
}

void loop()
{
    // photoValue = analogRead(photoresistor);

    buttonValue = digitalRead(but);
    cTemp = findTemp();
    int fear = calculateFear(cTemp, pulse_SPOOF);
    if (fear>0 && danger_state>0) {
        digitalWrite(led,HIGH);
        analogWrite(buz, 10);
//        tone(buz,50);
        Particle.publish("Update", "danger");
        analogWrite(vib,127);
    } else if (fear == 0){
        Particle.publish("NEW Update", "NO danger");
        digitalWrite(led,LOW);
        analogWrite(buz, 0);
        tone(buz,0);
        analogWrite(vib,0);
    }
    Particle.publish("fear level", String(fear));
    Particle.publish("danger ready", String(danger_state));
//    Particle.publish("temppppp", String(cTemp));
    sprintf(publishString,"%d, %f, %i", clicked, cTemp, fear);
    Spark.publish("ButtonClick",publishString);
    unsigned long now = millis();
    //Every 5 seconds publish uptime
    // if (now-lastTime>5000UL && !clicked) {
    //     lastTime = now;
    //     // now is in milliseconds
    //     unsigned nowSec = now/1000UL;
    //     if (clicked){
    //     	sprintf(publishString,"Clicked, %u and it's %f",nowSec, cTemp);

    //     }
    //     else{
    //     	sprintf(publishString,"Not Clicked and it's %f",nowSec, cTemp);
    //     }
    //     Spark.publish("ButtonClick",publishString);
    // }

    if( buttonValue == LOW )
    {
      clicked = true;
      Particle.publish("Update", "Clicked, and temperature is " + String(cTemp));
      digitalWrite( led, HIGH);
      if (danger_state > 0) {
        danger_state = 0;
      } else if (danger_state == 0){
          danger_state = 0;
      }
    }
    else{
      clicked = false;
//      Particle.publish("Update", "Not clicked, and temperature is " + String(cTemp));
    }
    delay(500);
}

// void setupWires(){
//   Wire.begin();
//   Serial.begin(9600);
//   Wire.beginTransmission(Addr);
//   Wire.write(0x02);
//   Wire.write(0x15);
//   Wire.write(0x40);
//   Wire.endTransmission();
// }



double findTemp(){
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
  return ((temp * 0.03125)* 1.8) + 32;
}

int calculateFear(float temp, float pulse){
    Particle.publish("fear calculator", String(temp+pulse));
    if (temp+pulse>210) {
        return 1;
    } else {
        return 0;
    }
}

int ledToggle(String command) {
    /* Particle.functions always take a string as an argument and return an integer.
    Since we can pass a string, it means that we can give the program commands on how the function should be used.
    In this case, telling the function "on" will turn the LED on and telling it "off" will turn the LED off.
    Then, the function returns a value to us to let us know what happened.
    In this case, it will return 1 for the LEDs turning on, 0 for the LEDs turning off,
    and -1 if we received a totally bogus command that didn't do anything to the LEDs.
    */

    if (command=="on") {
        digitalWrite(led,HIGH);
        analogWrite(buz, 50);
        delay(500);
        Particle.publish("Update", "danger");
//        analogWrite(buz, 90);
//        tone(buz,500);
        analogWrite(vib,127);
        return 1;
    }
    else if (command=="off") {
        digitalWrite(led,LOW);
        analogWrite(buz, 0);
        analogWrite(vib,0);
        return 0;
    }
    else {
        return -1;
    }
}
