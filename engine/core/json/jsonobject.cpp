#include"jsonobject.h"
#include<string>
#include<vector>
#include<unordered_map>
#include<string.h>

namespace cg
{
    jsonobject::jsonobject(const char *key)
    {
        this->key = key;
        this->data = nullptr;
        this->type = EJsonType_Null;
    }
    jsonobject::~jsonobject()
    {
        clear();
    }
    void jsonobject::setBool(bool value)
    {
        clear();
        data = new bool(value);
        type = EJsonType_Bool;
    }
    void jsonobject::setInt(int value)
    {
        clear();
        data = new int(value);
        type = EJsonType_Int;
    }
    void jsonobject::setLong(long value)
    {
        clear();
        data = new long(value);
        type = EJsonType_Long;
    }
    void jsonobject::setFloat(float value)
    {
        clear();
        data = new float(value);
        type = EJsonType_Float;
    }
    void jsonobject::setDouble(double value)
    {
        clear();
        data = new double(value);
        type = EJsonType_Double;
    }
    void jsonobject::setString(const char *value)
    {
        clear();
        data = new std::string(value);
        type = EJsonType_String;
    }
    void jsonobject::setNull()
    {
        clear();
        data = nullptr;
        type = EJsonType_Null;
    }
    void jsonobject::addToObject(jsonobject *value)
    {
        if (type != EJsonType_Object)
        {
            clear();
        }
        if(this->data==nullptr)
        {
            this->data=new std::unordered_map<std::string, jsonobject *>();
        }
        ((std::unordered_map<std::string, jsonobject *> *)data)->insert(std::make_pair(value->key, value));
        type == EJsonType_Object;
    }
    void jsonobject::addToArray(jsonobject *value)
    {
        if (type != EJsonType_Array)
        {
            clear();
        }
        if(this->data==nullptr)
        {
            this->data=new std::vector<jsonobject *>();
        }
        ((std::vector<jsonobject *> *)data)->push_back(value);
        type == EJsonType_Array;
    }
    bool jsonobject::getBool()
    {
        if (type == EJsonType_Bool)
        {
            return *(bool *)data;
        }
        return false;
    }
    int jsonobject::getInt()
    {
        if (type == EJsonType_Int)
        {
            return *(int *)data;
        }
        return 0;
    }
    long jsonobject::getLong()
    {
        if (type == EJsonType_Long)
        {
            return *(long *)data;
        }
        return 0;
    }
    float jsonobject::getFloat()
    {
        if (type == EJsonType_Float)
        {
            return *(float *)data;
        }
        return 0;
    }
    double jsonobject::getDouble()
    {
        if (type == EJsonType_Double)
        {
            return *(double *)data;
        }
        return 0;
    }
    const char *jsonobject::getString()
    {
        if (type == EJsonType_String)
        {
            return ((std::string *)data)->c_str();
        }
        return "";
    }
    jsonobject *jsonobject::getArray()
    {
        if (type == EJsonType_Array)
        {
            return (jsonobject *)data;
        }
        return nullptr;
    }
    jsonobject *jsonobject::getObject()
    {
        if (type == EJsonType_Object)
        {
            return (jsonobject *)data;
        }
        return nullptr;
    }
    EJsonType jsonobject::getType()
    {
        return type;
    }
    void jsonobject::clear()
    {
        if (type == EJsonType_Bool)
        {
            delete (bool *)data;
        }
        else if (type == EJsonType_Int)
        {
            delete (int *)data;
        }
        else if (type == EJsonType_Long)
        {
            delete (long *)data;
        }
        else if (type == EJsonType_Float)
        {
            delete (float *)data;
        }
        else if (type == EJsonType_Double)
        {
            delete (double *)data;
        }
        else if (type == EJsonType_String)
        {
            delete (std::string *)data;
        }
        else if (type == EJsonType_Array)
        {
            std::vector<jsonobject *> *array = (std::vector<jsonobject *> *)data;
            for (auto it = array->begin(); it != array->end(); it++)
            {
                delete *it;
            }
            delete array;
        }
        else if (type == EJsonType_Object)
        {
            std::unordered_map<std::string, jsonobject *> *object = (std::unordered_map<std::string, jsonobject *> *)data;
            for (auto it = object->begin(); it != object->end(); it++)
            {
                delete it->second;
            }
            delete object;
        }
        data = nullptr;
        type = EJsonType_Null;
    }
    const char *jsonobject::toString()
    {
        std::string key=this->key;
        std::string value="";
        if (type == EJsonType_Bool)
        {
            value = (*(bool *)data) ? "true" : "false";
        }
        else if (type == EJsonType_Int)
        {
            value = std::to_string(*(int *)data);
        }
        else if (type == EJsonType_Long)
        {
            value = std::to_string(*(long *)data);
        }
        else if (type == EJsonType_Float)
        {
            value = std::to_string(*(float *)data);
        }
        else if (type == EJsonType_Double)
        {
            value =std::to_string(*(double *)data);
        }
        else if (type == EJsonType_String)
        {
            value = *(std::string *)data;
        }
        else if (type == EJsonType_Array)
        {
            value = "[";
            std::vector<jsonobject *> *array = (std::vector<jsonobject *> *)data;
            for (auto it = array->begin(); it != array->end(); it++)
            {
                value += (*it)->toString();
                value += ",";
            }
            value += ']';
        }
        else if (type == EJsonType_Object)
        {
            value = "{";
            std::unordered_map<std::string, jsonobject *> *object = (std::unordered_map<std::string, jsonobject *> *)data;
            for (auto it = object->begin(); it != object->end(); it++)
            {
                value += it->second->toString();
                value += ",";
            }
            value += '}';
        }
        else if (type == EJsonType_Null)
        {
            value = "null";
        }
        std::string result=key+":"+value;
        return result.c_str();
    }
}