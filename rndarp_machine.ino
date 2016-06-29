
/**
  DEFINISCO LE COSTANTI PER LE LETTURE DEGLI INPUT
*/
const int PIN_SEED = -1;     
const int PIN_TEMPO_INC = -1;     
const int PIN_TEMPO_DEC = -1;

//FUNZIONI DI MENU
int sensorMin = 0;
int sensorMax = 1024;
int battute = 8;
int range = 1;
int octave = 5;
int tonalita = 1; //1=C, 2=C#, 12=B etc...
int tempo = 120;
int velocity[] = {127,127,127,127,127,127,127,127};
int tones[] = {60,60,60,60,60,60,60,60};
int rndArray[8];
int currentNotes[8];
int midi_ch=1;
int *ptones;
boolean play = false;

int currentMenu = 0;

String currentMode = "TEMPO";

String scale = "C";
int maggiore[] = {2,2,1,2,2,2,1};
int dorico[] = {2,1,2,2,2,1,2};
int frigio[] = {1,2,2,2,1,2,2};
int lidio[] = {2,2,2,1,2,2,1};
int misolidio[] = {2,2,1,2,2,1,2};
int minore[] = {2,1,2,2,1,2,2};
int locrio[] = {1,2,2,1,2,2,2};
int pentatonic[] = {1,2,3,2,2,3,1};


int ret[8];

float bps = tempo/60;
float tickDuration = 1/bps;
int del_tick = tickDuration*1000;
int arp_tick = del_tick / (battute/4);

int menuReading;
int changeReading;

boolean seed_btn;
boolean tempo_inc;
boolean tempo_dec;   

void noteOn(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}

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
  */

  ret[0] = initNote;
  int i;
  for (i = 0; i < 8; i=i+1){
     ret[i+1] = initNote+modoRange[i];
  } 
}

//Restituisce una rray di battute Randomizza gli array nelle battute (8)
void randomize(int rnd_min, int rnd_max){
  for(int i=0;i<battute;i++){
    int rnd = random(rnd_max-rnd_min);
    rndArray[i] = rnd + rnd_min;
  }
}

void setup() {
  //currentMode = pentatonic
  //range = 1
  //scala = scala(60,currentMode,range);
  play=true;
  // set up the LCD's number of columns and rows:
  //lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("RNDARP MACHINE");
  //Serial.begin(9600);
  Serial.begin(31250);
}

///play the rnd arp
void playSound(){
  for (int i=0;i<8;i=i+1){
    String space = " ";
    Serial.println(tones[0] + space + velocity[i] + space + midi_ch);
  }
}

String menuLabel = "";
int lastMenu = 0;

void updateTempo(){
  bps = tempo/60;
  tickDuration = 1/bps;
  del_tick = tickDuration*1000;
  //se 8 del_tick = 4 battute; 8/4=2
  arp_tick = del_tick / (battute/4);
} 

void inputReading(){
    menuReading = analogRead(A0);
    changeReading = analogRead(A1);
    seed_btn = digitalRead(PIN_SEED);
    tempo_inc = digitalRead(PIN_TEMPO_INC);
    tempo_dec = digitalRead(PIN_TEMPO_DEC);    
    if (tempo_inc){
      tempo += 1;
    }
    if (tempo_dec){
      tempo -= 1;
    }
    if (tempo_dec || tempo_inc){
      updateTempo();
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
  
  // map the sensor range to a range of four options:
  int range = map(menuReading, sensorMin, sensorMax, 0, 6);
  currentMenu=range;
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
      range = map(changeReading, sensorMin, sensorMax, 1, 4);
      break;
    case 4: //OCTAVE
      menuLabel = "OCTAVE";
      octave = map(changeReading, sensorMin, sensorMax, 1, 8);
      break;
  }
  
  if (lastMenu != currentMenu){
    Serial.println(menuLabel);
    lastMenu = currentMenu;
  }
  
  Serial.println(tempo);
  //Serial.println(del_tick);
  if (play==true){
  for (int b=0;b<battute;b++){
    inputReading();
    noteOn(0x90, tones[b], velocity[b]);
    Serial.print(String(tones[b],HEX)+"|" );
    delay(arp_tick);
    noteOn(tones[b], note, 0x00);
  }
  Serial.println();
  }
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  //lcd.print(millis() / 1000);
}
