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
}

void loop() {
  int store;
  analogWrite(buz, 127);
  digitalWrite(led, HIGH);
  analogWrite(vib,127);
  store = analogRead(lse);
  Serial.println(store);
  delay(1000);
  analogWrite(buz, 0;
  digitalWrite(led, LOW);
  analogWrite(vib,0);
  delay(1000);
}
