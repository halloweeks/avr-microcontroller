#include <avr/io.h>
#include <util/delay.h>

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

void adc_init() {
    // Set the ADC reference voltage to AVcc with external capacitor at AREF pin
    ADMUX = (1 << REFS0);
    
    // Set the ADC clock prescaler to 128 (16MHz/128 = 125kHz ADC clock)
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    
    // Enable the ADC
    ADCSRA |= (1 << ADEN);
}

unsigned short analog_read(unsigned char channel) {
    // Select ADC channel (0-7)
    ADMUX = (ADMUX & 0xF8) | (channel & 0x07);
    
    // Start the conversion
    ADCSRA |= (1 << ADSC);
    
    // Wait for the conversion to complete
    while (ADCSRA & (1 << ADSC));
    
    // Read and return the ADC result
    return ADC;
}

int main() {
    adc_init();
    
    UART_init(9600); // Initialize UART with 9600 baud
    
    char buffer[10];
    unsigned short value = 0;
    
    while (1) {
        value = analog_read(0);  // Read analog value from channel 0
        snprintf(buffer, sizeof(buffer), "%u\n", adc_value);
        UART_print(buffer);
        
        // Use the adc_value for further processing
    }
}