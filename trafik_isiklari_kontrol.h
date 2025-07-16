#ifndef TRAFIK_ISIKLARI_KONTROL_H
#define TRAFIK_ISIKLARI_KONTROL_H

#define YESIL_LED   12
#define KIRMIZI_LED 10
#define SARI_LED    11

bool ambulans_yakinda = false;
unsigned long son_ambulans_gorulme = 0;

void trafik_normale_don() {
  digitalWrite(YESIL_LED, LOW);
  digitalWrite(KIRMIZI_LED, HIGH);
  digitalWrite(SARI_LED, LOW);
}


void trafik_isik_baslat() {
  pinMode(YESIL_LED, OUTPUT);
  pinMode(KIRMIZI_LED, OUTPUT);
  pinMode(SARI_LED, OUTPUT);
  trafik_normale_don(); // Başlangıçta kırmızı
}

void trafik_yesil_yak() {
  digitalWrite(YESIL_LED, HIGH);
  digitalWrite(KIRMIZI_LED, LOW);
  digitalWrite(SARI_LED, LOW);
}

void trafik_durumu_guncelle() {
  if (ambulans_yakinda) {
    trafik_yesil_yak();
    if (millis() - son_ambulans_gorulme > 10000) { // 10 sn ambulans görülmezse
      ambulans_yakinda = false;
    }
  } else {
    trafik_normale_don();
  }
}

#endif
