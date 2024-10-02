# avr-microcontroller
This code is not tested in AVR microcontroller maybe error or bugs

```c
void _jump(void* address) {
    __asm__ volatile("br %0" : : "r"(address));
}

void _nop() {
	__asm__ volatile("nop");
}

void _load(void* src, void *dst, unsigned int size) {
	for (unsigned int i = 0; i < size; i++) {
		((unsigned char*)dst)[i] = ((unsigned char*)src)[i];
	}
}

// Jump to the application
void (*app_start)(void) = (void (*)(void))APPLICATION_START;
app_start(); // Jump to application


void (*application)(void) = (void*)0x50000075;
application();
```

```sh
avr-g++ -mmcu=atmega328p -DF_CPU=16000000UL -Os main.c -o main.elf

# Convert ELF to HEX
avr-objcopy -O ihex main.elf main.hex

# Convert ELF to binary 
avr-objcopy -O binary main.elf main.bin

avr-objcopy -O binary -R .eeprom main.elf main.bin

# Upload HEX to the microcontroller
avrdude -c usbasp -p atmega328p -U flash:w:main.hex:i

# Upload Bin to the microcontroller 
avrdude -c usbasp -p atmega328p -U flash:w:main.bin
```
