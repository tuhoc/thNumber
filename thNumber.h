
#ifndef THNUMBER_H
#define THNUMBER_H

#include "Arduino.h"

class thNumberClass 
{
  public:
	  static void begin(byte SDI, byte SCK, byte RCK);
    static void begin(byte SDI, byte SCK, byte RCK, byte numModules);
    static void begin(byte SDI, byte SCK, byte RCK, byte numModules, boolean autoUpdate);
    	
    static void setBitmap(byte pos, byte bitmap);
    
    static void setDigit(byte pos, unsigned int num);
    static void setNum2 (byte pos, unsigned int num);
	  static void setNum3 (byte pos, unsigned int num);
    static void setNum4 (byte pos, unsigned int num);
	  
    static void showDot(byte pos);
    static void hideDot(byte pos);
       
    static void update();
};

extern thNumberClass thNumber;

#define  SEG7_A    _BV(3)
#define  SEG7_B    _BV(5)
#define  SEG7_C    _BV(7)
#define  SEG7_D    _BV(1)
#define  SEG7_E    _BV(2)
#define  SEG7_F    _BV(4)
#define  SEG7_G    _BV(0)
#define  SEG7_DP   _BV(6)
#define  SEG7_NONE  0x00
#define  SEG7_ALL   0xFF

#define  FONT7_0  (SEG7_A | SEG7_B | SEG7_C | SEG7_D | SEG7_E | SEG7_F         )
#define  FONT7_1  (         SEG7_B | SEG7_C                                    )
#define  FONT7_2  (SEG7_A | SEG7_B |          SEG7_D | SEG7_E          | SEG7_G)
#define  FONT7_3  (SEG7_A | SEG7_B | SEG7_C | SEG7_D                   | SEG7_G)
#define  FONT7_4  (         SEG7_B | SEG7_C |                   SEG7_F | SEG7_G)
#define  FONT7_5  (SEG7_A |          SEG7_C | SEG7_D |          SEG7_F | SEG7_G)
#define  FONT7_6  (SEG7_A |          SEG7_C | SEG7_D | SEG7_E | SEG7_F | SEG7_G)
#define  FONT7_7  (SEG7_A | SEG7_B | SEG7_C                                    )
#define  FONT7_8  (SEG7_A | SEG7_B | SEG7_C | SEG7_D | SEG7_E | SEG7_F | SEG7_G)
#define  FONT7_9  (SEG7_A | SEG7_B | SEG7_C | SEG7_D |          SEG7_F | SEG7_G)

#define  AUTO_UPDATE    (true)
#define  MANUAL_UPDATE  (false)

#endif
