//#include "WiFly.h"
#include <Arduino.h>
#include <Time.h>
#include <SoftwareSerial.h>
#include <TimeAlarms.h>
#include <Streaming.h>
//#include <PString.h>
#include "LPD8806.h"
#include "SPI.h"

#define ARDUINO_RX_PIN  0
#define ARDUINO_TX_PIN  1


#define TODAY_DATA 4
#define TODAY_CLOCK 5   

#define ONE_DATA 7
#define ONE_CLOCK 6

#define TWO_DATA 8
#define TWO_CLOCK 9

#define THREE_DATA 12
#define THREE_CLOCK 11

#define DIM_PIN 0

char MY_SERVER_GET[] = "forecastshelf.blakesawyer.net";
char MY_SERVER_GET_URL[] = "GET http://forecastshelf.blakesawyer.net/getshelfdata.php?zip=24060 HTTP/1.0";

char SSID[] = "HankDaThird";
char PASSPHRASE[] = "stevemcqueen";

char *currentData = "719719719719";
int currentDataSize = 0;
boolean hasCurrentData = false;
boolean readingData = false;

#define PI 3.14159265

//Today's variables
LPD8806 today_strip = LPD8806(14, TODAY_DATA, TODAY_CLOCK);
//int today_current_pos = 0;
unsigned long today_last_move = millis();
boolean today_going_up = true;
float today_last_brightness = 1.0;
//uint32_t today_current_colors[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//Day one variables
LPD8806 one_strip = LPD8806(6, ONE_DATA, ONE_CLOCK);
//int one_current_pos = 0;
unsigned long one_last_move = millis();
boolean one_going_up = true;
float one_last_brightness = 1.0;
//uint32_t one_current_colors[6] = {0,0,0,0,0,0};

//Day two variables
LPD8806 two_strip = LPD8806(6, TWO_DATA, TWO_CLOCK);
//int two_current_pos = 0;
unsigned long two_last_move = millis();
boolean two_going_up = true;
float two_last_brightness = 1.0;
//uint32_t two_current_colors[6] = {0,0,0,0,0,0};

//Day three variables
LPD8806 three_strip = LPD8806(6, THREE_DATA, THREE_CLOCK);
//int three_current_pos = 0;
unsigned long three_last_move = millis();
boolean three_going_up = true;
float three_last_brightness = 1.0;
//uint32_t three_current_colors[6] = {0,0,0,0,0,0};

//WiFlySerial wifi(ARDUINO_RX_PIN,ARDUINO_TX_PIN);

SoftwareSerial xbee(2,3); //RX, TX

//Colors
uint32_t blue = today_strip.Color(0,0,127);
uint32_t dark_gray = today_strip.Color(0,0,0);
uint32_t white = today_strip.Color(127,127,127);
uint32_t light_gray = today_strip.Color(96,96,96);
uint32_t light_blue = today_strip.Color(0,127,127);
uint32_t yellow = today_strip.Color(127,127,0);
uint32_t green = today_strip.Color(0,127,0);
uint32_t orange = today_strip.Color(127,64,32);
uint32_t purple = today_strip.Color(48,0,127);

//COLOR SCHEMES
uint32_t tstorms[14] = {blue,purple,blue,purple,blue,purple,blue,purple,blue,purple,blue,purple,blue,purple};
uint32_t rainy[14] = {blue,blue,blue,blue,blue,blue,blue,blue,blue,blue,blue,blue,blue,blue};
uint32_t snowy[14] = {white,white,white,white,white,white,white,white,white,white,white,white,white,white};
uint32_t cloudy[14] = {white,white,white,white,white,white,white,white,white,white,white,white,white,white};
uint32_t mostly_cloudy[14] = {white,white,white,white,white,white,white,white,white,white,white,white,white,white};
uint32_t partly_cloudy[14] = {white,light_blue,white,light_blue,white,light_blue,white,light_blue,white,light_blue,white,light_blue,white,light_blue};
uint32_t sunny[14] = {light_blue,light_blue,light_blue,light_blue,light_blue,light_blue,light_blue,light_blue,light_blue,light_blue,light_blue,light_blue,light_blue,light_blue};

//Sun Color Schemes
uint32_t full_sun[4] = {yellow,yellow,yellow,yellow};
uint32_t partial_sun[4] = {white,yellow,yellow,white};
uint32_t covered_sun[4] = {white,white,white,white};

/*void (*wx_conditions[7])(LPD8806 *strip, int wind, int temp, unsigned long *last_move, boolean *going_up, float *last_brightness);

wx_conditions[0] = tstorms;
wx_conditions[1] = rainy;
wx_conditions[2] = snowy;
wx_conditions[3] = cloudy;
wx_conditions[4] = mostly_cloudy;
wx_conditions[5] = partly_cloudy;
wx_conditions[6] = sunny;*/

