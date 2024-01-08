#ifndef UTILS_H
#define UTILS_H

#define BUZZER_PIN 3
#define SEVSEG_SS_PIN 10

void setSegmentNum(int num);
void playWinMelody();
void playLostMelodyAsync();
void playLostMelodyAsyncUpdate();
void playWinMelodyAsync();
void playWinMelodyAsyncUpdate();
void setupSeg();

#endif