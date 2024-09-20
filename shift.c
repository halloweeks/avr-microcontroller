#include <avr/io.h>
#include <util/delay.h>

#define DATA_PIN PD5
#define CLOCK_PIN PD6
#define LATCH_PIN PD7

void _sys_write_register(unsigned char data) {
	for (unsigned char i = 8; i-->0;) {
		// Most Significant Bit
		if (data & (1 << i)) {
			PORTD |= (1 << DATA_PIN); // DATA BIT 1
		} else {
			PORTD &=~ (1 << DATA_PIN); // DATA BIT 0
		}
		
		PORTD &=~ (1 << CLOCK_PIN); // CLOCK LOW
		_delay_us(10);
		PORTD |= (1 << CLOCK_PIN); // CLOCK HIGH 
		_delay_us(10);
	}
	
	PORTD &=~ (1 << LATCH_PIN); // LATCH LOW
	_delay_us(10);
	PORTD |= (1 << LATCH_PIN); // LATCH HIGH 
	_delay_us(10);
}



int main() {
	// SET DATA_PIN, CLOCK_PIN, LATCH_PIN AS OUTPUT 
	DDRD |= (1 << DATA_PIN) | (1 << CLOCK_PIN) | (1 << LATCH_PIN);
	
	unsigned char data = 0b00000000;
	
	while (1) {
		data ^= (1 << 0); // TOGGLE PIN 0
		_sys_write_register(data); // WRITE OUTPUT IN SHIFT REGISTER 
		_delay_ms(1000); // DELAY FOR 1 SECOND 
	}
	
	return 0;
}