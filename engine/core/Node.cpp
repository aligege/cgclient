#include"Node.h"

namespace cg
{
    long Node::_nextId = 1;
    Node::Node()
    {
        _id = Node::_nextId;
        Node::_nextId++;
    }

    Node::Node(const std::string& name)
    {
        _id = Node::_nextId;
        Node::_nextId++;
        _name = name;
    }

    Node::~Node()
    {
        if(_pparent!=nullptr)
        {
            _pparent->removeChild(_id,false);
            setParent(nullptr);
        }
    }

    long Node::getId() const
    {
        return _id;
    }

    void Node::setName(const std::string& name)
    {
        _name = name;
    }

    std::string Node::getName() const
    {
        return _name;
    }

    void Node::show(bool show)
    {
        _show = show;
    }

    bool Node::isShow() const
    {
        return _show;
    }

    void Node::setPosition(Vector3 position)
    {
        _position = position;
    }

    void Node::setPosition(float x, float y, float z)
    {
        _position.x = x;
        _position.y = y;
        _position.z = z;
    }

    void Node::setPositionX(float x)
    {
        _position.x = x;
    }

    void Node::setPositionY(float y)
    {
        _position.y = y;
    }

    void Node::setPositionZ(float z)
    {
        _position.z = z;
    }

    Vector3 Node::getPosition() const
    {
        return _position;
    }

    float Node::getPositionX() const
    {
        return _position.x;
    }

    float Node::getPositionY() const
    {
        return _position.y;
    }

    float Node::getPositionZ() const
    {
        return _position.z;
    }

    void Node::setRotation(Vector3 rotation)
    {
        _rotation = rotation;
    }

    Vector3 Node::getRotation() const
    {
        return _rotation;
    }

    void Node::setScale(Vector3 scale)
    {
        _scale = scale;
    }

    Vector3 Node::getScale() const
    {
        return _scale;
    }

    void Node::setColor(const Vector4& color)
    {
        _color = color;
    }

    void Node::setColor(const float red, const float green, const float blue, const float alpha)
    {
        _color.x = red;
        _color.y = green;
        _color.z = blue;
        _color.w = alpha;
    }
    
    Vector4 Node::getColor() const
    {
        return _color;
    }

    void Node::setAlpha(float alpha){
        this->_color.w = alpha;
    }

    float Node::getAlpha() const{
        return _color.w;
    }

    void Node::addComponent(Component* component)
    {
        _components.push_back(component);
    }

    void Node::removeComponent(Component* component,bool cleanup)
    {
        for (auto it = _components.begin(); it != _components.end(); ++it)
        {
            if (*it == component)
            {
                _components.erase(it);
                if(cleanup)
                {
                    delete *it;
                }
                break;
            }
        }
    }

    void Node::removeComponents(const std::string& name,bool cleanup)
    {
        for (auto it = _components.begin(); it != _components.end();)
        {
            if ((*it)->getName() == name)
            {
                it = _components.erase(it);
                if(cleanup)
                {
                    delete *it;
                }
            }
            else
            {
                ++it;
            }
        }
    }

    void Node::removeComponent(long id,bool cleanup)
    {
        for (auto it = _components.begin(); it != _components.end(); ++it)
        {
            if ((*it)->getId() == id)
            {
                _components.erase(it);
                if(cleanup)
                {
                    delete *it;
                }
                break;
            }
        }
    }

    void Node::clearComponents(bool cleanup)
    {
        if(cleanup)
        {
            for (auto it = _components.begin(); it != _components.end(); ++it)
            {
                delete *it;
            }
        }
        _components.clear();
    }

    std::vector<Component*> Node::getComponents(const std::string& name)
    {
        std::vector<Component*> components;
        for (auto it = _components.begin(); it != _components.end(); ++it)
        {
            if ((*it)->getName() == name)
            {
                components.push_back(*it);
            }
        }
        return components;
    }

