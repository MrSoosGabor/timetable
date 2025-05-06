#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32httpUpdate.h>

//const char* ssid = "jcs";
//const char* password = "Jcsengo2023";
//const char* ssid = "IoT005705";
//const char* password = "WP1x5EUZgXWj2";
const char* ssid = "ASUS";
const char* password = "KalapocskaNapocska";
//const char* ssid = "Xiaomi 11T";
//const char* password = "kabalababa";

const char* fwImageURL = "http://zeus.jedlik.eu:8000/timetable.ino.bin";
const char* fwVersionURL = "http://zeus.jedlik.eu:8000/version";
//const char* fwImageURL = "https://timetable-jedlik.koyeb.app/update.bin";
//const char* fwVersionURL = "https://timetable-jedlik.koyeb.app/version";

void check_OTA() {
  HTTPClient http;
  if (http.begin(fwVersionURL)) {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      String newFWVersion = http.getString();
      float newVersion = newFWVersion.toFloat();
      Serial.print("A régi verzió: ");
      Serial.println(FW_VERSION);
      Serial.print("Az új verzió: ");
      Serial.println(newVersion);
      if (newVersion > FW_VERSION) {
        delay(100);
        Serial.println(F("Frissítés folyamatban..."));
        t_httpUpdate_return ret = ESPhttpUpdate.update(fwImageURL); // Telepíti a letöltött timetable.ino.bin firmware-t, majd újraindítja a mikrokontrollert
        // Az újraindulást követően a soros monitoron megjelenik a "ets Jun 8 2016 00:22:57" üzenet. Ez a ROM bejelentkezési üzenete; ez az első dolog, ami kinyomtatódik, miután az ESP32 bekapcsol és a CPU elindul.
      }
    }
  }  // end if http.begin
}  // end check_OTA()

void wifiLoad() {
  int szamlalo = 0;
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);
  Serial.println(F("Connecting"));
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
    szamlalo++;
    if(szamlalo >= 40) {
      errorCode = 1;
      return;
    }
  }
  Serial.println("");

  Serial.print(F("Connect to WiFi network with IP Address: "));
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.print(F("MAC Address: "));
  String mac = WiFi.macAddress();
  Serial.println(mac);
  if (mac == "24:DC:C3:81:FD:F4") Terem = 102;
  else if (mac == "24:DC:C3:8D:71:F8") Terem = 103;
  else if (mac == "3C:8A:1F:54:DF:10") Terem = 114;
  else if (mac == "94:54:C5:60:06:98") Terem = 115;
  else if (mac == "3C:8A:1F:54:1C:84") Terem = 116;
  else if (mac == "3C:8A:1F:54:03:28") Terem = 117;
  else if (mac == "3C:8A:1F:54:29:0C") Terem = 202;
  else if (mac == "3C:8A:1F:51:75:F8") Terem = 203;
  else if (mac == "3C:8A:1F:54:96:20") Terem = 302;
  else if (mac == "3C:8A:1F:51:5F:0C") Terem = 303;
  // ... ide jön a többi terem majd
  else Terem = 41;

  check_OTA();
}
