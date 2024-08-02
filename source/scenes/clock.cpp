#include "clock.h"
#include "../pixfont.h"
#include <ctime>

clockscene::clockscene() {

};
void clockscene::render() {
    int time = std::time(0)-3600*5;   // get time now
    int seconds = time % 60;
    time /= 60;
    int minutes = time % 60;
    time /= 60;
    int hours = (time % 24)%12;
    time /= 24;
    char timebuff[6] = "     ";
    sprintf(timebuff,"%02d:%02d",hours,minutes);
    std::string s(timebuff);

    graphics::backgrounds->at(backnum).render();
    graphics::fonts->at(1)->render(timebuff,848/2,240-128,true,255,255,255,0,false,0,0,0,8);

};
void clockscene::logic(double deltatime) {
    graphics::backgrounds->at(backnum).logic(deltatime);
};
void clockscene::input(SDL_Keycode keysym) {
    //there is no input, because well.. There's nothing to do in these menus, they're just graphical
};
Transition clockscene::endLogic() {
    Transition t = Transition();
    if (SDL_GetTicks64()-age > 30000) {
        t.gamemode = gameplay::gamemode+1;
        t.transition = true;
        t.fade = FADETYPE::BARS;
    }
    return t;

};
void clockscene::reset() {
    backnum = std::rand() % graphics::backgrounds->size();
}
