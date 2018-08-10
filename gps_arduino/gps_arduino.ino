#include <AltSoftSerial.h>

AltSoftSerial gpsSerial;
const int sentenceSize = 80;

char sentence[sentenceSize];

void setup()
{

Serial.begin(115200);
gpsSerial.begin(9600);

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
    
     char lat[15];
     toDegreeDecimalLat(field,field2,lat);


     getField(field, 5); 
     getField(field2, 6);

     char lng[15];
     toDegreeDecimalLng(field,field2,lng);

      Serial.print(lat);
      Serial.print(",");
      Serial.print(lng);
      Serial.print("\n");
  }
}

void toDegreeDecimalLat(char* x,char* n,char *buff){
    unsigned long  degTenDec=(100000*(x[2]-'0'))
                   +((unsigned long) 10000*(x[3]-'0'))
                   +(1000*(x[5]-'0'))
                   +(100*(x[6]-'0'))
                   +(10*(x[7]-'0'))
                   +((x[8]-'0'));

    unsigned long deg=(degTenDec * 1000)/6;
    char deg_string[9];
    char deg_string_decimal[9];

    sprintf(deg_string, "%lu", deg);

    //16.6666500
    int length=0;
    for( int i=0; deg_string[i] != 0 ;i++){
         length++;
    }
    for( int i=0; i< (9-length) ;i++){
        deg_string_decimal[i]='0';
    }
    int c=0;
    for(int i=(9-length); i< 9 ;i++){
        deg_string_decimal[i]=deg_string[c];
        c++;
    }
    deg_string_decimal[9]='\0';

    //add first two number to get the non decimal amount
    int DD=(10*(x[0]-'0'))+((x[1]-'0'));
    int MM=((deg_string_decimal[0]-'0'));
    int DD_MM=DD+MM;
    char dd_final[3];
    sprintf(dd_final, "%i", DD_MM);
    //if under ten we pad dd_final
    if(DD_MM < 10){
        dd_final[1]==dd_final[0];
        dd_final[0]=='0';
    }
  if (n[0]=='S'){ 
    buff[0]='-';
    buff[1]=dd_final[0];
    buff[2]=dd_final[1];
    buff[3]='.';
    for(int i=4; i< 12 ;i++){
        buff[i]=deg_string_decimal[i-3];
    }
    buff[12]='\0';
  }else{
    buff[0]=dd_final[0];
    buff[1]=dd_final[1];
    buff[2]='.';
    for(int i=3; i< 11 ;i++){
        buff[i]=deg_string_decimal[i-2];
    }
    buff[11]='\0';
  }
}

void toDegreeDecimalLng(char* x,char* n,char *buff){
    unsigned long  degTenDec=(100000*(x[3]-'0'))
                   +((unsigned long) 10000*(x[4]-'0'))
                   +(1000*(x[6]-'0'))
                   +(100*(x[7]-'0'))
                   +(10*(x[8]-'0'))
                   +((x[9]-'0'));

    unsigned long deg=(degTenDec * 1000)/6;
    char deg_string[9];
    char deg_string_decimal[9];

    sprintf(deg_string, "%lu", deg);


    int length=0;
    for( int i=0; deg_string[i] != 0 ;i++){
         length++;
    }
    for( int i=0; i< (9-length) ;i++){
        deg_string_decimal[i]='0';
    }

    int c=0;
    for(int i=(9-length); i< 9 ;i++){
        deg_string_decimal[i]=deg_string[c];
        c++;
    }
    deg_string_decimal[9]='\0';


    //add first three number to get the non decimal amount
    int DD= (100*(x[0]-'0')) + (10*(x[1]-'0')) + (x[2]-'0');
    int MM=((deg_string_decimal[0]-'0'));
    int DD_MM=DD+MM;
    char dd_final[4];
    sprintf(dd_final, "%i", DD_MM);

    length=0;
    for( int i=0; dd_final[i] != 0 ;i++){
         length++;
    }
    //padd if needed
    if(length==1){
        dd_final[2]=dd_final[0];
        dd_final[1]='0';
        dd_final[0]='0';
    }
    if(length==2){
        dd_final[2]=dd_final[1];
        dd_final[1]=dd_final[0];
        dd_final[0]='0';
    }

   //if under ten we pad dd_final
  if (n[0]=='W'){ 
    buff[0]='-';
    buff[1]=dd_final[0];
    buff[2]=dd_final[1];
    buff[3]=dd_final[2];
    buff[4]='.';
    for(int i=5; i< 13 ;i++){
        buff[i]=deg_string_decimal[i-4];
    }
    buff[13]='\0';
  }else{
    buff[0]=dd_final[0];
    buff[1]=dd_final[1];
    buff[2]=dd_final[2];
    buff[3]='.';
    for(int i=4; i< 12 ;i++){
        buff[i]=deg_string_decimal[i-3];
    }
    buff[12]='\0';
  }
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

