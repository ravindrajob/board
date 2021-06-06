//Author: Ravindra JOB
// SiteWeb: Ravindra-job.com
// LastUpdate : 22/04/2015

#include <Wire.h>

#define SLAVE_ADDRESS 0x12
int dataReceived = 0;

void setup() {
    Serial.begin(9600);
    Wire.begin(SLAVE_ADDRESS);
    Wire.onReceive(receiveData);
pinMode(8, OUTPUT);    // Set the switch pin as input    
}

void loop() {
    delay(100);
}

void receiveData(int byteCount){
    
  while(Wire.available()) {
        dataReceived = Wire.read();                        
        Serial.print("Donnee recue : ");
        Serial.println(dataReceived);
        
        if (dataReceived == 1){
        digitalWrite(8,HIGH); 
        Serial.print("LED ALLUME ");
    }
}}