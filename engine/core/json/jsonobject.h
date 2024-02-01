#ifndef __JSONOBJECT_H__
#define __JSONOBJECT_H__
#include <string>
namespace cg
{
    enum EJsonType
    {
        EJsonType_Null,
        EJsonType_Bool,
        EJsonType_Int,
        EJsonType_Long,
        EJsonType_Float,
        EJsonType_Double,
        EJsonType_String,
        EJsonType_Array,
        EJsonType_Object
    };
    class jsonobject
    {
    public:
        jsonobject();
        jsonobject(const char* key);
        ~jsonobject();
    protected:
        std::string key;
        void* data=nullptr;
        EJsonType type=EJsonType_Null;
    public:
        void setBool(bool value);
        void setInt(int value);
        void setLong(long value);
        void setFloat(float value);
        void setDouble(double value);
        void setString(const char* value);
        void setNull();
        void addToObject(jsonobject* value);
        void addToArray(jsonobject* value);
        bool getBool();
        int getInt();
        long getLong();
        float getFloat();
        double getDouble();
        std::string& getString();
        jsonobject* getArray();
        jsonobject* getObject(const char* key);
        EJsonType getType();
        void clear();
        std::string toString();
    };
}
#endif // __JSONOBJECT_H__