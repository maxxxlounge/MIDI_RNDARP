#include <LiquidCrystal.h>


/**
  DEFINISCO LE COSTANTI PER LE LETTURE DEGLI INPUT
*/
const int PIN_VAL = 7; 
const int PIN_MODE = 10;
const int PIN_INC = 9;     
const int PIN_DEC = 8;

// VALORI PERCENTUALI
int velocity_rate = 80;

//FUNZIONI DI MENU
int sensorMin = 0;
int sensorMax = 1024;
int battute = 8;
int range = 1;
int octave = 5;
int note = 0; //1=C, 2=C#, 12=B etc...
String note_str[] = {"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};

int rnd_notes[] = {60,60,60,60,60,60,60,60};

int tempo = 120;
int tones[] = {60,60,60,60,60,60,60,60};
int midi_ch=1;

int currentMenu = 0;
int scale = 0;
String scale_str[]={"random","maggiore","minore","dorico","frigio","lidio","misolidio","locrio","pentatonic"};
int maggiore[] = {2,2,1,2,2,2,1};
int minore[] = {2,1,2,2,2,1,2};
int dorico[] = {2,1,2,2,2,1,2};
int frigio[] = {1,2,2,2,1,2,2};
int lidio[] = {2,2,2,1,2,2,1};
int misolidio[] = {2,2,1,2,2,1,2};
int locrio[] = {1,2,2,1,2,2,2};
int pentatonic[] = {1,2,3,2,2,3,1};

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

String noteToString(int note_local){
  return note_str[(note_local % 12)];
}

void setRndNotes(){
  int noteincr = note;
  rnd_notes[0] = noteincr;
  for (int i=0;i<7;i++){
          switch(scale){
            case 1:
                noteincr += maggiore[i];
                break;
            case 2:
                noteincr += minore[i];
                break;                
            case 3:
            	noteincr += dorico[i];
            	break;
            case 4:
            	noteincr += frigio[i];
            	break;
            case 5:
            	noteincr += lidio[i];
            	break;
            case 6:
            	noteincr += misolidio[i];
            	break;
            case 7:
            	noteincr += locrio[i];
            	break;
            case 8:
            	noteincr += pentatonic[i];
            	break;
          }
          rnd_notes[i+1] = noteincr;
    }
}


String notesOfCurrentScale(){
  String c_scale = "";
  int noteincr = note;
    
    rnd_notes[0] = noteincr;
    
    c_scale = noteToString(noteincr) + "|"; //noteToString(noteincr) + "|";
    for (int i=0;i<7;i++){
          switch(scale){
            case 1:
                noteincr += maggiore[i];
                break;
            case 2:
                noteincr += minore[i];
                break;                
            case 3:
            	noteincr += dorico[i];
            	break;
            case 4:
            	noteincr += frigio[i];
            	break;
            case 5:
            	noteincr += lidio[i];
            	break;
            case 6:
            	noteincr += misolidio[i];
            	break;
            case 7:
            	noteincr += locrio[i];
            	break;
            case 8:
            	noteincr += pentatonic[i];
            	break;
          }
          c_scale +=  noteToString(noteincr)+ "|";
          rnd_notes[i+1] = noteincr;
    }
  return c_scale;
}
/*
void noteOn(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}
*/

void setup() {
  lcd.begin(16, 2);
  lcd.print("RNDARP MACHINE");
  Serial.begin(9600);
  pinMode(PIN_INC,INPUT_PULLUP);
  pinMode(PIN_DEC,INPUT_PULLUP);
  pinMode(PIN_MODE,INPUT_PULLUP);
  pinMode(PIN_VAL,INPUT_PULLUP);
  delay(1000);
  lcd.clear();
  lcd.print("VELOCITY");
  lcd.setCursor(0,1);
  lcd.print(velocity_rate);
  delay(100);
  currentMenu=0;
}


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
 
/* MAIN RANDOM FUNCTION */    
void randomize(){
          setRndNotes();
          for (int b=0;b<battute;b++){
            int rndnote = random(11);
            if (scale>1){
              rndnote = rnd_notes[random(0,7)];
            }
            tones[b]=random(octave,octave+range)*12+rndnote;
          }
}    


void inputReading(){
    menuReading = digitalRead(PIN_MODE);
    changeReading = digitalRead(PIN_VAL);
    if (changeReading==0){
      randomize();
    }
    if (menuReading==0){
      lcd.clear();
      if (currentMenu>=6){
        currentMenu=0;
      }else{
        currentMenu+=1;
      } 

      switch (currentMenu) {
        case 0:    // VELOCITY
          menuLabel = "VELOCITY";
          break;
        case 1:    // NOTE
          menuLabel = "NOTE";
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
        case 6: //PLAY
          menuLabel = "PLAY";
          randomize();
          break;
      }    
      setMenuLabel(menuLabel);
      delay(300);
    
  }
}

void incDecManage(int &param,int min_value,int max_value){
      int btn_inc = digitalRead(PIN_INC);
      int btn_dec = digitalRead(PIN_DEC);    
      if (btn_inc==0){
        if (param<max_value){
          param += 1;
        }else{
          param=min_value;
        }        
      }
      if (btn_dec==0){
        if (param>min_value){
          param -= 1;
        }else{
          param=max_value;
        }
      }
      lcd.setCursor(0,1);
}

void loop() {
  inputReading();
  switch (currentMenu) {
    case 0:    // VELOCITY
      incDecManage(velocity_rate,0,100);
      lcd.print(velocity_rate);
      lcd.print("   ");
      delay(100);
      break;
    case 1:    // NOTE
      incDecManage(note,0,11);
      lcd.print(note_str[note]+ " ");
      delay(300);    
      break;
    case 2:    // SCALE
      incDecManage(scale,0,8);
      lcd.print(scale_str[scale] + "   ");
      Serial.println(notesOfCurrentScale());
      delay(300);    
      break;  
    case 3: //RANGE
      incDecManage(range,0,4);
      lcd.print(range);
      delay(300);
      break;
    case 4: //OCTAVE
      incDecManage(octave,0,8);
      lcd.print(octave);
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
      String out = "";
      for (int b=0;b<battute;b++){
        //noteOn(0x90, tones[b], velocity[b]);
        out+= String(tones[b])+"|";
        delay(arp_tick);
        //noteOn(0x90,tones[b], 0x00);
      }
      Serial.println(out);
      break;    
  }
  
}
