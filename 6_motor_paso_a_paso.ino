// Definir pines para los leds del motor 1
#define LED_A_M1 2
#define LED_B_M1 3
#define LED_C_M1 4
#define LED_D_M1 5

// Definir pines para los leds del motor 2
#define LED_A_M2 6
#define LED_B_M2 7
#define LED_C_M2 8
#define LED_D_M2 9

// Variables para almacenar parámetros recibidos por Serial
int vueltasM1 = 0;
int speedM1 = 0;
int vueltasM2 = 0;
int speedM2 = 0;

// Número de pasos por vuelta del motor (ajusta según el motor que utilices)
const int stepsPerRevolution = 4096;

// Secuencia de activación de las leds para motor 1 y 2
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
  // Configurar pines de los leds como salidas para motor 1
  pinMode(LED_A_M1, OUTPUT);
  pinMode(LED_B_M1, OUTPUT);
  pinMode(LED_C_M1, OUTPUT);
  pinMode(LED_D_M1, OUTPUT);

  // Configurar pines de los leds como salidas para motor 2
  pinMode(LED_A_M2, OUTPUT);
  pinMode(LED_B_M2, OUTPUT);
  pinMode(LED_C_M2, OUTPUT);
  pinMode(LED_D_M2, OUTPUT);

  // Inicializar comunicación serial
  Serial.begin(9600);
}

void loop() {
  // Revisar si hay datos disponibles en el puerto serial
  if (Serial.available() > 0) {
    // Leer la trama serial
    String input = Serial.readStringUntil('\n');
    input.trim();

    // Separar los valores recibidos en la trama
    int index1 = input.indexOf(',');
    int index2 = input.indexOf(',', index1 + 1);
    int index3 = input.indexOf(',', index2 + 1);

    // Convertir los valores a enteros
    vueltasM1 = input.substring(0, index1).toInt();
    speedM1 = input.substring(index1 + 1, index2).toInt();
    vueltasM2 = input.substring(index2 + 1, index3).toInt();
    speedM2 = input.substring(index3 + 1).toInt();

    // Calcular el número de pasos a realizar
    int stepsM1 = vueltasM1 * stepsPerRevolution;
    int stepsM2 = vueltasM2 * stepsPerRevolution;

    Serial.println(String(vueltasM1) + ", " + String(speedM1) + ", " + String(vueltasM2) + ", " + String(speedM2));

    // Mover ambos motores simultáneamente
    for (int i = 0; i < max(stepsM1, stepsM2); i++) {
      if (i < stepsM1) {
        moveMotor(LED_A_M1, LED_B_M1, LED_C_M1, LED_D_M1, speedM1);
      }
      if (i < stepsM2) {
        moveMotor(LED_A_M2, LED_B_M2, LED_C_M2, LED_D_M2, speedM2);
      }
    }
  }
}

void moveMotor(int ledA, int ledB, int ledC, int ledD, int speed) {
  static int stepIndexM1 = 0;
  static int stepIndexM2 = 0;

  // Mover motor 1
  if (ledA == LED_A_M1) {
    stepIndexM1 = (stepIndexM1 + 1) % 8;
    if (stepIndexM1 < 0) stepIndexM1 += 8;
    digitalWrite(ledA, stepSequence[stepIndexM1][0]);
    digitalWrite(ledB, stepSequence[stepIndexM1][1]);
    digitalWrite(ledC, stepSequence[stepIndexM1][2]);
    digitalWrite(ledD, stepSequence[stepIndexM1][3]);
    delayMicroseconds(speed);
  }

  // Mover motor 2
  if (ledA == LED_A_M2) {
    stepIndexM2 = (stepIndexM2 - 1) % 8;
    if (stepIndexM2 < 0) stepIndexM2 += 8;
    digitalWrite(ledA, stepSequence[stepIndexM2][0]);
    digitalWrite(ledB, stepSequence[stepIndexM2][1]);
    digitalWrite(ledC, stepSequence[stepIndexM2][2]);
    digitalWrite(ledD, stepSequence[stepIndexM2][3]);
    delayMicroseconds(speed);
  }
}
