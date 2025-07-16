#ifndef LORA_GPS_YON_ALICI_H
#define LORA_GPS_YON_ALICI_H

#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>
#include <TinyGPSPlus.h>

// LoRa pinleri
#define LORA_SCK 6
#define LORA_MISO 4
#define LORA_MOSI 5
#define LORA_CS 7
#define LORA_RST 15
#define LORA_DIO0 16

// 📍 Trafik lambasının sabit konumu
#define TRAFIK_LAT 38.604800
#define TRAFIK_LON 27.086900

const char* AUTH_KEY = "Y3p97zXq!";
bool lora_durum = false;

extern bool ambulans_yakinda;
extern unsigned long son_ambulans_gorulme;

// Ambulans kimliği kontrolü
bool ambulans_tanimli(const char* gelen_id) {
  const char* izinli_ambulanslar[] = { "35ABC35", "34XYZ99" };
  for (int i = 0; i < sizeof(izinli_ambulanslar) / sizeof(izinli_ambulanslar[0]); i++) {
    if (strcmp(gelen_id, izinli_ambulanslar[i]) == 0) return true;
  }
  return false;
}

void lora_gps_yon_alici_baslat() {
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
  LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("❌ LoRa başlatılamadı!");
    lora_durum = false;
  } else {
    LoRa.setSpreadingFactor(12);
    LoRa.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(5);
    Serial.println("✅ LoRa alıcı başlatıldı.");
    lora_durum = true;
  }
}

void lora_verisini_kontrol_et() {
  if (!lora_durum) return;

  int paketBoyutu = LoRa.parsePacket();
  if (paketBoyutu > 0) {
    String gelenVeri = "";
    while (LoRa.available()) {
      gelenVeri += (char)LoRa.read();
    }

    Serial.println("📩 Alınan Veri: " + gelenVeri);

    StaticJsonDocument<256> doc;
    DeserializationError hata = deserializeJson(doc, gelenVeri);

    if (hata) {
      Serial.print("⚠️ JSON Hatası: ");
      Serial.println(hata.c_str());
      return;
    }

    const char* gelenAuth = doc["auth"];
    if (strcmp(gelenAuth, AUTH_KEY) != 0) {
      Serial.println("⛔ Yetkisiz paket! Reddedildi.");
      return;
    }

    const char* gelen_id = doc["id"];
    float heading = doc["heading"];
    float lat = doc["lat"];
    float lon = doc["lon"];
    float hiz = doc["hiz"];

    // 📝 Gelen veriyi HER ZAMAN göster
    Serial.println("🚑 Ambulans ID: " + String(gelen_id));
    Serial.print("Yön: ");
    Serial.print(heading);
    Serial.println("°");
    Serial.print("Konum: ");
    Serial.print(lat, 6);
    Serial.print(", ");
    Serial.println(lon, 6);
    Serial.print("Hız: ");
    Serial.print(hiz);
    Serial.println(" km/h");
    Serial.println("---------------------");

    // ✅ Işık müdahale kontrolü burada
    if (ambulans_tanimli(gelen_id)) {
      float mesafe = TinyGPSPlus::distanceBetween(lat, lon, TRAFIK_LAT, TRAFIK_LON);
      Serial.print("📏 Mesafe: ");
      Serial.print(mesafe);
      Serial.println(" m");

      if (mesafe < 1000) {
        ambulans_yakinda = true;
        son_ambulans_gorulme = millis();
        Serial.println("✅ Ambulans Etki Alanında! YEŞİL Zorla Açıldı.");
      }
    }
  }
}

#endif
