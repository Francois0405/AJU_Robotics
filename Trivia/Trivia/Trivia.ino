#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <avr/pgmspace.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

const int pinX = A0;
const int pinY = A1;
const int pinBoton = 12;

int valorX = 0;
int valorY = 0;
int estadoBoton = 0;
int Preguntas = 0;
int puntuacion = 0;

// Variables para el movimiento entre opciones
int opcionSeleccionada = 0;
unsigned long ultimoMovimiento = 0;
int delayMovimiento = 200;

// ===== TODOS LOS STRINGS EN PROGMEM (FLASH) =====
// Títulos de las preguntas (más cortos para que quepan)
const char titulo0[] PROGMEM = "Cap. Espanya";
const char titulo1[] PROGMEM = "Cap. Franca";
const char titulo2[] PROGMEM = "Cap. Italia";
const char titulo3[] PROGMEM = "Cap. Portugal";
const char titulo4[] PROGMEM = "2+2";
const char titulo5[] PROGMEM = "5x5";
const char titulo6[] PROGMEM = "10/2";
const char titulo7[] PROGMEM = "15-7";
const char titulo8[] PROGMEM = "Color cel?";
const char titulo9[] PROGMEM = "Planeta gran";
const char titulo10[] PROGMEM = "Dies any?";
const char titulo11[] PROGMEM = "Any America?";
const char titulo12[] PROGMEM = "Home lluna?";
const char titulo13[] PROGMEM = "Pintor Guernica";
const char titulo14[] PROGMEM = "Moneda UE";

const char* const titulos[] PROGMEM = {
  titulo0, titulo1, titulo2, titulo3, titulo4,
  titulo5, titulo6, titulo7, titulo8, titulo9,
  titulo10, titulo11, titulo12, titulo13, titulo14
};

// Opciones incorrectas
const char op00[] PROGMEM = "Barna";
const char op01[] PROGMEM = "Sevilla";
const char op02[] PROGMEM = "Girona";
const char op10[] PROGMEM = "Lyon";
const char op11[] PROGMEM = "Marsella";
const char op12[] PROGMEM = "Tolosa";
const char op20[] PROGMEM = "Venecia";
const char op21[] PROGMEM = "Napols";
const char op22[] PROGMEM = "Milan";
const char op30[] PROGMEM = "Porto";
const char op31[] PROGMEM = "Braga";
const char op32[] PROGMEM = "Faro";
const char op40[] PROGMEM = "3";
const char op41[] PROGMEM = "5";
const char op42[] PROGMEM = "6";
const char op50[] PROGMEM = "20";
const char op51[] PROGMEM = "30";
const char op52[] PROGMEM = "35";
const char op60[] PROGMEM = "3";
const char op61[] PROGMEM = "4";
const char op62[] PROGMEM = "6";
const char op70[] PROGMEM = "6";
const char op71[] PROGMEM = "7";
const char op72[] PROGMEM = "9";
const char op80[] PROGMEM = "Roig";
const char op81[] PROGMEM = "Verd";
const char op82[] PROGMEM = "Groc";
const char op90[] PROGMEM = "Venus";
const char op91[] PROGMEM = "Mart";
const char op92[] PROGMEM = "Saturn";
const char op100[] PROGMEM = "360";
const char op101[] PROGMEM = "366";
const char op102[] PROGMEM = "364";
const char op110[] PROGMEM = "1500";
const char op111[] PROGMEM = "1488";
const char op112[] PROGMEM = "1510";
const char op120[] PROGMEM = "1965";
const char op121[] PROGMEM = "1972";
const char op122[] PROGMEM = "1963";
const char op130[] PROGMEM = "Dali";
const char op131[] PROGMEM = "Goya";
const char op132[] PROGMEM = "Velaz";
const char op140[] PROGMEM = "Dolar";
const char op141[] PROGMEM = "Lliura";
const char op142[] PROGMEM = "Yen";

// Matriz de punteros a opciones incorrectas
const char* const opcionesIncorrectas[15][3] PROGMEM = {
  {op00, op01, op02},
  {op10, op11, op12},
  {op20, op21, op22},
  {op30, op31, op32},
  {op40, op41, op42},
  {op50, op51, op52},
  {op60, op61, op62},
  {op70, op71, op72},
  {op80, op81, op82},
  {op90, op91, op92},
  {op100, op101, op102},
  {op110, op111, op112},
  {op120, op121, op122},
  {op130, op131, op132},
  {op140, op141, op142}
};

// Letras correctas (en RAM)
const char letrasCorrectas[] = {
  'C', 'B', 'C', 'D', 'B',
  'B', 'C', 'C', 'B', 'C',
  'B', 'A', 'B', 'B', 'B'
};

