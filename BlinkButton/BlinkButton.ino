/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

const int Btn = 2;  // button on pin 2
const int Myled = 8;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(Myled, OUTPUT);
  // button
  pinMode(Btn, INPUT);
  // use mode HIGH for the button to limit the bounds
  digitalWrite(Btn, HIGH);
  
}

// the loop function runs over and over again forever
void loop() {
  static int freq=1000;
  static int prevst=HIGH;
  int st;
  
  if ((st = digitalRead(Btn)) != prevst && st == LOW)
    freq /= 2;
  if (freq < 100)
    freq = 1000;
   
  prevst = st;

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(Myled, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(freq);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(Myled, LOW);    // turn the LED off by making the voltage LOW
  delay(freq);                       // wait for a second
}
