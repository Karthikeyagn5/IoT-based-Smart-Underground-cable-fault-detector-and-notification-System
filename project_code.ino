#define BLYNK_TEMPLATE_ID "TMPL3QWndtMZA"
#define BLYNK_TEMPLATE_NAME "cablefaultdetection"
#define BLYNK_AUTH_TOKEN "w7yH4HsAMFZIQyU1sh3BQoMtZdAR"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <TinyGPS++.h>
TinyGPSPlus gps; // The TinyGPS++ object
float spd;       //Variable  to store the speed
float sats;      //Variable to store no. of satellites response
String bearing;  //Variable to store orientation or direction of GPS

float latitude;
float longitude;
//unsigned int move_index;         // moving index, to be used later
unsigned int move_index = 1;       // fixed location for now
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
BlynkTimer timer;

int cable1 =D5;
int cableval1;
int cable2 =D6;
int cableval2;
int cable3 =D7;
int cableval3;
int buzzer =D8;
bool cablestatus1 =false;
bool cablestatus2 =false;
bool cablestatus3 =false;

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "iot";
char pass[] = "12345678";

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting to....");
  lcd.setCursor(0, 1);
  lcd.print(ssid);
  Blynk.begin(auth, ssid, pass);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LINE CABLE FAULT");
  lcd.setCursor(0, 1);
  lcd.print("   DETECTION");
  delay(2000);
  pinMode(cable1 ,INPUT);
  pinMode(cable2 ,INPUT);
  pinMode(cable3 ,INPUT);
  pinMode(buzzer,OUTPUT);
  timer.setInterval(5000L, main_fun);
 }

 void main_fun()
 {
  cableval1 =digitalRead(cable1);
  cableval2 =digitalRead(cable2);
  cableval3 =digitalRead(cable3);

  if(cableval1 ==0 && !cablestatus1)
  {
    cablestatus1 =true;
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("CABLE FAULT AT");
   lcd.setCursor(0,1);
   lcd.print("2KM LOCATION");
   Blynk.logEvent("cable1","CABLE FAULT AT 2KM LOCATION\nhttp://maps.google.com/maps?q=loc:12.6827943,77.7153571");
   Blynk.virtualWrite(V0,"CABLE FAULT AT 2KM LOCATION");
   digitalWrite(buzzer,HIGH);
   delay(2000);
   digitalWrite(buzzer,LOW);
   
  }
  else if(cableval1 ==0 && cablestatus1)
  {
    cablestatus1 =false;
    lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("NO CABLE FAULT");
   Blynk.virtualWrite(V0,"NO CABLE FAULT AT 2KM LOCATION");
   
   
  }
   if(cableval2 ==0 && !cablestatus2)
  {
    cablestatus2 =true;
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("CABLE2 FAULT AT");
   lcd.setCursor(0,1);
   lcd.print("3kM LOCATION");
   Blynk.logEvent("cable2","CABLE FAULT AT 3KM LOCATION\nhttp://maps.google.com/maps?q=loc:12.6827943,77.7153571");
   Blynk.virtualWrite(V1,"CABLE FAULT AT 3KM LOCATION");
   digitalWrite(buzzer,HIGH);
   delay(2000);
   digitalWrite(buzzer,LOW);
   
  }
  else if(cableval2 ==1 && cablestatus2)
  {
    cablestatus2 =false;
    lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("NO CABLE FAULT");
   Blynk.virtualWrite(V1,"NO CABLE FAULT AT 3KM LOCATION");
   
  }
   if(cableval3 ==0 && !cablestatus3)
  {
    cablestatus3 =true;
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("CABLE3 FAULT IN");
   lcd.setCursor(0,1);
   lcd.print("4KM LOCATION");
   Blynk.logEvent("cable3","CABLE FAULT AT 4KM LOCATION\nhttp://maps.google.com/maps?q=loc:12.6827943,77.7153571");
   Blynk.virtualWrite(V2,"CABLE FAULT AT 4KM LOCATION");
   digitalWrite(buzzer,HIGH);
   delay(2000);
   digitalWrite(buzzer,LOW);
   
  }
  else if(cableval3 ==1 && cablestatus3)
 
  {
    cablestatus3=false;
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("NO CABLE FAULT");
    Blynk.virtualWrite(V2,"NO CABLE FAULT AT 4KM LOCATION");
  }
 
 }

void checkGPS(){
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
  }
}

void loop()
{
  timer.run();
  Blynk.run();
 
    while (Serial.available() > 0)
    {
      // sketch displays information every time a new sentence is correctly encoded.
      if (gps.encode(Serial.read()))
        displayInfo();
  }
 
}

void displayInfo()
{

  if (gps.location.isValid() )
  {
   
     latitude = (gps.location.lat());     //Storing the Lat. and Lon.
     longitude = (gps.location.lng());
   
    Serial.print("LAT:  ");
    Serial.println(latitude, 6);  // float to x decimal places
    Serial.print("LONG: ");
    Serial.println(longitude, 6);  
    spd = gps.speed.kmph();               //get speed
    sats = gps.satellites.value();    //get number of satellites
    bearing = TinyGPSPlus::cardinal(gps.course.value()); // get the direction
    Serial.print("Speed: ");
    Serial.println(spd);
    Serial.print("satellite: ");
    Serial.println(sats);
    Serial.print("Bearing: ");
    Serial.println(bearing);
     
  }
  Serial.println();
}
