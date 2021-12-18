#include "ControlPanel.h"

#include <LiquidCrystal.h>
#include <string.h>
#include "Arduino.h"

class LCDController
{
private:
    LiquidCrystal lcd = LiquidCrystal(2, 3, 9, 10, 11, 12);

public:
    LCDController(LiquidCrystal _lcd)
    {
        lcd = _lcd;
    }
    void printToLCD(String firstLine, String secondLine = "")
    {
        lcd.clear();
        if (secondLine == "")
        {
            lcd.print(firstLine);
        }
        else
        {
            lcd.print(firstLine + ":" + secondLine);
        }
    }
    void clearLCD()
    {
        lcd.clear();
    }
};
