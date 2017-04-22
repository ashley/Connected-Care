//sensor.ino
/*
Hookup guide:
Buzzer: D3
LED:    D2
Vibe:   D1
*/

int led = D0;
int buz = D1;
int vib = D2;
int lse = A4;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(buz, OUTPUT);
  pinMode(vib, OUTPUT);
  pinMode(lse, INPUT);
  Particle.function("led",ledToggle);
}

void loop() {
  int store;
  analogWrite(buz, 127);
  digitalWrite(led, LOW);
  analogWrite(vib,127);
  store = analogRead(lse);
  Serial.println(store);
  delay(1000);
  analogWrite(buz, 0);
  digitalWrite(led, LOW);
  analogWrite(vib,0);
  delay(1000);
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
