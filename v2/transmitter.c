// transmit data via digital pin 

#include <avr/io.h>  // Include AVR IO definitions
#include <util/delay.h> // Include delay functions

#define CLOCK_PIN PD0  // Define CLOCK PULSE PIN
#define DATA_PIN PD1   // Define DATA PIN for 1 and 0

#define bit(b) (1 << (b))

#define WAIT_FOR_HIGH(pin) while (!(PIND & bit(pin)))
#define WAIT_FOR_LOW(pin) while (PIND & bit(pin))

void transmit(unsigned char *data, unsigned int size, unsigned long bit_rate) {
	for (unsigned char index = 0; index < size * 8; index++) {
		if (data[index / 8] & bit(7 - (index % 8))) {
			PORTD |= (1 << DATA_PIN);
		} else {
			PORTD &= ~(1 << DATA_PIN);
		}
		
		PORTD |= (1 << CLOCK_PIN);  // Set CLOCK_PIN high
		_delay_us(1000000 / (2 * bit_rate));              
		
		PORTD &= ~(1 << CLOCK_PIN);  // Set CLOCK_PIN low
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