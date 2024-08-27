// transmit data via digital pin 

#include <avr/io.h>  // Include AVR IO definitions
#include <util/delay.h> // Include delay functions
#include "crc32.h" // Include CRC checksum 

#define CLOCK_PIN PD0  // Define CLOCK_PIN, adjust as necessary
#define DATA_PIN PD1   // Define DATA_PIN, adjust as necessary
#define ACK_PIN PD2

void transmit(void *data, unsigned int size, unsigned long bit_rate) {
	// calculate CRC value of the data
	crc32_t crc = crc32(data, size);
	
	// transmit 32bit crc
	for (unsigned char index = 0; index < 32; index++) {
		(crc & (1 << (31 - index))) ? PORTD |= (1 << DATA_PIN) : PORTD &= ~ (1 << DATA_PIN);
		
		PORTD |= (1 << CLOCK_PIN);  // Set CLOCK_PIN high
		_delay_us(1000000 / (2 * bit_rate));
		
		
		PORTD &= ~(1 << CLOCK_PIN);  // Set CLOCK_PIN low
		_delay_us(1000000 / (2 * bit_rate));              
	}
	
	// transmit 32bit data length 
	for (unsigned char i = 0; i < 32; i++) {
		if (size & (1 << (31 - i))) {
			PORTD |= (1 << DATA_PIN); // Set DATA_PIN high 
		} else {
			PORTD &= ~(1 << DATA_PIN); // Set DATA_PIN low
		}
		
		PORTD |= (1 << CLOCK_PIN);  // Set CLOCK_PIN high
		_delay_us(1000000 / (2 * bit_rate));
		
		
		PORTD &= ~(1 << CLOCK_PIN);  // Set CLOCK_PIN low
		_delay_us(1000000 / (2 * bit_rate));
	}
	
	// transmit data
    for (unsigned char i = 0; i < size; i++) {
        for (unsigned char x = 0; x < 8; x++) {
        	// Update DATA_PIN based on the bit value from data[i]
            if ((((unsigned char*)data)[i] >> x) & 0x01) {
                PORTD |= (1 << DATA_PIN);  // Set DATA_PIN high
            } else {
                PORTD &= ~(1 << DATA_PIN); // Set DATA_PIN low
            }
            
            PORTD |= (1 << CLOCK_PIN);  // Set CLOCK_PIN high
            _delay_us(1000000 / (2 * bit_rate));              
            
            PORTD &= ~(1 << CLOCK_PIN);  // Set CLOCK_PIN low
            _delay_us(1000000 / (2 * bit_rate));              
        }
        
        // wait for ACK pin high 
        while (!(PIND & (1 << ACK_PIN))) {}
    }
}

int main(void) {
    // Initialize PORTD for sending data (output mode)
    DDRD = (1 << CLOCK_PIN) | (1 << DATA_PIN); // Set CLOCK_PIN and DATA_PIN as output
    DDRD &= (1 << ACK_PIN); // Set ACK_PIN as input 
    
    unsigned char data[] = "halloweeks";
    
    transmit(data, sizeof(data) - 1, 1000);
    
    while (1) {}
    
    return 0;
}
