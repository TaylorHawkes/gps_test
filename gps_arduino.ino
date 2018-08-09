#include <AltSoftSerial.h>

AltSoftSerial gpsSerial;
const int sentenceSize = 80;

char sentence[sentenceSize];

void setup()
{

Serial.begin(115200);
gpsSerial.begin(9600);

char c[20] ="4099.9085";
char b[20] ="n";
toDegreeDecimalLat(c,b);

return;
 byte updateRateCmd[] = {160, 161, 00, 03, 14, 01, 00, 15, 13, 10}; // 1 Hz
//  byte updateRateCmd[] = {160, 161, 00, 03, 14, 08, 00, 06, 13, 10}; // 8 Hz			A0 A1 00 03 0E 08 00 06 0D 0A
//  byte updateRateCmd[] = {160, 161, 00, 03, 14, 10, 00, 04, 13, 10}; // 10 Hz			A0 A1 00 03 0E 0A 00 04 0D 0A
// byte updateRateCmd[] = {160, 161, 00, 03, 14, 20, 00, 26, 13, 10}; // 20 Hz			A0 A1 00 03 0E 14 00 1A 0D 0A
 byte baudrateCmd[] = {160, 161, 00, 04, 05, 00, 05, 00, 00, 13, 10}; // 115200		A0 A1 00 04 05 00 05 00 00 0D 0A
//byte Factory[] = {160, 161, 00, 02, 04, 01, 05, 13, 10}; // Factory reset

 gpsSerial.write(baudrateCmd, sizeof(baudrateCmd)); // tell GPS to change baudrate
 gpsSerial.flush(); // wait for command to go out
 delay(50);
 gpsSerial.begin(115200); // New baudrate
 gpsSerial.write(updateRateCmd, sizeof(updateRateCmd));
}

void loop()
{
  static int i = 0;
  if (gpsSerial.available())
  {
    char ch = gpsSerial.read();
    if (ch != '\n' && i < sentenceSize)
    {
      sentence[i] = ch;
      i++;
    }
    else
    {
     sentence[i] = '\0';
     i = 0;
     displayGPS();
    }
  }
}

void displayGPS()
{

  char field[20];
  char field2[20];
  getField(field, 0);

  if (strcmp(field, "$GPRMC") == 0)
  {
     getField(field, 3); 
     getField(field2, 4);

     float lat=toDegreeDecimalLat(field,field2);  // number

     Serial.print(field);
     Serial.print("----");
     Serial.print(lat);
     Serial.print("\n");

 // getField(field, 5); 
 // getField(field2, 6);
 // float lng=toDegreeDecimalLNG(field,field2);  // number

 //  Serial.println(lng);
 //  Serial.println("\n");
  }
}

float toDegreeDecimalLat(char* x,char* n){

   float degWhole=(10*(x[0]-'0'))+(x[1]-'0');

    unsigned long  degTenDec=(100000*(x[2]-'0'))
                   +((unsigned long) 10000*(x[3]-'0'))
                   +(1000*(x[5]-'0'))
                   +(100*(x[6]-'0'))
                   +(10*(x[7]-'0'))
                   +((x[8]-'0'));

     Serial.print("|");
     Serial.print(degTenDec);
     Serial.print("|");
     unsigned long deg=(degTenDec * 1000)/6;
     Serial.print("|deg:");
     Serial.print(deg);
     Serial.print("|");
    return degWhole+deg;

     //98.5085
    // float degWhole=float(int(x/100));
    // float degDec=(x-degWhole*100)/60;
    // float deg=degWhole + degDec;
    //    if (n=='S') { 
    //      deg= (-1)*deg;
    //    }
    // return deg; 
}

float toDegreeDecimalLNG(char* x, char* n){
    return 1.0;
// float degWhole=float(int(x/1000));
// float degDec=(x-degWhole*1000)/60;
// float deg=degWhole + degDec;
//  if (n=='W') { 
//   deg= (-1)*deg;
//  }
//  return deg;
}

void getField(char* buffer, int index)
{
  int sentencePos = 0;
  int fieldPos = 0;
  int commaCount = 0;
  while (sentencePos < sentenceSize)
  {
    if (sentence[sentencePos] == ',')
    {
      commaCount ++;
      sentencePos ++;
    }
    if (commaCount == index)
    {
      buffer[fieldPos] = sentence[sentencePos];
      fieldPos ++;
    }
    sentencePos ++;
  }
  buffer[fieldPos] = '\0';
} 
