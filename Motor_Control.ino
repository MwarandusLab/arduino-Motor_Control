
// Motor Connections (Both must use PWM pins)
#define RPWM 5
#define LPWM 6
#define Yellow 8
int Button_1 = 2;
int Tracker_1 = 1;
int Tracker_2 = 0;

int Blue_Led = 11;
int Red_Led = 10;
int Green_Led = 9;

#define POTENTIOMETER A0

volatile boolean buttonState1 = HIGH;
volatile boolean lastButtonState1 = HIGH;
volatile unsigned long lastDebounceTime1 = 0;
volatile unsigned long debounceDelay1 = 50;
enum State {
  IDLE,
  CLOCKWISE,
  ANTICLOCKWISE
};

State currentState = IDLE;

void setup() {
  Serial.begin(9600);
  pinMode(Button_1, INPUT_PULLUP);
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(Yellow, OUTPUT);
  pinMode(Blue_Led, OUTPUT);
  pinMode(Red_Led, OUTPUT);
  pinMode(Green_Led, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(Button_1), buttonInterrupt1_Service_1, FALLING);

  // Stop motors
  analogWrite(RPWM, 0);
  analogWrite(LPWM, 0);
  digitalWrite(Blue_Led, LOW);
  digitalWrite(Green_Led, LOW);
  digitalWrite(Red_Led, HIGH);
}

void loop() {
  buttonInterrupt1_Service_1();
  switch (currentState) {
    case IDLE:
      idle();
      break;
    case CLOCKWISE:
      clockwise();
      break;
    case ANTICLOCKWISE:
      anticlockwise();
      break;
  }
}
void idle() {
  digitalWrite(LPWM, LOW);
  analogWrite(RPWM, 0);
  digitalWrite(Blue_Led, LOW);
  digitalWrite(Green_Led, LOW);
  digitalWrite(Red_Led, HIGH);
}
void clockwise() {
  int potValue = analogRead(POTENTIOMETER);
  int pwmValue = map(potValue, 0, 1023, 0, 255);
  Serial.println(potValue);
  digitalWrite(Yellow, LOW);
  digitalWrite(LPWM, LOW);
  analogWrite(RPWM, pwmValue);
  digitalWrite(Blue_Led, HIGH);
  digitalWrite(Green_Led, LOW);
  digitalWrite(Red_Led, LOW);
  Tracker_1 = 2;
  Tracker_2 = 1;
}
void anticlockwise() {
  int potValue = analogRead(POTENTIOMETER);
  int pwmValue = map(potValue, 0, 1023, 0, 255);
  digitalWrite(Yellow, HIGH);
  digitalWrite(LPWM, LOW);
  analogWrite(RPWM, pwmValue);
  digitalWrite(Blue_Led, LOW);
  digitalWrite(Green_Led, HIGH);
  digitalWrite(Red_Led, LOW);
  Tracker_1 = 1;
  Tracker_2 = 0;
}
void buttonInterrupt1_Service_1() {
  if (millis() - lastDebounceTime1 > debounceDelay1) {
    buttonState1 = digitalRead(Button_1);
    if (buttonState1 == LOW && lastButtonState1 == HIGH) {
      if (currentState == IDLE && Tracker_1 == 2 && Tracker_2 == 1) {
        Serial.println("Button 1 Clockwise!");
        currentState = ANTICLOCKWISE;
      } else if (currentState == ANTICLOCKWISE || currentState == CLOCKWISE ) {
        Serial.println("Button 1 Idle!");
        currentState = IDLE;
      } else if (currentState == IDLE && Tracker_1 == 1 && Tracker_2 == 0) {
        Serial.println("Button 1 Anti-Clockwise!");
        currentState = CLOCKWISE;
      }
    }
    lastButtonState1 = buttonState1;
    lastDebounceTime1 = millis();
  }
}