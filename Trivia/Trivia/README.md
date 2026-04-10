# 🧠 Arduino Quiz Master v2.0

¡Un sistema de trivia interactivo y portátil! Este proyecto convierte tu **Arduino Uno** en una consola de juegos de cultura general con 15 preguntas dinámicas, navegación por joystick y una interfaz optimizada para pantallas LCD.

## 📝 Descripción
Arduino Quiz Master es un juego de preguntas y respuestas que desafía tus conocimientos en diversas áreas. El proyecto destaca por su eficiencia técnica, utilizando la memoria **Flash (PROGMEM)** para almacenar gran cantidad de texto, permitiendo que un microcontrolador pequeño maneje un juego de larga duración sin colapsar la RAM.

## 🎮 Cómo Jugar
1. **Inicio:** Al encender el sistema, verás una pantalla de bienvenida. Pulsa el **botón del joystick** para comenzar la partida.
2. **Navegación:** Utiliza el joystick analógico para moverte:
   - **Izquierda/Derecha:** Cambia entre opciones en la misma fila (A ↔ B o C ↔ D).
   - **Arriba/Abajo:** Cambia entre filas (A/B ↔ C/D).
3. **Selección:** El cursor `>` indica tu posición actual. Cuando estés sobre la respuesta que crees correcta, **presiona el botón del joystick**.
4. **Feedback:** - Si aciertas, los LEDs parpadearán y verás un mensaje de éxito con una breve explicación.
   - Si fallas, el sistema te mostrará cuál era la respuesta correcta.
5. **Puntuación:** Al final de las 15 preguntas, recibirás tu rango basado en tus aciertos (¡desde "A practicar" hasta "Expert"!").

## 🔌 Esquema de Conexiones (Hardware)

| Componente | Pin Arduino | Función |
| :--- | :--- | :--- |
| **LCD 16x2 (I2C) SDA** | A4 | Datos I2C |
| **LCD 16x2 (I2C) SCL** | A5 | Reloj I2C |
| **Joystick Eje X** | A0 | Movimiento Horizontal |
| **Joystick Eje Y** | A1 | Movimiento Vertical |
| **Joystick SW (Boton)** | D12 | Selección / Inicio |
| **LED 1 (Opción A)** | D8 | Indicador visual de selección |
| **LED 2 (Opción B)** | D9 | Indicador visual de selección |
| **LED 3 (Opción C)** | D10 | Indicador visual de selección |
| **LED 4 (Opción D)** | D11 | Indicador visual de selección |

*Nota: El botón del joystick utiliza la resistencia interna `INPUT_PULLUP`, por lo que no requiere resistencia externa.*

## 🚀 Características Técnicas

* **Optimización PROGMEM:** Los títulos, opciones y explicaciones se almacenan en la memoria de programa (Flash), liberando la SRAM para un funcionamiento fluido.
* **Sistema Anti-Flicker:** El renderizado de la pantalla solo actualiza los caracteres necesarios cuando hay un cambio de estado, evitando el parpadeo de la pantalla LCD.
* **Control Preciso:** Implementación de un sistema de "debounce" y gestión de tiempos con `millis()` para una respuesta del joystick suave y sin dobles pulsaciones.
* **Lógica de Mapeado:** Conversión de valores analógicos del joystick a una cuadrícula de 2x2 para una selección de respuestas intuitiva.

## 🛠️ Instalación
1. Clona este repositorio o copia el código en tu **Arduino IDE**.
2. Instala la librería `LiquidCrystal_I2C` desde el gestor de librerías.
3. Conecta los componentes según la tabla de conexiones.
4. Carga el código y abre el monitor serie (opcional) para depuración.

---
**Proyecto desarrollado para fines educativos y de entretenimiento.** ¡Siéntete libre de añadir tus propias preguntas modificando los arrays en el código!
