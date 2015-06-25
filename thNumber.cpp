
#include "thNumber.h"
#include <avr/pgmspace.h>


#define  MAX_MODULES  5
#define  MAX_DIGITS  (4 * MAX_MODULES)


thNumberClass thNumber;

byte _SDI, _SCLK, _RCLK, _numDigits;
boolean _autoUpdate = false;
byte _bitmaps[MAX_DIGITS];

/*
// NOTE: shift out ~bb
void _ShiftOutInv(byte bb)
{
  uint8_t SDI_bit = digitalPinToBitMask(_SDI);
  uint8_t CLK_bit = digitalPinToBitMask(_SCLK);

  volatile uint8_t *SDI_port = portOutputRegister(digitalPinToPort(_SDI));
#if defined(__AVR_ATmega8P__) || defined(__AVR_ATmega8__)
  volatile uint8_t *CLK_port = portOutputRegister(digitalPinToPort(_SCLK));
#else
  volatile uint8_t *CLK_pin  = portInputRegister(digitalPinToPort(_SCLK));
#endif

  uint8_t oldSREG = SREG;     // Save Interrupt_Enable status
  cli();                      // Disable Interrupt - prevent changes to SDI_port

  uint8_t SDI1 = *SDI_port | SDI_bit;
  uint8_t SDI0 =  SDI1     ^ SDI_bit;     //  & ~SDI_bit

  for (byte i = 8; i > 0; i--)
  {
    if (bb & 0x80)   *SDI_port = SDI0;    // shift out ~bb
    else             *SDI_port = SDI1;    // by swapping SDI0 and SDI1

    bb = bb << 1;

#if defined(__AVR_ATmega8P__) || defined(__AVR_ATmega8__)
    *CLK_port |= CLK_bit;     // clk edge here       invert SCLK pin
    *CLK_port ^= CLK_bit;     // or here             invert SCLK pin
#else
    *CLK_pin   = CLK_bit;     // clk edge here       invert SCLK pin
    *CLK_pin   = CLK_bit;     // or here             invert SCLK pin
#endif
  }
                                                  
  SREG = oldSREG;             // Restore Interrupt_Enable status
}
*/

void thNumberClass::update()
{
  uint8_t oldSREG = SREG;     // Save Interrupt_Enable status

  uint8_t SDI_bit = digitalPinToBitMask(_SDI);
  uint8_t CLK_bit = digitalPinToBitMask(_SCLK);

  volatile uint8_t *SDI_port = portOutputRegister(digitalPinToPort(_SDI));
#if defined(__AVR_ATmega8P__) || defined(__AVR_ATmega8__)
  volatile uint8_t *CLK_port = portOutputRegister(digitalPinToPort(_SCLK));
#else
  volatile uint8_t *CLK_pin  = portInputRegister(digitalPinToPort(_SCLK));
#endif

  for (byte i = _numDigits; i > 0 ; i--)
  {
    //_ShiftOutInv(_bitmaps[i-1]);
    byte bb = _bitmaps[i-1];
    
    cli();                      // Disable Interrupt - prevent changes to SDI_port
  
    uint8_t SDI1 = *SDI_port | SDI_bit;
    uint8_t SDI0 =  SDI1     ^ SDI_bit;     //  & ~SDI_bit
  
    for (byte k = 8; k > 0; k--)
    {
      if (bb & 0x80)   *SDI_port = SDI0;    // shift out ~bb
      else             *SDI_port = SDI1;    // by swapping SDI0 and SDI1
  
      bb = bb << 1;
  
#if defined(__AVR_ATmega8P__) || defined(__AVR_ATmega8__)
      *CLK_port |= CLK_bit;     // clk edge here       invert SCLK pin
      *CLK_port ^= CLK_bit;     // or here             invert SCLK pin
#else
      *CLK_pin   = CLK_bit;     // clk edge here       invert SCLK pin
      *CLK_pin   = CLK_bit;     // or here             invert SCLK pin
#endif
    }
                                                    
    SREG = oldSREG;             // Restore Interrupt_Enable status
  }
  
  digitalWrite(_RCLK, LOW);     // Latch pin RCLK
  digitalWrite(_RCLK, HIGH);    // positive edge on RCLK
  digitalWrite(_RCLK, LOW);     // RCLK = LOW: output enable
}


void checkUpdate()
{
  if (_autoUpdate)
  {
    thNumberClass::update();
  }
}


void thNumberClass::begin(byte SDI, byte SCK, byte RCK)
{
  begin(SDI, SCK, RCK, 1, true);
}

void thNumberClass::begin(byte SDI, byte SCK, byte RCK, byte numModules)
{
  begin(SDI, SCK, RCK, numModules, true);
}

void thNumberClass::begin(byte SDI, byte SCK, byte RCK, byte numModules, boolean autoUpdate)
{
  _SDI  = SDI;
  _SCLK = SCK;
  _RCLK = RCK;
  _numDigits = min(4 * numModules, MAX_DIGITS);
  _autoUpdate = autoUpdate;

  //digitalWrite(_RCLK, HIGH);
  pinMode(_SDI,  OUTPUT);
  pinMode(_SCLK, OUTPUT);
  pinMode(_RCLK, OUTPUT);

  digitalWrite(_RCLK, LOW);
  digitalWrite(_RCLK, HIGH);  // Suon len cua tin hieu chot, resync SPI
}


void thNumberClass::setBitmap(byte pos, byte bitmap)
{
  if (pos < MAX_DIGITS)
  {
    _bitmaps[pos] = bitmap;
    checkUpdate();
  }
}


void thNumberClass::showDot(byte pos)
{
  if (pos < MAX_DIGITS)
  {
    _bitmaps[pos] |=  SEG7_DP;
    checkUpdate();
  }
}

void thNumberClass::hideDot(byte pos)
{
  if (pos < MAX_DIGITS)
  {
    _bitmaps[pos] &= ~SEG7_DP;
    checkUpdate();
  }
}


void thNumberClass::setDigit(byte pos, unsigned int num)
{
  byte oldAuto = _autoUpdate;
  _autoUpdate = false;

  static const byte DigitFont[10] PROGMEM = 
  {
    FONT7_0, FONT7_1, FONT7_2, FONT7_3, FONT7_4, 
    FONT7_5, FONT7_6, FONT7_7, FONT7_8, FONT7_9
  };

  setBitmap(pos, pgm_read_byte_near(DigitFont + (num % 10)));

  _autoUpdate = oldAuto;
  checkUpdate();
}


void thNumberClass::setNum2(byte pos, unsigned int num)
{
  byte oldAuto = _autoUpdate;
  _autoUpdate = false;

  setDigit(pos + 0, num     );
  setDigit(pos + 1, num / 10);

  _autoUpdate = oldAuto;
  checkUpdate();
}


void thNumberClass::setNum4(byte pos, unsigned int num)
{
  byte oldAuto = _autoUpdate;
  _autoUpdate = false;

  setNum2(pos + 0, num    );
  setNum2(pos + 2, num/100);

  _autoUpdate = oldAuto;
  checkUpdate();
}


void thNumberClass::setNum3(byte pos, unsigned int num)
{
  byte oldAuto = _autoUpdate;
  _autoUpdate = false;

  setNum2 (pos + 0, num    );
  setDigit(pos + 2, num/100);

  _autoUpdate = oldAuto;
  checkUpdate();
}
