/*
  Copyright (c) 2014-2015 NicoHood
  See the readme for credit to other people.

  Improved Serial example

  Shows how to use the new Serial API.

  See HID Project documentation for more information.
  https://github.com/NicoHood/HID/wiki/Serial-API#improved-Serial
*/

// Valeur des positions des Analogiques
const int EGVERTI = A0;    // Analog A0 Essuie glace AVANT
const uint8_t stateEGVERTI = 0; // state buffer

const int EGX1 = 428;   // Essuie glace AV x1
const int EGSTOP = 19;  // Stop
const int EGV1 = 193;   // Lent
const int EGV2 = 102;   // Moyen
const int EGV3 = 53;    // Vite

const uint8_t EGHORI = A1;  // Analog A1 Lave vitre + Essuie glace AR
const uint8_t stateEGHORI = 1; // state buffer

const int EGAVX1 = 284; // Lave vitre AV
const int EGARSTOP = 20;// Stop
const int EGARV1 = 123; // Essuie glace AR (V1)
const int ERARX1 = 59;  // Essuie glace AR + Lave Vitre AR (V2)

const uint8_t EGBUTON = A2;   // Analog A2 // Régulateur vitesse
const uint8_t stateEGBUTON = 2; // state buffer

const int RALPLUS = 1;  // Trip +
const int RALMOINS = 1; // Trip -
const int OKRESET = 1;  // Reset/OK

const uint8_t MOLETTE_EG = A3;   // Analog A3 // Sensibilité Essuie Glace
const uint8_t stateMOLETTE_EG = 3; // state buffer

const int EG1 = 1;      // V1 Sensi
const int EG2 = 1;      // V2 Sensi
const int EG3 = 1;      // V3 Sensi
const int EG4 = 1;      // V4 Sensi

const uint8_t CGVERTI = A4; // Analog A4 // Cligno
const uint8_t stateCGVERTI =4; // state buffer

const int CGD = 1; // cligno droit
const int CGDX1 = 1; // cligno droit x1
const int CGSTOP = 1; // cligno stop
const int CGGX1 = 1; // cligno gauche x1
const int CGG = 1; // cligno gauche

const uint8_t CGHORI = A5; // Analog 5 // pleins phare
const uint8_t stateCGHORI = 5; // state buffer

const int CGPP =1; // pleins phare
const int CGFSTOP = 1; // pleins phare stop
const int CGAPF = 1; // appel de phare

const uint8_t CGBUTON = A6; // Analog 6 // Régulateur
const uint8_t stateCGbuton = 6; // state buffer

const int REGUPLUS =1; // Régulateur +
const int REGUMOINS =1; // Régulateur -
const int REGUON =1; // Régulateur on
const int REGUCANCEL =1; // régulateur cancel
const int REGUOFF =1; // R&égulateur off

const uint8_t tolerence_lecture = 10; // Plage de tolérence pour considéré la valeur lue comme bonne pour une position

int8_t stateCligno = 0; // Cligno neutre 0 ; Cligno gauche -1 ; Cligno Droite 1
int8_t origCligno = 0;  // Variable de comparaison etat cligno
int8_t stateEGAV = 0;   // Essuie glace avant stop 0; x1 -1; V1; 1; V2 2; V3 3
int8_t origEGAV = 0;    // Variable de comparaison etat EGAV
int8_t stateEGAR = 0;   // Essuie glace arrière stop 0; x1 -1; V1; 1; V2 2;
int8_t origEGAR = 0;    // Variable de comparaison etat EGAR
int8_t stateSEG = 0;    // Sensibilité capteur essuie glace 1 à 4
int8_t origSEG = 0;     // Variable de comparaison etat SEG
int8_t statePF = 0;     // Pleins phare
int8_t origPF = 0;      // Variable de comparaison Pleins Phare
int8_t stateREGU = 0;   // Régulateur
int8_t origREGU = 0;    // Variable de comparaison Régulateur


