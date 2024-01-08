#include <ButtonDebounce.h>

#include "LedControl.h"
#include "utils.h"

#define STOP_BTN_PIN 2
#define NEXT_LEVEL_BTN_PIN 4
#define INITIAL_INTERVAL 600

LedControl lc = LedControl(8, 7, 9, 1);

unsigned long gameIterationMs;
unsigned long stopBtnMs;

int level = 1;
int gameInterval = INITIAL_INTERVAL;
bool isPlayingLevel = true;

int activeNumber;
int activeLevelNumber;
int activeLevelStep = 1;

ButtonDebounce nextLevelButton(NEXT_LEVEL_BTN_PIN, 250);

void setup() {
  Serial.begin(9600);

  pinMode(STOP_BTN_PIN, INPUT_PULLUP);
  pinMode(NEXT_LEVEL_BTN_PIN, INPUT_PULLUP);
  setupSeg();

  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);

  setSegmentNum(level);
  nextLevelButton.setCallback(nextBtnHandler);
  attachInterrupt(digitalPinToInterrupt(STOP_BTN_PIN), stopBtnHandler, FALLING);

  Serial.println("Start");
}

void loop() {
  playLostMelodyAsyncUpdate();
  playWinMelodyAsyncUpdate();
  if (isPlayingLevel && (millis() > gameIterationMs + gameInterval)) {
    handleGameIteration();
    gameIterationMs = millis();
  }
  nextLevelButton.update();
}

void onButtonChange(const int state) {
  Serial.println("Changed: " + String(state));
}

void stopBtnHandler() {
  Serial.println("stop button");
  if (!isPlayingLevel || millis() - 300 < stopBtnMs) {
    return;
  }

  stopBtnMs = millis();

  if (activeLevelStep == 1) {
    activeLevelStep = 2;
    activeLevelNumber = activeNumber;
  } else {
    if (activeNumber == activeLevelNumber) {
      if (activeLevelStep == 4) {
        startNextLevel();
      } else {
        activeLevelStep += 1;
      }
    } else {
      looseGame();
    }
  }
}

void nextBtnHandler(const int state) {
  Serial.println("next button");
  if (isPlayingLevel || state == 0) {
    return;
  }

  startNextLevel();
}

void startNextLevel() {
  if (activeLevelStep != 4) {
    // level = 1;
    gameInterval = INITIAL_INTERVAL;
  } else {
    if (level == 9) {
      level = 1;
      gameInterval = INITIAL_INTERVAL;
      playWinMelodyAsync();
    } else {
      level += 1;
      gameInterval -= 50;
    }
  }

  lc.clearDisplay(0);
  setSegmentNum(level);
  activeLevelStep = 1;
  isPlayingLevel = true;
}

void handleGameIteration() {
  int r = random(9);
  while (r == activeNumber) {
    r = random(9);
  }
  activeNumber = r;
  lc.setDigit(0, activeLevelStep - 1, activeNumber, false); 
}

void looseGame() {
  playLostMelodyAsync();
  isPlayingLevel = false;
}