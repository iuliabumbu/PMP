#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <FastLED.h>
#define PIN 6

#include <string.h>
#include <Arduino.h>
#include <SPI.h>
#include <SoftwareSerial.h>

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
 
#include "BluefruitConfig.h"

#define FACTORYRESET_ENABLE     1
#define NUMPIXELS      64

//declaram sirul de pixeli folosit cu libraria FastLed
CRGB fastPixels[NUMPIXELS];

//declaram matricea de 8x8
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN,
                            NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
                            NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                            NEO_GRB            + NEO_KHZ800);

//declaram modululbluetooth utilizat                            
Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);     

//afisam eroarea si ramanem in bucla infinita pentru a nu se continua cat timp nu este remediata
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}
 
// prototipul functiilor din packetparser.cpp utilizate pentru parsarea informatiilor primite prin bluetooth
uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);
float parsefloat(uint8_t *buffer);
void printHex(const uint8_t * data, const uint32_t numBytes);
 
// bufferul in care pastram informatiile primite
extern uint8_t packetbuffer[];

//culoare predefinita
uint8_t red = 100;
uint8_t green = 100;
uint8_t blue = 100;

//starea de inceput
uint8_t animationState = 1;
//mesaj de intampinare
String text = "Welcome";
//mesaj predefinit al animatiei
String message = "Vacanta";
String inputString = "";
boolean stringComplete = false;

void setup(void)
{
  //rezervam 200 de caractere pentru citirea din interfata seriala
  inputString.reserve(200);

  //adaugam ledurile FastPixels
  LEDS.addLeds<WS2812,PIN,RGB>(fastPixels, NUMPIXELS);
  LEDS.setBrightness(80);

  //initializam starea matricii
  matrix.begin();
  matrix.setBrightness(80);

  //redarea animatiei de intampinare
  textAnimation(matrix.Color(red, green, blue), text);
  matrix.fillScreen(0);
  matrix.show();

//activam comunicarea seriala
  Serial.begin(115200);
  Serial.println("My Project: Wearable Matrix Animation");
  Serial.println("------------------------------------------------");
 

  Serial.print("Initializam modulul Bluefruit LE: ");

 //pentru a se putea initializa modulul acesta trebuie sa fie setat in modul Cmd
 //daca intervine o eroare programul se opreste
  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Modulul nu a fost gasit, verificati daca acesta este setat in Command Mode precum si firele!"));
  }
  Serial.println( F("Modul initializat!") );

 //se recomanda efectuarea unui reset din fabrica pentru a ne asigura ca modulul este intr-o stare consistenta, stergand setarile care persistau de la utilizarile anterioare
  if ( FACTORYRESET_ENABLE )
  {

    Serial.println("Se efectueaza resetarea din fabrica");
    if ( ! ble.factoryReset() ){
      error(F("Eroare la incercarea resetarii, va rugam inchideti fereastra si reincercati!"));
    }
  }
//dezactivam comanda echo a modulului deoarece nu o utilizam
  ble.echo(false);
 
  Serial.println("Informatii despre Bluefruit:");

  ble.info();
 
  Serial.println("Porniti aplicatia Bluefruit Connect si accesati Controller");


//setat pe false pentru a ajuta la efectuarea unui eventual debug
  ble.verbose(false);
 
