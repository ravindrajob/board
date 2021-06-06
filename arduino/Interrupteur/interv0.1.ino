//Author: Ravindra JOB
// SiteWeb: Ravindra-job.com
// LastUpdate : 24/06/2014

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int LED1 = 5;

 
const int interrupteur = 2;        
int InterStatus = 0; 


int MecaStatus = 0;
int RFStatus = 0;

void setup() 
    {
      pinMode(interrupteur, INPUT);    
      Serial.begin(9600);
      radio.begin();
      radio.openReadingPipe(1,pipe);
      radio.startListening();
      pinMode(LED1, OUTPUT);
    }


void loop(){
  
//---------------------------------------------------------------------------------------------------------------
//-----------------------------------------------2.4 GHZ SYSTEM------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
  
  
if (radio.available()){
bool done = false; 
while (!done)
 {
                done = radio.read(msg, 1); 
            
                
    if (msg[0] == 211)
    { 
    digitalWrite(LED1, HIGH);
    Serial.println("LED ON by 2.4 GHZ");
    RFStatus = 1;
    
    }
                                    
    if (msg[0] == 210)
    { 
    digitalWrite(LED1, LOW);
    Serial.println("LED OFF by 2.4 GHZ");
    RFStatus = 0;
    }                                       
                 
 }
}
                                           
                                           
                   
                   
//---------------------------------------------------------------------------------------------------------------
//-----------------------------------------------MECANIQUE SYSTEM------------------------------------------------
//---------------------------------------------------------------------------------------------------------------


InterStatus = digitalRead(interrupteur);



if ((InterStatus == HIGH)&&(MecaStatus == 0))  
  {  

    Serial.println("Interrupteur a bouger en position ON");
    if (RFStatus == 1)
    { 
      digitalWrite(LED1, LOW);
      Serial.println("LED OFF by Mecanique system avec RFStatus 1 ");
    }
                    
    else           
    { 
      digitalWrite(LED1, HIGH);
      Serial.println("LED ON by Mecanique system avec RFStatus 0");
    }                 
                          
      MecaStatus = 1;
  } 

if ((InterStatus == LOW)&&(MecaStatus == 1)) 
 {    
  Serial.println("Interrupteur a bouger en position OFF"); 
  if (RFStatus != 1)
    { 
    digitalWrite(LED1, LOW);
    Serial.println("LED OFF by Mecanique system avec RFStatus 1 ");
    }
                    
 else             
    { 
    digitalWrite(LED1, HIGH);
    Serial.println("LED ON by Mecanique system avec RFStatus 0");
    }
  MecaStatus = 0;
 } 
                                                                            
            
      }