#ifndef VALUE_H
#define VALUE_H

#include <stdint.h>

#define NS_TYPE_NIL         0
#define NS_TYPE_INT         1
#define NS_TYPE_FLOAT       2
#define NS_TYPE_BOOL        3
#define NS_TYPE_STRING      4
#define NS_TYPE_OBJECT      5
#define NS_TYPE_ARRAY       6
#define NS_TYPE_FUNCTION    7

/*
 * If LSB == 1 -> INT
 * Else the low byte is a type tag shifted and the rest is the payload.
 */
typedef uint64_t NsValue;
typedef uint8_t  NsType;
typedef uint32_t NsObjectId;
typedef uint32_t NsArrayId;
typedef uint32_t NsFunctionId;
typedef uint32_t NsStringId;

inline static NsType nsType(NsValue value)
{
    if (value & 1)
        return NS_TYPE_INT;
    else
        return (value >> 1) & 0x7f;
}

/*
 * Do not use directly, use the helper functions instead.
 * Does not support small integers.
 */
inline static NsValue _nsMakeValue(NsType type, uint64_t payload)
{
    return (payload << 8) | (type << 1);
}

inline static NsValue nsMakeInt(int64_t i)
{
    return ((uint64_t)i << 1) | 1;
}

inline static NsValue nsMakeFloat(float f)
{
    return _nsMakeValue(NS_TYPE_FLOAT, *(uint32_t*)(&f));
}

inline static NsValue nsMakeBool(int b)
{
    return _nsMakeValue(NS_TYPE_BOOL, b ? 1 : 0);
}

inline static NsValue nsMakeObject(NsObjectId obj)
{
    return _nsMakeValue(NS_TYPE_OBJECT, obj);
}

inline static NsValue nsMakeArray(NsArrayId arr)
{
    return _nsMakeValue(NS_TYPE_ARRAY, arr);
}

inline static NsValue nsMakeFunction(NsFunctionId fun)
{
    return _nsMakeValue(NS_TYPE_FUNCTION, fun);
}

/*
 * Do not use directly, use the helper functions instead.
 * Does not support small integers.
 */
inline static uint64_t _nsValue(NsValue value)
{
    return (value >> 8);
}

inline static int64_t nsValueInt(NsValue value)
{
    uint64_t v = value >> 1;
    v = v | ((v & (1 << 62)) << 1);
    return v;
}

inline static float nsValueFloat(NsValue value)
{
    uint32_t payload = _nsValue(value);
    return *(float*)(&payload);
}

inline static int nsValueBool(NsValue value)
{
    return _nsValue(value) ? 1 : 0;
}

inline static NsObjectId nsValueObject(NsValue value)
{
    return _nsValue(value);
}

inline static NsArrayId nsValueArray(NsValue value)
{
    return _nsValue(value);
}

inline static NsFunctionId nsValueFunction(NsValue value)
{
    return _nsValue(value);
}

inline static int nsIsNil(NsValue value)
{
    return value ? 0 : 1;
}

inline static int nsIsTruthy(NsValue value)
{
    return (nsIsNil(value) || value == nsMakeBool(0)) ? 0 : 1;
}

#endif
