#define LED_VERDE 2
#define LED_AMARILLO 3
#define LED_ROJO 4
#define INDUCTIVE_SENSOR 6

int counter = 0;

class LED {
  private:
    byte pin;

  public:
    LED(byte pin) {
      this->pin = pin;
    }
    void init() {
      pinMode(pin, OUTPUT);
    }
    void on() {
      digitalWrite(pin, HIGH);
    }
    void off() {
      digitalWrite(pin, LOW);
    }
};

class SENSOR {
  private:
    byte pin;

  public:
    SENSOR(byte pin) {
      this->pin = pin;
    }
    void init() {
      pinMode(pin, INPUT);
    }
    int read() {
      return digitalRead(pin);
    }
};

LED LedVerde (LED_VERDE);
LED LedAmarillo (LED_AMARILLO);
LED LedRojo (LED_ROJO);
SENSOR InductiveSensor (INDUCTIVE_SENSOR);

void setup()
{
  LedVerde.init();
  LedAmarillo.init();
  LedRojo.init();
  InductiveSensor.init();
  Serial.begin(9600);
}

void loop()
{
  if(InductiveSensor.read()==LOW){
    while(!InductiveSensor.read()){}
    Serial.println(String(counter));
    counter++;
  }
  switch(counter){
    case 0 ... 10:
      LedVerde.on();
      LedAmarillo.off();
      LedRojo.off();
      break;
    case 11 ... 19:
      LedVerde.off();
      LedAmarillo.on();
      LedRojo.off();
      break;
    case 20:
      LedVerde.off();
      LedAmarillo.off();
      LedRojo.on();
      break;
    default:
      counter = 0;
      break;
  }
}
