#include <IRremote.h>
#include <LiquidCrystal.h>
#include <string.h>
using namespace std;

int RECV_PIN = 8;

LiquidCrystal lcd(2, 3, 9, 10, 11, 12);

IRrecv irReceiver(RECV_PIN);
decode_results results;
String lastPrint = "";

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

void checkIRResults()
{
  String tag = "IRRemote";
  if (irReceiver.decode())
  {
    uint32_t value = irReceiver.decodedIRData.decodedRawData / 65280;

    printToSerial(tag, value);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(30);

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
      lastPrint = "";
    }
    else
    {
      lastPrint = serialValue;
    }
    printToLCD(lastPrint);
  }
}

void setup()
{
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  irReceiver.enableIRIn();
  lcd.begin(16, 2);
  Serial.println("Enabled IRin");
}

void loop()
{
  checkIRResults();
  // put your main code here, to run repeatedly:
  checkSerialIn();
  delay(30);
}
