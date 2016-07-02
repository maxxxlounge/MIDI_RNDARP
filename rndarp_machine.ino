#include <LiquidCrystal.h>


/**
  DEFINISCO LE COSTANTI PER LE LETTURE DEGLI INPUT
*/
const int PIN_VAL = 7; 
const int PIN_MODE = 10;
const int PIN_INC = 9;     
const int PIN_DEC = 8;


//FUNZIONI DI MENU
int sensorMin = 0;
int sensorMax = 1024;
int battute = 8;
int range = 1;
int octave = 5;
int tonalita = 1; //1=C, 2=C#, 12=B etc...
int tempo = 120;
int tones[] = {60,60,60,60,60,60,60,60};
int midi_ch=1;

int currentMenu = 0;

int maggiore[] = {3,5,6,8,10,12,13};
int minore[] = {3,4,6,8,9,11,13};
/*
int dorico[] = {2,1,2,2,2,1,2};
int frigio[] = {1,2,2,2,1,2,2};
int lidio[] = {2,2,2,1,2,2,1};
int misolidio[] = {2,2,1,2,2,1,2};

int locrio[] = {1,2,2,1,2,2,2};
int pentatonic[] = {1,2,3,2,2,3,1};
*/

float bps = tempo/60;
float tickDuration = 1/bps;
int del_tick = tickDuration*1000;
int arp_tick = del_tick / (battute/4);

int menuReading;
int changeReading;


boolean seed_btn;
boolean tempo_inc;
boolean tempo_dec;   
String menuLabel;

 LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
/*
void noteOn(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}
*/
/*
void scala(int initNote,int modo[], int range)
{
  int modoRange[]={};
  /*
    //octave 3
    //initNote = C
    //60
    //modo maggiore [2,2,1,2,2,2,1]
    //int modoRange[4];
    //modoRange = modo;
    //for (r=1;r<sizeOf(range);r++){
    //  for (n=0;n<sizeOf(modo);n++){
    //    modoRange[r*7+n] = modo[n];
    //  }
 

  ret[0] = initNote;
  int i;
  for (i = 0; i < 8; i=i+1){
     ret[i+1] = initNote+modoRange[i];
  } 
}
*/

//Restituisce una rray di battute Randomizza gli array nelle battute (8)
/*void randomize(int rnd_min, int rnd_max){
  for(int i=0;i<battute;i++){
    int rnd = random(rnd_max-rnd_min);
    rndArray[i] = rnd + rnd_min;
  }
}*/

void setup() {
  lcd.begin(16, 2);
  lcd.print("RNDARP MACHINE");
  Serial.begin(9600);
  pinMode(PIN_INC,INPUT_PULLUP);
  pinMode(PIN_DEC,INPUT_PULLUP);
  pinMode(PIN_MODE,INPUT_PULLUP);
  pinMode(PIN_VAL,INPUT_PULLUP);  
  currentMenu=0;
  //Serial.begin(31250);
}

///play the rnd arp
/*void playSound(){
  for (int i=0;i<8;i=i+1){
    String space = " ";
    Serial.println(tones[0] + space + velocity[i] + space + midi_ch);
  }
}*/

void updateTempo(){
  bps = tempo/60;
  tickDuration = 1/bps;
  del_tick = tickDuration*1000;
  //se 8 del_tick = 4 battute; 8/4=2
  arp_tick = del_tick / (battute/4);
} 

void setMenuLabel(String label){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(label);
//      lcd.setCursor(0,1);
    }


void inputReading(){
    menuReading = digitalRead(PIN_MODE);
    changeReading = digitalRead(PIN_VAL);
    if (menuReading==0){
      if (currentMenu>=6){
        currentMenu=0;
      }else{
        currentMenu+=1;
      } 

      switch (currentMenu) {
        case 0:    // VELOCITY
          menuLabel = "VELOCITY";
          break;
        case 1:    // TONE
          menuLabel = "TONE";
          break;
        case 2:    // SCALE
          menuLabel = "SCALE";
          break;  
        case 3: //RANGE
          menuLabel = "RANGE";
          break;
        case 4: //OCTAVE
          menuLabel = "OCTAVE";
          break;
        case 5: //TEMPO
          menuLabel = "TEMPO";
          break;
      }    
      setMenuLabel(menuLabel);
      delay(300);
    
  }
}

void seed(){
  for(int i=0;i<battute;i++){
    //60 = 5 * 12
    int minNote = tonalita + (12*octave);
    int maxNote = tonalita + (12*(octave+range));
    tones[i] = random(minNote,maxNote);
  }
}


void loop() {
  inputReading();
  switch (currentMenu) {
    case 0:    // VELOCITY
      delay(300);
      break;
    case 1:    // TONE
      delay(300);    
      break;
    case 2:    // SCALE
      delay(300);    
      break;  
    case 3: //RANGE
      lcd.setCursor(0,1);
      lcd.print(String(range));
    
      delay(300);
      break;
    case 4: //OCTAVE
      lcd.setCursor(0,1);
      lcd.print(String(octave));
      delay(300);
      break;
    case 5: //TEMPO
      tempo_inc = digitalRead(PIN_INC);
      tempo_dec = digitalRead(PIN_DEC);    
      if (tempo_inc==0){
        tempo += 1;
      }
      if (tempo_dec==0){
        tempo -= 1;
      }
      if (tempo_dec==0 || tempo_inc==0){
        updateTempo();
      }
      lcd.setCursor(0,1);
      lcd.print(String(tempo) + " BPM ");
      delay(100);
      break;
    case 6:    // PLAY
      for (int b=0;b<battute;b++){
        //inputReading();
        //noteOn(0x90, tones[b], velocity[b]);
        Serial.print(String(tones[b],HEX)+"|" );
        delay(arp_tick);
        //noteOn(0x90,tones[b], 0x00);
      }
      break;    
  }
  
}
