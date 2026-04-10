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

int valorX, valorY, estadoBoton;
int Preguntas = 0;
int puntuacion = 0;

int opcionSeleccionada = 0;
int ultimaOpcion = -1; // Para evitar parpadeo
unsigned long ultimoMovimiento = 0;
int delayMovimiento = 200;

// ===== STRINGS EN PROGMEM =====
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

// Opciones Incorrectas (3 por pregunta)
const char op00[] PROGMEM = "Barna";  const char op01[] PROGMEM = "Sevilla"; const char op02[] PROGMEM = "Girona";
const char op10[] PROGMEM = "Lyon";   const char op11[] PROGMEM = "Marsella"; const char op12[] PROGMEM = "Tolosa";
const char op20[] PROGMEM = "Venecia";const char op21[] PROGMEM = "Napols";   const char op22[] PROGMEM = "Milan";
const char op30[] PROGMEM = "Porto";  const char op31[] PROGMEM = "Braga";    const char op32[] PROGMEM = "Faro";
const char op40[] PROGMEM = "3";      const char op41[] PROGMEM = "5";        const char op42[] PROGMEM = "6";
const char op50[] PROGMEM = "20";     const char op51[] PROGMEM = "30";       const char op52[] PROGMEM = "35";
const char op60[] PROGMEM = "3";      const char op61[] PROGMEM = "4";        const char op62[] PROGMEM = "6";
const char op70[] PROGMEM = "6";      const char op71[] PROGMEM = "7";        const char op72[] PROGMEM = "9";
const char op80[] PROGMEM = "Roig";   const char op81[] PROGMEM = "Verd";     const char op82[] PROGMEM = "Groc";
const char op90[] PROGMEM = "Venus";  const char op91[] PROGMEM = "Mart";     const char op92[] PROGMEM = "Saturn";
const char op100[] PROGMEM = "360";  const char op101[] PROGMEM = "366";     const char op102[] PROGMEM = "364";
const char op110[] PROGMEM = "1500"; const char op111[] PROGMEM = "1488";    const char op112[] PROGMEM = "1510";
const char op120[] PROGMEM = "1965";  const char op121[] PROGMEM = "1972";    const char op122[] PROGMEM = "1963";
const char op130[] PROGMEM = "Dali";   const char op131[] PROGMEM = "Goya";     const char op132[] PROGMEM = "Velaz";
const char op140[] PROGMEM = "Dolar";  const char op141[] PROGMEM = "Lliura";   const char op142[] PROGMEM = "Yen";

const char* const opcionesIncorrectas[15][3] PROGMEM = {
  {op00, op01, op02}, {op10, op11, op12}, {op20, op21, op22}, {op30, op31, op32}, {op40, op41, op42},
  {op50, op51, op52}, {op60, op61, op62}, {op70, op71, op72}, {op80, op81, op82}, {op90, op91, op92},
  {op100, op101, op102}, {op110, op111, op112}, {op120, op121, op122}, {op130, op131, op132}, {op140, op141, op142}
};

const char letrasCorrectas[] = {'C', 'B', 'C', 'D', 'B', 'B', 'C', 'C', 'B', 'C', 'B', 'A', 'B', 'B', 'B'};

const char correcta0[] PROGMEM = "Madrid";  const char correcta1[] PROGMEM = "Paris";   const char correcta2[] PROGMEM = "Roma";
const char correcta3[] PROGMEM = "Lisboa";  const char correcta4[] PROGMEM = "4";       const char correcta5[] PROGMEM = "25";
const char correcta6[] PROGMEM = "5";       const char correcta7[] PROGMEM = "8";       const char correcta8[] PROGMEM = "Blau";
const char correcta9[] PROGMEM = "Jupiter"; const char correcta10[] PROGMEM = "365";     const char correcta11[] PROGMEM = "1492";
const char correcta12[] PROGMEM = "1969";   const char correcta13[] PROGMEM = "Picasso"; const char correcta14[] PROGMEM = "Euro";

const char* const textosCorrectos[] PROGMEM = {
  correcta0, correcta1, correcta2, correcta3, correcta4, correcta5, correcta6, correcta7, correcta8, correcta9,
  correcta10, correcta11, correcta12, correcta13, correcta14
};

const char exp0[] PROGMEM = "Madrid capital"; const char exp1[] PROGMEM = "Paris capital"; const char exp2[] PROGMEM = "Roma capital";
const char exp3[] PROGMEM = "Lisboa capital"; const char exp4[] PROGMEM = "2+2=4";          const char exp5[] PROGMEM = "5x5=25";
const char exp6[] PROGMEM = "10/2=5";         const char exp7[] PROGMEM = "15-7=8";         const char exp8[] PROGMEM = "Cel blau";
const char exp9[] PROGMEM = "Jupiter gran";   const char exp10[] PROGMEM = "365 dies";     const char exp11[] PROGMEM = "1492 America";
const char exp12[] PROGMEM = "1969 Apollo";   const char exp13[] PROGMEM = "Picasso pinto";const char exp14[] PROGMEM = "Euro moneda";

