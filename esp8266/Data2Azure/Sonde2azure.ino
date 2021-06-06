//Author: Ravindra JOB
// SiteWeb: Ravindra-job.com
// LastUpdate : 17/08/2017

#include "DHT.h"
#define DHTPIN 2//12    // what digital pin we're connected to  pin2 to D4 on esp board
#define DHTTYPE DHT11  

DHT dht(DHTPIN,DHTTYPE);

#include <ESP8266WiFi.h>
const char* ssid  = "HotSpot-wifi";
const char* password = "XXXXXXXXX";

const char* host = "prd-srv01-ravindra-job.azurewebsites.net";
//const char* passcode = "YOUR_PASSCODE";
int led = 12; // si la commutation ne se fait pas, utiliser une resistance entre le pin D6 (ARDUINO PIN12) et le pin IN du relay
int sensor = 13;
int lastTempState = 0;
int lastHumState = 0;
int lastPirState = 0;

String location = "SALON";
String relay = "SLedPanel1";
 
void setup() {
                Serial.begin(115200);
               
                dht.begin();
                pinMode(sensor, INPUT); // declare sensor as input
                pinMode(led, OUTPUT);  // declare LED as output
                Serial.println();
                Serial.print("Connecting to ");
                Serial.println(ssid);
                WiFi.begin(ssid, password);
                while (WiFi.status() != WL_CONNECTED) 
                {
                    delay(500);
                    Serial.print(".");
                }
                                    
                Serial.println("");
                Serial.println("WiFi connected");
                Serial.print("IP address: ");
                Serial.print(WiFi.localIP());
        // Connect to host
                Serial.print("Connecting to ");
                Serial.println(host);                        
                digitalWrite(led,LOW);
                        
              }

void loop(){

// Use WiFiClient class to create TCP connections
 WiFiClient client;
const int httpPort = 80;
if (!client.connect(host, httpPort)) 
    {
    Serial.println("Connection failed!");
    return;
    }

////////////////////////////////////RECUPERATION E LA MAC DU ESP8266////////////////////////   

String mac = WiFi.macAddress();
                 
////////////////////////////////////////////URL PREPARATION/////////////////////////////////  
 
mac.replace(":","");
String api = "http://prd-srv01-ravindra-job.azurewebsites.net/karaapi.php?MAC=" + mac;
api = api + "&LOCATION=" + location;

  
                          
////////////////////////////////////////////GET MOTION//////////////////////////////////////      
long state = digitalRead(sensor);
  
    if(state == HIGH)
        {
        Serial.println("Motion detected!");
        api = api + "&MOTION=1";
        // On enregistre qu'on a envoyé 1
        lastPirState = 1;
        }
    else 
      {
        if(lastPirState == 1) 
        {
        Serial.println("Motion absent!");
        api = api + "&MOTION=0";
        
        // On enregistre qu'on a déjà envoyé 0, pas la peine de le renvoyer tant qu'il y a pas de mouvement
        lastPirState = 0;
        }
        else 
        {
            Serial.println("On a deja envoye 0 donc on ne renvoit pas");
        }
       }
          

////////////////////////////////////////////GET TEMPERATURE////////////////////////////////////////////

//On recupère la temperature en la stockant dans une variable
int Humidity = dht.readHumidity();
int Temperature_Cel = dht.readTemperature();

// On affiche la temperature
Serial.println(dht.readTemperature());

//Si la TEMP ou HUM à changé par rapport ç l'autre cycle on renvoi la nouvelle temperature
if (Temperature_Cel != lastTempState || Humidity != lastHumState) 
    { 
    Serial.println("Nouvelle temperature detecte")  ;             
    api = api +  "&HUM=" + (String) Humidity +  "&TEMP="  +(String) Temperature_Cel;
    }

// On enregistre la temperature et l'humidité pour ne pas envoyer les mêmes données      
lastTempState = Temperature_Cel;
lastHumState = Humidity;

////////////////////////////////////////////Switch STATE////////////////////////////////////////////

 //Ajout dans l'url la demande de l'etat du switch
 api = api + "&SW=" + relay;

/////////////////////////////////////////////SEND API REST///////////////////////////////////////////


// On verifie que l'url dispose des DATA à envoyé de type MOTION ou HUM ou TEMP, histoire de ne pas envoyer des requetes pour rien
//Pour cela on va utiliser "index.Of". Celui si permet déterminer  la position du mot recherché, et retourne "-1" si non trouvée
if (api.indexOf("HUM") != -1 || api.indexOf("MOTION") != -1) 
        {
        Serial.println(api);
        client.print(String("GET ") + api + " HTTP/1.1\r\n" +
        
        "Host: " + host + "\r\n" +
        "Connection: close\r\n\r\n");
        unsigned long timeout = millis(); 
        while (client.available() == 0) 
            {
            if (millis() - timeout > 5000)
                {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
            }
            } 
                            
        }
 //////////////////////////////// Analyse du resultat switch (relay) ////////////////////////////////                                                                                                                     
 while(client.available()) 
   {
  
    // Lecture du resultat retourné par L'API REST
    String line = client.readStringUntil('\n');
    
    if(line == "0<br>")
        {
        Serial.println("0 : On eteint la lumiere");
        digitalWrite (led, HIGH);
        }
                        
    if(line == "1<br>") 
        {
        Serial.println("1 : On allume la lumiere");
        digitalWrite (led, LOW);
        }
    }                                    
                          
    delay(200);
     
}              
