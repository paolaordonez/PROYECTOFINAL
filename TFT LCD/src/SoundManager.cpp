#include "SoundManager.h"
#include "pitches.h"
#include <Arduino.h>
#define Buzzer 8

void SoundManager::reproducirVictoria() {
    
    tone(Buzzer, NOTE_C5); delay(160);
    tone(Buzzer, NOTE_E5); delay(140);
    tone(Buzzer, NOTE_G5); delay(160);
    tone(Buzzer, NOTE_E5); delay(130);
    tone(Buzzer, NOTE_G5); delay(180);
    tone(Buzzer, NOTE_C6); delay(230);
    tone(Buzzer, NOTE_E6); delay(350);
    tone(Buzzer, NOTE_C6); delay(350);
    noTone(Buzzer);
}

void SoundManager::reproducirDerrota() {

    tone(Buzzer, NOTE_DS5); delay(300);
    tone(Buzzer, NOTE_D5); delay(300);
    tone(Buzzer, NOTE_CS5); delay(300);
    for (byte i = 0; i < 10; i++) {
        for (int pitch = -10; pitch <= 10; pitch++) {
            tone(Buzzer, NOTE_C5 + pitch);
            delay(5);
        }
    }
    noTone(Buzzer);
    delay(500);
}