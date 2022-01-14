
#ifndef RelayModule_h
#define RelayModule_h

#include "Arduino.h"
#include <BaseModule.h>
#include <String.h>

class RelayModule : public BaseModule
{
public:
    RelayModule(uint8_t, bool, void (*callback)(String));
    void turnOn();
    void turnOff();
    void toggle();
    void doCommand(String);

private:
    uint8_t PIN;
    void (*logger)(String);
    bool currentValue;
};

#endif