const char* const explicaciones[] PROGMEM = {
  exp0, exp1, exp2, exp3, exp4, exp5, exp6, exp7, exp8, exp9, exp10, exp11, exp12, exp13, exp14
};

char buffer[21];

void setup() {
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(pinBoton, INPUT_PULLUP);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0); lcd.print("Quiz Game v2.0");
  lcd.setCursor(0, 1); lcd.print("15 Preguntes!");
  delay(2000);
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("PREPARAT?");
  lcd.setCursor(0, 1); lcd.print("Prem el boto...");
  while(digitalRead(pinBoton) == HIGH);
  
  lcd.clear();
  puntuacion = 0;
  for(Preguntas = 0; Preguntas < 15; Preguntas++) {
    mostrarPregunta(Preguntas);
    char res = seleccionarRespuesta(Preguntas);
    comprobarRespuesta(Preguntas, res);
  }
  mostrarResultados();
}

void cargarBuffer(const char* ptr) {
  strcpy_P(buffer, (char*)pgm_read_word(ptr));
}

void mostrarPregunta(int num) {
  lcd.clear();
  cargarBuffer(&titulos[num]);
  lcd.setCursor(0, 0); lcd.print(buffer);
  delay(1200);
  lcd.clear();

  char correcta = letrasCorrectas[num];
  const char letters[] = {'A', 'B', 'C', 'D'};
  int incorrectIdx = 0;

  for (int i = 0; i < 4; i++) {
    lcd.setCursor((i % 2) * 8, i / 2);
    lcd.print(letters[i]); lcd.print(":");
    
    if (letters[i] == correcta) {
      cargarBuffer(&textosCorrectos[num]);
    } else {
      strcpy_P(buffer, (char*)pgm_read_word(&(opcionesIncorrectas[num][incorrectIdx++])));
    }
    buffer[5] = '\0'; // Truncar para que no solape
    lcd.print(buffer);
  }
}

char seleccionarRespuesta(int numPregunta) {
  opcionSeleccionada = 0;
  ultimaOpcion = -1;
  while(true) {
    valorX = analogRead(pinX);
    valorY = analogRead(pinY);
    
    if(millis() - ultimoMovimiento > delayMovimiento) {
      if(valorX < 300) { if(opcionSeleccionada % 2 == 1) opcionSeleccionada--; ultimoMovimiento = millis(); }
      if(valorX > 700) { if(opcionSeleccionada % 2 == 0) opcionSeleccionada++; ultimoMovimiento = millis(); }
      if(valorY < 300) { if(opcionSeleccionada > 1) opcionSeleccionada -= 2; ultimoMovimiento = millis(); }
      if(valorY > 700) { if(opcionSeleccionada < 2) opcionSeleccionada += 2; ultimoMovimiento = millis(); }
    }

    if (opcionSeleccionada != ultimaOpcion) {
      actualizarLEDsPorOpcion(opcionSeleccionada);
      mostrarCursor(opcionSeleccionada);
      ultimaOpcion = opcionSeleccionada;
    }

    if(digitalRead(pinBoton) == LOW) {
      delay(200);
      return "ABCD"[opcionSeleccionada];
    }
    delay(10);
  }
}

void mostrarCursor(int opcion) {
  const int coords[4][2] = {{0,0}, {8,0}, {0,1}, {8,1}};
  for(int i=0; i<4; i++) {
    lcd.setCursor(coords[i][0], coords[i][1]);
    lcd.print(i == opcion ? ">" : " ");
  }
}

void actualizarLEDsPorOpcion(int opcion) {
  digitalWrite(IN1, opcion == 0);
  digitalWrite(IN2, opcion == 1);
  digitalWrite(IN3, opcion == 2);
  digitalWrite(IN4, opcion == 3);
}

void comprobarRespuesta(int num, char res) {
  lcd.clear();
  lcd.print("Triat: "); lcd.print(res);
  lcd.setCursor(0,1);
  if(res == letrasCorrectas[num]) {
    lcd.print("CORRECTE!"); puntuacion++;
    delay(1000);
    lcd.clear();
    cargarBuffer(&explicaciones[num]);
    lcd.print(buffer);
  } else {
    lcd.print("ERROR!");
    delay(1000);
    lcd.clear();
    lcd.print("Era: ");
    cargarBuffer(&textosCorrectos[num]);
    lcd.print(buffer);
  }
  delay(2000);
}

void mostrarResultados() {
  lcd.clear();
  lcd.print("FI! Punts: "); lcd.print(puntuacion); lcd.print("/15");
  for(int i=0; i<6; i++) {
    digitalWrite(IN1, i%2); digitalWrite(IN2, i%2); 
    digitalWrite(IN3, i%2); digitalWrite(IN4, i%2);
    delay(200);
  }
  delay(3000);
}
