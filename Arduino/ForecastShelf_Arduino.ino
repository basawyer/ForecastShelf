#include <Arduino.h>
#include <Time.h>
#include <SoftwareSerial.h>
#include <SimpleTimer.h>
#include <Streaming.h>
#include <PString.h>
#include <WiFlySerial.h>
#include "LPD8806.h"
#include "SPI.h"

#define ARDUINO_RX_PIN  2
#define ARDUINO_TX_PIN  4

#define TODAY_DATA 2
#define TODAY_CLOCK 3

#define ONE_DATA 4
#define ONE_CLOCK 5

#define TWO_DATA 7
#define TWO_CLOCK 6

#define THREE_DATA 8
#define THREE_CLOCK 9

#define DIM_PIN 0

#define MY_SERVER_GET "forecastshelf.blakesawyer.net"
#define MY_SERVER_GET_URL "GET http://forecastshelf.blakesawyer.net/getshelfdata.php?zip=24060 HTTP/1.0"

#define SSID "xanadu"
#define PASSPHRASE "stevemcqueen"

char currentData[12] = "00000000000";
int currentDataSize = 0;
boolean hasCurrentData = false;
boolean readingData = false;

int dataRefreshRate = 1000*15*60; //15 minutes
SimpleTimer timer;
int timerID = -1;

LPD8806 today_strip = LPD8806(14, TODAY_DATA, TODAY_CLOCK);
int today_scheme_pos = 0;
int today_last_move = millis();

LPD8806 one_strip = LPD8806(6, ONE_DATA, ONE_CLOCK);
int one_scheme_pos = 0;
int one_last_move = millis();
int 

LPD8806 two_strip = LPD8806(6, TWO_DATA, TWO_CLOCK);
LPD8806 three_strip = LPD8806(6, THREE_DATA, THREE_CLOCK);

WiFlySerial wifi(ARDUINO_RX_PIN ,ARDUINO_TX_PIN);

uint32_t blue = today_strip.Color(0,0,127);
uint32_t dark_gray = today_strip.Color(20,20,20);
uint32_t white = today_strip.Color(127,127,127);
uint32_t light_gray = today_strip.Color(64,64,64);
uint32_t yellow = today_strip.Color(127,127,0);
uint32_t green = today_strip.Color(0,127,0);
uint32_t orange = today_strip.Color(127,127,20);

//COLOR SCHEMES
#define COLORSCHEME_SIZE 10
uint32_t tstorms[10] = {dark_gray,blue,dark_gray,blue,dark_gray,blue,dark_gray,blue,dark_gray,blue};
uint32_t rain[10] = {dark_gray,blue,dark_gray,blue,dark_gray,blue,dark_gray,blue,dark_gray,blue};
uint32_t snowy[10] = {white,white,light_gray,light_gray,white,white,light_gray,light_gray,white,white};
uint32_t cloudy[10] = {dark_gray,dark_gray,light_gray,light_gray,dark_gray,dark_gray,light_gray,light_gray,dark_gray,dark_gray};
uint32_t mostly_cloudy[10] = {dark_gray,dark_gray,white,yellow,light_gray,light_gray,yellow,white,dark_gray,dark_gray};
uint32_t partly_cloudy[10] = {green,white,dark_gray,orange,yellow,yellow,orange,dark_gray,white,green};
uint32_t sunny[10] = {green,green,orange,yellow,yellow,yellow,yellow,orange,green,green};

uint32_t schemes[7][];
uint32_t schemes[][0] = {tstorms};

void setup() {
  Serial.begin(9600);
  
  

  today_strip.begin();
  one_strip.begin();
  two_strip.begin();
  three_strip.begin();

  today_strip.show();
  one_strip.show();
  two_strip.show();
  three_strip.show();
  
  Serial.println("waiting 30 secs");
  
  //delay(30000);
  
  Serial.println("starting wifi");
  
  //wifi.begin();
  //Serial.println("setting pass");
  //wifi.setPassphrase(PASSPHRASE);
  //Serial.println("joining");  
  //wifi.join(SSID);
  Serial.println("closing all connections");
  
  // close any open connections
  //wifi.closeConnection();
  
 // timerID = timer.setInterval(dataRefreshRate,refreshData);
  
  //refreshData();
}




void loop(){
  
  //if(hasCurrentData){
  //for(int i=0;i<50;i++){
    
      
      tick_strip(today_strip,1,1,4);
      tick_strip(one_strip,2,1,4);
      tick_strip(two_strip,3,1,4);
      tick_strip(three_strip,7,1,4);
      

   
      today_strip.show();
      one_strip.show();
      two_strip.show();
      three_strip.show();
      
      //delay(500);
  //}
 // }
 // else{
   
 //    refreshData(); 
 // }
 
}

