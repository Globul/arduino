// vim: ft=sh: set noet ts=2 sw=2:

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

#define IDA  2

// Max value for motor
#define MAXSPEED  1024

// Infrared receptor
int IRPIN=14;

// Sonar sensor
//int TRIG=5;
//int ECHO=4;
int TRIG=12;//1
int ECHO=13;//3

// CPU frequency
int	CpuFreqMHz;

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

IRrecv IrRecv(IRPIN);


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

	IrRecv.enableIRIn();  // initialize IR receptor

	// sensor init
	pinMode(TRIG, OUTPUT); // Sets the trigPin as an Output
	pinMode(ECHO, INPUT); // Sets the echoPin as an Input}

	//  pinMode(LED, OUTPUT); // Sets the trigPin as an Output


	// by default, we will generate the high voltage from the 3.3v line internally! (neat!)
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
	// init done

	// Show image buffer on the display hardware.
	// Since the buffer is intialized with an Adafruit splashscreen
	// internally, this will display the splashscreen.
	// display.display();
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,0);
	display.println("Starting ...");
	display.display();
	delay(2000);

	// Clear the buffer.
	display.clearDisplay();

	CpuFreqMHz = ESP.getCpuFreqMHz();
}

#define	ST_MANUAL		0		// manual mode
#define	ST_START		1		// auto, start phase
#define	ST_FORWARD	2		// auto, forward phase
#define	ST_TURN			3		// auto, turn phase
#define ST_SCAN     4   // scan

#define EVT_NONE				0		// no event
#define EVT_MODESWITCH	1		// switch mode manual/auto
#define EVT_SPEEDINC		2		// increase speed
#define EVT_SPEEDDEC		3		// decrease speed
#define EVT_SPEED0			4		// set speed to 0
#define EVT_SELMOTORL		5		// select left motor
#define EVT_SELMOTORR		6		// select right motor
#define EVT_SELMOTORB   7   // select both motors

int First=1;
int  State = ST_MANUAL;
int  PrevState = ST_MANUAL;
#define DTSZ  5
int DT[DTSZ];
int DTidx;
int ModeAuto = 0;
int	LastScan = -1;
int Count = 0;

void changeState(int st)
{
  PrevState = State;
  State = st;
}

void dispMsg(char *str)
{
  static int count = 0;
  
  if (count % 4 == 0)
  {
    display.clearDisplay();
    display.setCursor(0,0);
  }
  display.println(str);
  display.display();
  count++;
}

int	manualMode(int evt)
{
	static int	speedL=0, speedR=0;
	static int	motorSel=IDL;
	int					speed;

	switch(evt)
	{
		case	EVT_SPEED0:
			speedL=0;
			speedR=0;
			setMotor(IDL, 0);
			setMotor(IDR, 0);
			break;

		case	EVT_SPEEDINC:
      dispMsg("inc speed");
			if (motorSel == IDL || motorSel == IDA)
			{
				speedL += MAXSPEED/10;
				speed = speedL;
			}
      if (motorSel == IDR || motorSel == IDA)
			{
				speedR += MAXSPEED/10;
				speed = speedR;
			}
      if (motorSel == IDA)
      {
        setMotor(IDL, speed);
        setMotor(IDR, speed);
      }
      else
        setMotor(motorSel, speed);
			break;

		case	EVT_SPEEDDEC:
      dispMsg("dec speed");
      if (motorSel == IDL || motorSel == IDA)
			{
				speedL -= MAXSPEED/10;
				speed = speedL;
			}
      if (motorSel == IDR || motorSel == IDA)
			{
				speedR -= MAXSPEED/10;
				speed = speedR;
			}
			setMotor(motorSel, speed);
			break;
    case  EVT_SELMOTORB:
      dispMsg("select motor both");
      motorSel = IDA;
      break;
    case  EVT_SELMOTORR:    	
      dispMsg("select motor right");
      motorSel = IDR;
      break;
    case  EVT_SELMOTORL:
      dispMsg("select motor left");
      motorSel = IDL;
      break;
  }
}

int start(int evt)
{
  int i;
  
  dispMsg("State = ST_START");
  for(i=0; i<DTSZ; i++)
    DT[i] = -1;
  DTidx=0;
  changeState(ST_FORWARD);
  return 0;
}

