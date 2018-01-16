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

#if 0
static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;
#endif

static const uint8_t LEDMODE  = 0;
static const uint8_t LED  = 16;

// 16 = LED near usb connector
// 2 = LED near antenna

// the setup function runs once when you press reset or power the board
void setup() {
  int i;
  // initialize digital pin LED_BUILTIN as an output.
//  pinMode(LED_BUILTIN, OUTPUT);
//  pinMode(LEDMODE, OUTPUT);
//  for (i=0; i<17; i++)
//    pinMode(i, OUTPUT);  
    pinMode(LED, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  static int val=0, cnt=0;

  cnt++;
  if (cnt % 3 == 0)
    val = (val+1)%17;
    
//  digitalWrite(LEDMODE, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
