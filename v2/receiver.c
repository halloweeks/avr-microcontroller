// receive data via digital pin 
#include <avr/io.h>
#include <util/delay.h>

#define CLOCK_PIN 0  // Define CLOCK_PIN
#define DATA_PIN 1   // Define DATA_PIN

#define bit(b) (1 << (b))

#define WAIT_FOR_HIGH(pin) while (!(PIND & bit(pin)))
#define WAIT_FOR_LOW(pin) while (PIND & bit(pin))

void recv_data(unsigned char *data) {
	unsigned int length = 0;
	
	// read length 
	for (unsigned char index = 0; index < 32; index++) {
		// Wait for clock signal to go high
		WAIT_FOR_HIGH(CLOCK_PIN);
		
		if (PIND & bit(DATA_PIN)) {
			length |= bit(index);
		}
		
		// Wait for clock signal to go low
		WAIT_FOR_LOW(CLOCK_PIN);
	}
	
	for (unsigned int index = 0; index < length * 8; index++) {
		// Wait for clock signal to go high
		WAIT_FOR_HIGH(CLOCK_PIN);
		
		if (PIND & bit(DATA_PIN)) {
			data[index / 8] |= bit(7 - (index % 8));
		}
		
		// Wait for clock signal to go low
		WAIT_FOR_LOW(CLOCK_PIN);
	}
}

int main(void) {
	// Initialize PORTA for receiving data (input mode)
	DDRD &= ~bit(CLOCK_PIN); // Set clock pin for input
	DDRD &= ~bit(DATA_PIN); // Set Data pin for input
	
	unsigned char buffer[100] = {0}; // hold 100 byte data in memory 
	
	recv_data(buffer);
	
	
	while (1) {}
	return 0;
}
