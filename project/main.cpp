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
    
}
