#include "weather.h"
#include "../pixfont.h"

weather::weather() {
    text = new model("models/amigaball.kmf",{0,6,-16.5},{0.5,0.5,0.5},{0,0,0});
    peachhouse = new model("models/peachhouse.kmf",{0,0,0},{0.5,0.5,0.5},{0,0,0});
    sky = new skybox();
    p = new plane({0,-0.75,0},{25,25,1},{90,0,0});

};
void weather::render() {
    

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)INTERNAL_WIDTH / (float)INTERNAL_HEIGHT, 0.5f, 1000.0f);
    glm::mat4 view = glm::mat4(1.0f); //view is the **Camera**'s perspective
    view = glm::rotate(view, (float) glm::radians(60+SDL_GetTicks64()/100.0), glm::vec3(0.0f, 1.0f, 0.0f));

    sky->render(1,projection,view);
    view = glm::translate(view,{cos(SDL_GetTicks64()/2000.0)*8/15.0,-20/15.0,20/15.0});

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); 
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);  
    glm::vec3 lightcolor = glm::vec3(1.0f, 1.0f, 1.0f);

    peachhouse->scale = {1,1,1};
    // peachhouse->rotation = {0,SDL_GetTicks()/100.0f,0};
    // peachhouse->position = {cos(SDL_GetTicks64()/2000.0),-0.5,-2.5};
    // peachhouse->rotation = {0,60+SDL_GetTicks64()/100.0,0};
    glm::vec3 lightpos = glm::vec3(0, 0, -6);
    graphics::shaders.at(10)->activate();
    graphics::shaders.at(10)->setVec3("lightPos", glm::value_ptr(lightpos));
    graphics::shaders.at(10)->setFloat("alpha",1.0);
    graphics::shaders.at(10)->setVec3("lightColor", glm::value_ptr(lightcolor));

    graphics::shaders.at(9)->activate();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, graphics::cubemaps->at(1));
    glm::vec3 objColor = glm::vec3(1.0f, 0.5f, 0.31f);
    lightcolor = glm::vec3(1.0f, 1.0f, 1.0f);
    lightpos = glm::vec3(0, 0.5, 0);
    graphics::sprites.at("normalbrick")->activate(1);
    graphics::shaders.at(9)->setInt("normalMap",1);
    graphics::shaders.at(9)->setFloat("offset",SDL_GetTicks()/10000.0f);

    graphics::shaders.at(9)->setVec3("objectColor", glm::value_ptr(objColor));
    graphics::shaders.at(9)->setVec3("lightColor", glm::value_ptr(lightcolor));
    graphics::shaders.at(9)->setVec3("lightPos", glm::value_ptr(lightpos));
    peachhouse->render(graphics::shaders.at(0),projection,view); //second pass
    p->render(graphics::shaders.at(9),graphics::sprites.at("homophobicdog"),projection,view);

    glDisable(GL_CULL_FACE);

    glDisable(GL_DEPTH_TEST);

    //RENDER UI HERE NOW!

    graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("firstmenubackground"),{424-256,240-120},{512,240},0,{SDL_GetTicks64()/10.0,0},{512,240});
    graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("currentweatheroutline"),{424-256,240-120},{512,240},0,{0,0},{384,192});
    graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("currentweatheroutline"),{424-256,240},{512,240-120},0,{0,0},{384,192});

    graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("weathericons"),{424-256+8,240-120+8},{96,96},0,{networking::weathericonmap.at((std::string)weatherData["weather"][0]["icon"])*96,0},{96,96});
    graphics::fonts->at(1)->render(424-256+8+96+8,240-120+8+32,(std::string)weatherData["weather"][0]["main"],false,255,255,255,-1,true,SDL_GetTicks64()/1000.0,8,8);
    graphics::fonts->at(2)->render(424-256+8+96+8+128,240-120+8+32,"Current Temp: " + networking::to_string_with_precision((double)weatherData["main"]["temp"]) + "f",false,255,255,255,-1,false,SDL_GetTicks64()/1000.0,8,8);
    graphics::fonts->at(2)->render(424-256+8+96+8+128,240-120+8+32+8,"Feels like: " + networking::to_string_with_precision((double)weatherData["main"]["feels_like"]) + "f",false,255,255,255,-1,false,SDL_GetTicks64()/1000.0,8,8);
    graphics::fonts->at(2)->render(424-256+8+96+8+128,240-120+8+32+16,"Temp Min/Max: " + std::to_string((int)weatherData["main"]["temp_min"]) + "f / "+ std::to_string((int)weatherData["main"]["temp_max"]) + "f",false,255,255,255,-1,false,SDL_GetTicks64()/1000.0,8,8);
    graphics::fonts->at(2)->render(424-256+8+96+8+128,240-120+8+32+24,"Humidity: " + networking::to_string_with_precision((int)weatherData["main"]["humidity"]) + "%",false,255,255,255,-1,false,SDL_GetTicks64()/1000.0,8,8);
    int forecastShown = 0;
    for(int i = 0; i < forecastData["list"].size(); i++) {
        int location = 424-256+8+(forecastShown%8)*64;
        int time = (int)forecastData["list"][i]["dt"]-3600*5;
        int seconds = time % 60;
        time /= 60;
        int minutes = time % 60;
        time /= 60;
        int hours = time % 24;
        time /= 24;
        time = (int)forecastData["list"][i]["dt"]-3600*5;
        int dayoftheWeek = ((time / (24*3600)))+4;
        if(forecastShown >= 16) {
            break;
        }

        if(hours == 10 || hours == 13 || hours == 16 || hours == 19) {
            graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("weathericons"),{location,240+8+(forecastShown/8)*48},{48,48},0,{networking::weathericonmap.at((std::string)forecastData["list"][i]["weather"][0]["icon"])*96,0},{96,96});
            graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("weathericons"),{location,240+8+(forecastShown/8)*48},{48,48},0,{networking::weathericonmap.at((std::string)forecastData["list"][i]["weather"][0]["icon"])*96,0},{96,96});
            graphics::fonts->at(2)->render(location+24,240+8+(forecastShown/8)*48,(std::string)forecastData["list"][i]["weather"][0]["main"],true,255,255,255,0,false,SDL_GetTicks64()/1000.0,8,8);
            graphics::fonts->at(2)->render(location+24,240+8+(forecastShown/8)*48+12,networking::to_string_with_precision((double)forecastData["list"][i]["main"]["temp"]),true,255,255,255,0,false,SDL_GetTicks64()/1000.0,8,8);
            graphics::fonts->at(2)->render(location+24,240+8+(forecastShown/8)*48+24,std::to_string(hours)+":00",true,255,255,255,0,false,SDL_GetTicks64()/1000.0,8,8);
            graphics::fonts->at(2)->render(location+24,240+8+(forecastShown/8)*48+32,days.at(dayoftheWeek%7),true,255,255,255,0,false,SDL_GetTicks64()/1000.0,8,8);
            forecastShown++;
        }

    }
    graphics::fonts->at(1)->render(424,64,"WEATHER",true,((int)(SDL_GetTicks()/100.0)%3==0)*255,((int)(SDL_GetTicks()/100.0)%3==1)*255,((int)(SDL_GetTicks()/100.0)%3==2)*255,-1,true,SDL_GetTicks()/1000.0,10,10);


};
void weather::logic(double deltatime) {
};
void weather::input(SDL_Keycode keysym) {
    //there is no input, because well.. There's nothing to do in these menus, they're just graphical
};
Transition weather::endLogic() {
    Transition t = Transition();
    if (SDL_GetTicks64()-age > 30000) {
        t.gamemode = gameplay::gamemode+1;
        t.transition = true;
        t.fade = FADETYPE::BARS;
    }
    return t;

};
void weather::reset() {
    networking::loadNews();
    age = SDL_GetTicks64();
    std::ifstream fW("/opt/knuxfanwaiter/weather.json");
    weatherData = nlohmann::json::parse(fW);
    std::ifstream fF("/opt/knuxfanwaiter/forecast.json");
    forecastData = nlohmann::json::parse(fF);


    backnum = std::rand() % graphics::backgrounds->size();
}
