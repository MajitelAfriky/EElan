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
int event[15];



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
    int parsedValues = sscanf(buffer, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &event[0], &event[1], &event[2], &event[3], &event[4], &event[5], &event[6], &event[7], &event[8],
                                                                                      &event[9], &event[10], &event[11], &event[12], &event[13], &event[14] );
    //                                                                                 yellow       red       docking       dock        hull       shields
    uCases();
    lever = digitalRead(10);
  }
}




void uCases() {
  if (event[9] == 255) {//yellow
    uAni(9,174);
  }
  if (event[10] == 255) {//red
    uAni(9,0);
  }
  if (event[14] == 255) {//shields
    uAni(14,0);
  }
  matrix.show();


}

void uAni(int val, int g) {
  switch (val) {
    case 9:
      for (int i = 0; i < 2; i++ ) {
        for (int j = 0 j < 7; j++ ) {
          matrix.drawPixel(i, j, matrix.Color(255, g, 0));
        }
      }
      /*for (int i = 5; i < 7; i++ ) {
        for (int j = 0; j < 7; j++ ) {
          matrix.drawPixel(i, j, matrix.Color(255, g, 0));
        }
      }
      for (int i = 0; i < 7; i++ ) {
        for (int j = 0; j < 2; j++ ) {
          matrix.drawPixel(i, j, matrix.Color(255, g, 0));
        }
      }
      for (int i = 0; i < 7; i++ ) {
        for (int j = 5; j <= 7; j++ ) {
          matrix.drawPixel(i, j, matrix.Color(255, g, 0));
        }
      }*/
      break;
    case 10:


      break;
    case 14:
      matrix.drawPixel(0, 0, matrix.Color(0, 0, 255));
      matrix.drawPixel(0, 1, matrix.Color(0, 0, 255));
      matrix.drawPixel(0, 2, matrix.Color(0, 0, 255));
      matrix.drawPixel(1, 0, matrix.Color(0, 0, 255));
      matrix.drawPixel(1, 1, matrix.Color(0, 0, 255));
      matrix.drawPixel(2, 0, matrix.Color(0, 0, 255));
      matrix.drawPixel(0, 7, matrix.Color(0, 0, 255));
      matrix.drawPixel(0, 6, matrix.Color(0, 0, 255));
      matrix.drawPixel(0, 5, matrix.Color(0, 0, 255));
      matrix.drawPixel(1, 7, matrix.Color(0, 0, 255));
      matrix.drawPixel(1, 6, matrix.Color(0, 0, 255));
      matrix.drawPixel(2, 7, matrix.Color(0, 0, 255));
      matrix.drawPixel(7, 0, matrix.Color(0, 0, 255));
      matrix.drawPixel(7, 1, matrix.Color(0, 0, 255));
      matrix.drawPixel(7, 2, matrix.Color(0, 0, 255));
      matrix.drawPixel(6, 0, matrix.Color(0, 0, 255));
      matrix.drawPixel(6, 1, matrix.Color(0, 0, 255));
      matrix.drawPixel(5, 0, matrix.Color(0, 0, 255));
      matrix.drawPixel(7, 7, matrix.Color(0, 0, 255));
      matrix.drawPixel(7, 6, matrix.Color(0, 0, 255));
      matrix.drawPixel(7, 5, matrix.Color(0, 0, 255));
      matrix.drawPixel(6, 7, matrix.Color(0, 0, 255));
      matrix.drawPixel(6, 6, matrix.Color(0, 0, 255));
      matrix.drawPixel(5, 7, matrix.Color(0, 0, 255));
      break;
    default:



      break;
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
