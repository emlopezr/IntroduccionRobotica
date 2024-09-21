#include <LiquidCrystal.h>

//Código sensores inductivo, capacitivo y pantalla LCD:
#define LED_AMARILLO 2
#define LED_AZUL 3
#define LED_ROJO 4
#define INDUCTIVE_SENSOR 6
#define CAPACITIVE_SENSOR 7
#define Rs 8
#define E 9
#define D4 10
#define D5 11
#define D6 12
#define D7 13
int counter = 0;
/* 
Vss = GND
VDD = +5V
VO = 220 Ohm + GND
Rs = [Pin]
Rw = GND
E = [Pin]
D0..D7 = [Pin]
A = +5V
K = 220 Ohm + GND

void setup(){
  lcd.begin(16,2); // [Columnas, Filas]
  lcd.setCursor(0,0);
  lcd.print("Hola");
  lcd.print("Contador: "+String(counter));
}
*/
LiquidCrystal lcd(Rs,E,D4,D5,D6,D7);

class LED {
  private:
    byte pin;

   public:
    LED(byte pin){
      this->pin = pin;
    }
    void init(){
      pinMode(pin,OUTPUT);
    }
    void on(){
      digitalWrite(pin,HIGH);
    }
    void off(){
      digitalWrite(pin,LOW);
    }
};

class SENSOR {
  private:
    byte pin;

   public:
    SENSOR(byte pin){
      this->pin = pin;
    }
    void init(){
      pinMode(pin,INPUT);
    }
    int read(){
      return digitalRead(pin);
    }
};

LED LedAmarillo (LED_AMARILLO);
LED LedAzul (LED_AZUL);
LED LedRojo (LED_ROJO);
SENSOR InductiveSensor (INDUCTIVE_SENSOR);
SENSOR CapacitiveSensor (CAPACITIVE_SENSOR);

void setup(){
  LedAmarillo.init();
  LedAzul.init();
  LedRojo.init();
  InductiveSensor.init();
  CapacitiveSensor.init();
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop(){
  if(InductiveSensor.read()==LOW){
    while(!InductiveSensor.read()){}
    if (counter < 20){
      counter++;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("S:");
    lcd.setCursor(0, 1);
    lcd.print("Count:");
    lcd.setCursor(3, 0);
    lcd.print("Inductive");
    Serial.println(String(counter));
    lcd.setCursor(7, 1);
    lcd.print(String(counter));
  }

  if(CapacitiveSensor.read()==HIGH){
    while(CapacitiveSensor.read()){}
    if(counter > 0){
      counter--;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("S:");
    lcd.setCursor(0, 1);
    lcd.print("Count:");
    lcd.setCursor(3, 0);
    lcd.print("Capacitive");
    Serial.println(String(counter));
    lcd.setCursor(7, 1);
    lcd.print(String(counter));
  }

  switch(counter){
    case 0 ... 10: 
      LedAmarillo.on();
      LedAzul.off();
      LedRojo.off();
      break;
    case 11 ... 19:
      LedAmarillo.off();
      LedAzul.on();
      LedRojo.off();
      break;
    case 20:
      LedAmarillo.off();
      LedAzul.off();
      LedRojo.on();
      break;
  }
}