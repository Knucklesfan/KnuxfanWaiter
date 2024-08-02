#pragma once
#include "../gamemode.h"
#include "../utils/defs.h"
#include "../opengl/model.h"
class mainmenu : public Gamemode { //the main menu and screen saver
    public:
        void render();
        void input(SDL_Keycode keysym);
        Transition endLogic();
        void logic(double deltatime);
        void reset();
        mainmenu();
    private:
        int r = 128 + rand() % (256 - 128);
        int g = 128 + rand() % (256 - 128);
        int b = 128 + rand() % (256 - 128);
        int backnum = 0;
        int age = 0;
        float starX[255];
        float starY[255];
        float starZ[255];
        float starBlock[255];
        model* text;
        model* ourhouse;
};