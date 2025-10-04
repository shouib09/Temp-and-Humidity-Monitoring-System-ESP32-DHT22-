#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ====== DHT22 SETUP ======
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ====== FAN + LED PINS ======
#define FAN_PIN 16
#define ALERT_PIN 17
#define STATUS_PIN 5

// ====== LCD SETUP ======
// Try 0x27 first; if not working, change to 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ====== THINGSPEAK WRITE API KEY ======
String apiKey = "WL1494L3E0I4ZTC5";

// ====== WIFI CREDENTIALS ======
const char* ssid = "Wokwi-GUEST";
const char* password = "";

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Pin modes
  pinMode(FAN_PIN, OUTPUT);
  pinMode(ALERT_PIN, OUTPUT);
  pinMode(STATUS_PIN, OUTPUT);

  dht.begin();

 // LCD init
  Wire.begin(21, 22);   // ESP32 I2C pins: SDA=21, SCL=22
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

 // WiFi connect
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected!");
  delay(2000);
  lcd.clear();
}

void loop()
{
  // ====== READ SENSORS ======
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
   Serial.println(" Failed to read from DHT22");
   return;
  }
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" °C | Hum: ");
  Serial.print(hum);
  Serial.println(" %");

 // ====== FAN + LED CONTROL ======
  bool fanStatus = false;
  if (temp > 30) {
  digitalWrite(FAN_PIN, HIGH);
  digitalWrite(STATUS_PIN, HIGH);
  fanStatus = true;
  } else {
  digitalWrite(FAN_PIN, LOW);
  digitalWrite(STATUS_PIN, LOW);
  }

 // Control alert LED
 if (temp > 50) {
  digitalWrite(ALERT_PIN, HIGH);
  } else {
  digitalWrite(ALERT_PIN, LOW);
  }


  // ====== LCD DISPLAY ======
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp, 1);
  lcd.print("C H:");
  lcd.print(hum, 0);
  lcd.print("% ");

  lcd.setCursor(0, 1);
  lcd.print("Fan:");
  lcd.print(fanStatus ? "ON " : "OFF");
  if (temp > 50) {
     lcd.setCursor(9, 1);
     lcd.print("ALERT!");
    } else {
     lcd.setCursor(9, 1);
     lcd.print("      "); // clear old text
    }
  // Send data to ThingSpeak via Wi-Fi
 if (WiFi.status() == WL_CONNECTED) {
   HTTPClient http;
   String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
               "&field1=" + String(temp, 1) +    // Field1 = Temperature
               "&field2=" + String(hum, 1) +     // Field2 = Humidity
               "&field3=" + String(fanStatus);   // Field3 = Fan (1=ON, 0=OFF)

   http.begin(url);
   int httpCode = http.GET();

   if (httpCode > 0) {
   Serial.printf(" ThingSpeak Response: %d\n", httpCode);
   } else {
   Serial.printf(" HTTP request failed: %s\n",
   http.errorToString(httpCode).c_str());
   }
   http.end();
  }

 delay(15000); // ThingSpeak update every 15 sec
}