int forward(int evt)
{
	static	int	speed=0.8*MAXSPEED;
  static  int lightTurn=0;  // light turn when approaching obstacle
	int	ave=0, i;
	char	buf[40];

//  dispMsg("State = ST_FORWARD");

	if (LastScan < 0)
		return -1;

  if (LastScan == 0)
  {
    speed = -20000;
    setMotor(IDL, speed);
    setMotor(IDR, speed);
    dispMsg("FREINAGE!");
    delay(100);
    dispMsg("Arriere");
    speed = -0.4*MAXSPEED;
    setMotor(IDL, speed);
    setMotor(IDR, speed);
    delay(500);
    dispMsg("Stop");
    speed = 0;
    setMotor(IDL, speed);
    setMotor(IDR, speed);
    changeState(ST_TURN);
    return 1;
  }
  
	DT[DTidx++] = LastScan;
	if (DTidx >= DTSZ)
		DTidx = 0;

	for (i=0; i<DTSZ; i++)
	{
		// Start
		if (DT[i] < 0)
		{
			sprintf(buf, "DT[%d}<0", i);
			//      Serial.println(buf);
      setMotor(IDL, 0);
      setMotor(IDR, 0);
			return 0;
		}
		ave += DT[i];
	}

	ave = ave / DTSZ;

	if (ave > 70)
	{
    if (speed < MAXSPEED)
      speed += MAXSPEED*0.1;
    else
      speed = MAXSPEED;
		setMotor(IDL, speed);
		setMotor(IDR, speed);
    // just select lightTurn way for next time
    if (lightTurn == 0)
      lightTurn = random(0, 2)*2 - 1;
	}
	else if (ave > 20)
	{
    if (speed > ave*MAXSPEED/100)
      speed = ave*MAXSPEED/100;
    if (speed < MAXSPEED*0.3)
      speed = MAXSPEED*0.3;
    // obstacle detected, lightTurn to try to avoid
		setMotor(IDL, speed+MAXSPEED*0.1*lightTurn);
		setMotor(IDR, speed-MAXSPEED*0.1*lightTurn);
	}
	else
  {
    lightTurn = 0;
    changeState(ST_TURN);
  }
}

int turn(int evt)
{
  static int fact=0;

  dispMsg("State = ST_TURN");

  // don't change the way if scanning process running
  if ((fact == 0 || PrevState != ST_SCAN) && random(0,2))
    fact = -1;
  else
    fact = 1;

  setMotor(IDL, -fact*0.4*MAXSPEED);
  setMotor(IDR, fact*0.4*MAXSPEED);
  delay(300);
  changeState(ST_SCAN);
	return 0;
}

int scan(int evt)
{
  dispMsg("State = ST_SCAN");
  if (LastScan < 20)
    changeState(ST_TURN);
  else
    changeState(ST_FORWARD);
  return 0;
}

/*!
 * \brief Treat events
 *
 * \param evt event received
 */
void  treatEvt(int evt)
{
	if (evt == EVT_NONE && State == ST_MANUAL)
		return;

	if (evt == EVT_MODESWITCH)
	{
		if (State != ST_MANUAL)
		{
			changeState(ST_MANUAL);
			manualMode(EVT_SPEED0);
			return;
		}
		else
		{
			changeState(ST_START);
		}
	}

	switch (State)
	{
		case ST_MANUAL:
			manualMode(evt);
			return;
		case ST_START:
			start(evt);
			return;
		case ST_FORWARD:
			forward(evt);
			return;
		case ST_TURN:
			turn(evt);
			return;
    case ST_SCAN:
      scan(evt);
      return;
		default:
			break;
	}
}

/*!
 * \brief Read IR sensor
 *
 * \return event related to remote key entered
 */
