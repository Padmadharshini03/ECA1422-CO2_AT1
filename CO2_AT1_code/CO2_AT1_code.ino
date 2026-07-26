#include <LiquidCrystal.h>
#include <Servo.h>

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
Servo arm;

int tempSensor = A0;
int vibrationSensor = 8;
int proximitySensor = 9;

int fan = 10;
int buzzer = 11;

unsigned long startTime, responseTime;

void setup() {
  pinMode(vibrationSensor, INPUT);
  pinMode(proximitySensor, INPUT);
  pinMode(fan, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  arm.attach(12); // servo motor
  
  lcd.begin(16, 2);
  lcd.print("Industry 5.0 Sys");
  delay(2000);
  Serial.begin(9600);
}

void loop() {
  startTime = millis(); // performance metric
  
  int tempValue = analogRead(tempSensor);
  int vibrationValue = digitalRead(vibrationSensor);
  int proximityValue = digitalRead(proximitySensor);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(tempValue);

  // Decision rules
  if(tempValue > 600) { // overheating
    digitalWrite(fan, HIGH);
    lcd.setCursor(0,1);
    lcd.print("Fan ON");
  } else {
    digitalWrite(fan, LOW);
  }

  if(vibrationValue == HIGH || proximityValue == HIGH) {
    digitalWrite(buzzer, HIGH);
    lcd.setCursor(0,1);
    lcd.print("ALERT!");
    arm.write(90); // move robotic arm
  } else {
    digitalWrite(buzzer, LOW);
    arm.write(0); // reset arm
  }

  // Performance evaluation
  responseTime = millis() - startTime;
  Serial.print("Response Time: ");
  Serial.println(responseTime);
  delay(500);
}
