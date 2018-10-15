/*
	Arrosage

	Gestion de l'arrosage
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCDWIDTH  20
#define LCDHEIGHT 4

#define EVT_NULL	0
#define EVT_INIT	1
#define EVT_DISP	2
#define EVT_BTNPREV	3
#define EVT_BTNNEXT	4
#define EVT_BTNOK	5
#define EVT_BTNCANCEL	6

typedef int (*callback_t)(int);

// Circular queue for waiting events
#define	MAXEVT		10
char	EvtQ[MAXEVT];
char	EvtIdx = 0;	
char	EvtCount = 0;	

// Stack to store callbacks
#define	MAXSTACK	10
callback_t	Stack[MAXSTACK];
char		StackIdx = 0;	



const int BtnPrev = 2;	// button on pin 2
const int BtnNext = 3;	// button on pin 3
const int BtnOk = 4;	// button on pin 4
const int BtnCancel = 5;  // button on pin 5

unsigned int	Tick = 0;
char		Vanne = 1;	// current selected vanne
char		Digit;		// current selected digit
int		Hour;		// current selected hour

LiquidCrystal_I2C lcd(0x27, LCDWIDTH, LCDHEIGHT);


void printlcd(int c, int l, char *str)
{
  char disp[LCDWIDTH+1];

  memset(disp, ' ', LCDWIDTH);
  strncpy(disp, str, strlen(str));
  disp[sizeof(disp)-1] = '\0';
  lcd.setCursor(c, l);
//  lcd.print(disp);
  lcd.print(str);
}

// the setup function runs once when you press reset or power the board
void oldsetup() {
  Wire.begin();
  lcd.begin(LCDWIDTH, LCDHEIGHT);
  lcd.init();
  lcd.backlight();
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  // button
  pinMode(BtnPrev,INPUT_PULLUP);
  pinMode(BtnNext,INPUT_PULLUP);
  pinMode(BtnOk,INPUT_PULLUP);
  pinMode(BtnCancel,INPUT_PULLUP);
  // use mode HIGH for the button to limit the bounds
//  digitalWrite(BtnPrev, HIGH);
//  digitalWrite(BtnNext, HIGH);
//  digitalWrite(BtnOk, HIGH);
//  digitalWrite(BtnCancel, HIGH);
  
}

// the loop function runs over and over again forever
void oldloop() {
  static int freq=500;
  static int prevst=HIGH;
  int st;

  printlcd(1, 1, "Start");
  if (digitalRead(BtnPrev) == LOW)
  {
    printlcd(1, 1, "BtnPrev");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(freq);                       // wait for a second
  }
  else if (digitalRead(BtnNext) == LOW)
  {
    printlcd(1, 1, "BtnNext");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(freq);                       // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(freq);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(freq);                       // wait for a second
  }
  else if (digitalRead(BtnOk) == LOW)
  {
    printlcd(1, 1, "BtnOk");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(freq);                       // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(freq);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(freq);                       // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(freq);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(freq);                       // wait for a second
  }
  else if (digitalRead(BtnCancel) == LOW)
  {
    printlcd(1, 1, "BtnCancel");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(freq);                       // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(freq);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(freq);                       // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(freq);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(freq);                       // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(freq);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(freq);                       // wait for a second
  }
  
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
}

/*!
 * \brief scan IO for a new event
 *
 * \return 1 for one new event, 0 for no new event, -1 for an error (=EvtQ full)
 */
int addEvt(int evt)
{
	char	tmp[40];

	if (EvtCount >= MAXEVT)
		return -1;

	EvtQ[EvtIdx] = evt;
	EvtIdx = (EvtIdx + 1) % MAXEVT;
	EvtCount++;
	sprintf(tmp, "addEvt %d sz=%d", evt, EvtCount);
	printlcd(0,3,tmp);
	return 1;
}

/*!
 * \brief scan IO for a new event
 *
 * \return 1 for one new event, 0 for no new event, -1 for an error (=EvtQ full)
 */
int getEvt()
{
	static int	lastEvt=EVT_NULL;
	char	evt=EVT_NULL;

	if (digitalRead(BtnPrev) == LOW)
		evt = EVT_BTNPREV;
	else if (digitalRead(BtnNext) == LOW)
		evt = EVT_BTNNEXT;
	else if (digitalRead(BtnOk) == LOW)
		evt = EVT_BTNOK;
	else if (digitalRead(BtnCancel) == LOW)
		evt = EVT_BTNCANCEL;

	if (evt == EVT_NULL)
		lastEvt = EVT_NULL;
	else if (evt != lastEvt)	// check if event is different from previous
	{
		addEvt(evt);
//		delay(5000);
		lastEvt = evt;
		delay(400);	// to avoid bounds
		return 1;
	}
	return 0;
}

