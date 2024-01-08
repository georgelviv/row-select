#include "utils.h"
#include "pitches.h"
#include <Arduino.h>
#include <SPI.h>

bool shouldPlayLostMelody = false;
bool shouldPlayWinMelody = false;

void setupSeg() {
  SPI.begin();
  pinMode(SEVSEG_SS_PIN, OUTPUT);
}

void playMelody(int melody[], int noteDurations[], int melodySize) {
  for (int thisNote = 0; thisNote < melodySize; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER_PIN, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZER_PIN);
  }
}

void setSegmentNum(int num) {
  int datArray[16] = {
    B00111111, // 0
    B00000110, // 1
    B01011011, // 2
    B01001111, // 3
    B01100110, // 4
    B01101101, // 5
    B01111101, // 6
    B00000111, // 7
    B01111111, // 8
    B01101111, // 9
  };
  digitalWrite(SEVSEG_SS_PIN, LOW);
  SPI.transfer(datArray[num]);
  digitalWrite(SEVSEG_SS_PIN, HIGH);
}

void playWinMelody() {
  int melody[] = {
    NOTE_A5, NOTE_B5, NOTE_C5, NOTE_B5, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_D5,
    NOTE_E5, NOTE_D5, NOTE_E5, NOTE_E5
  };
  int noteDurations[] = {
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
  };

  playMelody(melody, noteDurations, 12);
}

void playLostMelody() {
  tone(BUZZER_PIN, NOTE_G4);
  delay(250);
  tone(BUZZER_PIN, NOTE_C4);
  delay(500);
  noTone(BUZZER_PIN);
}

void playLostMelodyAsync() {
  shouldPlayLostMelody = true;
}

void playLostMelodyAsyncUpdate() {
  if (shouldPlayLostMelody) {
    playLostMelody();
    shouldPlayLostMelody = false;
  }
}

void playWinMelodyAsync() {
  shouldPlayWinMelody = true;
}

void playWinMelodyAsyncUpdate() {
  if (shouldPlayWinMelody) {
    playWinMelody();
    shouldPlayWinMelody = false;
  }
}