// Déclaration des touches clavier (QWERTY) correspondant aux positions
// Essuie Glace AV
const char C_EGX1 =     'a';
const char C_EGSTOP =   'z';
const char C_EGV1 =     'e';
const char C_EGV2 =     'r';
const char C_EGV3 =     't';
// Esssuie Glace AR
const char C_EGAVX1 =   'y';
const char C_EGARSTOP = 'u';
const char C_EGARV1 =   'i';
const char C_ERARX1 =   'o';
// Ralentisseur
const char C_RALPLUS =  ' ';
const char C_RALMOINS = ' ';
const char C_OKRESET =  ' ';
//Sensibité essuie glace
const char C_EG1 =      ' ';
const char C_EG2 =      ' ';
const char C_EG3 =      ' ';
const char C_EG4 =      ' ';
// Cligno
const char C_CGD =      ' ';
const char C_CGG =      ' ';
const char C_CGSTOP =   ' ';
const char C_CGDX1 =    ' ';
const char C_CGGX1 =    ' ';
// Pleins phare
const char C_CGPP =      ' ';
const char C_CGFSTOP =   ' ';
const char C_CGAPF =      ' ';
// Régulateur
const char C_RPLUS =    ' ';
const char C_RMOINS =   ' ';
const char C_RON =      ' ';
const char C_RCANCEL =  ' '; 
const char C_RSTOP =     ' ';


//buffer serie
byte buf[2]={0, 0};

void setup() {
  //Lecture au démarage des positions des commodos
  if( analogRead(EGHORI) >= EGSTOP-tolerence_lecture && analogRead(EGHORI) <= EGSTOP+tolerence_lecture ){ // EGAV neutre
    origEGAV = 0;
  }
  if( analogRead(EGHORI) >= EGX1-tolerence_lecture && analogRead(EGHORI) <= EGX1+tolerence_lecture ){ // EGAV x1
    origEGAV = -1;
  }
  if( analogRead(EGHORI) >= EGV1-tolerence_lecture && analogRead(EGHORI) <= EGV1+tolerence_lecture ){ // EGAV V1
    origEGAV = 1;
  }
  if( analogRead(EGHORI) >= EGV2-tolerence_lecture && analogRead(EGHORI) <= EGV2+tolerence_lecture ){ // EGAV V2
    origEGAV = 2;
  }
  if( analogRead(EGHORI) >= EGV3-tolerence_lecture && analogRead(EGHORI) <= EGV3+tolerence_lecture ){ // EGAV V3
    origEGAV = 3;
  }
  if( analogRead(EGVERTI) >= EGARSTOP-tolerence_lecture && analogRead(EGVERTI) <= EGARSTOP+tolerence_lecture ){ // EGAR neutre
    origEGAR = 0;
  }
  if( analogRead(EGVERTI) >= EGAVX1-tolerence_lecture && analogRead(EGVERTI) <= EGAVX1+tolerence_lecture ){ // EGAR x1
    origEGAR = -1;
  }
  if( analogRead(EGVERTI) >= EGARV1-tolerence_lecture && analogRead(EGVERTI) <= EGARV1+tolerence_lecture ){ // EGAR V1
    origEGAR = 1;
  }
  if( analogRead(EGVERTI) >= ERARX1-tolerence_lecture && analogRead(EGVERTI) <= ERARX1+tolerence_lecture ){ // EGAR V2
    origEGAR = 2;
  }
  if( analogRead(MOLETTE_EG) >= EG1-tolerence_lecture && analogRead(MOLETTE_EG) <= EG1+tolerence_lecture ){ // EG1
    origSEG = 1;
  }
  if( analogRead(MOLETTE_EG) >= EG2-tolerence_lecture && analogRead(MOLETTE_EG) <= EG2+tolerence_lecture ){ // EG2
    origSEG = 2;
  }
  if( analogRead(MOLETTE_EG) >= EG3-tolerence_lecture && analogRead(MOLETTE_EG) <= EG3+tolerence_lecture ){ // EG3
    origSEG = 3;
  }
  if( analogRead(MOLETTE_EG) >= EG4-tolerence_lecture && analogRead(MOLETTE_EG) <= EG4+tolerence_lecture ){ // EG4
    origSEG = 4;
  }
  if( analogRead(CGVERTI) >= CGD-tolerence_lecture && analogRead(CGVERTI) <= CGD+tolerence_lecture ){ // CGD
    origCligno = 1;
  }
  if( analogRead(CGVERTI) >= CGDX1-tolerence_lecture && analogRead(CGVERTI) <= CGDX1+tolerence_lecture ){ // CGD
    origCligno = 2;
  }
  if( analogRead(CGVERTI) >= CGSTOP-tolerence_lecture && analogRead(CGVERTI) <= CGSTOP+tolerence_lecture ){ // CGD
    origCligno = 3;
  }
  if( analogRead(CGVERTI) >= CGGX1-tolerence_lecture && analogRead(CGVERTI) <= CGGX1+tolerence_lecture ){ // CGD
    origCligno = 4;
  }
  if( analogRead(CGVERTI) >= CGG-tolerence_lecture && analogRead(CGVERTI) <= CGG+tolerence_lecture ){ // CGD
    origCligno = 5;
  }
  if( analogRead(CGHORI) >= CGPP-tolerence_lecture && analogRead(CGHORI) <= CGPP+tolerence_lecture ){ // CPP
    origPF = 0;
  }
  if( analogRead(CGHORI) >= CGFSTOP-tolerence_lecture && analogRead(CGHORI) <= CGFSTOP+tolerence_lecture ){ // CGPSTOP
    origPF = 1;
  }
  if( analogRead(CGHORI) >= CGAPF-tolerence_lecture && analogRead(CGHORI) <= CGAPF+tolerence_lecture ){ // CGAPF
    origPF = 2;
  }
  if( analogRead(CGBUTON) >= REGUON-tolerence_lecture && analogRead(CGBUTON) <= REGUON+tolerence_lecture ){ // REGUON
    origREGU = 0;
  }
  if( analogRead(CGBUTON) >= REGUCANCEL-tolerence_lecture && analogRead(CGBUTON) <= REGUCANCEL+tolerence_lecture ){ // REGUCANCEL
    origREGU = 1;
  }
  if( analogRead(CGBUTON) >= REGUOFF-tolerence_lecture && analogRead(CGBUTON) <= REGUOFF+tolerence_lecture ){ // REGUOFF
    origREGU = 2;
  }
  // initialisation interface serie => 16u2
  while (!Serial);
  Serial.begin(115200);
}

