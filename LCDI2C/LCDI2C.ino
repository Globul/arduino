#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD Display connection :
// LCD --- Arduino
// GND --- GND
// VCC --- 5V
// SDA --- A4
// SDL --- A5
#define LCDWIDTH  16
#define LCDHEIGHT 2

// Construct an LCD object and pass it the 
// I2C address, width (in characters) and
// height (in characters). Depending on the
// Actual device, the IC2 address may change.
//LiquidCrystal_I2C lcd(0x3F, 16, 2);
LiquidCrystal_I2C lcd(0x27, LCDWIDTH, LCDHEIGHT);

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
  Wire.begin();
  // The begin call takes the width and height. This
  // Should match the number provided to the constructor.
  lcd.begin(LCDWIDTH,LCDHEIGHT);
  lcd.init();

  // Turn on the backlight.
  lcd.backlight();

  // Print to the screen, starting at 5,0.
  printlcd(5, 0, "BONJOUR");
  printlcd(5, 1, "MAITRE");
}

 void loop()
{
}


 
 
