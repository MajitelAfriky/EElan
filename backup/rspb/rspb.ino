void setup() {
  // Inicializace sériové komunikace s počítačem
  Serial.begin(115200);

  // Inicializace sériové komunikace s ESP-01
  Serial1.begin(9600);
}

void loop() {
  if (Serial1.available()) {
    // Čtení dat ze sériového portu ESP-01
    String data = "";
    while (Serial1.available()) {
      char c = (char)Serial1.read();
      data += c;
    }
    // Zobrazení dat na sériovém monitoru
    Serial.print(data);
  }
}