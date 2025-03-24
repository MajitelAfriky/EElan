#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
// #0000000000000000010100010111001001110110011111000111110001010100#60d670
#ifndef PSTR
#define PSTR // Prevent compiler error
#endif
#define PIN 8
#define NUM_LEDS 64

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 1, 1, PIN,
  NEO_TILE_BOTTOM   + NEO_TILE_LEFT   + NEO_TILE_COLUMNS   + NEO_TILE_PROGRESSIVE +
  NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB + NEO_KHZ800);
uint16_t color = matrix.Color(255, 0, 0); // Red color for text
String displayText = "."; // Default text
String hexColor;
int pip = 1;
int lever;
//
int event[16];
unsigned long startMillis = 0;
unsigned long currentMillis;
int on = false;




void setup() {
  Serial.begin(9600); // USB-C
  Serial1.begin(9600); // UART1 for ESP-01

  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(color);

  pinMode(10, INPUT_PULLUP);
  pinMode(2, OUTPUT);
}

int x = matrix.width();
String dataBuffer; // Buffer to store incoming data

void loop() {
  lever = digitalRead(10);
  if (lever == 0 ) {
    if (pip == 1) {
      tone(2, 1000, 250);
      delay(250);
      tone(2, 1500, 250);
      delay(250);
    }
    eSource();
    pip = 0;
  } else {
    if (pip == 0) {
      tone(2, 1000, 250);
      delay(250);
      tone(2, 500, 250);
      matrix.fillScreen(0);
      matrix.show();
      pip = 1;
    }
    uDmxRecieve();
  }
}

void uDmxRecieve() {
  while (Serial.available() > 0 && lever == 1) {
    
    String receivedData = Serial.readStringUntil('\n');
    char buffer[50];  // Dočasné pole pro převedení Stringu na C-string
    receivedData.toCharArray(buffer, sizeof(buffer)); //                                reaktor   paprsky    torpedo    manev      impuls      warp       jump       front      rear
    int parsedValues = sscanf(buffer, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &event[0], &event[1], &event[2], &event[3], &event[4], &event[5], &event[6], &event[7], &event[8],
                                                                                      &event[9], &event[10], &event[11], &event[12], &event[13], &event[14], &event[15] );
    //                                                                                 yellow       red       docking       dock       hull%     shieldsF%    shieldsR%
    uCases();
    lever = digitalRead(10);
  }
}




void uCases() {
  if (event[9] == 255) {//yellow          0 0 0 0 0 0 0 0 0 255 0 0 0 130 200 100
    uAni(1, 179, 149, 0);
  } else if (event[10] == 255) {//red     0 0 0 0 0 0 0 0 0 0 255 0 0 130 200 100
    uAni(1, 179, 0, 0);
  } else {
    uAni(1, 0, 0, 0);
  }
  int r;
  int g;
  uGradient(event[14], r, g);
  uAni(2, r, g, 0);//front shield
  uGradient(event[13], r, g);
  uAni(3, r, g, 0);//hull
  uGradient(event[15], r, g);
  uAni(4, r, g, 0);//rear shield

  matrix.show();



}

void uAni(int val, int r, int g, int b) {
  switch (val) {
    case 1:
    currentMillis = millis();
    if (currentMillis - startMillis >= 500)
    {
      startMillis = currentMillis;
      on = !on;
    }
    if (on) {
      matrix.fillScreen(matrix.Color(r, g, b));
    } else {
      matrix.fillScreen(0);

    }
    break;
    case 2:
      //front shield
      matrix.drawPixel(3, 0, matrix.Color(r, g, 0));
      matrix.drawPixel(4, 0, matrix.Color(r, g, 0));
      matrix.drawPixel(2, 1, matrix.Color(r, g, 0));
      matrix.drawPixel(5, 1, matrix.Color(r, g, 0));
      matrix.drawPixel(2, 2, matrix.Color(r, g, 0));
      matrix.drawPixel(5, 2, matrix.Color(r, g, 0));
    break;
    case 3:
      //hull
      for (int i = 1; i <= 5; i++){
        matrix.drawPixel(3, i, matrix.Color(r, g, 0));
        matrix.drawPixel(4, i, matrix.Color(r, g, 0));
      }
    break;
    case 4:
      //rear shield
      matrix.drawPixel(1, 7, matrix.Color(r, g, 0));
      matrix.drawPixel(6, 7, matrix.Color(r, g, 0));
      for (int i = 1; i <= 6; i++) {
        matrix.drawPixel(i, 6, matrix.Color(r, g, 0));
      }
      matrix.drawPixel(2, 5, matrix.Color(r, g, 0));
      matrix.drawPixel(5, 5, matrix.Color(r, g, 0));

    break;
    default:
      // statements
      break;
  }
}

