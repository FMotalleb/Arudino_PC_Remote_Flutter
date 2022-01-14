#include "Arduino.h"
#include <BaseModule.h>
#include <String.h>
#include "RelayModule.h"
void temp(String)
{
}
RelayModule::RelayModule(uint8_t pin, bool bootValue = false, void (*callback)(String) = temp)
{
    logger = callback;
    currentValue = bootValue;
    pinMode(pin, OUTPUT);
    PIN = pin;
    if (currentValue)
    {
        turnOn();
    }
    else
    {
        turnOff();
    }
}
void RelayModule::turnOn()
{
    analogWrite(PIN, 1024);
    currentValue = true;
    logger("turned On");
}
void RelayModule::turnOff()
{
    analogWrite(PIN, 0);
    currentValue = false;
    logger("turned Off");
}
void RelayModule::toggle()
{
    currentValue = !currentValue;
    if (currentValue)
    {
        turnOn();
    }
    else
    {
        turnOff();
    }
}

void RelayModule::doCommand(String command)
{

    if (command.startsWith("on"))
    {
        turnOn();
    }
    else if (command.startsWith("off"))
    {
        turnOff();
    }
}