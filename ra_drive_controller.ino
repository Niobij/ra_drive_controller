#include <Stepper.h>

const int btnForwardPin = 8;
const int btnStopPin = 9;
const int btnBackwardPin = 10;

Stepper myStepper(4096, 3, 5, 4, 6);

void setup() {
  pinMode(btnForwardPin, INPUT_PULLUP);
  pinMode(btnBackwardPin, INPUT_PULLUP);
  pinMode(btnStopPin, INPUT_PULLUP);

//  Serial.begin(9600);

//  myStepper.setSpeed(15);
}

const unsigned long MICROS_INTERVAL = 48695;

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
  
  const unsigned long currentMicros = micros();
  if (currentMicros - lastMicros >= (velocity <= VELOCITY_1 ? MICROS_INTERVAL : MICROS_INTERVAL / velocity)) {
    lastMicros = currentMicros;
    myStepper.step(rotationDirection);
  }
}

void onClickBtnForward() {
  const unsigned long currentMillis = millis();
  if (currentMillis - lastTimeButtonClick > BUTTON_CLICK_INTERVAL) {
    lastTimeButtonClick = currentMillis;
    Serial.println("onClickBtnForward()");
    clickedOnDirectionButton(true);
  }
}

void onClickBtnBackward() {
  const unsigned long currentMillis = millis();
  if (currentMillis - lastTimeButtonClick > BUTTON_CLICK_INTERVAL) {
    lastTimeButtonClick = currentMillis;
    Serial.println("onClickBtnBackward()");
    clickedOnDirectionButton(false);
  }
}

void clickedOnDirectionButton(const bool isForward) {
  switch (velocity) {
    case VELOCITY_0:
      Serial.println("VELOCITY_0 start");
      Serial.println("velocity=" + String(velocity));
      Serial.println("rotationDirection=" + String(rotationDirection));
      velocity = VELOCITY_1;
      rotationDirection = isForward ? DIRECTION_CW : DIRECTION_CCW;
      Serial.println("VELOCITY_0 middle");
      Serial.println("velocity=" + String(velocity));
      Serial.println("rotationDirection=" + String(rotationDirection));
      Serial.println("VELOCITY_0 end");
      break;
      
    case VELOCITY_1:
      Serial.println("VELOCITY_1 start");
      Serial.println("velocity=" + String(velocity));
      Serial.println("rotationDirection=" + String(rotationDirection));
      if (rotationDirection == (isForward ? DIRECTION_CCW : DIRECTION_CW)) {
        velocity = VELOCITY_0;
      } else {
        velocity = VELOCITY_2;
      }
      Serial.println("VELOCITY_1 middle");
      Serial.println("velocity=" + String(velocity));
      Serial.println("rotationDirection=" + String(rotationDirection));
      Serial.println("VELOCITY_1 end");
      break;
      
    case VELOCITY_2:
      Serial.println("VELOCITY_2 start");
      Serial.println("velocity=" + String(velocity));
      Serial.println("rotationDirection=" + String(rotationDirection));
      if (rotationDirection == (isForward ? DIRECTION_CCW : DIRECTION_CW)) {
        velocity = VELOCITY_1;
      } else {
        velocity = VELOCITY_3;
      }
      Serial.println("VELOCITY_2 middle");
      Serial.println("velocity=" + String(velocity));
      Serial.println("rotationDirection=" + String(rotationDirection));
      Serial.println("VELOCITY_2 end");
      break;
      
    case VELOCITY_3:
      Serial.println("VELOCITY_3 start");
      Serial.println("velocity=" + String(velocity));
      Serial.println("rotationDirection=" + String(rotationDirection));
      if (rotationDirection == (isForward ? DIRECTION_CCW : DIRECTION_CW)) {
        velocity = VELOCITY_2;
      } else {
        velocity = VELOCITY_4;
      }
      Serial.println("VELOCITY_3 middle");
      Serial.println("velocity=" + String(velocity));
      Serial.println("rotationDirection=" + String(rotationDirection));
      Serial.println("VELOCITY_3 end");
      break;
      
    case VELOCITY_4:
      Serial.println("VELOCITY_4 start");
      Serial.println("velocity=" + String(velocity));
      Serial.println("rotationDirection=" + String(rotationDirection));
      if (rotationDirection == (isForward ? DIRECTION_CCW : DIRECTION_CW)) {
        velocity = VELOCITY_3;
      } else {
        velocity = VELOCITY_5;
      }
      Serial.println("VELOCITY_4 middle");
      Serial.println("velocity=" + String(velocity));
      Serial.println("rotationDirection=" + String(rotationDirection));
      Serial.println("VELOCITY_4 end");
      break;
      
    case VELOCITY_5:
      Serial.println("VELOCITY_5 start");
      Serial.println("velocity=" + String(velocity));
      Serial.println("rotationDirection=" + String(rotationDirection));
      if (rotationDirection == (isForward ? DIRECTION_CCW : DIRECTION_CW)) {
        velocity = VELOCITY_4;
      }
      Serial.println("VELOCITY_5 middle");
      Serial.println("velocity=" + String(velocity));
      Serial.println("rotationDirection=" + String(rotationDirection));
      Serial.println("VELOCITY_5 end");
      break;
  }
}

void onClickBtnStop() {
  const unsigned long currentMillis = millis();
  if (currentMillis - lastTimeButtonClick > BUTTON_CLICK_INTERVAL) {
    lastTimeButtonClick = currentMillis;
    Serial.println("onClickBtnStop()");
    velocity = VELOCITY_0;
  }
}
