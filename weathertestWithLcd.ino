#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> 
#include <ArduinoJson.h> 
#include <WiFiClient.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiClient wiFiClient;
WiFiClient wiFiClient1;
WiFiClient wiFiClient2;

const char *ssid = "pato";
const char *password = "mayurpato";
 
String Location = "Vellore, IN";
String API_Key = " ";

String air1,air2;

const char* url1 = "https://api.thingspeak.com/apps/thinghttp/send_request?api_key=HTGEPHLQ5K5DG9NW";
const char* host1 = "api.thingspeak.com";

const char* url2 = "https://api.thingspeak.com/apps/thinghttp/send_request?api_key=JL4QKUZ12KZL08A4";
const char* host2 = "api.thingspeak.com";

const int httpPortRead = 80;  
 
void setup(void)
{
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();
  delay(1000);
  WiFi.begin(ssid, password);
  Serial.print("Connecting.");
  while ( WiFi.status() != WL_CONNECTED )
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected");
  delay(1000);
}
 
void loop()
{
  if (WiFi.status() == WL_CONNECTED) 
  {
    
    HTTPClient http;  
    http.begin(wiFiClient,"http://api.openweathermap.org/data/2.5/weather?q=" + Location + "&APPID=" + API_Key); 
    int httpCode = http.GET(); 
    
    if (httpCode > 0) 
    {
      String payload = http.getString(); 
     
      DynamicJsonBuffer jsonBuffer(512);
      JsonObject& root = jsonBuffer.parseObject(payload);
      if (!root.success()) {
      Serial.println(F("Parsing failed!"));
      return;
      }
     
      float temp = (float)(root["main"]["temp"]) - 273.15; 
      int humidity = root["main"]["humidity"]; 
      float pressure = (float)(root["main"]["pressure"]) / 1000; 
      float wind_speed = root["wind"]["speed"]; 
      int wind_degree = root["wind"]["deg"]; 
      float feels = (float)(root["main"]["feels_like"]) - 273.15;
      float temp_max = (float)(root["main"]["temp_max"]) - 273.15;
      float temp_min = (float)(root["main"]["temp_min"]) - 273.15;
      int visible = (root["visibility"]);
      //String sky = (String)(root["weather"]["description"]);
       
      Serial.printf("Temperature = %.2f°C\r\n", temp);
      Serial.printf("Feels Like %.2f°C\r\n", feels);
      Serial.printf("Humidity = %d %%\r\n", humidity);
      Serial.printf("Pressure = %.3f bar\r\n", pressure);
      Serial.printf("Wind Speed = %.1f m/s\r\n", wind_speed);
      Serial.printf("Wind Degree = %d°\r\n", wind_degree);
      //Serial.printf("Max Temperature = %.2f°C\r\n", temp_max);
      //Serial.printf("Min Temperature = %.2f°C\r\n", temp_min);
      Serial.printf("Visibility = %d km\r\n\r", visible);
      
      if(http.begin(wiFiClient1,host1,httpPortRead,url1))                                          
          {
            int httpCode = http.GET();                                                                  
            if (httpCode > 0)                                                              
              {
               if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
                  {                
                    air2 = http.getString();                                                                               
                  }
              }                                                                    
            }
      
      if(http.begin(wiFiClient2,host2,httpPortRead,url2))                                          
          {
            int httpCode = http.GET();                                                                  
            if (httpCode > 0)                                                              
              {
               if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
                  {                
                    air1 = http.getString();
                    Serial.print("Air Quality Index = ");
                    Serial.print(air1);Serial.print(" (");Serial.print(air2);Serial.println(")");
                    Serial.println();Serial.println();                                                                                 
                  }
              }                                                                    
           }
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Temp: ");
          lcd.print(temp);
          lcd.print(" C");
          lcd.setCursor(0,1);
          lcd.print("Hum: ");
          lcd.print(humidity);
          delay(10000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Pres: ");
          lcd.print(pressure);
          lcd.print("bar");
          lcd.setCursor(0,1);
          lcd.print("WS: ");
          lcd.print(wind_speed);
          delay(10000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Feels Like: ");
          lcd.print(feels);
          lcd.print("bar");
          lcd.setCursor(0,1);
          lcd.print("Visibility: ");
          lcd.print(visible);

         
    }
     
    http.end();
  
  }
   
  delay(20000);
 
}
