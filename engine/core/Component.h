#ifndef COMPONENT_H
#define COMPONENT_H

#include "Vector3.h"
#include "Color.h"

namespace cg
{
    class Node;
    class Component
    {
        protected:
            static long _nextId;
        protected:
            bool _active = true;
            std::string _name;
            long _id;
            Node* _pnode=nullptr;
        public:
            Component();
            virtual ~Component();
        public:
            void setActive(bool active);
            bool isActive() const;

            void setName(const std::string& name);
            std::string getName() const;

            long getId() const;
            /**
             * @brief attach to a node,if the component has been attached to another node, it will be failed.
             * if the pnode is nullptr, the component will be removed from the previous node,and release this component
            */
            void setNode(Node* pnode);
            Node* getNode() const;

            void removeComponent(bool cleanup=true);
            void update(float dt);
            void fixedUpdate(float dt);
        private:
            virtual void _onLoad();
            virtual void _onDestory();

    };
}

#endif //COMPONENT_H