//Returns 0.0-1.0 from potentiometer
float getDimReading(){
      
  Serial.print(analogRead(DIM_PIN));
  return (float)(680.0 - (float)analogRead(DIM_PIN)) / 680.0;
  
}

void dimmer(LPD8806 strip, float value){
  
  uint32_t c = strip.Color(0,0,127);
  byte  r, g, b;
  
  // Need to decompose color into its r, g, b elements
  g = (c >> 16) & 0x7f;
  r = (c >>  8) & 0x7f;
  b =  c        & 0x7f;
 
   g = (byte)((float)g * value);
   r = (byte)((float)r * value);
   b = (byte)((float)b * value); 
  
  for(int i=0;i<strip.numPixels();i++){

     strip.setPixelColor(i, r, g, b);
    
  }
}

void tick_strip(LPD8806 strip, int x, int conditions, int wind, int gust){
  
  float y;
  byte  r, g, b, r2, g2, b2;
  
  uint32_t c = strip.Color(0,0,100);
  
  int cycles = 2;
  
  // Need to decompose color into its r, g, b elements
  g = (c >> 16) & 0x7f;
  r = (c >>  8) & 0x7f;
  b =  c        & 0x7f; 
  
  for(int i=0; i<strip.numPixels(); i++) {
      y = sin(PI * (float)cycles * (float)(x + i) / (float)strip.numPixels());
      if(y >= 0.0) {
        // Peaks of sine wave are white
        y  = 1.0 - y; // Translate Y to 0.0 (top) to 1.0 (center)
        r2 = 127 - (byte)((float)(127 - r) * y);
        g2 = 127 - (byte)((float)(127 - g) * y);
        b2 = 127 - (byte)((float)(127 - b) * y);
      } else {
        // Troughs of sine wave are black
        y += 1.0; // Translate Y to 0.0 (bottom) to 1.0 (center)
        r2 = (byte)((float)r * y);
        g2 = (byte)((float)g * y);
        b2 = (byte)((float)b * y);
      }
      strip.setPixelColor(i, r2, g2, b2);
  }
  
}

// Sine wave effect
#define PI 3.14159265
/*void wave(uint32_t c, int cycles, uint8_t wait) {
  float y;
  byte  r, g, b, r2, g2, b2;

  // Need to decompose color into its r, g, b elements
  g = (c >> 16) & 0x7f;
  r = (c >>  8) & 0x7f;
  b =  c        & 0x7f; 

  for(int x=0; x<(strip.numPixels()*5); x++)
  {
    for(int i=0; i<strip.numPixels(); i++) {
      y = sin(PI * (float)cycles * (float)(x + i) / (float)strip.numPixels());
      if(y >= 0.0) {
        // Peaks of sine wave are white
        y  = 1.0 - y; // Translate Y to 0.0 (top) to 1.0 (center)
        r2 = 127 - (byte)((float)(127 - r) * y);
        g2 = 127 - (byte)((float)(127 - g) * y);
        b2 = 127 - (byte)((float)(127 - b) * y);
      } else {
        // Troughs of sine wave are black
        y += 1.0; // Translate Y to 0.0 (bottom) to 1.0 (center)
        r2 = (byte)((float)r * y);
        g2 = (byte)((float)g * y);
        b2 = (byte)((float)b * y);
      }
      strip.setPixelColor(i, r2, g2, b2);
    }
    strip.show();
    delay(wait);
  }
}*/

void refreshData(){
  
  timer.disable(timerID);
   
  hasCurrentData = false;
  currentDataSize = 0;

  Serial.println("trying to get data");
  
  Serial.println("opening connection");
  
  if(wifi.openConnection(MY_SERVER_GET)){
    
    Serial.println("sending get request");
   
     wifi.println(MY_SERVER_GET_URL);
     wifi.println();
     
     unsigned long timeout = millis() + 15000;
     
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
               
               for(int i=0;i<12;i++){
                  Serial.print(currentData[i]);
               }
               Serial.println();
             }
             else{
                hasCurrentData = true;
                Serial.println("Got data: refreshing in 15min");
                timer.deleteTimer(timerID);
                timerID = timer.setInterval(dataRefreshRate,refreshData);
            }
          }
        }
      }
    }//WHILE wifi is connected and not timed out
    // Force-close connection
    wifi.closeConnection();
  }
  else{
    
    Serial.println("connection failed");
  }

  timer.enable(timerID);
}

