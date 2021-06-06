#include <VirtualWire.h> // inclusion de la librairie VirtualWire

int switchPin = 2;   // switch is connected to pin 2
int val;             // variable for reading the pin status
int buttonState;     // variable to hold the last button state

int raspPIN3 = 3;    // switch is connected to pin 2
int ReadPIN3;        // variable for reading the pin status
int lastReadPIN3;    // variable to hold the last button state

int raspPIN4 = 4;     // switch is connected to pin 2
int ReadPIN4;        // variable for reading the pin status
int lastReadPIN4;    // variable to hold the last button state

int raspPIN5 = 5;    // switch is connected to pin 2
int ReadPIN5;        // variable for reading the pin status
int lastReadPIN5;    // variable to hold the last button state

int raspPIN6 = 6;    // switch is connected to pin 2
int ReadPIN6;        // variable for reading the pin status
int lastReadPIN6;    // variable to hold the last button state

int raspPIN7 = 7;    // switch is connected to pin 2
int ReadPIN7;        // variable for reading the pin status
int lastReadPIN7;    // variable to hold the last button state


void setup() {
  
pinMode(raspPIN3, INPUT);    // Set the switch pin as input
pinMode(raspPIN4, INPUT);    // Set the switch pin as input
pinMode(raspPIN5, INPUT);    // Set the switch pin as input
pinMode(raspPIN6, INPUT);    // Set the switch pin as input
pinMode(raspPIN7, INPUT);    // Set the switch pin as input
pinMode(8, OUTPUT);    // Set the switch pin as input

  Serial.begin(9600);// Set up serial communication at 9600bps
  
  buttonState = digitalRead(switchPin);   // read the initial state
  lastReadPIN3 = digitalRead(raspPIN3);   // read the initial state
  lastReadPIN4 = digitalRead(raspPIN4);   // read the initial state
  lastReadPIN5 = digitalRead(raspPIN5);   // read the initial state
  lastReadPIN6 = digitalRead(raspPIN6);   // read the initial state
  lastReadPIN7 = digitalRead(raspPIN7);   // read the initial state
  
  vw_setup(2000);     // initialisation de la librairie VirtualWire à 2000 bauds (note: je n'utilise pas la broche PTT)
}


void loop()
{
  val = digitalRead(switchPin);      // read input value and store it in val
  if (val != buttonState) 
   {          // the button state has changed!
    if (val == LOW) 
        {                // check if the button is pressed
        Serial.println("LOW donc la lumiere s'allume ");
        const char *msg = "AAAAAAAAAAAAAA"; // Tableau qui contient notre message
        vw_send((uint8_t *)msg, strlen(msg)); // On envoie le message 
        vw_wait_tx(); // On attend la fin de l'envoi
        vw_send((uint8_t *)msg, strlen(msg)); // On envoie le message 
        vw_wait_tx(); // On attend la fin de l'envoi
        vw_send((uint8_t *)msg, strlen(msg)); // On envoie le message 
        vw_wait_tx(); // On attend la fin de l'envoi 
        
        } 
    else  
        {              // the button is -not- pressed...
        Serial.println("HIGH la lumière s eteint");
            const char *msg = "BBBBBBBBBBBBBBBBBBBB"; // Tableau qui contient notre message
        vw_send((uint8_t *)msg, strlen(msg)); // On envoie le message 
        vw_wait_tx(); // On attend la fin de l'envoi
        vw_send((uint8_t *)msg, strlen(msg)); // On envoie le message 
        vw_wait_tx(); // On attend la fin de l'envoi
        vw_send((uint8_t *)msg, strlen(msg)); // On envoie le message 
        vw_wait_tx(); // On attend la fin de l'envoi 
        }
     buttonState = val;      // save the new state in our variable
  }
  
ReadPIN3 = digitalRead(raspPIN3);      // read input ReadPIN3ue and store it in ReadPIN3

  if (ReadPIN3 != lastReadPIN3) 
   {          // the button state has changed!
    
    if(ReadPIN3==HIGH)
    {
      digitalWrite(8,HIGH); 
    } 
    else 
    {
     digitalWrite(8,LOW); 
    }
    
   lastReadPIN3 = ReadPIN3;      // save the new state in our variable
  }
  
 
}
