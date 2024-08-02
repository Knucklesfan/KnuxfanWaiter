#pragma once
#include "../gamemode.h"
#include "../utils/defs.h"
#include "../opengl/model.h"
#include "../opengl/skybox.h"
#include "../opengl/plane.h"
#include <json.hpp>

class weather : public Gamemode { //the main menu and screen saver
    public:
        void render();
        void input(SDL_Keycode keysym);
        Transition endLogic();
        void logic(double deltatime);
        void reset();
        weather();
    private:
        int backnum = 0;
        int age = 0;
        nlohmann::json weatherData;
        nlohmann::json forecastData;
        std::vector<std::string> days = {
            "SUN",
            "MON",
            "TUE",
            "WED",
            "THU",
            "FRI",
            "SAT"
        };

        model* text;
        model* peachhouse;
        skybox* sky;
        plane* p; // water plane
};