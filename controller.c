#include <avr/io.h>
#include <util/delay.h>

typedef struct {
    short x; // Joystick X axis 
    short y; // Joystick Y axis 
    char b;  // Button state
} __attribute__((packed)) Joystick;

typedef struct {
    Joystick joystick1;
    Joystick joystick2;
} __attribute__((packed)) ControllerData;

static ControllerData previous_data = {{0, 0, 0}, {0, 0, 0}};

char OnChangeController(ControllerData current_data) {
    if (current_data.joystick1.x != previous_data.joystick1.x ||
        current_data.joystick1.y != previous_data.joystick1.y ||
        current_data.joystick1.b != previous_data.joystick1.b ||
        current_data.joystick2.x != previous_data.joystick2.x ||
        current_data.joystick2.y != previous_data.joystick2.y ||
        current_data.joystick2.b != previous_data.joystick2.b) {
        
        // Update previous data
        previous_data = current_data;
        return 1; // Indicates that there is a change
    }
    
    return 0; // No change
}

void SendControllerData(ControllerData data) {
    // Implement your data sending logic here
    // For example, printing to stdout:
    printf("Joystick 1 - X: %d, Y: %d, Button: %d\n",
           data.joystick1.x, data.joystick1.y, data.joystick1.b);
    printf("Joystick 2 - X: %d, Y: %d, Button: %d\n",
           data.joystick2.x, data.joystick2.y, data.joystick2.b);
}

int main() {
	ControllerData data;
	
	while (1) {
		data.joystick1.x = analog_read(A0);
		data.joystick1.y = analog_read(A1);
		data.joystick1.b = digital_read(0);
		data.joystick2.x = analog_read(A2);
		data.joystick2.y = analog_read(A3);
		data.joystick2.b = digital_read(1);
		
		if (OnChangeController(data)) {
			SendControllerData(data);
		}
	}
	
    return 0;
}