// transmit data via digital pin 

#include <avr/io.h>  // Include AVR IO definitions
#include <util/delay.h> // Include delay functions
#include "crc32.h"

#define CLOCK_PIN 0  // Define CLOCK_PIN, adjust as necessary
#define DATA_PIN 1   // Define DATA_PIN, adjust as necessary
#define ACK_PIN 2

void transmit(void *data, unsigned int size, unsigned long bit_rate) {
	// calculate CRC value of the data
	crc32_t crc = crc32(data, size);
	
	// transmit 32bit crc
	for (unsigned char index = 0; index < 32; index++) {
		if (crc & (1 << (31 - index))) {
			PORTA |= (1 << DATA_PIN);
		} else {
			PORTA &= ~ (1 << DATA_PIN);
		}
		
		PORTA |= (1 << CLOCK_PIN);  // Set CLOCK_PIN high
		_delay_us(1000000 / (2 * bit_rate));
		
		
		PORTA &= ~(1 << CLOCK_PIN);  // Set CLOCK_PIN low
		_delay_us(1000000 / (2 * bit_rate));              
	}
	
	// transmit 32bit data length 
	for (unsigned char i = 0; i < 32; i++) {
		if (size & (1 << (31 - i))) {
			PORTA |= (1 << DATA_PIN); // Set DATA_PIN high 
		} else {
			PORTA &= ~(1 << DATA_PIN); // Set DATA_PIN low
		}
		
		PORTA |= (1 << CLOCK_PIN);  // Set CLOCK_PIN high
		_delay_us(1000000 / (2 * bit_rate));
		
		
		PORTA &= ~(1 << CLOCK_PIN);  // Set CLOCK_PIN low
		_delay_us(1000000 / (2 * bit_rate));
	}
	
	// transmit data
    for (unsigned char i = 0; i < size; i++) {
        for (unsigned char x = 0; x < 8; x++) {
        	// Update DATA_PIN based on the bit value from data[i]
            if ((((unsigned char*)data)[i] >> x) & 0x01) {
                PORTA |= (1 << DATA_PIN);  // Set DATA_PIN high
            } else {
                PORTA &= ~(1 << DATA_PIN); // Set DATA_PIN low
            }
            
            PORTA |= (1 << CLOCK_PIN);  // Set CLOCK_PIN high
            _delay_us(1000000 / (2 * bit_rate));              
            
            PORTA &= ~(1 << CLOCK_PIN);  // Set CLOCK_PIN low
            _delay_us(1000000 / (2 * bit_rate));              
        }
        
        // wait for ACK pin high 
        while (!(PINA & (1 << ACK_PIN))) {}
    }
}

int main(void) {
    // Initialize PORTA for sending data (output mode)
    DDRA = (1 << CLOCK_PIN) | (1 << DATA_PIN); // Set CLOCK_PIN and DATA_PIN as output
    DDRA &= (1 << ACK_PIN); // Set ACK_PIN as input 
    
    unsigned char data[] = "halloweeks";
    
    transmit(data, sizeof(data) - 1);
    
    while (1) {}
    
    return 0;
}