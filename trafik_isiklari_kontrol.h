#ifndef TRAFIK_ISIKLARI_KONTROL_H
#define TRAFIK_ISIKLARI_KONTROL_H

#define YESIL_LED   12
#define KIRMIZI_LED 10
#define SARI_LED    11

bool ambulans_yakinda = false;
unsigned long son_ambulans_gorulme = 0;

// Yeni: Trafik döngüsü için kontrol değişkenleri
enum IsikDurumu { KIRMIZI, SARI, YESIL };
IsikDurumu aktif_isik = KIRMIZI;
unsigned long isik_son_degisim = 0;

void trafik_isik_baslat() {
  pinMode(YESIL_LED, OUTPUT);
  pinMode(KIRMIZI_LED, OUTPUT);
  pinMode(SARI_LED, OUTPUT);
  aktif_isik = KIRMIZI;
  isik_son_degisim = millis();
  Serial.println("🚦 Başlangıçta: KIRMIZI yanıyor.");
}

void trafik_isik_guncelle() {
  if (ambulans_yakinda) {
    // Ambulans varsa yeşil sabit yansın
    digitalWrite(YESIL_LED, HIGH);
    digitalWrite(KIRMIZI_LED, LOW);
    digitalWrite(SARI_LED, LOW);
    Serial.println("🚑 Ambulans Yakında → YEŞİL sabit.");
    // Eğer 10 saniyedir ambulans görünmüyorsa normale dön
    if (millis() - son_ambulans_gorulme > 10000) {
      ambulans_yakinda = false;
      aktif_isik = KIRMIZI;
      isik_son_degisim = millis();
      Serial.println("✅ Ambulans Uzaklaştı → Normal döngüye dönüldü.");
    }
    return; // Döngü çalıştırılmasın
  }

  // 🚥 Normal döngü çalışıyor
  unsigned long suan = millis();

  switch (aktif_isik) {
    case KIRMIZI:
      if (suan - isik_son_degisim >= 5000) { // 5 sn sonra sarıya geç
        aktif_isik = SARI;
        isik_son_degisim = suan;
        Serial.println("🟡 KIRMIZI → SARI geçişi.");
      }
      break;
    case SARI:
      if (suan - isik_son_degisim >= 3000) { // 3 sn sonra yeşile geç
        aktif_isik = YESIL;
        isik_son_degisim = suan;
        Serial.println("🟢 SARI → YEŞİL geçişi.");
      }
      break;
    case YESIL:
      if (suan - isik_son_degisim >= 5000) { // 5 sn sonra kırmızıya geç
        aktif_isik = KIRMIZI;
        isik_son_degisim = suan;
        Serial.println("🔴 YEŞİL → KIRMIZI geçişi.");
      }
      break;
  }

  // LED çıkışlarını ayarla
  digitalWrite(YESIL_LED, aktif_isik == YESIL ? HIGH : LOW);
  digitalWrite(KIRMIZI_LED, aktif_isik == KIRMIZI ? HIGH : LOW);
  digitalWrite(SARI_LED, aktif_isik == SARI ? HIGH : LOW);
}

#endif
