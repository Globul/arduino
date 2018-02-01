#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Construct an LCD object and pass it the 
// I2C address, width (in characters) and
// height (in characters). Depending on the
// Actual device, the IC2 address may change.
//LiquidCrystal_I2C lcd(0x3F, 16, 2);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Déclaration du tableau définissant l'image
// Un bit à '0' correspont à une LED éteinte, un bit à '1' à une LED allumée
byte fortyTwo[] =
{
  B00000000,
  B00100000,
  B01100111,
  B10100001,
  B11110111,
  B00100100,
  B00000111,
  B00000000,
};

// Déclaration des pins utilisées pour le branchement de la matrice
const int columnPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
const int rowPins[] = {10, 11, 12, 15, 16, 17, 18, 19};

// Sous-routine pour la commande des LEDs à partir des données du tableau
// Elle s'affiche pendant la durée donnée en second paramètre
void aff(byte * image, unsigned long duree)
{
  // On note le moment où l'on commence
 unsigned long debut = millis();
 // On joue la boucle tant qu'on n'a pas atteint la durée souhaitée
 while (debut + duree > millis())
  {
    // Première itération (boucle for) sur les lignes
    for(int row = 0; row < 8; row++)
    {
      // Connexion de la ligne au +5V
      digitalWrite(rowPins[row], HIGH);
      // Seconde itération sur les colonnes
      for(int col = 0; col < 8; col++)
      {
        // Miroir sur le poids du bit pour avoir le bon numéro de colonne
        int situ = 7 - col;
        // Lecture du bit concerné
        boolean pixel = bitRead(image[row],col);
        // S'il est à un...
        if(pixel == 1)
        {
          // ...on connecte la colonne concernée à la masse pour allumer la LED
          digitalWrite(columnPins[situ], LOW);
        }
        // On attend un peu entre chaque LED (délai total ~20ms pour 64 LEDs)
        delayMicroseconds(300);
        // Et on déconnecte la colonne de la masse...
        digitalWrite(columnPins[situ], HIGH);
      }
      // ...puis la ligne de l'alimentation
      digitalWrite(rowPins[row], LOW);
    }
  }
}

void printmsg(int c, int l, char *str)
{
  lcd.setCursor(c, l);      
  lcd.print(str);
}

void aff2()
{
    char tmp[20];
    
    // Première itération (boucle for) sur les lignes
    for(int row = 0; row < 3; row++)
    {
      // Seconde itération sur les colonnes
      for(int col = 0; col < 3; col++)
      {
          // Connexion de la ligne au +5V
          sprintf(tmp, "row=%d HIGH    ", row);
          printmsg(0,1, tmp);
          digitalWrite(rowPins[row], HIGH);
          delay(5000);
          sprintf(tmp, "col=%d LOW    ", col);
          printmsg(0,0, tmp);
          digitalWrite(columnPins[col], LOW);
          delay(5000);
          sprintf(tmp, "col=%d HIGH    ", col);
          printmsg(0,0, tmp);
          digitalWrite(columnPins[col], HIGH);
          delay(5000);
          sprintf(tmp, "row=%d LOW    ", row);
          printmsg(0,1, tmp);
          digitalWrite(rowPins[row], LOW);
          delay(5000);
        }
      }
}

void alltype(int isRow, int st)
{
  int i;
    // Première itération (boucle for) sur les lignes
    for(i = 0; i < 3; i++)
    {
      if (isRow)
          digitalWrite(rowPins[i], st);
      else
          digitalWrite(columnPins[i], st);
     }
}


void setup()
{
  Wire.begin();
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
  lcd.print("BONJOUR");

  // Move the cursor to the next line and print
  // WORLD.
  lcd.setCursor(5, 1);      
  lcd.print("MAITRE");

  // Utilisation d'une boucle for pour optimiser l'initialisation
  for (int i = 0; i < 3; i++)
  {
    pinMode(rowPins[i], OUTPUT);        // lignes en sorties
    pinMode(columnPins[i], OUTPUT);     // colonnes en sorties
   
    /* Optionnel - permet d'éviter un "scintillement" au démarrage du programme
    digitalWrite(columnPins[i], HIGH);  // déconnexion des cathodes de la masse
    */
  }
}

void loop()
{
  // Délai entre deux affichages
  int tempo = 250 ;
  // Afficher le nombre pendant une seconde
//  aff(fortyTwo, 1000);
  // ne rien afficher pendant la temporisation définie
//  delay(tempo);
  printmsg(0,1, "all col=LOW row=HIGH");
  alltype(1, LOW);
  alltype(0, HIGH);
  delay(5000);
  printmsg(0,0, "col 0 LOW");
  digitalWrite(columnPins[0], LOW);
  delay(5000);
  printmsg(0,0, "col 0 HIGH");
  digitalWrite(columnPins[0], HIGH);
  delay(5000);
  printmsg(0,0, "row 0 HIGH");
  digitalWrite(rowPins[0], HIGH);
  delay(5000);
  
}


 
 
