#ifndef NODE_H
#define NODE_H

#include "Vector3.h"
#include "Color.h"
#include "Component.h"
#include<vector>

namespace cg
{
    class Node
    {
        protected:
            static long _nextId;
        protected:
            long _id;
            std::string _name;
            bool _show = true;
            Vector3 _position = Vector3::ZERO;
            Vector3 _rotation = Vector3::ZERO;
            Vector3 _scale = Vector3::UNIT_SCALE;
            Vector4 _color = Vector4::ZERO;
            std::vector <Component*> _components;
            Node* _pparent=nullptr;
            std::vector <Node*> _children;
        public:
            Node();
            virtual ~Node();
        public:
            //about basic
            long getId() const;

            void setName(const std::string& name);
            std::string getName() const;

            void show(bool show);
            bool isShow() const;

            void setPosition(Vector3 position);
            Vector3 getPosition() const;

            void setRotation(Vector3 rotation);
            Vector3 getRotation() const;

            void setScale(Vector3 scale);
            Vector3 getScale() const;

            void setColor(Vector4 color);
            Vector4 getColor() const;

            //about component
            void addComponent(Component* component);
            void removeComponent(Component* component,bool cleanup=true);
            void removeComponents(const std::string& name,bool cleanup=true);
            void removeComponent(long id,bool cleanup=true);
            void clearComponents(bool cleanup=true);
            std::vector<Component*> getComponents(const std::string& name);
            Component* getComponent(const std::string& name);

            //about node
            void addChild(Node* pnode);
            void removeFromParent(bool cleanup=true);
            void removeChild(Node* pnode,bool cleanup=true);
            void removeChildren(const std::string& name,bool cleanup=true);
            void removeChild(long id,bool cleanup=true);
            void clearChildren(bool cleanup=true);
            std::vector<Node*> getChildren(const std::string& name);
            Node* getChild(const std::string& name);
            Node* getChild(long id);
            Node* getParent() const;
            void setParent(Node* pnode);
            //about update
            void update(float dt);
            void fixedUpdate(float dt);
    };
}

#endif //NODE_H