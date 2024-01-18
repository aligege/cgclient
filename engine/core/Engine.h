//
// (c) 2023 Eduardo Doria.
//

#ifndef engine_h
#define engine_h

#ifndef MAX_SCENE_LAYERS
#define MAX_SCENE_LAYERS 5
#endif

#ifndef MAX_LIGHTS
#define MAX_LIGHTS 6
#endif

#ifndef MAX_SHADOWSMAP
#define MAX_SHADOWSMAP 6
#endif

#ifndef MAX_SHADOWSCUBEMAP
#define MAX_SHADOWSCUBEMAP 1
#endif

#ifndef MAX_SHADOWCASCADES
#define MAX_SHADOWCASCADES 4
#endif

#ifndef MAX_SUBMESHES
#define MAX_SUBMESHES 4
#endif

#ifndef MAX_TILEMAP_TILESRECT
#define MAX_TILEMAP_TILESRECT 200
#endif

#ifndef MAX_TILEMAP_TILES
#define MAX_TILEMAP_TILES 200
#endif

#ifndef MAX_SPRITE_FRAMES
#define MAX_SPRITE_FRAMES 64
#endif

#ifndef MAX_BONES
#define MAX_BONES 70
#endif

#ifndef MAX_MORPHTARGETS
#define MAX_MORPHTARGETS 8
#endif

#ifndef MAX_EXTERNAL_BUFFERS
#define MAX_EXTERNAL_BUFFERS 12
#endif

#include "util/FunctionSubscribe.h"
#include "math/Rect.h"
#include "util/ThreadUtils.h"
#include <atomic>

void init();

namespace cg {

    class Scene;
    //class Rect;

    enum class Scaling{
        FITWIDTH,
        FITHEIGHT,
        LETTERBOX,
        CROP,
        STRETCH,
        NATIVE
    };

    // texture power of two strategy
    enum class TextureStrategy{
        FIT,
        RESIZE,
        NONE
    };


    enum class Platform{
        MacOS,
        iOS,
        Web,
        Android,
        Linux,
        Windows
    };

    enum class GraphicBackend{
        GLCORE33,
        GLES3,
        D3D11,
        METAL,
        WGPU
    };

    enum class BodyType{
        STATIC,
        KINEMATIC,
        DYNAMIC
    };

    class Engine {
        
    private:
        //-----Supernova config-----
        static Scene* scenes[MAX_SCENE_LAYERS];
        static size_t numScenes;

        static Scene* mainScene;
        
        static int canvasWidth;
        static int canvasHeight;
        
        static int preferredCanvasWidth;
        static int preferredCanvasHeight;
        
        static Rect viewRect;

        static Scaling scalingMode;
        static TextureStrategy textureStrategy;
        
        static bool callMouseInTouchEvent;
        static bool callTouchInMouseEvent;
        static bool useDegrees;
        static bool automaticTransparency;

        static bool allowEventsOutCanvas;

        static bool fixedTimeSceneUpdate;

        static uint64_t lastTime;
        static float updateTimeCount;
        
        static double deltatime;
        static float framerate;
        
        static float updateTime;

        static std::atomic<bool> viewLoaded;
        static std::atomic<bool> paused;

        thread_local static bool asyncThread;

        static Semaphore drawSemaphore;
        
        static bool transformCoordPos(float& x, float& y);
        static void calculateCanvas();
        static void includeScene(size_t index, Scene* scene);
        static void rearrangeScenes(size_t index);
        
    public:
        //Engine();
        //virtual ~Engine();
        
        //-----Supernova config-----
        static void setScene(Scene* scene);
        static Scene* getScene();
        static void addSceneLayer(Scene* scene);
        static void removeSceneLayer(Scene* scene);
        static void removeAllSceneLayers();

        static Scene* getMainScene();
        static Scene* getLastScene();
        
        static int getCanvasWidth();
        static int getCanvasHeight();
        static void setCanvasSize(int canvasWidth, int canvasHeight);
        
