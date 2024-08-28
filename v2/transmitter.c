// transmit data via digital pin 

#include <avr/io.h>  // Include AVR IO definitions
#include <util/delay.h> // Include delay functions

#define CLOCK_PIN PD0  // Define CLOCK PULSE PIN
#define DATA_PIN PD1   // Define DATA PIN for 1 and 0

#define bit(b) (1 << (b))

#define WAIT_FOR_HIGH(pin) while (!(PIND & bit(pin)))
#define WAIT_FOR_LOW(pin) while (PIND & bit(pin))

void transmit(unsigned char *data, unsigned int size, unsigned long bit_rate) {
	// transmit 32bit data length 
	for (unsigned char index = 0; index < 32; index++) {
		(size & (1 << (31 - index))) ? PORTD |= (1 << DATA_PIN) : PORTD &= ~(1 << DATA_PIN);
		
		PORTD |= (1 << CLOCK_PIN);  // Set CLOCK_PIN high
		_delay_us(1000000 / (2 * bit_rate));
		
		
		PORTD &= ~(1 << CLOCK_PIN);  // Set CLOCK_PIN low
		_delay_us(1000000 / (2 * bit_rate));
	}
	
	// transmit data
    for (unsigned char i = 0; i < size; i++) {
        for (unsigned char x = 0; x < 8; x++) {
        	(data[i] & bit(7 - x)) ? PORTD |= (1 << DATA_PIN) : PORTD &= ~(1 << DATA_PIN);
            
            PORTD |= (1 << CLOCK_PIN);  // Set CLOCK_PIN high
            _delay_us(1000000 / (2 * bit_rate));              
            
            PORTD &= ~(1 << CLOCK_PIN);  // Set CLOCK_PIN low
            _delay_us(1000000 / (2 * bit_rate));              
        }
    }
}

int main(void) {
    // Initialize PORTD for sending data (output mode)
    DDRD = bit(CLOCK_PIN) | bit(DATA_PIN); // Set CLOCK_PIN and DATA_PIN as output
    
    // unsigned char data[] = "halloweeks";
    
    unsigned char data[] = "Hello, AVR!";
    transmit(data, sizeof(data) - 1, 1000); // Send "Hello, AVR!" at 1000 bps

    // transmit(data, sizeof(data) - 1, 1000); // 1000 bps
    
    while (1) {}
    
    return 0;
}