/*!
 * \brief treat next evt if needed
 *
 * \return 0 if no evt treated, 1 if one treated
 */
int	treatEvt()
{
	callback_t	cb;
	char	evt=EVT_NULL;
	char	tmp[40];
	int	idx;
	int	ret;

	if (EvtCount <= 0)
		return 0;

	idx = (EvtIdx + MAXEVT - 1) % MAXEVT;
	evt = EvtQ[idx];

	if (StackIdx > 0)
	{
//		sprintf(tmp, "%u: call %d evt%d", Tick, StackIdx-1, evt);
//		printlcd(0,3,tmp);
//		delay(5000);
		cb = Stack[StackIdx-1];
		EvtIdx = idx;
		EvtCount--;
		// cb return 1 if event treated
		ret = cb(evt);
//		sprintf(tmp, "%u: call cb evt%d ret%d", Tick, evt, ret);
//		printlcd(0,3,tmp);
//		delay(5000);
	}
	return 0;
}

/*!
 * \brief Push a callback on the stack
 *
 * \param func callback to be pushed
 * \return -1 on error, current stack size on success
 */
int	pushStack(callback_t func)
{
	if (StackIdx >= MAXSTACK)
		return -1;

	Stack[StackIdx++] = func;
	return StackIdx;
}

/*!
 * \brief Pop callback on the top of the stack
 *
 * \return callback or NULL if stack empty
 */
callback_t popStack()
{
	if (StackIdx <= 0)
		return NULL;

	return Stack[--StackIdx];
}

void previousMenu()
{
	popStack();
	addEvt(EVT_DISP);
}

void clearScreen()
{
	printlcd(0,0,"                    ");
	printlcd(0,1,"                    ");
	printlcd(0,2,"                    ");
	printlcd(0,3,"                    ");
}

/*!
 * \brief Display menu
 *
 * \param evt event to treat, EVT_DISP for initial display
 * \param lst menu entries
 * \param count number of entries
 * \return -1 on Cancel, 0 for Prev or Next, cur for Ok (0<cur<=count)
 */
int menu(int evt, const char *lst[], int count)
{
	static int cur=0;
	int	i, c;

	if (evt == EVT_DISP)
	{
		clearScreen();
		for (i=0; i<count; i++)
			printlcd(3, i, lst[i]);
		cur = 0;
		printlcd(0, 0, "=>");
//		printlcd(0, 3, "menu evt_null");
	}
	else 
	{
		printlcd(0, cur, "  ");
		switch (evt)
		{
			case EVT_BTNPREV:
				cur = ((cur+count-1)%count);
				printlcd(0, cur, "=>");
				break;
			case EVT_BTNNEXT:
				cur = (((cur+1)%count));
				printlcd(0, cur, "=>");
				break;
			case EVT_BTNOK:
				return cur+1;
				break;
			case EVT_BTNCANCEL:
				return -1;
				break;
		}
	}

	return 0;
}

int menuPompe(int evt)
{
	const char	*lst[] = {"Demarrer", "Arreter"};
	int	ret;

	// init
	if (evt == EVT_INIT)
	{
		// add this function to treat event
		pushStack(menuPompe);
		addEvt(EVT_DISP);
		return 1;
	}

	ret = menu(evt, lst, 2);
	switch (ret)
	{
		case 1:	clearScreen();
			printlcd(0,1, "Pompe demarree");
			delay(3000);
			previousMenu();
			break;
		case 2:	clearScreen();
			printlcd(0,1, "Pompe arretee");
			delay(3000);
			previousMenu();
			break;
		case -1:
			previousMenu();
			break;
	}
	return 1;	// evt used
}

int selectDigitCB(int evt)
{
	return selectDigit(evt, -1, -1, -1, -1, -1);
}

/*!
 * \brief select a digit
 *
 * \param x x coordinate
 * \param y y coordinate
 * \param min minimum value allowed
 * \param max maximum value allowed
 * \param def default value
 * \return digit selected, or -1 if Cancel selected
 */
