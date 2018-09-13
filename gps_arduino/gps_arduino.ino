//#include <AltSoftSerial.h>

//AltSoftSerial Serial1;
const int sentenceSize = 80;

char sentence[sentenceSize];

void setup()
{

Serial.begin(115200);
Serial1.begin(9600);

byte baudrateCmd[] = {160, 161, 00, 04, 05, 00, 05, 00, 00, 13, 10}; // 115200		A0 A1 00 04 05 00 05 00 00 0D 0A

byte updateRateCmd[] = {160, 161, 00, 03, 14, 01, 00, 15, 13, 10}; // 1 Hz			A0 A1 00 03 0E 01 00 0F 0D 0A
//  byte updateRateCmd[] = {160, 161, 00, 03, 14, 02, 00, 12, 13, 10}; // 2 Hz			A0 A1 00 03 0E 02 00 0C 0D 0A
//  byte updateRateCmd[] = {160, 161, 00, 03, 14, 04, 00, 10, 13, 10}; // 4 Hz			A0 A1 00 03 0E 04 00 0A 0D 0A
//byte updateRateCmd[] = {160, 161, 00, 03, 14, 05, 00, 11, 13, 10}; // 5 Hz			A0 A1 00 03 0E 05 00 0B 0D 0A
//  byte updateRateCmd[] = {160, 161, 00, 03, 14, 08, 00, 06, 13, 10}; // 8 Hz			A0 A1 00 03 0E 08 00 06 0D 0A
//  byte updateRateCmd[] = {160, 161, 00, 03, 14, 10, 00, 04, 13, 10}; // 10 Hz			A0 A1 00 03 0E 0A 00 04 0D 0A
//  byte updateRateCmd[] = {160, 161, 00, 03, 14, 20, 00, 26, 13, 10}; // 20 Hzbyte updateRateCmd[] =  {160, 161, 00, 03, 14, 01, 00, 15, 13, 10}; // 1 Hz

//byte Factory[] = {160, 161, 00, 02, 04, 01, 05, 13, 10}; // Factory reset

//Enable WAS command
byte enableWASS[] = {160, 161, 00, 03, 55, 01, 00, 54, 13, 10}; //set WAS to enabled  //A0 A1 00 03 37 01 00 36 0D 0A 

                                                                                    

 Serial1.write(baudrateCmd, sizeof(baudrateCmd)); // tell GPS to change baudrate
 Serial1.flush(); // wait for command to go out
 delay(50);
 Serial1.begin(115200); // New baudrate
 Serial1.write(updateRateCmd, sizeof(updateRateCmd));//update rate
 Serial1.write(enableWASS, sizeof(enableWASS));//update enable WASS

}

void loop()
{
  static int i = 0;
  if (Serial1.available())
  {

    char ch = Serial1.read();
  //Serial.print(ch);
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
  char lat_raw[10];
  char lng_raw[11];
  char lat_raw_type[1];
  char lng_raw_type[1];
  char lat_compare[10];
  char lng_compare[11];
  //see: i=https://cdn.sparkfun.com/datasheets/Sensors/GPS/Venus/638/doc/Venus638FLPx_DS_v07.pdf
  //gpgga
  char gga_quality_indicator[3];
  char gga_satellites[2];
  char gga_hdop[4];

  char ggsa_hdop[4];
  char ggsa_vdop[4];
  char ggsa_mode[2];
  char ggsa_mode_fix[2];

 char lat[15];
 char lng[15];

//   getField(field, 0);
//if (strcmp(field, "$GPGSA") == 0){

//    getField(ggsa_mode, 1); //
//    getField(ggsa_mode_fix, 2); //
//    getField(ggsa_hdop, 15); //

//   Serial.print("mode:"); Serial.println(ggsa_mode);
//   Serial.print("modefix:"); Serial.println(ggsa_mode_fix);
//   Serial.print("hdop:"); Serial.println(ggsa_hdop);
//   return;
//  //     Serial.print("vdop:"); Serial.print(ggsa_vdop);
//  //     Serial.println("");
// }


  getField(field, 0);
  if (strcmp(field, "$GPGGA") == 0){
     getField(lat_compare, 2); 
     getField(lng_compare, 4); 
     getField(gga_quality_indicator, 6); //
     getField(gga_satellites, 7); //
     getField(gga_hdop, 8); //

//// Serial.print("hdop:"); Serial.print(gga_hdop);
//// Serial.println("");

// Serial.print("sats:"); Serial.print(gga_satellites); 
 //run some filtering testings
    int gga_qi=(gga_quality_indicator[0]-'0');
    int hdop= +(10*(gga_hdop[0]-'0')) +((gga_hdop[2]-'0'));//1.0 = 10 ,1.2 =12 
 // if(hdop < 15){
 //     return;
 // }

 // if(gga_qi < 1){
 //      return;
 // }


}

   getField(field, 0);
  if (strcmp(field, "$GPRMC") == 0)
  {
     getField(lat_raw, 3); 
     getField(lat_raw_type, 4);

     getField(lng_raw, 5); 
     getField(lng_raw_type, 6);


  // if(strcmp(lat_raw, lat_compare) || strcmp(lng_raw, lng_compare)){
  //  //  Serial.println("Serial error: ignoring packet:");
  //    return;
  // }

     toDegreeDecimalLat(lat_raw,lat_raw_type,lat);
     toDegreeDecimalLng(lng_raw,lng_raw_type,lng);
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
  if(n[0]=='W' || 1  ){ //just always do this 
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

