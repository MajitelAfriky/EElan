#include <Joystick.h>
#include <Adafruit_NeoPixel.h>
#define BTN1 0
#define BTN2 1
#define BTN3 17
#define BTN4 14
#define BTN11 10
#define BTN12 11
#define BTN13 12
#define BTN14 13
#define BTN15 6
#define BTN16 7
#define BTN17 8
#define BTN18 9
#define LGHT11 2
#define LGHT12 3
#define LGHT13 4
#define LGHT14 5
#define LGHT15 21
#define LGHT16 20
#define LGHT17 19
#define LGHT18 18
#define LGHT4 15
#define NEO 16
#define NEO_PIX 14
#define AXI_X 27
#define AXI_Y 28
#define AXI_Z 26
Adafruit_NeoPixel pixels(NEO_PIX, NEO, NEO_GRB + NEO_KHZ800);
int btn1_state;
int btn2_state;
int btn3_state;
int btn4_state;
int btn11_state;
int btn12_state;
int btn13_state;
int btn14_state;
int btn15_state;
int btn16_state;
int btn17_state;
int btn18_state;
bool light = true;

void setup() {
  Serial.begin(115200);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(BTN4, INPUT_PULLUP);  
  pinMode(BTN11, INPUT_PULLUP);
  pinMode(BTN12, INPUT_PULLUP);
  pinMode(BTN13, INPUT_PULLUP);
  pinMode(BTN14, INPUT_PULLUP);
  pinMode(BTN15, INPUT_PULLUP);
  pinMode(BTN16, INPUT_PULLUP);
  pinMode(BTN17, INPUT_PULLUP);
  pinMode(BTN18, INPUT_PULLUP);
  pinMode(LGHT11, OUTPUT);
  pinMode(LGHT12, OUTPUT);
  pinMode(LGHT13, OUTPUT);
  pinMode(LGHT14, OUTPUT);
  pinMode(LGHT15, OUTPUT);
  pinMode(LGHT16, OUTPUT);
  pinMode(LGHT17, OUTPUT);
  pinMode(LGHT18, OUTPUT);  
  pinMode(LGHT4, OUTPUT);   
  Joystick.begin();
  pixels.begin();
}

void loop() {
  if (light == true) {intro();}
  joystick();
}

void joystick() {
  btn1_state = digitalRead(BTN1);
  btn2_state = digitalRead(BTN2);
  btn3_state = digitalRead(BTN3);
  btn4_state = digitalRead(BTN4);  
  btn11_state = digitalRead(BTN11);
  btn12_state = digitalRead(BTN12);
  btn13_state = digitalRead(BTN13);
  btn14_state = digitalRead(BTN14);
  btn15_state = digitalRead(BTN15);
  btn16_state = digitalRead(BTN16);
  btn17_state = digitalRead(BTN17);
  btn18_state = digitalRead(BTN18);

  if (btn1_state == LOW)   {ifBtn(1);}  else {elseBtn(1);}
  if (btn2_state == LOW)   {ifBtn(2);}  else {elseBtn(2);}
  if (btn3_state == LOW)   {ifBtn(3);}  else {elseBtn(3);}
  if (btn4_state == LOW)   {ifBtn(4);}  else {elseBtn(4);}
  if (btn11_state == LOW)  {ifBtn(11);} else {elseBtn(11);}
  if (btn12_state == LOW)  {ifBtn(12);} else {elseBtn(12);}
  if (btn13_state == LOW)  {ifBtn(13);} else {elseBtn(13);}
  if (btn14_state == LOW)  {ifBtn(14);} else {elseBtn(14);}
  if (btn15_state == LOW)  {ifBtn(15);} else {elseBtn(15);}
  if (btn16_state == LOW)  {ifBtn(16);} else {elseBtn(16);}
  if (btn17_state == LOW)  {ifBtn(17);} else {elseBtn(17);}
  if (btn18_state == LOW)  {ifBtn(18);} else {elseBtn(18);}
  
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

void ifBtn(int n) {
  Joystick.button(n, true);
}

void elseBtn(int n) {
  Joystick.button(n, false);
}

void intro() {
  for (int i = LGHT11; i <= LGHT14; i++) {
    digitalWrite(i, HIGH);
    delay(60);
    digitalWrite(i, LOW);
  }
  for (int i = LGHT18; i <= LGHT15; i++) {
    digitalWrite(i, HIGH);
    delay(60);
    digitalWrite(i, LOW);
  }
  for (int i = LGHT11; i <= LGHT14; i++) {
    digitalWrite(i, HIGH);
    delay(60);
  }
  for (int i = LGHT18; i <= LGHT15; i++) {
    digitalWrite(i, HIGH);
    delay(60);
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

  digitalWrite(LGHT4, HIGH);
  light = false;
}