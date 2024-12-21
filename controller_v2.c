#include <avr/io.h>
#include <util/delay.h>

#include <stdbool.h>

typedef struct {
	short x; // joystick X axis 
	short y; // joystick Y axis 
	char b;  // joystick push button state
} __attribute__((packed)) Joystick;

typedef struct {
	Joystick joystick1;
	Joystick joystick2;
} __attribute__((packed)) Controller;

Controller previous_data = {{0, 0, 0}, {0, 0, 0}};

void ReadControllerInput(Controller *data) {
	data.joystick1.x = ANALOG_READ(0);
	data.joystick1.y = ANALOG_READ(1);
	data.joystick1.b = DIGITAL_READ(2);
	data.joystick2.x = ANALOG_READ(3);
	data.joystick2.y = ANALOG_READ(4);
	data.joystick2.b = DIGITAL_READ(5);
}

bool OnChangeController(Controller current_data) {
	if (current_data.joystick1.x != previous_data.joystick1.x || 
		current_data.joystick1.y != previous_data.joystick1.y || 
		current_data.joystick1.b != previous_data.joystick1.b || 
		current_data.joystick2.x != previous_data.joystick2.x || 
		current_data.joystick2.y != previous_data.joystick2.y || 
		current_data.joystick2.b != previous_data.joystick2.b) {
			
		previous_data = current_data;
		return true;
	}
	return false;
}

void SendControllerData(Controller data) {
	printf("Joystick 1 - X: %d, Y: %d, Button: %d\n", data.joystick1.x, data.joystick1.y, data.joystick1.b);
	printf("Joystick 2 - X: %d, Y: %d, Button: %d\n", data.joystick2.x, data.joystick2.y, data.joystick2.b);
}

int main() {
	Controller data;
	
	while (1) {
		ReadControllerInput(&data);
		
		if (OnChangeController(data)) {
			SendControllerData(data);
		}
	}
	
    return 0;
}