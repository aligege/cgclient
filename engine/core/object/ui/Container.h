//
// (c) 2022 Eduardo Doria.
//

#ifndef CONTAINER_H
#define CONTAINER_H

#include "UILayout.h"

namespace Supernova{
    class Container: public UILayout{

    public:
        Container(Scene* scene);
        
        void setType(ContainerType type);
        ContainerType getType() const;

        void resize();

        void setBoxExpand(bool expand);
        void setBoxExpand(size_t id, bool expand);
        bool isBoxExpand(size_t id) const;
    };
}

#endif //CONTAINER_H