//
// (c) 2023 Eduardo Doria.
//

#include "Engine.h"
#include "Scene.h"
#include "System.h"
#include "Input.h"
#include "render/SystemRender.h"
#include "script/LuaBinding.h"
#include "subsystem/AudioSystem.h"
#include "subsystem/RenderSystem.h"
#include "subsystem/UISystem.h"
#include "pool/TexturePool.h"
#include "pool/TextureDataPool.h"
#include "pool/ShaderPool.h"
#include "pool/FontPool.h"

#include "sokol_time.h"

using namespace cg;

//-----cgclient user config-----
Scene* Engine::scenes[MAX_SCENE_LAYERS] = {NULL};
size_t Engine::numScenes = 0;

Scene* Engine::mainScene = NULL;

int Engine::canvasWidth;
int Engine::canvasHeight;

int Engine::preferredCanvasWidth;
int Engine::preferredCanvasHeight;

Rect Engine::viewRect;

Scaling Engine::scalingMode;
TextureStrategy Engine::textureStrategy;

bool Engine::callMouseInTouchEvent;
bool Engine::callTouchInMouseEvent;
bool Engine::useDegrees;
bool Engine::automaticTransparency;
bool Engine::allowEventsOutCanvas;
bool Engine::fixedTimeSceneUpdate;

uint64_t Engine::lastTime = 0;
float Engine::updateTimeCount = 0;

double Engine::deltatime = 0;
float Engine::framerate = 0;

float Engine::updateTime = 0.01667; //60Hz

std::atomic<bool> Engine::viewLoaded = false;
std::atomic<bool> Engine::paused = false;

thread_local bool Engine::asyncThread = false;
Semaphore Engine::drawSemaphore;

//-----cgclient user events-----
FunctionSubscribe<void()> Engine::onViewLoaded;
FunctionSubscribe<void()> Engine::onViewChanged;
FunctionSubscribe<void()> Engine::onViewDestroyed;
FunctionSubscribe<void()> Engine::onDraw;
FunctionSubscribe<void()> Engine::onUpdate;
FunctionSubscribe<void()> Engine::onPause;
FunctionSubscribe<void()> Engine::onResume;
FunctionSubscribe<void()> Engine::onShutdown;
FunctionSubscribe<void(int,float,float)> Engine::onTouchStart;
FunctionSubscribe<void(int,float,float)> Engine::onTouchEnd;
FunctionSubscribe<void(int,float,float)> Engine::onTouchMove;
FunctionSubscribe<void()> Engine::onTouchCancel;
FunctionSubscribe<void(int,float,float,int)> Engine::onMouseDown;
FunctionSubscribe<void(int,float,float,int)> Engine::onMouseUp;
FunctionSubscribe<void(float,float,int)> Engine::onMouseScroll;
FunctionSubscribe<void(float,float,int)> Engine::onMouseMove;
FunctionSubscribe<void()> Engine::onMouseEnter;
FunctionSubscribe<void()> Engine::onMouseLeave;
FunctionSubscribe<void(int,bool,int)> Engine::onKeyDown;
FunctionSubscribe<void(int,bool,int)> Engine::onKeyUp;
FunctionSubscribe<void(wchar_t)> Engine::onCharInput;


void Engine::setScene(Scene* scene){
    if (asyncThread)
        drawSemaphore.acquire();

    numScenes = 0;
    if (scene){
        includeScene(0, scene);
        mainScene = scene;

        for (int i = 0; i < MAX_SCENE_LAYERS; i++){
            if (scenes[i]){
                numScenes++;
            }else{
                break;
            }
        }
    }else{
        scenes[0] = NULL;
    }

    if (asyncThread)
        drawSemaphore.release();
}

Scene* Engine::getScene(){
    return Engine::scenes[0];
}

