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

void setup() {
  pinMode(led, OUTPUT);
  pinMode(buz, OUTPUT);
  pinMode(vib, OUTPUT);
}

void loop() {
  analogWrite(buz, 127);
  digitalWrite(led, HIGH);
  analogWrite(vib,127);
  delay(1000);
  analogWrite(buz, 0;
  digitalWrite(led, LOW);
  analogWrite(vib,0);
  delay(1000);

}
