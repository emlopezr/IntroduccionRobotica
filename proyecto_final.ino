#define in1 2
#define in2 4

#define in3 7
#define in4 8

#define enA 3
#define enB 5

#define echoPinForward 10
#define triggerPinForward 9

#define echoPinRight 12
#define triggerPinRight 11

#define echoPinLeft 6
#define triggerPinLeft 13

#define alpha 0.2

const int DISTANCIA_SEGURIDAD = 40;
const int VELOCIDAD = 150;

int mode = 0;

int speed = 0;
int direction = 0;
int speedInput = 0;

uint32_t duration = 0;
float speedSensor = 0.017;
float distance = 0.0;

float distanceForward;
float distanceRight;
float distanceLeft;

void setup() {
  pinMode(triggerPinForward, OUTPUT);
  pinMode(echoPinForward, INPUT);

  pinMode(triggerPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);

  pinMode(triggerPinLeft, OUTPUT);
  pinMode(echoPinLeft, INPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  detener();

  Serial.begin(9600);
}

void loop() {
  // Verificar si hay datos disponibles en el puerto serie
  mode = 1;

  if (Serial.available() > 0 && mode == 0) {
    // Leer la trama de dirección y velocidad
    String trama = Serial.readStringUntil('\n');
    
    // Parsear la trama (formato: [direction,speedInput])
    int commaIndex = trama.indexOf(',');
    if (commaIndex > 0) {
      direction = trama.substring(0, commaIndex).toInt();        // Dirección
      speedInput = trama.substring(commaIndex + 1).toInt(); // Velocidad en porcentaje
      
      // Convertir el porcentaje de velocidad a un valor PWM (0-255)
      speed = map(speedInput, 0, 100, 0, 255);
      
      // Ejecutar el movimiento según la dirección
      moverRobot(direction, speed);
    }
  }

  distanceForward = medirDistancia(triggerPinForward, echoPinForward, distanceForward);
  distanceRight = medirDistancia(triggerPinRight, echoPinRight, distanceRight);
  distanceLeft = medirDistancia(triggerPinLeft, echoPinLeft, distanceLeft);

  // Evaluar condiciones
  if (distanceForward >= DISTANCIA_SEGURIDAD) {
    if (distanceRight >= DISTANCIA_SEGURIDAD && distanceLeft >= DISTANCIA_SEGURIDAD) {
      // Caso 1: Todo despejado
      adelante(VELOCIDAD);
    } else if (distanceRight < DISTANCIA_SEGURIDAD && distanceLeft >= DISTANCIA_SEGURIDAD) {
      // Caso 6: Obstáculo a la derecha
      izquierda(VELOCIDAD);
    } else if (distanceRight >= DISTANCIA_SEGURIDAD && distanceLeft < DISTANCIA_SEGURIDAD) {
      // Caso 7: Obstáculo a la izquierda
      derecha(VELOCIDAD);
    } else if (distanceRight < DISTANCIA_SEGURIDAD && distanceLeft < DISTANCIA_SEGURIDAD) {
      // Caso 8: Obstáculos a ambos lados
      detener();
    }
  } else { // distanceForward < DISTANCIA_SEGURIDAD
    if (distanceRight >= DISTANCIA_SEGURIDAD && distanceLeft >= DISTANCIA_SEGURIDAD) {
      // Caso 2: Obstáculo adelante, elegir dirección basada en mayor distancia
      if (distanceRight > distanceLeft) {
        derecha(VELOCIDAD);
      } else {
        izquierda(VELOCIDAD);
      }
    } else if (distanceRight < DISTANCIA_SEGURIDAD && distanceLeft >= DISTANCIA_SEGURIDAD) {
      // Caso 3: Obstáculo adelante y a la derecha
      izquierda(VELOCIDAD);
    } else if (distanceRight >= DISTANCIA_SEGURIDAD && distanceLeft < DISTANCIA_SEGURIDAD) {
      // Caso 4: Obstáculo adelante y a la izquierda
      derecha(VELOCIDAD);
    }
    else if (distanceRight < DISTANCIA_SEGURIDAD && distanceLeft < DISTANCIA_SEGURIDAD) {
      // Caso 5: Obstáculo adelante y en ambos lados
      detener();
      // Opcional: Retroceder
      atras(VELOCIDAD);
      delay(500); // Tiempo para retroceder
      // Luego, elegir una dirección para girar después de retroceder
      if (distanceRight > distanceLeft) {
        derecha(VELOCIDAD);
      } else {
        izquierda(VELOCIDAD);
      }
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
void atras(int velocidad) {
  analogWrite(enA, velocidad);  // Motores del lado izquierdo
  analogWrite(enB, velocidad);  // Motores del lado derecho
  digitalWrite(in1, HIGH);      // Adelante motor izquierdo
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);      // Adelante motor derecho
  digitalWrite(in4, HIGH);
}

void adelante(int velocidad) {
  analogWrite(enA, velocidad);
  analogWrite(enB, velocidad);
  digitalWrite(in1, LOW);       // Atrás motor izquierdo
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);       // Atrás motor derecho
  digitalWrite(in4, LOW);
}

void izquierda(int velocidad) {
  analogWrite(enA, velocidad);  // Motores del lado izquierdo giran
  analogWrite(enB, velocidad);          // Motores del lado derecho detenidos
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

void derecha(int velocidad) {
  analogWrite(enA, velocidad);          // Motores del lado izquierdo detenidos
  analogWrite(enB, velocidad);  // Motores del lado derecho giran
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}

void detener() {
  analogWrite(enA, 0);          // Apagar motores
  analogWrite(enB, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

float medirDistancia(uint8_t triggerPin, uint8_t echoPin, float st_1) {
  // Clean
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(1);

  // Trigger
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Get distance
  duration = pulseIn(echoPin, HIGH);
  distance = duration * speedSensor;

  return (alpha * distance) + ( (1.0-alpha) * st_1);
}
