#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <BH1750.h>
#include <PubSubClient.h>

#include <iostream>
#include <string>

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266WiFiMulti.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <WiFiMulti.h>
#endif

Adafruit_INA219 ina219(0x40);
Adafruit_INA219 ina219_B(0x41);
BH1750 lightMeter(0x24);

#if defined(ESP8266)
ESP8266WiFiMulti WiFiMulti;
#elif defined(ESP32)
WiFiMulti WiFiMulti;
#endif


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


const char* mqttServer = "192.168.1.30";
const int mqttPort = 1883;
const char* mqttUser = "MQTTuser";
const char* mqttPassword = "PasswordMQTTX";
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
        Serial.begin(115200);
        WiFiMulti.addAP("ravindrajob-SSID ", "WPAKEY");
        while (WiFiMulti.run() != WL_CONNECTED) {
                delay(100);
        }
        Serial.println("Ready");       
        Serial.println("WiFi connected");
        Serial.println("IP address: "); Serial.println(WiFi.localIP());        
      
        ina219.begin();
        ina219_B.begin();
        lightMeter.begin();

 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.publish("wemos/test", "hello"); //Topic name        

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }



  
}


 String testString = "";                               //String for complete message
const float bigNumber = 0xFFFFFFFFFF;                 //Very large number (trillions!) 
//********************************************  
// ----- DEFINE/DECLARE GLOBALS -----
  char leftBrace =     0x7B;     //ASCII code for {
  char rightBrace =    0x7D;     //ASCII code for }
  char qMark =         0x3F;     //ASCII code for ?
  char plusSign =      0x2B;     //ASCII code for +
  char minusSign =     0x2D;     //ASCII code for -
  char charBelowZero = 0x2F;     //ASCII for "0" minus 1 (0x30 - 1 = 0x2F)
  char charAboveNine = 0x3A;     //ASCII for "9" plus 1 (0x39 + 1 = 0x3A)
  char quoteMark =     0x22;     //ASCII for "
  char commaMark =     0x2C;     //ASCII for comma
  
//Declare global arrays to save message names, text, and numeric info
  #define arraySize 5                 //Change this value as needed
  String JName[arraySize];            //Name, always starts with text
  String JValue[arraySize];           //Message text, if message starts with a character,
                                      //  otherwise, empty.
  float  JNumber[arraySize];          //Number, if message starts with 0-9, +, or -,
                                      //  otherwise bigNumber (0xFFFFFFFFFF).
//********************************************


void callback(char* topic, byte* payload, unsigned int length) {
 
String testString = "";                             //Initialize an empty string
  Serial.print("\n\rMessage arrived in topic: ");     //Message arrived, so print topic
  Serial.println(topic);
  Serial.print("Payload: ");                          //Print "Payload: "
  Serial.println((char*)payload);
  Serial.println("-----------------------");
  
  //Known-good String for testing if you need it
  //String testString = "{\"SensorType\":\"Temperature\",\"Value\":10,\"Testing\":\"ON\",\"Another value\":987.437}";

  int returnCode = JSON_Parse((char*)payload);        //Parse the TestString and print values
  switch (returnCode)                                 //Test return value for any error codes
   {
    case 97:
      Serial.println("Error in Value code.");
      break;
    case 98:
      Serial.println("No colon found.");
      break;
    case 99:
      Serial.println("Invalid message.");  
   }      
   if (returnCode < 90)                           //No error codes, test returned number of message pairs
   {                                              //Less than 95 (change as needed)
    Serial.println("Parsing OK");                 //Print message and print pairs of values
    Serial.println("-----------------------------");
    for (int arrayCounter = 0; arrayCounter < returnCode; arrayCounter++)
      {
        //After you have values, change the Serial.print statements to whatever you want for a name-value pair.
        //The following statements display the results of JSON parsing.
        Serial.print("Name-Value pair#: ");                    //Print the message Name                  
        Serial.println(arrayCounter);
        Serial.print("JName: ");
        Serial.println(JName[arrayCounter]);
        if (JValue[arrayCounter] != "")                 //If value holds something               
        {
          Serial.print("JValue: ");                     //Print the value
          Serial.println(JValue[arrayCounter]);
        }
        if (JNumber[arrayCounter] != bigNumber)         //If the number not equal to 0xFFFFFFFFFF
        {
          Serial.print("JNumber: ");                    //Print the number
          Serial.println(JNumber[arrayCounter]);
        }
        Serial.println("-----------------------------");
        //Serial.println(" ");
      }
   }
}

