/////////////GPS Module
#include <SoftwareSerial.h> 
#include <LiquidCrystal.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
int RXPin = 4;
int TXPin = 7;

int GPSBaud = 9600;
float latitude,longitude ;
// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial gpsSerial(RXPin, TXPin);

const int buzzer = 6;
const int rs = 9, en = 8, d4 = 12, d5 = 10, d6 = 11, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//#include <TinyGPS.h> 

String dataObject;

int temp;
int hum;
String airStatus;
int airPPM;
String lpgStatus;

/////////////temperature & humidity///////////////////////
#include "DHT.h"
#define DHTPIN 2  // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

////////////////Gas sensor///////////////////
const int gasSensor = 5;
int sensorValue,sensorVolts,RS,R0;
int state = 0;


///////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(GPSBaud); // connect gps sensor
  lcd.begin(16, 2);
  lcd.setCursor(2, 0);
  lcd.print("Air Quality");
  lcd.setCursor(3, 1);
  lcd.print("detector");
  lcd.setCursor(13, 1);
  lcd.print("by");
  delay(2000);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Muhammedsani");
  lcd.setCursor(2, 1);
  lcd.print("Mustafa E.");
  delay(2000);
  pinMode(3,INPUT);
  pinMode(5,OUTPUT);
 
  dht.begin();
}

