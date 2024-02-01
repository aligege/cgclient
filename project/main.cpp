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
    //http://127.0.0.1:6002/logic/login
    //http://10.10.1.41/user/skin/ranks
    jsonobject json("Uid");
    json.setLong(1008611);
    cg::global::phttptool->post("http://10.10.1.41/user/skin/ranks",&json,&json, [](httpresponse* pres){
        auto code = pres->pheader->get("Code");
        Log::debug("code:%s\ndata:%s\n",code.c_str(),pres->memory);
        delete pres;
    });
    
}
