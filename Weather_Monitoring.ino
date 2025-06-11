#define BLYNK_TEMPLATE_ID "TMPL6w9tSvxII"
#define BLYNK_TEMPLATE_NAME "báo hiệu thời tiết"
#define BLYNK_AUTH_TOKEN "XmUVs7LU_v0cocjdoMf-3vkiBre-VSDO"

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// Thông tin kết nối WiFi và Blynk
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "2.4Ghz MK";
char pass[] = "77777777";

// Định nghĩa các chân kết nối cảm biến
#define LDR   4
#define TH    5
#define Rain  35

// Biến toàn cục
double T, P;
char status;

// Khai báo LCD I2C: địa chỉ 0x27, 16 cột, 2 dòng
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Khởi tạo Blynk timer
BlynkTimer timer;

// Khởi tạo đối tượng DHT
DHT dht(TH, DHT11);

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  dht.begin();
  lcd.init();
  lcd.backlight();

  pinMode(LDR, INPUT);
  pinMode(Rain, INPUT);
  analogReadResolution(12);

  lcd.setCursor(0, 0);
  lcd.print("weather.....");
  lcd.setCursor(4, 1);
  lcd.print("system");
  delay(4000);
  lcd.clear();
}

void DHT11sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("không đọc được dữ liệu");
    return;
  }

  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V2, h);

  lcd.setCursor(10, 0);
  lcd.print("T:");
  lcd.print(t);

  lcd.setCursor(0, 0);
  lcd.print("H:");
  lcd.print(h);
}
void rainSensor() {
  int Rvalue = analogRead(Rain);
  Rvalue = map(Rvalue, 0, 4095, 0, 100);
  Rvalue = (Rvalue - 100) * -1;

  Blynk.virtualWrite(V1, Rvalue);

  lcd.setCursor(0, 1);
  lcd.print("R:");
  lcd.print(Rvalue);
  lcd.print(" ");
  
  Serial.println(Rvalue);
}

void LDRsensor() {
  bool value = digitalRead(LDR);

  WidgetLED LED(V3);

  if (value == 1) {
    LED.on();
  } else {
    LED.off();
  }
}

void loop() {
  DHT11sensor();
  rainSensor();
  LDRsensor();
  Blynk.run();
}
