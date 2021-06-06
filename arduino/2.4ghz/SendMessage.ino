//Author: Ravindra JOB
// SiteWeb: Ravindra-job.com
// LastUpdate : 12/02/2015

#include <Wire.h>
#define SLAVE_ADDRESS 0x12
int dataReceived = 0;


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;


void setup() {
              Serial.begin(9600);
              Wire.begin(SLAVE_ADDRESS);
              Wire.onReceive(receiveData);
              Wire.onRequest(sendData);
              
              
                           radio.begin();
                           radio.openWritingPipe(pipe);
                            String theMessage = "Start";
                            
                          
            }


void loop() {
delay(100);



if (dataReceived == 101)
{
        String theMessage = "RCSB1";
        int messageSize = theMessage.length();
        for (int i = 0; i < messageSize; i++) 
        {
        int charToSend[1];
        charToSend[0] = theMessage.charAt(i);
        radio.write(charToSend,1);
        Serial.print("jenvoi 101 : ");                                      
        }  
    }
    

if (dataReceived == 102){

        String theMessage = "RCSB2";
        int messageSize = theMessage.length();
        for (int i = 0; i < messageSize; i++) 
        {
            int charToSend[1];
            charToSend[0] = theMessage.charAt(i);
            radio.write(charToSend,1);
        }  
    }    



if (dataReceived == 103){

        String theMessage = "RCSB3";
        int messageSize = theMessage.length();
        for (int i = 0; i < messageSize; i++) 
        {
            int charToSend[1];
            charToSend[0] = theMessage.charAt(i);
            radio.write(charToSend,1);
        }  
    }                                          
    
    
if (dataReceived == 104){

        String theMessage = "RCSB4";
        int messageSize = theMessage.length();
        for (int i = 0; i < messageSize; i++) 
        {
            int charToSend[1];
            charToSend[0] = theMessage.charAt(i);
            radio.write(charToSend,1);
        }  
    }                                     


    



if (dataReceived == 105){

        String theMessage = "RCSB5";
        int messageSize = theMessage.length();
        for (int i = 0; i < messageSize; i++) 
        {
            int charToSend[1];
            charToSend[0] = theMessage.charAt(i);
            radio.write(charToSend,1);
        }  
    }    



if (dataReceived == 106){

        String theMessage = "RCSB6";
        int messageSize = theMessage.length();
        for (int i = 0; i < messageSize; i++) 
        {
        int charToSend[1];
        charToSend[0] = theMessage.charAt(i);
        radio.write(charToSend,1);
       }  
    }                                          
    
    
if (dataReceived == 107){

        String theMessage = "RCSB7";
        int messageSize = theMessage.length();
        for (int i = 0; i < messageSize; i++) 
        {
            int charToSend[1];
            charToSend[0] = theMessage.charAt(i);
            radio.write(charToSend,1);
        }  
    }      
    
if (dataReceived == 108){

        String theMessage = "RCSB8";
        int messageSize = theMessage.length();
        for (int i = 0; i < messageSize; i++) 
        {
            int charToSend[1];
            charToSend[0] = theMessage.charAt(i);
            radio.write(charToSend,1);
       }  
    }                                                         

    
//send the 'terminate string' value...  
msg[0] = 2; 
radio.write(msg,1);                             
radio.powerDown(); 
delay(1000);
radio.powerUp();

}





           

void receiveData(int byteCount){
                                dataReceived = Wire.read();                        
                                      Serial.print("Donnee recue : ");
                                      Serial.println(dataReceived);
                                      
}



void sendData(){
    int envoi = dataReceived + 1;
    Wire.write(envoi);
}