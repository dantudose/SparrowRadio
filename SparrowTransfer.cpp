#include "SparrowTransfer.h"
#include "RadioFunctions.h"


//Captures address and size of struct
void SparrowTransfer::begin(uint8_t * ptr, uint8_t length){
	rfBegin(11);  // Initialize ATmega128RFA1 radio on channel 11 (can be 11-26)
	
	address = ptr;
	size = length;
	

	//dynamic creation of rx parsing buffer in RAM
	rx_buffer = (uint8_t*) malloc(BYTES_TO_PAYLOAD+size+1);
}

//Sends out struct in binary, with header, length info and checksum
void SparrowTransfer::sendData(){
  uint8_t checksum;
   
  
   checksum = size; //checksum starts with payloadsize

   rx_buffer[0] = HEADER;
   rx_buffer[1] = size;
    
        
   
   for(int i = 0; i < size; i++)
   {
      checksum ^= address[i];
      rx_buffer[BYTES_TO_PAYLOAD + i] = address[i];
   }
    
	// Complete with the checksum
    rx_buffer[BYTES_TO_PAYLOAD + size] = checksum;

    rfSend(rx_buffer, BYTES_TO_PAYLOAD + size + 1);

}

uint8_t SparrowTransfer::receiveData(){
  
    
   uint8_t state;
   uint8_t data;
   
   rx_len = 0; 
    
    if (rfAvailable())
    {
       
    
    while(true)
    {
        data = rfRead();

        if (data == -1)
        {
            return rx_len > 0 ? WAITING_FOR_DATA : NO_DATA;
        }

        if (rx_len == 0)
        {
            // Look for header
            if (data != HEADER)
            {
                return NO_HEADER;
            }

            ++rx_len;
            continue;
        }

        if (rx_len == 1)
        {
            // Look for payload size
            if (data != size)
            {
                rx_len = 0;
                return INVALID_SIZE;
            }

            // The checksum starts with the payload size
            calc_CS = data;

            ++rx_len;
            continue;
        }

        // The > check ensures that we don't overflow and
        // that we discard bad messages
        if (rx_len >= (size + BYTES_TO_PAYLOAD))
        {
            // We are done with the message. Regardless of the outcome
            // only a new message can come next
            rx_len = 0;

            // Before we can be fully done, we need the
            // checksum to match
            if (calc_CS != data)
            {
                return INVALID_CHECKSUM;
            }
           
            return SUCCESS;
        }
                
        address[rx_len - BYTES_TO_PAYLOAD] = data;
        calc_CS ^= data;
        
        ++rx_len;
    }
    
    }
     // Cannot get any data
        return NO_RF;
}