void uGradient(int value, int &r, int &g) {//0 0 0 0 0 0 0 0 0 0 0 0 0 98 80 55
  if (value <= 90 ) {
    r = 255;
    g = 0;
  } else if (value <= 172) {
    r = 255;  // Červená
    g = map(value, 90, 172, 0, 255);  // Zelená roste od 0 do 255
  } else {
    r = map(value, 172, 255, 255, 0);  // Červená klesá od 255 do 0
    g = 255;  // Zelená
  }
}




void eSource() {
  while (Serial1.available()) {
    char c = Serial1.read();
    if (c == '\n') { // Pokud přijde nový řádek, zpracuj data
      dataBuffer.trim(); // Remove any leading or trailing whitespace
      if (dataBuffer.length() > 0) {
        displayText = dataBuffer;
        x = matrix.width(); // Reset position for new text
        matrix.fillScreen(0);
      }
      dataBuffer = ""; // Clear the buffer for next data
    } else {
      dataBuffer += c; // Add character to buffer
    }
  }

  if (displayText.startsWith("$", 0) || displayText.startsWith("#", 0)) {
    if (displayText.startsWith("red", 1)) {
      matrix.fillScreen(color);
      matrix.show();
    }
    if (displayText.startsWith("green", 1)) {
      matrix.fillScreen(matrix.Color(0, 255, 0));
      matrix.show();
    }
    if (displayText.startsWith("blue", 1)) {
      matrix.fillScreen(matrix.Color(0, 0, 255));
      matrix.show();
    }
    if (displayText.startsWith("#", 0)) {
      eClient();
    }
  } else {
    eText();
  }
}

void eClient() {
  hexColor = displayText.substring(65);
  long number = (long) strtol( &hexColor[1], NULL, 16);
  int r = number >> 16;
  int g = number >> 8 & 0xFF;
  int b = number & 0xFF;

  for (int i = 1; i <= 64; i++) {
    // Výpočet souřadnic z indexu
    int Yrow = (i - 1) / 8;
    int Xcol = (i - 1) % 8;
    // Přečtení bitu na aktuální pozici
    char bit = displayText.charAt(i);
    if (bit == '1') {
      matrix.drawPixel(Xcol, Yrow, matrix.Color(r, g, b));      
    }
  }
  matrix.show();
}

void eText() {
  matrix.fillScreen(0); // Vyčistíme matici
  matrix.setTextColor(color); // Nastavíme barvu textu
  matrix.setTextWrap(false); // Vypneme zalamování textu
  matrix.setCursor(x, 0); // Nastavíme pozici kurzoru pro text

  int textWidth = displayText.length() * 6; // Předpokládáme, že každý znak má šířku 6 pixelů
  int displayTime = textWidth * 10; // Čas zobrazení je proporcionální k délce textu

  for (int i = 0; i < textWidth + matrix.width(); i++) {
    matrix.fillScreen(0); // Vyčistíme matici
    matrix.setCursor(x - i, 0); // Nastavíme pozici kurzoru pro text
    if (x - i < -textWidth) {
      x = matrix.width(); // Resetujeme pozici, pokud text překročí matici
    }
    matrix.print(displayText); // Zobrazíme text
    matrix.show(); // Aktualizujeme matici
    delay(100); // Čekáme 100 ms
  }
  delay(displayTime); // Čekáme po dobu odpovídající délce textu
}