char *to_string(float num)
{
    char *a = (char*)malloc(50 * sizeof(char));
    sprintf(a, "%f", num);
    return a;
}
  
void loop() {
  scani2c();

  float busvoltage = 0;
  float current_mA = 0;
  float busvoltageb = 0;
  float current_mAb = 0;
  uint16_t lux = 0;

  float energy_Wh=0;
  float energy_Whb=0;

  float power_mW = 0;
  float power_mWb = 0;

  long time_s=0;

  
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  busvoltageb = ina219_B.getBusVoltage_V();
  current_mAb = ina219_B.getCurrent_mA();
  lux = lightMeter.readLightLevel();
  
time_s=millis()/(1000); // convert time to sec

  power_mW=current_mA*busvoltage; 
  energy_Wh=(power_mW*time_s)/3600;   //energy in watt hour
  
  power_mWb=current_mAb*busvoltageb; 
  energy_Whb=(power_mWb*time_s)/3600;   //energy in watt hour
 
  Serial.print(busvoltage);
  Serial.print(" V / ");
  Serial.print(current_mA);
  Serial.print(" mA - ina219_B ");
  Serial.print(busvoltageb);
  Serial.print(" V / ");
  Serial.print(current_mAb);
  Serial.print(" mA - Lux ");
  Serial.println(lux);

//char bufferLux[50];
//sprintf(bufferLux, "%f", lux);

//lux
client.publish("wemos/solar1/lux", to_string(lux));

//ina 1
client.publish("wemos/solar1/current_mA", to_string(current_mA));
client.publish("wemos/solar1/busvoltage", to_string(busvoltage));
client.publish("wemos/solar1/energy_Wh", to_string(energy_Whb));
client.publish("wemos/solar1/power_mW", to_string(power_mWb));

//ina 2
client.publish("wemos/solar1/current_mAb", to_string(current_mAb));
client.publish("wemos/solar1/busvoltageb", to_string(busvoltageb));
client.publish("wemos/solar1/energy_Whb", to_string(energy_Whb));
client.publish("wemos/solar1/power_mWb", to_string(power_mWb));


  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  
  // Display static SolarPanel info
  display.print("Solar");
  display.println(busvoltage);
  
  display.print("Bat");
  display.println(busvoltageb);
  
  display.print("LUX: ");
  display.println(lux);

  
  display.display(); 
  client.loop();
  delay(5000);
}

void scani2c()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");



  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static SolarPanel info
  display.print("Scan..");
  display.display(); 

  
  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknow error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}

//******************************************** 
// ----- JSON PARSING FUNCTION -----
//For reference on Arduino C++ String objects: https://www.arduino.cc/reference/en/language/variables/data-types/stringobject/.

