#include <Joystick.h>
#include <Adafruit_NeoPixel.h>
#define NEO 16
#define NEO_PIX 14
#define AXI_X 27
#define AXI_Y 28
#define AXI_Z 26
const int btn[] = {0, 0, 1, 17, 14};
const int btn1[] = {0, 10, 11, 12, 13, 6, 7, 8, 9};
int btnState[4];// = {0, 0, 0, 0, 0};
int btnState1[9];// = {0, 0, 0, 0, 0, 0, 0, 0, 0};
const int lght[] = {0, 0, 0, 0, 15};
const int lght1[] = {0, 2, 3, 4, 5, 21, 20, 19, 18};
unsigned long preMil[9];// = {0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned long warMil;
int blinkVal[10];// = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int blinkInt[10];// = {0, 250, 250, 250, 250, 0, 0, 0, 0};
bool ledState[9];// = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int eventVal[6];
bool light = true;
bool on = true;

Adafruit_NeoPixel pixels(NEO_PIX, NEO, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  for (int i = 1; i <= 4; i++) {
    pinMode(btn[i], INPUT_PULLUP);
  } 
  for (int i = 1; i <= 8; i++) {
    pinMode(btn1[i], INPUT_PULLUP);
  } 
  pinMode(lght[4], OUTPUT);   
  for (int i = 1; i <= 8; i++) {
    pinMode(lght1[i], OUTPUT);
  }  
  Joystick.begin();
  pixels.begin();
}

void loop() {
if (light == true) {intro();}
  dmx();
  joystick();
  heat();
}
void dmx() {

  if (Serial.available() > 0) {
    String receivedData = Serial.readStringUntil('\n');
    char buffer[50];  // Dočasné pole pro převedení Stringu na C-string
    receivedData.toCharArray(buffer, sizeof(buffer)); //                                reaktor       paprsky       torpedo        manev        impuls         warp           jump          front         rear
    int parsedValues = sscanf(buffer, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &blinkVal[1], &blinkVal[2], &blinkVal[3], &blinkVal[4], &blinkVal[5], &blinkVal[6], &blinkVal[20], &blinkVal[7], &blinkVal[8],
                                                                                      &eventVal[0], &eventVal[1], &eventVal[2], &eventVal[3], &eventVal[4], &eventVal[5], &eventVal[6] );
  } else {//                                                                             yellow         red         docking        dock          hull%       shieldsF%     shieldsR%
    for (int i = 1; i <= 9; i++) {
        blinkVal[i] = 0;
    }
    blinkVal[20] = 0;
  }
  if (blinkVal[1] > 170) {
    for (int i = 1; i <= 9; i++) {
      blinkInt[i] = 80;
    }
    } else {
      for (int i = 2; i <= 9; i++) {
        if (blinkVal[i] > 60) {
          blinkInt[i] = map(blinkVal[i],60, 255,500,80);
        } else if (blinkVal[i] > 50) {
          blinkInt[i] = 500;
        } else {
          blinkInt[i] = 0;
        }
      }
      if (blinkVal[20] > 60) {
        blinkInt[20] = map(blinkVal[20],60, 255,500,80);
      } else if (blinkVal[20] > 50) {
        blinkInt[20] = 500;
      } else {
        blinkInt[20] = 0;
      }
      if (blinkVal[1] > 60) {
        blinkInt[1] = map(blinkVal[1],60, 170,500,80);
      } else if (blinkVal[1] > 50) {
        blinkInt[1] = 500;
      } else {
        blinkInt[1] = 0;
      }
      blinkInt[6] = blinkInt[6] + blinkInt[20];
  }
}

void heat() {

  unsigned long currentMillis = millis();

  for (int i = 1; i <= 9; i++) {
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
  if (eventVal[0] == 255 || eventVal[1] == 255) {
    if (currentMillis - warMil >= 400){
      warMil = currentMillis;
      on = !on;
    }
    if (on) {
      if(eventVal[0] == 255) {
        pixels.fill(pixels.Color(15, 15, 0));
        pixels.show();
      }else if (eventVal[1] == 255) {
        pixels.fill(pixels.Color(30, 0, 0));
        pixels.show();
      }
    } else {
        pixels.fill(pixels.Color(0, 0, 0));
        pixels.show();
    }
  } else {
    pixels.fill(pixels.Color(0, 0, 20));
    pixels.show();
  }
}



void joystick() {
  for (int i = 1; i <= 4; i++) {
    btnState[i] = digitalRead(btn[i]);
  }
  for (int i = 1; i <= 8; i++) {
    btnState1[i] = digitalRead(btn1[i]);
  }
  for (int i = 1; i <= 4; i++) {
    if (btnState[i] == LOW)   {button(i, true);}  else {button(i, false);}
  }
  for (int i = 1; i <= 8; i++) {
    if (btnState1[i] == LOW)  {button(10 + i, true);} else {button(10 + i, false);}
  }
  
  int value_x;
  if (analogRead(AXI_X) >= 280) {
    value_x = map(analogRead(AXI_X), 280, 1023, 512, 1023);
  } else {
    value_x = map(analogRead(AXI_X), 0, 280, 0, 511);
  }
  //value_x = constrain(value_x, 0, 1023);
  Joystick.X(value_x);
  int value_y;
  if (analogRead(AXI_Y) >= 280) {
    value_y = map(analogRead(AXI_Y), 280, 1023, 511, 0);
  } else {
    value_y = map(analogRead(AXI_Y), 0, 280, 1023, 512);
  }
  //value_y = constrain(value_y, 0, 1023);
  Joystick.Y(value_y);


  int value_z;
  if (analogRead(AXI_Z) >= 280) {
    value_z = map(analogRead(AXI_Z), 280, 1023, 511, 0);
  } else {
    value_z = map(analogRead(AXI_Z), 0, 280, 1023, 512);
  }
  //value_z = constrain(value_z, 0, 1023);
  Joystick.Z(value_z);

}

void button(int n, bool tf) {
  Joystick.button(n, tf);
}

void intro() {
  for(int j = 1; j < 3; j++) {
    for (int i = 1; i <= 4; i++) {
      digitalWrite(lght1[i], HIGH);
      delay(60);
    }
    for (int i = 8; i >= 5; i--) {
      digitalWrite(lght1[i], HIGH);
      delay(60);
    }
    for (int i = 1; i <= 4; i++) {
      digitalWrite(lght1[i], LOW);
      delay(60);
    }
    for (int i = 8; i >= 5; i--) {
      digitalWrite(lght1[i], LOW);
      delay(60);
    }
  }
    for (int i = 1; i <= 4; i++) {
      digitalWrite(lght1[i], HIGH);
      delay(60);
    }
    for (int i = 8; i >= 5; i--) {
      digitalWrite(lght1[i], HIGH);
      delay(60);
    }
  pixels.clear();
  for (int i = 0; i <= 6; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 200));
    pixels.setPixelColor(13-i, pixels.Color(0, 0, 200));
    pixels.show();
    delay(80);
  }
  for (int i = 0; i <= 6; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 20));
    pixels.setPixelColor(13-i, pixels.Color(0, 0, 20));
    pixels.show();
    delay(80);
  }
  digitalWrite(lght[4], HIGH);
  light = false;
}