int selectDigit(int evt, int x, int y, int min, int max, int def)
{
	static char	sx, sy, smin, smax, sdef;
	char	tmp[40];

	switch (evt)
	{
		case EVT_INIT:
			// add the callback function to treat event
			pushStack(selectDigitCB);
//			clearScreen();
			Digit = def;
			sx = x;
			sy = y;
			smin = min;
			smax = max;
			sdef = def;
			lcd.setCursor(x, y);
			lcd.blink();
			break;
		
		case EVT_BTNPREV:
			if (Digit <= smin)
				Digit = smax;
			else
				Digit--;
			break;
		case EVT_BTNNEXT:
			if (Digit >= smax)
				Digit = smin;
			else
				Digit++;
			break;
		case EVT_BTNOK:
			lcd.noBlink();
			previousMenu();	// go to previous menu
			addEvt(EVT_BTNOK);
			return 1;
		case EVT_BTNCANCEL:
			lcd.noBlink();
			Digit = def;
			previousMenu();	// go to previous menu
			addEvt(EVT_BTNCANCEL);
			return 1;
	}
	sprintf(tmp, "%d", Digit);
	printlcd(sx, sy, tmp);
	lcd.setCursor(sx, sy);
	return 1;
}

/*!
 * \brief select a time
 *
 * \param evt event
 * \param x x coordinate
 * \param y y coordinate
 * \return 1
 */
int selectTime(int evt, int x, int y)
{
	static char	curdigit, d[4];
	static int	sx, sy;
	char	dec = 0;

	switch (evt)
	{
		case EVT_INIT:
			// add this function to treat event
			pushStack(selectTime);
			printlcd(x, y, "12:00");
			curdigit = 0;
			sx = x;
			sy = y;
			memset(d, 0, sizeof(d));
			selectDigit(EVT_INIT, x, y, 0, 2, 1);
			return 1;
			break;
		case EVT_BTNOK:
			d[curdigit++] = Digit; 
			if (curdigit > 1)
				dec = 1;
			if (curdigit > 3)
			{
				Hour = d[0]*1000+d[1]*100+d[2]*10+d[3];
				previousMenu();	
				addEvt(EVT_BTNOK);
			}
			else
			{
				switch (curdigit)
				{
					case 0:
						selectDigit(EVT_INIT, sx+curdigit+dec, sy, 0, 2, 1);
						break;
					case 1:
						selectDigit(EVT_INIT, sx+curdigit+dec, sy, 0, 9, 2);
						break;
					case 2:
						selectDigit(EVT_INIT, sx+curdigit+dec, sy, 0, 5, 0);
						break;
					default:
						selectDigit(EVT_INIT, sx+curdigit+dec, sy, 0, 9, 0);
						break;
				}
			}
			return 1;
		case EVT_BTNCANCEL:
			curdigit--;
			if (curdigit > 1)
				dec = 1;
			if (curdigit < 0)
			{
				previousMenu();	
				addEvt(EVT_BTNCANCEL);
			}
			else
			{
				switch (curdigit)
				{
					case 0:
						selectDigit(EVT_INIT, sx+curdigit+dec, sy, 0, 2, 1);
						break;
					case 1:
						selectDigit(EVT_INIT, sx+curdigit+dec, sy, 0, 9, 2);
						break;
					case 2:
						selectDigit(EVT_INIT, sx+curdigit+dec, sy, 0, 5, 0);
						break;
					default:
						selectDigit(EVT_INIT, sx+curdigit+dec, sy, 0, 9, 0);
						break;
				}
			}
			return 1;
	}
}

/*!
 * \brief select a date
 *
 * \param evt event
 * \param x x coordinate
 * \param y y coordinate
 * \return 1
 */
