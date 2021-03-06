
// -----------------------------------
// Controlling LEDs over the Internet
// -----------------------------------

/* First, let's create our "shorthand" for the pins
Same as in the Blink an LED example:
led1 is D0, led2 is D7 */

int led = D0;
int buz = D1;
int vib = D2;
int lse = A4;
int but = D3;
int analogvalue;
q
// Last time, we only needed to declare pins in the setup function.
// This time, we are also going to register our Particle function

void setup()
{
    Serial.begin(9600);
   // Here's the pin configuration, same as last time
   pinMode(led, OUTPUT);
  pinMode(buz, OUTPUT);
  pinMode(vib, OUTPUT);
  pinMode(lse, INPUT);
  pinMode( but , INPUT_PULLUP);

   // We are also going to declare a Particle.function so that we can turn the LED on and off from the cloud.
   Particle.function("led",ledToggle);
   // This is saying that when we ask the cloud for the function "led", it will employ the function ledToggle() from this app.

   // For good measure, let's also make sure both LEDs are off when we start:
   digitalWrite(led, LOW);

   Particle.variable("analogvalue", &analogvalue, INT);


}


/* Last time, we wanted to continously blink the LED on and off
Since we're waiting for input through the cloud this time,
we don't actually need to put anything in the loop */

void loop()
{
    analogvalue = analogRead(but);
//    digitalWrite(led,HIGH);
      // find out if the button is pushed
   // or not by reading from it.
   int buttonState = digitalRead( but );

  // remember that we have wired the pushbutton to
  // ground and are using a pulldown resistor
  // that means, when the button is pushed,
  // we will get a LOW signal
  // when the button is not pushed we'll get a HIGH

  // let's use that to set our LED on or off

  if( buttonState == LOW )
  {
      Particle.publish("pushed");
    // turn the LED On
    digitalWrite( led, HIGH);
    analogWrite(buz, 90);
  }else{
    // otherwise
    // turn the LED Off
    digitalWrite( led, LOW);
    analogWrite(buz, 0);

  }
 // Particle.publish(led);
}

// We're going to have a super cool function now that gets called when a matching API request is sent
// This is the ledToggle function we registered to the "led" Particle.function earlier.

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
// //        analogWrite(buz, 127);
//         delay(500);
// //        analogWrite(buz, 155);
//         delay(500);
// //        analogWrite(buz, 90);
// //        tone(buz,500);
//         delay(500);
//        analogWrite(vib,127);
        return 1;
    }
    else if (command=="off") {
        digitalWrite(led,LOW);
//        analogWrite(buz, 0);
//        analogWrite(vib,0);
        return 0;
    }
    else {
        return -1;
    }
}
