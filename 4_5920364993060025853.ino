#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

// شاشة I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

// إعدادات حساس DHT
#define DHTPIN 8
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// إعدادات المروحة
#define FAN_PIN 9
#define TEMP_LIMIT 22 // درجة الحرارة اللي عندها المروحة تشتغل

float temp;
float humi;

void setup() {
  lcd.init();          // تهيئة الشاشة
  lcd.backlight();     // تشغيل الإضاءة الخلفية

  dht.begin();

  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW);   // المروحة مطفّية في البداية (Active HIGH)

  lcd.setCursor(3, 0);
  lcd.print("DHT Sensor");
  lcd.setCursor(2, 1);
  lcd.print("Initializing");
  delay(3000);
  lcd.clear();
}

void loop() {
  delay(2000);

  temp = dht.readTemperature();
  humi = dht.readHumidity();

  // لو في مشكلة في قراءة الحساس
  if (isnan(temp) || isnan(humi)) {
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!   ");
    lcd.setCursor(0, 1);
    lcd.print("Check Wiring   ");
    digitalWrite(FAN_PIN, LOW);   // إيقاف المروحة
    return;
  }

  // عرض القيم على الشاشة
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print(" C   ");

  lcd.setCursor(0, 1);
  lcd.print("Humi: ");
  lcd.print(humi);
  lcd.print(" %   ");

  // التحكم في المروحة
  if (temp >= TEMP_LIMIT) {
    digitalWrite(FAN_PIN, HIGH);   // تشغيل المروحة
  } else {
    digitalWrite(FAN_PIN, LOW);    // إيقاف المروحة
  }
}
