#include <avr/io.h>  // Include AVR IO definitions
#include <util/delay.h> // Include delay functions

#define CLOCK_PIN PD7  // Define CLOCK PIN
#define DATA_PIN PD6   // Define DATA PIN

#define bit(b) (1 << (b))

void transmit(char *data, unsigned int size, unsigned long bit_rate) {
	for (unsigned char index = 0; index < size * 8; index++) {
		if (data[index / 8] & bit(7 - (index % 8))) {
			PORTD |= bit(DATA_PIN);
		} else {
			PORTD &= ~bit(DATA_PIN);
		}
		
		PORTD |= bit(CLOCK_PIN);  // Set CLOCK_PIN high
		_delay_us(1000000 / (2 * bit_rate));              
		
		PORTD &= ~bit(CLOCK_PIN);  // Set CLOCK_PIN low
		_delay_us(1000000 / (2 * bit_rate));
	}
}

int main() {
	DDRD = bit(CLOCK_PIN) | bit(DATA_PIN); // Set CLOCK_PIN and DATA_PIN as output
	
	unsigned char data[] = "Hello, AVR!";
	transmit(data, sizeof(data) - 1, 1000); // Send "Hello, AVR!" at 1000 bps
	
	while (1) {}
	
	return 0;
}
