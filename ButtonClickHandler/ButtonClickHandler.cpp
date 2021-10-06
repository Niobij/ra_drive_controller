#include "Arduino.h"
#include "ButtonClickHandler.h"

const unsigned long BUTTON_CLICK_INTERVAL = 50;

ButtonClickHandler::ButtonClickHandler(const int buttonPin, const void (*onClick)()) {
  _buttonPin = buttonPin;
  _onClick = onClick;
}

bool ButtonClickHandler::checkButton() {
	_currentButtonState = digitalRead(_buttonPin);
	const unsigned long currentMillis = millis();
	
	if (_currentButtonState != _lastButtonState) {
		lastTimeButtonClick = currentMillis;
	}
	
	if (currentMillis - lastTimeButtonClick > BUTTON_CLICK_INTERVAL) {
		if (_currentButtonState != _buttonState) {
			_buttonState = _currentButtonState;
			if (_buttonState == LOW) {
				_onClick();
				return true;
			}
		}
	}
	
	_lastButtonState = _currentButtonState;
	
	return false;
}
