#include "SparrowTransfer.h"
 
//create object
SparrowTransfer ST; 
 
struct SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  uint16_t data;
};
 
//give a name to the group of data
SEND_DATA_STRUCTURE mydata;
 
void blinkLED() //blinks the LED
{
  digitalWrite(8,LOW);
  delay(20);
  digitalWrite(8,HIGH);  
}
 
void setup(){
 Serial.begin(9600);
 
 //start the library, pass in the data details
 ST.begin(details(mydata));
 
 pinMode(8, OUTPUT);
 digitalWrite(8, LOW);
 
 mydata.data = 0;
 
}
 
void loop(){
 
  mydata.data++;
 
  //send the data
  ST.sendData();
  blinkLED();
 
  delay(1000);
}