void setup() {
  
  Serial.begin(9600);
  
  randomSeed(analogRead(1));
  

  today_strip.begin();
  one_strip.begin();
  two_strip.begin();
  three_strip.begin();

  today_strip.show();
  one_strip.show();
  two_strip.show();
  three_strip.show();
  
  tick_strip(&today_strip,currentData[0] - '0',currentData[1] - '0',currentData[2] - '0',&today_last_move,&today_going_up,&today_last_brightness);
  today_strip.show();
  
  Serial.println("waiting 5 secs");

  delay(5000);

  Serial.println("begin xbee");
  
  xbee.begin(9600);

 
  tick_strip(&one_strip,currentData[3] - '0',currentData[4] - '0',currentData[5] - '0',&one_last_move,&one_going_up,&one_last_brightness);
  one_strip.show();
  
  delay(1000);
  
  //hasCurrentData = true;

}




void loop(){
 
  //if(hasCurrentData){
    
      tick_strip(&today_strip,currentData[0] - '0',currentData[1] - '0',currentData[2] - '0',&today_last_move,&today_going_up,&today_last_brightness);
      tick_strip(&one_strip,currentData[3] - '0',currentData[4] - '0',currentData[5] - '0',&one_last_move,&one_going_up,&one_last_brightness);
      tick_strip(&two_strip,currentData[6] - '0',currentData[7] - '0',currentData[8] - '0',&two_last_move,&two_going_up,&two_last_brightness);
      tick_strip(&three_strip,currentData[9] - '0',currentData[10] - '0',currentData[11] - '0',&three_last_move,&three_going_up,&three_last_brightness);
      
      today_strip.show();
      one_strip.show();
      two_strip.show();
      three_strip.show();
      
      //xbee.println(currentData);

  //}
  
  if(xbee.available()){
    refreshData();
  }
 
}

void tick_strip(LPD8806 *strip, int condition, int wind, int temp, unsigned long *last_move, boolean *going_up, float *last_brightness){
  
  uint32_t colors[14];
  uint32_t *sun_color;
  
  //Serial.print("tick: ");
  //Serial.println(condition,DEC);
  
  if(condition == 1){
    
    for(int i=0;i<14;i++){
      colors[i] = tstorms[i];
    }
    sun_color = covered_sun;
  }
  else if(condition == 2){
    
    for(int i=0;i<14;i++){
      colors[i] = rainy[i];
    }

    sun_color = covered_sun;
  }
  else if(condition == 3){
    
    for(int i=0;i<14;i++){
      colors[i] = snowy[i];
    }

    sun_color = covered_sun;
  }
  else if(condition == 4){
    
    for(int i=0;i<14;i++){
      colors[i] = cloudy[i];
    }

    sun_color = covered_sun;
  }
  else if(condition == 5){
    
    for(int i=0;i<14;i++){
      colors[i] = mostly_cloudy[i];
    }
 
    sun_color = partial_sun;
  }
  else if(condition == 6){
    
    for(int i=0;i<14;i++){
      colors[i] = partly_cloudy[i];
    }

    sun_color = full_sun;
  }
  else if(condition == 7){
    
    for(int i=0;i<14;i++){
      colors[i] = sunny[i];
    }

    sun_color = full_sun;
  }
  
  int sun_size = 4;
  int sun_position = 0;
  if(strip->numPixels() == 6){
   
   sun_size = 2;
   sun_position = 2;
   
  }
  else{
    
    //Serial.print("hour = ");
    //Serial.println(hour(),DEC);
   
   //Adjust for EST
    int h = hour();
    if(h <= 3){
      h = (h-4)+24;
    }
    else{
      h = h - 4;
    }
    
    float tmp = (float)h/23.0;
    
    //Serial.print("tmp= ");
    //Serial.println(tmp);
    
    sun_position = ceil(14.0 * tmp);
    
    //Serial.print("sun pos = ");
    //Serial.println(sun_position,DEC);
    
    if(sun_position <= 1){
     
          sun_position = 0;
    }
    else{
      sun_position = sun_position - 2;
    }
    
    //Serial.print("sun pos = ");
    //Serial.println(sun_position,DEC);
  }
  
  
  
  //write sun color to correct position in strip's colors
  for(int i=0; sun_position+i < strip->numPixels() && i<sun_size; i++){
    colors[sun_position+i] = sun_color[i];
  }
  
  //Calculate current brightness breathing effect speed based on wind speed and time
  float windFactor = 0.25;
  if(wind == 1){
   windFactor = 1.0; 
  }
  if(wind == 2){
   windFactor = 1.5; 
  }
  if(wind == 3){
   windFactor = 2.0; 
  }
  if(wind == 4){
   windFactor = 2.5; 
  }
  
  //Breath using cosine function, reset time on peak. i.e. cos(0) = 1
  float secondsLapsed = ((float)(millis() - (*last_move))) / 1000.0;
  //Serial.print("seconds sapsed: ");
  //Serial.println(secondsLapsed);
  windFactor = windFactor * secondsLapsed;
  float sun_brightness = cos( windFactor ) + 1.0;
  sun_brightness = (9.0*sun_brightness) / 20.0;
  sun_brightness = sun_brightness + 0.1;
  //brightness = abs(brightness - 1.0);
  //Serial.print("last: ");
  //Serial.print(today_last_brightness,DEC);
  //Serial.print(" < brightness: ");
 // Serial.println(brightness,DEC);
  
  if((*last_brightness) < sun_brightness){
    
    if(!(*going_up)){ // Do nothing on a trough
       
    }
      
     (*going_up) = true;
     (*last_brightness) = sun_brightness;   
  }
  else{

    //Reset on peak
    if((*going_up)){
      (*last_move) = millis();
      (*last_brightness) = 1.0;
    }
    else{
      (*last_brightness) = sun_brightness;
    }
    (*going_up) = false;
  }
  
  //Serial.print("Brightness = ");
  //Serial.println(brightness,DEC);

  //Write colors to strip
  for(int i=0;i<strip->numPixels();i++){
    
    uint32_t c = colors[i];
    byte  r, g, b;
  
    // Need to decompose color into its r, g, b elements
    g = (c >> 16) & 0x7f;
    r = (c >>  8) & 0x7f;
    b =  c        & 0x7f;

   //Make sun breath...
    if(i >= sun_position && i < sun_position + sun_size){
      g = (byte)((float)g * sun_brightness * (((float)temp)/9.0) * getDimReading());
      r = (byte)((float)r * sun_brightness * (((float)temp)/9.0) * getDimReading());
      b = (byte)((float)b * sun_brightness * (((float)temp)/9.0) * getDimReading());
    }
     else{ //Else adjust for temp and dimmer control
      g = (byte)((float)g * (((float)temp)/9.0) * getDimReading());
      r = (byte)((float)r * (((float)temp)/9.0) * getDimReading());
      b = (byte)((float)b * (((float)temp)/9.0) * getDimReading());
     } 
    
    strip->setPixelColor(i, r, g, b);
  }
  
}


