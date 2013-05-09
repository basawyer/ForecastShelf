#include <Arduino.h>
#include <Time.h>
#include <SoftwareSerial.h>
#include <TimeAlarms.h>
#include <Streaming.h>
//#include <PString.h>
#include <WiFlySerial.h>
#include "LPD8806.h"
#include "SPI.h"
#include <stdio.h>
#include <stdlib.h>

#define ARDUINO_RX_PIN  0
#define ARDUINO_TX_PIN  1

#define TODAY_DATA 2
#define TODAY_CLOCK 3   

#define ONE_DATA 4
#define ONE_CLOCK 5

#define TWO_DATA 8
#define TWO_CLOCK 10

#define THREE_DATA 12
#define THREE_CLOCK 11

#define DIM_PIN 0

#define MY_SERVER_GET "forecastshelf.blakesawyer.net"
#define MY_SERVER_GET_URL "GET http://forecastshelf.blakesawyer.net/getshelfdata.php?zip=24060 HTTP/1.0"

#define SSID "xanadu"
#define PASSPHRASE "stevemcqueen"

char *currentData = "719429639149";
int currentDataSize = 0;
boolean hasCurrentData = false;
boolean readingData = false;

#define PI 3.14159265

//Today's variables
LPD8806 today_strip = LPD8806(14, TODAY_DATA, TODAY_CLOCK);
int today_current_pos = 0;
unsigned long today_last_move = millis();
boolean today_going_up = true;
float today_last_brightness = 1.0;
uint32_t today_current_colors[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//Day one variables
LPD8806 one_strip = LPD8806(6, ONE_DATA, ONE_CLOCK);
int one_current_pos = 0;
unsigned long one_last_move = millis();
boolean one_going_up = true;
float one_last_brightness = 1.0;
uint32_t one_current_colors[6] = {0,0,0,0,0,0};

//Day two variables
LPD8806 two_strip = LPD8806(6, TWO_DATA, TWO_CLOCK);
int two_current_pos = 0;
unsigned long two_last_move = millis();
boolean two_going_up = true;
float two_last_brightness = 1.0;
uint32_t two_current_colors[6] = {0,0,0,0,0,0};

//Day three variables
LPD8806 three_strip = LPD8806(6, THREE_DATA, THREE_CLOCK);
int three_current_pos = 0;
unsigned long three_last_move = millis();
boolean three_going_up = true;
float three_last_brightness = 1.0;
uint32_t three_current_colors[6] = {0,0,0,0,0,0};

WiFlySerial wifi(ARDUINO_RX_PIN,ARDUINO_TX_PIN);

//Colors
uint32_t blue = today_strip.Color(0,0,127);
uint32_t dark_gray = today_strip.Color(0,0,0);
uint32_t white = today_strip.Color(127,127,127);
uint32_t light_gray = today_strip.Color(96,96,96);
uint32_t light_blue = today_strip.Color(0,127,127);
uint32_t yellow = today_strip.Color(127,127,0);
uint32_t green = today_strip.Color(0,127,0);
uint32_t orange = today_strip.Color(127,64,32);

//COLOR SCHEMES
#define COLORSCHEME_SIZE 10
uint32_t tstorms[10] = {blue,blue,blue,blue,blue,blue,blue,blue,blue,blue};
uint32_t rain[10] = {blue,blue,white,white,blue,blue,white,white,blue,blue};
uint32_t snowy[10] = {white,white,white,white,white,white,white,white,white,white};
uint32_t cloudy[10] = {dark_gray,white,light_gray,light_gray,white,white,light_gray,light_gray,white,dark_gray};
uint32_t mostly_cloudy[10] = {white,white,yellow,yellow,white,white,yellow,yellow,white,white};
uint32_t partly_cloudy[10] = {white,light_blue,light_blue,yellow,yellow,yellow,yellow,light_blue,light_blue,white};
uint32_t sunny[10] = {light_blue,light_blue,yellow,yellow,light_blue,light_blue,yellow,yellow,light_blue,light_blue};

uint32_t *schemes[] = {tstorms,rain,snowy,cloudy,mostly_cloudy,partly_cloudy,sunny};

int lightening_pattern[3] = {500,100,500};

void setup() {
  //Serial.begin(9600);
  
  randomSeed(analogRead(1));
  
  //setDemoData();
  //Serial.println(currentData[0]);

  today_strip.begin();
  one_strip.begin();
  two_strip.begin();
  three_strip.begin();

  today_strip.show();
  one_strip.show();
  two_strip.show();
  three_strip.show();
  
  //Serial.println("waiting 5 secs");

  delay(5000);
  
  setTime(8,29,0,1,1,11);
  
  wifi.begin();
  
  delay(1000);
  
  refreshData();

}




void loop(){
 
  if(hasCurrentData){
   
      tick_today();
      tick_one();
      tick_two();
      tick_three();
      
      today_strip.show();
      one_strip.show();
      two_strip.show();
      three_strip.show();

  }
  
  Alarm.delay(1);
 
}

void tick_today(){
  
  int condition = currentData[0] - '0';
  int wind = currentData[1] - '0';
  int temp = currentData[2] - '0';
  
  //Calculate current brightness breathing effect speed based on wind speed and time
  float windFactor = 0.25;
  if(wind == 1){
   windFactor = 0.25; 
  }
  if(wind == 2){
   windFactor = 0.5; 
  }
  if(wind == 3){
   windFactor = 1.0; 
  }
  if(wind == 4){
   windFactor = 1.5; 
  }

  float secondsLapsed = ((float)(millis() - today_last_move)) / 1000.0;
  //Serial.print("seconds sapsed: ");
  //Serial.println(secondsLapsed);
  windFactor = windFactor * secondsLapsed;
  float brightness = cos( windFactor ) + 1.0;
  brightness = (9.0*brightness) / 20.0;
  brightness = brightness + 0.1;
  //brightness = abs(brightness - 1.0);
  //Serial.print("last: ");
  //Serial.print(today_last_brightness,DEC);
  //Serial.print(" < brightness: ");
  //Serial.println(brightness,DEC);
  
  if(today_last_brightness < brightness){
    
    //Serial.println("going up");
   
    if(!today_going_up){ // Hit a trough, shift color scheme
       //today_last_move = millis();
       //Serial.println("Shifting down color scheme:--------------------------------------------");
       for(int i=0;i<today_strip.numPixels();i++){
        
         int index = today_current_pos + i;
         while(index >= COLORSCHEME_SIZE){
          
           index = index - COLORSCHEME_SIZE;
         } 
         
         today_current_colors[i] = schemes[condition-1][index];
  
       }
     
       today_current_pos++;
       if(today_current_pos >= COLORSCHEME_SIZE){
         today_current_pos = 0;
       }
    }
      
     today_going_up = true;
     today_last_brightness = brightness;   
  }
  else{

    if(today_going_up){
      today_last_move = millis();
      today_last_brightness = 1.0;
    }
    else{
      today_last_brightness = brightness;
    }
    today_going_up = false;
  }
  
    
  //Now change brightness based off current temperatures
  float tempFactor = ((float)temp) / 9.0;
  brightness = brightness * tempFactor;
  
  //Now adjust brightness based off the shelf's physical knob
  brightness = brightness * getDimReading();
  
  //Serial.print("Brightness = ");
  //Serial.println(brightness,DEC);
  
  for(int i=0;i<today_strip.numPixels();i++){
    
    uint32_t c = today_current_colors[i];
    byte  r, g, b;
  
    // Need to decompose color into its r, g, b elements
    g = (c >> 16) & 0x7f;
    r = (c >>  8) & 0x7f;
    b =  c        & 0x7f;
 
    g = (byte)((float)g * brightness);
    r = (byte)((float)r * brightness);
    b = (byte)((float)b * brightness); 
    
    today_strip.setPixelColor(i, r, g, b);
  }
  
}

void tick_one(){
  
  int condition = currentData[3] - '0';
  int wind = currentData[4] - '0';
  int temp = currentData[5] - '0';
  
  //Calculate current brightness (breathing effect), speed based on wind speed and time
  float windFactor = 1.0;
  if(wind == 1){
   windFactor = 0.25; 
  }
  if(wind == 2){
   windFactor = 0.5; 
  }
  if(wind == 3){
   windFactor = 1.0; 
  }
  if(wind == 4){
   windFactor = 1.5; 
  }

  float secondsLapsed = ((float)(millis() - one_last_move)) / 1000.0;
  //Serial.print("seconds sapsed: ");
  //Serial.println(secondsLapsed);
  windFactor = windFactor * secondsLapsed;
  float brightness = cos( windFactor ) + 1.0;
  brightness = (9.0*brightness) / 20.0;
  brightness = brightness + 0.1;
  //brightness = abs(brightness - 1.0);
  //Serial.print("last: ");
  //Serial.print(today_last_brightness,DEC);
  //Serial.print(" < brightness: ");
  //Serial.println(brightness,DEC);
  
  if(one_last_brightness < brightness){
    
    //Serial.println("going up");
   
    if(!one_going_up){ // Hit a trough, shift color scheme
       //today_last_move = millis();
       //Serial.println("Shifting down color scheme:--------------------------------------------");
       for(int i=0;i<one_strip.numPixels();i++){
        
         int index = one_current_pos + i;
         while(index >= COLORSCHEME_SIZE){
          
           index = index - COLORSCHEME_SIZE;
         } 
         
         one_current_colors[i] = schemes[condition-1][index];
  
       }
     
       one_current_pos++;
       if(one_current_pos >= COLORSCHEME_SIZE){
         one_current_pos = 0;
       }
    }
      
     one_going_up = true;
     one_last_brightness = brightness;   
  }
  else{

    if(one_going_up){
      one_last_move = millis();
      one_last_brightness = 1.0;
    }
    else{
      one_last_brightness = brightness;
    }
    one_going_up = false;
  }

  
  //Now change brightness based off current temperatures
  float tempFactor = ((float)temp) / 9.0;
  brightness = brightness * tempFactor;
  
  //Now adjust brightness based off the shelf's physical knob
  brightness = brightness * getDimReading();
  
  //Serial.print("Brightness = ");
  //Serial.println(brightness,DEC);
  
  for(int i=0;i<one_strip.numPixels();i++){
    
    uint32_t c = one_current_colors[i];
    byte  r, g, b;
  
    // Need to decompose color into its r, g, b elements
    g = (c >> 16) & 0x7f;
    r = (c >>  8) & 0x7f;
    b =  c        & 0x7f;
 
    g = (byte)((float)g * brightness);
    r = (byte)((float)r * brightness);
    b = (byte)((float)b * brightness); 
    
    one_strip.setPixelColor(i, r, g, b);
  }
  
}

void tick_two(){
  
  int condition = currentData[6] - '0';
  int wind = currentData[7] - '0';
  int temp = currentData[8] - '0';
  
  //Calculate current brightness (breathing effect), speed based on wind speed and time
  float windFactor = 1.0;
  if(wind == 1){
   windFactor = 0.25; 
  }
  if(wind == 2){
   windFactor = 0.5; 
  }
  if(wind == 3){
   windFactor = 1.0; 
  }
  if(wind == 4){
   windFactor = 1.5; 
  }

  float secondsLapsed = ((float)(millis() - two_last_move)) / 1000.0;
  //Serial.print("seconds sapsed: ");
  //Serial.println(secondsLapsed);
  windFactor = windFactor * secondsLapsed;
  float brightness = cos( windFactor ) + 1.0;
  brightness = (9.0*brightness) / 20.0;
  brightness = brightness + 0.1;
  //brightness = abs(brightness - 1.0);
  //Serial.print("last: ");
  //Serial.print(today_last_brightness,DEC);
  //Serial.print(" < brightness: ");
  //Serial.println(brightness,DEC);
  
  if(two_last_brightness < brightness){
    
    //Serial.println("going up");
   
    if(!two_going_up){ // Hit a trough, shift color scheme
       //today_last_move = millis();
       //Serial.println("Shifting down color scheme:--------------------------------------------");
       for(int i=0;i<two_strip.numPixels();i++){
        
         int index = two_current_pos + i;
         while(index >= COLORSCHEME_SIZE){
          
           index = index - COLORSCHEME_SIZE;
         } 
         
         two_current_colors[i] = schemes[condition-1][index];
  
       }
     
       two_current_pos++;
       if(two_current_pos >= COLORSCHEME_SIZE){
         two_current_pos = 0;
       }
    }
      
     two_going_up = true;
     two_last_brightness = brightness;   
  }
  else{

    if(two_going_up){
      two_last_move = millis();
      two_last_brightness = 1.0;
    }
    else{
      two_last_brightness = brightness;
    }
    two_going_up = false;
  }
    
  //Now change brightness based off current temperatures
  float tempFactor = ((float)temp) / 9.0;
  brightness = brightness * tempFactor;
  
  //Now adjust brightness based off the shelf's physical knob
  brightness = brightness * getDimReading();
  
  //Serial.print("Brightness = ");
  //Serial.println(brightness,DEC);
  
  for(int i=0;i<two_strip.numPixels();i++){
    
    uint32_t c = two_current_colors[i];
    byte  r, g, b;
  
    // Need to decompose color into its r, g, b elements
    g = (c >> 16) & 0x7f;
    r = (c >>  8) & 0x7f;
    b =  c        & 0x7f;
 
    g = (byte)((float)g * brightness);
    r = (byte)((float)r * brightness);
    b = (byte)((float)b * brightness); 
    
    two_strip.setPixelColor(i, r, g, b);
  }
  
}

void tick_three(){
  
  int condition = currentData[9] - '0';
  int wind = currentData[10] - '0';
  int temp = currentData[11] - '0';
  
  //Calculate current brightness (breathing effect), speed based on wind speed and time
  float windFactor = 1.0;
  if(wind == 1){
   windFactor = 0.25; 
  }
  if(wind == 2){
   windFactor = 0.5; 
  }
  if(wind == 3){
   windFactor = 1.0; 
  }
  if(wind == 4){
   windFactor = 1.5; 
  }

  float secondsLapsed = ((float)(millis() - three_last_move)) / 1000.0;
  //Serial.print("seconds sapsed: ");
  //Serial.println(secondsLapsed);
  windFactor = windFactor * secondsLapsed;
  float brightness = cos( windFactor ) + 1.0;
  brightness = (9.0*brightness) / 20.0;
  brightness = brightness + 0.1;
  //brightness = abs(brightness - 1.0);
  //Serial.print("last: ");
  //Serial.print(today_last_brightness,DEC);
  //Serial.print(" < brightness: ");
  //Serial.println(brightness,DEC);
  
  if(three_last_brightness < brightness){
    
    //Serial.println("going up");
   
    if(!three_going_up){ // Hit a trough, shift color scheme
       //three_last_move = millis();
       //Serial.println("Shifting down color scheme:--------------------------------------------");
       for(int i=0;i<three_strip.numPixels();i++){
        
         int index = three_current_pos + i;
         while(index >= COLORSCHEME_SIZE){
          
           index = index - COLORSCHEME_SIZE;
         } 
         
         three_current_colors[i] = schemes[condition-1][index];
  
       }
     
       three_current_pos++;
       if(three_current_pos >= COLORSCHEME_SIZE){
         three_current_pos = 0;
       }
    }
      
     three_going_up = true;
     three_last_brightness = brightness;   
  }
  else{

    if(three_going_up){
      three_last_move = millis();
      three_last_brightness = 1.0;
    }
    else{
      three_last_brightness = brightness;
    }
    three_going_up = false;
  }
    
  //Now change brightness based off current temperatures
  float tempFactor = ((float)temp) / 9.0;
  brightness = brightness * tempFactor;
  
  //Now adjust brightness based off the shelf's physical knob
  brightness = brightness * getDimReading();
  
  //Serial.print("Brightness = ");
  //Serial.println(brightness,DEC);
  
  for(int i=0;i<three_strip.numPixels();i++){
    
    uint32_t c = three_current_colors[i];
    byte  r, g, b;
  
    // Need to decompose color into its r, g, b elements
    g = (c >> 16) & 0x7f;
    r = (c >>  8) & 0x7f;
    b =  c        & 0x7f;
 
    g = (byte)((float)g * brightness);
    r = (byte)((float)r * brightness);
    b = (byte)((float)b * brightness); 
    
    three_strip.setPixelColor(i, r, g, b);
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

void setDemoData(){
  
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
  
}

void refreshData(){

    hasCurrentData = false;
    currentDataSize = 0;
    readingData = false;
    
    if(!wifi.isAuthenticated()){
      //Serial.println("setting pass");
      wifi.setPassphrase(PASSPHRASE);
      //Serial.println("joining");  
      wifi.join(SSID);
      //Serial.println("joined");
    }

  
    //Serial.println("trying to get data");
    
    //Serial.println("opening connection");
    
    if(wifi.openConnection(MY_SERVER_GET)){
      
      //Serial.println("sending get request");
     
       wifi.println(MY_SERVER_GET_URL);
       wifi.println();
       
       unsigned long timeout = millis() + 5000;
       
       while (  timeout > millis() && wifi.isConnectionOpen() ) {
        if (  wifi.available() > 0 ) {
          char c = (char) wifi.read();
          
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
                  //Serial.print("Data: ");
                  //Serial.println(currentData);
                  //Serial.println("Got data: refreshing in 15min");
                  Alarm.timerOnce(60*15, refreshData);
              }
            }
          }
        }
      }//WHILE wifi is connected and not timed out
      // Force-close connection
      wifi.closeConnection();
      
      if(!hasCurrentData){
        hasCurrentData = true;
        currentDataSize = 12;
        setDemoData();
        Alarm.timerOnce(60*2, refreshData);
        //Serial.println("connection failed: going to demo mode, trying again in 2mins");
      }
    }
    else{
      hasCurrentData = true;
      currentDataSize = 12;
      setDemoData();
      Alarm.timerOnce(60*2, refreshData);
      //Serial.println("connection failed: going to demo mode, trying again in 2mins");
    }

}

