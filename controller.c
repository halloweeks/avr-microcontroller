#include <stdio.h>

typedef struct {
    short x;
    short y;
    char b; // Button state
} Joystick;

typedef struct {
    Joystick joystick1;
    Joystick joystick2;
} ControllerData;

char OnChangeController(ControllerData current_data, ControllerData *prev_data) {
    if (current_data.joystick1.x != prev_data->joystick1.x ||
        current_data.joystick1.y != prev_data->joystick1.y ||
        current_data.joystick1.b != prev_data->joystick1.b ||
        current_data.joystick2.x != prev_data->joystick2.x ||
        current_data.joystick2.y != prev_data->joystick2.y ||
        current_data.joystick2.b != prev_data->joystick2.b) {
        
        // Update previous data
        *prev_data = current_data;
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
    ControllerData prev_data = {{0, 0, 0}, {0, 0, 0}};
    ControllerData curr_data;
    
    while (1) {
        // Here you would normally update curr_data with real joystick data
        // For demonstration, we'll use dummy data
        curr_data.joystick1.x = 1; // Update with actual function to read joystick 1 X
        curr_data.joystick1.y = 2; // Update with actual function to read joystick 1 Y
        curr_data.joystick1.b = 1; // Update with actual function to read joystick 1 button
        curr_data.joystick2.x = 3; // Update with actual function to read joystick 2 X
        curr_data.joystick2.y = 4; // Update with actual function to read joystick 2 Y
        curr_data.joystick2.b = 0; // Update with actual function to read joystick 2 button

        if (OnChangeController(curr_data, &prev_data)) {
            SendControllerData(curr_data);
        }
        
        // Add a delay or other logic to prevent continuous rapid execution
    }
    
    return 0;
}