void Engine::addSceneLayer(Scene* scene){
    if (asyncThread)
        drawSemaphore.acquire();

    if (scene){
        bool foundSlot = false;
        // 0 is reserved to mainScene
        for (int i = 1; i < MAX_SCENE_LAYERS; i++){
            if (scenes[i] == scene){
                foundSlot = true;
                break;
            }
            if (!scenes[i]){
                includeScene(i, scene);

                numScenes++;
                foundSlot = true;
                break;
            }
        }
        if (!foundSlot){
            Log::error("Scene layers is full. MAX_SCENE_LAYERS is set to: %i", MAX_SCENE_LAYERS);
        }
    }

    if (asyncThread)
        drawSemaphore.release();
}

void Engine::removeSceneLayer(Scene* scene){
    if (asyncThread)
        drawSemaphore.acquire();

    if (scene){
        for (int i = 1; i < MAX_SCENE_LAYERS; i++){
            if (scenes[i] && scenes[i] == scene){
                scenes[i] = NULL;
                numScenes--;
                rearrangeScenes(i);
            }
        }
    }

    if (asyncThread)
        drawSemaphore.release();
}

void Engine::removeAllSceneLayers(){
    if (asyncThread)
        drawSemaphore.acquire();

    for (int i = 1; i < MAX_SCENE_LAYERS; i++){
        if (scenes[i]){
            scenes[i] = NULL;
        }
    }
    numScenes = 0;

    if (asyncThread)
        drawSemaphore.release();
}

Scene* Engine::getMainScene(){
    return mainScene;
}

Scene* Engine::getLastScene(){
    int index = 0;
    for (int i = 1; i < MAX_SCENE_LAYERS; i++){
        if (scenes[i]){
            index = i;
        }
    }

    return scenes[index];
}

void Engine::includeScene(size_t index, Scene* scene){
    scenes[index] = scene;
    if (viewLoaded){
        scene->load();
        scene->updateSizeFromCamera();
    }
}

void Engine::rearrangeScenes(size_t index){
    if (index >= 0 && index < (MAX_SCENE_LAYERS-1)){
        for (int i = index; i < (MAX_SCENE_LAYERS-1); i++){
            Scene* temp = scenes[i+1];
            scenes[i+1] = scenes[i];
            scenes[i] = temp;
        }
    }
}

int Engine::getCanvasWidth(){
    return Engine::canvasWidth;
}

int Engine::getCanvasHeight(){
    return Engine::canvasHeight;
}

void Engine::setCanvasSize(int canvasWidth, int canvasHeight){
    Engine::preferredCanvasWidth = canvasWidth;
    Engine::preferredCanvasHeight = canvasHeight;

    calculateCanvas();
}

int Engine::getPreferredCanvasWidth(){
    return Engine::preferredCanvasWidth;
}

int Engine::getPreferredCanvasHeight(){
    return Engine::preferredCanvasHeight;
}

Rect Engine::getViewRect(){
    return viewRect;
}

void Engine::setScalingMode(Scaling scalingMode){
    Engine::scalingMode = scalingMode;
}

Scaling Engine::getScalingMode(){
    return scalingMode;
}

void Engine::setTextureStrategy(TextureStrategy textureStrategy){
    Engine::textureStrategy = textureStrategy;
}

TextureStrategy Engine::getTextureStrategy(){
    return textureStrategy;
}

void Engine::setCallMouseInTouchEvent(bool callMouseInTouchEvent){
    Engine::callMouseInTouchEvent = callMouseInTouchEvent;
}

bool Engine::isCallMouseInTouchEvent(){
    return Engine::callMouseInTouchEvent;
}

void Engine::setCallTouchInMouseEvent(bool callTouchInMouseEvent){
    Engine::callTouchInMouseEvent = callTouchInMouseEvent;
}

bool Engine::isCallTouchInMouseEvent(){
    return Engine::callTouchInMouseEvent;
}

void Engine::setUseDegrees(bool useDegrees){
    Engine::useDegrees = useDegrees;
}

bool Engine::isUseDegrees(){
    return Engine::useDegrees;
}

void Engine::setAutomaticTransparency(bool automaticTransparency){
    Engine::automaticTransparency = automaticTransparency;
}

