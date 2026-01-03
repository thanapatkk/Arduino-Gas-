#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "time.h"

#define GAS_PIN 34
#define LED_PIN 26
#define BUZZER_PIN 25

LiquidCrystal_I2C lcd(0x27,16,2);

// ---- WiFi ----
const char* ssid = "username";        
const char* password = "password";    

// ---- LINE Messaging API ----
const String CHANNEL_ACCESS_TOKEN = "--";
const String LINE_TO_ID = "--";

int baseline = 0;
int threshold = 0;
bool alertSent = false;

// ฟังก์ชันอ่านค่าเฉลี่ยจาก sensor
int readGasAvg() {
  long sum = 0;
  for(int i=0;i<20;i++){
    sum += analogRead(GAS_PIN);
    delay(50);
  }
  return sum / 20;
}

// ตั้งเวลา NTP
void initTime(){
  configTime(7*3600, 0, "pool.ntp.org", "time.nist.gov");
}

// ดึงวัน/เดือน/ปี เวลาในรูปแบบ DD/MM/YYYY HH:MM:SS
String getDateTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    return "NoTime";
  }
  char buf[32];
  sprintf(buf, "%02d/%02d/%04d %02d:%02d:%02d",
    timeinfo.tm_mday, timeinfo.tm_mon+1, timeinfo.tm_year+1900,
    timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  return String(buf);
}

// ส่งข้อความผ่าน LINE Messaging API
void sendLineMessage(String msg){
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin("https://api.line.me/v2/bot/message/push");
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer " + CHANNEL_ACCESS_TOKEN);

    // Escape \n เป็น \\n เพื่อ LINE แสดงผลขึ้นบรรทัดใหม่
    msg.replace("\n", "\\n");

    String body = "{\"to\":\"" + LINE_TO_ID + "\",\"messages\":[{\"type\":\"text\",\"text\":\"" + msg + "\"}]}";

    int res = http.POST(body);
    Serial.print("LINE API status: ");
    Serial.println(res);
    http.end();
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.print("Connecting WiFi...");
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
  lcd.clear();
  lcd.print("WiFi Connected");
  delay(1000);
  lcd.clear();

  initTime();

  lcd.print("Calibrating...");
  delay(3000);
  baseline = readGasAvg();
  threshold = baseline + 1200;
  lcd.clear();
}

void loop() {
  int gas = readGasAvg();
  String dt = getDateTime();

  lcd.setCursor(0,0);
  lcd.print("Gas: " + String(gas) + " Unit   ");

  lcd.setCursor(0,1);
  if(gas > threshold){
    lcd.print("Dangerous!!!       ");
    digitalWrite(LED_PIN, HIGH);
    //digitalWrite(BUZZER_PIN, HIGH);

    if(!alertSent){
      // ข้อความ LINE อันตราย + อิโมจิ ⚠️
      String message = dt + "\nค่าแก๊ส: " + String(gas) + " Unit\n⚠️ อันตราย! ควรปิดวาล์วแก๊สและระบายอากาศทันที";
      sendLineMessage(message);
      alertSent = true;
    }

  } else {
    lcd.print("Safe          ");
    digitalWrite(LED_PIN, LOW);
    //digitalWrite(BUZZER_PIN, LOW);

    // ส่งข้อความปลอดภัยเมื่อค่าก๊าซกลับปกติ
    if(alertSent){
      String message = dt + "\nค่าแก๊ส: " + String(gas) + " Unit\n✅ ปลอดภัยแล้ว";
      sendLineMessage(message);
      alertSent = false; // รีเซ็ตเพื่อแจ้งเตือนครั้งต่อไป
    }
  }

  Serial.println(dt + " | Gas: " + String(gas) + " Unit");
  delay(500);
}
