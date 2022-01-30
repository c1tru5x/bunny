//
// Created by maxia on 07/02/2021.
//

#ifndef BUNNY_SOUND_H
#define BUNNY_SOUND_H
#include <Windows.h>

    class sound {

    public:
        static void playSound(bool state);

        static void onSound();

        static void offSound();
    };


#endif //BUNNY_SOUND_H
