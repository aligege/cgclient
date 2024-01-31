#ifndef __JSONOBJECT_H__
#define __JSONOBJECT_H__

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
        ~jsonobject();
    protected:
        void* data=nullptr;
        EJsonType type=EJsonType_Null;
    public:
        void setBool(bool value);
        void setInt(int value);
        void setLong(long value);
        void setFloat(float value);
        void setDouble(double value);
        void setString(const char* value);
        void setArray();
        void setObject();
        void setNull();
        bool getBool();
        int getInt();
        long getLong();
        float getFloat();
        double getDouble();
        const char* getString();
        jsonobject* getArray();
        jsonobject* getObject();
        EJsonType getType();
        void clear();
        void print();
        void parse(const char* jsonStr);
        const char* toString();
    };
}
#endif // __JSONOBJECT_H__