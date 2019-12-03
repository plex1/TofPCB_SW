// Gepin.h

#ifndef Gepin_h
#define Gepin_h

#include <stdlib.h>
#include <stdint.h>

class GepinSlave
{
public:

    typedef enum
    {
	COMMAND_READ  = 0,
	COMMAND_WRITE    = 1, 
    } CommandType;

	// header definition
	typedef struct { 
	  uint8_t id; // protocol identifier?
	  uint8_t tag;
	  uint8_t command;
	  union {
		struct {
		  uint8_t request : 1;
		  uint8_t incr : 1;
		  uint8_t nack : 1; // tbc
		  uint8_t use_checksum : 1; //tbc
		  uint8_t reserved : 4; //byte/word addressed, 8/16/32/64 bit addressing, address shift
		} fields;
		uint8_t value;
	  } flags;	  
	  uint32_t addr;
	  uint32_t len;
	} __attribute__ ((aligned (4), packed)) msg_header_t;

	typedef struct{
	  msg_header_t *header;
	  uint32_t *data;
	}
	message_t, *pMessage_t;

	// instantiation
	msg_header_t msg_header;
	
	static const uint8_t header_len = 12; //in bytes
	static const uint8_t max_data_len = 16;
	uint32_t msg_data[max_data_len];

	message_t message;
	
	uint32_t *pVariables;
	uint32_t variable_list_len;
	
	uint32_t offset;
	
	bool waitingForHeader;
	
	GepinSlave(uint32_t *pVariables);
	GepinSlave(void);
	
	void update(void);
	
	bool newMessage;
	
	uint32_t getVarAddr(uint32_t *pVar);
	
	typedef bool (*rw_cb_t)(message_t *message);  //read message / write message callback
	typedef bool (*re_cb_t)(uint32_t addr, uint32_t *data);  // read element callback
	typedef bool (*we_cb_t)(uint32_t addr, uint32_t data);   // write element callback
		
	
	void registerReadCallback(rw_cb_t r_cb);
	void registerWriteCallback(rw_cb_t w_cb);
	void registerReadElemCallback(re_cb_t re_cb);
	void registerWriteElemCallback(we_cb_t we_cb);
	
	uint16_t debug_level;
	
private:

	rw_cb_t readCallback;
	rw_cb_t writeCallback;
	re_cb_t readElemCallback;
	we_cb_t writeElemCallback;
	
	bool isMessageAvailable(void);
	bool isHeaderAvailable(void);
	bool isDataAvailable(uint16_t data_len_words);
	void getMessageHeader(msg_header_t *header);
	void getMessageData(uint32_t *data, uint8_t len);
	void getMessage(message_t *message);
	void sendMessage(message_t *message);
	void HAL_sendChar(uint8_t character);
	bool HAL_charAvailable();
	uint8_t HAL_readChar();
	
};

#endif 

	
	
