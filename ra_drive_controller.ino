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

  //  Serial.begin(9600);

  motor.setAcceleration(MAX_SPEED);
}

const int DIRECTION_CW = 1;
const int DIRECTION_CCW = -1;

int rotationDirection = DIRECTION_CW;

const float VELOCITIES[8] { 0, 0.7, 1, 2, 4, 8, 12, 28 };
const int VELOCITIES_FIRST_POSITION = 0;
const int VELOCITIES_LAST_POSITION = 7;
int velocitiesPosition = 0;

int btnValue = -1;
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

  if (velocitiesPosition == VELOCITIES_FIRST_POSITION) {
    return;
  }

  motor.move(STEPS_PER_REVOLUTION * rotationDirection);
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
  const unsigned long currentMillis = millis();
  if (currentMillis - lastTimeButtonClick > BUTTON_CLICK_INTERVAL) {
    lastTimeButtonClick = currentMillis;
    velocitiesPosition = VELOCITIES_FIRST_POSITION;
    motor.stop();
  }
}