int selectDate(int evt, int x, int y)
{
	static char	curdigit, d[6];
	static int	sx, sy;
	char	dec = 0;

	switch (evt)
	{
		case EVT_INIT:
			// add this function to treat event
			pushStack(selectDate);
			printlcd(x, y, "01/01/2018");
			curdigit = 0;
			sx = x;
			sy = y;
			memset(d, 0, sizeof(d));
			selectDigit(EVT_INIT, x, y, 0, 3, 0);
			return 1;
		case EVT_BTNOK:
			d[curdigit++] = Digit; 
			if (curdigit > 3)
				dec = 4;
			else if (curdigit > 1)
				dec = 1;
			if (curdigit > 5)
			{
//				Hour = d[0]*1000+d[1]*100+d[2]*10+d[3];
				previousMenu();	
				addEvt(EVT_BTNOK);
			}
			else
				break;
			return 1;
		case EVT_BTNCANCEL:
			curdigit--;
			if (curdigit > 3)
				dec = 4;
			else if (curdigit > 1)
				dec = 1;
			if (curdigit < 0)
			{
				previousMenu();	
				addEvt(EVT_BTNCANCEL);
			}
			else
				break;
			return 1;
	}
	switch (curdigit)
	{
		case 0:
			selectDigit(EVT_INIT, sx+curdigit+dec, sy, 0, 3, 0);
			break;
		case 1:
			selectDigit(EVT_INIT, sx+curdigit+dec, sy, 0, 9, 1);
			break;
		case 2:
			selectDigit(EVT_INIT, sx+curdigit+dec, sy, 0, 1, 0);
			break;
		case 3:
			selectDigit(EVT_INIT, sx+curdigit+dec, sy, 0, 9, 1);
			break;
		case 4:
			selectDigit(EVT_INIT, sx+curdigit+dec, sy, 0, 9, 1);
			break;
		case 5:
			selectDigit(EVT_INIT, sx+curdigit+dec, sy, 0, 9, 8);
			break;
	}
}

/*!
 * \brief select a digit
 *
 * \param x x coordinate
 * \param y y coordinate
 * \param min minimum value allowed
 * \param max maximum value allowed
 * \param def default value
 * \return digit selected, or -1 if Cancel selected
 */
int selectName(int evt, char *lib)
{
	static char	curpos=0, namepos=0, liblen=0, res[20];
	char	tmp[40];
	char	x, y, c, i;

	switch (evt)
	{
		case EVT_INIT:
			// add this function to treat event
			pushStack(selectName);
			printlcd(0, 0, lib);
			liblen = strlen(lib);
			x = 0;
			y = 2;
			for (i=0; i<20; i++)
			{
				sprintf(tmp, "%c", 'A'+i);
				printlcd(x+i, y, tmp);
			}
			y++;
			for (i=0; i<6; i++)
			{
				sprintf(tmp, "%c", 'U'+i);
				printlcd(x+i, y, tmp);
			}
			for (i=0; i<10; i++)
			{
				sprintf(tmp, "%c", '0'+i);
				printlcd(x+i+6, y, tmp);
			}
			printlcd(16, y, "-fin");
			lcd.blink();
			break;
		case EVT_BTNPREV:
			curpos--;
			if (curpos < 0)
				curpos = 37;
			break;
		case EVT_BTNNEXT:
			curpos++;
			if (curpos >= 38)
				curpos = 0;
			break;
		case EVT_BTNOK:
			if (curpos < 26)
				c = 'A' + curpos;
			else if (curpos < 36)
				c = '0' + curpos - 26;
			else switch (curpos)
			{
				case 36:
					c = '-';
					break;
				case 37:
					res[namepos] = '\0';
					return res;
			}
			sprintf(tmp, "%c", c);
			printlcd(liblen+1+namepos, 0, tmp);
			res[namepos] = c;
			if (namepos < 19)
				namepos++;
			break;
		case EVT_BTNCANCEL:
			if (namepos <= 0)
			{
			}
			printlcd(namepos--, 1, " ");
			break;
	}

	lcd.setCursor(curpos%20, curpos%20+1);
}

int selectVanne(int evt)
{
	static char	previous;
	char	tmp[40];

	// init
	if (evt == EVT_INIT)
	{
		// add this function to treat event
		pushStack(selectVanne);
		clearScreen();
		previous = Vanne;
	}
	else switch (evt)
	{
		case EVT_BTNPREV:
			if (Vanne > 1)
				Vanne--;
			break;
		case EVT_BTNNEXT:
			Vanne++;
			break;
		case EVT_BTNOK:
			previousMenu();	// go to previous menu
			return 1;
		case EVT_BTNCANCEL:
			Vanne = previous;
			previousMenu();	// go to previous menu
			return 1;
	}
	sprintf(tmp, "Vanne: %d <=", Vanne);
	printlcd(3, 0, tmp);
	return 1;
}

