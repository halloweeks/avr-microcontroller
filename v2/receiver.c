#include <avr/io.h>
#include <util/delay.h>

#define CLOCK_PIN PD7  // Define CLOCK_PIN
#define DATA_PIN PD6   // Define DATA_PIN

#define bit(b) (1 << (b))

#define WAIT_FOR_HIGH(pin) while (!(PIND & bit(pin)))
#define WAIT_FOR_LOW(pin) while (PIND & bit(pin))

void recv_data(char *data, unsigned int size) {
	for (unsigned int index = 0; index < size * 8; index++) {
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
	DDRD &= ~bit(CLOCK_PIN); // Set clock pin for input
	DDRD &= ~bit(DATA_PIN); // Set Data pin for input
	
	char buffer[100] = {0}; // hold 100 byte data in memory 
	
	recv_data(buffer, 100);
	
	while (1) {}
	return 0;
}
