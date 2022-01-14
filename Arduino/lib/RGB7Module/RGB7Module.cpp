#include "Arduino.h"
#include <BaseModule.h>
#include <String.h>
#include "RGB7Module.h"
void nullLogger(String){};
RGB7Module::RGB7Module(uint8_t red, uint8_t green, uint8_t blue, void (*callback)(String) = nullLogger)
{
    logger = callback;
    PIN_RED = red;
    PIN_GREEN = green;
    PIN_BLUE = blue;
}
void RGB7Module::setColor(uint8_t red, uint8_t green, uint8_t blue)
{
    analogWrite(PIN_RED, red);
    analogWrite(PIN_GREEN, green);
    analogWrite(PIN_BLUE, blue);
    logger("RGB color changed");
}
void RGB7Module::doCommand(String command)
{

    String holder = command;
    char delimiter = ',';
    int pos = 0;
    int itemIndex = 0;
    int red = 0;
    int green = 0;
    int blue = 0;
    int fullLength = holder.length() + 1;
    while (holder.length() > 0)
    {
        // printToSerial("log", holder);

        pos = holder.indexOf(delimiter);
        if (pos == -1)
            pos = fullLength;
        String temp = holder.substring(0, pos);
        switch (itemIndex++)
        {
        case 0:
            red = toInt(temp.c_str());
            break;
        case 1:
            green = toInt(temp.c_str());
            break;
        case 2:
            blue = toInt(temp.c_str());
            break;
        }

        holder.remove(0, pos + 1);
    }
    setColor(red, green, blue);
}

int RGB7Module::toInt(const char *input)
{

    int x = 0;
    sscanf(input, "%d", &x);
    return x;
}