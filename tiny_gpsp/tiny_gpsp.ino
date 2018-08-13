#include <TinyGPS++.h>
#include <SoftwareSerial.h>
/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 4800;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(115200);

  ss.begin(9600);

byte updateRateCmd[] =  {160, 161, 00, 03, 14, 01, 00, 15, 13, 10}; // 1 Hz
// byte updateRateCmd[] = {160, 161, 00, 03, 14, 08, 00, 06, 13, 10}; // 8 Hz			A0 A1 00 03 0E 08 00 06 0D 0A
// byte updateRateCmd[] = {160, 161, 00, 03, 14, 10, 00, 04, 13, 10}; // 10 Hz			A0 A1 00 03 0E 0A 00 04 0D 0A
// byte updateRateCmd[] ={160, 161, 00, 03, 14, 20, 00, 26, 13, 10}; // 20 Hz			A0 A1 00 03 0E 14 00 1A 0D 0A


byte enableWASS[] = {160, 161, 00, 03, 55, 01, 00, 54, 13, 10}; //set WAS to enabled  //A0 A1 00 03 37 01 00 36 0D 0A 

byte baudrateCmd[] = {160, 161, 00, 04, 05, 00, 00, 00, 05, 13, 10}; // 4800		A0 A1 00 04 05 00 00 00 05 0D 0A 
 ss.write(baudrateCmd, sizeof(baudrateCmd)); // tell GPS to change baudrate
 ss.flush(); // wait for command to go out
 delay(50);
	ss.begin(GPSBaud);
	ss.write(updateRateCmd, sizeof(updateRateCmd));//update rate
	ss.write(enableWASS, sizeof(enableWASS));//update enable WASS





  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo()
{
 // Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

 // Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
//  Serial.print(gps.date.month());
//  Serial.print(F("/"));
//  Serial.print(gps.date.day());
//  Serial.print(F("/"));
//  Serial.print(gps.date.year());
  }
  else
  {
//    Serial.print(F("INVALID"));
  }

 // Serial.print(F(" "));
  if (gps.time.isValid())
  {
/// if (gps.time.hour() < 10) Serial.print(F("0"));
/// Serial.print(gps.time.hour());
/// Serial.print(F(":"));
/// if (gps.time.minute() < 10) Serial.print(F("0"));
/// Serial.print(gps.time.minute());
/// Serial.print(F(":"));
/// if (gps.time.second() < 10) Serial.print(F("0"));
/// Serial.print(gps.time.second());
/// Serial.print(F("."));
/// if (gps.time.centisecond() < 10) Serial.print(F("0"));
/// Serial.print(gps.time.centisecond());
  }
  else
  {
//    Serial.print(F("INVALID"));
  }

  Serial.println();
}