// Textos correctos en PROGMEM
const char correcta0[] PROGMEM = "Madrid";
const char correcta1[] PROGMEM = "Paris";
const char correcta2[] PROGMEM = "Roma";
const char correcta3[] PROGMEM = "Lisboa";
const char correcta4[] PROGMEM = "4";
const char correcta5[] PROGMEM = "25";
const char correcta6[] PROGMEM = "5";
const char correcta7[] PROGMEM = "8";
const char correcta8[] PROGMEM = "Blau";
const char correcta9[] PROGMEM = "Jupiter";
const char correcta10[] PROGMEM = "365";
const char correcta11[] PROGMEM = "1492";
const char correcta12[] PROGMEM = "1969";
const char correcta13[] PROGMEM = "Picasso";
const char correcta14[] PROGMEM = "Euro";

const char* const textosCorrectos[] PROGMEM = {
  correcta0, correcta1, correcta2, correcta3, correcta4,
  correcta5, correcta6, correcta7, correcta8, correcta9,
  correcta10, correcta11, correcta12, correcta13, correcta14
};

// Explicaciones en PROGMEM
const char exp0[] PROGMEM = "Madrid capital";
const char exp1[] PROGMEM = "Paris capital";
const char exp2[] PROGMEM = "Roma capital";
const char exp3[] PROGMEM = "Lisboa capital";
const char exp4[] PROGMEM = "2+2=4";
const char exp5[] PROGMEM = "5x5=25";
const char exp6[] PROGMEM = "10/2=5";
const char exp7[] PROGMEM = "15-7=8";
const char exp8[] PROGMEM = "Cel blau";
const char exp9[] PROGMEM = "Jupiter gran";
const char exp10[] PROGMEM = "365 dies";
const char exp11[] PROGMEM = "1492 America";
const char exp12[] PROGMEM = "1969 Apollo";
const char exp13[] PROGMEM = "Picasso pinto";
const char exp14[] PROGMEM = "Euro moneda";

const char* const explicaciones[] PROGMEM = {
  exp0, exp1, exp2, exp3, exp4,
  exp5, exp6, exp7, exp8, exp9,
  exp10, exp11, exp12, exp13, exp14
};

// Buffer para leer strings de PROGMEM
char buffer[20];

void setup() 
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  Serial.begin(9600);
  pinMode(pinBoton, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("Joystick + LCD");
  lcd.setCursor(0, 1);
  lcd.print("15 preguntes!");
  delay(2000);
  lcd.clear();
}

void loop() 
{
  // INICIO DEL JUEGO
  lcd.setCursor(0, 0);
  lcd.print("PREPARAT?");
  lcd.setCursor(0, 1);
  lcd.print("Prem el boto");
  
  while(digitalRead(pinBoton) == HIGH) {
    delay(50);
  }
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Comenca!");
  delay(1000);
  lcd.clear();
  
  // BUCLE PRINCIPAL DEL JUEGO
  Preguntas = 0;
  puntuacion = 0;
  boolean juegoActivo = true;
  
  while(juegoActivo) {
    
    // MOSTRAR PREGUNTA ACTUAL
    mostrarPregunta(Preguntas);
    delay(2000);
    
    // MOSTRAR Y SELECCIONAR RESPUESTA
    char respuestaJugador = seleccionarRespuesta(Preguntas);
    
    // COMPROBAR RESPUESTA
    comprobarRespuesta(Preguntas, respuestaJugador);
    
    // PASAR A LA SIGUIENTE PREGUNTA
    Preguntas++;
    
    if(Preguntas >= 15) {
      juegoActivo = false;
    }
  }
  
  // FIN DEL JUEGO
  mostrarResultados();
  
  delay(5000);
}

void mostrarPregunta(int num) {
  lcd.clear();
  
  // Leer título
  strcpy_P(buffer, (char*)pgm_read_word(&(titulos[num])));
  lcd.setCursor(0, 0);
  lcd.print(buffer);
  delay(1500);
  
  lcd.clear();
  
  char correcta = letrasCorrectas[num];
  
  // Mostrar opción A
  lcd.setCursor(0, 0);
  lcd.print("A:");
  if(correcta == 'A') {
    strcpy_P(buffer, (char*)pgm_read_word(&(textosCorrectos[num])));
  } else {
    strcpy_P(buffer, (char*)pgm_read_word(&(opcionesIncorrectas[num][0])));
  }
  buffer[6] = '\0';  // Limitar a 6 caracteres
  lcd.print(buffer);
  
  // Mostrar opción B
  lcd.setCursor(8, 0);
  lcd.print("B:");
  if(correcta == 'B') {
    strcpy_P(buffer, (char*)pgm_read_word(&(textosCorrectos[num])));
  } else {
    strcpy_P(buffer, (char*)pgm_read_word(&(opcionesIncorrectas[num][1])));
  }
  buffer[6] = '\0';
  lcd.print(buffer);
  
  // Mostrar opción C
  lcd.setCursor(0, 1);
  lcd.print("C:");
  if(correcta == 'C') {
    strcpy_P(buffer, (char*)pgm_read_word(&(textosCorrectos[num])));
  } else {
    strcpy_P(buffer, (char*)pgm_read_word(&(opcionesIncorrectas[num][2])));
  }
  buffer[6] = '\0';
  lcd.print(buffer);
  
  // Mostrar opción D
  lcd.setCursor(8, 1);
  lcd.print("D:");
  if(correcta == 'D') {
    strcpy_P(buffer, (char*)pgm_read_word(&(textosCorrectos[num])));
  } else {
    strcpy_P(buffer, (char*)pgm_read_word(&(opcionesIncorrectas[num][0]))); // Reusamos la primera
  }
  buffer[6] = '\0';
  lcd.print(buffer);
}

