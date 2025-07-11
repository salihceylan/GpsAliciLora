#ifndef LORA_GPS_YON_ALICI_H
#define LORA_GPS_YON_ALICI_H

#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>

#define LORA_SCK 6
#define LORA_MISO 4
#define LORA_MOSI 5
#define LORA_CS 7
#define LORA_RST 15
#define LORA_DIO0 16

const char* AUTH_KEY = "Y3p97zXq!";
bool lora_durum = false;

void lora_gps_yon_alici_baslat() {
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
  LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("❌ LoRa başlatılamadı!");
    lora_durum = false;
  } else {
    LoRa.setSpreadingFactor(12);     // max menzil, min hız
    LoRa.setSignalBandwidth(125E3);  // 125 kHz (varsayılan)
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

    // 🔐 Güvenlik kontrolü
    const char* gelenAuth = doc["auth"];
    if (strcmp(gelenAuth, AUTH_KEY) != 0) {
      Serial.println("⛔ Yetkisiz paket! Reddedildi.");
      return;
    }

    // ✅ Verileri oku
    float heading = doc["heading"];
    float lat = doc["lat"];
    float lon = doc["lon"];
    float hiz = doc["hiz"];
    float yukseklik = doc["yukseklik"];
    int uydu = doc["uydu"];

    // 🧭 Verileri göster
    Serial.println("🌍 GPS + Yön Verisi:");
    Serial.print("Yön (Baş): ");
    Serial.print(heading);
    Serial.println("°");
    Serial.print("Enlem:     ");
    Serial.println(lat, 6);
    Serial.print("Boylam:    ");
    Serial.println(lon, 6);
    Serial.print("Hız:       ");
    Serial.print(hiz);
    Serial.println(" km/h");
    Serial.print("Yükseklik: ");
    Serial.print(yukseklik);
    Serial.println(" m");
    Serial.print("Uydu Sayısı: ");
    Serial.println(uydu);
    Serial.println("-----");
  }
}

#endif
