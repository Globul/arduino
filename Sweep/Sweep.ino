/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

#define SERVOECARTE 11
#define SERVOROTA   10
#define SERVOPIED   9

Servo ServoEcart;  // create servo object to control a servo
Servo ServoRota;  // create servo object to control a servo
Servo ServoPied;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  ServoEcart.attach(SERVOECARTE);  // attaches the servo on pin 9 to the servo object
  ServoRota.attach(SERVOROTA);  // attaches the servo on pin 9 to the servo object
  ServoPied.attach(SERVOPIED);  // attaches the servo on pin 9 to the servo object

  ServoEcart.write(90);
  delay(1000);
  ServoRota.write(150);
  delay(1000);
  ServoPied.write(0);
  delay(1000);
}

angleToCoord(int e, int r, int p)
{
  int lgPied = 65;
  int angVertPied = 20; // angle du servo 0 par rapport à la verticale
  int lgCuisse = 68;
  int angVertCuisse = 20; // angle du servo 0 par rapport à la verticale

  70 => y = lgPied
  160 => y = 0
  180 => y < 0
  y = cos(-90+lgPied+p)*lgPied;
  z = sin(90+lgPied+p)*lgPied;
}


void loop() {
  int posr, posp;

  return;
  ServoRota.write(130);
  ServoPied.write(20);              // tell servo to go to position in variable 'pos'
  delay(3000);                       // waits 15ms for the servo to reach the position
  
  for (posr = 130; posr >= 50; posr -= 1) { // goes from 0 degrees to 180 degrees
    ServoRota.write(posr);              // tell servo to go to position in variable 'pos'

    if (posr > 90)
      posp = 20 - (130 - posr)/2;
    else
      posp = 0 + (90 - posr)/2;
    ServoPied.write(posp);              // tell servo to go to position in variable 'pos'
    delay(50);                       // waits 15ms for the servo to reach the position
  }

  return;
  for (posr = 50; pos <= 130; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    ServoRota.write(posr);              // tell servo to go to position in variable 'pos'
    delay(50);                       // waits 15ms for the servo to reach the position
  }

  ServoEcart.write(60);
  delay(3000);
  ServoEcart.write(120);
  delay(3000);
  ServoEcart.write(90);
  delay(3000);
  ServoRota.write(60);
  delay(3000);
  ServoRota.write(120);
  delay(3000);
  ServoRota.write(90);
  delay(3000);
  ServoPied.write(60);
  delay(3000);
  ServoPied.write(120);
  delay(3000);
  ServoPied.write(90);
  delay(3000);
  return;

#if 0  
  for (pos = 0; pos <= 160; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 160; pos <= 200; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  delay(10000);
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  delay(10000);
#endif
}

