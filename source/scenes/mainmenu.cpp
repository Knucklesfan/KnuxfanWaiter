#include "mainmenu.h"
#include "../pixfont.h"
mainmenu::mainmenu() {
    memset(starX, 0, sizeof(starX));
    memset(starY, 0, sizeof(starY)); //clear out the coordinates for the stars
    memset(starZ, 0, sizeof(starZ));
    memset(starBlock, 0, sizeof(starBlock));
    text = new model("models/amigaball.kmf",{0,6,-16.5},{0.5,0.5,0.5},{0,0,0});
    ourhouse = new model("models/ourhouse.kmf",{0,6,-16.5},{0.5,0.5,0.5},{0,0,0});

    for(int i = 0; i < 255; i++) {
        starX[i] = (rand() % 848) - 424;
        starY[i] = (rand() % 480) - 240;
        starZ[i] = (rand() % 1700) - 100;
        starBlock[i] = (rand() % 7);

    }

};
void mainmenu::render() {
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)INTERNAL_WIDTH / (float)INTERNAL_HEIGHT, 0.001f, 10000.0f);
    glm::mat4 view = glm::mat4(1.0f); //view is the **Camera**'s perspective
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); 
    glm::vec3 lightcolor = glm::vec3(1.0f, 1.0f, 1.0f);

    text->scale = {15,15,15};
    text->rotation = {0,SDL_GetTicks()/100.0f,0};
    text->position = {20,-1,-60};
    glm::vec3 lightpos = glm::vec3(20, 60, -60);
    graphics::shaders.at(10)->activate();
    graphics::shaders.at(10)->setVec3("lightPos", glm::value_ptr(lightpos));
    graphics::shaders.at(10)->setFloat("alpha",1.0);
    graphics::shaders.at(10)->setVec3("lightColor", glm::value_ptr(lightcolor));
    text->render(graphics::shaders.at(10),projection,view);

    text->position = {-20,-10,-20};
    text->rotation = {0,SDL_GetTicks()/-100.0f,0};
    text->render(graphics::shaders.at(10),projection,view);

    text->scale = {1,1,1};
    text->position = {5,-1,-10};
    text->render(graphics::shaders.at(10),projection,view);

    for(int i = 0; i < 255; i++) {
        float perspective = 300.0 / (300.0 - starZ[i]);
        int x = 424 + starX[i] * perspective;
        int y = 240 + starY[i] * perspective;

        starZ[i] += 2;
        if (starZ[i] > 300) {
            starX[i] = (rand() % 848) - 424;
            starY[i] = (rand() % 480) - 240;

            starZ[i] -= 600;
        }
        graphics::sprite->render(graphics::shaders.at(4),graphics::blocks->at(starBlock[i]),{x,y},{16*(perspective / 2),16*(perspective / 2)},{0,0,starZ[i]*4},{0,0},{16,16},{848,480},(perspective / 2));            
    }
    lightpos = glm::vec3(20, 60, 60);
    graphics::shaders.at(10)->activate();
    graphics::shaders.at(10)->setVec3("lightPos", glm::value_ptr(lightpos));
    graphics::shaders.at(10)->setFloat("alpha",1.0);
    graphics::shaders.at(10)->setVec3("lightColor", glm::value_ptr(lightcolor));

    ourhouse->scale = {1,1,1};
    ourhouse->rotation = {0,SDL_GetTicks()/10.0f,0};
    ourhouse->position = {0,0.45,-5};
    ourhouse->render(graphics::shaders.at(10),projection,view);

    glDisable(GL_DEPTH_TEST);

    graphics::fonts->at(1)->render(424,60,"WELCOME TO",true);

    graphics::fonts->at(2)->render(424,256,"please dont steal anything",true);
    graphics::fonts->at(2)->render(424,256+16,"I own nothing of value!",true);


};
void mainmenu::logic(double deltatime) {
    graphics::backgrounds->at(backnum).logic(deltatime);
};
void mainmenu::input(SDL_Keycode keysym) {
    //there is no input, because well.. There's nothing to do in these menus, they're just graphical
};
Transition mainmenu::endLogic() {
    Transition t = Transition();
    if (SDL_GetTicks64()-age > 30000) {
        t.gamemode = gameplay::gamemode+1;
        t.transition = true;
        t.fade = FADETYPE::BARS;
    }
    return t;

};
void mainmenu::reset() {
    age = SDL_GetTicks64();
    backnum = std::rand() % graphics::backgrounds->size();
}