int menuVanne(int evt)
{
	char	*lst[] = {"Vanne:", "Ouvrir", "Fermer"};
	char	txt[40];
	int	ret;
	char	tmp[40];

	// init
	if (evt == EVT_INIT)
	{
		// add this function to treat event
		pushStack(menuVanne);
		addEvt(EVT_DISP);
		return 1;
	}

	sprintf(txt, "Vanne: %d", Vanne);
	lst[0] = txt;
	ret = menu(evt, lst, 3);
	switch (ret)
	{
		case 1:	selectVanne(EVT_INIT);
			break;
		case 2:	clearScreen();
			sprintf(tmp, "Vanne %d ouverte", Vanne);
			printlcd(0,1, tmp);
			delay(3000);
			addEvt(EVT_DISP);	// to refresh menu
			break;
		case 3:	clearScreen();
			sprintf(tmp, "Vanne %d fermee", Vanne);
			printlcd(0,1, tmp);
			delay(3000);
			addEvt(EVT_DISP);	// to refresh menu
			break;
		case -1:
			previousMenu();	// go to previous menu
			break;
	}
	return 1;	// evt used
}

int menuDemarrer(int evt)
{
	const char	*lst[] = {"Demarrer pompe", "Actionner vanne"};
	int	ret;
	char	tmp[40];

	// init
	if (evt == EVT_INIT)
	{
		// add this function to treat event
		pushStack(menuDemarrer);
		addEvt(EVT_DISP);
		return 1;
	}

	sprintf(tmp, "%u: menuDem evt %d", Tick, evt);
	printlcd(0,3,tmp);
//	delay(10000);

	ret = menu(evt, lst, 2);
//	sprintf(tmp, "%u: menuDem count %d", Tick, EvtCount);
//	printlcd(0,3,tmp);
//	delay(10000);
	switch (ret)
	{
		case 1:	menuPompe(EVT_INIT);
			break;
		case 2:	menuVanne(EVT_INIT);
			break;
		case -1:
			previousMenu();	// go to previous menu
			break;
	}
	return 1;	// evt used
}

/*!
 * \brief set hour and date
 *
 * \param evt event
 * \return 1
 */
int setClock(int evt)
{
	static char	field=0;

	switch (evt)
	{
		case EVT_INIT:
			// add this function to treat event
			pushStack(setClock);
			clearScreen();
			printlcd(0, 0, "Reglage horloge");
			printlcd(0, 1, "Heure: ");
			printlcd(0, 2, "Date: ");
			field = 0;
			selectTime(EVT_INIT, 7, 1);
			return 1;
			break;
		case EVT_BTNOK:
			if (field == 0)
			{
				field++;
				selectDate(EVT_INIT, 7, 2);
			}
			else
			{
				// TODO: set clock
				previousMenu();	
				addEvt(EVT_BTNOK);
			}
			return 1;
		case EVT_BTNCANCEL:
			if (field > 0)
			{
				field--;
				selectTime(EVT_INIT, 7, 1);
			}
			else
			{
				previousMenu();	
				addEvt(EVT_BTNCANCEL);
			}
			return 1;
	}
}

int menuConfigurer(int evt)
{
	const char	*lst[] = {"Regler horloge", "Configurer vanne", "Configurer plage"};
	int	ret;

	// init
	if (evt == EVT_INIT)
	{
		// add this function to treat event
		pushStack(menuConfigurer);
		addEvt(EVT_DISP);
		return 1;
	}

	ret = menu(evt, lst, 3);
	switch (ret)
	{
/*		
		case 2:	createPlage();
			break;
*/
		case 1:	setClock(EVT_INIT);
			break;
		case -1:
			previousMenu();	// go to previous menu
			break;
	}
	return 1;	// evt used
}

int menuPrincipal(int evt)
{
	const char	*lst[] = {"Demarrage manuel", "Configuration"};
	char	tmp[40];
	int	ret;

	// init
	if (evt == EVT_INIT)
	{
		// add this function to treat event
		pushStack(menuPrincipal);
		addEvt(EVT_DISP);
		return 1;
	}

	ret = menu(evt, lst, 2);
	switch (ret)
	{
		case 1:	menuDemarrer(EVT_INIT);
			break;
		case 2:	menuConfigurer(EVT_INIT);
			break;
		case -1:
			addEvt(EVT_DISP);
			break;
	}
	return 1;	// evt used
}