void loop() {

   while (gpsSerial.available() > 0){
    if (gps.encode(gpsSerial.read())){
      
      if (gps.location.isValid()){ 
      
          latitude = gps.location.lat();
          longitude = gps.location.lng();
      }
 }
   }
  // Wait a few seconds between measurements.
  
  //lcd.setCursor(16, 1);
  delay(300);
  digitalWrite(5,LOW);
  // Reading temperature or humidity takes about 250 milliseconds!
  int h = int(round(dht.readHumidity()));
   //Read temperature as Celsius (the default)
  int t = int(round(dht.readTemperature()));
  
  if (isnan(h) || isnan(t)) {
     Serial.println(F("Failed to read from DHT sensor!"));
     //return;       
  }
  
  //Serial.print(F("Humidity: "));
 // Serial.print(h);
 // Serial.print(F("%  Temperature: "));
  temp = t;
  hum = h;
 // Serial.print(t);
 // Serial.print(F("°C "));
 // Serial.print("");
//////////////END OF TEMP & HUMIDITY CODE////////////////////

  
//MQ6 LPG GAS DETECTION ROUTINE...///////////////////////////
  state = digitalRead(3);
  if (state == LOW){

  //Serial.print(" , LPG detected...");
  lpgStatus = "1";// "LPG detected in air ";
  
  }

  else{

    lpgStatus = "0";// "No LPG detected in air ";
    }

  
/////////////END OF MQ6 CODE/////////////////////////////////////  
  
// MQ135 AIR QUALITY DETECTION...//////////  
  int voltage;
  voltage = analogRead(5);
  int xxx = getVoltage(gasSensor);
  // Serial.print(" volts , ");
  //Serial.print(analogRead(5));
  //Serial.println();
  airPPM = round(voltage);
  
  if (voltage <=350){
    
   // Serial.print(" , MQ135 ---->> ");
    //Serial.print(voltage);
   // Serial.print(" ppm ,");
    //Serial.print("Healthy, Normal level");
    airStatus = "0"; //"Healthy, Normal level";
    //Serial.println();
    delay(300);
    }

  if ((voltage >= 350) && (voltage <=450)){
    
    ///Serial.print(" , MQ135 ---->> ");
   // Serial.print(voltage);
   // Serial.print(" ppm ,");
   // Serial.print("Acceptable level");
    airStatus = "1"; // "Acceptable level";
   // Serial.println();
    delay(300);
    }

  if ((voltage >= 450) && (voltage <=600)){
    
   // Serial.print(" , MQ135 ---->> ");
   // Serial.print(voltage);
   // Serial.print(" ppm ,");
   // Serial.print("Least healthy level (Complaints of stiffness and odors)");
    airStatus = "2"; // "Least healthy level (Complaints of stiffness and odors)";
   // Serial.println();
    delay(300);
    }    


  if ((voltage >= 600) && (voltage <=1000)){
    
    //Serial.print(" , MQ135 ---->> ");
   // Serial.print(voltage);
   // Serial.print(" ppm ,");
   // Serial.print("Poor air quality (general drowsiness)");
    airStatus ="3";//"Poor air quality (general drowsiness)";
  //  Serial.println();
     delay(300);
    }


  if ((voltage >= 1000) && (voltage <=2500)){
    
   // Serial.print(" , MQ135 ---->> ");
   // Serial.print(voltage);
   // Serial.print(" ppm ,");
   // Serial.print("Very poor air quality (Adverse health effects expected) ");
    airStatus ="4";//"Very poor air quality (Adverse health effects expected) ";
   // Serial.println();
      delay(300);
    }

  if ((voltage >= 2500) && (voltage <=5000)){
    
    //Serial.print(" , MQ135 ---->> ");
   // Serial.print(voltage);
   // Serial.print(" ppm ,");
   // Serial.print("Hazardous air quality (average exposure less than 4 hours) ");
    airStatus ="5";  //"Hazardous air quality (average exposure less than 4 hours) ";
   // Serial.println();
    delay(300);
    
    }
///////////////////END OF MQ135 CODE/////////////////////////////////
    

///////GPS sensor part///////////////////////////////////////
  //////////////////gps code///////////////

  
////////////END OF GPS SENSOR PART/////////////////// 

   String dataObject; 
   dataObject += String("+");
    dataObject += String(",");
   dataObject += String(temp);
   dataObject += String(",");
   dataObject += String(hum);
   dataObject += String(",");
   dataObject += String(lpgStatus);
   dataObject += String(",");
   dataObject += String(airPPM);
   dataObject += String(",");
   dataObject += String(airStatus);
   dataObject += String(",");
   dataObject += String(latitude);
   dataObject += String(",");
   dataObject += String(longitude);
   dataObject += String(",");
  
   // Serial.print("Data to be sent: ");
   //Serial.println(dataObject); //DATA TO BE PASSED TO ESP8266//
   // Serial.println();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.setCursor(5, 0);
  lcd.print(temp);
  
  lcd.setCursor(9, 0);
  lcd.print("Hum:");
  lcd.setCursor(13, 0);
  lcd.print(hum);

  lcd.setCursor(0, 1);
  lcd.print("A/Q:");
  lcd.setCursor(4, 1);
  lcd.print(airPPM);
  
  if (lpgStatus.toInt()==0){
    
    lcd.setCursor(7, 1);
    lcd.print(",");
    lcd.setCursor(8, 1);
    lcd.print("No lpg");
    }

  if (lpgStatus.toInt()==1){
    
    lcd.setCursor(7, 1);
    lcd.print(",");
    lcd.setCursor(8, 1);
    lcd.print("lpg seen");
    tone(buzzer,1000,3000);
    digitalWrite(5,HIGH);
    } 

  if ((airPPM >400)&& (lpgStatus.toInt()==1)){
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(airPPM);
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("1:");
    lcd.setCursor(3, 0);
    lcd.print("Dangerous air");
    lcd.setCursor(0, 1);
    lcd.print("2:");
    lcd.setCursor(3, 1);
    lcd.print("Lpg detected");
    tone(buzzer,1000,2000);
    
    }   

   int str_len = dataObject.length();
   //Serial.println(str_len);
   char mystr[str_len];
   dataObject.toCharArray(mystr,str_len);
  //Serial.println(mystr); //Write the serial data
   delay(400);
   Serial.write(mystr,str_len);
   Serial.flush();
   
   
}
///////////////////////////////////////////////// 
int getVoltage(int pin){
 
  return (analogRead(5) * 0.004882814);
  
  }
//////////////////////////////////////////////////
