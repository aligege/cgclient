#include "Engine.h"
#include "SupernovaSokol.h"
#include "Input.h"

#include "sokol_gfx.h"
#include "sokol_app.h"
#include "sokol_glue.h"

void sokol_init(void) {
    cg::Engine::systemViewLoaded();
    cg::Engine::systemViewChanged();
}

void sokol_frame(void) {
    cg::Engine::systemDraw();
}

int convMouseButtom(sapp_mousebutton mouse_button){
    if (mouse_button == SAPP_MOUSEBUTTON_LEFT){
        return S_MOUSE_BUTTON_LEFT;
    }else if (mouse_button == SAPP_MOUSEBUTTON_RIGHT){
        return S_MOUSE_BUTTON_RIGHT;
    }else if (mouse_button == SAPP_MOUSEBUTTON_MIDDLE){
        return S_MOUSE_BUTTON_MIDDLE;
    }
    return S_MOUSE_BUTTON_LEFT;
}

static void sokol_event(const sapp_event* e) {
    if (e->type == SAPP_EVENTTYPE_RESIZED)
        cg::Engine::systemViewChanged();
    else if (e->type == SAPP_EVENTTYPE_CHAR)
        cg::Engine::systemCharInput(e->char_code);
    else if (e->type == SAPP_EVENTTYPE_KEY_DOWN){
        if (e->key_code == SAPP_KEYCODE_TAB)
            cg::Engine::systemCharInput('\t');
        if (e->key_code == SAPP_KEYCODE_BACKSPACE)
            cg::Engine::systemCharInput('\b');
        if (e->key_code == SAPP_KEYCODE_ENTER)
            cg::Engine::systemCharInput('\r');
        if (e->key_code == SAPP_KEYCODE_ESCAPE)
            cg::Engine::systemCharInput('\e');
        //Use same keycode of GLFW
        cg::Engine::systemKeyDown(e->key_code, e->key_repeat, e->modifiers);
    }else if (e->type == SAPP_EVENTTYPE_KEY_UP)
        //Use same keycode of GLFW
        cg::Engine::systemKeyUp(e->key_code, e->key_repeat, e->modifiers);
    else if (e->type == SAPP_EVENTTYPE_SUSPENDED)
        cg::Engine::systemPause();
    else if (e->type == SAPP_EVENTTYPE_RESUMED)
        cg::Engine::systemResume();  
    else if (e->type == SAPP_EVENTTYPE_MOUSE_UP)
        cg::Engine::systemMouseUp(convMouseButtom(e->mouse_button), e->mouse_x, e->mouse_y, e->modifiers);  
    else if (e->type == SAPP_EVENTTYPE_MOUSE_DOWN)
        cg::Engine::systemMouseDown(convMouseButtom(e->mouse_button), e->mouse_x, e->mouse_y, e->modifiers);
    else if (e->type == SAPP_EVENTTYPE_MOUSE_UP)
        cg::Engine::systemMouseUp(convMouseButtom(e->mouse_button), e->mouse_x, e->mouse_y, e->modifiers);
    else if (e->type == SAPP_EVENTTYPE_MOUSE_MOVE)
        cg::Engine::systemMouseMove(e->mouse_x, e->mouse_y, e->modifiers);
    else if (e->type == SAPP_EVENTTYPE_MOUSE_SCROLL)
        cg::Engine::systemMouseScroll(e->scroll_x, e->scroll_y, e->modifiers);
    else if (e->type == SAPP_EVENTTYPE_MOUSE_ENTER)
        cg::Engine::systemMouseEnter();
    else if (e->type == SAPP_EVENTTYPE_MOUSE_LEAVE)
        cg::Engine::systemMouseLeave();
}

void sokol_cleanup(void) {
    cg::Engine::systemViewDestroyed();
    cg::Engine::systemShutdown();
}

sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    sapp_desc app_desc = {0};

    app_desc.init_cb = sokol_init;
    app_desc.frame_cb = sokol_frame;
    app_desc.event_cb = sokol_event;
    app_desc.cleanup_cb = sokol_cleanup;
    app_desc.width = 960;
    app_desc.height = 540;
    app_desc.sample_count = 4;
    app_desc.window_title = "cgclient";

    cg::Engine::systemInit(argc, argv);

    return app_desc;
}