bool Engine::isAutomaticTransparency(){
    return automaticTransparency;
}

void Engine::setAllowEventsOutCanvas(bool allowEventsOutCanvas){
    Engine::allowEventsOutCanvas = allowEventsOutCanvas;
}

bool Engine::isAllowEventsOutCanvas(){
    return allowEventsOutCanvas;
}

void Engine::setFixedTimeSceneUpdate(bool fixedTimeSceneUpdate) {
    Engine::fixedTimeSceneUpdate = fixedTimeSceneUpdate;
}

bool Engine::isFixedTimeSceneUpdate() {
    return fixedTimeSceneUpdate;
}

void Engine::setUpdateTimeMS(unsigned int updateTimeMS){
    Engine::updateTime = updateTimeMS / 1000.0f;
}

void Engine::setUpdateTime(float updateTime){
    Engine::updateTime = updateTime;
}

float Engine::getUpdateTime(){
    return Engine::updateTime;
}

float Engine::getSceneUpdateTime(){
    if (isFixedTimeSceneUpdate()){
        return getUpdateTime();
    }else{
        return getDeltatime();
    }
}

Platform Engine::getPlatform(){
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    return Platform::Windows;
#elif __APPLE__
    #include <TargetConditionals.h>
    #if TARGET_OS_SIMULATOR
    return Platform::iOS;
    #elif TARGET_OS_IPHONE
    return Platform::iOS;
    #elif TARGET_OS_MAC
    return Platform::MacOS;
    #endif
#elif __ANDROID__
    return Platform::Android;
#elif __EMSCRIPTEN__
    return Platform::Web;
#elif __linux__
    return Platform::Linux;
#endif
}

GraphicBackend Engine::getGraphicBackend(){
#if defined(SOKOL_GLCORE33)
    return GraphicBackend::GLCORE33;
#elif defined(SOKOL_GLES3)
    return GraphicBackend::GLES3;
#elif defined(SOKOL_D3D11)
    return GraphicBackend::D3D11;
#elif defined(SOKOL_METAL)
    return GraphicBackend::METAL;
#elif defined(SOKOL_WGPU)
    return GraphicBackend::WGPU;
#elif defined(SUPERNOVA_APPLE) //Xcode template
    return GraphicBackend::METAL;
#endif
}

bool Engine::isOpenGL(){
    GraphicBackend gbackend = getGraphicBackend();

    if (gbackend == GraphicBackend::GLCORE33)
        return true;
    if (gbackend == GraphicBackend::GLES3)
        return true;

    return false;
}

float Engine::getFramerate(){
    return framerate;
}

float Engine::getDeltatime(){
    return deltatime;
}

void Engine::startAsyncThread(){
    asyncThread = true;
}

void Engine::commitThreadQueue(){
    SystemRender::commitQueue();
}

void Engine::endAsyncThread(){
    commitThreadQueue();
    asyncThread = false;
}

bool Engine::isAsyncThread(){
    return asyncThread;
}

bool Engine::isViewLoaded(){
    return viewLoaded;
}

void Engine::calculateCanvas(){
    Engine::canvasWidth = preferredCanvasWidth;
    Engine::canvasHeight = preferredCanvasHeight;

    int screenWidth = System::instance().getScreenWidth();
    int screenHeight = System::instance().getScreenHeight();

    //When canvas size is changed
    if (screenWidth != 0 && screenHeight != 0){
        if (scalingMode == Scaling::FITWIDTH){
            Engine::canvasWidth = preferredCanvasWidth;
            Engine::canvasHeight = screenHeight * preferredCanvasWidth / screenWidth;
        }
        if (scalingMode == Scaling::FITHEIGHT){
            Engine::canvasHeight = preferredCanvasHeight;
            Engine::canvasWidth = screenWidth * preferredCanvasHeight / screenHeight;
        }
        if (scalingMode == Scaling::NATIVE){
            Engine::canvasHeight = screenHeight;
            Engine::canvasWidth = screenWidth;
        }
    }
}

