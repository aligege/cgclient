//
// (c) 2020 Eduardo Doria.
//

#ifndef LUA_H
#define LUA_H

#include "Object.h"

namespace cg {
    class LuaScript {
    public:
        static void setObject(std::string global, Object* object);
        static Object* getObject(std::string global);
    };

}


#endif //ANDROIDSTUDIO_LUA_H
