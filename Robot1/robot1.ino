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
}

#define	ST_MANUAL		0		// manual mode
#define	ST_START		1		// auto, start phase
#define	ST_FORWARD	2		// auto, forward phase
#define	ST_TURN			3		// auto, turn phase

#define EVT_NONE				0		// no event
#define EVT_MODESWITCH	1		// switch mode manual/auto
#define EVT_SPEEDINC		2		// increase speed
#define EVT_SPEEDDEC		3		// decrease speed
#define EVT_SPEED0			4		// set speed to 0
#define EVT_SELMOTORL		5		// select left motor
#define EVT_SELMOTORR		6		// select right motor

int First=1;
int	State = ST_MANUAL;
#define DTSZ  5
int DT[DTSZ];
int DTidx;
int ModeAuto = 0;
int	LastScan = -1;

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
			if (motorSel == IDL)
			{
				speedL += MAXSPEED/10;
				speed = speedL;
			}
			else
			{
				speedR += MAXSPEED/10;
				speed = speedR;
			}
			setMotor(motorSel, speed);
			break;

		case	EVT_SPEEDDEC:
			if (motorSel == IDL)
			{
				speedL -= MAXSPEED/10;
				speed = speedL;
			}
			else
			{
				speedR -= MAXSPEED/10;
				speed = speedR;
			}
			setMotor(motorSel, speed);
			break;
	}
}

int start(int evt)
{
}

int forward(int evt)
{
	static	int	speed=0;
	int	ave=0, i;
	char	buf[40];

	if (LastScan < 0)
		return -1;

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
			return 0;
		}
		ave += DT[i];
	}

	ave = ave / DTSZ;

	if (ave > 100)
	{
		setMotor(IDL, speed);
		setMotor(IDR, speed);
	}
	else if (ave > 20)
	{
		setMotor(IDL, ave*speed/100);
		setMotor(IDR, ave*speed/100);
	}
	else
	{
		setMotor(IDL, -0.5*speed);
		setMotor(IDR, 0.5*speed);
		delay(200);
	}
}

int turn(int evt)
{
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
			State = ST_MANUAL;
			manualMode(EVT_SPEED0);
			return;
		}
		else
		{
			State = ST_START;
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
			start(evt);
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

/*!
 * \brief perform a scan
 *
 * \return distance in mm
 */
int sonarScan()
{
	static int	count=0;
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

	if (count%4 == 0)
	{
		display.clearDisplay();
		display.setCursor(0,0);
	}
	sprintf(buf, "distance=%d", distance);
	display.println(buf);
	display.display();
	count++;

	return distance;
}

/*!
 * \brief main loop
 */
void loop()
{
	char  buf[40];
	char  tmp[128];
	int   key, i, evt;

	if (First)
	{
		//    Serial.println("IR remote codes");
		First = 0;
		for(i=0; i<DTSZ; i++)
			DT[i] = -1;
		DTidx=0;
	}

	LastScan = sonarScan();

	evt = readIR();
	treatEvt(evt);

	return;
}
