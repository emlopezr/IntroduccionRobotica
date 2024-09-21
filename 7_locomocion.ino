#define in1 2
#define in2 4
#define in3 7
#define in4 8
#define enA 3
#define enB 5

// Variables para almacenar la dirección y la velocidad
int direction = 0;
int speedPercentage = 0;
int speedPWM = 0;

void setup() {
  // Definir los pines como salidas
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  // Inicializar todo apagado
  detener();

  // Inicializar comunicación serie
  Serial.begin(9600);
}

void loop() {
  // Verificar si hay datos disponibles en el puerto serie
  if (Serial.available() > 0) {
    // Leer la trama de dirección y velocidad
    String trama = Serial.readStringUntil('\n');
    
    // Parsear la trama (formato: [direction,SpeedPercentage])
    int commaIndex = trama.indexOf(',');
    if (commaIndex > 0) {
      direction = trama.substring(0, commaIndex).toInt();        // Dirección
      speedPercentage = trama.substring(commaIndex + 1).toInt(); // Velocidad en porcentaje
      
      // Convertir el porcentaje de velocidad a un valor PWM (0-255)
      speedPWM = map(speedPercentage, 0, 100, 0, 255);
      
      // Ejecutar el movimiento según la dirección
      moverRobot(direction, speedPWM);
    }
  }
}

// Función para controlar el movimiento del robot
void moverRobot(int dir, int velocidad) {
  switch (dir) {
    case 0: // Detener el robot
      detener();
      break;
    case 1: // Avanzar
      adelante(velocidad);
      break;
    case 2: // Retroceder
      atras(velocidad);
      break;
    case 3: // Girar a la derecha
      derecha(velocidad);
      break;
    case 4: // Girar a la izquierda
      izquierda(velocidad);
      break;
    default:
      detener();  // Detener si no se reconoce la dirección
      break;
  }
}

// Funciones para controlar las direcciones
void adelante(int velocidad) {
  analogWrite(enA, velocidad);  // Motores del lado izquierdo
  analogWrite(enB, velocidad);  // Motores del lado derecho
  digitalWrite(in1, HIGH);      // Adelante motor izquierdo
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);      // Adelante motor derecho
  digitalWrite(in4, HIGH);
}

void atras(int velocidad) {
  analogWrite(enA, velocidad);
  analogWrite(enB, velocidad);
  digitalWrite(in1, LOW);       // Atrás motor izquierdo
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);       // Atrás motor derecho
  digitalWrite(in4, LOW);
}

void derecha(int velocidad) {
  analogWrite(enA, velocidad);  // Motores del lado izquierdo giran
  analogWrite(enB, velocidad);          // Motores del lado derecho detenidos
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void izquierda(int velocidad) {
  analogWrite(enA, velocidad);          // Motores del lado izquierdo detenidos
  analogWrite(enB, velocidad);  // Motores del lado derecho giran
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void detener() {
  analogWrite(enA, 0);          // Apagar motores
  analogWrite(enB, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
