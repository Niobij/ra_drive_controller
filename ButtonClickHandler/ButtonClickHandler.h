#ifndef ButtonClickHandler_h
#define ButtonClickHandler_h

#include "Arduino.h"

class ButtonClickHandler {
  public:
    ButtonClickHandler(const int buttonPin, const void (*onClick)());
	bool checkButton();
  private:
    int _buttonPin = -1;
	void (*_onClick)();
	
	int _lastButtonState = -1;
	int _currentButtonState = -1;
	int _buttonState = -1;
	unsigned long lastTimeButtonClick = 0;
};

#endif