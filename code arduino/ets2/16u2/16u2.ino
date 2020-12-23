#include <HID-Project.h>

byte buf[2]; // buffer de lecture de la liaison série

char touche;

// Valeur des positions des Analogiques
// Analog A0 Essuie glace AVANT
const uint8_t stateEGHORI = 0; // state buffer
// Analog A1 Lave vitre + Essuie glace AR
const uint8_t stateEGVERTI = 1; // state buffer
// Analog A2 // Régulateur vitesse
const uint8_t stateEGBUTON = 2; // state buffer
// Analog A3 // Sensibilité Essuie Glace
const uint8_t stateMOLETTE_EG = 3; // state buffer
// Analog A4 // Cligno
const uint8_t stateCGVERTI = 4; // state buffer
//Analog A5// Clignox1
const uint_t stateCLIGNOX1 = 5; // state buffer
// Analog A6 // pleins phare
const uint8_t stateCGHORI = 6; // state buffer
// Analog A7 // Appel phare
const uint8_t stateCGHORI2 = 7; // state buffer
// Analog A8 // Régulateur
const uint8_t stateCGbuton = 8; // state buffer
// Analog A9 // Régulateur Molette
const uint8_t stateCGMOLREG = 9; // state buffer
// Analog

void setup() {
  // démarrage lien série vers le mega
  Serial1.begin(115200);
  // Serial de debugage, pour le terminal
  //Serial.begin(115200);
  // Sends a clean report to the host. This is important on any Arduino type.
  Keyboard.begin();
}

void decodeBuffer(){
  touche = char(buf[1]);
  if( buf[0] == stateEGHORI){
    //Serial.println(F("Essuie glace AVANT"));
    //Serial.print(F("Touche clavier : "));
    //Serial.println(touche);
  }
  if( buf[0] == stateEGHORI){
    //Serial.println(F("Lave vitre + Essuie glace AR"));
    //Serial.print(F("Touche clavier : "));
    //Serial.println(touche);
  }
  if( buf[0] == stateEGHORI){
    //Serial.println(F("Régulateur vitesse"));
    //Serial.print(F("Touche clavier : "));
    //Serial.println(touche);
  }
  if( buf[0] == stateEGHORI){
    //Serial.println(F("Sensibilité Essuie Glace"));
    //Serial.print(F("Touche clavier : "));
    //Serial.println(touche);
  }
  Keyboard.write(touche);
}

void loop() {
  if (Serial1.available()) {
    Serial1.readBytes(buf,2);
    decodeBuffer();
  }
}