int JSON_Parse(String Jstring)  //Enter this function with a string object
{
  // ----- LOCAL VARIABLES -----
  int Jlength;                  //Length of entire JSON string
  int quotestart;               //pos'n of first quote in a name or value
  int quoteend;                 //pos'n of last quote in a name or value
  int numberEnd;                //String pos'n of comma after numeric value
  int index;                    //Loop counter
  int dataIndex;                //One less than number of NAME/VALUE pairs 
  char signTest;                //Result from test of 1st char in a numeric value
  
  Jlength = (Jstring.length());                     //Get length of string; printing character. Note: \" = one char
  int numbOfCommas = 0;                             //Reset number of commas from previous JSON object
  for (index = 0; index < Jlength; index++)         //Loop to count commas; # of commas is one less than
  {                                                 //  number of message/value pairs
     if ((char)Jstring.charAt(index) == 0x2C)       //Find a comma in the JSON payload
     {
        numbOfCommas += 1;                          //Accumulate number of commas
     }
  }
  if(((char)Jstring.charAt(0) != leftBrace) || (char)Jstring.charAt(Jlength - 1) != rightBrace)    //Test for "{" and "}" in JSON object
  {                                                                                                // at start and end.
    goto error99;                                                   //Missing bracket triggers error.
  }
  for (dataIndex = 0; dataIndex < (numbOfCommas+1); dataIndex++)    //Main message-decoding steps in this section
  {                                                                 //Get JSON message's NAME between quotes
    quotestart = Jstring.indexOf("\"");                             //Find quote at start of section & save location
    Jstring.setCharAt(quotestart, qMark);                           //Replace quote mark with a ?-mark, just a place holder
    quoteend = Jstring.indexOf("\"");                               //Find quote at end of section & save location
    Jstring.setCharAt(quoteend, qMark);                             //Replace quote mark with a ?-mark, just a place holder
    JName[dataIndex]= Jstring.substring(quotestart+1, quoteend);    //Start at first char after first quote and go to last quote
    
    //Test for a colon that separates NAME and VALUE information
    if((char)Jstring.charAt(quoteend+1) != 0x3A)                //Look at character after the last quote in text.  Is is colon? ":" 
      {
      goto error98;                                             //No colon found, error message
      }
    Jstring.setCharAt(quoteend + 1, qMark);                     //Yes, colon found. Replace colon with a ? mark, just placeholder.
    if ((char)Jstring.charAt(quoteend + 2) == quoteMark)        //Is next char a quote mark?
    {                                                           //Yes, it is a quote mark, continue...
        quotestart = Jstring.indexOf("\"");                     //Find first quote in VALUE message 
        Jstring.setCharAt(quotestart, qMark);                   //Replace quote mark with a ?-mark as a placeholder.
        quoteend = Jstring.indexOf("\"");                       //Find location of ending quote mark                               
        Jstring.setCharAt(quoteend, qMark);                     //Replace quote mark with a ?-mark as a placeholder.
        if((char)Jstring.charAt(quoteend + 1) == commaMark)     //If there is a comma after the quote mark, replace it with a ? mark.
          {
            Jstring.setCharAt(quoteend + 1, qMark);
          }           
        JValue[dataIndex] = Jstring.substring(quotestart+1, quoteend);        //Start after first quote and get string up to last quote.
        JNumber[dataIndex] = bigNumber;                                       //Put bigNumber here as signal to NOT use it.
    }
    else                                                                      //No quote after the colon, so we have a numeric value
    {
        signTest = Jstring.charAt(quoteend + 2);                              //Get first character after the colon.  What is it?       
                                                                              //Is 1st char == +, -, or a number..?
        if((signTest == plusSign) || (signTest == minusSign) || ((signTest > charBelowZero) && (signTest < charAboveNine)))
          {                                                                   //Yes
            if (Jstring.indexOf(",") != -1)                                   //Value can end with a comma or a rightBrace
              {                                                               //Test for comma
                numberEnd = Jstring.indexOf(",");                             //If a comma at the end of numeric value save its index
                Jstring.setCharAt(numberEnd, qMark);                          //Replace comma with "?" as a place holder
              }
              else if ((Jstring.indexOf("}")) != -1)                          //Test for right brace at end of value
              {
                //Serial.println("A brace ends it");                          //For testing
                numberEnd = Jstring.indexOf("}");                             //if number ends with a }, save its string index 
              }       
        JValue[dataIndex] = Jstring.substring(quoteend+2, numberEnd);         //Get numeric string, incl sign and decimal point
        JNumber[dataIndex] = JValue[dataIndex].toFloat();                     //Convert value in string to float
        JValue[dataIndex] = "";                                               //Clear anything in the text VALUE string.
      }
      else
      {
        goto error97;
      }
    }
  }
  return(numbOfCommas + 1);      //Everything parsed properly, return number of message pairs
  
 // Error codes.
  error99:
    return(99); 
  error98:
    return(98); 
  error97:
    return(97);
}
//********************************************
// ----- END JSON PARSING FUNCTION -----
//----- END -----
