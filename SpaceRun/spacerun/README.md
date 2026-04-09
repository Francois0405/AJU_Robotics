# 🚀 Arduino LCD Space Runner

¡Un adictivo minijuego arcade de supervivencia espacial construido completamente desde cero para Arduino UNO! Esquiva asteroides, destruye enemigos con tu láser y compite por el Highscore al ritmo de una banda sonora épica.

## ✨ Funcionalidades Principales

* **Motor sin Lag (`millis()`):** El juego procesa los inputs del joystick y el botón de forma instantánea, sin bloqueos ni retrasos.
* **Renderizado Fluido:** Sistema de sobrescritura de pantalla que elimina el molesto parpadeo (flickering) típico de las pantallas LCD I2C.
* **Banda Sonora y Efectos (Multitarea):** Reproduce música de fondo dinámica que se acelera con la dificultad, sin interrumpir la acción. Incluye efectos de sonido para disparos y colisiones.
* **Guardado Permanente (EEPROM):** Tu récord máximo (Highscore) se guarda de forma segura en la memoria interna del Arduino. ¡No se borra al apagar la consola!
* **Sistema de Dificultad Dinámico:** 4 modos de juego (EASY, MED, HARD, INSANE) que ajustan la velocidad del juego, el tempo de la música y aplican multiplicadores de puntuación (de x1.0 hasta x1.3).
* **Formateo Inteligente:** Los puntos superiores a 10.000 se comprimen automáticamente (ej. `12k`) para aprovechar al máximo los 16 caracteres de la pantalla.

## 🛠️ Material Necesario

* 1x Placa Arduino UNO (o compatible)
* 1x Pantalla LCD 16x2 con módulo I2C
* 1x Joystick Analógico
* 1x Pulsador (Botón) + Resistencia (si no usas módulo)
* 1x Zumbador Pasivo (Passive Buzzer) - *Importante que sea pasivo para que suene la música.*
* Cables Jumpers y Protoboard

## 🔌 Esquema de Conexiones

Conecta los componentes a tu Arduino UNO siguiendo esta tabla:

| Componente | Pin del Componente | Pin del Arduino |
| :--- | :--- | :--- |
| **Pantalla LCD I2C** | SDA | A4 (SDA) |
| | SCL | A5 (SCL) |
| | VCC / GND | 5V / GND |
| **Joystick** | VRy (Eje Y) | A0 |
| | VCC / GND | 5V / GND |
| **Botón (Disparo)** | Pin de señal | D7 |
| | GND | GND |
| **Zumbador Pasivo** | Positivo (+) | D8 |
| | Negativo (-) | GND |
| **Random Seed** | (Nada conectado) | A1 (Dejar libre para entropía) |

> **Nota sobre el botón:** El pin 7 utiliza la resistencia interna del Arduino (`INPUT_PULLUP`), por lo que el botón debe conectar el pin 7 a GND al ser pulsado.

## 🎮 Cómo Jugar

1.  **Menú Principal:** Usa el joystick (Arriba/Abajo) para seleccionar la dificultad. Pulsa el botón de disparo para comenzar.
2.  **Movimiento:** Mueve el joystick hacia arriba o hacia abajo para cambiar tu nave (`>`) de carril.
3.  **Obstáculos:**
    * `#` **Asteroides:** Son indestructibles. ¡Tienes que esquivarlos!
    * `O` **Enemigos:** Puedes esquivarlos o pulsar tu botón para **disparar el láser**. Si destruyes uno, ganas puntos extra.
4.  **Puntuación:** Ganas puntos por sobrevivir a cada ciclo y un gran bonus por destruir enemigos (`O`). Las dificultades más altas aplican un multiplicador a tus puntos. ¡Intenta superar el HI-SCORE!

## ⚙️ Instalación y Configuración

1.  Clona o descarga este repositorio.
2.  Abre el código en el **Arduino IDE**.
3.  Asegúrate de tener instalada la librería `LiquidCrystal_I2C`. (Puedes buscarla en el Gestor de Librerías del IDE o descargarla de GitHub).
4.  La dirección I2C por defecto en el código es `0x27`. Si tu pantalla no muestra nada, comprueba si tu dirección es `0x3F` y cámbiala en la línea:
    ```cpp
    LiquidCrystal_I2C lcd(0x27, 16, 2); 
    ```
5.  ¡Sube el código a tu placa y a jugar!

## 📜 Licencia
Este proyecto es de código abierto. Siéntete libre de modificarlo, crear tus propios gráficos personalizados o añadir nuevos niveles. ¡El espacio es el límite!