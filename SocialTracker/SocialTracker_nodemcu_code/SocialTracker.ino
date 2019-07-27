#include <Arduino.h>
#include <SPI.h>

#include <ArduinoJson.h>           // https://github.com/bblanchon/ArduinoJson
#include "InstagramStats.h"       // https://github.com/witnessmenow/arduino-instagram-stats
#include "JsonStreamingParser.h"  // https://github.com/squix78/json-streaming-parser
#include <YoutubeApi.h>           // https://github.com/witnessmenow/arduino-youtube-api/
#include <ESP8266WiFi.h> 

#include <WiFiClientSecure.h> 
#include<WiFiManager.h>

#include <TM1637Display.h>

const int CLK = D1; //Set the CLK pin connection to the display
const int facebookPin = D2;

const int youtubePin = D3;

const int instaPin = D4;


#define HotSpotPin D8

TM1637Display facebookDisplay0(CLK, facebookPin); //set up the 4-Digit Display.
//TM1637Display facebookDisplay(CLK, facebookPin2);
TM1637Display youtubeDisplay0(CLK, youtubePin);
//TM1637Display youtubeDisplay(CLK, youtubePin2);
TM1637Display instaDisplay0(CLK, instaPin);
//TM1637Display instaDisplay(CLK, instaPin2);

WiFiManager wifiManager;

void wifi()
{
  
  //exit after config instead of connecting
  wifiManager.setBreakAfterConfig(true);

  //wifiManager.resetSettings();
  
  wifiManager.setTimeout(30);
  //tries to connect to last known settings
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP" with password "password"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("SmartPlug", "password")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  Serial.println("local ip");
  Serial.println(WiFi.localIP());
}

void wifireset()
{
//exit after config instead of connecting
  //wifiManager.setBreakAfterConfig(true);

  wifiManager.resetSettings();
  delay(500);
  //tries to connect to last known settings
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP" with password "password"
  //and goes into a blocking loop awaiting configuration

    ESP.reset();
  

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

 // Serial.println("local ip");
//  Serial.println(WiFi.localIP());
  
}

//#define WIFI_SSID "JARVIS"       // Enter your SSID here
//#define WIFI_PASS "111111111"    // Enter your WiFi password here
#define WIFI_HOSTNAME "Social_Counter" 
#define PORT 5444
#define WIFICLIENT_MAX_PACKET_SIZE 100
/*_______End of WiFi definitions______*/
 

/*______Define Facebook config_______*/ 
#define FACEBOOK_HOST "graph.facebook.com"
#define FACEBOOK_PORT 443
#define PAGE_ID "1801701440063674 "
#define ACCESS_TOKEN "EAAHA6rZB3Xm8BAN31WOiCcDbRybZBgbq9xgTLUUxD6NN5ltDK1aWOS0nFTfwvVfaoTJqWZAqvpqIpq4mGlA3ovLGZAJT3TJw47wYfyv39YxyyBpqKDfUmBzu7NbKX0bFxiYioaEQ3VWFRPK6KowpuHejSEbZBPxZBZBnrFNbC7wawZDZD"
// graph.facebook.com SHA1 fingerprint
const char* facebookGraphFingerPrint = "c522f115f8b2adae1263bc8d5fa7ba80f4f8a2f7";
/*_______End of Facebook config______*/

/*______Define Youtube config_______*/
#define API_KEY "AIzaSyCX3-aI_T_gANDe9dhmCcAMCfU36HU2A8I"  // your google apps API Token
#define CHANNEL_ID "UCxkP6bJRc5JGeV-h0uif4tQ" // makes up the url of channel
/*_______End of Youtube config______*/


/*______Define Instagram config_______*/ 
String Instagram_userName = "technicalhubio";    // from their instagram url https://www.instagram.com/userName/ 
/*_______End of Youtube config______*/


int FB_Follower=0;
long YT_Subscriber=0;
long YT_Videos=0;
long YT_Views=0;
int IG_Follower=0;

WiFiClientSecure secureClient;
YoutubeApi api(API_KEY, secureClient); 
InstagramStats instaStats(secureClient); 

//void connectWifi() {
//  if (WiFi.status() == WL_CONNECTED) return;
//  //Manual Wifi
//  Serial.print("Connecting to WiFi ");
//  Serial.print(WIFI_SSID);
//  Serial.print("/");
//  Serial.println(WIFI_PASS);
//  WiFi.mode(WIFI_STA);
//  WiFi.hostname(WIFI_HOSTNAME);
//  WiFi.begin(WIFI_SSID,WIFI_PASS);
//  int i = 0;
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    if (i>80) i=0;
//    i+=10;
//    Serial.print(".");
//  }
//  Serial.print("Connected...");
//} 

