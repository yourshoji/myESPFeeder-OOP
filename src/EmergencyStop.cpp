#include "EmergencyStop.h"
#include "config.h"

// This is the constructor, the initializer function that sets up our object
EmergencyStop::EmergencyStop(int pin, Servo& servo) : _pin(pin), _servo(servo) {
    // before running anything, we set up our internal variables (pin to _pin, servo to _servo)

    _previousState = false; // assuming E-STOP is not pressed at start
    _currentState = false;  // assuming E-STOP is not pressed at start
}

// This is the E-STOP function definition (:: means "belongs to")
void EmergencyStop::E_STOP(bool state) {
    if (state != _previousState) {
        _previousState = state;

        if (state) {
            _servo.detach();
            DEBUG_PRINTLN("E-STOP IS ACTIVATED!");
        }
        else {
            _servo.attach(_pin);
            DEBUG_PRINTLN("SERVO IS BACK ONLINE!");
        }
    }

    _currentState = state;
}