        static int getPreferredCanvasWidth();
        static int getPreferredCanvasHeight();
        
        static Rect getViewRect();
        
        static void setScalingMode(Scaling scalingMode);
        static Scaling getScalingMode();

        static void setTextureStrategy(TextureStrategy textureStrategy);
        static TextureStrategy getTextureStrategy();
        
        static void setCallMouseInTouchEvent(bool callMouseInTouchEvent);
        static bool isCallMouseInTouchEvent();

        static void setCallTouchInMouseEvent(bool callTouchInMouseEvent);
        static bool isCallTouchInMouseEvent();
        
        static void setUseDegrees(bool useDegrees);
        static bool isUseDegrees();

        static void setAutomaticTransparency(bool automaticTransparency);
        static bool isAutomaticTransparency();

        static void setAllowEventsOutCanvas(bool allowEventsOutCanvas);
        static bool isAllowEventsOutCanvas();
        
        static void setFixedTimeSceneUpdate(bool fixedTimeSceneUpdate);
        static bool isFixedTimeSceneUpdate();

        static void setUpdateTimeMS(unsigned int updateTimeMS);
        static void setUpdateTime(float updateTime);
        static float getUpdateTime();

        static float getSceneUpdateTime();
        
        static Platform getPlatform();
        static GraphicBackend getGraphicBackend();
        static bool isOpenGL();
        static float getFramerate();
        static float getDeltatime();

        static void startAsyncThread();
        static void commitThreadQueue();
        static void endAsyncThread();
        static bool isAsyncThread();
        static bool isViewLoaded();

        //-----Supernova API functions-----
        static void systemInit(int argc, char* argv[]);
        static void systemViewLoaded();
        static void systemViewChanged();
        static void systemDraw();
        static void systemViewDestroyed();
        static void systemShutdown();

        static void systemPause();
        static void systemResume();

        static void systemTouchStart(int pointer, float x, float y);
        static void systemTouchEnd(int pointer, float x, float y);
        static void systemTouchMove(int pointer, float x, float y);
        static void systemTouchCancel();

        static void systemMouseDown(int button, float x, float y, int mods);
        static void systemMouseUp(int button, float x, float y, int mods);
        static void systemMouseMove(float x, float y, int mods);
        static void systemMouseScroll(float xoffset, float yoffset, int mods);
        static void systemMouseEnter();
        static void systemMouseLeave();

        static void systemKeyDown(int key, bool repeat, int mods);
        static void systemKeyUp(int key, bool repeat, int mods);

        static void systemCharInput(wchar_t codepoint);

        //-----Supernova user events-----
        static FunctionSubscribe<void()> onViewLoaded;
        static FunctionSubscribe<void()> onViewChanged;
        static FunctionSubscribe<void()> onViewDestroyed;
        static FunctionSubscribe<void()> onDraw;
        static FunctionSubscribe<void()> onUpdate;
        static FunctionSubscribe<void()> onPause;
        static FunctionSubscribe<void()> onResume;
        static FunctionSubscribe<void()> onShutdown;
        static FunctionSubscribe<void(int,float,float)> onTouchStart;
        static FunctionSubscribe<void(int,float,float)> onTouchEnd;
        static FunctionSubscribe<void(int,float,float)> onTouchMove;
        static FunctionSubscribe<void()> onTouchCancel;
        static FunctionSubscribe<void(int,float,float,int)> onMouseDown;
        static FunctionSubscribe<void(int,float,float,int)> onMouseUp;
        static FunctionSubscribe<void(float,float,int)> onMouseScroll;
        static FunctionSubscribe<void(float,float,int)> onMouseMove;
        static FunctionSubscribe<void()> onMouseEnter;
        static FunctionSubscribe<void()> onMouseLeave;
        static FunctionSubscribe<void(int,bool,int)> onKeyDown;
        static FunctionSubscribe<void(int,bool,int)> onKeyUp;
        static FunctionSubscribe<void(wchar_t)> onCharInput;

    };
    
}

#endif /* engine_h */
