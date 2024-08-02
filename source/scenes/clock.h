#pragma once
#include "../gamemode.h"
#include "../utils/defs.h"
#include "../opengl/model.h"
#include "../opengl/skybox.h"
#include "../opengl/plane.h"
#include <json.hpp>

class clockscene : public Gamemode { //the main menu and screen saver
    public:
        void render();
        void input(SDL_Keycode keysym);
        Transition endLogic();
        void logic(double deltatime);
        void reset();
        clockscene();
    private:
        int backnum = 0;
        int age = 0;
    };