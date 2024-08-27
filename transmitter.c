// transmit data via digital pin 

#include <avr/io.h>  // Include AVR IO definitions
#include <util/delay.h> // Include delay functions
#include "crc32.h" // Include CRC checksum 

#define CLOCK_PIN PD0  // Define CLOCK PULSE PIN
#define DATA_PIN PD1   // Define DATA PIN for 1 and 0
#define ACK_PIN PD2    // Define ACKNOWLEDGE PIN

#define bit(b) (1 << (b))

void transmit(unsigned char *data, unsigned int size, unsigned long bit_rate) {
	// calculate CRC value of the data
	crc32_t crc = crc32(data, size);
	
	// wait for ACK pin high 
	while (!(PIND & bit(ACK_PIN))) {}
	
	// transmit 32bit crc
	for (unsigned char index = 0; index < 32; index++) {
		(crc & bit(31 - index)) ? PORTD |= (1 << DATA_PIN) : PORTD &= ~ (1 << DATA_PIN);
		
		PORTD |= bit(CLOCK_PIN);  // Set CLOCK_PIN high
		_delay_us(1000000 / (2 * bit_rate));
		
		
		PORTD &= ~(1 << CLOCK_PIN);  // Set CLOCK_PIN low
		_delay_us(1000000 / (2 * bit_rate));              
	}
	
	// wait for ACK pin high 
    while (!(PIND & bit(ACK_PIN))) {}
	
	// transmit 32bit data length 
	for (unsigned char index = 0; index < 32; index++) {
		(size & (1 << (31 - index))) ? PORTD |= (1 << DATA_PIN) : PORTD &= ~(1 << DATA_PIN);
		
		PORTD |= (1 << CLOCK_PIN);  // Set CLOCK_PIN high
		_delay_us(1000000 / (2 * bit_rate));
		
		
		PORTD &= ~(1 << CLOCK_PIN);  // Set CLOCK_PIN low
		_delay_us(1000000 / (2 * bit_rate));
	}
	
	// wait for ACK pin high 
    while (!(PIND & bit(ACK_PIN))) {}
	
	// transmit data
    for (unsigned char i = 0; i < size; i++) {
        for (unsigned char x = 0; x < 8; x++) {
        	(data[i] & bit(7 - x)) ? PORTD |= (1 << DATA_PIN) : PORTD &= ~(1 << DATA_PIN);
            
            PORTD |= (1 << CLOCK_PIN);  // Set CLOCK_PIN high
            _delay_us(1000000 / (2 * bit_rate));              
            
            PORTD &= ~(1 << CLOCK_PIN);  // Set CLOCK_PIN low
            _delay_us(1000000 / (2 * bit_rate));              
        }
        
        // wait for ACK pin high 
        while (!(PIND & bit(ACK_PIN))) {}
    }
}

int main(void) {
    // Initialize PORTD for sending data (output mode)
    DDRD = bit(CLOCK_PIN) | bit(DATA_PIN); // Set CLOCK_PIN and DATA_PIN as output
    DDRD &= ~ bit(ACK_PIN); // Set ACK_PIN as input 
    
    unsigned char data[] = "halloweeks";
    
    transmit(data, sizeof(data) - 1, 1000);
    
    while (1) {}
    
    return 0;
}