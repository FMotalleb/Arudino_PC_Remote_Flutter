
#ifndef RGB7Module_h
#define RGB7Module_h

#include "Arduino.h"
#include <BaseModule.h>
#include <String.h>

class RGB7Module : public BaseModule
{
public:
    RGB7Module(uint8_t, uint8_t, uint8_t, void (*callback)(String));
    void doCommand(String);
    void setColor(uint8_t, uint8_t, uint8_t);

private:
    uint8_t PIN_RED;
    uint8_t PIN_GREEN;
    uint8_t PIN_BLUE;
    int toInt(const char *);
    void (*logger)(String);
};

#endif
