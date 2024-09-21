#define BUTTON_1 2
#define LED_BLUE_1 3
#define LED_YELLOW_1 4
#define LED_RED_1 5
#define BUTTON_2 6
#define LED_BLUE_2 7
#define LED_YELLOW_2 8
#define LED_RED_2 9

int counter_1 = 0;
int counter_2 = 0;
bool active = 1;

void setup()
{
  pinMode(BUTTON_1, INPUT);
  pinMode(LED_BLUE_1, OUTPUT);
  pinMode(LED_YELLOW_1, OUTPUT);
  pinMode(LED_RED_1, OUTPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(LED_BLUE_2, OUTPUT);
  pinMode(LED_YELLOW_2, OUTPUT);
  pinMode(LED_RED_2, OUTPUT);
  Serial.begin(9600);
}

void loop()
{ 
  if(digitalRead(BUTTON_1)==LOW && !active){
    while(!digitalRead(BUTTON_1)){}
    counter_1++;
    Serial.println(String(counter_1)+", "+String(counter_2));
  }
  if(digitalRead(BUTTON_2)==LOW && active){
    while(!digitalRead(BUTTON_2)){}
    counter_2++;
    Serial.println(String(counter_1)+", "+String(counter_2));
  }
  digitalWrite(LED_BLUE_1, active);
  digitalWrite(LED_YELLOW_1, 0);
  digitalWrite(LED_RED_1, !active);
  digitalWrite(LED_BLUE_2, !active);
  digitalWrite(LED_YELLOW_2, 0);
  digitalWrite(LED_RED_2, active);
  if (counter_1 > 4) {
    changeYellow();
    counter_1 = 0;
    active = !active;
  }
  if (counter_2 > 4) {
    changeYellow();
    counter_2 = 0;
    active = !active;
  }
}

void changeYellow() {
  digitalWrite(LED_BLUE_1, 0);
  digitalWrite(LED_YELLOW_1, 1);
  digitalWrite(LED_RED_1, 0);
  digitalWrite(LED_BLUE_2, 0);
  digitalWrite(LED_YELLOW_2, 1);
  digitalWrite(LED_RED_2, 0);
  delay(1000);
}