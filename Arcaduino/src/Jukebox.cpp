#include <Arduino.h>
#include "Jukebox.h"
#include "Pins.h"
#include <LiquidCrystal_I2C.h>
#include <avr/pgmspace.h>

extern LiquidCrystal_I2C lcd;

static int cancionSeleccionada = 0;
static int ultimaCancion = -1;
static unsigned long ultimoMovJukebox = 0;
const int TOTAL_OPCIONES = 10;

void setupJukebox() {}

// =========================================================
bool reproducirMelodia(const int* notas, const int* duraciones, int longitud, int tempo) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Reproduciendo...")); // ¡Macro F() activada!
  lcd.setCursor(0, 1);
  lcd.print(F("Boton para parar")); // ¡Macro F() activada!

  int wholenote = (60000 * 4) / tempo;
  
  for (int i = 0; i < longitud; i++) {
    if (digitalRead(PIN_BOTON_BACK) == LOW || digitalRead(PIN_BOTON) == LOW) {
      noTone(PIN_BUZZER);
      delay(300);
      return false; 
    }

    int duracionNotaBruta = (int)pgm_read_word(&duraciones[i]);
    int notaActual = (int)pgm_read_word(&notas[i]);
    int noteDuration = 0;
    
    if (duracionNotaBruta > 0) {
      noteDuration = wholenote / duracionNotaBruta;
    } else if (duracionNotaBruta < 0) {
      noteDuration = wholenote / abs(duracionNotaBruta);
      noteDuration *= 1.5;
    }

    if (notaActual != 0) {
      tone(PIN_BUZZER, notaActual, noteDuration * 0.9);
    }
    
    unsigned long startDelay = millis();
    while (millis() - startDelay < (unsigned long)noteDuration) {
      if (digitalRead(PIN_BOTON_BACK) == LOW || digitalRead(PIN_BOTON) == LOW) {
        noTone(PIN_BUZZER);
        delay(300);
        return false; 
      }
    }
    noTone(PIN_BUZZER);
  }
  return true; 
}

