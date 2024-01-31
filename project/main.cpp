#include "Supernova.h"
using namespace cg;

#include "Image.h"
#include <iostream>
#include "global.h"
#include "json/jsonobject.h"
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
    //http://127.0.0.1:6002/logic/login
    //http://10.10.1.41/user/skin/ranks
    jsonobject json("Uid");
    json.setLong(1008611);
    cg::global::phttptool->post("http://10.10.1.41/user/skin/ranks",nullptr,&json, [](const char* data){
        std::cout << data << std::endl;
    });
    
}
