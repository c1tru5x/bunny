//
// Created by maxia on 07/02/2021.
//

#include "sound.h"

    void sound::playSound(const bool state) {
        if (!state) {
            onSound();
        } else {
            offSound();
        }
    }

    void sound::onSound() {
        Beep(250, 200);
    }

    void sound::offSound() {
        Beep(400, 200);
    }