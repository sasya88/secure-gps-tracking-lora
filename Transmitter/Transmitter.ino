#include <SPI.h>
#include <LoRa.h>
#include <TinyGPS++.h>
#include <AESLib.h>
#include <base64.h>  // Include the Arduino base64 library

TinyGPSPlus gps;
HardwareSerial gpsSerial(2);

byte aes_key[] = "1234567890123456"; // 16-byte key for AES
AESLib aes;
byte aes_iv[16] = {0}; // 16-byte IV (static for now)

String encryptData(String plainText) {
  char encryptedText[128];
  int encryptedLength = aes.encrypt64(
    (byte*)plainText.c_str(),
    plainText.length(),
    encryptedText,
    aes_key,
    128,
    aes_iv
  );
  if (encryptedLength > 0) {
    return base64::encode(encryptedText, encryptedLength);
  } else {
    return "Encryption Failed";
  }
}

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  LoRa.setPins(5, 14, 26);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed!");
    while (1);
  }

  Serial.println("LoRa GPS transmitter ready.");
}

void loop() {
  if (gpsSerial.available()) {
    char c = gpsSerial.read();
    Serial.print(c);

    if (gps.encode(c) && gps.location.isValid()) {
      String lat = String(gps.location.lat(), 6);
      String lng = String(gps.location.lng(), 6);
      unsigned long timestamp = millis();

      String payload = lat + "," + lng + "|" + String(timestamp);
      String encryptedPayload = encryptData(payload);

      Serial.println("Encrypted + Base64: " + encryptedPayload);

      LoRa.beginPacket();
      LoRa.print(encryptedPayload);
      LoRa.endPacket();

      Serial.println("Sent via LoRa.");
    }
  }

  delay(1000);
}
