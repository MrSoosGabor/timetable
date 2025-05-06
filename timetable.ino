#include "config.h"

void setup() {
  pinMode(D8, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(115200);

  wifiLoad();   // A Wifi kapcsolat beállítása, majd a frissítések ellenőrzése
  ++bootCount;  // A bootolások száma, hogy tudjuk ha nem sikerül egyből frissíteni az aktuális nap órarendjét (frissítések után visszaáll 1-re), illetve hogy sikerült-e egyből frissíteni az órarendet

  if (errorCode != 1) {            // Ha az errorCode = 1, akkor nem jött létre WiFi kapcsolat
    setClock();                    // Az aktuális dátum és idő lekérése egy időszerverről
    periodusSzam = PeriodsLoad();  // Betölti az aktuális nap időrendjét (lehet, hogy rövidített órák vannak) a /ringsystem végpontról a periodusok listába
    kartyaSzam = CardsLoad();      // Betölti az aktuális nap óráit a /cards végpontról a cards listába
    Serial.print("Peródusok száma");
    Serial.println(periodusSzam);
    Serial.print("Kártyák száma");
    Serial.println(kartyaSzam);

    //timeToSleep = (360-15);         // TESZTELÉSHEZ: 6 percenként frissít, a frissítés kb. 15 mp-ig tart  //
    setTimeToSleep();               // Beállítja a szükséges alvási időtartamot

    if (timeToSleep > 600 && !visszaAlszik) { // Ha valamilyen hiba miatt újra kell frissíteni, akkor nem küldünk adatot a merülési táblázatba ill. emailre
      int accuLevel = analogRead(A0);         // 3,3V -> 4095 (12 bit)
      sendBatteryLevel(accuLevel);            // Elküldi az aktuális elemfeszültség értékét a Google táblázatba, hogy lássuk a merülés tendenciáját

      //if (accuLevel < 2235) {                 // Ha az órarend elemfeszültsége 3,6 Volt alá esik, akkor figyelmeztető emailt küldünk.
      emailSend(accuLevel);
      //}
    }
  }

  drawing();  // Kirajzolja az E-ink kijelzőre az aktuális nap órarendjét, illetve ha hiba volt, akkor a hibát

  esp_sleep_enable_timer_wakeup(timeToSleep * uS_TO_S_FACTOR);  // Beállítja a következő ébredésig visszalévő időt
  Serial.println("Az ébredésig hátralévő idő: " + String(timeToSleep) + " másodperc");
  Serial.println("Most elalszom...");
  Serial.flush();
  esp_deep_sleep_start();  // Mélyalvásba küldi a mikrokontrollert
}

void loop(void) {}
