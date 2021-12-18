#include <IRremote.h>
#include <string.h>
using namespace std;
int RECV_PIN = 8;
// IR Library stuff
IRrecv irReceiver(RECV_PIN);
decode_results results;

void setup()
{
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  irReceiver.enableIRIn();
  Serial.println("Enabled IRin");
}

void loop()
{
  checkIRResults();
  // put your main code here, to run repeatedly:

  delay(30);
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
