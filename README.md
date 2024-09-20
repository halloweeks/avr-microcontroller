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
    
```
