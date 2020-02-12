/*
 * GccApplication2.cpp
 *
 * Created: 11/29/2019 3:34:12 PM
 * Author : felix
 */ 

//#define F_CPU 12000000UL
#define F_CPU (16000000UL/1)
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>

#include "GepinSlave.h"
#include "../tof_pcb.h"
//#define BAUD_RATE_9600_BPS  77 12mhz
#define BAUD_RATE_9600_BPS  (103/1)


// variable list declaration
typedef TofPCB __attribute__ ((aligned (4), packed)) var_table_t, *p_var_table_t;

var_table_t var_table;

static int printCHAR(char character, FILE *stream)
{
	while ((UCSR0A & (1 << UDRE0)) == 0) {};

	UDR0 = character;

	return 0;
}

static FILE *pmystdout = fdevopen(printCHAR, NULL); 

bool writeElemCallback(uint32_t addr, uint32_t data);
bool readElemCallback(uint32_t addr, uint32_t *data);
uint32_t readAdc(char chan);
uint8_t start_pulse_detect();
uint8_t stop_pulse_detect();


// global variables
GepinSlave gepin_slave = GepinSlave();

int main(void)
{

	int i = 0;
	unsigned int ubrr = BAUD_RATE_9600_BPS;
	unsigned char data[] = "Hello from ATmega328p   ";

	/* Set Baudrate  */
	UBRR0H = (ubrr>>8);
	UBRR0L = (ubrr);

	UCSR0C = 0x06;       /* Set frame format: 8data, 1stop bit  */
	UCSR0B = (1<<TXEN0); /* Enable  transmitter                 */
	UCSR0B |= (1<<RXEN0);

	PORTB =0xff ;  //set all pins of port b as outputs
	
	// gepin	
	gepin_slave.variable_list_len = sizeof(var_table)/sizeof(uint32_t);
	gepin_slave.pVariables = (uint32_t *) &var_table;
	var_table.id = 0xfa19;
	
	gepin_slave.registerWriteElemCallback(&writeElemCallback);
	gepin_slave.registerReadElemCallback(&readElemCallback);
	
	// pwm initializations
	// see example https://www.avrfreaks.net/forum/atmega328p-4-pwm-outputs
	
	// PD6, OC0A
	DDRD |= (1 << DDD6); // PD6 is now an output
	var_table.pwm_comp_level_2 = 0;	
	OCR0A = var_table.pwm_comp_level_2; // set PWM for 50% duty cycle
	TCCR0A |= (1 << COM0A1);// set none-inverting mode
	TCCR0A |= (1 << WGM01) | (1 << WGM00); // set fast PWM Mode
	TCCR0B |= (1 << CS01); // set prescaler to 8 and starts PWM
	
	// PD3, OC2B
	DDRD |= (1 << DDD3); // PD3 is now an output
	var_table.pwm_comp_level_1= 0;
	OCR2B = var_table.pwm_comp_level_1; // set PWM for 50% duty cycle	
	TCCR2A |= 1<<COM2B1; // set none-inverting mode
	TCCR2A  |= (1 << WGM10) | (1 << WGM21); // set fast PWM Mode
    TCCR2B |= (1 << CS21); // set prescaler to 8 and starts PWM
	
	// PD5, OC0B
	DDRD |= (1 << DDD5); // PD5 is now an output
	var_table.pwm_v_adj_apd = 0;
	OCR0B  = var_table.pwm_v_adj_apd;
	TCCR0A |= 1<<COM0B1;
	
	// adc init
	ADMUX = (1<<REFS0)  ; //REFS0: select AVCC as reference
	ADCSRA  = (1<<ADEN) | 7;  //enable and prescale = 128 (16MHz/128 = 125kHz)
	

	/* Replace with your application code */
	while (1)
	{
		
		//DDRB &= ~(1<<5); // switch of led
		
		//_delay_ms(1000);
		//DDRB |= 1<<5; // switch on led
		//_delay_ms(2000);
		//DDRB &= ~(1<<5); //0b00000000;

		//i = 0;
		//while(data[i] != 0) /* print the String  "Hello from ATmega328p" */
		//{
		//	while (!( UCSR0A & (1<<UDRE0))); /* Wait for empty transmit buffer*/
		//	UDR0 = data[i];            /* Put data into buffer, sends the data */
		//	i++;                             /* increment counter           */
		//}
		
		//stdout = pmystdout;
		//printf("Hello World 2\n\r");
		
		gepin_slave.update();
		gepin_slave.newMessage = false;
	}
	
}

