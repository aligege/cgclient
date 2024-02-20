#include "Supernova.h"
using namespace cg;

#include "Image.h"
#include <iostream>
#include "global.h"
#include "json/jsonobject.h"
Scene scene;

void init(){
    Engine::setScalingMode(Scaling::FITWIDTH);
    Engine::setCanvasSize(1000,480);
    Engine::setScene(&scene);
    Text text(&scene);
    text.setText("Hello World");
    text.setPosition(100,100);
    text.setColor(1,0,0,1);
}
