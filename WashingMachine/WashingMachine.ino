/* Relay pins */
const int RELAY1 = 4; //Soap electro valve
const int RELAY2 = 5; //Softener electro valve
const int RELAY3 = 6; //Draining pump
const int RELAY4 = 7; //Empty
const int RELAY5 = 8; //Motor
const int RELAY6 = 9; //Motor
const int RELAY7 = 10; //Empty
const int RELAY8 = 11; //Empty
/* LED pins*/
const int LED1 = 14; //A0 - Awaiting configuration
const int LED2 = 15; //A1 - Half Load
const int LED3 = 16; //A2 - Wash with soap and rinse
const int LED4 = 17; //A3 - Wash with soap, wash with fabric softener and rinse
const int LED5 = 18; //A4 - Centrifugation
/* Switch pins */
const int SWITCH1 = 2; // Begin/Stop procedures
const int SWITCH2 = 12; //Change load mode
const int SWITCH3 = 13; //Change procedure
/* Variables */
int timeWaterLoadFullLoad = 5000;
int timeWaterLoadHalfLoad = timeWaterLoadFullLoad/2;

int washTime = 2500;

int timeDrainingFullLoad = 2000;
int timeDrainingHalfLoad = timeDrainingFullLoad/2;

int timeRinseFullLoad = 4000;
int timeRinseHalfLoad = timeRinseFullLoad/2;

int timeCentrifugation = 3000;
/* Logic */
int buttonDelay = 250;
bool halfLoad = false; //Full load by default
volatile int mode = 0;
volatile bool rest = true;
/* Board configuration */
void setup()
{   
  Serial.begin(9600);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
  pinMode(RELAY5, OUTPUT);
  pinMode(RELAY6, OUTPUT);
  pinMode(RELAY7, OUTPUT);
  pinMode(RELAY8, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(SWITCH1, INPUT_PULLUP);
  pinMode(SWITCH2, INPUT_PULLUP);
  pinMode(SWITCH3, INPUT_PULLUP);
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);
  digitalWrite(RELAY5, HIGH);
  digitalWrite(RELAY6, HIGH);
  digitalWrite(RELAY7, HIGH);
  digitalWrite(RELAY8, HIGH);
  digitalWrite(LED3, HIGH);
  attachInterrupt(digitalPinToInterrupt(SWITCH1), changeProcedure, LOW);
  Serial.println("Ready");
}

void loop()
{
  if(rest == true){ turnOnStandbyLed(); }
  if(rest == false){
    turnOffStandbyLed();
    if(mode == 0 || mode == 1){ washWithSoap(); }
    if(mode == 1){ washWithSoftener(); }
    if(mode == 0 || mode == 1){ rinse(); }
    if(mode == 2){ centrifuge(); }
    rest = true;
    Serial.println("Done!");
  }
  if(digitalRead(SWITCH1) == LOW){ changeProcedure(); }
  if(digitalRead(SWITCH2) == LOW){ changeLoad(); }
  if(digitalRead(SWITCH3) == LOW){ changeMode(); }
}

//Operating methods
void turnOnStandbyLed(){
digitalWrite(LED1, HIGH);
}
void turnOffStandbyLed(){
    digitalWrite(LED1, LOW);
}
void changeProcedure(){
  delay(buttonDelay);
  rest = !rest;
}
void changeLoad(){
  delay(buttonDelay);
  if(halfLoad == true){
    halfLoad = false;
    digitalWrite(LED2, LOW);
    Serial.println("Half load OFF");
  }
  else if(halfLoad == false){
    halfLoad = true;
    digitalWrite(LED2, HIGH);
    Serial.println("Half load ON");
  }
}
void changeMode(){
  delay(buttonDelay);
  mode++;
  if(mode >= 4) { mode = 0; }
  switch(mode){
    case 0:
      Serial.println("Program : Wash with soap and rinse");
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      break;
    case 1:
      Serial.println("Program : Wash with soap, wash with softener and rinse");
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, LOW);
      break;
    case 2:
      Serial.println("Program : Centrifuge");
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, HIGH);
      break;
  }
}
/* Procedures */
void loadWater(int valve){
  Serial.println("Loading water");
  if(valve == 1){ digitalWrite(RELAY7, LOW); }
  if(valve == 2){ digitalWrite(RELAY8, LOW); }
  if(halfLoad == true){ delay(timeWaterLoadHalfLoad); }
  if(halfLoad == false){ delay(timeWaterLoadFullLoad); }
  if(valve == 1){ digitalWrite(RELAY7, HIGH); }
  if(valve == 2){ digitalWrite(RELAY8, HIGH); }
  Serial.println("Water load completed");
}
void drain(){
  Serial.println("Draining");
  digitalWrite(RELAY3, LOW);
  if(halfLoad == true){ delay(timeDrainingHalfLoad); }
  if(halfLoad == false){ delay(timeDrainingFullLoad); }
  digitalWrite(RELAY3, HIGH);
  Serial.println("Draining completed");
}
void wash(){
  digitalWrite(RELAY5, LOW);
  delay(washTime);
  digitalWrite(RELAY5, HIGH);
}
void washWithSoap(){
  loadWater(1);
  Serial.println("Starting soap wash");
  wash();
  Serial.println("Soap wash completed");
  drain();
}
void washWithSoftener(){
  loadWater(2);
  Serial.println("Starting softener wash");
  wash();
  Serial.println("Softener wash completed");
  drain();
}
void rinse(){
  Serial.println("Rinsing");
  digitalWrite(RELAY7, LOW);
  digitalWrite(RELAY3, LOW);
  digitalWrite(RELAY5, LOW);
  if(halfLoad == true){ delay(timeRinseHalfLoad); }
  if(halfLoad == false){ delay(timeRinseFullLoad); }
  digitalWrite(RELAY7, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY5, HIGH);
  Serial.println("Rinsing completed");
}
void centrifuge(){
  Serial.println("Centrifuging");
  while(rest == false){
    digitalWrite(RELAY3, LOW);
    digitalWrite(RELAY5, LOW);
  }
  Serial.println("Stopping centrifugation");
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY5, HIGH);
}
