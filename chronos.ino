#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);

int MVAL;
const byte RH = 8;
const byte LH = 9;
const byte LEDV = 12;
const byte LEDA = 10;
const byte LEDR = 11;

int LEDRSTATE;
unsigned long LTIME;
unsigned long BTIME;

const char* DER = "";
const char* IZQ = "";

// Se acorto el codigo mediante esta funcion. Ejecuta una prueba: enciende ledPin, mide tiempo hasta que se suelte un botón,
// imprime resultado, apaga el LED y espera a que se presionen ambos para reiniciar.
unsigned long runTrial(const char* label, byte ledPin) {
  digitalWrite(ledPin, HIGH);
  unsigned long L = millis();
  LTIME = L;

  Serial.print(label);
  Serial.print(L);
  Serial.print("ms ");

  // Espera hasta que se suelte un botón (dejen de ser iguales)
  while (digitalRead(RH) == digitalRead(LH)) { ; }

  unsigned long B = millis();
  BTIME = B;

  // LOW = soltó (por tu pull-down a GND y botón a 5V)
  if (digitalRead(RH) == LOW) {
    IZQ = "";
    DER = "DERECHA:";
  } else if (digitalRead(LH) == LOW) {
    IZQ = "IZQUIERDA:";
    DER = "";
  }

  Serial.print(IZQ);
  Serial.print(DER);
  Serial.print(" ");
  Serial.print(B);
  Serial.print("ms TIEMPO DE REACCIÓN: ");
  Serial.print(B - L);
  Serial.println("ms");

  digitalWrite(ledPin, LOW);

  // Espera que se presionen ambos para reiniciar (vuelven a ser iguales)
  while (digitalRead(RH) != digitalRead(LH)) { ; }

  return (B - L);
}

void setup() {
  Serial.begin(9600);

  lcd.setBacklightPin(3, POSITIVE);  // puerto P3 de PCF8574 como positivo
  lcd.setBacklight(HIGH);            // habilita iluminacion posterior de LCD
  lcd.begin(16, 2);
  lcd.clear();

  pinMode(RH, INPUT);
  pinMode(LH, INPUT);

  pinMode(LEDR, OUTPUT);
  pinMode(LEDV, OUTPUT);
  pinMode(LEDA, OUTPUT);
  digitalWrite(LEDA, LOW);           // Azul apagado (futura implementación)

  randomSeed(micros());              // semilla para random del case 2
}

void loop() {
  Serial.println("----------------------------");
  Serial.println("------------MENU------------");
  Serial.println("Ingresa 1 para prueba normal");
  Serial.println("Ingresa 2 para prueba aleatoria");

  while (Serial.available() <= 0) { ; }

  MVAL = Serial.read();   // espera una respuesta en monitor serial

  switch (MVAL) {

    case '1':
      Serial.println("");
      Serial.println("Inicia prueba normal");
      Serial.println("");
      delay(5000);

      lcd.home();
      lcd.print("PREPARATE");
      lcd.setCursor(0, 1);
      lcd.print("presiona ambos");
      delay(5000);

      lcd.clear();
      lcd.print("Rojo derecha   >");
      lcd.setCursor(0, 1);
      lcd.print("Verde izquierda<");
      delay(5000);

      lcd.clear();
      lcd.print("Atento al  LED");
      delay(5000);

      for (int i = 0; i <= 5; i++) {
        delay(5000);
        runTrial("LEDV ", LEDV);

        delay(5000);
        runTrial("LEDR ", LEDR);
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
      lcd.setCursor(0, 1);
      lcd.print("presiona ambos");
      delay(5000);

      lcd.clear();
      lcd.print("rojo  derecha");
      lcd.setCursor(0, 1);
      lcd.print("verde izquierda");
      delay(5000);

      lcd.clear();
      lcd.print("Atento al  LED");
      delay(5000);

      for (int i = 0; i <= 5; i++) {
        delay(5000);
        LEDRSTATE = random(2);

        if (LEDRSTATE == 1) {
          runTrial("LEDV ", LEDV);
        } else {
          runTrial("LEDR ", LEDR);
        }
      }

      Serial.println("");
      Serial.println("Prueba Finalizada");
      Serial.println("");
      break;

    default:
      Serial.println("Opcion invalida (usa 1 o 2)");
      // limpia el buffer por si quedó \r o \n
      while (Serial.available() > 0) Serial.read();
      break;
  }
}