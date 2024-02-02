#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include "Component.h"

namespace cg
{
    class UIComponent: public Component
    {
        public:
            UIComponent();
            virtual ~UIComponent();
    };
}

#endif //UI_COMPONENT_H