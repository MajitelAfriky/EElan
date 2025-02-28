#include <ESP8266WiFi.h>

const char* ssid = "AFRIKA";
const char* password = "lolopolo";
const char* host = "sluk.borec.cz";

void setup() {
  Serial.begin(9600); // Inicializace sériové komunikace
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  WiFiClient client;
  const int httpPort = 80;

  if (!client.connect(host, httpPort)) {
    delay(1000);
    return;
  }

  client.print(String("GET ") + "/esp/esp.php" + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  delay(500);

  String response = "";
  boolean httpMain = false; // Indikátor, že jsme ve vnitřním obsahu <main>
  boolean captureContent = false; // Indikátor, že zpracováváme obsah <main>

  while (client.available()) {
    char c = client.read();
    if (c == '<') {
      if (client.readStringUntil('>') == "main") { // Pokud jsme našli tag <main>
        httpMain = true;
        captureContent = true;
      } else {
        captureContent = false; // Ukončení záznamu obsahu po každém tagu
      }
    } else if (c == '>' && httpMain) {
      httpMain = false;
    } else if (captureContent) {
      response += c;
    }
  }

  response.trim(); // Odstranění bílých znaků ze začátku a konce řetězce

  Serial.println(response); // Výpis čistého obsahu <main> na jednom řádku

  delay(5000); // Opakování každých 10 sekund
}
