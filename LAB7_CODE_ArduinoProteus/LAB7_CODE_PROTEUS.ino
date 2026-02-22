#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>

// ------------------- SENSOR DEFINITIONS -------------------
#define LED_RED 2
#define LED_GREEN 3
#define DHTPIN 4
#define PPDPIN 5
#define DHTTYPE DHT11
#define TRIG 12
#define ECHO 11
#define LDRPIN A1
#define BUZZERPIN 7

// ------------------- MOTOR DRIVER PINS -------------------
#define EN1 6
#define IN1 10
#define IN2 9

// ------------------- OBJECT DECLARATIONS -------------------
LiquidCrystal_I2C lcd(0x20, 16, 2);
Adafruit_BMP085 bmp;
DHT dht(DHTPIN, DHTTYPE);

// ------------------- VARIABLES -------------------
float temperatureDHT, humidity;
float temperatureBMP, pressure;
float distance, lightLevel, dustValue;

int occupancy = 0;   // FIELD 2
int fanState = 0;    // FIELD 9

// ------------------- ULTRASONIC FUNCTION -------------------
float getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  float duration = pulseIn(ECHO, HIGH);
  return (duration * 0.0343) / 2;
}

// ------------------- SETUP -------------------
void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LDRPIN, INPUT);
  pinMode(PPDPIN, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(BUZZERPIN, OUTPUT);

  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  dht.begin();
  bmp.begin();

  lcd.setCursor(0, 0);
  lcd.print("ENVIRONMENT SYST");
  delay(2000);
  lcd.clear();
}

// ------------------- LOOP -------------------
void loop() {
  // --- Sensor Readings ---
  distance = getDistance();
  humidity = dht.readHumidity();
  temperatureDHT = dht.readTemperature();
  temperatureBMP = bmp.readTemperature();
  pressure = bmp.readPressure() / 100.0;
  lightLevel = analogRead(LDRPIN);
  unsigned long duration = pulseIn(PPDPIN, LOW);
  dustValue = duration / 1000.0;

  lcd.clear();

  // ------------------- OCCUPANCY -------------------
  if (distance >= 50) {
    occupancy = 0;
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    noTone(BUZZERPIN);

    fanState = 0;
    digitalWrite(EN1, LOW);
    analogWrite(IN1, LOW);
    analogWrite(IN2, LOW);

    lcd.setCursor(0, 0);
    lcd.print("No Occupancy");

  } else {
    occupancy = 1;
    lcd.setCursor(0, 0);
    lcd.print("Monitor Active");

    // --- R2: High Temperature ---
    if (temperatureDHT >= 30 || temperatureBMP >= 30) {
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW);
      tone(BUZZERPIN, 1000);

      fanState = 1;
      digitalWrite(EN1, HIGH);
      analogWrite(IN1, 255);
      analogWrite(IN2, LOW);

      lcd.setCursor(0, 0);
      lcd.print("Monitor Alert");
      lcd.setCursor(0, 1);
      lcd.print("High Temperature");
    }
    // --- R3: Poor Air Quality ---
    else if (dustValue > 150) {
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW);
      tone(BUZZERPIN, 1000);

      fanState = 1;
      digitalWrite(EN1, HIGH);
      analogWrite(IN1, 255);
      analogWrite(IN2, LOW);

      lcd.setCursor(0, 0);
      lcd.print("Monitor Alert");
      lcd.setCursor(0, 1);
      lcd.print("Poor Air Quality");
    }
    // --- R4: High Humidity ---
    else if (humidity > 70) {
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW);
      tone(BUZZERPIN, 1000);

      fanState = 1;
      digitalWrite(EN1, HIGH);
      analogWrite(IN1, 255);
      analogWrite(IN2, LOW);

      lcd.setCursor(0, 0);
      lcd.print("Monitor Alert");
      lcd.setCursor(0, 1);
      lcd.print("High Humidity");
    }
    // --- R5: Low Humidity ---
    else if (humidity < 40) {
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW);
      tone(BUZZERPIN, 800);

      fanState = 0;
      digitalWrite(EN1, LOW);
      analogWrite(IN1, LOW);
      analogWrite(IN2, LOW);

      lcd.setCursor(0, 0);
      lcd.print("Monitor Alert");
      lcd.setCursor(0, 1);
      lcd.print("Low Humidity");
    }
    // --- R8: All Normal ---
    else {
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, HIGH);
      noTone(BUZZERPIN);

      fanState = 0;
      digitalWrite(EN1, LOW);
      analogWrite(IN1, LOW);
      analogWrite(IN2, LOW);

      lcd.setCursor(0, 0);
      lcd.print("Monitor System");
      lcd.setCursor(0, 1);
      lcd.print("ALL NORMAL");
    }
  }

  // ------------------- TIMESTAMP -------------------
  unsigned long timestamp = millis() / 1000;  // Simulated Unix time for Proteus

  // ------------------- CSV FORMAT (9 FIELDS) -------------------
  String csvData =
    String(timestamp) + "," +
    String(occupancy) + "," +
    String(temperatureDHT, 2) + "," +
    String(humidity, 2) + "," +
    String(pressure, 2) + "," +
    String(dustValue, 2) + "," +
    String(lightLevel, 2) + "," +
    String(distance, 2) + "," +
    String(fanState);

  Serial.println(csvData);

  delay(2000);
}
