#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

// UART initialization function
void UART_init(unsigned int baud) {
    unsigned int ubrr = F_CPU / 16 / baud - 1;
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << TXEN0);  // Enable transmitter
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  // 8-bit data
}

// UART transmit function
void UART_transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0)));  // Wait for empty transmit buffer
    UDR0 = data;  // Send data
}

// Function to send a string over UART
void UART_print(const char *str) {
    while (*str) {
        UART_transmit(*str++);
    }
}
// Initialize the ADC
void ADC_init() {
    ADMUX = (1 << REFS0);           // Reference voltage AVcc with external capacitor at AREF pin, left adjust ADC result
    ADCSRA = (1 << ADEN) |          // Enable ADC
             (1 << ADPS2) |         // ADC prescaler 16 (ADPS2:0)
             (1 << ADPS1);
}

// Read an ADC value from a given channel
uint16_t ADC_read(uint8_t channel) {
    ADMUX = (ADMUX & 0xF8) | (channel & 0x07); // Mask the channel to 3 bits and select it
    ADCSRA |= (1 << ADSC);                    // Start the conversion
    
    // Wait for conversion to complete
    while (ADCSRA & (1 << ADSC));
    
    // Read the ADC value (10-bit result)
    return ADC;
}

int main(void) {
    ADC_init(); // Initialize ADC
    
    UART_init(9600); // Initialize UART with 9600 baud
    
    char buffer[10];
    
    while (1) {
        uint16_t adc_value = ADC_read(0); // Read from ADC channel 0
        snprintf(buffer, sizeof(buffer), "%u\n", adc_value);
        UART_print(buffer);
        
        // Do something with adc_value
    }
}