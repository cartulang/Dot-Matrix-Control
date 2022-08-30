// Program to exercise the MD_Parola library
//
// Display text using various fonts.
//
// MD_MAX72XX library can be found at https://github.com/MajicDesigns/MD_MAX72XX
//

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "Fonts.h"


// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN   13
#define DATA_PIN  12
#define CS_PIN    10
#define MAX_STRING_LENGTH 8

MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// Global variables
typedef struct
{
  MD_MAX72XX::fontType_t *pFont;
} font_t;


const font_t F[] =
{
  { defaultFont },
  { secondFont },
  { thirdFont }
};

const textEffect_t E[] {
  PA_NO_EFFECT,
  PA_SCROLL_LEFT,
  PA_SCROLL_RIGHT,
  PA_SCROLL_UP,
  PA_SCROLL_DOWN
};

uint8_t curF = 0;  // current message definition to use
uint8_t curE = 0;
textPosition_t scrollAlign = PA_LEFT;


void setup(void) {
  Serial.begin(9600);
  Serial.println("Hello!");
  P.begin();
  P.setFont(F[curF].pFont);
}

void loop(void) {

  if (P.displayAnimate()) {
    P.displayReset();
  }
  readSerial();
}


void readSerial() {
  while (Serial.available() > 0) {
    static char newStr[MAX_STRING_LENGTH];
    static unsigned int charPos = 0;
    char input = Serial.read();

    if (input == '%' || input == '^' || input == '&' || input == '*' || input == '(') {
      setAnimationDirection(input);
      return;
    }

    if (input == '#') {
      changeFont();
      return;
    }

    if (input == '$') {
      displayStr("");
      return;
    }

    if (input != '\n' && input != ' ') {
      newStr[charPos] = input;
      charPos++;
    }

    else {
      newStr[charPos] = '\0';
      displayStr(newStr);
      charPos = 0;
    }
  }
}

void displayStr(char str[]) {
  int animationSpeed = 25;
  int pauseTime = 2000;
  P.displayClear();
  P.displayText(str, scrollAlign, animationSpeed, pauseTime, E[curE], E[curE]);
}
void changeFont() {
  curF++;

  if (curF > 2 || curF < 0) {
    curF = 0;
  }
  P.displayReset();
  P.setFont(F[curF].pFont);
}

void setAnimationDirection(char dir) {
  P.displayReset();
  switch (dir) {
    case '^':
      curE = 1;
      break;
    case '&':
      curE = 2;
      break;
    case '*':
      curE = 3;
      break;
    case '(':
      curE = 4;
      break;
    default:
      curE = 0;
      break;
  }
  P.setTextEffect(E[curE], E[curE]);
}
