#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> 
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

WiFiClient wiFiClient;

const char *ssid = "pato";
const char *password = "mayurpato";

const int RXPin = 4, TXPin = 5;
const uint32_t GPSBaud = 9600; 
SoftwareSerial gps_module(RXPin, TXPin);

TinyGPSPlus gps; 

float gps_speed;
float no_of_satellites;
String satellite_orientation;
double latitude;
double longitude;  
unsigned int move_index = 1;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  gps_module.begin(GPSBaud);
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

void checkGPS(){
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring.")); 
  }
}


void loop()
{
  while (gps_module.available() > 0) 
  {
    if (gps.encode(gps_module.read()))
    displayInfo();
  }
 
}

void displayInfo()
{
  if (gps.location.isValid()) 
  {
    latitude = (gps.location.lat());
    longitude = (gps.location.lng());      
    gps_speed = gps.speed.kmph();
    no_of_satellites = gps.satellites.value();
    satellite_orientation = TinyGPSPlus::cardinal(gps.course.value());
    Serial.print("LAT:  ");
    Serial.println(latitude, 6);  // float to x decimal places
    Serial.print("LONG: ");
    Serial.println(longitude, 6);
  }
  if(Serial.available() > 0){
    char x = Serial.read();
    if(x == 't');{
    if (WiFi.status() == WL_CONNECTED) 
  {
    HTTPClient http;  
    String url = "http://maker.ifttt.com/trigger/SMS_GPS/with/key/jwzMImMUxfm2PytL-NJrgb3yVU0-YB-yWoXBFWnZsAo";
    url += "?value1=";
    url += longitude;
    url += "&value2=";
    url += latitude;
    http.begin(wiFiClient,url); 
    int httpCode = http.GET(); 
    
    if (httpCode > 0) 
    {
      String payload = http.getString(); 
      Serial.println(payload);
    }
    http.end();
  }
  
  }
  }
 
}
