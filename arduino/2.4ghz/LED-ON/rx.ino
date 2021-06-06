//Author: Ravindra JOB
// SiteWeb: Ravindra-job.com
// LastUpdate : 14/12/2016


#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int LED1 = 5;

void setup(void){
 Serial.begin(9600);
 radio.begin();
 radio.openReadingPipe(1,pipe);
 radio.startListening();
 pinMode(LED1, OUTPUT);}

void loop(void){
 if (radio.available()){
 bool done = false; 
 while (!done){
 done = radio.read(msg, 1); 
 Serial.println(msg[0]);
 if (msg[0] == 111){ 
 digitalWrite(LED1, HIGH);
 delay(100);
 }
 else {digitalWrite(LED1, LOW);}
 delay(100);
 }
 }
 else{Serial.println("No radio available");
 digitalWrite(LED1, LOW);
 delay(1000);
 }
}