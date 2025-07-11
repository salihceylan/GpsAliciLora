#ifndef WIFI_BAGLANTI_H
#define WIFI_BAGLANTI_H

#include <WiFi.h>

const char *ssid = "Fingon";
const char *password = "Fingon08";

void baglanWiFi() {
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  Serial.println("WiFi'ye bağlanılıyor...");

  unsigned long baslangic = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - baslangic < 10000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\\nWiFi bağlı!");
    Serial.print("IP Adresi: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\\nWiFi bağlantısı başarısız, offline modda çalışılıyor.");
  }
}

#endif