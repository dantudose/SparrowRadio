#ifndef SparrowTransfer_h
#define SparrowTransfer_h

//make it a little prettier on the front end. 
#define details(name) (byte*)&name,sizeof(name)

//Not neccessary, but just in case. 
#if ARDUINO > 22
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Stream.h"

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#define NO_RF 0
#define NO_DATA 1
#define NO_HEADER 2
#define WAITING_FOR_DATA 3
#define INVALID_SIZE 4
#define INVALID_CHECKSUM 5
#define SUCCESS 6

#define HEADER 0x23

// The number of bytes required to read before the actual payload
// is encountered
#define BYTES_TO_PAYLOAD 2


class SparrowTransfer {
public:
void begin(uint8_t *, uint8_t);
//void begin(uint8_t *, uint8_t, NewSoftSerial *theSerial);
void sendData();
uint8_t receiveData();

private:

//NewSoftSerial *_serial;
uint8_t * address;  //address of struct
uint8_t size;       //size of struct
uint8_t * rx_buffer; //address for temporary storage and parsing buffer
uint8_t rx_array_inx;  //index for RX parsing buffer
uint8_t rx_len;		//RX packet length according to the packet
uint8_t calc_CS;	   //calculated Checksum
};



#endif