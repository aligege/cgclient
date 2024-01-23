#include "Supernova.h"
using namespace cg;

#include "Image.h"
#include <iostream>
#include "global.h"

Scene scene;
Image image(&scene);

void init(){

    image.setAnchorPreset(AnchorPreset::CENTER);
    image.setTexture("cgclient.png");

    Engine::setScalingMode(Scaling::FITWIDTH);
    Engine::setCanvasSize(1000,480);
    Engine::setScene(&scene);
    cg::global::http.get("https://www.baidu.com", [](const char* data){
        std::cout << data << std::endl;
    });
}
