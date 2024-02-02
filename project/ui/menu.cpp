#include "menu.h"
#include "io/File.h"

menu::menu()
{
}
menu::~menu()
{
}
bool menu::initWithFile(const char *filename)
{
    cg::File* pfile = new cg::File();
    if(!pfile->open(filename, "r"))
    {
        delete pfile;
        return false;
    }
    auto str = pfile->readString();
    auto ret = this->jsmenu.parse(str.c_str());
    delete pfile;
    return ret;
}