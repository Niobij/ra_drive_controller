#include <ButtonClickHandler.h>
#include <AccelStepper.h>

const int MAX_SPEED = 1024;
const float SIDEREAL_SPEED = 20.5360723908529;
const int STEPS_PER_REVOLUTION = 4096;

const int BTN_FORWARD_PIN = 8;
const int BTN_STOP_PIN = 9;
const int BTN_BACKWARD_PIN = 10;

const AccelStepper motor(AccelStepper::HALF4WIRE, 3, 5, 4, 6);

void setup() {
  pinMode(BTN_FORWARD_PIN, INPUT_PULLUP);
  pinMode(BTN_BACKWARD_PIN, INPUT_PULLUP);
  pinMode(BTN_STOP_PIN, INPUT_PULLUP);

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

const ButtonClickHandler buttonForwardHandler(BTN_FORWARD_PIN, onClickBtnForward);
const ButtonClickHandler buttonBackwardHandler(BTN_BACKWARD_PIN, onClickBtnBackward);
const ButtonClickHandler buttonStopHandler(BTN_STOP_PIN, onClickBtnStop);

void loop() {
  if (buttonForwardHandler.checkButton() ||
      buttonBackwardHandler.checkButton() ||
      buttonStopHandler.checkButton());

  if (velocitiesPosition == VELOCITIES_FIRST_POSITION) {
    return;
  }

  motor.move(STEPS_PER_REVOLUTION * rotationDirection);
  motor.run();
}

void onClickBtnForward() {
  Serial.println("onClickBtnForward()");
  clickedOnDirectionButton(true);
}

void onClickBtnBackward() {
  Serial.println("onClickBtnBackward()");
  clickedOnDirectionButton(false);
}

void onClickBtnStop() {
  Serial.println("onClickBtnStop()");
  velocitiesPosition = VELOCITIES_FIRST_POSITION;
  motor.stop();
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
