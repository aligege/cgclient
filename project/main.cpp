#include "Supernova.h"
using namespace cg;

#include "Image.h"

Scene scene;
Image image(&scene);

void init(){

    image.setAnchorPreset(AnchorPreset::CENTER);
    image.setTexture("cgclient.png");

    Engine::setScalingMode(Scaling::FITWIDTH);
    Engine::setCanvasSize(1000,480);
    Engine::setScene(&scene);
}
