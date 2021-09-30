#include <AccelStepper.h>

const int btnForwardPin = 8;
const int btnStopPin = 9;
const int btnBackwardPin = 10;

AccelStepper motor(AccelStepper::HALF4WIRE, 3, 5, 4, 6);

const int MAX_SPEED = 1024;
const float SIDEREAL_SPEED = 20.5360723908529;
const int STEPS_PER_REVOLUTION = 4096;

void setup() {
  pinMode(btnForwardPin, INPUT_PULLUP);
  pinMode(btnBackwardPin, INPUT_PULLUP);
  pinMode(btnStopPin, INPUT_PULLUP);

  Serial.begin(9600);

  motor.setAcceleration(MAX_SPEED);
}

const int DIRECTION_CW = 1;
const int DIRECTION_CCW = -1;

int rotationDirection = DIRECTION_CW;

const float VELOCITIES[8] { 0, 0.7, 1, 2, 4, 8, 12, 28 };
const int VELOCITIES_FIRST_POSITION = 0;
const int VELOCITIES_LAST_POSITION = 7;
int velocitiesPosition = 0;

void loop() {
  if (checkBtnForward() ||
      checkBtnBackward() ||
      checkBtnStop());

  if (velocitiesPosition == VELOCITIES_FIRST_POSITION) {
    return;
  }

  motor.move(STEPS_PER_REVOLUTION * rotationDirection);
  motor.run();
}

const unsigned long BUTTON_CLICK_INTERVAL = 50;

int lastForwardButtonState = -1;
int currentForwardButtonState = -1;
int forwardButtonState = -1;
unsigned long lastTimeForwardButtonClick = 0;

bool checkBtnForward() {
  currentForwardButtonState = digitalRead(btnForwardPin);
  const unsigned long currentMillis = millis();

  if (currentForwardButtonState != lastForwardButtonState) {
    lastTimeForwardButtonClick = currentMillis;
  }

  if (currentMillis - lastTimeForwardButtonClick > BUTTON_CLICK_INTERVAL) {
    if (currentForwardButtonState != forwardButtonState) {
      forwardButtonState = currentForwardButtonState;
      if (forwardButtonState == LOW) {
        onClickBtnForward();
        return true;
      }
    }
  }

  lastForwardButtonState = currentForwardButtonState;

  return false;
}

int lastBackwardButtonState = -1;
int currentBackwardButtonState = -1;
int backwardButtonState = -1;
unsigned long lastTimeBackwardButtonClick = 0;

bool checkBtnBackward() {
  currentBackwardButtonState = digitalRead(btnBackwardPin);
  const unsigned long currentMillis = millis();

  if (currentBackwardButtonState != lastBackwardButtonState) {
    lastTimeBackwardButtonClick = currentMillis;
  }

  if (currentMillis - lastTimeBackwardButtonClick > BUTTON_CLICK_INTERVAL) {
    if (currentBackwardButtonState != backwardButtonState) {
      backwardButtonState = currentBackwardButtonState;
      if (backwardButtonState == LOW) {
        onClickBtnBackward();
        return true;
      }
    }
  }

  lastBackwardButtonState = currentBackwardButtonState;

  return false;
}

int lastStopButtonState = -1;
int currentStopButtonState = -1;
int stopButtonState = -1;
unsigned long lastTimeStopButtonClick = 0;

bool checkBtnStop() {
  currentStopButtonState = digitalRead(btnStopPin);
  const unsigned long currentMillis = millis();

  if (currentStopButtonState != lastStopButtonState) {
    lastTimeStopButtonClick = currentMillis;
  }

  if (currentMillis - lastTimeStopButtonClick > BUTTON_CLICK_INTERVAL) {
    if (currentStopButtonState != stopButtonState) {
      stopButtonState = currentStopButtonState;
      if (stopButtonState == LOW) {
        onClickBtnStop();
        return true;
      }
    }
  }

  lastStopButtonState = currentStopButtonState;

  return false;
}

void onClickBtnForward() {
  Serial.println("onClickBtnForward()");
  clickedOnDirectionButton(true);
}

void onClickBtnBackward() {
  Serial.println("onClickBtnBackward()");
  clickedOnDirectionButton(false);
}

void clickedOnDirectionButton(const bool isForward) {
  if (VELOCITIES_FIRST_POSITION == velocitiesPosition) {
    increaseVelocitiesPosition();
    rotationDirection = isForward ? DIRECTION_CW : DIRECTION_CCW;
  } else {
    if (DIRECTION_CW == rotationDirection) {
      if (isForward) {
        increaseVelocitiesPosition();
      } else {
        decreaseVelocitiesPosition();
      }
    } else {
      if (isForward) {
        decreaseVelocitiesPosition();
      } else {
        increaseVelocitiesPosition();
      }
    }
  }

  motor.setMaxSpeed(SIDEREAL_SPEED * VELOCITIES[velocitiesPosition]);
}

void increaseVelocitiesPosition() {
  if (velocitiesPosition < VELOCITIES_LAST_POSITION) {
    velocitiesPosition++;
  }
}

void decreaseVelocitiesPosition() {
  if (velocitiesPosition > VELOCITIES_FIRST_POSITION) {
    velocitiesPosition--;
  }
}

void onClickBtnStop() {
  Serial.println("onClickBtnStop()");
  velocitiesPosition = VELOCITIES_FIRST_POSITION;
  motor.stop();
}