int readIR()
{
	decode_results	results;
	char						buf[40];
	int							ret=EVT_NONE, key;

	if (IrRecv.decode(&results))
	{
		sprintf(buf, "code = 0x%08x", (long) results.value);
		//    Serial.println(buf);
		key = results.value & 0xffff;
		dispKey(key);

		switch (key)
		{
			case T_1:     ret = EVT_SELMOTORL; break;
			case T_2:     ret = EVT_SELMOTORR; break;
      case T_3:     ret = EVT_SELMOTORB; break;
			case T_VOLM:  ret = EVT_SPEEDDEC; break;
			case T_VOLP:  ret = EVT_SPEEDINC; break;
//			case T_PREV:  Speed=-MAXSPEED; break;
//			case T_NEXT:  Speed=MAXSPEED; break;
			case T_0:     ret = EVT_SPEED0; break;
			case T_PLAY:  ret = EVT_MODESWITCH; break;
		}
		IrRecv.resume();  // get next value
	}
	return ret;
}

#include "wiring_private.h"
#include "pins_arduino.h"

/* Measures the length (in microseconds) of a pulse on the pin; state is HIGH
* or LOW, the type of pulse to measure.  Works on pulses from 2-3 microseconds
* to 3 minutes in length, but must be called at least a few dozen microseconds
* before the start of the pulse. */
unsigned long myPulseIn(uint8_t pin, uint8_t state, unsigned long timeout)
{
	// cache the port and bit of the pin in order to speed up the
	// pulse width measuring loop and achieve finer resolution.  calling
	// digitalRead() instead yields much coarser resolution.
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	uint8_t stateMask = (state ? bit : 0);
	unsigned long width = 0; // keep initialization out of time critical area

	// convert the timeout from microseconds to a number of times through
	// the initial loop; it takes 16 clock cycles per iteration.
	unsigned long numloops = 0;
	unsigned long maxloops = microsecondsToClockCycles(timeout) / 16;

	// wait for any previous pulse to end
	while ((*portInputRegister(port) & bit) == stateMask)
		if (numloops++ == maxloops)
			return 0;

	// wait for the pulse to start
	while ((*portInputRegister(port) & bit) != stateMask)
		if (numloops++ == maxloops)
			return 0;

	// wait for the pulse to stop
	while ((*portInputRegister(port) & bit) == stateMask) {
		if (numloops++ == maxloops)
			return 0;
		width++;
	}

	// convert the reading to microseconds. The loop has been determined
	// to be 20 clock cycles long and have about 16 clocks between the edge
	// and the start of the loop. There will be some error introduced by
	// the interrupt handlers.
	return clockCyclesToMicroseconds(width * 21 + 16);
}

/*!
 * \brief perform a scan
 *
 * \return distance in mm
 */
int sonarScan()
{
	int	duration, distance;
	char	buf[40];

	// Clears the trigPin
	digitalWrite(TRIG, LOW);
	delayMicroseconds(2);

	// Sets the trigPin on HIGH state for 10 micro seconds
	digitalWrite(TRIG, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIG, LOW);

	// Reads the echoPin, returns the sound wave travel time in microseconds
	duration = pulseIn(ECHO, HIGH, 300000);

	// Calculating the distance in cm
	distance= duration*0.034/2;

	return distance;
}

// return duration (in microseconds) for a number of cycles
uint32_t cycle2Microsec(uint32_t c)
{
	return c/CpuFreqMHz;
}

uint32_t microsec2Cycle(uint32_t ms)
{
	return ms*CpuFreqMHz;
}

/*!
 * \brief perform a scan
 *
 * \return distance in mm
 */
