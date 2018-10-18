#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// Front left motor
#define IDL 0
int ENL = 16;
int IN1L = 0;
int IN2L = 2;

// Front right motor
#define IDR  1
int ENR = 15;
int IN1R = 1;//13
int IN2R = 3;//12

// Max value for motor
#define MAXSPEED  1024

// Infrared receptor
int IRPIN=14;

// Ultrasound sensor
//int TRIG=5;
//int ECHO=4;
int TRIG=12;//1
int ECHO=13;//3

#define OLED_RESET LED_BUILTIN
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#define T_CHM 0xa25d
#define T_CH  0x629d
#define T_CHP  0xe21d
#define T_PREV  0x22dd
#define T_NEXT  0x02fd
#define T_PLAY  0xc23d
#define T_VOLM  0xe01f
#define T_VOLP  0xa857
#define T_EQ  0x906f
#define T_0  0x6897
#define T_100  0x9867
#define T_200  0xb04f
#define T_1  0x30cf
#define T_2  0x18e7
#define T_3  0x7a85
#define T_4  0x10ef
#define T_5  0x38c7
#define T_6  0x5aa5
#define T_7  0x42bd
#define T_8  0x4ab5
#define T_9  0x52ad



void setMotor(int id, int speed)
{
  char tmp[40];
  int en, in1, in2;

  switch(id)
  {
    case IDL :
      en = ENL;
      in1 = IN1L;
      in2 = IN2L;
      break;
    case IDR :
      en = ENR;
      in1 = IN1R;
      in2 = IN2R;
      break;
  }

  if (abs(speed) > 10000)
  {
    // active brake
    Serial.println("use brake");
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
  }
  else if (speed > 0)
  {
    // move forward
    Serial.println("forward");
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
  else if (speed < 0)
  {
    // move backward
    Serial.println("backward");
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
  else
  {
    // turn motor off
    Serial.println("turn motor off");
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }

  sprintf(tmp, "set speed to %d", abs(speed));
  Serial.println(tmp);
  analogWrite(en, abs(speed));
}

void LED_P8x16Str(int x, int y, char *s)
{
  Serial.println(s);
}

void dispKey(int code)
{
  switch (code)
  {
    case T_CHM:   LED_P8x16Str(1,5,"CH- "); break;
    case T_CH:    LED_P8x16Str(1,5,"CH  "); break;
    case T_CHP:   LED_P8x16Str(1,5,"CH+ "); break;
    case T_PREV:  LED_P8x16Str(1,5,"PREV"); break;
    case T_NEXT:  LED_P8x16Str(1,5,"NEXT"); break;
    case T_PLAY:  LED_P8x16Str(1,5,"PLAY"); break;
    case T_VOLM:  LED_P8x16Str(1,5,"VOL-"); break;
    case T_VOLP:  LED_P8x16Str(1,5,"VOL+"); break;
    case T_EQ:    LED_P8x16Str(1,5,"EQ  "); break;
    case T_0:     LED_P8x16Str(1,5,"0   "); break;
    case T_100:   LED_P8x16Str(1,5,"100+"); break;
    case T_200:   LED_P8x16Str(1,5,"200+"); break;
    case T_1:     LED_P8x16Str(1,5,"1   "); break;
    case T_2:     LED_P8x16Str(1,5,"2   "); break;
    case T_3:     LED_P8x16Str(1,5,"3   "); break;
    case T_4:     LED_P8x16Str(1,5,"4   "); break;
    case T_5:     LED_P8x16Str(1,5,"5   "); break;
    case T_6:     LED_P8x16Str(1,5,"6   "); break;
    case T_7:     LED_P8x16Str(1,5,"7   "); break;
    case T_8:     LED_P8x16Str(1,5,"8   "); break;
    case T_9:     LED_P8x16Str(1,5,"9   "); break;
    default :     LED_P8x16Str(1,5,"????"); break;
  }
}

IRrecv irrecv(IRPIN);
decode_results results;


//int LED=2;
int LED=16;

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  delay(200);
  digitalWrite(LED, LOW);
  delay(200);
  digitalWrite(LED, HIGH);
  delay(200);
  digitalWrite(LED, LOW);
  delay(200);
  digitalWrite(LED, HIGH);
  delay(200);
  digitalWrite(LED, LOW);


//  Serial.begin(115200);
  pinMode(1, FUNCTION_3);
  pinMode(3, FUNCTION_3);

  // set all the motor control pins to outputs
  pinMode(ENL, OUTPUT);
  pinMode(IN1L, OUTPUT);
  pinMode(IN2L, OUTPUT);
  pinMode(ENR, OUTPUT);
  pinMode(IN1R, OUTPUT);
  pinMode(IN2R, OUTPUT);

  irrecv.enableIRIn();  // initialize IR receptor
  
  // sensor init
  pinMode(TRIG, OUTPUT); // Sets the trigPin as an Output
  pinMode(ECHO, INPUT); // Sets the echoPin as an Input}

//  pinMode(LED, OUTPUT); // Sets the trigPin as an Output

  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();
}

int First=1;
int MotorSel=1;
int Speed=0;
int Distance;
#define DTSZ  5
int DT[DTSZ];
int DTidx;
long Duration;
int ModeAuto=0;

void loop() {
  char  buf[40];
  int   key, i;

  
  if (First)
  {
//    Serial.println("IR remote codes");
    First = 0;
    for(i=0; i<DTSZ; i++)
      DT[i] = -1;
    DTidx=0;
  }

  if (irrecv.decode(&results))
  {
    sprintf(buf, "code = 0x%08x", (long) results.value);
//    Serial.println(buf);
    key = results.value & 0xffff;
    dispKey(key);

    switch (key)
    {
      case T_1:     MotorSel=IDL; Speed=0; break;
      case T_2:     MotorSel=IDR; Speed=0; break;
      case T_VOLM:  Speed-=MAXSPEED/10; break;
      case T_VOLP:  Speed+=MAXSPEED/10; break;
      case T_PREV:  Speed=-MAXSPEED; break;
      case T_NEXT:  Speed=MAXSPEED; break;
      case T_0:     setMotor(IDL, 0); setMotor(IDR, 0); Speed=0; break;
      case T_PLAY:  ModeAuto=!ModeAuto; break;
    }
    irrecv.resume();  // get next value
  }

    if (ModeAuto)
    {
      // Clears the trigPin
      digitalWrite(TRIG, LOW);
      delayMicroseconds(2);

      // Sets the trigPin on HIGH state for 10 micro seconds
      digitalWrite(TRIG, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG, LOW);
  
      // Reads the echoPin, returns the sound wave travel time in microseconds
      Duration = pulseIn(ECHO, HIGH);
  
      // Calculating the distance in cm
      Distance= Duration*0.034/2;
      sprintf(buf, "distance=%d duration=%d", Distance, Duration);
//      Serial.println(buf);

      DT[DTidx++] = Distance;
      if (DTidx >= DTSZ)
        DTidx = 0;

      Distance = 0;
      for (i=0; i<DTSZ; i++)
      {
        // Start
        if (DT[i] < 0)
        {
      sprintf(buf, "DT[%d}<0", i);
//      Serial.println(buf);
          return;
        }
        Distance += DT[i];
      }

      Distance = Distance / DTSZ;

      if (Distance > 100)
      {
        setMotor(IDL, Speed);
        setMotor(IDR, Speed);
      }
      else if (Distance > 20)
      {
        setMotor(IDL, Distance*Speed/100);
        setMotor(IDR, Distance*Speed/100);
      }
      else
      {
        setMotor(IDL, -0.5*Speed);
        setMotor(IDR, 0.5*Speed);
        delay(200);
      }
    }
    else
    {
//      Serial.print("Motor selected: ");
      switch (MotorSel)
      {
//        case IDL: Serial.println("left"); break;
//        case IDR: Serial.println("right"); break;
      }
      sprintf(buf, "Speed: %d", Speed);
//      Serial.println(buf);
      setMotor(MotorSel, Speed);
    }
  
  return;
}
