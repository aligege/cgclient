#include"jsonobject.h"
#include<string>
#include<vector>
#include<unordered_map>

namespace cg
{
    jsonobject::jsonobject()
    {
        data = nullptr;
        type = EJsonType_Null;
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
    void jsonobject::setArray()
    {
        clear();
        data = new std::vector<jsonobject *>();
        type = EJsonType_Array;
    }
    void jsonobject::setObject()
    {
        clear();
        data = new std::unordered_map<std::string, jsonobject *>();
        type = EJsonType_Object;
    }
    void jsonobject::setNull()
    {
        clear();
        data = nullptr;
        type = EJsonType_Null;
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
    void jsonobject::print()
    {
        if (type == EJsonType_Bool)
        {
            printf("%s", (*(bool *)data) ? "true" : "false");
        }
        else if (type == EJsonType_Int)
        {
            printf("%d", *(int *)data);
        }
        else if (type == EJsonType_Long)
        {
            printf("%ld", *(long *)data);
        }
        else if (type == EJsonType_Float)
        {
            printf("%f", *(float *)data);
        }
        else if (type == EJsonType_Double)
        {
            printf("%lf", *(double *)data);
        }
        else if (type == EJsonType_String)
        {
            printf("%s", ((std::string *)data)->c_str());
        }
        else if (type == EJsonType_Array)
        {
            printf("[");
            std::vector<jsonobject *> *array = (std::vector<jsonobject *> *)data;
            for (auto it = array->begin(); it != array->end(); it++)
            {
                (*it)->print();
                if (it != array->end() - 1)
                {
                    printf(",");
                }
            }
            printf("]");
        }
        else if (type == EJsonType_Object)
        {
            printf("{");
            std::unordered_map<std::string, jsonobject *> *object = (std::unordered_map<std::string, jsonobject *> *)data;
            for (auto it = object->begin(); it != object->end(); it++)
            {
                printf("\"%s\":", it->first.c_str());
                it->second->print();
                if (it != object->end())
                {
                    printf(",");
                }
            }
            printf("}");
        }
        else if (type == EJsonType_Null)
        {
            printf("null");
        }
    }
    void jsonobject::parse(const char *jsonStr)
    {
        clear();
        if (jsonStr == nullptr)
        {
            return;
        }
        int len = strlen(jsonStr);
        if (len == 0)
        {
            return;
        }
        int index = 0;
        while (index < len)
        {
            if (jsonStr[index] == '{')
            {
                index++;
                setObject();
                while (index < len)
                {
                    if (jsonStr[index] == '}')
                    {
                        index++;
                        break;
                    }
                    else if (jsonStr[index] == ',')
                    {
                        index++;
                    }
                    else if (jsonStr[index] == '"')
                    {
                        index++;
                        std::string key;
                        while (index < len)
                        {
                            if (jsonStr[index] == '"')
                            {
                                index++;
                                break;
                            }
                            else
                            {
                                key += jsonStr[index];
                                index++;
                            }
                        }
                        if (index < len)
                        {
                            if (jsonStr[index] == ':')
                            {
                                index++;
                                jsonobject *value = new jsonobject();
                                value->parse(jsonStr + index);
                                index += strlen(value->toString());
                                ((std::unordered_map<std::string, jsonobject *> *)data)->insert(std::make_pair(key, value));
                            }
                        }
                    }
                }
            }
            else if (jsonStr[index] == '[')
            {
                index++;
                setArray();
                while (index < len)
                {
                    if (jsonStr[index] == ']')
                    {
                        index++;
                        break;
                    }
                    else if (jsonStr[index] == ',')
                    {
                        index++;
                    }
                    else
                    {
                        jsonobject *value = new jsonobject();
                        value->parse(jsonStr + index);
                        index += strlen(value->toString());
                        ((std::vector<jsonobject *> *)data)->push_back(value);
                    }
                }
            }
            else if (jsonStr[index] == '"')
            {
                index++;
                std::string value;
                while (index < len)
                {
                    if (jsonStr[index] == '"')
                    {
                        index++;
                        break;
                    }
                    else
                    {
                        value += jsonStr[index];
                        index++;
                    }
                }
                setString(value.c_str());
            }
            else if (jsonStr[index
}