void Engine::systemInit(int argc, char* argv[]){

    Engine::setCanvasSize(1000,480);
    Engine::setScalingMode(Scaling::FITWIDTH);
    Engine::setTextureStrategy(TextureStrategy::RESIZE);
    Engine::setCallMouseInTouchEvent(false);
    Engine::setCallTouchInMouseEvent(false);
    Engine::setUseDegrees(true);
    Engine::setAutomaticTransparency(true);
    Engine::setAllowEventsOutCanvas(false);
    Engine::setFixedTimeSceneUpdate(true);

    lastTime = 0;
    updateTimeCount = 0;
    viewLoaded = false;
    paused = false;

    drawSemaphore.release();

    stm_setup();
    
    std::vector<std::string> args(argv, argv + argc);
    System::instance().args = args;

    LuaBinding::createLuaState();

    #ifndef NO_LUA_INIT
    LuaBinding::init();
    #endif
    #ifndef NO_CPP_INIT
    init();
    #endif
}

void Engine::systemViewLoaded(){
    SystemRender::setup();

    viewLoaded = true;
    onViewLoaded.call();
    
    for (int i = 0; i < numScenes; i++){
        scenes[i]->load();
    }
}

void Engine::systemViewChanged(){
    calculateCanvas();

    int screenWidth = System::instance().getScreenWidth();
    int screenHeight = System::instance().getScreenHeight();
    
    int viewX = 0;
    int viewY = 0;
    int viewWidth = screenWidth;
    int viewHeight = screenHeight;
    
    float screenAspect = (float)screenWidth / (float)screenHeight;
    float canvasAspect = (float)Engine::getPreferredCanvasWidth() / (float)Engine::getPreferredCanvasHeight();
    
    //When canvas size is not changed
    if (Engine::getScalingMode() == Scaling::LETTERBOX){
        if (screenAspect < canvasAspect){
            float aspect = (float)screenWidth / (float)Engine::getPreferredCanvasWidth();
            int newHeight = (int)((float)Engine::getPreferredCanvasHeight() * aspect);
            int dif = screenHeight - newHeight;
            viewY = (dif/2);
            viewHeight = screenHeight-(viewY*2); //diff could be odd, for this use view*2
        }else{
            float aspect = (float)screenHeight / (float)Engine::getPreferredCanvasHeight();
            int newWidth = (int)((float)Engine::getPreferredCanvasWidth() * aspect);
            int dif = screenWidth - newWidth;
            viewX = (dif/2);
            viewWidth = screenWidth-(viewX*2);
        }
    }
    
    if (Engine::getScalingMode() == Scaling::CROP){
        if (screenAspect > canvasAspect){
            float aspect = (float)screenWidth / (float)Engine::getPreferredCanvasWidth();
            int newHeight = (int)((float)Engine::getPreferredCanvasHeight() * aspect);
            int dif = screenHeight - newHeight;
            viewY = (dif/2);
            viewHeight = screenHeight-(viewY*2);
        }else{
            float aspect = (float)screenHeight / (float)Engine::getPreferredCanvasHeight();
            int newWidth = (int)((float)Engine::getPreferredCanvasWidth() * aspect);
            int dif = screenWidth - newWidth;
            viewX = (dif/2);
            viewWidth = screenWidth-(viewX*2);
        }
    }
    
    // S_SCALING_STRETCH do not need nothing
    
    viewRect.setRect(viewX, viewY, viewWidth, viewHeight);

    for (int i = 0; i < numScenes; i++){
        scenes[i]->updateSizeFromCamera();
    }

    onViewChanged.call();
}

