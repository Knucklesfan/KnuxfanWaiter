#include "solitaire.h"
#include "../pixfont.h"
#include <ctime>
void solitaireBlock::logic() {
    x += xvel*(graphics::deltaTime*0.01);
    y += yvel*(graphics::deltaTime*0.01);
    if(y > SCREEN_HEIGHT-graphics::sprites.at(currsprite)->h) { 
        yvel = -yvel/1.5;
        y = SCREEN_HEIGHT-graphics::sprites.at(currsprite)->h;
    }
    if(x > SCREEN_WIDTH-graphics::sprites.at(currsprite)->w || x < 0) {
        xvel = -xvel;
    }
    yvel += 0.8; //constant gravity
    std::cout << x << " " << y << " " << " " << xvel << " " << yvel << "\n";
}
solitairescene::solitairescene() {
    buff=new buffermanager(SCREEN_WIDTH,SCREEN_HEIGHT,false);
    blocks.push_back(solitaireBlock("bblock"));
};
void solitairescene::render() {
    buff->enable(false);
    for(int i = 0; i < blocks.size(); i++) {
        texture* curr = graphics::sprites.at(blocks.at(i).currsprite);
        graphics::sprite->render(graphics::shaders.at(4),curr,{blocks.at(i).x,blocks.at(i).y},{curr->w,curr->h},0,{0,0},{curr->w,curr->h});
    }
    buff->disable(SCREEN_WIDTH,SCREEN_HEIGHT,true);
    graphics::sprite->render(graphics::shaders.at(4),&buff->renderTexture,{0,0},{SCREEN_WIDTH,SCREEN_HEIGHT},0,{0,0},{SCREEN_WIDTH,SCREEN_HEIGHT});
};
void solitairescene::logic(double deltatime) {
    for(int i = 0; i < blocks.size(); i++) {
        blocks.at(i).logic();
        if(SDL_GetTicks64()-blocks.at(i).lifetime > 5000) {
            int icon = rand()%7;
            if(icons[icon] == blocks.at(i).currsprite) {
                icon = rand()%7; //roll again
            } //if we get dupes again, then we just cry ourselves to sleep
            blocks.erase(blocks.begin()+i);
            blocks.push_back(solitaireBlock(icons[icon]));
        }
    }

};
void solitairescene::input(SDL_Keycode keysym) {
    //there is no input, because well.. There's nothing to do in these menus, they're just graphical
};
Transition solitairescene::endLogic() {
    Transition t = Transition();
    if (SDL_GetTicks64()-age > 50000) {
        t.gamemode = gameplay::gamemode+1;
        t.transition = true;
        t.fade = FADETYPE::BARS;
    }
    return t;

};
void solitairescene::reset() {
    age = SDL_GetTicks64();
}