void displayStatus()
{
  if(FB_Follower/10000 == 0)
  {
    facebookDisplay0.showNumberDec(FB_Follower); //Display the facebook count.
  }
  else
  {
    facebookDisplay0.showNumberDec(FB_Follower%10000); //Display the facebook count.
//    facebookDisplay.showNumberDec(FB_Follower/10000); //Display the facebook count.
  }

  if(YT_Subscriber/10000 == 0)
  {
    youtubeDisplay0.showNumberDec(YT_Subscriber); //Display the youtube count.
  }
  else
  {
    youtubeDisplay0.showNumberDec(YT_Subscriber%10000); //Display the youtube count.
//    youtubeDisplay.showNumberDec(YT_Subscriber/10000); //Display the youtube count.
  }
  if(IG_Follower/10000 == 0)
  {
    instaDisplay0.showNumberDec(IG_Follower); //Display the insta count.
  }
  else
  {
    instaDisplay0.showNumberDec(IG_Follower%10000); //Display the insta count.
//    instaDisplay.showNumberDec(IG_Follower/10000); //Display the insta count.
  }
 
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  pinMode(HotSpotPin,INPUT);
  facebookDisplay0.setBrightness(0x0a);
  youtubeDisplay0.setBrightness(0x0a);
  instaDisplay0.setBrightness(0x0a);
//  facebookDisplay.setBrightness(0x0a);
//  youtubeDisplay.setBrightness(0x0a);
//  instaDisplay.setBrightness(0x0a);
  wifi();
//Serial.print("Connecting Wifi: ");
//  Serial.println(WIFI_SSID);
//  WiFi.begin(WIFI_SSID, WIFI_PASS);
//  while (WiFi.status() != WL_CONNECTED) {
//    Serial.print(".");
//    delay(500);
//  }
  pinMode(HotSpotPin,INPUT);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip); 
}

void loop() {
  bool HOTSPOT=digitalRead(HotSpotPin);
  if(digitalRead(HotSpotPin)== HIGH)
    {
      Serial.println(digitalRead(HotSpotPin));
      Serial.println("Hotspot reset");
       wifireset();
    }
  if(strlen(WiFi.localIP().toString().c_str())==0)
    {
      Serial.println("Jarvis reconnect");
      wifi();
    }
  // put your main code here, to run repeatedly:
  FB_Follower=getPageFansCount();
  Serial.print("Followers:");
  Serial.println(FB_Follower);
  getYoutubeStats();
  getInstagramStatsForUser();
  displayStatus();
  delay(10000);
}

void getInstagramStatsForUser() {
  Serial.println("Getting instagram user stats for channel" + Instagram_userName );
  InstagramUserStats response = instaStats.getUserStats(Instagram_userName);
  Serial.println("Response:");
  Serial.print("Number of followers: ");
  IG_Follower = response.followedByCount;
  Serial.println(response.followedByCount);    
} 

void getYoutubeStats() {
  Serial.println("Getting youtube for " );
  if(api.getChannelStatistics(CHANNEL_ID))
  {
      Serial.println("---------Stats---------");
      Serial.print("Subscriber Count: ");
      Serial.println(api.channelStats.subscriberCount);
      YT_Subscriber=api.channelStats.subscriberCount;
      Serial.print("View Count: ");
      Serial.println(api.channelStats.viewCount);
      YT_Views=api.channelStats.viewCount;
      Serial.print("Comment Count: ");
      Serial.println(api.channelStats.commentCount);
      Serial.print("Video Count: ");
      Serial.println(api.channelStats.videoCount);
      YT_Videos=api.channelStats.videoCount;
      Serial.println("------------------------");
   } 
   
}

String makeRequestGraph(){
  if (!secureClient.connect(FACEBOOK_HOST, FACEBOOK_PORT)) {
    Serial.println("Host connection failed");
    return "";    
  }
  
  String params = "?fields=fan_count&access_token="+String(ACCESS_TOKEN);
  String path = "/v3.0/1801701440063674";
  String url = path + params;
  Serial.print("requesting URL: ");
  Serial.println(url);

  String request = "GET " + url + " HTTP/1.1\r\n" +
    "Host: " + String(FACEBOOK_HOST) + "\r\n\r\n";
  
  secureClient.print(request);

  String response = "";
  String chunk = "";  
  
  do {
    if (secureClient.connected()) {
      delay(5);
      
      chunk = secureClient.readStringUntil('\n');
      if(chunk.startsWith("{")){
        Serial.print(chunk);
        response += chunk;
      }
    }
  } while (chunk.length() > 0);
    
  Serial.print(" Message ");
  Serial.println(response);  

  return response;
}

int getPageFansCount(){
  String response = makeRequestGraph();  
  
  const size_t bufferSize = JSON_OBJECT_SIZE(2) + 40;
  DynamicJsonBuffer jsonBuffer(bufferSize);  
  
  JsonObject& root = jsonBuffer.parseObject(response);
  
  int fanCount = root["fan_count"];  
  return fanCount;
}
