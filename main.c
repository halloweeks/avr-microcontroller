#define _SFR_IO32(io_addr) (*(volatile unsigned int*)(io_addr))

#define GPIOA_BASE (0x50000000)
#define GPIOA_MODER _SFR_IO32(GPIOA_BASE + 0x00)
#define GPIOA_ODR _SFR_IO32(GPIOA_BASE + 0x14)

#define bit(b) (1 << (b))

void delay_us(unsigned int us) {
	while (us--) {
		__asm__ volatile ("nop");
	}
}

int main() {
	// Set PA0 as output (MODER[1:0] = 01 for PA0)
	GPIOA_MODER &= ~(0x03 << (0 * 2)); // Clear bits for PA0 (bits 0 and 1)
	GPIOA_MODER |= (0x01 << (0 * 2));  // Set bits 0 and 1 to 01 (output mode)
	
	// Set PA0 high
	// GPIOA_ODR |= bit(0); // Set pin PA0 high 
	
	while (1) {
		GPIOA_ODR ^= bit(0);
		delay(1000000);
	}
	
	return 0;
}