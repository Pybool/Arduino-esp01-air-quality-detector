// Import required libraries
#include "ESPAsyncWebServer.h"
#include <ESP8266WiFi.h>
#include <FS.h>
#include <ESPAsyncTCP.h>
#include <WiFiClient.h>
#include <Pushsafer.h>
#include <ESP8266HTTPClient.h>
float latitude, longitude ;
//#include "index.h"
String  Array[10];
String sensorData ;
String Message;
char mystr[50];
int i = 0;
int j = 0;
// Replace with your network credentials
const char* ssid = "Genisys-iPhone";
const char* password = "bvn10111010";

// Pushsafer private or alias key
#define PushsaferKey "frePWhOd2h9WXzZoz7Ck"
int lpgMsgFlag = 0;
int airMsgFlag = 0;
/*WiFiClientSecure client;*/

//////////Geolocation parsing data/////////////////////

const char* address_village ;
const char* address_county ;
const char* address_state ;

//////////////////////////////////////////////////////
WiFiClient client;
Pushsafer pushsafer(PushsaferKey, client);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80); //

const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>

    <meta charset = "utf-8" >
   
    <style type="text/css">

      b{

        font-size: 150%;
      }

      li{
        color: orange;
      }

      h1,h4{

        background: #FFB76B;
        background: -webkit-linear-gradient(to top left, #FFB76B 10%, #FFA73D 43%, #FF7C00 52%, #FF7F04 100%);
        background: -moz-linear-gradient(to top left, #FFB76B 10%, #FFA73D 43%, #FF7C00 52%, #FF7F04 100%);
        background: linear-gradient(to top left, #FFB76B 10%, #FFA73D 43%, #FF7C00 52%, #FF7F04 100%);
        -webkit-background-clip: text;
        -webkit-text-fill-color: transparent;
      }

    </style>

    <title>Air Quality </title>
  </head>
    <body onload="initialize()"  style="background-image: radial-gradient(circle, #000000, #3b3839, #766f70, #b8adad, #feeeee);" >
      

      <div background-image: radial-gradient(circle, #000000, #3b3839, #766f70, #b8adad, #feeeee); border:2px solid black;margin:20px;padding:20px; color:lavender;  >
        <h1 style= "font-size:200%; padding-left: 10px;"> Arduino Based Air Quality detector...</h1><br>
  
          <ul>
            <li>
              <p>
                <div> 
                <p>
                    <label style=color:lavender; value = temperature><b>TEMPERATURE:</b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</label>                  
                     <b><label id="temp_val"  font-size: 100%;  style=color:lavender; >&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</label></b>
                     <b><label  font-size: 100%; style=color:lavender;> °C &#10052</label></b>
                </p>     
                </div>

                <div>
                    <label style=color:lavender; value = humidity><b>HUMIDITY:</b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</label>
                    <b><label id="hum_val" font-size: 100%;  style=color:lavender; >&nbsp;&nbsp;&nbsp;&nbsp;</b>
                    <b><label  font-size: 100%; style=color:lavender;>%</label></b>
                </div>
                <hr id ="rule" style=height:0px;background-color:#FFFFF;>

              </p>

            </li>
            <br>

            <li>
              <p>
                <label style=color:lavender; font-family:lucida calligraphy;font-size:130%; name=MQ135 value = MQ135><b>Air Quality(MQ135):</b>

                <label id="fix3" font-size: 100%; style=color:#FFFFF;>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b id= "ppm" > ppm ,</b>
                <label id="fix4" font-size: 100%; style=color:#FFFFF;> <b id ="airqual"> </b><hr id ="rule" style=height:0px;background-color: grey;border-width: 0;>
              </p>
            </li>
            <br>

            <li>
              <p>
                <label style=color:lavender; font-family:lucida calligraphy;font-size:130%; ><b>LPG(MQ6):&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b>
                <b><label id="lpg"  font-size: 100%;></label></b>
                <hr id ="rule" style=height:0px;background-color: grey;border-width: 0;>
              </p>
            </li>
            <br>

            <li>

               <p> 
                <label style=color:lavender; font-family:lucida calligraphy;font-size:150%;><b>LOCATION:</b>&nbsp;

      
                <label  style=color:#FFFFF;> <b>Lat:&nbsp;</b>
                <b><label id="lat"  font-size: 100%;></label></b><label  style=color:#FFFFF;> <b>°&nbsp;</b>
                <label  style=color:#FFFFF;> <b>Lon:&nbsp;</b> 
                <b><label id="lon" font-size: 100%;> °</label></b><label  style=color:#FFFFF;> <b>°&nbsp;</b>

                <b><label id="village" font-size: 100%; style=color:#FFFFF;>N/A&nbsp;</label></b><label  style=color:#FFFFF;> <b>,&nbsp;</b>  
                <b><label id="county" font-size: 100%; style=color:#FFFFF;>N/A&nbsp;</label></b><label  style=color:#FFFFF;> <b>,&nbsp;</b>
                <b><label id="state" font-size: 100%; style=color:#FFFFF;>N/A&nbsp;</label></b><label  style=color:#FFFFF;> <b>&nbsp;</b> 
              
              </p>
              <br>

            </li>
          </ul>
          <h4 align="left" ><b>Project by : Muhammedsani Mustafa E.</b></h4>
          
      </div>
      
      <script>
         
        function getData() {
          var xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
              var this_responseText =this.responseText;
              var responseText =this_responseText;
              console.log(responseText);
              var data = responseText.split(",");
              console.log(data);
              console.log(data.length);

              if (data.length ==11) {
            
              document.getElementById("temp_val").innerHTML = data[1];
              document.getElementById("hum_val").innerHTML =  data[2];
              document.getElementById("ppm").innerHTML =      data[4] + '' + ' ppm ,';
              var lpgStatus;
              switch(parseInt(data[3])){

                case 0:
                  lpgStatus = "No LPG detected &#127807  &#128513 ";
                  document.getElementById("lpg").innerHTML =  lpgStatus;
                  break;

                case 1:
                  lpgStatus = "LPG detected in Air &#129314 &#127810";
                  document.getElementById("lpg").innerHTML =  lpgStatus;
                  break;   
                
                }

              var airQuality;
              switch(parseInt(data[5])){

                case 0:
                  airQuality = "Healthy, Normal level &#127807  &#128513";
                  document.getElementById("airqual").innerHTML =  airQuality;
                  break;

                case 1:
                  airQuality = "Acceptable level &#127807  &#128513";
                  document.getElementById("airqual").innerHTML =  airQuality;
                  break;   

                case 2:
                  airQuality = "Least healthy level &#128567 (Complaints of stiffness and odors)";
                  document.getElementById("airqual").innerHTML =  airQuality;
                  break;

                case 3:
                  airQuality = "Poor air quality (general drowsiness &#129314 &#127810)";
                  document.getElementById("airqual").innerHTML =  airQuality;
                  break;

                case 4:
                  airQuality = "Very poor air quality (Adverse health effects expected &#129314 &#127810) ";
                  document.getElementById("airqual").innerHTML =  airQuality;
                  break;

                case 5:
                  airQuality = "Hazardous air quality (average exposure less than 4 hours) &#9760 ";
                  document.getElementById("airqual").innerHTML =  airQuality;
                  break;
                
                }  
              document.getElementById("lat").innerHTML = data[6];
              document.getElementById("lon").innerHTML =  data[7];
              document.getElementById("village").innerHTML = data[8];
              document.getElementById("county").innerHTML =  data[9];
              document.getElementById("state").innerHTML = data[10];
              }
            }
          };
          xhttp.open("GET", "temp" , true);

          xhttp.send();
        }
        setInterval(function() 
        {
          getData();
        }, 2000);

        </script>

    </body>

</html>

)=====";
  
void setup(void){
  
  Serial.begin(9600);
  // Begin the Serial at 9600 Baud
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  // WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("....");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(10000);
    WiFi.begin(ssid, password);
    Serial.println(".");

  }
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  pushsafer.debug = true;

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "text/html",webpage); 
  });

  server.on("/temp", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "text/html",sensorData);
  });


