#include <AccelStepper.h>

const int btnForwardPin = 8;
const int btnStopPin = 9;
const int btnBackwardPin = 10;

AccelStepper motor(AccelStepper::HALF4WIRE, 3, 5, 4, 6);

const float maxSpeed = 1024;
const float siderealSpeed = 20.5360723908529;
const float stepsPerRevolution = 4096;

void setup() {
  pinMode(btnForwardPin, INPUT_PULLUP);
  pinMode(btnBackwardPin, INPUT_PULLUP);
  pinMode(btnStopPin, INPUT_PULLUP);

  //  Serial.begin(9600);
  
  motor.setAcceleration(maxSpeed);
}

const int DIRECTION_CW = 1;
const int DIRECTION_CCW = -1;

const unsigned int VELOCITY_0 = 0;
const unsigned int VELOCITY_1 = 1;
const unsigned int VELOCITY_2 = 4;
const unsigned int VELOCITY_3 = 8;
const unsigned int VELOCITY_4 = 12;
const unsigned int VELOCITY_5 = 28;

int btnValue = -1;
int rotationDirection = DIRECTION_CW;
unsigned int velocity = VELOCITY_0;
unsigned long lastMicros = 0;

const unsigned long BUTTON_CLICK_INTERVAL = 300;
unsigned long lastTimeButtonClick = 0;

void loop() {
  btnValue = digitalRead(btnForwardPin);
  if (btnValue == LOW) {
    onClickBtnForward();
  }

  btnValue = digitalRead(btnBackwardPin);
  if (btnValue == LOW) {
    onClickBtnBackward();
  }

  btnValue = digitalRead(btnStopPin);
  if (btnValue == LOW) {
    onClickBtnStop();
  }

  if (velocity == VELOCITY_0) {
    return;
  }

  motor.move(stepsPerRevolution * rotationDirection);
  motor.run();
}

void onClickBtnForward() {
  const unsigned long currentMillis = millis();
  if (currentMillis - lastTimeButtonClick > BUTTON_CLICK_INTERVAL) {
    lastTimeButtonClick = currentMillis;
    clickedOnDirectionButton(true);
  }
}

void onClickBtnBackward() {
  const unsigned long currentMillis = millis();
  if (currentMillis - lastTimeButtonClick > BUTTON_CLICK_INTERVAL) {
    lastTimeButtonClick = currentMillis;
    clickedOnDirectionButton(false);
  }
}

void clickedOnDirectionButton(const bool isForward) {
  switch (velocity) {
    case VELOCITY_0:
      velocity = VELOCITY_1;
      rotationDirection = isForward ? DIRECTION_CW : DIRECTION_CCW;
      break;

    case VELOCITY_1:
      if (rotationDirection == (isForward ? DIRECTION_CCW : DIRECTION_CW)) {
        velocity = VELOCITY_0;
      } else {
        velocity = VELOCITY_2;
      }
      break;

    case VELOCITY_2:
      if (rotationDirection == (isForward ? DIRECTION_CCW : DIRECTION_CW)) {
        velocity = VELOCITY_1;
      } else {
        velocity = VELOCITY_3;
      }
      break;

    case VELOCITY_3:
      if (rotationDirection == (isForward ? DIRECTION_CCW : DIRECTION_CW)) {
        velocity = VELOCITY_2;
      } else {
        velocity = VELOCITY_4;
      }
      break;

    case VELOCITY_4:
      if (rotationDirection == (isForward ? DIRECTION_CCW : DIRECTION_CW)) {
        velocity = VELOCITY_3;
      } else {
        velocity = VELOCITY_5;
      }
      break;

    case VELOCITY_5:
      if (rotationDirection == (isForward ? DIRECTION_CCW : DIRECTION_CW)) {
        velocity = VELOCITY_4;
      }
      break;
  }

  motor.setMaxSpeed(siderealSpeed * velocity);
}

void onClickBtnStop() {
  const unsigned long currentMillis = millis();
  if (currentMillis - lastTimeButtonClick > BUTTON_CLICK_INTERVAL) {
    lastTimeButtonClick = currentMillis;
    velocity = VELOCITY_0;
    motor.stop();
  }
}
