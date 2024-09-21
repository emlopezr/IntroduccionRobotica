#include <LiquidCrystal.h>
#include <Servo.h>

#define buttonPin A0

// Pines para los servos y el botón
const int servoPins[6] = {3, 5, 6, 9, 10, 11};
Servo servos[6];

// Pines para la pantalla LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(2, 4, 7, 8, 12, 13);

// Variables para el menú
int menuIndex = 0;
const int numMenus = 3;
int angles[6] = {0, 0, 0, 0, 0, 0}; // Valores iniciales de los ángulos
String inputBuffer = "";

void setup() {
  // Configurar servos
  for (int i = 0; i < 6; i++) {
    servos[i].attach(servoPins[i]);
    servos[i].write(angles[i]);
  }

  // Configurar la pantalla LCD
  lcd.begin(16, 2);

  // Configurar el botón
  pinMode(buttonPin, INPUT);

  // Configurar la comunicación serie
  Serial.begin(9600);

  // Enviar la trama inicial
  sendAngles();

  // Mostrar el primer menú
  displayMenu();
}

void loop() {
  // Leer el estado del botón para navegar en el menú
  if (digitalRead(buttonPin) == HIGH) {
    while(digitalRead(buttonPin)) {}
    menuIndex = (menuIndex + 1) % numMenus;
    displayMenu();
  }

  // Leer ángulos continuamente
  if (Serial.available() > 0) {
    char inChar = Serial.read();
    if (inChar == '-') {
      processAngles();
      displayMenu();  // Actualiza la pantalla con los nuevos valores
    } else {
      inputBuffer += inChar;
    }
  }
}

// Función para enviar los ángulos como una trama
void sendAngles() {
  String trama = "";
  for (int i = 0; i < 6; i++) {
    trama += String(angles[i]);
    if (i < 5) {
      trama += ",";
    }
  }
  trama += "-";
  Serial.println(trama);
}

// Función para mostrar el menú en la LCD
void displayMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SERVOMOTORES");

  switch (menuIndex) {
    case 0:
      lcd.setCursor(0, 1);
      lcd.print("S1:" + String(angles[0]) + "  S2:" + String(angles[1]));
      break;
    case 1:
      lcd.setCursor(0, 1);
      lcd.print("S3:" + String(angles[2]) + "  S4:" + String(angles[3]));
      break;
    case 2:
      lcd.setCursor(0, 1);
      lcd.print("S5:" + String(angles[4]) + "  S6:" + String(angles[5]));
      break;
  }
}

// Función para procesar la trama recibida y actualizar los ángulos
void processAngles() {
  int index = 0;
  int start = 0;
  int end = inputBuffer.indexOf(',');

  while (end != -1 && index < 6) {
    angles[index] = inputBuffer.substring(start, end).toInt();
    start = end + 1;
    end = inputBuffer.indexOf(',', start);
    index++;
  }

  angles[index] = inputBuffer.substring(start).toInt(); // último ángulo

  // Actualizar los servos con los nuevos ángulos
  for (int i = 0; i < 6; i++) {
    servos[i].write(angles[i]);
  }

  // Limpiar el buffer para la siguiente lectura
  inputBuffer = "";

  // Enviar la nueva trama de ángulos
  sendAngles();
}