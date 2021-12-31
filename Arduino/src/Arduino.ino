
#include <SPI.h>
#include <IRremote.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>

using namespace std;

int RELAY_PIN = A1;
bool relayValue = false;
// volume
int volumePin = A0;
int minimumInput = 45;                 // The minimum
int maximumInput = 920 - minimumInput; // The maximum
int lastVolumeValue = 0;
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
void handleInternalCommands(int command)
{
    switch (command)
    {
    case 62465:
        if (relayValue)
        {
            analogWrite(RELAY_PIN, 0);
            relayValue = false;
        }
        else
        {
            analogWrite(RELAY_PIN, 1024);
            relayValue = true;
        }
        break;
    case 47617:
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
    default:
        break;
    }
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
}
void printToSerial(String tag, String text)
{
    Serial.println(tag + ":" + text);
}
// ANCHOR Check Serial Input
void checkSerialInput()
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
        if (serialValue == "on")
        {
            analogWrite(RELAY_PIN, 1024);
            // pinMode(RELAY_PIN, HIGH);
            printToLCD("relay control", "on");
        }
        else
        {
            analogWrite(RELAY_PIN, 0);
            printToLCD("relay control", "off");
        }
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
void checkVolumeInput()
{
    String tag = "volume";
    int volumeInput = analogRead(volumePin);
    volumeInput = volumeInput - minimumInput;
    int value = ((double)volumeInput) / maximumInput * 100;
    if (value < 0)
    {
        value = 0;
    }
    else if (value > 100)
    {
        value = 100;
    }
    if (value != lastVolumeValue)
    {

        lastVolumeValue = value;
        printToSerial(tag, value);

        printToLCD("Volume Changed", toString(value));
    }
}
// ANCHOR Setup
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
    Serial.setTimeout(15);
    SPI.begin(); // Initiate  SPI bus
}
// ANCHOR Loop
void loop()
{
    // checkRFID();
    checkIRResults();
    checkSerialInput();
    // checkTemperature();
    // checkVolumeInput();
}