//asteptam pana cand aplicatia mobila se conecteaza la modulul Bluefruit
  while (! ble.isConnected()) {
      delay(500);
  }
 
  Serial.println(F("--------------------------------------"));
  Serial.println("Acum puteti folosi color picker/controller-ul!");
  // modulul Bluefruit trebuie setat in Data mode pentru a prelua informatii
  Serial.println( F("Setati modulul in modul Data,folosind switch-ulacestuia!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);
 
  Serial.println(F("--------------------------------------"));
}

void loop(void)
{
  //citim informatiile primite de la modulul Bluefruit
  uint8_t len = readPacket(&ble, BLE_READPACKET_TIMEOUT);

  //in functie de packetbuffer[1] stabilim daca am folosit color picker sau controlul butoanelor
  //culoare
  if (packetbuffer[1] == 'C') {
    
    //daca am primit o culoare citim valorile RGB si le afisam pe ecran
    red = packetbuffer[2];
    green = packetbuffer[3];
    blue = packetbuffer[4];
    Serial.print ("RGB #");
    if (red < 0x10) Serial.print("0");
    Serial.print(red, HEX);
    if (green < 0x10) Serial.print("0");
    Serial.print(green, HEX);
    if (blue < 0x10) Serial.print("0");
    Serial.println(blue, HEX);
  }
 
  // butoane
  if (packetbuffer[1] == 'B') {
    
    //daca am accesat un buton citim numarul acestuia si verificam cand este apasat
    uint8_t number = packetbuffer[2] - '0';
    boolean pressed = packetbuffer[3] - '0';
    Serial.print("Button ");
    Serial.print(number);

    //starea animatiei se schimba in functie de numarul butonului apasat
    animationState = number;
    
    if (pressed) {
      Serial.println(" pressed");
    } else {
      Serial.println(" released");
    }
  }
    //daca am ajuns aici inseamna ca se cunoaste atat animatia dorita cat si culoarea

    //animatia butonului 1 in forma de inima si culoare primita de la modul
      if (animationState == 1){
        matrix.fillScreen(0);
        heartAnimation(matrix.Color(red, green, blue));
        matrix.show();
      }
    //animatia butonului 2 in forma de ceasca de cafea si culoare primita de la modul
      if (animationState == 2){
        matrix.fillScreen(0);
        coffeeAnimation(matrix.Color(red, green, blue));
        matrix.show();
      }
    //animatia butonului 3 in forma de expresii faciale (fericire si tristete) si culoare primita de la modul
      if (animationState == 3){
        matrix.fillScreen(0);
        emotionsAnimation(matrix.Color(red, green, blue));
        matrix.show(); 
      }
    //animatia butonului 4 in forma de fulgi de zapada si culoare primita de la modul
      if (animationState == 4){ 
        matrix.fillScreen(0);
        snowflakesAnimation(matrix.Color(red, green, blue));
        matrix.show(); 
      }
     //animatia butonului 5(sageata in sus) care preia un text si il afiseaza pe litere in culoarea primita de la modul
      if(animationState == 5){
        //string-ul este complet daca exista un cuvant trimis de catre utilizator in interfata seriala
         if (stringComplete) {
                 matrix.fillScreen(0);
                 textAnimation(matrix.Color(red, green, blue), inputString);
                 matrix.show();     
                 // golim sirul pentru a-l pregati pentru o noua citire
                 inputString = "";
                 // resetam variabila care verifica daca stringul este complet
                 stringComplete = false;
          }
          //altfel folosim un cuvant predefinit
          else{
           matrix.fillScreen(0);
           textAnimation(matrix.Color(red, green, blue), message);
           matrix.show(); // This sends the updated pixel colors to the hardware.     
          }

           
      }
      //animatia butonului 6 (sageata in jos) afiseaza multiple puncte cu culori generate aleator
      if (animationState == 6){ 
        matrix.fillScreen(0);
        dotsAnimation();
        matrix.show();
      }
       //animatia butonului 7 (sageata la stanga)foloseste libraria FastLed si coloreaza succesiv liniile matricii
      if (animationState == 7){ 
        lineAnimation();
      }
      //animatia butonului 8 (sageata la dreapta). 
      if (animationState == 8){ 
         spiralsAnimation();
      }

     
         
}

//functie folosita pentru citirea din interfata seriala
void serialEvent() {
     while (Serial.available()) {
     char inChar = (char)Serial.read();
     
     //cuvantul se formeaza caracter cu caracter
     if (inChar != '\n')
        inputString += inChar;

      //sirul este complet cand se citeste caracterul '\n'
     if (inChar == '\n') {
        stringComplete = true;
        Serial.println("The input string is: " + inputString);
     }
     }
}
//sirurile pentru fiecare patrat al spiralei
uint8_t v1[] = {0, 1, 2, 3, 4, 5, 6, 7, 15, 23, 31, 39, 47, 55, 63, 62, 61, 60, 59, 58, 57, 56, 48, 40, 32, 24, 16, 8};
uint8_t v2[] = {9, 10, 11, 12, 13, 14, 22, 30, 38, 46, 54, 53, 52, 51, 50, 49, 41, 33, 25, 17};
uint8_t v3[] = {18, 19, 20, 21, 29, 37, 45, 44, 43, 42, 34, 26};
uint8_t v4[] = {27, 28, 36, 35};

//sirul cu dimensiunile fiecarui patrat al spiralei
uint8_t vSize[] = {sizeof(v1), sizeof(v2), sizeof(v3), sizeof(v4)}; 
//vectorul v contine toate patratele, putand fi astfel accesat fiecare pixel
const unsigned char * v[] ={v1, v2, v3, v4}; 

//spirala 1 incepe de la interior spre exterior
void spiral1(int hue) {    
  for(int i = 0; i < 4; i++) {
      int x = vSize[3-i];  
      for(int j = 0; j < x; j++) {
        fastPixels[v[3-i][x-1-j]]= CHSV(hue, 255,255);
        FastLED.show();
        FastLED.delay(25);     
      }
  }

}

//spirala 2 incepe de la exterior spre interior
void spiral2(int hue) {    
    for(int i = 0; i < 4; i++) {
      int x = vSize[i];  
      for(int j = 0; j < x; j++) {
         fastPixels[v[i][j]]= CHSV(hue, 255,255);
         FastLED.show();
         FastLED.delay(25);
      }
  }
}

//animatia 8: genereaza o nuanta aleatoare si o aplica pe prima spirala, iar cea de a doua spirala va avea culoarea complementara
void spiralsAnimation(){
  int hue = random(0, 255);
  spiral1(hue);
  spiral2(255-hue);
  fill_solid( fastPixels, NUMPIXELS, CRGB::Black);
}

//animatia 7
void lineAnimation(){
  int hue = 0;
  //pentru fiecare led din cei NUMPIXELS existenti setam culoarea si incrementam nuanta pentru configurarea unui efect de trecere gradata
  for(int i = 0; i < NUMPIXELS; i++) {
    //seteaza culoarea ledului i
    fastPixels[i] = CHSV(hue++, 255, 255);
    //afiseaza schimbarile ledurilor
    FastLED.show(); 
    delay(10);
  }
 
  //dupa setarea tuturor ledurilor se reincepe in ordine inversa 
  for(int i = NUMPIXELS - 1; i >= 0; i--) {
    fastPixels[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    delay(10);
  }
}

//functii folosite pentru desenarea literelor din alfabet
void letterA(uint32_t c) {
  matrix.drawLine(1, 3, 1, 4, c);
  matrix.drawLine(4, 2, 4, 5, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(3, 2, c);
  matrix.drawPixel(3, 5, c);
  matrix.drawPixel(5, 2, c);
  matrix.drawPixel(5, 5, c);
  matrix.drawPixel(6, 2, c);
  matrix.drawPixel(6, 5, c);
}
void letterB(uint32_t c) {
  matrix.drawLine(1, 3, 1, 5, c);
  matrix.drawLine(3, 3, 3, 5, c);
  matrix.drawLine(6, 2, 6, 4, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(4, 2, c);
  matrix.drawPixel(4, 5, c);
  matrix.drawPixel(5, 2, c);
  matrix.drawPixel(5, 5, c);
}
void letterC(uint32_t c) {
  matrix.drawLine(1, 3, 1, 4, c);
  matrix.drawLine(6, 3, 6, 4, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(3, 5, c);
  matrix.drawPixel(4, 2, c);
  matrix.drawPixel(5, 2, c);
  matrix.drawPixel(5, 5, c);
}
void letterD(uint32_t c) {
  matrix.drawLine(1, 3, 1, 5, c);
  matrix.drawLine(6, 2, 6, 4, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(3, 2, c);
  matrix.drawPixel(3, 5, c);
  matrix.drawPixel(4, 2, c);
  matrix.drawPixel(4, 5, c);
  matrix.drawPixel(5, 2, c);
  matrix.drawPixel(5, 5, c);
}
void letterE(uint32_t c) {
  matrix.drawLine(1, 2, 1, 5, c);
  matrix.drawLine(3, 3, 3, 5, c);
  matrix.drawLine(6, 2, 6, 5, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(4, 2, c);
  matrix.drawPixel(5, 5, c);
}
void letterF(uint32_t c) {
  matrix.drawLine(1, 2, 1, 5, c);
  matrix.drawLine(3, 3, 3, 5, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(4, 2, c);
  matrix.drawPixel(5, 5, c);
  matrix.drawPixel(6, 2, c);
}
void letterG(uint32_t c) {
  matrix.drawLine(1, 3, 1, 4, c);
  matrix.drawLine(4, 4, 4, 5, c);
  matrix.drawLine(6, 3, 6, 4, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(3, 5, c);
  matrix.drawPixel(4, 2, c);
  matrix.drawPixel(5, 2, c);
  matrix.drawPixel(5, 5, c);
}
void letterH(uint32_t c) {
  matrix.drawLine(4, 2, 4, 5, c);
  matrix.drawPixel(1, 2, c);
  matrix.drawPixel(1, 5, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(3, 2, c);
  matrix.drawPixel(3, 5, c);
  matrix.drawPixel(4, 2, c);
  matrix.drawPixel(4, 5, c);
  matrix.drawPixel(5, 2, c);
  matrix.drawPixel(5, 5, c);  
  matrix.drawPixel(6, 2, c);
  matrix.drawPixel(6, 5, c);
}
void letterI(uint32_t c) {
  matrix.drawLine(1, 3, 1, 5, c);
  matrix.drawLine(6, 2, 6, 4, c);
  matrix.drawPixel(2, 3, c);
  matrix.drawPixel(3, 4, c);
  matrix.drawPixel(4, 3, c);
  matrix.drawPixel(5, 4, c);
}
void letterJ(uint32_t c) {
  matrix.drawLine(6, 3, 6, 4, c);
  matrix.drawPixel(1, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(3, 2, c);
  matrix.drawPixel(4, 5, c);
  matrix.drawPixel(5, 2, c);
  matrix.drawPixel(5, 5, c);
}
void letterK(uint32_t c) {
  matrix.drawLine(3, 4, 3, 5, c);
  matrix.drawPixel(1, 3, c);
  matrix.drawPixel(1, 5, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 4, c);
  matrix.drawPixel(4, 2, c);
  matrix.drawPixel(4, 4, c);
  matrix.drawPixel(5, 2, c);
  matrix.drawPixel(5, 5, c);
  matrix.drawPixel(6, 2, c);
  matrix.drawPixel(6, 5, c);
}
void letterL(uint32_t c) {
  matrix.drawLine(6, 2, 6, 5, c);
  matrix.drawPixel(1, 5, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(3, 5, c);
  matrix.drawPixel(4, 2, c);
  matrix.drawPixel(5, 5, c);
}
void letterM(uint32_t c) {
  matrix.drawLine(1, 1, 1, 2, c);
  matrix.drawLine(1, 4, 1, 5, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 4, c);
  matrix.drawPixel(2, 6, c);
  matrix.drawPixel(3, 1, c);
  matrix.drawPixel(3, 3, c);
  matrix.drawPixel(3, 5, c);
  matrix.drawPixel(4, 2, c);
  matrix.drawPixel(4, 6, c);
  matrix.drawPixel(5, 1, c);
  matrix.drawPixel(5, 5, c);
  matrix.drawPixel(6, 2, c);
  matrix.drawPixel(6, 6, c);
}
void letterN(uint32_t c) {
  matrix.drawLine(2, 2, 2, 3, c);
  matrix.drawLine(4, 5, 4, 6, c);
  matrix.drawPixel(1, 1, c);
  matrix.drawPixel(1, 5, c);
  matrix.drawPixel(2, 6, c);
  matrix.drawPixel(3, 1, c);
  matrix.drawPixel(3, 3, c);
  matrix.drawPixel(3, 5, c);
  matrix.drawPixel(4, 2, c);
  matrix.drawPixel(5, 1, c);
  matrix.drawPixel(5, 5, c);
  matrix.drawPixel(6, 2, c);
  matrix.drawPixel(6, 6, c);
}
void letterO(uint32_t c) {
  matrix.drawLine(1, 3, 1, 4, c);
  matrix.drawLine(6, 3, 6, 4, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(3, 2, c);
  matrix.drawPixel(3, 5, c);
  matrix.drawPixel(4, 2, c);
  matrix.drawPixel(4, 5, c);
  matrix.drawPixel(5, 2, c);
  matrix.drawPixel(5, 5, c);
}
void letterP(uint32_t c) {
  matrix.drawLine(1, 3, 1, 5, c);
  matrix.drawLine(4, 2, 4, 4, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(3, 2, c);
  matrix.drawPixel(3, 5, c);
  matrix.drawPixel(5, 5, c);
  matrix.drawPixel(6, 2, c);
}
void letterQ(uint32_t c) {
  matrix.drawLine(1, 3, 1, 4, c);
  matrix.drawLine(6, 3, 6, 5, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(3, 2, c);
  matrix.drawPixel(3, 5, c);
  matrix.drawPixel(4, 2, c);
  matrix.drawPixel(4, 5, c);
  matrix.drawPixel(5, 2, c);
  matrix.drawPixel(5, 5, c);
  matrix.drawPixel(7, 1, c);
}
void letterR(uint32_t c) {
  matrix.drawLine(1, 3, 1, 5, c);
  matrix.drawLine(4, 2, 4, 4, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(3, 2, c);
  matrix.drawPixel(3, 5, c);
  matrix.drawPixel(5, 2, c);
  matrix.drawPixel(5, 5, c);
  matrix.drawPixel(6, 2, c);
  matrix.drawPixel(6, 5, c);
}
void letterS(uint32_t c) {
  matrix.drawLine(1, 2, 1, 4, c);
  matrix.drawLine(4, 3, 4, 5, c);
  matrix.drawLine(6, 2, 6, 4, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(3, 5, c);
  matrix.drawPixel(5, 2, c);
}
void letterT(uint32_t c) {
  matrix.drawLine(1, 1, 1, 5, c);
  matrix.drawPixel(2, 4, c);
  matrix.drawPixel(3, 3, c);
  matrix.drawPixel(4, 4, c);
  matrix.drawPixel(5, 3, c);
  matrix.drawPixel(6, 4, c);
}
void letterU(uint32_t c) {
  matrix.drawLine(6, 2, 6, 5, c);
  matrix.drawPixel(1, 2, c);
  matrix.drawPixel(1, 5, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(3, 2, c);
  matrix.drawPixel(3, 5, c);
  matrix.drawPixel(4, 2, c);
  matrix.drawPixel(4, 5, c);
  matrix.drawPixel(5, 2, c);
  matrix.drawPixel(5, 5, c);
}
void letterV(uint32_t c) {
  matrix.drawPixel(1, 1, c);
  matrix.drawPixel(1, 5, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 6, c);
  matrix.drawPixel(3, 1, c);
  matrix.drawPixel(3, 5, c);
  matrix.drawPixel(4, 2, c);
  matrix.drawPixel(4, 6, c);
  matrix.drawPixel(5, 1, c);
  matrix.drawPixel(5, 5, c);
  matrix.drawPixel(6, 3, c);
  matrix.drawPixel(6, 5, c);
  matrix.drawPixel(7, 3, c);
}
void letterW(uint32_t c) {
  matrix.drawLine(5, 1, 5, 2, c);
  matrix.drawLine(5, 4, 5, 5, c);
  matrix.drawPixel(1, 1, c);
  matrix.drawPixel(1, 5, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 6, c);
  matrix.drawPixel(3, 1, c);
  matrix.drawPixel(3, 5, c);
  matrix.drawPixel(4, 2, c);
  matrix.drawPixel(4, 4, c);
  matrix.drawPixel(4, 6, c);
  matrix.drawPixel(6, 2, c);
  matrix.drawPixel(6, 6, c);
}
void letterX(uint32_t c) {
  matrix.drawPixel(1, 2, c);
  matrix.drawPixel(1, 5, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(3, 4, c);
  matrix.drawPixel(4, 4, c);
  matrix.drawPixel(5, 2, c);
  matrix.drawPixel(5, 5, c);
  matrix.drawPixel(6, 2, c);
  matrix.drawPixel(6, 5, c);
}
void letterY(uint32_t c) {
  matrix.drawPixel(1, 1, c);
  matrix.drawPixel(1, 5, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 6, c);
  matrix.drawPixel(3, 2, c);
  matrix.drawPixel(3, 4, c);
  matrix.drawPixel(4, 4, c);
  matrix.drawPixel(5, 3, c);
  matrix.drawPixel(6, 4, c);
}
void letterZ(uint32_t c) {
  matrix.drawLine(1, 2, 1, 5, c);
  matrix.drawLine(6, 2, 6, 5, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(3, 3, c);
  matrix.drawPixel(4, 3, c);
  matrix.drawPixel(5, 5, c);
}



//animatia 5: de tip text preia sirul primit ca parametru si la fiecare secunda afiseaza pe matrice cate o litera
void textAnimation(uint32_t c, String text) {

  for(int i = 0; i < text.length(); i++){
     matrix.fillScreen(0);
     switch(text[i]){
      case 'a' : case 'A': letterA(c); break;
      case 'b' : case 'B': letterB(c); break;
      case 'c' : case 'C': letterC(c); break;
      case 'd' : case 'D': letterD(c); break;
      case 'e' : case 'E': letterE(c); break;
      case 'f' : case 'F': letterF(c); break;
      case 'g' : case 'G': letterG(c); break;
      case 'h' : case 'H': letterH(c); break;
      case 'i' : case 'I': letterI(c); break;
      case 'j' : case 'J': letterJ(c); break;
      case 'k' : case 'K': letterK(c); break;
      case 'l' : case 'L': letterL(c); break;
      case 'm' : case 'M': letterM(c); break;
      case 'n' : case 'N': letterN(c); break;
      case 'o' : case 'O': letterO(c); break;
      case 'p' : case 'P': letterP(c); break;
      case 'q' : case 'Q': letterQ(c); break;
      case 'r' : case 'R': letterR(c); break;
      case 's' : case 'S': letterS(c); break;
      case 't' : case 'T': letterT(c); break;
      case 'u' : case 'U': letterU(c); break;
      case 'v' : case 'V': letterV(c); break;
      case 'w' : case 'W': letterW(c); break;
      case 'x' : case 'X': letterX(c); break;
      case 'y' : case 'Y': letterY(c); break;
      case 'z' : case 'Z': letterZ(c); break;
      case ' ': delay(1000); break;
     }
     matrix.show();
     delay(1000);
     
    
  }
}

//animatia 6: de tip puncte genereaza aprinderea ledurilor intr-o culoare si la pozitie aleatoare
void dotsAnimation() {
  for(int j = 0; j < 8; j++)
  for(int i = 0 ; i < 30 ; i++){
      int randX = random(0, 8);
      int randY = random(0, 8);
      int randR1 = random(0, 255);
      int randG1 = random(0, 255);
      int randB1 = random(0, 255);
      matrix.drawPixel(randX, randY, matrix.Color(randR1, randG1, randB1));
  }
}

//functii folosite la crearea animatiei de tip inima
void Heart1(uint32_t c) {
  matrix.fillRect(4, 3, 2, 2, c);
}

void Heart2(uint32_t c) {
  matrix.drawLine(3, 3, 3, 4, c);
  matrix.drawLine(4, 2, 4, 5, c);
  matrix.drawLine(5, 2, 5, 5, c);
  matrix.drawLine(6, 3, 6, 4, c);
}

void Heart3(uint32_t c) {
  matrix.drawLine(2, 1, 2, 2, c);
  matrix.drawLine(2, 5, 2, 6, c);
  matrix.drawLine(3, 0, 3, 7, c);
  matrix.drawLine(4, 0, 4, 7, c);
  matrix.drawLine(5, 1, 5, 6, c);
  matrix.drawLine(6, 2, 6, 5, c);
  matrix.drawLine(7, 3, 7, 4, c);
}

void Heart4(uint32_t c) {
  matrix.drawLine(1, 1, 1, 2, c);
  matrix.drawLine(1, 5, 1, 6, c);
  matrix.drawLine(2, 0, 2, 7, c);
  matrix.drawLine(3, 0, 3, 7, c);
  matrix.drawLine(4, 1, 4, 6, c);
  matrix.drawLine(5, 2, 5, 5, c);
  matrix.drawLine(6, 3, 6, 4, c);
}

void Heart5(uint32_t c) {
  matrix.drawLine(0, 1, 0, 2, c);
  matrix.drawLine(0, 5, 0, 6, c);
  matrix.drawLine(1, 0, 1, 7, c);
  matrix.drawLine(2, 0, 2, 7, c);
  matrix.drawLine(3, 1, 3, 6, c);
  matrix.drawLine(4, 2, 4, 5, c);
  matrix.drawLine(5, 3, 5, 4, c);
}

//functii folosite la crearea animatiei de tip cana de cafea
void Coffee1(uint32_t c) {
  matrix.drawLine(3, 1, 3, 7, c);
  matrix.drawLine(4, 0, 5, 0, c);
  matrix.drawLine(3, 7, 5, 7, c);
  matrix.drawLine(7, 3, 7, 5, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 5, c);
  matrix.drawPixel(4, 6, c);
  matrix.drawPixel(5, 1, c);
}

void Coffee2(uint32_t c) {
  matrix.drawLine(3, 1, 3, 7, c);
  matrix.drawLine(4, 0, 5, 0, c);
  matrix.drawLine(3, 7, 5, 7, c);
  matrix.drawLine(7, 3, 7, 5, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 5, c);
  matrix.drawPixel(4, 6, c);
  matrix.drawPixel(5, 1, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
}
void Coffee3(uint32_t c) {
  matrix.drawLine(3, 1, 3, 7, c);
  matrix.drawLine(4, 0, 5, 0, c);
  matrix.drawLine(3, 7, 5, 7, c);
  matrix.drawLine(7, 3, 7, 5, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 5, c);
  matrix.drawPixel(4, 6, c);
  matrix.drawPixel(5, 1, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(1, 3, c);
  matrix.drawPixel(1, 6, c);
}

void Coffee4(uint32_t c) {
  matrix.drawLine(3, 1, 3, 7, c);
  matrix.drawLine(4, 0, 5, 0, c);
  matrix.drawLine(3, 7, 5, 7, c);
  matrix.drawLine(7, 3, 7, 5, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 5, c);
  matrix.drawPixel(4, 6, c);
  matrix.drawPixel(5, 1, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(1, 3, c);
  matrix.drawPixel(1, 6, c);
  matrix.drawPixel(0, 2, c);
  matrix.drawPixel(0, 5, c);
}

//functii folosite la crearea animatiei de tip expresie faciala
void Face1(uint32_t c) {
  matrix.drawLine(0, 2, 0, 5, c);
  matrix.drawLine(7, 2, 7, 5, c);
  matrix.drawLine(2, 7, 3, 7, c);
  matrix.drawLine(2, 0, 3, 0, c);
  matrix.drawLine(4, 0, 5, 0, c);
  matrix.drawLine(5, 7, 4, 7, c);
  matrix.drawPixel(1, 1, c);
  matrix.drawPixel(1, 6, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 6, c);
}

void Face2(uint32_t c) {
  matrix.drawLine(0, 2, 0, 5, c);
  matrix.drawLine(7, 2, 7, 5, c);
  matrix.drawLine(2, 7, 3, 7, c);
  matrix.drawLine(2, 0, 3, 0, c);
  matrix.drawLine(4, 0, 5, 0, c);
  matrix.drawLine(5, 7, 4, 7, c);
  matrix.drawPixel(1, 1, c);
  matrix.drawPixel(1, 6, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 6, c);
  matrix.drawLine(5, 3, 5, 4, c);
}

void Face3(uint32_t c) {
  matrix.drawLine(0, 2, 0, 5, c);
  matrix.drawLine(7, 2, 7, 5, c);
  matrix.drawLine(2, 7, 3, 7, c);
  matrix.drawLine(2, 0, 3, 0, c);
  matrix.drawLine(4, 0, 5, 0, c);
  matrix.drawLine(5, 7, 4, 7, c);
  matrix.drawPixel(1, 1, c);
  matrix.drawPixel(1, 6, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 6, c);
  matrix.drawLine(5, 3, 5, 4, c);
  matrix.drawPixel(4, 2, c);
  matrix.drawPixel(4, 5, c);
}

void Face4(uint32_t c) {
  matrix.drawLine(0, 2, 0, 5, c);
  matrix.drawLine(7, 2, 7, 5, c);
  matrix.drawLine(2, 7, 3, 7, c);
  matrix.drawLine(2, 0, 3, 0, c);
  matrix.drawLine(4, 0, 5, 0, c);
  matrix.drawLine(5, 7, 4, 7, c);
  matrix.drawPixel(1, 1, c);
  matrix.drawPixel(1, 6, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 6, c);
  matrix.drawLine(5, 3, 5, 4, c);
  matrix.drawPixel(4, 2, c);
  matrix.drawPixel(4, 5, c);
  matrix.drawPixel(5, 2, c);
  matrix.drawPixel(5, 5, c);
}

void Face5(uint32_t c) {
  matrix.drawLine(0, 2, 0, 5, c);
  matrix.drawLine(7, 2, 7, 5, c);
  matrix.drawLine(2, 7, 3, 7, c);
  matrix.drawLine(2, 0, 3, 0, c);
  matrix.drawLine(4, 0, 5, 0, c);
  matrix.drawLine(5, 7, 4, 7, c);
  matrix.drawPixel(1, 1, c);
  matrix.drawPixel(1, 6, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 6, c);
  matrix.drawLine(5, 3, 5, 4, c);
  matrix.drawPixel(3, 1, c);
  matrix.drawPixel(3, 6, c);
}

void Face6(uint32_t c) {
  matrix.drawLine(0, 2, 0, 5, c);
  matrix.drawLine(7, 2, 7, 5, c);
  matrix.drawLine(2, 7, 3, 7, c);
  matrix.drawLine(2, 0, 3, 0, c);
  matrix.drawLine(4, 0, 5, 0, c);
  matrix.drawLine(5, 7, 4, 7, c);
  matrix.drawPixel(1, 1, c);
  matrix.drawPixel(1, 6, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 6, c);
  matrix.drawLine(5, 3, 5, 4, c);
  matrix.drawPixel(3, 1, c);
  matrix.drawPixel(3, 6, c);
  matrix.drawPixel(4, 1, c);
  matrix.drawPixel(4, 6, c);
}
//functii folosite la crearea animatiei de tip fulgi de zapada
void Snowflake1(uint32_t c) {
  matrix.drawPixel(1, 0, c);
  matrix.drawPixel(1, 3, c);
  matrix.drawPixel(1, 6, c);
  matrix.drawPixel(2, 3, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawLine(3, 1, 3, 2, c);
  matrix.drawLine(3, 4, 3, 5, c);
  matrix.drawPixel(4, 1, c);
  matrix.drawPixel(4, 4, c);
  matrix.drawPixel(4, 7, c);
  matrix.drawLine(5, 1, 5, 2, c);
  matrix.drawLine(5, 4, 5, 5, c);
  matrix.drawPixel(6, 3, c);
  matrix.drawPixel(6, 5, c);
  matrix.drawPixel(7, 0, c);
  matrix.drawPixel(7, 3, c);
  matrix.drawPixel(7, 6, c);
}

void Snowflake2(uint32_t c) {
  matrix.drawLine(0, 2, 0, 3, c);
  matrix.drawLine(0, 5, 0, 6, c);
  matrix.drawLine(1, 0, 1, 1, c);
  matrix.drawLine(1, 5, 1, 6, c);
  matrix.drawLine(3, 1, 3, 2, c);
  matrix.drawLine(3, 4, 3, 5, c);
  matrix.drawLine(4, 2, 4, 3, c);
  matrix.drawLine(4, 5, 4, 6, c);
  matrix.drawLine(6, 1, 6, 2, c);
  matrix.drawLine(6, 6, 6, 7, c);
  matrix.drawLine(7, 1, 7, 2, c);
  matrix.drawLine(7, 4, 7, 5, c);
  matrix.drawPixel(1, 3, c);
  matrix.drawPixel(2, 1, c);
  matrix.drawPixel(2, 4, c);
  matrix.drawPixel(2, 7, c);
  matrix.drawPixel(6, 6, c);
  matrix.drawPixel(5, 0, c);
  matrix.drawPixel(5, 3, c);
  matrix.drawPixel(5, 6, c);
  matrix.drawPixel(6, 4, c);
  matrix.drawPixel(2, 3, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(5, 4, c);
  matrix.drawPixel(5, 2, c);
}

void Snowflake3(uint32_t c) {
  matrix.drawLine(3, 2, 3, 4, c);
  matrix.drawLine(4, 2, 4, 3, c);
  matrix.drawLine(4, 5, 4, 6, c);
  matrix.drawLine(5, 2, 5, 4, c);
  matrix.drawPixel(1, 2, c);
  matrix.drawPixel(1, 4, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 4, c);
  matrix.drawPixel(2, 6, c);
  matrix.drawPixel(3, 0, c);
  matrix.drawPixel(3, 6, c);
  matrix.drawPixel(5, 0, c);
  matrix.drawPixel(5, 6, c);
  matrix.drawPixel(6, 2, c);
  matrix.drawPixel(6, 4, c);
  matrix.drawPixel(6, 6, c);
  matrix.drawPixel(7, 2, c);
  matrix.drawPixel(7, 4, c);
}
void Snowflake4(uint32_t c) {
  matrix.drawPixel(1, 0, c);
  matrix.drawPixel(1, 3, c);
  matrix.drawPixel(1, 6, c);
  matrix.drawLine(2, 3, 2, 5, c);
  matrix.drawPixel(3, 1, c);
  matrix.drawPixel(3, 3, c);
  matrix.drawPixel(3, 5, c);
  matrix.drawLine(4, 1, 4, 3, c);
  matrix.drawLine(4, 5, 4, 7, c);
  matrix.drawPixel(5, 1, c);
  matrix.drawPixel(5, 3, c);
  matrix.drawPixel(5, 5, c);
  matrix.drawLine(6, 3, 6, 5, c);
  matrix.drawPixel(7, 0, c);
  matrix.drawPixel(7, 3, c);
  matrix.drawPixel(7, 6, c);
}
void Snowflake5(uint32_t c) {
  matrix.fillRect(0, 3, 2, 2, c);
  matrix.fillRect(3, 3, 2, 2, c);
  matrix.fillRect(6, 3, 2, 2, c);
  matrix.drawLine(3, 0, 3, 1, c);
  matrix.drawLine(3, 6, 3, 7, c);
  matrix.drawLine(4, 0, 4, 1, c);
  matrix.drawLine(4, 6, 4, 7, c);
  matrix.drawPixel(1, 1, c);
  matrix.drawPixel(1, 6, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(5, 5, c);
  matrix.drawPixel(5, 2, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 6, c);
}

void Snowflake6(uint32_t c) {
  matrix.drawLine(2, 2, 2, 3, c);
  matrix.drawLine(2, 5, 2, 6, c);
  matrix.drawLine(4, 3, 4, 5, c);
  matrix.drawLine(6, 2, 6, 3, c);
  matrix.drawLine(6, 5, 6, 6, c);
  matrix.drawPixel(1, 3, c);
  matrix.drawPixel(3, 1, c);
  matrix.drawPixel(3, 3, c);
  matrix.drawPixel(3, 5, c);
  matrix.drawPixel(4, 1, c);
  matrix.drawPixel(4, 7, c);
  matrix.drawPixel(5, 1, c);
  matrix.drawPixel(5, 3, c);
  matrix.drawPixel(5, 5, c);
  matrix.drawPixel(7, 3, c);
}

//animatia 1: la fiecare jumatate de secunda se afiseaza cate o etapa a animatiei
void heartAnimation(uint32_t c) {
  matrix.fillScreen(0);
  Heart1(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Heart2(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Heart3(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Heart4(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Heart5(c);
  matrix.show();
  delay(500);
}

//animatia 2: la fiecare jumatate de secunda se afiseaza cate o etapa a animatiei
void coffeeAnimation(uint32_t c) {
  matrix.fillScreen(0);
  Coffee1(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Coffee2(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Coffee3(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Coffee4(c);
  matrix.show();
  delay(500);
}

//poate fi folosita la afisarea unei fete care incepe sa zambeasca
void happyAnimation(uint32_t c) {
  matrix.fillScreen(0);
  Face1(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face2(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face3(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face4(c);
  matrix.show();
  delay(500);
}
//poate fi folosita la afisarea unei fete care plange
void sadAnimation(uint32_t c) {
  matrix.fillScreen(0);
  Face1(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face2(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face5(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face6(c);
  matrix.show();
  delay(500);
}


//animatia 3: la fiecare jumatate de secunda se afiseaza cate o etapa a animatiei
void emotionsAnimation(uint32_t c) {
  matrix.fillScreen(0);
  Face4(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face3(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face2(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face1(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face2(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face5(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face6(c);
  matrix.show();
  delay(500);
  Face5(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face2(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face1(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face2(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face3(c);
  matrix.show();
  delay(500);
}

//animatia 4: la fiecare jumatate de secunda se afiseaza cate o etapa a animatiei
void snowflakesAnimation(uint32_t c) {
  matrix.fillScreen(0);
  Snowflake1(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Snowflake2(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Snowflake3(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Snowflake4(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Snowflake5(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Snowflake6(c);
  matrix.show();
  delay(500);
}
