#include <SoftwareSerial.h>

#define rxPin 8
#define txPin 7

//SoftwareSerial bluetooth(rxPin, txPin);
#define bluetooth Serial
volatile unsigned long tagID = 0;
volatile unsigned long lastBitArrivalTime;
volatile int bitCount = 0;

void ISRone(void)
{ 
  lastBitArrivalTime = millis();
  bitCount++;
  tagID <<= 1;
  tagID |= 1;
}

void ISRzero(void)
{ 
  lastBitArrivalTime = millis();
  bitCount++;
  tagID <<= 1;
}

void setup()
{
  bluetooth.begin(9600);
  bluetooth.println("Hello, world?");

  pinMode(2, INPUT);
  digitalWrite(2, HIGH);  // Enable pull-up resistor
  attachInterrupt(0, ISRzero, FALLING);

  pinMode(3, INPUT);
  digitalWrite(3, HIGH);  // Enable pull-up resistor
  attachInterrupt(1, ISRone,  FALLING);

  tagID = 0;
  bitCount = 0;
}

void loop()
{
  //  See if it has been more than 1/4 second since the last bit arrived
  if(bitCount > 0 && millis() - lastBitArrivalTime >  250){

    bluetooth.print("$");
    bluetooth.print(tagID,DEC);
    bluetooth.print("*");
    tagID = 0;
    bitCount = 0;
  }
}
