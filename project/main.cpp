#include "Supernova.h"
using namespace cg;

#include "Image.h"
#include "Text.h"
#include <iostream>
#include "global.h"
#include "json/jsonobject.h"
#include "UIText.h"

Scene scene;
using namespace cg;
void init(){
    Engine::setScalingMode(Scaling::FITWIDTH);
    Engine::setCanvasSize(1000,480);
    Engine::setScene(&scene);

    //Image image(&scene);
    //image.setAnchorPreset(AnchorPreset::CENTER);
    //image.setTexture("cgclient.png");
    
    Text* ptext = new Text(&scene);
    ptext->setText("Hello World");
    ptext->setName("textobject");
    ptext->setColor(Vector4(1, 0.2, 0.2, 1.0));
    ptext->setAnchorPreset(AnchorPreset::CENTER);

    Node* pnode = new Node("nd_txt");
    scene.addNode(pnode);
    
    UIText* ptext2 = new UIText("hailo shijie!");
    pnode->addComponent(ptext2);
    
    ptext2->init();

}
