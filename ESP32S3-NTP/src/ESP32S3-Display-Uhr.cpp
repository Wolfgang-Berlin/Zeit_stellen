/*
   ESP32-S3-LCD-1.47 von Waveshare

*/

#include <Arduino.h>  
#include <Wire.h>    
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <WiFi.h>
#include <time.h>

#define TFT_CS        42
#define TFT_RST       39
#define TFT_DC        41
#define TFT_MOSI      45
#define TFT_SCLK      40
#define TFT_BL        48

#define TIMEZONE "CET-1CEST,M3.5.0/02,M10.5.0/03"

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

unsigned long wifiStartTime = 0;
bool wifiConnected = false;
bool wifiDisconnected = false;

// Hilfsfunktion für Statusanzeige
void showStatus(const char* msg, uint16_t color = ST77XX_WHITE, int textSize = 2) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(10, 130); // Statuszeile 130
  tft.setTextSize(textSize);
  tft.setTextColor(color);
  tft.print(msg);
}

void disconnectWiFi() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  wifiConnected = false;
}

bool connectToAvailableWiFi() {
  showStatus("Scanne nach WLANs...", ST77XX_YELLOW, 2);
  int networksFound = WiFi.scanNetworks();
  if (networksFound == 0) {
    showStatus("Kein WLAN gefunden", ST77XX_RED, 2);
    delay(2000);
    return false;
  }

  for (int i = 0; i < WIFI_COUNT; i++) {
    for (int j = 0; j < networksFound; j++) {
      if (WiFi.SSID(j) == WIFI_SSIDS[i]) {
        char buf[50];
        snprintf(buf, sizeof(buf), "Verbinde zu: %s", WIFI_SSIDS[i]);
        showStatus(buf, ST77XX_CYAN, 2);
        
        WiFi.begin(WIFI_SSIDS[i], WIFI_PASSWORDS[i]);
        unsigned long startAttempt = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 8000) {
          delay(500);
        }
        if (WiFi.status() == WL_CONNECTED) {

          snprintf(buf, sizeof(buf), "WLAN: %s", WIFI_SSIDS[i]);
          showStatus(buf, ST77XX_GREEN, 2);
          delay(1500);
          return true;
        }
      }
    }
  }
  showStatus("Kein bekanntes WLAN", ST77XX_RED, 2);
  delay(2000);
  return false;
}

void drawTime_S(struct tm* timeinfo) {
  char fullTimeStr[30];
  strftime(fullTimeStr, sizeof(fullTimeStr), "%Y-%m-%d %H:%M:%S", timeinfo);
  Serial.println(fullTimeStr);
}

void draw_Display(struct tm* timeinfo) {
  char timeStr[6];
  strftime(timeStr, sizeof(timeStr), "%H:%M", timeinfo);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(6);
  tft.setTextColor(tft.color565(180, 180, 180));
  tft.setCursor(63, 40);
  tft.print(timeStr);
  tft.setTextSize(2);
  tft.setCursor(10, 130);

  if (wifiConnected) {
    tft.setTextColor(ST77XX_GREEN);
    tft.println(WiFi.SSID());
  } else {
    tft.setTextColor(ST77XX_RED);
    tft.print("Kein WLAN verbunden");
  }
}

void setup() {
  Serial.begin(115200);
  // Initialisiere TFT-Display
  SPI.begin(TFT_SCLK, -1, TFT_MOSI);
  pinMode (TFT_BL, OUTPUT);
  digitalWrite (TFT_BL, HIGH);
  tft.init(172, 320);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  WiFi.mode(WIFI_STA);
  wifiConnected = connectToAvailableWiFi();
  wifiStartTime = millis();

  configTime(0, 0, "pool.ntp.org");
  setenv("TZ", TIMEZONE, 1);

  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    delay(1000);
  }
}

void loop() {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);

  drawTime_S(timeinfo);

  static int lastDisplayedMinute = -1; 
  if (timeinfo->tm_min != lastDisplayedMinute) { 
    draw_Display(timeinfo);
    lastDisplayedMinute = timeinfo->tm_min;
  }

  if (!wifiDisconnected && wifiConnected && millis() - wifiStartTime > 60000) {
    disconnectWiFi();
    wifiDisconnected = true;
  }

  if (millis() > 180000) { 
    showStatus("Gehe sclafen ...", ST77XX_YELLOW, 2);
    delay(10000);
    esp_deep_sleep_start();
  }

  delay(200);
}
