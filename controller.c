#include <avr/io.h>
#include <util/delay.h>

typedef struct {
    short x;
    short y;
    char b; // Button state
} Joystick;

typedef struct {
    Joystick joystick1;
    Joystick joystick2;
} ControllerData;

static ControllerData prev_data = {{0, 0, 0}, {0, 0, 0}};

char OnChangeController(ControllerData current_data) {
    if (current_data.joystick1.x != prev_data.joystick1.x ||
        current_data.joystick1.y != prev_data.joystick1.y ||
        current_data.joystick1.b != prev_data.joystick1.b ||
        current_data.joystick2.x != prev_data.joystick2.x ||
        current_data.joystick2.y != prev_data.joystick2.y ||
        current_data.joystick2.b != prev_data.joystick2.b) {
        
        // Update previous data
        prev_data = current_data;
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
	ControllerData curr_data;
	
	while (1) {
		curr_data.joystick1.x = analog_read(A0);
		curr_data.joystick1.y = analog_read(A1);
		curr_data.joystick1.b = digital_read(0);
		curr_data.joystick2.x = analog_read(A2);
		curr_data.joystick2.y = analog_read(A3);
		curr_data.joystick2.b = digital_read(1);
		
		if (OnChangeController(curr_data)) {
			SendControllerData(curr_data);
		}
		
		_delay_ms(100);
	}
	
    return 0;
}