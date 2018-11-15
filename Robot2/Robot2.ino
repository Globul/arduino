/* Robot2

// Coordinates
// 0,0 is the cuisse joint
// y is forward(+) backward(-) axe
// z is up down axe. Under the cuisse joint is negative
*/

#include <Servo.h>

#define SERVOECARTE 11
#define SERVOROTA   10
#define SERVOPIED   9

Servo ServoEcart;  // create servo object to control a servo
Servo ServoRota;  // create servo object to control a servo
Servo ServoPied;  // create servo object to control a servo

int LgPied = 65;
int AngCuissePied = 20; // angle du servo 0 par rapport à la verticale
int LgCuisse = 68;
int AngVertCuisse = 90; // angle du servo 0 par rapport à la verticale
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

double Sin(double a)
{
	return sin(a*M_PI/180);
}

double Cos(double a)
{
	return cos(a*M_PI/180);
}

double Acos(double a)
{
	return acos(a)*180/M_PI;
}

int angleToCoord(int e, int r, int p)
{
  double yr, zr, yp, zp, y, z;

  yr = Cos(r)*LgCuisse;
  zr = Sin(-r)*LgCuisse;
  printf("yr=%f zr=%f\n", yr, zr);
  y = yr+Sin(180-(r-90)-(AngCuissePied+p))*LgPied;
  z = zr-Cos(180-(r-90)-(AngCuissePied+p))*LgPied;
  printf("y=%f z=%f\n", y, z);
}

// return coord corresponding to angles set for servos
int getAnglePied(int z, int r)
{
  double zr;
  int p;

  zr = Sin(-r)*LgCuisse;
  p = 270-r-AngCuissePied-Acos(-(z-zr)/LgPied);
  printf("getAnglePied(%d, %d) => zr=%f p=%d\n", z, r, zr, p);
/*
  printf("180-(r-90)-(AngCuissePied+p)=%d\n", 180-(r-90)-(AngCuissePied+60));
  printf("cos(180-(r-90)-(AngCuissePied+p))=%f\n",Cos(180-(r-90)-(AngCuissePied+60)));
  printf("cos()*LgPied=%f\n",Cos(180-(r-90)-(AngCuissePied+60))*LgPied);
  zr = Sin(-r)*LgCuisse;
  z = zr-Cos(180-(r-90)-(AngCuissePied+p))*LgPied;
  z = zr-Cos(270-r-AngCuissePied-p)*LgPied;
  Cos(270-r-AngCuissePied-p) = (zr-z)/LgPied
  270-r-AngCuissePied-p = acos((zr-z)/LgPied)
  p = 270-r-AngCuissePied-acos((zr-z)/LgPied)
*/
  return p;
}


void loop() {
  int posr, posp;

  for (posr=60; posr<120; posr++)
  {
     posp = getAnglePied(-45, posr);
     ServoRota.write(posr);
     ServoPied.write(posp);
     delay(100);
  }

  for (posr=120; posr>60; posr--)
  {
     posp = getAnglePied(-35, posr);
     ServoRota.write(posr);
     ServoPied.write(posp);
     delay(100);
  }

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

