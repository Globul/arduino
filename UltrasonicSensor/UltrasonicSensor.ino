/*
* Ultrasonic Sensor HC-SR04 and Arduino Tutorial
*
* Crated by Dejan Nedelkovski,
* www.HowToMechatronics.com
*
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// defines sensor pins numbers
//const int TrigPin = 9;
//const int EchoPin = 10;
const int TrigPin = 2;
const int EchoPin = 3;

// LCD Display connection :
// LCD --- Arduino
// GND --- GND
// VCC --- 5V
// SDA --- A4
// SDL --- A5
#define LCDWIDTH  16
#define LCDHEIGHT 2

LiquidCrystal_I2C lcd(0x27, LCDWIDTH, LCDHEIGHT);
// defines variables
long  Duration;
int   Distance;

void printlcd(int c, int l, char *str)
{
  char disp[LCDWIDTH+1];

  memset(disp, ' ', LCDWIDTH);
  strncpy(disp, str, strlen(str));
  disp[sizeof(disp)-1] = '\0';
  lcd.setCursor(c, l);
  lcd.print(disp);
}

void setup()
{
  // sensor init
  pinMode(TrigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(EchoPin, INPUT); // Sets the echoPin as an Input

  Wire.begin();
  // The begin call takes the width and height. This
  // Should match the number provided to the constructor.
  lcd.begin(LCDWIDTH,LCDHEIGHT);
  lcd.init();

  // Turn on the backlight.
  lcd.backlight();

  printlcd(0, 0, "Ultrasonic test");
  Serial.begin(115200);
}


void loop() {
  char  tmp[40];
  
  // Clears the trigPin
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  Duration = pulseIn(EchoPin, HIGH);
  
  // Calculating the distance in cm
  Distance= Duration*0.034/2;
  
  // Prints the distance on the Serial Monitor
  sprintf(tmp, "Dist: %d cm", Distance);
  printlcd(0, 1, tmp);
  Serial.println(tmp);

  delay(500);
}