void Engine::systemDraw(){
    //Deltatime in seconds
    deltatime = stm_sec(stm_laptime(&lastTime));
    framerate = 1 / deltatime;

    drawSemaphore.acquire();

    SystemRender::executeQueue();

    // avoid increment updateTimeCount after resume
    if (!paused) {
        int updateLoops = 0;
        updateTimeCount += deltatime;
        while (updateTimeCount >= updateTime && updateLoops <= 100) {
            updateLoops++;
            updateTimeCount -= updateTime;

            Engine::onUpdate.call();

            if (isFixedTimeSceneUpdate()) {
                for (int i = 0; i < numScenes; i++) {
                    scenes[i]->update(updateTime);
                }
            }
        }
        if (updateLoops > 100) {
            Log::warn("More than 100 updates in a frame");
        }

        if (!isFixedTimeSceneUpdate()) {
            for (int i = 0; i < numScenes; i++) {
                scenes[i]->update(deltatime);
            }
        }
    }

    Engine::onDraw.call();

    for (int i = 0; i < numScenes; i++){
        scenes[i]->render();
    }

    SystemRender::commit();

    drawSemaphore.release();

    AudioSystem::checkActive();
}

void Engine::systemViewDestroyed(){
    drawSemaphore.acquire();
    
    viewLoaded = false;
    Engine::onViewDestroyed.call();

    //TODO: must destroy all scenes (even if not a layer)
    for (int i = 0; i < numScenes; i++){
        scenes[i]->destroy();
    }

    SystemRender::shutdown();

    TexturePool::clear();
    TextureDataPool::clear();
    ShaderPool::clear();
    FontPool::clear();

    drawSemaphore.release();
}

void Engine::systemShutdown(){
    Engine::onShutdown.call();

    LuaBinding::cleanup();

    removeAllSceneLayers();
}

void Engine::systemPause(){
    AudioSystem::pauseAll();
    Engine::onPause.call();
    paused = true;
}

void Engine::systemResume(){
    AudioSystem::resumeAll();
    Engine::onResume.call();
    paused = false;
}

bool Engine::transformCoordPos(float& x, float& y){

    x = (x - viewRect.getX()) / viewRect.getWidth();
    y = (y - viewRect.getY()) / viewRect.getHeight();
    
    x = (float)Engine::getCanvasWidth() * x;
    y = (float)Engine::getCanvasHeight() * y;
    
    if (allowEventsOutCanvas)
        return true;
    
    return ((x >= 0) && (x <= Engine::getCanvasWidth()) && (y >= 0) && (y <= Engine::getCanvasHeight()));
}

void Engine::systemTouchStart(int pointer, float x, float y){
    if (transformCoordPos(x, y)){
        //-----------------
        Engine::onTouchStart.call(pointer, x, y);
        Input::addTouch(pointer, x, y);
        //-----------------
        if (Engine::isCallMouseInTouchEvent()){
            //-----------------
            Engine::onMouseDown.call(S_MOUSE_BUTTON_1, x, y, 0);
            Input::addMousePressed(S_MOUSE_BUTTON_1);
            Input::setMousePosition(x, y);
            //-----------------
        }

        for (int i = 0; i < numScenes; i++){
            if (scenes[i]->canReceiveUIEvents())
                scenes[i]->getSystem<UISystem>()->eventOnPointerDown(x, y);
        }
    }
}

void Engine::systemTouchEnd(int pointer, float x, float y){
    if (transformCoordPos(x, y)){
        //-----------------
        Engine::onTouchEnd.call(pointer, x, y);
        Input::removeTouch(pointer);
        //-----------------
        if (Engine::isCallMouseInTouchEvent()){
            //-----------------
            Engine::onMouseUp.call(S_MOUSE_BUTTON_1, x, y, 0);
            Input::releaseMousePressed(S_MOUSE_BUTTON_1);
            Input::setMousePosition(x, y);
            //-----------------
        }

        for (int i = 0; i < numScenes; i++){
            if (scenes[i]->canReceiveUIEvents())
                scenes[i]->getSystem<UISystem>()->eventOnPointerUp(x, y);
        }
    }
}

void Engine::systemTouchMove(int pointer, float x, float y){
    if (transformCoordPos(x, y)){
        //-----------------
        Engine::onTouchMove.call(pointer, x, y);
        Input::setTouchPosition(pointer, x, y);
        //-----------------
        if (Engine::isCallMouseInTouchEvent()){
            //-----------------
            Engine::onMouseMove.call(x, y, 0);
            Input::setMousePosition(x, y);
            //-----------------
        }
    }
}

