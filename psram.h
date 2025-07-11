#ifndef PSRAM_H
#define PSRAM_H

#include <Arduino.h>

void kontrolEtPSRAM() {
  if (psramFound()) {
    Serial.println("PSRAM Var");
    Serial.print("Toplam PSRAM: ");
    Serial.print(ESP.getPsramSize() / 1024);
    Serial.print(" KB 📦 Flash: ");
    Serial.print(ESP.getFlashChipSize() / 1024 / 1024);
    Serial.println(" MB");

    Serial.print("⚡ Flash Hızı: ");
    Serial.print(ESP.getFlashChipSpeed() / 1000000);
    Serial.println(" MHz");

    Serial.print("📁 Flash Modu: ");
    Serial.println(ESP.getFlashChipMode());
  } else {
    Serial.println("PSRAM Yok");
  }
}

#endif