//Returns 0.0-1.0 from potentiometer
float getDimReading(){
      
  int reading = analogRead(DIM_PIN);

  if(reading < 20){
    return 0.0;
  }
  else if(reading > 840){
    return 1.0;
  }
  else{
   float diff = reading;
   return diff / 840.0;
  }
  
}

/*void setDemoData(){
  
  char buf[3];
  ltoa(random(1,8),buf,3);
  currentData[0] = buf[0];
  ltoa(random(1,8),buf,3);
  currentData[3] = buf[0];
  ltoa(random(1,8),buf,3);
  currentData[6] = buf[0];
  ltoa(random(1,8),buf,3);
  currentData[9] = buf[0];
  
  ltoa(random(1,5),buf,3);
  currentData[1] = buf[0];
  ltoa(random(1,5),buf,3);
  currentData[4] = buf[0];
  ltoa(random(1,5),buf,3);
  currentData[7] = buf[0];
  ltoa(random(1,5),buf,3);
  currentData[10] = buf[0];

  currentData[2] = '9';
  currentData[5] = '9';
  currentData[8] = '9';
  currentData[11] = '9';
  
  Serial.println(currentData);
  
}*/

void refreshData(){

    hasCurrentData = false;
    currentDataSize = 0;
    readingData = false;
    boolean readingTime = false;
    boolean currentTimeSize = 0;
    char *currentTime = "1369843200"; //Turn on at noon to calibrate?
    char *oldData = currentData;
    
    //if(!WiFly.join(SSID,PASSPHRASE)){
     //Failed
    //}

  
  
    Serial.println("trying to get data");
     
       
       while ( xbee.available() ) {
         
          char c = (char) xbee.read();
          
          if(readingTime && currentTimeSize < 10){
           
            currentTime[currentTimeSize] = c;
            currentTimeSize++;
            
          }
 
          if(readingTime && c == '#'){
            
            Serial.print("got time: ");
            Serial.println(atol(currentTime),DEC);
            
            
            setTime(atol(currentTime));
            //Serial.print("hour= ");
            //Serial.println(hour());
          }         
          
          if(!readingTime && c == '#'){
           
           readingTime = true; 
            
          }
          
          
          
          if(!hasCurrentData){
         
            if(!readingData){
             
               if(c == '<'){
                  readingData = true;
               } 
              
            }
            else{
               if(currentDataSize < 12){
                 //Serial.print(c);
                 
                 currentData[currentDataSize] = c;
                 
                 currentDataSize++;
                 
                 //for(int i=0;i<12;i++){
                 //   Serial.print(currentData[i]);
                 //}
                 //Serial.println();
               }
               else{
                  hasCurrentData = true;
                  tick_strip(&three_strip,currentData[9] - '0',currentData[10] - '0',currentData[11] - '0',&three_last_move,&three_going_up,&three_last_brightness);
                  three_strip.show();
                  Serial.print("Data: ");
                  Serial.println(currentData);
                  //xbee.println("1");
              }
            }
          }
        }
      
      if(!hasCurrentData){
        hasCurrentData = true;
        currentDataSize = 12;
        currentData = oldData;
        //setDemoData();
        Serial.println("did not get new data");
        //xbee.println("0");
      }

}