void Engine::systemTouchCancel(){
    //-----------------
    Engine::onTouchCancel.call();
    Input::clearTouches();
    //-----------------
}

void Engine::systemMouseDown(int button, float x, float y, int mods){
    if (transformCoordPos(x, y)){
        //-----------------
        Engine::onMouseDown.call(button, x, y, mods);
        Input::addMousePressed(button);
        Input::setMousePosition(x, y);
        if (mods != 0)
            Input::setModifiers(mods);
        //-----------------
        if (Engine::isCallTouchInMouseEvent()){
            //-----------------
            Engine::onTouchStart.call(0, x, y);
            Input::addTouch(0, x, y);
            //-----------------
        }

        for (int i = 0; i < numScenes; i++){
            if (scenes[i]->canReceiveUIEvents())
                if (button == S_MOUSE_BUTTON_1)
                    scenes[i]->getSystem<UISystem>()->eventOnPointerDown(x, y);
        }
    }
}
void Engine::systemMouseUp(int button, float x, float y, int mods){
    if (transformCoordPos(x, y)){
        //-----------------
        Engine::onMouseUp.call(button, x, y, mods);
        Input::releaseMousePressed(button);
        Input::setMousePosition(x, y);
        if (mods != 0)
            Input::setModifiers(mods);
        //-----------------
        if (Engine::isCallTouchInMouseEvent()){
            //-----------------
            Engine::onTouchEnd.call(0, x, y);
            Input::removeTouch(0);
            //-----------------
        }

        for (int i = 0; i < numScenes; i++){
            if (scenes[i]->canReceiveUIEvents())
                if (button == S_MOUSE_BUTTON_1)
                    scenes[i]->getSystem<UISystem>()->eventOnPointerUp(x, y);
        }
    }
}

void Engine::systemMouseMove(float x, float y, int mods){
    if (transformCoordPos(x, y)){
        //-----------------
        Engine::onMouseMove.call(x, y, mods);
        Input::setMousePosition(x, y);
        if (mods != 0)
            Input::setModifiers(mods);
        //-----------------
        if (Engine::isCallTouchInMouseEvent()){
            //-----------------
            if (Input::isMousePressed(S_MOUSE_BUTTON_LEFT) || Input::isMousePressed(S_MOUSE_BUTTON_RIGHT)){
                Engine::onTouchMove.call(0, x, y);
                Input::setTouchPosition(0, x, y);
            }
            //-----------------
        }
    }
}

void Engine::systemMouseScroll(float xoffset, float yoffset, int mods){
    //-----------------
    Engine::onMouseScroll.call(xoffset, yoffset, mods);
    Input::setMouseScroll(xoffset, yoffset);
    if (mods != 0)
        Input::setModifiers(mods);
    //-----------------
}

void Engine::systemMouseEnter(){
    //-----------------
    Engine::onMouseEnter.call();
    Input::addMouseEntered();
    //-----------------
}

void Engine::systemMouseLeave(){
    //-----------------
    Engine::onMouseLeave.call();
    Input::releaseMouseEntered();
    //-----------------
}

void Engine::systemKeyDown(int key, bool repeat, int mods){
    //-----------------
    Engine::onKeyDown.call(key, repeat, mods);
    Input::addKeyPressed(key);
    if (mods != 0)
        Input::setModifiers(mods);
    //-----------------
}

void Engine::systemKeyUp(int key, bool repeat, int mods){
    //-----------------
    Engine::onKeyUp.call(key, repeat, mods);
    Input::releaseKeyPressed(key);
    Input::setModifiers(mods); // Now it can be 0
    //-----------------
}

void Engine::systemCharInput(wchar_t codepoint){
    onCharInput.call(codepoint);

    for (int i = 0; i < numScenes; i++){
        if (scenes[i]->canReceiveUIEvents())
            scenes[i]->getSystem<UISystem>()->eventOnCharInput(codepoint);
    }
}