void setup()
{
	Wire.begin();
	lcd.begin(LCDWIDTH, LCDHEIGHT);
	lcd.init();
	lcd.backlight();

	// initialize digital pin LED_BUILTIN as an output.
	pinMode(LED_BUILTIN, OUTPUT);

	// button
	pinMode(BtnPrev,INPUT_PULLUP);
	pinMode(BtnNext,INPUT_PULLUP);
	pinMode(BtnOk,INPUT_PULLUP);
	pinMode(BtnCancel,INPUT_PULLUP);
  
	memset(EvtQ, EVT_NULL, sizeof(EvtQ));
	memset(Stack, 0, sizeof(Stack));
}

void loop()
{
	static int	first = 1;

	if (first)
	{
		delay(1000);	// pb affichage ecran
		menuPrincipal(EVT_INIT);
		first = 0;
	}
		
	getEvt();
	treatEvt();
	Tick++;
//	delay(500);
}


#if 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <gtk/gtk.h> 

#define	clear()	printf("\033[H\033[J")
#define	gotoxy(x,y)	printf("\033[%d;%dH", (y), (x))

#define	TEXTW	20
#define	TEXTH	4

char		TextBuf[TEXTW*TEXTH+(TEXTH-1)+1]; // '\n' and '\0'
char		TextWideBuf[256+TEXTW*TEXTH+(TEXTH-1)+1];
GtkWidget	*Text;

void OnDestroy(GtkWidget *pWidget, gpointer pData);

#define	SIZEX	300
#define	SIZEY	100


typedef struct plage_s {
	time_t	start;
	time_t	end;
	struct plage_s	*next;
} plage_t;


void use()
{
	printf("menu.x\n");
	exit(-1);
}

void printScreen()
{
	printf("----------------------\n");
	printf("|                    |\n");
	printf("|                    |\n");
	printf("|                    |\n");
	printf("|                    |\n");
	printf("----------------------");
}

void setText(int x, int y, char *text)
{
	char	*pIn, *pBuf;
	int	diff;

	if (!text || !*text)
		return;

	pBuf = TextBuf+(y*TEXTW)+x+y; 	// count '\n'
	pIn = text;
	diff = pBuf-TextBuf;
	g_print("diff=%d\n", diff);
	while (diff < sizeof(TextBuf) && *pIn)
	{
		// end of line ?
		if (diff > 0 && (diff == 20 || diff == 41 || diff == 62))
		{
			g_print("diff=%d add nl\n", diff);
			*pBuf++ = '\n';
		}
			
		*pBuf++ = *pIn++;
		diff = pBuf-TextBuf;
	}
	
	TextBuf[sizeof(TextBuf)-1] = '\0';
	sprintf(TextWideBuf, "<span font_desc=\"courier 12\">%s</span>", TextBuf);
#if 0
// gtk_label_set_markup(GTK_LABEL(label), "<span font_desc=\"sans 10\">4. This label using</span><span foreground=\"red\" font_desc=\"fixed 14\"> Pango markup</span> ")
//	gtk_label_set_text(Text, TextBuf);
#endif
	gtk_label_set_markup(GTK_LABEL(Text), TextWideBuf);
}

int menu(const char *lst[], int count)
{
	int	i, c, cur=0;

	printScreen();
	for (i=0; i<count; i++)
	{
/*
		gotoxy(5, i+2);
		printf(lst[i]);
		printf("\n");
*/
		setText(5, i+2, lst[i]);
	}
	while (1)
	{
		gotoxy(1, 5+2);
		printf("P(revious) N(ext) O(k) C(ancel)");
		gotoxy(2,cur+2);
		printf("=>");
		gotoxy(1, 8);
		c = fgetc(stdin);
		if (toupper(c) == 'P')
		{
			gotoxy(2,cur+2);
			printf("  ");
			cur = ((cur+count-1)%count);
		}
		else if (toupper(c) == 'N')
		{
			gotoxy(2,cur+2);
			printf("  ");
			cur = (((cur+1)%count));
		}
		else if (toupper(c) == 'O')
		{
			printf("return %d\n", cur);
			return cur;
		}
		else if (toupper(c) == 'C')
		{
			printf("return -1\n");
			return -1;
		}
	}

	return 0;
}

/*!
 * \brief select a digit
 *
 * \param x x coordinate
 * \param y y coordinate
 * \param min minimum value allowed
 * \param max maximum value allowed
 * \param def default value
 * \return digit selected, or -1 if Cancel selected
 */
