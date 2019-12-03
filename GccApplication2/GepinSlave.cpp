
#include "GepinSlave.h"
#include "queue.h"
#include "utils.h"

#include <avr/io.h> // to be moved in hal file

GepinSlave::GepinSlave(uint32_t *pVariablesInit)
{
	pVariables = pVariablesInit;
	GepinSlave();
}

GepinSlave::GepinSlave(void)
{
	newMessage = false;
	waitingForHeader = true;
	debug_level = 0;
	variable_list_len = 16;
	offset = 0;
	readCallback = NULL;
	writeCallback = NULL;
	readElemCallback = NULL;
	writeElemCallback = NULL;
	// assign variables
	message.header = &msg_header;
	message.data = msg_data;
}

void GepinSlave::registerReadCallback(rw_cb_t r_cb){
	readCallback = r_cb;
}

void GepinSlave::registerWriteCallback(rw_cb_t w_cb){
	writeCallback = w_cb;
}

void GepinSlave::registerReadElemCallback(re_cb_t re_cb){
	readElemCallback = re_cb;
}

void GepinSlave::registerWriteElemCallback(we_cb_t we_cb){
	writeElemCallback = we_cb;
}

uint32_t GepinSlave::getVarAddr(uint32_t *pVar)
{
	return ((uint32_t) pVar - (uint32_t) pVariables)/4;
}

void GepinSlave::update(void)
{
  int inByte;         // incoming serial byte	
  if (HAL_charAvailable() > 0) {
    // get incoming byte:
    inByte = HAL_readChar();
	//if (debug_level>0){
		//Serial.print(" IN:");
		//Serial.print(inByte, HEX); //Debug
	//}
    queue_push(inByte);
  }

  if (waitingForHeader && isHeaderAvailable()){
    getMessageHeader(message.header);
    waitingForHeader = false;
   // if (debug_level>0){//Serial.print("Rx Header \n");}
  }

  if (waitingForHeader == false && (isDataAvailable(message.header->len) || message.header->command==0)){
    //if (debug_level>0){
		//Serial.print(" ID:");
		//Serial.print(message.header->id, HEX); 
		//Serial.print(" LEN:");
		//Serial.print(message.header->len, HEX); 
		//Serial.print(" Addr:");
		//Serial.print(message.header->addr,HEX);  
		//Serial.print(" request: ");
		//Serial.print(message.header->flags.fields.request, HEX);
		//Serial.print(" command: ");
		//Serial.print(message.header->command, HEX);
		//Serial.print("\n");
	//}

    bool response_generated = false;
	bool cb_return=false;
	
    // write command handling
    if ((message.header->command == 1) && message.header->flags.fields.request) {
      if(message.header->len >= 1){
		getMessageData(message.data, message.header->len);
		//if (debug_level>0){
			//Serial.print(" Data:");
			//Serial.print(message.data[0],HEX);
		//}
        if (message.header->addr + message.header->len <= variable_list_len){
		  if (writeCallback!=NULL){
			(*writeCallback)(&message);
		  }
		  uint32_t addr_cnt = message.header->addr;
	      for (int i=0; i<message.header->len; i++){
			cb_return=false;
			if (writeElemCallback !=NULL){
					cb_return =(*writeElemCallback)(addr_cnt, message.data[i]);
			}
			if (writeElemCallback !=NULL && cb_return) {	
				((uint32_t *) pVariables)[addr_cnt] =  message.data[i];
			}
            //if (debug_level>0){//Serial.print("Saved: "); //Serial.print(message.data[i],HEX);
			//}
			if  (message.header->flags.fields.incr) addr_cnt += 1;
          }
          //generate response    
          response_generated = true;
		  newMessage = true;
          //if (debug_level>0){//Serial.print(" write done ");
		  //}
        }
      }
    }

    // read command handling
    if ((message.header->command == 0) && message.header->flags.fields.request) {
      if(message.header->len >= 1){
        if (message.header->addr + message.header->len <= variable_list_len){
		  cb_return=false;
		  if (readCallback!=NULL){
			  cb_return =(*readCallback)(&message);
		  }
		  if (!cb_return){
			  uint32_t addr_cnt = message.header->addr;
			  for (int i=0; i<message.header->len; i++){
				cb_return=false;
				if (readElemCallback !=NULL){
					cb_return =(*readElemCallback)(addr_cnt, &message.data[i]);
				}
				if (!cb_return) {message.data[i] = ((uint32_t *) pVariables)[addr_cnt];}
				//if (debug_level>0){
					//Serial.print("Retrived: "); //Serial.print(message.data[i],HEX);
				//}
				if  (message.header->flags.fields.incr) addr_cnt += 1;
				}
		  }
          //generate response
          response_generated = true;
		  newMessage = true;
          //if (debug_level>0){//Serial.print(" read done ");
		  //}
        }
      }
    }

    if (response_generated == false) {
      // generate nack
      message.header->flags.fields.nack=1;
    }
    
	message.header->flags.fields.request = 0;
    sendMessage(&message);
    waitingForHeader=true;
    
  }
}

bool GepinSlave::isMessageAvailable(){
 return (queueLength>=16);
}

bool GepinSlave::isHeaderAvailable(){
 return (queueLength>=header_len);
}

bool GepinSlave::isDataAvailable(uint16_t data_len_words){
  return (queueLength>=4*data_len_words);
}

void GepinSlave::getMessageHeader(msg_header_t *header){
    for (int i=0; i<header_len; i++) {
      ((uint8_t *)header)[i] = queue_pop();
    }
	header->addr = ntohl(header->addr)-offset;
	header->len = ntohl(header->len);
}

void GepinSlave::getMessageData(uint32_t *data, uint8_t len){
  for (int i=0; i<len; i++){
    uint32_t dataword=0;
    uint32_t b_rx;
    for (int j=0; j<4; j++){
      b_rx = (unsigned char) queue_pop();
	  if (debug_level>0){
		  //Serial.print(" pop:");
		  //Serial.print(b_rx, HEX);
	  }
      dataword = dataword + (b_rx<<((3-j)*8)); //ntohl already here
    }
	if (debug_level>0){
		//Serial.print(" dataword in getMessageData:");
		//Serial.print(dataword, HEX);
	}
    data[i] = dataword; //ntohl(dataword); 
  }
}

void GepinSlave::sendMessage(message_t *message){
  message->header->addr = htonl(message->header->addr-offset);
  message->header->len = htonl(message->header->len);
  for (int i=0; i<12; i++) {
    //Serial1.write(((uint8_t *)(message->header))[i]);
	HAL_sendChar(((uint8_t *)(message->header))[i]);
  }
  message->header->addr = ntohl(message->header->addr)+offset;
  message->header->len = ntohl(message->header->len);

  uint32_t data;
  if (message->header->command == 0 && message->header->flags.fields.nack == 0){	
    for (int i=0; i<message->header->len; i++){
	data = message->data[i]; //htonl(message->data[i]);		
      for (int j=0; j<4; j++){
        //Serial1.write(data>>((3-j)*8)); //htonl already here
		HAL_sendChar(data>>((3-j)*8));
      } 
    }
  }
}

void GepinSlave::HAL_sendChar(uint8_t character){
	while (!( UCSR0A & (1<<UDRE0))); /* Wait for empty transmit buffer*/
	UDR0 = character;
}

bool GepinSlave::HAL_charAvailable(){
	return (UCSR0A & (1<<RXC0));
}

uint8_t GepinSlave::HAL_readChar(){
	return UDR0;
}
