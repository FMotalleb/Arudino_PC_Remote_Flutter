#include <IRremote.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>
using namespace std;

int RECV_PIN = 8;

LiquidCrystal_I2C lcd(0x27, 16, 2);

IRrecv irReceiver(RECV_PIN);
decode_results results;
String lastPrint = "";
String lastPrintLine2 = "";

void printToSerial(String tag, uint32_t text)
{
    Serial.println(tag + ":" + text);
}

String toString(int value)
{
    String result = "";

    int buffer = value;
    while (value > 0)
    {
        int cache = buffer % 10 + 48;
        result = (char)cache + result;
        buffer = buffer / 10;
    }
    return result;
}

void printToLCD(String firstLine, String secondLine = "test")
{
    lcd.clear();
    lcd.print(firstLine);
    lcd.setCursor(0, 1);
    lcd.print(secondLine);

    if (firstLine == "")
    {
        lcd.clear();
    }
}

void clearLCD()
{
    lcd.clear();
}

void checkIRResults()
{
    String tag = "IRRemote";
    if (irReceiver.decode())
    {
        uint32_t value = irReceiver.decodedIRData.decodedRawData / 65280;

        printToSerial(tag, value);
        digitalWrite(LED_BUILTIN, HIGH);

        irReceiver.resume();
        digitalWrite(LED_BUILTIN, LOW);
    }
}
void checkSerialIn()
{
    String serialValue = Serial.readString();
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
            }
            else
            {
                lcd.noBacklight();
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
}

void setup()
{
    // put your setup code here, to run once:
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    irReceiver.enableIRIn();
    // lcd.begin(16, 2);
    lcd.init();
    lcd.backlight();
    Serial.println("Enabled IRin");
    Serial.setTimeout(10);
}

void loop()
{
    checkIRResults();
    // put your main code here, to run repeatedly:
    checkSerialIn();
}