int selectDigitold(int x, int y, int min, int max, int def)
{
	int	cur=0;
	char	c;

	cur = def;
	while (1)
	{
		gotoxy(x, y);
		printf("%d", cur);
		gotoxy(x, y+1);
		printf("^");
		gotoxy(1, 8);
		c = fgetc(stdin);
		if (toupper(c) == 'P')
		{
			cur -= 1;
			if (cur < min)
				cur = max;
		}
		else if (toupper(c) == 'N')
		{
			cur += 1;
			if (cur > max)
				cur = min;
		}
		else if (toupper(c) == 'O')
		{
			gotoxy(x, y+1);
			printf(" ");
			return cur;
		}
		else if (toupper(c) == 'C')
		{
			gotoxy(x, y+1);
			printf(" ");
			return -1;
		}
	}
}

int selectHourold(int x, int y)
{
	int	h, m, v;

	while (1)
	{
		gotoxy(x, y);
		printf("12:00");
		h = selectDigit(x, y, 0, 2, 1);
		if (h < 0)
			return -1;
		v = selectDigit(x+1, y, 0, 9, 2);
		if (v < 0)
			continue;
		h = h * 10 + v;
		if (h > 23)
			h = 23;
		m = selectDigit(x+3, y, 0, 5, 0);
		if (m < 0)
			continue;
		v = selectDigit(x+4, y, 0, 9, 0);
		if (v < 0)
			continue;
		m = m * 10 + v;
		return h*100+m;
	}
}

int selectValue(int x, int y, const char *v[])
{
	int	choice = 0;
	char	c;

	while (1)
	{
		gotoxy(x, y);
		printf("%s", v[choice]);
		c = fgetc(stdin);
		if (toupper(c) == 'P')
		{
			choice--;
		}
		else if (toupper(c) == 'N')
		{
			choice++;
		}
		else if (toupper(c) == 'O')
		{
			return choice;
		}
		else if (toupper(c) == 'C')
		{
			return -1;
		}
		if (choice < 0)
		{
			choice = 0;
			while (v[choice][0])
				choice++;
			if (choice > 0)
				choice--;
		}
		else if (v[choice][0] == '\0')
			choice = 0;
	}
}
char *selectName(char *res)
{
	int	cursor=0, namepos=0, i;
	char	c, l;

	gotoxy(2, 3);
	printf("                    ");
	while (1)
	{
		gotoxy(2, 4);
		for (i=0; i<20; i++)
			printf("%c", 'A'+i);
		gotoxy(2, 5);
		for (i=0; i<6; i++)
			printf("%c", 'U'+i);
		for (i=0; i<10; i++)
			printf("%c", '0'+i);
		printf("-fin");
		gotoxy(4+namepos, 3);
		printf("_");
		gotoxy(2+cursor%20, 4+cursor/20);
		c = fgetc(stdin);
		if (toupper(c) == 'P')
		{
			cursor = (cursor-1+38)%38;
		}
		else if (toupper(c) == 'N')
		{
			cursor = (cursor+1)%38;
		}
		else if (toupper(c) == 'O')
		{
			if (cursor < 26)
				l = 'A' + cursor;
			else if (cursor < 36)
				l = '0' + cursor - 26;
			else switch (cursor)
			{
				case 36:
					l = '-';
					break;
				case 37:
					res[namepos] = '\0';
					gotoxy(2, 4);
					printf("                    ");
					gotoxy(2, 5);
					printf("                    ");
					printf("res=%s", res);

					return res;
			}
			gotoxy(4+namepos, 3);
			printf("%c", l);
			res[namepos] = l;
			if (namepos < 19)
				namepos++;
		}
		else if (toupper(c) == 'C')
		{
			gotoxy(4+namepos, 3);
			printf(" ");
			if (namepos > 0)
				namepos--;
			else
			{
				gotoxy(2, 4);
				printf("                    ");
				gotoxy(2, 5);
				printf("                    ");
				return NULL;
			}
		}
	}
}

