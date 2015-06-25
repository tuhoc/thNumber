
#include <thNumber.h>

int SDI_pin = 7;
int SCK_pin = 8;
int RCK_pin = 9;

int segments[8] = {SEG7_A, SEG7_B, SEG7_C, SEG7_D, SEG7_E, SEG7_F, SEG7_G, SEG7_DP};

int FONT7_DEGREE = SEG7_A | SEG7_B | SEG7_G | SEG7_F;
int FONT7_C      = SEG7_A | SEG7_F | SEG7_E | SEG7_D;

int counter = 0;

void setup()
{
  thNumber.begin(SDI_pin, SCK_pin, RCK_pin, 1, MANUAL_UPDATE);
}

void loop()
{
  thNumber.setBitmap(0, segments[counter]);
  thNumber.setBitmap(2, FONT7_DEGREE);
  thNumber.setBitmap(1, FONT7_C);
  thNumber.showDot(4);
  thNumber.setNum4(3, 1234);
  thNumber.update();
  
  delay(200);
  counter = (counter + 1) % 6;
}

