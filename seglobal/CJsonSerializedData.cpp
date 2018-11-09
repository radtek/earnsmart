/* 
 * File:   CJsonSerializedData.cpp
 * Author: santony
 * 
 * Created on March 22, 2013, 11:10 PM
 */

#include "CJsonSerializedData.h"
#include "Object.h"
#include "CString.h"
#include <string.h>

using namespace se;

CJsonSerializedData::CJsonSerializedData(ISession* ps):CObjectRoot(ps), _len(-1)
{
}

CJsonSerializedData::~CJsonSerializedData() 
{
}

void CJsonSerializedData::set_TypeName(CSTR typeName) 
{
    root["TypeName"] = typeName;
}

CSTR CJsonSerializedData::get_TypeName()
{
    return root["TypeName"].asCString();
}

ErrorId CJsonSerializedData::get_Output(IString** ppOut) 
{
    SP<CString> str;
    auto ret = CObject<CString>::Create(get_Session(), &str);
    
    if (!_formatted)
    {
        Json::FastWriter writer;
        str->set_Buffer( writer.write(root).c_str());
    }
    else
    {
        Json::StyledWriter writer;
        str->set_Buffer( writer.write(root).c_str());
    }
    str.CopyTo(ppOut);
    return ret;
}

void CJsonSerializedData::Add(CSTR propertyName, const BOOL propertyValue)
{
    root[propertyName] = propertyValue;
}

void CJsonSerializedData::Add(CSTR propertyName, CSTR propertyValue)
{
    if (!propertyValue || !strlen(propertyValue))
        root[propertyName] = "";
    else
        root[propertyName] = propertyValue;
}

void CJsonSerializedData::Add(CSTR propertyName, const CHR propertyValue)
{
    root[propertyName] = propertyValue;
}

void CJsonSerializedData::Add(CSTR propertyName, const I16 propertyValue)
{
    root[propertyName] = propertyValue;
}

void CJsonSerializedData::Add(CSTR propertyName, const I32 propertyValue)
{
    root[propertyName] = propertyValue;
}

void CJsonSerializedData::Add(CSTR propertyName, const I64 propertyValue)
{
    root[propertyName] = (Json::Int64)propertyValue;
}

void CJsonSerializedData::Add(CSTR propertyName, const U8 propertyValue)
{
    root[propertyName] = propertyValue;
}

void CJsonSerializedData::Add(CSTR propertyName, const U16 propertyValue)
{
    root[propertyName] = propertyValue;
}

void CJsonSerializedData::Add(CSTR propertyName, const U32 propertyValue)
{
    root[propertyName] = propertyValue;
}

void CJsonSerializedData::Add(CSTR propertyName, const U64 propertyValue)
{
    root[propertyName] = (Json::UInt64) propertyValue;
}

void CJsonSerializedData::Add(CSTR propertyName, const R4 propertyValue)
{    
    root[propertyName] = propertyValue;
}

void CJsonSerializedData::Add(CSTR propertyName, const R8 propertyValue)
{
    root[propertyName] = propertyValue;
}

void CJsonSerializedData::Add(CSTR propertyName, const IString* propertyValue)
{
    root[propertyName] = ((CString*) propertyValue)->GetInternalBuffer();
}

void CJsonSerializedData::Add(CSTR propertyName, ISerialization* propertyValue)
{
    if (!propertyValue) return ;
    
    // throw exception if failed.
    SP<CJsonSerializedData> local;
    if (S_Success != CObject<CJsonSerializedData>::Create(get_Session(), &local))
        throw std::bad_alloc();
    propertyValue->Serialize( (ISerializedData<IString>*) local._ptr);
    root[propertyName] = local->root;
}

void CJsonSerializedData::AddList(CSTR propertyName, ISerialization* pList)
{
    _tmp = root[propertyName];
    _len = 0;
    
    // we support only IList, IROList, ISPList or IROSPList
    // cast it
    
    
    
}

void CJsonSerializedData::AddListItem(ISerialization* pItem) 
{
    if (!pItem) return;
    SP<CJsonSerializedData> local;
    if (S_Success != CObject<CJsonSerializedData>::Create(get_Session(), &local))
	throw std::bad_alloc();
    pItem->Serialize((ISerializedData<IString>*)local._ptr);
    _tmp[_len++] = local->root; 
}


ErrorId CJsonSerializedData::Create(ISession* ps, ISerializedData<>** ppOut, bool formatted)
{
    return CObject<CJsonSerializedData>::Create(ps, formatted, ppOut);
}

ErrorId CJsonSerializedData::Create(ISession* ps, const char* jsonstring, ISerializedData<>** ppOut, bool formatted)
{
    Json::Value value;
    Json::Reader reader;
    if (!reader.parse(jsonstring, value, false))
        return E_NotJsonDeserializable;
    return CObject<CJsonSerializedData>::Create(ps, value, formatted, ppOut);
}

ErrorId CJsonSerializedData::Init(bool formatted)
{
    _formatted = formatted;
    return S_Success;
}

ErrorId CJsonSerializedData::Init(const Value& jsonobject, bool formatted)
{
    _formatted = formatted;
    root = jsonobject;
    return S_Success;
}

BOOL CJsonSerializedData::getBool(CSTR propertyName)
{
    return root[propertyName].asBool();
}

CSTR CJsonSerializedData::getString(CSTR propertyName)
{
    return root[propertyName].asCString();
}

CHR CJsonSerializedData::getChar(CSTR propertyName)
{
    return root[propertyName].asInt();
}

I16 CJsonSerializedData::getShort(CSTR propertyName)
{
    return root[propertyName].asInt();
    
}

I32 CJsonSerializedData::getInt32(CSTR propertyName)
{
    return root[propertyName].asInt();
    
}

I64 CJsonSerializedData::getInt64(CSTR propertyName)
{
    return root[propertyName].asInt64();
    
}

U8  CJsonSerializedData::getUChar(CSTR propertyName)
{
    return root[propertyName].asInt();
    
}

U16 CJsonSerializedData::getUShort(CSTR propertyName)
{
    return root[propertyName].asUInt();
    
}

U32 CJsonSerializedData::getUInt32(CSTR propertyName)
{
    return root[propertyName].asUInt();
    
}

U64 CJsonSerializedData::getUInt64(CSTR propertyName)
{
    return root[propertyName].asUInt64();
    
}

R4 CJsonSerializedData::getSingleReal(CSTR propertyName)
{
    return root[propertyName].asFloat();
    
}

R8 CJsonSerializedData::getDoubleReal(CSTR propertyName)
{
    return root[propertyName].asDouble();
}

ErrorId CJsonSerializedData::getString(CSTR propertyName, IString** ppOut)
{
    return CObject<CString>::Create(this->get_Session(), root[propertyName].asCString(), ppOut);
}

ErrorId CJsonSerializedData::getObject(CSTR propertyName, IDeserialization* pInOut)
{
    if (!pInOut)
        return E_InvalidArg;
    Value objectData = root[propertyName];
    SP<ISerializedData<>> local;
    ErrorId ret = CObject<CJsonSerializedData>::Create( this->get_Session(), objectData, &local);
    if (ret != S_Success)
        return ret;
    ret = pInOut->Deserialize(local);
    return ret;
}

bool CJsonSerializedData::get_IsFormatted()
{
    return _formatted;
}

void CJsonSerializedData::Reset()
{
    root.clear();
}