#define	SONAR_START					0
#define	SONAR_CLEAR					1
#define	SONAR_SEND					2
#define	SONAR_READWAITLOW1	3
#define	SONAR_READWAITHIGH	4
#define	SONAR_ERROR					5
#define	SONAR_FIRST					6
#define	SONAR_READWAITLOW2	7
int sonarScanNew()
{
	static int state = SONAR_FIRST;
	static uint32_t last, start;
	static uint8_t	bit, port, stateMask;
	static int disp = 1;
	uint32_t cur, diff;
	int	duration, distance;
	char	buf[40];

	cur = ESP.getCycleCount();
	if (cur - last > microsec2Cycle(10000000))
	{
			sprintf(buf, "lock st=%d => reset", state);
			dispMsg(buf);
			state = SONAR_START;
	}
	switch(state)
	{
		case SONAR_FIRST:
			bit = digitalPinToBitMask(ECHO);
			port = digitalPinToPort(ECHO);
			stateMask = (HIGH ? bit : 0);

		case SONAR_START:
			if (disp)
			{
				dispMsg("st=START");
				disp = 0;
			}
			state = SONAR_CLEAR;
			disp = 1;
			digitalWrite(TRIG, LOW);
			last = ESP.getCycleCount();
			start = last;
			return -1;

		case SONAR_CLEAR:
			if (disp)
			{
				dispMsg("st=CLEAR");
				disp = 0;
			}
			if (cur - last < microsec2Cycle(2))
				return -2;
			state = SONAR_SEND;
			disp = 1;
			digitalWrite(TRIG, HIGH);
			last = ESP.getCycleCount();
			return -3;

		case SONAR_SEND:
			if (disp)
			{
//				dispMsg("st=SEND");
				disp = 0;
			}
			if (cur - last < microsec2Cycle(10))
				return -4;
			digitalWrite(TRIG, LOW);
			state = SONAR_READWAITLOW2;
			disp = 1;
//			state = SONAR_READWAITHIGH;
			return -5;

		case SONAR_READWAITLOW1:
			if (disp)
			{
//				dispMsg("st=READWAITLOW1");
				disp = 0;
			}
//			if ((*portInputRegister(port) & bit) == stateMask)
			if (digitalRead(ECHO) == HIGH)
				return -6;
			state = SONAR_READWAITHIGH;
			disp = 1;
			return -7;

		case SONAR_READWAITHIGH:
			if (disp)
			{
//				dispMsg("st=READWAITHIGH");
				disp = 0;
			}
//			if ((*portInputRegister(port) & bit) != stateMask)
			if (digitalRead(ECHO) == LOW)
				return -8;
			last = ESP.getCycleCount();
			state = SONAR_READWAITLOW2;
			disp = 1;
			return -9;

		case SONAR_READWAITLOW2:
			if (0 && disp)
			{
//				dispMsg("st=READWAITLOW2");
				disp = 0;
			}
//			if ((*portInputRegister(port) & bit) == stateMask)
//			if (digitalRead(ECHO) == HIGH)
//				return -10;
			state = SONAR_START;
			disp = 1;
//			duration = cycle2Microsec(cur - last);

			duration = pulseIn(ECHO, HIGH, 300000);
			distance= duration*0.034/2;
			return distance;

		default:
			dispMsg("st=ERROR");
			dispMsg("Error sonar !");
			disp = 1;
			return -11;
	}
}

/*!
 * \brief main loop
 */
void loop()
{
	static int count = 0;
	char  buf[40];
	char  tmp[128];
	int   key, i, evt;

#if 0
	unsigned long int	count1, count2;

	count1 = ESP.getCycleCount();
	sprintf(buf, "test");
	dispMsg(buf);
	count2 = ESP.getCycleCount();
	sprintf(buf, "cc1 = %u\ncc2 = %u", count1, count2);
	dispMsg(buf);
	sprintf(buf, "diff = %u", count2-count1);
	dispMsg(buf);
	delay(5000);
#endif

/*
	evt = readIR();
  if (evt != EVT_NONE)
  {
    sprintf(buf, "evt = %d", evt);
    dispMsg(buf);
  }
*/

//	if (!((count / 300) % 2))
	if (0)
	{
		LastScan = sonarScan();
//		if (State != ST_MANUAL)
		{
			sprintf(buf, "old d=%d", LastScan);
			dispMsg(buf);
		}
	}
	else
	{
//		while (1)
		{
		LastScan = sonarScanNew();
//		if (State != ST_MANUAL)
		if (LastScan >= 0)
		{
			sprintf(buf, "new d=%d", LastScan);
			dispMsg(buf);
			delay(1000);
		}
		}
	}

//	treatEvt(evt);

	count++;
	return;
}


/*	Informations

	count1 = ESP.getCycleCount();
	count2 = ESP.getCycleCount();
	diff entre count1 et count2 = 2 
	avec un delay(1000) entre les 2 = de 80001700 a 80003400

	avec
	sprintf(buf, "test");
	dispMsg(buf);
	=> 4500000

	*/