void sendComEGAV(){ // fonction d'envoie des touches clavier pour les essuie glace AV
  buf[0] = stateEGVERTI;
  if (stateEGAV == -1){
    buf[1] = C_EGX1;
  } else if (stateEGAV == 0){
    buf[1] = C_EGSTOP;
  } else if (stateEGAV == 1){
    buf[1] = C_EGV1;
  } else if (stateEGAV == 2){
    buf[1] = C_EGV2;
  } else if (stateEGAV == 3){
    buf[1] = C_EGV3;
  }
  Serial.write(buf, 2);
}

void readEssuieGlaceAV() { // Lecture de la valeur commodo essuie glace AV
  if( analogRead(EGHORI) >= EGSTOP-tolerence_lecture && analogRead(EGHORI) <= EGSTOP+tolerence_lecture ){ // EGAV neutre
    stateEGAV = 0;
  }
  if( analogRead(EGHORI) >= EGX1-tolerence_lecture && analogRead(EGHORI) <= EGX1+tolerence_lecture ){ // EGAV x1
    stateEGAV = -1;
  }
  if( analogRead(EGHORI) >= EGV1-tolerence_lecture && analogRead(EGHORI) <= EGV1+tolerence_lecture ){ // EGAV V1
    stateEGAV = 1;
  }
  if( analogRead(EGHORI) >= EGV2-tolerence_lecture && analogRead(EGHORI) <= EGV2+tolerence_lecture ){ // EGAV V2
    stateEGAV = 2;
  }
  if( analogRead(EGHORI) >= EGV3-tolerence_lecture && analogRead(EGHORI) <= EGV3+tolerence_lecture ){ // EGAV V3
    stateEGAV = 3;
  }
  if(origEGAV != stateEGAV){
    origEGAV = stateEGAV;
    sendComEGAV();
  }
}

void sendComEGAR(){ // fonction d'envoie des touches clavier pour les essuie glace AR
  buf[0] = stateEGHORI;
  if (stateEGAR == -1){
    buf[1] = C_EGAVX1;
  } else if (stateEGAR == 0){
    buf[1] = C_EGARSTOP;
  } else if (stateEGAR == 1){
    buf[1] = C_EGARV1;
  } else if (stateEGAR == 2){
    buf[1] = C_ERARX1;
  }
  Serial.write(buf, 2);
}