    Component* Node::getComponent(const std::string& name)
    {
        for (auto it = _components.begin(); it != _components.end(); ++it)
        {
            if ((*it)->getName() == name)
            {
                return *it;
            }
        }
        return nullptr;
    }

    void Node::addChild(Node* pnode)
    {
        _children.push_back(pnode);
    }

    void Node::removeFromParent(bool cleanup)
    {
        if(_pparent!=nullptr)
        {
            _pparent->removeChild(_id,cleanup);
            if(!cleanup)
            {
                setParent(nullptr);
            }
        }
    }

    void Node::removeChild(Node* pnode,bool cleanup)
    {
        for (auto it = _children.begin(); it != _children.end(); ++it)
        {
            if (*it == pnode)
            {
                _children.erase(it);
                if(cleanup)
                {
                    delete *it;
                }
                else
                {
                    pnode->setParent(nullptr);
                }
                break;
            }
        }
    }

    void Node::removeChildren(const std::string& name,bool cleanup)
    {
        for (auto it = _children.begin(); it != _children.end();)
        {
            if ((*it)->getName() == name)
            {
                it = _children.erase(it);
                if(cleanup)
                {
                    delete *it;
                }
                else
                {
                    (*it)->setParent(nullptr);
                }
            }
            else
            {
                ++it;
            }
        }
    }

    void Node::removeChild(long id,bool cleanup)
    {
        for (auto it = _children.begin(); it != _children.end(); ++it)
        {
            if ((*it)->getId() == id)
            {
                _children.erase(it);
                if(cleanup)
                {
                    delete *it;
                }
                else
                {
                    (*it)->setParent(nullptr);
                }
                break;
            }
        }
    }

    void Node::clearChildren(bool cleanup)
    {
        if(cleanup)
        {
            for (auto it = _children.begin(); it != _children.end(); ++it)
            {
                delete *it;
            }
        }
        else
        {
            for (auto it = _children.begin(); it != _children.end(); ++it)
            {
                (*it)->setParent(nullptr);
            }
        }
        _children.clear();
    }

    std::vector<Node*> Node::getChildren(const std::string& name)
    {
        std::vector<Node*> children;
        for (auto it = _children.begin(); it != _children.end(); ++it)
        {
            if ((*it)->getName() == name)
            {
                children.push_back(*it);
            }
        }
        return children;
    }

    Node* Node::getChild(const std::string& name)
    {
        for (auto it = _children.begin(); it != _children.end(); ++it)
        {
            if ((*it)->getName() == name)
            {
                return *it;
            }
        }
        return nullptr;
    }

    Node* Node::getChild(long id)
    {
        for (auto it = _children.begin(); it != _children.end(); ++it)
        {
            if ((*it)->getId() == id)
            {
                return *it;
            }
        }
        return nullptr;
    }

    Node* Node::getParent() const
    {
        return _pparent;
    }

    void Node::setParent(Node* pnode)
    {
        _pparent = pnode;
    }

    void Node::update(double dt)
    {
        for (auto it = _components.begin(); it != _components.end(); ++it)
        {
            if((*it)->isActive())
            {
                (*it)->update(dt);
            }
        }
        for (auto it = _children.begin(); it != _children.end(); ++it)
        {
            if((*it)->isShow())
            {
                (*it)->update(dt);
            }
        }
    }

    void Node::fixedUpdate(double dt)
    {
        for (auto it = _components.begin(); it != _components.end(); ++it)
        {
            if((*it)->isActive())
            {
                (*it)->fixedUpdate(dt);
            }
        }
        for (auto it = _children.begin(); it != _children.end(); ++it)
        {
            if((*it)->isShow())
            {
                (*it)->fixedUpdate(dt);
            }
        }
    }

    void Node::render()
    {
        for (auto it = _children.begin(); it != _children.end(); ++it)
        {
            if((*it)->isShow())
            {
                (*it)->render();
            }
        }
    }
}