void tocarCancion(int indice) {
  delay(300); 
  
  switch(indice) {
    case 0: { // Mario Bros
      static const int notas[] PROGMEM = {659, 659, 0, 659, 0, 523, 659, 0, 784, 0, 0, 0, 392, 0, 0, 0, 523, 0, 392, 0, 330, 0, 440, 494, 466, 440, 392, 659, 784, 880, 698, 784, 659, 523, 587, 494};
      static const int dur[] PROGMEM =   {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 8, 4, 8, 4, 8, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
      reproducirMelodia(notas, dur, 36, 150); break;
    }
    case 1: { // Star Wars
      static const int notas[] PROGMEM = {392, 392, 392, 311, 466, 392, 311, 466, 392, 587, 587, 587, 622, 466, 370, 311, 466, 392};
      static const int dur[] PROGMEM =   {4, 4, 4, -8, 16, 4, -8, 16, 2, 4, 4, 4, -8, 16, 4, -8, 16, 2};
      reproducirMelodia(notas, dur, 18, 120); break;
    }
    case 2: { // Tetris
      static const int notas[] PROGMEM = {659, 494, 523, 587, 523, 494, 440, 440, 523, 659, 587, 523, 494, 494, 523, 587, 659, 523, 440, 440};
      static const int dur[] PROGMEM =   {4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 4, 4, 4, 4};
      reproducirMelodia(notas, dur, 20, 140); break;
    }
    case 3: { // Indiana Jones
      static const int notas[] PROGMEM = {330, 349, 392, 523, 0, 294, 330, 349, 0, 392, 440, 494, 698, 0, 440, 494, 523, 587, 659};
      static const int dur[] PROGMEM =   {8, 8, 4, 2, 8, 8, 8, 2, 8, 8, 8, 4, 2, 8, 8, 8, 4, 4, 2};
      reproducirMelodia(notas, dur, 19, 150); break;
    }
    case 4: { // Harry Potter
      static const int notas[] PROGMEM = {494, 659, 784, 740, 659, 988, 880, 740, 659, 784, 740, 622, 698, 494, 0, 494, 659, 784, 740, 659, 988, 1175, 1109};
      static const int dur[] PROGMEM =   {4, -4, 8, 4, 2, 4, 2, 2, -4, 8, 4, 2, 4, 2, 4, 4, -4, 8, 4, 2, 4, 2, 2};
      reproducirMelodia(notas, dur, 23, 110); break;
    }
    case 5: { // Nokia Theme
      static const int notas[] PROGMEM = {1319, 1175, 740, 831, 1109, 988, 587, 659, 988, 880, 554, 659, 880, 0};
      static const int dur[] PROGMEM =   {8, 8, 4, 4, 8, 8, 4, 4, 8, 8, 4, 4, 2, 4};
      for(int i = 0; i < 2; i++) {
        if (!reproducirMelodia(notas, dur, 14, 180)) break; 
      }
      break;
    }
    case 6: { // Beethoven
      static const int notas[] PROGMEM = {330, 330, 349, 392, 392, 349, 330, 294, 262, 262, 294, 330, 330, 294, 294, 330, 330, 349, 392, 392, 349, 330, 294, 262, 262, 294, 330, 294, 262, 262};
      static const int dur[] PROGMEM =   {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, -4, 8, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, -4, 8, 2};
      reproducirMelodia(notas, dur, 30, 120); break;
    }
    case 7: { // Cumpleaños Feliz
      static const int notas[] PROGMEM = {262, 262, 294, 262, 349, 330, 262, 262, 294, 262, 392, 349, 262, 262, 523, 440, 349, 330, 294, 466, 466, 440, 349, 392, 349};
      static const int dur[] PROGMEM =   {8, 8, 4, 4, 4, 2, 8, 8, 4, 4, 4, 2, 8, 8, 4, 4, 4, 4, 2, 8, 8, 4, 4, 4, 2};
      reproducirMelodia(notas, dur, 25, 120); break;
    }
    case 8: { // Mision Imposible
      static const int bajoN[] PROGMEM = {392, 392, 466, 523, 0, 392, 392, 349, 370, 0};
      static const int bajoD[] PROGMEM = {-4, -4, 8, 8, 8, -4, -4, 8, 8, 8};
      static const int riffN[] PROGMEM = {466, 392, 523, 0, 466, 392, 523};
      static const int riffD[] PROGMEM = {4, 4, 2, 4, 4, 4, 2};
      
      bool seguir = true;
      for(int i = 0; i < 2; i++) {
        if (!reproducirMelodia(bajoN, bajoD, 10, 150)) { seguir = false; break; }
      }
      if (seguir) reproducirMelodia(riffN, riffD, 7, 150);
      break;
    }
    case 9: { // Doom
      static const int notas[] PROGMEM = {165, 165, 330, 165, 165, 311, 165, 165, 294, 165, 165, 277, 165, 165, 262, 165, 165, 233, 247};
      static const int dur[] PROGMEM =   {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4};
      for(int i = 0; i < 2; i++) {
        if (!reproducirMelodia(notas, dur, 19, 220)) break;
      }
      break;
    }
  }
  
  lcd.clear();
  ultimaCancion = -1; 
  delay(300);
}

void runJukebox() {
  lcd.clear();
  cancionSeleccionada = 0;
  ultimaCancion = -1;
  bool enJukebox = true;

  while(enJukebox) {
    if (digitalRead(PIN_BOTON_BACK) == LOW) {
      delay(300);
      enJukebox = false; 
      break;
    }

    int y = analogRead(PIN_JOY_Y);
    
    if (millis() - ultimoMovJukebox > 250) {
      if (y > 700) { cancionSeleccionada--; ultimoMovJukebox = millis(); }
      if (y < 300) { cancionSeleccionada++; ultimoMovJukebox = millis(); }
      
      if (cancionSeleccionada < 0) cancionSeleccionada = TOTAL_OPCIONES - 1;
      if (cancionSeleccionada >= TOTAL_OPCIONES) cancionSeleccionada = 0;
    }

    if (cancionSeleccionada != ultimaCancion) {
      lcd.clear();
      lcd.setCursor(0, 0); 
      lcd.print(F("=== JUKEBOX ===")); // ¡Macro F() activada!
      lcd.setCursor(0, 1); 
      lcd.print(">");
      
      // Adiós array, hola switch directo (consume 0 RAM)
      switch(cancionSeleccionada) {
        case 0: lcd.print(F("Mario Bros")); break;
        case 1: lcd.print(F("Star Wars")); break;
        case 2: lcd.print(F("Tetris")); break;
        case 3: lcd.print(F("Indiana Jones")); break;
        case 4: lcd.print(F("Harry Potter")); break;
        case 5: lcd.print(F("Nokia Theme")); break;
        case 6: lcd.print(F("Beethoven")); break;
        case 7: lcd.print(F("Cumpleanos")); break;
        case 8: lcd.print(F("Mision Imp.")); break;
        case 9: lcd.print(F("Doom")); break;
      }
      ultimaCancion = cancionSeleccionada;
    }

    if (digitalRead(PIN_BOTON) == LOW) {
      tocarCancion(cancionSeleccionada);
    }
  }
}