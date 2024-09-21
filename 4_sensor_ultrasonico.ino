#define triggerPin1 2
#define echoPin1 3
#define triggerPin2 5
#define echoPin2 6
#define alpha 0.2

uint32_t duration = 0;
float speed = 0.017; // Velocidad del sonido/2 en cm/mseg
float distance = 0.0;
float measureDistance1;
float measureDistance2;

void setup() {
  pinMode(triggerPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(triggerPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  Serial.begin(9600);
}

void loop() {
  measureDistance1 = getMesasureDistance(triggerPin1, echoPin1, measureDistance1);
  measureDistance2 = getMesasureDistance(triggerPin2, echoPin2, measureDistance2);
  Serial.println(String(distance) + "," + String(measureDistance2));
}

float getMesasureDistance(uint8_t triggerPin, uint8_t echoPin, float st_1) {
  // Clean
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(1);

  // Trigger
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Get distance
  duration = pulseIn(echoPin, HIGH);
  distance = duration * speed;
  return (alpha*distance) + ((1.0-alpha)*st_1);
}
