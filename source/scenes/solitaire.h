#pragma once
#include "../gamemode.h"
#include "../utils/defs.h"
#include "../opengl/model.h"
#include "../opengl/skybox.h"
#include "../opengl/plane.h"
#include <json.hpp>
class solitaireBlock {
    public:
        void logic();
        std::string currsprite = "bblock";
        int x = 0;
        int y = 0;
        int lifetime = 0;
        solitaireBlock(std::string sprite) {
            lifetime = SDL_GetTicks64();
            currsprite = sprite;
            x=rand()%(SCREEN_WIDTH-graphics::sprites.at(currsprite)->w);
            y=0;
            xvel=10+rand()%50;
            yvel=5+rand()%100;


        }
    private:
        double xvel = 25; //constant velocity of 5, positive is right, negative is left
        double yvel = 50; //negative is up, positive is down
};
class solitairescene : public Gamemode { //the main menu and screen saver
    public:
        void render();
        void input(SDL_Keycode keysym);
        Transition endLogic();
        void logic(double deltatime);
        void reset();
        solitairescene();
    private:
        std::vector<solitaireBlock> blocks;
        buffermanager* buff;
        int age = 0;
        std::string icons[7] = {
            "bblock",
            "pblock",
            "logos_1",
            "logos_2",
            "logos_3",
            "logos_4",
            "logos_5",
        };
    };