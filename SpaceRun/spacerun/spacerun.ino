#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- MEMORIA Y PUNTOS ---
const int EEPROM_ADDR = 0; // Dirección de inicio
long puntuacion = 0;
long maxPuntuacion = 0;

// --- HARDWARE ---
int joystickY = A0;
int boton = 7;
int buzzer = 8; 

// --- JUEGO ---
char fila0[16], fila1[16];
int playerRow = 0;
bool enMenu = true;
int seleccion = 0;
int velocidad = 200;
int lastSeleccion = -1;
String nombres[] = {"EASY", "MED", "HARD", "INSANE"};
int velocidades[] = {250, 180, 120, 70};

unsigned long tiempoAnterior = 0; 

// --- MÚSICA MAESTRA (12 NOTAS ÉPICAS) ---
int melodia[] = {165, 196, 220, 165, 196, 247, 220, 165, 147, 131, 165, 123};
int notaActual = 0;
unsigned long tiempoUltimaNota = 0;

// -------------------- LÓGICA DE AUDIO --------------------
void sonarMuerte() {
  // Efecto de explosión y caída de energía
  for (int f = 1500; f > 40; f -= 20) {
    tone(buzzer, f);
    delay(10);
  }
  noTone(buzzer);
}

void sonarLaser() {
  tone(buzzer, 1800, 40); // Láser muy agudo y rápido
}

// -------------------- UTILIDADES --------------------
String formatearPuntos(long puntos) {
  if (puntos < 10000) return String(puntos);
  return String(puntos / 1000) + "k"; 
}

// -------------------- MOTOR DEL JUEGO --------------------
void disparar() {
  bool golpe = false;
  for (int i = 0; i < 16; i++) {
    if ((playerRow == 0 && fila0[i] == 'O') || (playerRow == 1 && fila1[i] == 'O')) {
      if (playerRow == 0) fila0[i] = ' '; else fila1[i] = ' ';
      golpe = true;
    }
  }
  if (golpe) {
    puntuacion += 10; 
    lcd.setCursor(0, playerRow);
    lcd.print("*"); 
    sonarLaser();
  }
}

void generarObstaculo() {
  int r = random(0, 10);
  if (r == 0) fila0[15] = '#';      
  else if (r == 1) fila1[15] = '#'; 
  else if (r == 2) fila0[15] = 'O'; 
  else if (r == 3) fila1[15] = 'O';
}

void moverMundo() {
  for (int i = 0; i < 15; i++) {
    fila0[i] = fila0[i + 1];
    fila1[i] = fila1[i + 1];
  }
  fila0[15] = ' '; fila1[15] = ' ';
}

void dibujar() {
  for (int r = 0; r < 2; r++) {
    lcd.setCursor(0, r);
    for (int i = 0; i < 16; i++) {
      if (playerRow == r && i == 0) lcd.print(">");
      else lcd.print(r == 0 ? fila0[i] : fila1[i]);
    }
  }
}

void gameOver() {
  sonarMuerte();
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("GAME OVER");

  if (puntuacion > maxPuntuacion) {
    maxPuntuacion = puntuacion;
    EEPROM.put(EEPROM_ADDR, maxPuntuacion); // Guardado físico
  }

  lcd.setCursor(0, 1);
  lcd.print("Pts:"); lcd.print(formatearPuntos(puntuacion));
  lcd.setCursor(9, 1);
  lcd.print("HI:"); lcd.print(formatearPuntos(maxPuntuacion));

  delay(3000);
  enMenu = true;
  lastSeleccion = -1;
  lcd.clear();
}

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(joystickY, INPUT);
  pinMode(boton, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  
  // Recuperar Highscore de forma segura
  EEPROM.get(EEPROM_ADDR, maxPuntuacion);
  if (maxPuntuacion < 0 || maxPuntuacion > 999999) maxPuntuacion = 0; 
  
  randomSeed(analogRead(A1));
  lcd.clear();
}

void loop() {
  if (enMenu) {
    int y = analogRead(joystickY);
    if (y > 700) { seleccion--; delay(200); }
    else if (y < 300) { seleccion++; delay(200); }
    if (seleccion < 0) seleccion = 3;
    if (seleccion > 3) seleccion = 0;

    if (seleccion != lastSeleccion) {
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("Modo: "); lcd.print(nombres[seleccion]);
      lcd.setCursor(0, 1); lcd.print("Record: "); lcd.print(formatearPuntos(maxPuntuacion));
      lastSeleccion = seleccion;
    }

    if (digitalRead(boton) == LOW) {
      velocidad = velocidades[seleccion];
      puntuacion = 0;
      for (int i = 0; i < 16; i++) { fila0[i] = ' '; fila1[i] = ' '; }
      enMenu = false;
      lcd.clear();
      tiempoAnterior = millis();
      delay(300);
    }
    return;
  }

  unsigned long tiempoActual = millis();

  // --- REPRODUCTOR MÚSICA (Ritmo dinámico) ---
  if (tiempoActual - tiempoUltimaNota >= (velocidad * 1.8)) {
    tiempoUltimaNota = tiempoActual;
    tone(buzzer, melodia[notaActual], 60);
    notaActual = (notaActual + 1) % 12;
  }

  // --- INPUTS ---
  if (digitalRead(boton) == LOW) { disparar(); delay(150); }
  int y = analogRead(joystickY);
  if (y > 700) playerRow = 0; else if (y < 300) playerRow = 1;

  // --- MOTOR ---
  if (tiempoActual - tiempoAnterior >= velocidad) {
    tiempoAnterior = tiempoActual;
    moverMundo();
    generarObstaculo();
    puntuacion++;
    if ((playerRow == 0 && (fila0[0]=='#' || fila0[0]=='O')) || 
        (playerRow == 1 && (fila1[0]=='#' || fila1[0]=='O'))) {
      gameOver();
    } else {
      dibujar();
    }
  }
}