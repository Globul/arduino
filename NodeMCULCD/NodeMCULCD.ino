#include <LiquidCrystal_I2C.h>

// Construct an LCD object and pass it the 
// I2C address, width (in characters) and
// height (in characters). Depending on the
// Actual device, the IC2 address may change.
//LiquidCrystal_I2C lcd(0x3F, 16, 2);
LiquidCrystal_I2C lcd(0x27, 16, 2);


// DISPLAY I2C 16x2 characters
// Display --- NodeMCU
//   GND   ---   GND
//   VCC   ---   Vin
//   SDA   ---   D2
//   SCL   ---   D1


const int Btn1 = 14;
const int Btn2 = 12;
const int Btn3 = 13;
const int Btn4 = 15;

void setup() {

  pinMode(Btn1, INPUT);
  pinMode(Btn2, INPUT);
  pinMode(Btn3, INPUT);
  pinMode(Btn4, INPUT);  

  // use mode HIGH for the buttons to limit the bounds
/*  digitalWrite(Btn1, HIGH);
  digitalWrite(Btn2, HIGH);
  digitalWrite(Btn3, HIGH);
  digitalWrite(Btn4, HIGH);*/
  
  // The begin call takes the width and height. This
  // Should match the number provided to the constructor.
  lcd.begin(16,2);
  lcd.init();

  // Turn on the backlight.
  lcd.backlight();

  // Move the cursor characters to the right and
  // zero characters down (line 1).
  lcd.setCursor(5, 0);

  // Print HELLO to the screen, starting at 5,0.
  lcd.print("HELLO");

  // Move the cursor to the next line and print
  // WORLD.
  lcd.setCursor(5, 1);      
//  lcd.print("WORLD");
}

void loop() {
  lcd.setCursor(1, 1);
  if (digitalRead(Btn1) == HIGH)
  {
    lcd.print("HIGH");
  }
  else
  {
    lcd.print("LOW ");
  }

  lcd.setCursor(10, 1);
  if (digitalRead(Btn2) == HIGH)
    lcd.print("HIGH");
  else
    lcd.print("LOW ");

  delay(100);
#if 0
// Turn on the backlight.
  lcd.backlight();
  delay(1000);
  lcd.noBacklight();
  delay(1000);
#endif
 }
