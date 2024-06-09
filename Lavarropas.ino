/*Pines de los Relé*/
const int RELE1 = 4; //Electro válvula de jabón
const int RELE2 = 5; //Electro válvula de suavizante
const int RELE3 = 6; //Bomba de vaciado
const int RELE4 = 7; //Vacío
const int RELE5 = 8; //Motor
const int RELE6 = 9; //Motor
const int RELE7 = 10; //Vacío
const int RELE8 = 11; //Vacío
/*Pines de los LEDs*/
const int LED1 = 14; //A0 - Esperando configuracion
const int LED2 = 15; //A1 - Media carga
const int LED3 = 16; //A2 - Lavado con jabon y enjuague
const int LED4 = 17; //A3 - Lavado con jabón, lavado con suavizante y enjuague
const int LED5 = 18; //A4 - Centrifugado
/*Pines de los interruptores*/
const int INTERRUPTOR1 = 2; // Arranca/Para el procedimiento
const int INTERRUPTOR2 = 12; //Cambiar modo de carga
const int INTERRUPTOR3 = 13; //Cambiar procedimiento
/*Variables*/
int tiempoCargarAguaCargaCompleta = 5000;
int tiempoCargarAguaCargaMedia = tiempoCargarAguaCargaCompleta/2;

int tiempoLavado = 2500;

int tiempoVaciadoCargaCompleta = 2000;
int tiempoVaciadoCargaMedia = tiempoVaciadoCargaCompleta/2;

int tiempoEnjuagueCargaCompleta = 4000;
int tiempoEnjuagueMediaCarga = tiempoEnjuagueCargaCompleta/2;

int tiempoCentrifugado = 3000;
/*Lógica*/
int delayBoton = 250;
bool mediaCarga = false; //Por defecto lleno, cambiar a true si se desea
volatile int modo = 0;
volatile bool reposo = true;
/*Configuración de la placa*/
void setup()
{   
  Serial.begin(9600);
  pinMode(RELE1, OUTPUT);
  pinMode(RELE2, OUTPUT);
  pinMode(RELE3, OUTPUT);
  pinMode(RELE4, OUTPUT);
  pinMode(RELE5, OUTPUT);
  pinMode(RELE6, OUTPUT);
  pinMode(RELE7, OUTPUT);
  pinMode(RELE8, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(INTERRUPTOR1, INPUT_PULLUP);
  pinMode(INTERRUPTOR2, INPUT_PULLUP);
  pinMode(INTERRUPTOR3, INPUT_PULLUP);
  digitalWrite(RELE1, HIGH);
  digitalWrite(RELE2, HIGH);
  digitalWrite(RELE3, HIGH);
  digitalWrite(RELE4, HIGH);
  digitalWrite(RELE5, HIGH);
  digitalWrite(RELE6, HIGH);
  digitalWrite(RELE7, HIGH);
  digitalWrite(RELE8, HIGH);
  digitalWrite(LED3, HIGH);
  attachInterrupt(digitalPinToInterrupt(INTERRUPTOR1), cambiarProcedimiento, LOW);
  Serial.println("Preparado");
}
void loop()
{
  if(reposo == true){ digitalWrite(LED1, HIGH); }
  if(reposo == false){
    digitalWrite(LED1, LOW);
    if(modo == 0 || modo == 1){ lavarConJabon(); }
    if(modo == 1){ lavarConSuavizante(); }
    if(modo == 0 || modo == 1){ enjuagar(); }
    if(modo == 2){ centrifugar(); }
    if(modo == 3){ enjuagar(); }
    reposo = true;
    Serial.println("Listo!");
  }
  if(digitalRead(INTERRUPTOR1) == LOW){ cambiarProcedimiento(); }
  if(digitalRead(INTERRUPTOR2) == LOW){ cambiarCarga(); }
  if(digitalRead(INTERRUPTOR3) == LOW){ cambiarModo(); }
}
void cambiarProcedimiento(){
  delay(delayBoton);
  reposo = !reposo;
}
void cambiarCarga(){
  delay(delayBoton);
  if(mediaCarga == true){
    mediaCarga = false;
    digitalWrite(LED2, LOW);
    Serial.println("Media carga desactivada");
  }
  else if(mediaCarga == false){
    mediaCarga = true;
    digitalWrite(LED2, HIGH);
    Serial.println("Media carga activada");
  }
}
void cambiarModo(){
  delay(delayBoton);
  modo++;
  if(modo >= 4) { modo = 0; }
  switch(modo){
    case 0:
      Serial.println("Programa : Lavado con jabón y enjuague");
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, HIGH);
      break;
    case 1:
      Serial.println("Programa : Lavado con jabón, lavado con suavizante y enjuague");
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, HIGH);
      break;
    case 2:
      Serial.println("Programa : Centrifugado");
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, HIGH);
      digitalWrite(LED6, LOW);
      break;
    case 3:
      Serial.println("Programa : Enjuagar");
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, HIGH);
      
  }
}
/*Procedimientos*/
void cargarAgua(int valvula){
  Serial.println("Cargando agua");
  if(valvula == 1){ digitalWrite(RELE1, LOW); }
  if(valvula == 2){ digitalWrite(RELE2, LOW); }
  if(mediaCarga == true){ delay(tiempoCargarAguaCargaMedia); }
  if(mediaCarga == false){ delay(tiempoCargarAguaCargaCompleta); }
  if(valvula == 1){ digitalWrite(RELE1, HIGH); }
  if(valvula == 2){ digitalWrite(RELE2, HIGH); }
  Serial.println("Carga de agua completada");
}
void vaciar(){
  Serial.println("Vaciando");
  digitalWrite(RELE3, LOW);
  if(mediaCarga == true){ delay(tiempoVaciadoCargaMedia); }
  if(mediaCarga == false){ delay(tiempoVaciadoCargaCompleta); }
  digitalWrite(RELE3, HIGH);
  Serial.println("Vaciado completado");
}
void lavar(){
  digitalWrite(RELE5, LOW);
  delay(tiempoLavado);
  digitalWrite(RELE5, HIGH);
}
void lavarConJabon(){
  cargarAgua(1);
  Serial.println("Iniciando lavado con jabón");
  lavar();
  Serial.println("Lavado con jabón completado");
  vaciar();
}
void lavarConSuavizante(){
  cargarAgua(2);
  Serial.println("Iniciando lavado con suavizante");
  lavar();
  Serial.println("Lavado con suavizante completado");
  vaciar();
}
void enjuagar(){
  Serial.println("Enjuagando");
  digitalWrite(RELE1, LOW);
  digitalWrite(RELE3, LOW);
  digitalWrite(RELE5, LOW);
  if(mediaCarga == true){ delay(tiempoEnjuagueMediaCarga); }
  if(mediaCarga == false){ delay(tiempoEnjuagueCargaCompleta); }
  digitalWrite(RELE1, HIGH);
  digitalWrite(RELE3, HIGH);
  digitalWrite(RELE5, HIGH);
  Serial.println("Enjuague completado");
}
void centrifugar(){
  Serial.println("Centrifugando");
  while(reposo == false){
    digitalWrite(RELE3, LOW);
    digitalWrite(RELE5, LOW);
  }
  Serial.println("Deteniendo centrifugado");
  digitalWrite(RELE3, HIGH);
  digitalWrite(RELE5, HIGH);
}
