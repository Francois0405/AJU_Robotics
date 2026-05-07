#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "SpaceRun.h"
#include "Trivia.h"
#include "Jukebox.h"
#include "pins.h"

// === CREACIÓN OFICIAL DE LA PANTALLA ===
// Los otros archivos usan 'extern LiquidCrystal_I2C lcd;' para conectarse a esta
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- ESTADOS DEL SISTEMA ---
enum ArcaduinoState {
  ESTADO_MENU,
  ESTADO_SPACE,
  ESTADO_TRIVIA,
  ESTADO_JUKEBOX
};
ArcaduinoState estadoActual = ESTADO_MENU;

// --- VARIABLES DEL MENÚ ---
int juegoSeleccionado = 0;
int ultimoJuego = -1;
unsigned long ultimoMovimientoMenu = 0;
const int TOTAL_JUEGOS = 3;
const char* nombresJuegos[] = {"1. Space Runner", "2. Trivia Quiz, 3. Jukebox"};

// ==========================================
//                 SETUP
// ==========================================
void setup() {
  Serial.begin(9600);
  
  // Inicializar pantalla compartida
  lcd.init();
  lcd.backlight();
  
  // Inicializar pines del menú
  pinMode(PIN_JOY_Y, INPUT);
  pinMode(PIN_BOTON, INPUT_PULLUP);
  
  // Llamar al setup de cada minijuego
  setupSpaceRun();
  setupTrivia();
}

// ==========================================
//             LÓGICA DEL MENÚ
// ==========================================
void handleMenu() {
  int y = analogRead(PIN_JOY_Y);
  
  // Navegación con el joystick (con delay integrado sin frenar el código)
  if (millis() - ultimoMovimientoMenu > 250) {
    if (y > 700) { juegoSeleccionado--; ultimoMovimientoMenu = millis(); }
    if (y < 300) { juegoSeleccionado++; ultimoMovimientoMenu = millis(); }
    
    // Efecto "bucle": si pasas del último, vuelves al primero
    if (juegoSeleccionado < 0) juegoSeleccionado = TOTAL_JUEGOS - 1;
    if (juegoSeleccionado >= TOTAL_JUEGOS) juegoSeleccionado = 0;
  }

  // Dibujar el menú SOLO si hemos cambiado de opción (evita parpadeos horribles)
  if (juegoSeleccionado != ultimoJuego) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("=== ARCADUINO ===");
    lcd.setCursor(0, 1);
    lcd.print(">");
    lcd.print(nombresJuegos[juegoSeleccionado]);
    ultimoJuego = juegoSeleccionado;
  }

  // Seleccionar juego con el botón
  if (digitalRead(PIN_BOTON) == LOW) {
    delay(300); // Pequeño anti-rebote
    lcd.clear();
    
    if (juegoSeleccionado == 0) {
      estadoActual = ESTADO_SPACE;
    } else if (juegoSeleccionado == 1) {
      estadoActual = ESTADO_TRIVIA;
    } else if (juegoSeleccionado == 2) {
      estadoActual = ESTADO_JUKEBOX;
    }
  }
}

// ==========================================
//                LOOP PRINCIPAL
// ==========================================
void loop() {
  // El cerebro del Arcaduino
  switch (estadoActual) {
    
    case ESTADO_MENU:
      handleMenu();
      break;
      
    case ESTADO_SPACE:
      // SpaceRun es un juego "infinito", así que se queda atrapado en su propio código
      runSpaceRun();
      break;
      
    case ESTADO_TRIVIA:
      // Trivia ejecuta sus 15 preguntas seguidas. 
      // Al terminar, el código vuelve aquí.
      runTrivia();
      
      // Cuando la Trivia termina, le decimos que vuelva al menú de Arcaduino
      estadoActual = ESTADO_MENU;
      ultimoJuego = -1; // Esto fuerza a que el menú se vuelva a dibujar al volver
      break;

    case ESTADO_JUKEBOX:
      runJukebox();
      estadoActual = ESTADO_MENU;
      ultimoJuego = -1;
      break;
  }
}