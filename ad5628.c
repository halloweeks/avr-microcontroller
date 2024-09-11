#include <avr/io.h> // AVR I/O library for GPIO control

#define bit(b) (1 << (b))

#define DATA_PIN PD2
#define CLOCK_PIN PD3

// Set all 8 channels with 000...
static unsigned short data[8] = {0};

void write_analog(unsigned char pin, short value) {
    if (value > 4095) value = 4095; // Clamp value to 12-bit range (0-4095)
    
    if (pin >= 8) return; // Correct range check (0-7 for 8 channels)
    
    data[pin] = value;
    
    // Send data for the specified pin
    for (unsigned char offset = 0; offset < 8; offset++) {
        // Check if the data should be high or low for each bit
        for (unsigned char index = 0; index < 12; index++) {
            if (data[offset] & bit(11 - index)) {
                PORTD |= (1 << DATA_PIN); // Set DATA_PIN high
            } else {
                PORTD &= ~(1 << DATA_PIN); // Set DATA_PIN low
            }
            
            // Pulse CLOCK_PIN to latch data
            PORTD |= (1 << CLOCK_PIN); // Set CLOCK_PIN high
            PORTD &= ~(1 << CLOCK_PIN); // Set CLOCK_PIN low
            
            // Optional: Add a small delay here if needed
        }
    }
}

int main() {
    // Configure DATA_PIN and CLOCK_PIN as outputs
    DDRD |= (1 << DATA_PIN) | (1 << CLOCK_PIN);
    
    short value = 2907; // Example DAC value
    
    write_analog(0, value); // Send value to channel 0
    
    // Optionally, loop or add other functionality
    
    return 0;
}