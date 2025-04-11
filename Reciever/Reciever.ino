#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <AESLib.h>
#include <base64.h>

const char* ssid = "JYO";
const char* password = "password";
const char* serverUrl = "http://your-server-ip:5000/update-location";

byte aes_key[] = "1234567890123456";
AESLib aes;
byte aes_iv[16] = {0};

String decryptData(String base64Input) {
  String decoded = base64::decode(base64Input);
  char decryptedText[128];

  int decryptedLength = aes.decrypt64(
    (byte*)decoded.c_str(),
    decoded.length(),
    decryptedText,
    aes_key,
    128,
    aes_iv
  );

  if (decryptedLength > 0) {
    decryptedText[decryptedLength] = '\0';
    return String(decryptedText);
  } else {
    return "Decryption Failed";
  }
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected.");

  LoRa.setPins(5, 14, 26);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed!");
    while (1);
  }

  Serial.println("LoRa receiver ready.");
}

void sendToServer(String gpsData) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    int commaIndex = gpsData.indexOf(',');
    String lat = gpsData.substring(0, commaIndex);
    String lng = gpsData.substring(commaIndex + 1);

    String json = "{\"latitude\":\"" + lat + "\",\"longitude\":\"" + lng + "\"}";

    int code = http.POST(json);
    Serial.println(code > 0 ? "Sent to server" : "HTTP Error");
    http.end();
  }
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String encryptedBase64 = "";
    while (LoRa.available()) {
      encryptedBase64 += (char)LoRa.read();
    }

    Serial.println("Received: " + encryptedBase64);
    String decrypted = decryptData(encryptedBase64);
    Serial.println("Decrypted: " + decrypted);

    int sep = decrypted.indexOf('|');
    if (sep == -1) {
      Serial.println("Invalid format");
      return;
    }

    String gpsData = decrypted.substring(0, sep);
    unsigned long sentTime = decrypted.substring(sep + 1).toInt();

    if (millis() - sentTime < 5000) {
      sendToServer(gpsData);
    } else {
      Serial.println("Stale data (replay attempt?).");
    }
  }
}
