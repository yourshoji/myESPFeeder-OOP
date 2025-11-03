#include <Arduino.h>
#include "Feeder.h"
// #include "config.h"

FeedManager::FeedManager(Servo& servo, int defaultPos, int buzzerPin, int ledPin, int servoPin)
    : _servo(servo), _defaultPos(defaultPos), _buzzerPin(buzzerPin), _ledPin(ledPin), _servoPin(servoPin), _active(false) {
    pinMode(_buzzerPin, OUTPUT);
    pinMode(_ledPin, OUTPUT);
    // Attach the servo using the configured pin number (was incorrectly passing the Servo object)
    _servo.attach(_servoPin);
    _servo.write(_defaultPos);

