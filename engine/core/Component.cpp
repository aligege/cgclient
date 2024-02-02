#include"Component.h"
#include"Node.h"
#include"Log.h"
namespace cg
{
    long Component::_nextId = 1;
    Component::Component()
    {
        _id=Component::_nextId;
        Component::_nextId++;
    }

    Component::~Component()
    {
        if(_pnode!=nullptr)
        {
            _pnode->removeComponent(_id,false);
            setNode(nullptr);
        }
        try
        {
            _onDestory();
        }
        catch(const std::exception& e)
        {
            Log::error("Component::~Component() exception: name=%s,id=%ld-%s",_name.c_str(),_id,e.what());
        }
    }

    void Component::setActive(bool active)
    {
        _active = active;
    }

    bool Component::isActive() const
    {
        return _active;
    }

    void Component::setName(const std::string& name)
    {
        _name = name;
    }

    std::string Component::getName() const
    {
        return _name;
    }

    long Component::getId() const
    {
        return _id;
    }

    void Component::setNode(Node* pnode)
    {
        if(_pnode==pnode)
        {
            return;
        }
        if(_pnode!=nullptr)
        {
            Log::error("Component::setNode() _pnode!=nullptr: name=%s,id=%ld",_name.c_str(),_id);
            return;
        }
        _pnode = pnode;
        if(_pnode!=nullptr)
        {
            try
            {
                _onLoad();
            }
            catch(const std::exception& e)
            {
                Log::error("Component::setNode() exception: name=%s,id=%ld-%s",_name.c_str(),_id,e.what());
            }
        }
        else
        {
            delete this;
        }
    }

    Node* Component::getNode() const
    {
        return _pnode;
    }

    void Component::removeComponent(bool cleanup)
    {
        if(_pnode!=nullptr)
        {
            _pnode->removeComponent(_id);
            if(!cleanup)
            {
                _pnode=nullptr;
            }
        }
    }

    void Component::update(float dt)
    {
    }

    void Component::fixedUpdate(float dt)
    {
    }

    void Component::_onLoad()
    {
    }

    void Component::_onDestory()
    {
    }
}