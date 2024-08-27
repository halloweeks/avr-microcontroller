#include <avr/io.h>
#include <util/delay.h>
#include "crc32.h"

#define CLOCK_PIN 0  // Define CLOCK_PIN
#define DATA_PIN 1   // Define DATA_PIN
#define ACK_PIN 2 // Define ACK_PIN

#define bit(b) (1 << (b))

unsigned int recv_crc() {
	PORTD |= bit(ACK_PIN); // ACK HIGH 
	
	unsigned int crc = 0;
	
	for (unsigned char index = 0; index < 32; index++) {
		// Wait for clock signal to go high
		while (!(PINA & bit(CLOCK_PIN)));
		
		// if read bit high then set CRC bit high in position index value 
		(PINA & bit(DATA_PIN)) ? crc |= bit(index) : crc &= ~bit(index);
		
		// Wait for clock signal to go low
		while (PINA & bit(CLOCK_PIN));
	}
	
	PORTD &= ~bit(ACK_PIN); // Set ACK low 
	return crc;
}

unsigned int recv_length() {
	unsigned int length = 0;
	
	PORTD |= bit(ACK_PIN); // ACK HIGH 
	
	for (unsigned char index = 0; index < 32; index++) {
		// Wait for clock signal to go high
		while (!(PINA & bit(CLOCK_PIN)));
		
		// if read bit high then set length bit high in position index value 
		(PINA & bit(DATA_PIN)) ? length |= bit(index) : length &= ~bit(index);
		
		// Wait for clock signal to go low
		while (PINA & bit(CLOCK_PIN));
	}
	
	PORTD &= ~bit(ACK_PIN); // Set ACK low 
	return length;
}

unsigned int recv_data(unsigned char *data, unsigned int size) {
	unsigned int length = 0;
	unsigned char byte = 0;
	
	PORTD |= bit(ACK_PIN); // ACK HIGH 
	
	for (unsigned int index = 0; index < size; index++) {
		byte = 0;
		
		for (unsigned char x = 0; x < 8; x++) {
			// Wait for clock signal to go high
			while (!(PINA & bit(CLOCK_PIN)));
			
			(PIND & bit(DATA_PIN)) ? byte |= bit(x) : byte &= ~bit(x);
			
			// Wait for clock signal to go low
			while (PINA & (1 << CLOCK_PIN));
		}
		
		data[i] = byte;
	}
	
	PORTD &= ~bit(ACK_PIN); // Set ACK low 
}

int main(void) {
	// Initialize PORTA for receiving data (input mode)
	DDRD &= ~bit(CLOCK_PIN); // Set clock pin for input
	DDRD &= ~bit(DATA_PIN); // Set Data pin for input
	DDRD |= bit(ACK_PIN); // Set ACK_PIN output
	
	// PORTD |= (1 << CLOCK_PIN) | (1 << DATA_PIN);  // Enable pull-ups if necessary
	
	unsigned char buffer[100]; // hold 100 byte data in memory 
	
	crc32_t crc = recv_crc();
	
	unsigned int size = recv_length();
	
	_delay_ms(1000);
	
	recv_data(buffer, 20);
	
	while (1) {}
	return 0;
}
