#include <avr/io.h>
#include <util/delay.h>

void delay_us(uint16_t us) {
    while (us--) {
        _delay_us(1);
    }
}

void write_pwm(uint8_t pin, uint16_t pwmValue, uint16_t period) {
    // Map pwmValue (0-1023) to 1000-2000 microseconds
    uint16_t highTime = 1000 + ((pwmValue * 1000) / 1023); // Convert 0-1023 to 1000-2000 μs
    uint16_t lowTime = period - highTime;                  // Remaining time for low signal

    // Generate PWM signal
    PORTD |= (1 << pin);   // Set pin HIGH
    delay_us(highTime);    // Wait for high time
    PORTD &= ~(1 << pin);  // Set pin LOW
    delay_us(lowTime);     // Wait for low time
}

int main() {
    DDRD |= (1 << PD6);    // Set PD6 as output
    uint16_t pwmValue = 512; // Set a constant PWM value (midpoint, e.g., 512 for 1.5 ms pulse)

    while (1) {
        write_pwm(PD6, pwmValue, 20000);  // Generate PWM on PD6 with a period of 20 ms (50 Hz)
        // No change to pwmValue, it stays at 512
    }

    return 0; // This line is never reached
}


#include <avr/io.h>
#include <util/delay.h>

void delay_us(uint16_t us) {
	while (us--) {
		_delay_us(1);
	}
}

void write_pwm(uint8_t pin, uint8_t pwmValue, uint16_t period) {
    // Calculate high time and low time based on the pwmValue
    uint16_t highTime = (pwmValue * period) / 255; // Convert 0-255 to high time
    uint16_t lowTime = period - highTime;           // Remaining time for low time

    // Generate PWM signal
    PORTD |= (1 << pin);   // Set pin HIGH
    delay_us(highTime);    // Wait for high time
    PORTD &= ~(1 << pin);  // Set pin LOW
    delay_us(lowTime);     // Wait for low time
}

int main() {
	DDRD |= (1 << PD6);
	uint8_t value = 0;
	
    while (1) {
        write_pwm(PD6, value, 100); // Generate PWM with value 192 (75% duty cycle) on PORTD pin 0
        if (value != 255) {
        	value++;
        } else {
        	value = 0;
        }
        _delay_ms(50);        // Delay for 1 second
    }

    return 0; // This line is never reached
}