#include <LiquidCrystal.h>

#define LED_A_M1 8
#define LED_B_M1 9
#define LED_C_M1 10
#define LED_D_M1 11

#define LED_A_M2 2
#define LED_B_M2 3
#define LED_C_M2 4
#define LED_D_M2 5

#define SPEED 500

int angleM1 = 0;
int angleM2 = 0;

// Pines para la pantalla LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);

// Número de pasos por vuelta del motor
const int stepsPerRevolution = 4096;
const double stepsPerGrade = stepsPerRevolution / 360;

// Secuencia de activación de las bobinas para motor 1 y 2
int stepSequence[8][4] = {
  {HIGH, LOW,  LOW,  LOW },  // A
  {HIGH, HIGH, LOW,  LOW },  // AB
  {LOW,  HIGH, LOW,  LOW },  // B
  {LOW,  HIGH, HIGH, LOW },  // BC
  {LOW,  LOW,  HIGH, LOW },  // C
  {LOW,  LOW,  HIGH, HIGH},  // CD
  {LOW,  LOW,  LOW,  HIGH},  // D
  {HIGH, LOW,  LOW,  HIGH}   // DA
};

void setup() {
  pinMode(LED_A_M1, OUTPUT);
  pinMode(LED_B_M1, OUTPUT);
  pinMode(LED_C_M1, OUTPUT);
  pinMode(LED_D_M1, OUTPUT);

  pinMode(LED_A_M2, OUTPUT);
  pinMode(LED_B_M2, OUTPUT);
  pinMode(LED_C_M2, OUTPUT);
  pinMode(LED_D_M2, OUTPUT);

  // Configurar la pantalla LCD
  lcd.begin(16, 2);

  // Configurar la comunicación serie
  Serial.begin(9600);

  // Mostrar el primer menú
  displayMenu();
}

void loop() {
  // Revisar si hay datos disponibles en el puerto serial
  if (Serial.available() > 0) {
    // Leer la trama serial
    String input = Serial.readStringUntil('\n');
    input.trim();

    // Separar los valores recibidos en la trama
    int index = input.indexOf(',');

    // Convertir los valores a enteros
    angleM1 = input.substring(0, index).toInt();
    angleM2 = input.substring(index + 1).toInt();

    if (angleM1 >= 180) {
      angleM1 = 180;
    }
    if (angleM2 >= 180) {
      angleM2 = 180;
    }

    // Calcular el número de pasos a realizar
    int stepsM1 = angleM1 * stepsPerGrade;
    int stepsM2 = angleM2 * stepsPerGrade;

    // Display
    displayMenu();

    // Mover ambos motores simultáneamente
    moveMotors(stepsM1, stepsM2);
  }
}

void moveMotors(int stepsM1, int stepsM2) {
  int currentStep = 0;

  bool finishedM1 = false;
  bool finishedM2 = false;

  int index1 = 0;
  int index2 = 0;

  /*
  while (stepCounterM1 < abs(stepsM1) && stepCounterM2 < abs(stepsM2)) {
    if (stepCounterM1 < abs(stepsM1)) {
      stepMotor(LED_A_M1, LED_B_M1, LED_C_M1, LED_D_M1, &stepIndexM1, stepsM1 > 0);
      stepCounterM1++;
      delayMicroseconds(SPEED);
    }
    if (stepCounterM2 < abs(stepsM2)) {
      stepMotor(LED_A_M2, LED_B_M2, LED_C_M2, LED_D_M2, &stepIndexM2, stepsM2 > 0);
      stepCounterM2++;
      delayMicroseconds(SPEED);
    }
  }
  */

  while(true) {
    if (currentStep >= stepsM1 && !finishedM1) {
      finishedM1 = true;
    }
    if (currentStep >= stepsM2  && !finishedM2) {
      finishedM2 = true;
    }

    if (!finishedM1) {
      stepMotorM1(index1, finishedM2);
      index1++;
      if (index1 > 7) {
        index1 = 0;
      }
    }

    if (!finishedM2) {
      stepMotorM2(index2, finishedM1);
      index2++;
      if (index2 > 7) {
        index2 = 0;
      }
    }

    currentStep++;

    if(finishedM1 && finishedM2) {
      break;
    }
  }
}

void stepMotorM1(int index, bool otherFinished) {
  digitalWrite(LED_A_M1, stepSequence[index][0]);
  digitalWrite(LED_B_M1, stepSequence[index][1]);
  digitalWrite(LED_C_M1, stepSequence[index][2]);
  digitalWrite(LED_D_M1, stepSequence[index][3]);

  if (otherFinished) {
    delayMicroseconds(SPEED * 2);
    return;
  }

  delayMicroseconds(SPEED);
}

void stepMotorM2(int index, bool otherFinished) {
  digitalWrite(LED_A_M2, stepSequence[index][0]);
  digitalWrite(LED_B_M2, stepSequence[index][1]);
  digitalWrite(LED_C_M2, stepSequence[index][2]);
  digitalWrite(LED_D_M2, stepSequence[index][3]);

  if (otherFinished) {
    delayMicroseconds(SPEED * 2);
    return;
  }

  delayMicroseconds(SPEED);
}

void displayMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ANGULOS");

  lcd.setCursor(0, 1);
  lcd.print("M1:" + String(angleM1) + "  M2:" + String(angleM2));
}