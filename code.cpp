#include <Servo.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 13, 6, 7, 8, A3);

Servo servo1;
Servo servo2;
Servo servo3;

const int pot1Pin = A0;
const int pot2Pin = A1;
const int pot3Pin = A2;

const int btnRecordPin = 2;
const int btnPlayPin = 3;
const int ledRecordPin = 4;
const int ledPlayPin = 5;

int currentMode = 0; 
const int MAX_STEPS = 100;
int pos1[MAX_STEPS];
int pos2[MAX_STEPS];
int pos3[MAX_STEPS];
int totalSteps = 0;   
int playIndex = 0;    
unsigned long lastActionTime = 0;
const int timePerStep = 100; 

unsigned long lastLcdUpdate = 0;

bool lastRecordState = HIGH;
bool lastPlayState = HIGH;

void setup() {
  Serial.begin(9600);
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Robotkar Rendszer");
  lcd.setCursor(0, 1);
  lcd.print("Inicializalas...");
  delay(1000);
  lcd.clear();
  
  servo1.attach(9);
  servo2.attach(10);
  servo3.attach(11);

  pinMode(btnRecordPin, INPUT_PULLUP);
  pinMode(btnPlayPin, INPUT_PULLUP);

  pinMode(ledRecordPin, OUTPUT);
  pinMode(ledPlayPin, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  
  bool currentRecordState = digitalRead(btnRecordPin);
  if (currentRecordState == LOW && lastRecordState == HIGH) {
    delay(50); 
    if (currentMode != 1) {
      currentMode = 1; 
      totalSteps = 0;  
      digitalWrite(ledRecordPin, HIGH);
      digitalWrite(ledPlayPin, LOW);
      lcd.clear();
    } else {
      currentMode = 0; 
      digitalWrite(ledRecordPin, LOW);
      lcd.clear();
    }
  }
  lastRecordState = currentRecordState; 

  bool currentPlayState = digitalRead(btnPlayPin);
  if (currentPlayState == LOW && lastPlayState == HIGH) {
    delay(50); 
    if (currentMode != 2 && totalSteps > 0) {
      currentMode = 2; 
      playIndex = 0;
      digitalWrite(ledPlayPin, HIGH);
      digitalWrite(ledRecordPin, LOW);
      lcd.clear();
    } else {
      currentMode = 0; 
      digitalWrite(ledPlayPin, LOW);
      lcd.clear();
    }
  }
  lastPlayState = currentPlayState;

  if (currentMode == 0) {
    servo1.write(map(analogRead(pot1Pin), 0, 1023, 0, 180));
    servo2.write(map(analogRead(pot2Pin), 0, 1023, 0, 180));
    servo3.write(map(analogRead(pot3Pin), 0, 1023, 0, 180));
  }
  else if (currentMode == 1) {
    int val1 = map(analogRead(pot1Pin), 0, 1023, 0, 180);
    int val2 = map(analogRead(pot2Pin), 0, 1023, 0, 180);
    int val3 = map(analogRead(pot3Pin), 0, 1023, 0, 180);
    
    servo1.write(val1);
    servo2.write(val2);
    servo3.write(val3);

    if (currentMillis - lastActionTime >= timePerStep) {
      if (totalSteps < MAX_STEPS) {
        pos1[totalSteps] = val1;
        pos2[totalSteps] = val2;
        pos3[totalSteps] = val3;
        totalSteps++;
        lastActionTime = currentMillis;
      } else {
        currentMode = 0; 
        digitalWrite(ledRecordPin, LOW);
        lcd.clear();
      }
    }
  }
  else if (currentMode == 2) {
    if (currentMillis - lastActionTime >= timePerStep) {
      servo1.write(pos1[playIndex]);
      servo2.write(pos2[playIndex]);
      servo3.write(pos3[playIndex]);
      
      playIndex++;
      if (playIndex >= totalSteps) {
        playIndex = 0; 
      }
      lastActionTime = currentMillis;
    }
  }
  
  
  if (currentMillis - lastLcdUpdate >= 200) {
    lcd.setCursor(0, 0);
    if (currentMode == 0) {
      lcd.print("Mod: Manualis   ");
      lcd.setCursor(0, 1);
      lcd.print("Memoria: ");
      lcd.print(totalSteps);
      lcd.print("/");
      lcd.print(MAX_STEPS);
      lcd.print("   ");
    }
    else if (currentMode == 1) {
      lcd.print("Mod: Felvetel   ");
      lcd.setCursor(0, 1);
      lcd.print("Rogzitve: ");
      lcd.print(totalSteps);
      lcd.print("   ");
    }
    else if (currentMode == 2) {
      lcd.print("Mod: Lejatszas  ");
      lcd.setCursor(0, 1);
      lcd.print("Lepes: ");
      lcd.print(playIndex);
      lcd.print("/");
      lcd.print(totalSteps);
      lcd.print("   ");
    }
    lastLcdUpdate = currentMillis;
  }
}
