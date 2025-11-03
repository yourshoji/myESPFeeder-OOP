#pragma once
#include <ESP32Servo.h>

class EmergencyStop {
private:
    // the stuff that only the object can see and use
    
    // variables that will be used internally
    // giving them "_" prefix to indicate they are private
    Servo& _servo; // (nicknamed "_servo");
    int _pin;
    bool _previousState;
    bool _currentState;

public:
    // main.cpp can see and use these
    // the blueprint with the specified pin and servo (bedroom, restroom, etc.)
    EmergencyStop(int pin, Servo& servo); // Constructor

    void E_STOP(bool state); // a declaration of the E-STOP function (declaration only)
};