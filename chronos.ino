#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);

int MVAL;
int RH = 8;
int LH = 9;
int LEDV = 12;
int LEDA = 10;
int LEDR = 11;
int LEDRSTATE;
int brightness = 180;
int off_brightness = 0;
String LEDCOLOR;
unsigned long LTIME;
unsigned long BTIME;
String DER;
String IZQ;

void setup(){
  Serial.begin(9600);
  lcd.setBacklightPin(3,POSITIVE);  // puerto P3 de PCF8574 como positivo
  lcd.setBacklight(HIGH); // habilita iluminacion posterior de LCD
  lcd.begin(16, 2);
  lcd.clear();
  pinMode(RH, INPUT);
  pinMode(LH, INPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDV, OUTPUT);
  pinMode(LEDA, OUTPUT); 
  analogWrite(LEDA, off_brightness);

}

void loop(){
  Serial.println("----------------------------");
  Serial.println("------------MENU------------");
  Serial.println("Ingresa 1 para prueba normal");
  Serial.println("Ingresa 2 para prueba aleatoria");
  while (Serial.available() <= 0){
    
  }
  
  MVAL = Serial.read();   //espera una respuesta en monitor serial
  
  switch(MVAL){
    case '1':
    
  Serial.println("");
  Serial.println("Inicia prueba normal");
  Serial.println("");
  delay(5000);
  
  lcd.home();
  lcd.print("PREPARATE");
  lcd.setCursor(0, 2);
  lcd.print("presiona ambos");
  delay(5000);
  lcd.clear();
  lcd.print("rojo  derecha");
  lcd.setCursor(0, 2);
  lcd.print("verde izquierda");
  delay(5000);
  lcd.clear();
  lcd.print("Atento al  LED");
  delay(5000);

  for (int i=0; i <= 14; i++)
    {
       delay(5000);
       digitalWrite(LEDV, HIGH);  //Enciende el led despues de una pausa de 10s
  LTIME = millis();
  Serial.print("LEDV ");
  Serial.print(LTIME);   //Imprime el momento en el que se activo el led +-?ns
  Serial.print("ms ");
  
  while(digitalRead(RH) == digitalRead(LH)){ // Espera hasta que se suelte un boton
  
 }
  BTIME = millis();
  if(digitalRead(RH) == LOW){
  IZQ = "";
  DER = "DERECHA:";
 }
 else if(digitalRead(LH) == LOW){
  IZQ = "IZQUIERDA:";
  DER = "";
 }
 
 Serial.print(IZQ);
 Serial.print(DER);
 Serial.print(" ");
 Serial.print(BTIME);
 Serial.print("ms TIEMPO DE REACCIÓN: ");   //Imprime el momento en que se solto el boton +-ns? 
 Serial.print(BTIME - LTIME);
 Serial.println("ms");
 digitalWrite(LEDV, LOW); 

 while(digitalRead(RH) != digitalRead(LH)){ // Espera que se presionen ambos para reiniciar
  
 }
  delay(5000);
  digitalWrite(LEDR, HIGH);  //Enciende el led despues de una pausa de 5s
  LTIME = millis();
  Serial.print("LEDR ");
  Serial.print(LTIME);   //Imprime el momento en el que se activo el led +-?ns
  Serial.print("ms ");
  
  while(digitalRead(RH) == digitalRead(LH)){ // Espera hasta que se suelte un boton
  
 }
  BTIME = millis();
  if(digitalRead(RH) == LOW){
  IZQ = "";
  DER = "DERECHA:";
 }
 else if(digitalRead(LH) == LOW){
  IZQ = "IZQUIERDA:";
  DER = "";
 }
 
 Serial.print(IZQ);
 Serial.print(DER);
 Serial.print(" ");
 Serial.print(BTIME);
 Serial.print("ms TIEMPO DE REACCIÓN: ");   //Imprime el momento en que se solto el boton +-ns? 
 Serial.print(BTIME - LTIME);
 Serial.println("ms");
 digitalWrite(LEDR, LOW); 

 while(digitalRead(RH) != digitalRead(LH)){ // Espera que se presionen ambos para reiniciar
  
 }
    }
  Serial.println("");
  Serial.println("Prueba Finalizada");
  Serial.println("");
  
 break;

 case '2':
 
  Serial.println("");
  Serial.println("Inicia prueba aleatoria");
  Serial.println("");
  delay(5000);
  
  lcd.home();
  lcd.print("PREPARATE");
  lcd.setCursor(0, 2);
  lcd.print("presiona ambos");
  delay(5000);
  lcd.clear();
  lcd.print("rojo  derecha");
  lcd.setCursor(0, 2);
  lcd.print("verde izquierda");
  delay(5000);
  lcd.clear();
  lcd.print("Atento al  LED");
  delay(5000);

  for (int i=0; i <= 31; i++)
    {
      delay(5000);
      LEDRSTATE = random(2);
  if (LEDRSTATE == 1){
    analogWrite(LEDR, off_brightness);
    analogWrite(LEDV, brightness);   //Enciende el led despues de una pausa de 5s
    LEDCOLOR = "LEDV ";
  }else{
    analogWrite(LEDR, brightness);
    analogWrite(LEDV, off_brightness);
    LEDCOLOR = "LEDR ";
  }
  LTIME = millis();
  Serial.print(LEDCOLOR);
  Serial.print(LTIME);   //Imprime el momento en el que se activo el led +-?ns
  Serial.print("ms ");
  
  while(digitalRead(RH) == digitalRead(LH)){ // Espera hasta que se suelte un boton
  
 }
  BTIME = millis();
  if(digitalRead(RH) == LOW){
  IZQ = "";
  DER = "DERECHA:";
 }
 else if(digitalRead(LH) == LOW){
  IZQ = "IZQUIERDA:";
  DER = "";
 }
 
 Serial.print(IZQ);
 Serial.print(DER);
 Serial.print(" ");
 Serial.print(BTIME);
 Serial.print("ms TIEMPO DE REACCIÓN: ");   //Imprime el momento en que se solto el boton +-ns? 
 Serial.println(BTIME - LTIME);
 digitalWrite(LEDR, LOW);
 digitalWrite(LEDV, LOW); 

 while(digitalRead(RH) != digitalRead(LH)){ // Espera que se presionen ambos para reiniciar
  
 } 
    }

  Serial.println("");
  Serial.println("Prueba Finalizada");
  Serial.println("");
  
 break;
 
 }
   
}
    