void readEssuieGlaceAR() { // Lecture de la valeur commodo essuie glace AR
  if( analogRead(EGVERTI) >= EGARSTOP-tolerence_lecture && analogRead(EGVERTI) <= EGARSTOP+tolerence_lecture ){ // EGAR neutre
    stateEGAR = 0;
  }
  if( analogRead(EGVERTI) >= EGAVX1-tolerence_lecture && analogRead(EGVERTI) <= EGAVX1+tolerence_lecture ){ // EGAR x1
    stateEGAR = -1;
  }
  if( analogRead(EGVERTI) >= EGARV1-tolerence_lecture && analogRead(EGVERTI) <= EGARV1+tolerence_lecture ){ // EGAR V1
    stateEGAR = 1;
  }
  if( analogRead(EGVERTI) >= ERARX1-tolerence_lecture && analogRead(EGVERTI) <= ERARX1+tolerence_lecture ){ // EGAR V2
    stateEGAR = 2;
  }
  if(origEGAR != stateEGAR){
    origEGAR = stateEGAR;
    sendComEGAR();
  }
}

void sendComRal(uint8_t stateRal){ // envoie instruction ralentisseur
  buf[0] = stateEGBUTON;
  if(stateRal == 0){
    buf[1] = C_RALPLUS;
  } else if (stateRal == 1){
    buf[1] = C_RALMOINS;
  } else if (stateRal == 2){
    buf[1] = C_OKRESET;
  }
  Serial.write(buf, 2);
}

void readRalentisseur(){ // fonction de lecteur des bouton TRP+/- et OK/RESET
  if( analogRead(EGBUTON) >= RALPLUS-tolerence_lecture && analogRead(EGBUTON) <= RALPLUS+tolerence_lecture ){ // Trip+
    sendComRal(0);
  }
  if( analogRead(EGBUTON) >= RALMOINS-tolerence_lecture && analogRead(EGBUTON) <= RALMOINS+tolerence_lecture ){ // Trip-
    sendComRal(1);
  }
  if( analogRead(EGBUTON) >= OKRESET-tolerence_lecture && analogRead(EGBUTON) <= OKRESET+tolerence_lecture ){ // OK/RESET
    sendComRal(2);
  }
}

void sendComSEG(){ // fonction d'envoie sensi essuie glace
  buf[0] = stateEGHORI;
  if (stateSEG == -1){
    buf[1] = C_EGAVX1;
  } else if (stateEGAR == 0){
    buf[1] = C_EGARSTOP;
  } else if (stateEGAR == 1){
    buf[1] = C_EGARV1;
  } else if (stateEGAR == 2){
    buf[1] = C_ERARX1;
  }
  Serial.write(buf, 2);
}

void readSEG() { // Lecture de la sensibilité capteur essuie glace
  if( analogRead(MOLETTE_EG) >= EG1-tolerence_lecture && analogRead(MOLETTE_EG) <= EG1+tolerence_lecture ){ // EG1
    stateSEG = 1;
  }
  if( analogRead(MOLETTE_EG) >= EG2-tolerence_lecture && analogRead(MOLETTE_EG) <= EG2+tolerence_lecture ){ // EG2
    stateSEG = 2;
  }
  if( analogRead(MOLETTE_EG) >= EG3-tolerence_lecture && analogRead(MOLETTE_EG) <= EG3+tolerence_lecture ){ // EG3
    stateSEG = 3;
  }
  if( analogRead(MOLETTE_EG) >= EG4-tolerence_lecture && analogRead(MOLETTE_EG) <= EG4+tolerence_lecture ){ // EG4
    stateSEG = 4;
  }
  if(origSEG != stateSEG){
    origSEG = stateSEG;
    sendComSEG();
  }
}

void sendComCligno(){ // fonction d'envoie des touches clavier pour les Cligno
  buf[0] = stateCligno;
  if (stateCligno == 1){
    buf[1] = C_CGG;
  } else if (stateCligno == 2){
    buf[1] = C_CGGX1;
  } else if (stateCligno == 3){
    buf[1] = C_CGSTOP;
  } else if (stateCligno == 4){
    buf[1] = C_CGGX1;
  } else if (stateCligno == 5){
    buf[1] = C_CGG;
  }
  Serial.write(buf, 2);
}

