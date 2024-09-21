#include <LiquidCrystal.h>

//Código canales y pantalla LCD:
#define ChA 3
#define ChB 2
#define Rs 8
#define E 9
#define D4 10
#define D5 11
#define D6 12
#define D7 13

int ant = 0;
int act = 0;
int n = 0;
int n0 = 0;
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
*/
LiquidCrystal lcd(Rs,E,D4,D5,D6,D7);

void setup(){
  pinMode(ChA, INPUT);
  pinMode(ChB, INPUT);
  attachInterrupt(digitalPinToInterrupt(ChA), Encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ChB), Encoder, CHANGE);
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop(){
  lcd.setCursor(0, 0);
  lcd.print("ENCODER");
  lcd.setCursor(0, 1);
  lcd.print("Contador=");
  lcd.setCursor(9, 1);
  lcd.print(String(n));
  Serial.println(String(n));
  if(n0!=n){
    lcd.clear();
  }
  n0 = n;
}

void Encoder(){
  ant = act;
  act = PIND&12;

  if(ant==0 && act==4) n++;
  if(ant==4 && act==12) n++;
  if(ant==8 && act==0) n++;
  if(ant==12 && act==8) n++;

  if(ant==0 && act==8) n--;
  if(ant==4 && act==0) n--;
  if(ant==8 && act==12) n--;
  if(ant==12 && act==4) n--;
}