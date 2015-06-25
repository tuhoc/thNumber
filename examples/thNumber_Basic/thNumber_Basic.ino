
#include <thNumber.h>

int SDI_pin = 7;
int SCK_pin = 8;
int RCK_pin = 9;

void setup()
{
  thNumber.begin(SDI_pin, SCK_pin, RCK_pin);
}

void loop()
{
  for (int minute = 0; minute < 60; minute++)
  {
    thNumber.setNum2(2, minute);
    
    for (int second = 0; second < 60; second++)
    {
      thNumber.setNum2(0, second);

      thNumber.showDot(2);
      delay(500);
      thNumber.hideDot(2);
      delay(500);
    }
  }
}

