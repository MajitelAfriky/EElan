#include <Joystick.h>
#include <Adafruit_NeoPixel.h>
#define NEO 16
#define NEO_PIX 14
#define AXI_X 27
#define AXI_Y 28
#define AXI_Z 26
const int btn[] = {0, 0, 1, 17, 14};
const int btn1[] = {0, 10, 11, 12, 13, 6, 7, 8, 9};
int btnState[] = {0, 0, 0, 0, 0};
int btnState1[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
const int lght[] = {0, 0, 0, 0, 15};
const int lght1[] = {0, 2, 3, 4, 5, 21, 20, 19, 18};
unsigned long preMil[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned long blinkInt[] = {0, 250, 250, 250, 250, 0, 0, 0, 0};
bool ledState[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
bool light = true;
Adafruit_NeoPixel pixels(NEO_PIX, NEO, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  for (int i = 1; i < 5; i++) {
    pinMode(btn[i], INPUT_PULLUP);
  } 
  for (int i = 1; i < 9; i++) {
    pinMode(btn1[i], INPUT_PULLUP);
  } 
  pinMode(lght[4], OUTPUT);   
  for (int i = 1; i < 9; i++) {
    pinMode(lght1[i], OUTPUT);
  }  
  Joystick.begin();
  pixels.begin();
}

void loop() {
  if (light == true) {intro();}
  joystick();
  heat();
}

void heat() {

  unsigned long currentMillis = millis();

  for (int i = 1; i < 9; i++) {
    if (blinkInt[i] > 0) {
      if (currentMillis - preMil[i] >= blinkInt[i]) {
      preMil[i] = currentMillis;
      ledState[i] = !ledState[i];
      digitalWrite(lght1[i], ledState[i]);
      }
    } else {
      digitalWrite(lght1[i], true);
    }
  }

}

void joystick() {
  for (int i = 1; i < 5; i++) {
    btnState[i] = digitalRead(btn[i]);
  }
  for (int i = 1; i < 9; i++) {
    btnState1[i] = digitalRead(btn1[i]);
  }
  for (int i = 1; i < 5; i++) {
    if (btnState[i] == LOW)   {button(i, true);}  else {button(i, false);}
  }
  for (int i = 1; i < 9; i++) {
    if (btnState1[i] == LOW)  {button(i, true);} else {button(i, false);}
  }
  
  int value_x;
  if (analogRead(AXI_X) >= 280) {
    value_x = map(analogRead(AXI_X), 280, 1023, 512, 1023);
  } else {
    value_x = map(analogRead(AXI_X), 0, 280, 0, 511);
  }
  //value_x = constrain(value_x, 0, 1023);
  Joystick.X(value_x);
  Serial.print(value_x);
  Serial.print("   |   ");

  int value_y;
  if (analogRead(AXI_Y) >= 280) {
    value_y = map(analogRead(AXI_Y), 280, 1023, 511, 0);
  } else {
    value_y = map(analogRead(AXI_Y), 0, 280, 1023, 512);
  }
  //value_y = constrain(value_y, 0, 1023);
  Joystick.Y(value_y);
  Serial.print(value_y);
  Serial.print("   |   ");

  int value_z;
  if (analogRead(AXI_Z) >= 280) {
    value_z = map(analogRead(AXI_Z), 280, 1023, 511, 0);
  } else {
    value_z = map(analogRead(AXI_Z), 0, 280, 1023, 512);
  }
  //value_z = constrain(value_z, 0, 1023);
  Joystick.Z(value_z);
  Serial.println(value_z);

}

void button(int n, bool tf) {
  Joystick.button(n, tf);
}

void intro() {
  for (int i = 1; i < 9; i++) {
    digitalWrite(lght1[i], HIGH);
    delay(60);
    digitalWrite(lght1[i], LOW);
  }
  pixels.clear();
  for (int i = 0; i <= 6; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 200));
    pixels.setPixelColor(13-i, pixels.Color(0, 0, 200));
    pixels.show();
    delay(80);
  }
  pixels.clear();
  for (int i = 200; i >= 10; i--) {
    pixels.fill((0, 0, i));
    pixels.show();
    delay(5);
  }

  digitalWrite(lght[4], HIGH);
  light = false;
}