char seleccionarRespuesta(int numPregunta) {
  opcionSeleccionada = 0;
  boolean respondida = false;
  char respuesta = ' ';
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tria resposta");
  lcd.setCursor(0, 1);
  lcd.print("Mou el joystick");
  delay(1500);
  
  // Volver a mostrar las opciones
  mostrarPregunta(numPregunta);
  
  while(!respondida) {
    valorX = analogRead(pinX);
    valorY = analogRead(pinY);
    estadoBoton = digitalRead(pinBoton);
    
    if(millis() - ultimoMovimiento > delayMovimiento) {
      if(valorX < 200) {
        if(opcionSeleccionada == 1) opcionSeleccionada = 0;
        else if(opcionSeleccionada == 3) opcionSeleccionada = 2;
        ultimoMovimiento = millis();
      }
      else if(valorX > 800) {
        if(opcionSeleccionada == 0) opcionSeleccionada = 1;
        else if(opcionSeleccionada == 2) opcionSeleccionada = 3;
        ultimoMovimiento = millis();
      }
      
      if(valorY < 200) {
        if(opcionSeleccionada == 2) opcionSeleccionada = 0;
        else if(opcionSeleccionada == 3) opcionSeleccionada = 1;
        ultimoMovimiento = millis();
      }
      else if(valorY > 800) {
        if(opcionSeleccionada == 0) opcionSeleccionada = 2;
        else if(opcionSeleccionada == 1) opcionSeleccionada = 3;
        ultimoMovimiento = millis();
      }
    }
    
    actualizarLEDsPorOpcion(opcionSeleccionada);
    mostrarSeleccion(opcionSeleccionada);
    
    static int ultimoBoton = HIGH;
    if(ultimoBoton == HIGH && estadoBoton == LOW) {
      switch(opcionSeleccionada) {
        case 0: respuesta = 'A'; break;
        case 1: respuesta = 'B'; break;
        case 2: respuesta = 'C'; break;
        case 3: respuesta = 'D'; break;
      }
      respondida = true;
      delay(200);
    }
    ultimoBoton = estadoBoton;
    
    delay(50);
  }
  
  return respuesta;
}

void actualizarLEDsPorOpcion(int opcion) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  
  switch(opcion) {
    case 0: digitalWrite(IN1, HIGH); break;
    case 1: digitalWrite(IN2, HIGH); break;
    case 2: digitalWrite(IN3, HIGH); break;
    case 3: digitalWrite(IN4, HIGH); break;
  }
}

void mostrarSeleccion(int opcion) {
  // Limpiar indicadores anteriores
  lcd.setCursor(0, 0); lcd.print(" ");
  lcd.setCursor(8, 0); lcd.print(" ");
  lcd.setCursor(0, 1); lcd.print(" ");
  lcd.setCursor(8, 1); lcd.print(" ");
  
  // Mostrar nuevo indicador
  switch(opcion) {
    case 0: lcd.setCursor(0, 0); lcd.print(">"); break;
    case 1: lcd.setCursor(8, 0); lcd.print(">"); break;
    case 2: lcd.setCursor(0, 1); lcd.print(">"); break;
    case 3: lcd.setCursor(8, 1); lcd.print(">"); break;
  }
}

void comprobarRespuesta(int numPregunta, char respuesta) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Has triat: ");
  lcd.print(respuesta);
  
  lcd.setCursor(0, 1);
  if(respuesta == letrasCorrectas[numPregunta]) {
    lcd.print("CORRECTE!");
    puntuacion++;
    
    delay(1500);
    lcd.clear();
    lcd.setCursor(0, 0);
    strcpy_P(buffer, (char*)pgm_read_word(&(explicaciones[numPregunta])));
    lcd.print(buffer);
  } else {
    lcd.print("INCORRECTE!");
    delay(1500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Correcta: ");
    strcpy_P(buffer, (char*)pgm_read_word(&(textosCorrectos[numPregunta])));
    lcd.print(buffer);
  }
  
  delay(2000);
  lcd.clear();
}

void mostrarResultados() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("FI DEL JOC!");
  lcd.setCursor(0, 1);
  lcd.print("Punts: ");
  lcd.print(puntuacion);
  lcd.print("/15");
  
  for(int i = 0; i < 3; i++) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, HIGH);
    delay(300);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    delay(300);
  }
  
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  
  if(puntuacion >= 12) lcd.print("EXPERT!");
  else if(puntuacion >= 8) lcd.print("MOLT BE!");
  else if(puntuacion >= 5) lcd.print("BE!");
  else lcd.print("A PRACTICAR!");
  
  lcd.setCursor(0, 1);
  lcd.print("Fins la propera");
  delay(3000);
}