void readCG() { // Lecture du Cligno
  if( analogRead(CGVERTI) >= CGD-tolerence_lecture && analogRead(CGVERTI) <= CGD+tolerence_lecture ){ // CGD
    stateSEG = 1;
  }
  if( analogRead(CGVERTI) >= CGDX1-tolerence_lecture && analogRead(CGVERTI) <= CGDX1+tolerence_lecture ){ // CGDX1
    stateSEG = 2;
  }
  if( analogRead(CGVERTI) >= CGSTOP-tolerence_lecture && analogRead(CGVERTI) <= CGSTOP+tolerence_lecture ){ // CGSTOP
    stateSEG = 3;
  }
  if( analogRead(CGVERTI) >= CGGX1-tolerence_lecture && analogRead(CGVERTI) <= CGGX1+tolerence_lecture ){ // CGGX1
    stateSEG = 4;
  }
  if( analogRead(CGVERTI) >= CGG-tolerence_lecture && analogRead(CGVERTI) <= CGG+tolerence_lecture ){ // CGG
    stateSEG = 5;
  }
  if(origCligno != stateCligno){
    origCligno = stateCligno;
    sendComCligno();
  }
}

void sendComPF(){ // fonction d'envoie des touches clavier pour les pleins phare
  buf[0] = statePF;
  if (statePF == 1){
    buf[1] = C_CGPP;
  } else if (statePF == 2){
    buf[1] = C_CGFSTOP;
  } else if (statePF == 3){
    buf[1] = C_CGAPF;
  }
  Serial.write(buf, 2);
}

void readPF() { // Lecture des pleins phare
  if( analogRead(CGHORI) >= CGPP-tolerence_lecture && analogRead(CGHORI) <= CGPP+tolerence_lecture ){ // CGPP
    stateSEG = 1;
  }
  if( analogRead(CGHORI) >= CGFSTOP-tolerence_lecture && analogRead(CGHORI) <= CGFSTOP+tolerence_lecture ){ // CGFSTOP
    stateSEG = 2;
  }
  if( analogRead(CGHORI) >= CGAPF-tolerence_lecture && analogRead(CGHORI) <= CGAPF  +tolerence_lecture ){ // CGAPF
    stateSEG = 3;
  }
  if(origPF != statePF){
    origPF = statePF;
    sendComPF();
  }
}

void sendComREGU(){ // fonction d'envoie des touches clavier pour le régulateur
  if (stateREGU == 1){
    buf[1] = C_RON;
  } else if (stateREGU == 2){
    buf[1] = C_RCANCEL;
  } else if (stateREGU == 3){
    buf[1] = C_RSTOP;
  }
  Serial.write(buf, 2);
}

void readREGU() { // Lecture molette du Régulateur
  if( analogRead(CGBUTON) >= REGUON-tolerence_lecture && analogRead(CGBUTON) <= REGUON+tolerence_lecture ){ // REGUON
    stateREGU = 1;
  }
  if( analogRead(CGBUTON) >= REGUCANCEL-tolerence_lecture && analogRead(CGBUTON) <= REGUCANCEL+tolerence_lecture ){ // REGUCANCEL
    stateREGU = 2;
  }
  if( analogRead(CGBUTON) >= REGUOFF-tolerence_lecture && analogRead(CGBUTON) <= REGUOFF+tolerence_lecture ){ // REGUOFF
    stateREGU = 3;
  }
  if(origREGU != stateREGU){
    origREGU = stateREGU;
    sendComREGU();
  }
}

void sendComREGUBUTON(uint8_t stateREGUBUTON){ // fonction d'envoie des touches clavier pour les boutons du régulateur
  buf[0] = stateREGUBUTON;
  if (stateREGUBUTON == 1){
    buf[1] = C_RPLUS;
  } else if (stateREGUBUTON == 2){
    buf[1] = C_RMOINS;
  }
}
  
void readREGUBUTON() { // lecture bouton du régulateur
  if( analogRead(CGBUTON) >= REGUPLUS-tolerence_lecture && analogRead(CGBUTON) <= REGUPLUS+tolerence_lecture ){ // REGUPLUS
    sendComREGUBUTON(0);
  }
  if( analogRead(CGBUTON) >= REGUMOINS-tolerence_lecture && analogRead(CGBUTON) <= REGUMOINS+tolerence_lecture ){ // REGUMOINS
    sendComREGUBUTON(1);
  }
}


void loop() {
  readEssuieGlaceAV();
  readEssuieGlaceAR();
  readRalentisseur();
  readSEG();
  readCG();
  readPF();
  readREGU();
  readREGUBUTON();
}
