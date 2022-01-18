
#include <SPI.h>
#include <IRremote.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>
#include <SoftwareSerial.h>
#include <Ticker.h>
// #include <RGB7Module.h>
#include <RelayModule.h>
// #include <virtuabotixRTC.h>
// virtuabotixRTC myRTC(5, 7, 10);

using namespace std;

void printToLCD(String, String);
void clearLCD();
void printToSerial(String, String);
Ticker clearLCDTicker(clearLCD, 3500, 0, MILLIS);
SoftwareSerial blueToothSlaveSerial(2, 3); // RX, TX

int LOG_LEVEL_STATE = 1;
void logger(String data)
{

    if (LOG_LEVEL_STATE >= 1)
    {
        printToSerial("log", data);
    }
    if (LOG_LEVEL_STATE >= 2)
    {
        printToLCD("log", data);
    }
}
// Relay
RelayModule relay0(A0, false, logger);
// RGB module connected to red=A1 green=A2 blue=A3
// RGB7Module RGBModule(A1, A2, A3, logger);

/// 0 : internal commands
/// 1 : serial command mode
int WORKING_MODE = 0;

// volume
// int volumePin = A0;
// int minimumInput = 45;                 // The minimum
// int maximumInput = 920 - minimumInput; // The maximum
// int lastVolumeValue = 0;
// IR remote
int RECV_PIN = 8;
IRrecv irReceiver(RECV_PIN);
decode_results results;

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);
bool hasBacklight = true;
String lastPrint = "";
String lastPrintLine2 = "";

// ANCHOR Int To String
String toString(int value)
{
    String result = "";

    if (value == 0)
    {
        return "0";
    }
    else if (value < 0)
    {
        value = value * -1;
        result += "-";
    }
    int buffer = value;
    while (buffer > 0)
    {
        int cache = buffer % 10 + 48;
        result = (char)cache + result;
        buffer = buffer / 10;
    }
    return result;
}

// ANCHOR LiquidCrystal LCD Methodes
void printToLCD(String firstLine, String secondLine = "")
{
    lcd.backlight();
    lcd.clear();
    lcd.print(firstLine);
    lcd.setCursor(0, 1);
    lcd.print(secondLine);

    if (firstLine == "")
    {
        lcd.clear();
    }
    clearLCDTicker.stop();
    clearLCDTicker.start();
}

void clearLCD()
{
    lcd.clear();
    lcd.noBacklight();
}
void showWorkingMode()
{
    if (WORKING_MODE == 0)
    {
        printToLCD("Working Mode", "internal");
    }
    else if (WORKING_MODE == 1)
    {
        printToLCD("Working Mode", "serial");
    }
}
void handleInternalCommands(int command)
{
    if (WORKING_MODE == 0)
    {
        // RGBModule.setColor(500, 0, 0);
    }
    else if (WORKING_MODE == 1)
    {
        // RGBModule.setColor(0, 500, 500);
    }

    switch (command)
    {
    case 47873:
        if (WORKING_MODE > 0)
        {
            WORKING_MODE--;
            // RGBModule.setColor(0, 500, 500);
            showWorkingMode();
        }
        break;
    case 47361:
        if (WORKING_MODE < 1)
        {
            WORKING_MODE++;
            // RGBModule.setColor(500, 0, 500);
            showWorkingMode();
        }
        break;
    case 62465:
        if (WORKING_MODE == 0)
            relay0.toggle();
        break;
    case 47617:
        if (WORKING_MODE == 0)
            if (hasBacklight)
            {
                lcd.noBacklight();
                hasBacklight = false;
            }
            else
            {
                lcd.backlight();
                hasBacklight = true;
            }
        break;
        // default:
    }

    delay(50);

    // RGBModule.setColor(0, 0, 0);
}
// ANCHOR Check Infrared remote result
void checkIRResults()
{
    String tag = "IRRemote";

    if (irReceiver.decode())
    {
        uint32_t value = irReceiver.decodedIRData.decodedRawData / 65280;
        printToSerial(tag, value);
        handleInternalCommands(value);
        irReceiver.resume();
    }
}
// ANCHOR Print to serial
void printToSerial(String tag, uint32_t text)
{
    Serial.println(tag + ":" + text);
    blueToothSlaveSerial.println(tag + ":" + text);
}
void printToSerial(String tag, String text)
{
    Serial.println(tag + ":" + text);
    blueToothSlaveSerial.println(tag + ":" + text);
}

// ANCHOR Check Serial Input
void checkSerialInput()
{
    String serialValue = "";
    if (Serial.available())
    {
        serialValue = Serial.readString();
    }
    if (blueToothSlaveSerial.available())
    {
        serialValue = blueToothSlaveSerial.readString();
    }
    // if (serialValue.startsWith("time"))
    // {

    //     String time = toString(myRTC.hours) + ":" + toString(myRTC.minutes);
    //     printToLCD(time);
    //     printToSerial("time", time);
    // }
    if (serialValue.startsWith("lcd:"))
    {
        serialValue.replace("lcd:", "");
        if (serialValue == "flush")
        {
            clearLCD();
            lastPrintLine2 = "";
            lastPrint = "";
        }
        else if (serialValue.startsWith("backLight:"))
        {
            serialValue.replace("backLight:", "");
            bool setBackLight = serialValue == "on";
            if (setBackLight)
            {
                lcd.backlight();
                hasBacklight = true;
            }
            else
            {
                lcd.noBacklight();
                hasBacklight = false;
            }
        }
        else if (serialValue.indexOf(":") < 0)
        {
            lastPrint = serialValue;
            lastPrintLine2 = "";
        }
        else
        {
            String nextLine = serialValue;
            nextLine.remove(0, serialValue.indexOf(":") + 1);
            serialValue.remove(serialValue.indexOf(":"), 0xffff);
            lastPrintLine2 = nextLine;
            lastPrint = serialValue;
        }

        printToLCD(lastPrint, lastPrintLine2);
    }
    else if (serialValue.startsWith("relay:"))
    {
        serialValue.replace("relay:", "");
        relay0.doCommand(serialValue);
    }
    else if (serialValue.startsWith("rgb:"))
    {
        serialValue.replace("rgb:", "");
        // RGBModule.doCommand(serialValue);
    }
}
void log(String)
{
}
// ANCHOR Setup
void setup()
{
    // put your setup code here, to run once:
    pinMode(LED_BUILTIN, OUTPUT);

    // myRTC.setDS1302Time(0, 35, 20, 4, 18, 1, 2022);
    irReceiver.enableIRIn();
    // lcd.begin(16, 2);
    lcd.init();
    lcd.noBacklight();
    Serial.begin(9600);
    Serial.setTimeout(15);
    blueToothSlaveSerial.begin(9600);
    blueToothSlaveSerial.setTimeout(15);
    SPI.begin(); // Initiate  SPI bus
    Serial.println("setup finished");
    printToLCD("Boot sequence", "completed");
}
// ANCHOR Loop
void loop()
{
    checkIRResults();
    checkSerialInput();
    clearLCDTicker.update();
    // myRTC.updateTime();
}
