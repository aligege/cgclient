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
    cg::global::phttptool->get("http://www.baidu.com", [](const char* data){
        std::cout << data << std::endl;
    });
    cg::global::phttptool->post("http://10.10.1.90:8088", [](const char* data){
        std::cout << data << std::endl;
    });
    
}