// Start server  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>32°C &#10052</b>
  server.begin();

}


void loop() {
  HTTPClient http;
  char buffer[51];
   
  if (Serial.available()>0){
    
   Serial.readBytes(buffer, 51); //Read the serial data and store in var

   String geoloc;
   String comma = String(",");
   String latitude = String(latitude);
   String longitude = String(longitude);
   sensorData = String(buffer);//String(mystr);
   //Serial.println(sensorData); 
   char *token = strtok(buffer,",");
  
   for(int i; token !=NULL; i++){   

    //Serial.println(token);
    Array[i]=token;
    //Serial.print(" ");
    //Serial.print(Array[i]);
    token = strtok(NULL,",");
    
    }
     latitude = Array[6];
     longitude = Array[7];
     String lonn = "&lon=";
     String format = "&format=json";
     String link = "http://us1.locationiq.com/v1/reverse.php?key=pk.a9a12cd8c887f67ad747eaab2a9879cc&lat=";
     String linkk = link + latitude +lonn + longitude + format;
     
    http.begin(linkk);
    int httpCode = http.GET();
    String payload = http.getString();
    //Serial.println(httpCode);
    if (httpCode > 0){
          
      String payload = http.getString();
      //Serial.print(payload);
      StaticJsonDocument<1024> doc;
      DeserializationError error = deserializeJson(doc,payload);
      
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
  
      JsonObject address = doc["address"];
       //Serial.println(address);
       address_village = address["village"]; // "2nd Efosa Lane"
       address_county = address["county"]; // "Benin City"
       address_state = address["state"]; // "Edo"  
       geoloc += comma;
       geoloc += address_village;
       geoloc += comma;
       geoloc += address_county;
       geoloc += comma;
       geoloc += address_state;
       sensorData += geoloc;
       //Serial.println(sensorData);
  
      }
  
    http.end();    
  }

  if (lpgMsgFlag == 0){
   
    if  (Array[3].toInt()==1){       
    struct PushSaferInput input;
    Message = "LPG detected!!";
    Serial.println("Lpg detected");
    input.message = Message;
    input.title = "Air Quality detector: ";
    input.sound = "8";
    input.vibration = "1";
    input.icon = "1";
    input.iconcolor = "#FFCCCC";
    input.priority = "1";
    input.device = "a";
    input.url = "https://www.pushsafer.com";
    input.urlTitle = "Open Pushsafer.com";
    input.picture = "";
    input.picture2 = "";
    input.picture3 = "";
    input.time2live = "";
    input.retry = "";
    input.expire = "";
    input.answer = "";
    int i=0;
    for (i=0;i<3;i++){
      if (i<=1){
      Serial.println(pushsafer.sendEvent(input));
      lpgMsgFlag =1;
      }
      if(i==2){lpgMsgFlag=0;}
    }
    //Serial.println("Sent a push notification");    
    
       } 
    
    } 
  
    if (airMsgFlag == 0){

        if (Array[5].toInt() >= 450){
        struct PushSaferInput input;
        Message = "Unhealthy Air Quality detected!!";
        Serial.println("Unhealthy Air Quality detected!!");
        input.message = Message;
        input.title = "Air Quality detector: ";
        input.sound = "8";
        input.vibration = "1";
        input.icon = "1";
        input.iconcolor = "#FFCCCC";
        input.priority = "1";
        input.device = "a";
        input.url = "https://www.pushsafer.com";
        input.urlTitle = "Open Pushsafer.com";
        input.picture = "";
        input.picture2 = "";
        input.picture3 = "";
        input.time2live = "";
        input.retry = "";
        input.expire = "";
        input.answer = "";
        int i=0;
        for (i=0;i<3;i++){
          if (i<=1){
          Serial.println(pushsafer.sendEvent(input));
          airMsgFlag =1;
          }
          if(i==2){airMsgFlag=0;}
        } 
        //Serial.println("Sent a push notification");
       
        }  
      
      }

   }
