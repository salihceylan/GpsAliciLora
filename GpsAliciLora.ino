#include <Arduino.h>
#include "psram.h"
#include "wifi_baglanti.h"
#include "lora_gps_yon_alici.h"
#include "trafik_isiklari_kontrol.h"



#define LORA_LED 18

void setup() {
  Serial.begin(115200); 
  kontrolEtPSRAM();
  baglanWiFi();


  pinMode(LORA_LED, OUTPUT);

  lora_gps_yon_alici_baslat();
  LoRa.setTxPower(20);
  digitalWrite(LORA_LED, lora_durum ? HIGH : LOW);
  
}

void loop() {

 lora_verisini_kontrol_et();
 trafik_durumu_guncelle(); 
  delay(500);
}