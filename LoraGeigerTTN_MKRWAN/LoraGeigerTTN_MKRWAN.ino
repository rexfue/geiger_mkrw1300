#include <MKRWAN.h>
#include <CayenneLPP.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define MAXCNT 100 
#define CalFactor 1 

LoRaModem modem;

#define OLED_RESET 4
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire,OLED_RESET);

//#include "arduino_secrets.h"
// Please enter your sensitive data in the Secret tab or arduino_secrets.h
String appEui = "70B3D57ED0017A98";
String appKey = "61989423979892487C2597C2BAA3DB20";


CayenneLPP lpp(51);

volatile int counter = 0; 
unsigned long oldTime = 0; 

void writeText(String txt, int x, int y, int size) {
  display.setTextSize(size);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(x, y);     // Start at top-left corner
  display.print(txt);
  display.display();
}


void setup()
{
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(1000);
  display.clearDisplay();
  delay(1000);
  writeText("     Geigerzaehler",0,0,1);
  
  pinMode(LED_BUILTIN, OUTPUT);        //sign of live
/*  for (int i=1; i<=10; i++) {           
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);
    // wait for a second
    Serial.println(10-i);
  }
*/  
  // change this to your regional band (eg. US915, AS923, ...)
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  };
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());
  
  writeText("DEVEUI: ",0,20,1);
  writeText(String(modem.deviceEUI()),10,30,1);
  
  Serial.println("Trying to join TTN ...");
  writeText("Joining ..",0,40,1);
  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
 //   while (1) {}
  }
  else
    Serial.println("Successfully joined");
    writeText("Joined !",65,40,1);
  // Set poll interval to 60 secs.
  modem.minPollInterval(60);
  // NOTE: independently by this setting the modem will
  // not allow to send more than one message every 2 minutes,
  // this is enforced by firmware and can not be changed.

  delay(2000);
  display.clearDisplay();
  writeText("     Geigerzaehler",0,0,1);
  Serial.println("Start waiting for geiger pulses ...");
  attachInterrupt(digitalPinToInterrupt(0), count, FALLING); 

}

void loop()
{
  unsigned long time; 
  unsigned long dt; 
  float rate; 
  int err;

  if (counter == MAXCNT) { 
    detachInterrupt(digitalPinToInterrupt(0)); 
    time = millis(); 
    dt = time-oldTime; 
    rate = (float)MAXCNT*60.0*1000.0/(float)dt/CalFactor; 
    Serial.println(round(rate));
    
    display.clearDisplay();
    writeText("     Geigerzaehler",0,0,1);
    writeText(String(rate),10,30,2);
    writeText("cpm",100,50,1);
    Serial.println("Sent to LCD");
    
    lpp.reset();
    lpp.addLuminosity(1, round(rate));
    modem.beginPacket();
    modem.write(lpp.getBuffer(), lpp.getSize());
    err = modem.endPacket(true);
    if (err > 0) {
      Serial.println("Message sent correctly!");
    } else {
      Serial.println("Error sending message :(");
      Serial.println("(you may send a limited amount of messages per minute, depending on the signal strength");
      Serial.println("it may vary from 1 message every couple of seconds to 1 message every minute)");
    }
    delay(1000);
    if (!modem.available()) {
      Serial.println("No downlink message received at this time.");
    }
    else {
      String rcv;
      rcv.reserve(64);
      while (modem.available()) {
        rcv += (char)modem.read();
      }
      Serial.print("Received: " + rcv + " - ");
      for (unsigned int i = 0; i < rcv.length(); i++) {
        Serial.print(rcv[i] >> 4, HEX);
        Serial.print(rcv[i] & 0xF, HEX);
        Serial.print(" ");
      }
      Serial.println();
    }
    Serial.println("waiting 60 seconds");
    delay(60000);
    
    oldTime = millis(); 
    counter = 0;     
    attachInterrupt(digitalPinToInterrupt(0), count, FALLING);   
  }   
}

void count() 
{ 
  counter++; 
}
