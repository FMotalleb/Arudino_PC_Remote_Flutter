
#include <SPI.h>
#include <IRremote.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>
#include <vector.h>
#include <SoftwareSerial.h>
#include <Ticker.h>

#include <stdio.h>

using namespace std;

void printToLCD(String, String);
void clearLCD();

Ticker clearLCDTicker(clearLCD, 3500, 0, MILLIS);
SoftwareSerial blueToothSlaveSerial(2, 3); // RX, TX

// Relay
int RELAY_PIN = A0;
bool relayValue = false;
// RGB LED
int RGB_RED = A1;
int RGB_GREEN = A2;
int RGB_BLUE = A3;

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
void setRGB(int red, int green, int blue)
{
    analogWrite(RGB_RED, red);
    analogWrite(RGB_GREEN, green);
    analogWrite(RGB_BLUE, blue);
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
void handleInternalCommands(int command)
{
    if (WORKING_MODE == 0)
    {
        setRGB(350, 0, 0);
    }
    else if (WORKING_MODE == 1)
    {
        setRGB(0, 150, 130);
    }

    switch (command)
    {
    case 47873:
        if (WORKING_MODE > 0)
        {
            WORKING_MODE--;
            setRGB(0, 150, 150);
        }
        break;
    case 47361:
        if (WORKING_MODE < 1)
        {
            WORKING_MODE++;
            setRGB(150, 0, 150);
        }
        break;
    case 62465:
        if (WORKING_MODE == 0)
            if (relayValue)
            {
                analogWrite(RELAY_PIN, 0);
                relayValue = false;
                lcd.noBacklight();
                hasBacklight = false;
            }
            else
            {
                analogWrite(RELAY_PIN, 1024);
                relayValue = true;
            }
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
    if (WORKING_MODE == 0)
    {
        printToLCD("Working Mode", "internal");
    }
    else if (WORKING_MODE == 1)
    {
        printToLCD("Working Mode", "serial");
    }

    delay(50);

    setRGB(0, 0, 0);
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
int toInt(const char *input)
{
    int x = 0;
    sscanf(input, "%d", &x);
    return x;
}
void splitRGB(String input, char delimiter)
{
    Vector<String> *result = new Vector<String>();
    String holder = input;
    int pos = 0;
    int itemIndex = 0;
    int red = 0;
    int green = 0;
    int blue = 0;
    while (holder.length() > 0)
    {
        // printToSerial("log", holder);

        pos = holder.indexOf(delimiter);
        if (pos == -1)
            pos = 99999;
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
        result->push_back(temp);

        holder.remove(0, pos + 1);
    }
    setRGB(red, green, blue);
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
        if (serialValue.startsWith("on"))
        {
            analogWrite(RELAY_PIN, 1024);
            // pinMode(RELAY_PIN, HIGH);
            printToLCD("relay control", "on");
        }
        else if (serialValue.startsWith("off"))
        {
            analogWrite(RELAY_PIN, 0);
            printToLCD("relay control", "off");
        }
    }
    else if (serialValue.startsWith("rgb:"))
    {
        serialValue.replace("rgb:", "");
        splitRGB(serialValue, ',');
        // delay(50);
        // printToSerial("Red", rgb->at(0));
        // delay(50);
        // printToSerial("Blue", rgb->at(1));
        // delay(50);
        // printToSerial("Green", rgb->at(2));

        // setRGB(toInt(rgb[0].c_str()), toInt(rgb[1].c_str()), toInt(rgb[2].c_str()));
    }
}
// ANCHOR RFID methodes
// void checkRFID()
// {
//     // Look for new cards
//     if (!mfrc522.PICC_IsNewCardPresent())
//     {
//         return;
//     }
//     // Select one of the cards
//     if (!mfrc522.PICC_ReadCardSerial())
//     {
//         return;
//     }
//     // Show UID on serial monitor

//     String content = "";
//     byte letter;
//     for (byte i = 0; i < mfrc522.uid.size; i++)
//     {
//         content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
//         content.concat(String(mfrc522.uid.uidByte[i], HEX));
//     }

//     content.toUpperCase();
//     String key = content.substring(1);
//     printToSerial("RFID", key);
//     printToLCD("RFID", key);
//     String response = "";
//     while (response != "gotIt")
//     {
//         response = Serial.readString();
//     }
// }
// ANCHOR temperature sensor
// void checkTemperature()
// {
//     // Uncomment whatever type you're using!
//     int readData = DHT.read22(temperaturePin); // DHT22/AM2302
//     // int readData = DHT.read11(dataPin); // DHT11

//     float t = DHT.temperature; // Gets the values of the temperature
//     float h = DHT.humidity;    // Gets the values of the humidity

//     // Printing the results on the serial monitor
//     Serial.print("Temperature = ");
//     Serial.print(t);
//     Serial.print(" ");

//     Serial.print("C | ");
//     Serial.print((t * 9.0) / 5.0 + 32.0); // print the temperature in Fahrenheit
//     Serial.print(" ");

//     Serial.println("F ");
//     Serial.print("Humidity = ");
//     Serial.print(h);
//     Serial.println(" % ");
//     Serial.println("");

//     printToLCD(toString(readData));
//     delay(1500);
//     // delay(2000);
// }
// void checkVolumeInput()
// {
//     String tag = "volume";
//     int volumeInput = analogRead(volumePin);
//     volumeInput = volumeInput - minimumInput;
//     int value = ((double)volumeInput) / maximumInput * 100;
//     if (value < 0)
//     {
//         value = 0;
//     }
//     else if (value > 100)
//     {
//         value = 100;
//     }
//     if (value != lastVolumeValue)
//     {

//         lastVolumeValue = value;
//         printToSerial(tag, value);

//         printToLCD("Volume Changed", toString(value));
//     }
// }
// ANCHOR Setup
void passData()
{
    Serial.println(blueToothSlaveSerial.readString());
    delay(100);
}
void setup()
{
    // put your setup code here, to run once:
    pinMode(LED_BUILTIN, OUTPUT);

    irReceiver.enableIRIn();
    // lcd.begin(16, 2);
    lcd.init();
    lcd.noBacklight();
    Serial.begin(9600);

    Serial.setTimeout(15);
    blueToothSlaveSerial.begin(9600);
    blueToothSlaveSerial.println("Enabled IRin-BT");
    blueToothSlaveSerial.setTimeout(15);
    SPI.begin(); // Initiate  SPI bus
    Serial.println("setup:startup:setupFinished");
}
// ANCHOR Loop
void loop()
{
    // passData();
    checkIRResults();
    checkSerialInput();
    clearLCDTicker.update();
    // checkTemperature();
    // checkVolumeInput();
    // checkRFID();
}
