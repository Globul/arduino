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


const int LedRouge=0;	// D3
const int LedVert=2;	// D4
const int LedBleu=14;	// D5

void setup()   { // debut de la fonction setup()
	pinMode (LedVert,OUTPUT); // Broche LedVert configurée en sortie
	pinMode (LedRouge,OUTPUT); // Broche LedRouge configurée en sortie
	pinMode (LedBleu,OUTPUT); // Broche LedBleu configurée en sortie
	randomSeed(analogRead(0));
}

#define MAXVAL  1023
void loop(){ // debut de la fonction loop()
	static int	r=0, v=0, b=0, first=1;
	int		nr, nv, nb, dr, dv, db, maxd;

  digitalWrite(LedRouge,HIGH); // éteint la couleur voulue
  digitalWrite(LedVert,HIGH); // éteint la couleur voulue
  digitalWrite(LedBleu,HIGH); // éteint la couleur voulue

	if (first)
	{
		//---- rouge ---
    ledRVBpwm(MAXVAL,0,0);
    delay(3000); // pause
    ledRVBpwm(0,0,0);
		delay(1000); // pause

		//---- vert ---
    ledRVBpwm(0,MAXVAL,0);
		delay(2000); // pause
    ledRVBpwm(0,0,0);
		delay(1000); // pause

		//---- bleu ---
    ledRVBpwm(0,0,MAXVAL);
		delay(1000); // pause
    ledRVBpwm(0,0,0);
		delay(1000); // pause
		first = 0;

	}

	//----- test des couleurs au format RVB ----
	nr = random(MAXVAL+1);
	nv = random(MAXVAL+1);
	nb = random(MAXVAL+1);

	dr = nr - r;
	dv = nv - v;
	db = nb - b;

	maxd = abs(dr);
	if (abs(dv) > maxd)
		maxd = abs(dv);
	if (abs(db) > maxd)
		maxd = abs(db);

	for (int i=0; i<=maxd; i++)
	{
		nr = r + i*dr/maxd;
		nv = v + i*dv/maxd;
		nb = b + i*db/maxd;
		ledRVBpwm(nr,nv,nb);
		delay(10); //pause
	}
  r = nr;
  v = nv;
  b = nb;

//  ledRVBpwm(0,0,0);
//	delay(1000);
}

//---- fonction pour variation progressive des couleurs ----

void ledRVBpwm(int pwmRouge, int pwmVert, int pwmBleu) { // reçoit valeur 0-255 par couleur

   //--- attention - avec une LED RGB anode commune : la LED s'allume sur niveau BAS !

 analogWrite(LedRouge, MAXVAL-pwmRouge); // impulsion largeur voulue sur la broche 0 = 0% et 255 = 100% haut
 analogWrite(LedVert, MAXVAL-pwmVert); // impulsion largeur voulue sur la broche 0 = 0% et 255 = 100% haut
 analogWrite(LedBleu, MAXVAL-pwmBleu); // impulsion largeur voulue sur la broche 0 = 0% et 255 = 100% haut


}
// ////////////////////////// Fin du programme ////////////////////
