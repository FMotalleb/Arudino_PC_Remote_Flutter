#ifndef ControlPanel_h
#define ControlPanel_h

#include <LiquidCrystal.h>
#include <string.h>
void printToLCD(LiquidCrystal, String, String = "");
void clearLCD();
class LCDController
{
public:
    LCDController(LiquidCrystal _lcd = LiquidCrystal(2, 3, 9, 10, 11, 12));
    void printToLCD(String, String = "");
    void clearLCD();

private:
    LiquidCrystal lcd;
};

#endif