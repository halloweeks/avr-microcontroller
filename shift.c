#include <avr/io.h>
#include <util/delay.h>

#define DATA_PIN PD5
#define CLOCK_PIN PD6
#define LATCH_PIN PD7

void shiftOut(unsigned int data) {
	for (unsigned char i = 0; i < 8; i++) {
		PORTD &=~ (1 << CLOCK_PIN); // CLOCK LOW
		
		if (data & (1 << i)) {
			PORTD |= (1 << DATA_PIN); // DATA BIT 1
		} else {
			PORTD &=~ (1 << DATA_PIN); // DATA BIT 0
		}
		
		PORTD |= (1 << CLOCK_PIN); // CLOCK HIGH 
	}
	
	PORTD &=~ (1 << LATCH_PIN); // LATCH LOW
	PORTD |= (1 << LATCH_PIN); // LATCH HIGH 
}

int main() {
	// SET DATA_PIN, CLOCK_PIN, LATCH_PIN OUTPUT 
	DDRD |= (1 << DATA_PIN) | (1 << CLOCK_PIN) | (1 << LATCH_PIN);
	
	// RUN CONTINUOUSLY
	while (1) {
		shiftOut(0xFF); // All LEDs on
		_delay_ms(1000); // Delay 1 Second 
		shiftOut(0x00); // All LEDs off
		_delay_ms(1000); // Delay 1 Second 
	}
	
	return 0;
}