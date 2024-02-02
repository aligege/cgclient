#ifndef __CGUI_MENU_H__
#define __CGUI_MENU_H__
#include "json/jsonobject.h"

class menu 
{
    public:
        menu();
        ~menu();
    protected:
        cg::jsonobject jsmenu;
    public:
        bool initWithFile(const char *filename);
};

#endif