
const int battute = 8;

int sensorMin = 0;
int sensorMax = 500;
int tempo = 120;
int velocities[] = {127,127,127,127,127,127,127,127};
int seeds[8];
int tones[] = {127,127,127,127,127,127,127,127};

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

void scala(int initNote,int modo[], int range)
{
  int modoRange[]={};
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

//Restituisce una rray di battute Randomizza gli array nelle battute (8)
void randomize(int generator[])
{
  int ret[8];
  int i;
  for(i=0;i<battute;i++){
    int rnd = random(sizeOf(generator));
    ret[i] = generator[rnd];
  }
}


void setup() {
  battute = 8;
  currentMode = pentatonic
  range = 1
  scala = scala(60,currentMode,range);
  
  // set up the LCD's number of columns and rows:
  //lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("RNDARP MACHINE");
}

void loop() {
  
  int menuReading = analogRead(A0);
  // map the sensor range to a range of four options:
  int range = map(menuReading, sensorMin, sensorMax, 0, 3);
  
  switch (range) {
    case 0:    // TEMPO
      Serial.println("");
      break;
    case 1:    // VELOCITY
      Serial.println("dim");
      break;
    case 2:    // SEEDS
      Serial.println("seeds");
      break;
    case 3:    // TONE
      Serial.println("tone");
      break;
    case 4:    // SCALE
      Serial.println("scale");
      break;  
    case 5: //RANGE
      Serial.println("Range");
      break;
    case 6: //OCTAVE
      Serial.println("Range octave");
      break;
  }
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
}