plage_t *createPlage()
{
	char	c, name[20];
	short	start, end;
	const char	*types[] = {"Hebdomadaire", "Périodique  ", ""};
	const char	*days[] = {"Lundi       ", "Mardi       ", "Mercredi    ", "Jeudi       ", "Vendredi     ", "Samedi      ", "Dimanche    ", "Tous         ", ""};

	name[0] = '\0';
	while (1)
	{
		gotoxy(3, 2);
		printf("Nom de la plage :");
		if (selectName(name) == NULL)
			return NULL;
		gotoxy(3, 2);
		printf("                    ");
		gotoxy(3, 2);
		printf("Plage : %s", name);
		gotoxy(3, 3);
		printf("Type : ");
		selectValue(10, 3, types);
		gotoxy(3, 3);
		printf("Jour : ");
		selectValue(10, 3, days);
		gotoxy(3, 3);
		printf("Début : ");
		start = selectHour(11,3);
		if (start < 0)
			continue;
		gotoxy(3, 4);
		printf("Fin   : ");
		end = selectHour(11,4);
		if (end < 0)
			continue;
		gotoxy(3, 5);
		printf("Ok=Créer, Can=Annul");

		while (1)
		{
			c = fgetc(stdin);
			gotoxy(3,10);
			if (toupper(c) == 'P')
				continue;
			else if (toupper(c) == 'O')
			{
				return NULL;
			}
			else if (toupper(c) == 'C')
			{
				return NULL;
			}
		}
	}
}

void menuConfigurer()
{
	const char	*lst[] = {"Régler heure", "Configurer vanne", "Configurer plage"};
	int	ret;

	while (1)
	{
		ret = menu(lst, 3);
		switch (ret)
		{
			case 2:	//createPlage();
				break;
			case 1:	//menuConfigurer();
				break;
		}
	}

}

void menuPompe()
{
	const char	*lst[] = {"Démarrer", "Arrêter"};
	int	ret;

	ret = menu(lst, 2);
	switch (ret)
	{
		case 0:	printf("Pompe démarrée");
			break;
		case 1:	printf("Pompe arrêtée");
			break;
	}
	sleep(2);
}

int selectVanne()
{
	int	cur=0, count=6;
	char	c;

	while (1)
	{
		gotoxy(12, 2);
		printf("%d <=", cur+1);
		gotoxy(1, 8);
		c = fgetc(stdin);
		if (toupper(c) == 'P')
		{
			cur = ((cur+count-1)%count);
		}
		else if (toupper(c) == 'N')
		{
			cur = (((cur+1)%count));
		}
		else if (toupper(c) == 'O')
		{
			gotoxy(14, 2);
			printf("  ");
			return cur;
		}
		else if (toupper(c) == 'C')
		{
			gotoxy(12, 2);
			printf("1   ");
			return 0;
		}
	}
}

int menuVanne()
{
	int	c, cur=0, count=3;

	printScreen();
	gotoxy(5, 2);
	printf("Vanne: 1");
	gotoxy(5, 3);
	printf("Démarrer");
	gotoxy(5, 4);
	printf("Arrêter");
	while (1)
	{
		gotoxy(1, 5+2);
		printf("P(revious) N(ext) O(k) C(ancel)");
		gotoxy(2,cur+2);
		printf("=>");
		gotoxy(1, 8);
		c = fgetc(stdin);
		if (toupper(c) == 'P')
		{
			gotoxy(2,cur+2);
			printf("  ");
			cur = ((cur+count-1)%count);
		}
		else if (toupper(c) == 'N')
		{
			gotoxy(2,cur+2);
			printf("  ");
			cur = (((cur+1)%count));
		}
		else if (toupper(c) == 'O')
		{
			if (cur == 0)
			{
				gotoxy(2,cur+2);
				printf("  ");
				selectVanne();
			}
			else
			{
				printf("return %d\n", cur);
				return cur;
			}
		}
		else if (toupper(c) == 'C')
		{
			printf("return -1\n");
			return -1;
		}
	}

	return 0;
}

void menuDemarrer()
{
	const char	*lst[] = {"Démarrer pompe", "Actionner vanne"};
	int	ret;

	while (1)
	{
		ret = menu(lst, 2);
		switch (ret)
		{
			case 0:	menuPompe();
				break;
			case 1:	menuVanne();
				break;
			case -1: return;
		}
	}

}

void menuPrincipal()
{
	const char	*lst[] = {"Démarrage manuel", "Configuration"};
	int	ret;

	while (1)
	{
		ret = menu(lst, 2);
		switch (ret)
		{
			case 0:	menuDemarrer();
				break;
			case 1:	menuConfigurer();
				break;
		}
	}
}

int mainold(int argc, char *argv[])
{
	menuPrincipal();
	return 0;
}

#endif