bool readElemCallback(uint32_t addr, uint32_t *data){
	if (addr == gepin_slave.getVarAddr(&var_table.v_5v_sense)) {
		*data = readAdc(0);
		return true; 
	}
	else if (addr == gepin_slave.getVarAddr(&var_table.v_apd_sense)) {
		*data = readAdc(1);
		return true;
	}
	else if (addr == gepin_slave.getVarAddr(&var_table.v_apd_r_sense)) {
		*data = readAdc(1); // not possible to read
		return true;
	}
	else if (addr == gepin_slave.getVarAddr(&var_table.v_sipm_sense)) {
		*data = readAdc(2);
		return true;
	}
	else if (addr == gepin_slave.getVarAddr(&var_table.lockin_1_filt)) {
		*data = readAdc(3);
		return true;
	}
	else if (addr == gepin_slave.getVarAddr(&var_table.lockin_2_filt)) {
		*data = readAdc(4);
		return true;
	}
	else if (addr == gepin_slave.getVarAddr(&var_table.lockin_1_peak)) {
		*data = readAdc(5);
		return true;
	}
	else if (addr == gepin_slave.getVarAddr(&var_table.temperature)) {
		ADMUX = (1<<REFS0) | (1<<REFS1); // set to internal reference for temperature measurement
		_delay_ms(5);
		*data = readAdc(8);
		_delay_ms(5);
		ADMUX = (1<<REFS0); 
		return true;
	}
	else if (addr == gepin_slave.getVarAddr(&var_table.start_pulses)) {
		*data = start_pulse_detect();
		return true;
	}
	else if (addr == gepin_slave.getVarAddr(&var_table.stop_pulses)) {
		*data = stop_pulse_detect();
		return true;
	}
	else {
		return false;
	}
	
	
}

bool writeElemCallback(uint32_t addr, uint32_t data){

	if (addr == gepin_slave.getVarAddr(&var_table.pwm_comp_level_1)) {
		OCR2B = (int8_t) data;
		return true;
	}
	else if (addr == gepin_slave.getVarAddr(&var_table.pwm_comp_level_2)){
		OCR0A = (int8_t) data;	
		return true;
	}
	else if (addr == gepin_slave.getVarAddr(&var_table.pwm_v_adj_apd)){
		OCR0B = (int8_t) data;
		return true;
	}
	else {
		return false;
	}
}

uint32_t readAdc(char chan)
{
	PRR0 &= ~(1 << PRADC);
	ADMUX = (ADMUX&(1<<REFS0)) | (ADMUX&(1<<REFS1)) | (chan & 0x0f);  //select input and ref
	ADCSRA |= (1<<ADSC);                 //start the conversion
	while (!(ADCSRA & (1 << ADIF)));          //wait for end of conversion
	ADCSRA |= (1 << ADIF); // ADIF does not self-clear like ADSC so now you must clear it too
	uint8_t adcl;
	adcl = ADCL;
	return (ADCH << 8) | adcl;
}

uint8_t start_pulse_detect()
{
	uint8_t count = 0;
	for(int i; i<255; i++) {
		if (PINE1) count++;		
	}
	return count;
}

uint8_t stop_pulse_detect()
{
	uint8_t count = 0;
	for(int i; i<255; i++) {
		if (PINE3) count++;
	}
	return count;
}


