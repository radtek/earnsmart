/* 
 * File:   CJsonSerializedData.h
 * Author: santony
 *
 * Created on March 22, 2013, 11:10 PM
 */

#ifndef CJSONSERIALIZEDDATA_H
#define	CJSONSERIALIZEDDATA_H

#include "IObject.h"
#include "CObjectRoot.h"
#include "Object.h"
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>

using namespace Json;

namespace se
{
    class CJsonSerializedData : public CObjectRoot, public ISerializedData<>
    {
    public:
        CJsonSerializedData(ISession*);
        CJsonSerializedData(const CJsonSerializedData& orig) = delete;
        virtual ~CJsonSerializedData();
        
        virtual CSTR get_TypeName() ;
        virtual void set_TypeName(CSTR) ;
        
        virtual void Add(CSTR propertyName, const BOOL propertyValue) ;
        virtual void Add(CSTR propertyName, CSTR propertyValue) ;
        virtual void Add(CSTR propertyName, const CHR propertyValue) ;
        virtual void Add(CSTR propertyName, const I16 propertyValue) ;
        virtual void Add(CSTR propertyName, const I32 propertyValue) ;
        virtual void Add(CSTR propertyName, const I64 propertyValue) ;
        virtual void Add(CSTR propertyName, const U8 propertyValue) ;
        virtual void Add(CSTR propertyName, const U16 propertyValue) ;
        virtual void Add(CSTR propertyName, const U32 propertyValue) ;
        virtual void Add(CSTR propertyName, const U64 propertyValue) ;
        virtual void Add(CSTR propertyName, const R4 propertyValue) ;
        virtual void Add(CSTR propertyName, const R8 propertyValue) ;
        virtual void Add(CSTR propertyName, const IString* propertyValue) ;
        virtual void Add(CSTR propertyName, ISerialization* propertyValue) ;
        virtual void AddList(CSTR propertyName, ISerialization* propertyValue);
        virtual void AddListItem(ISerialization* value);
        
        virtual BOOL getBool(CSTR propertyName);
        virtual CSTR getString(CSTR propertyName);
        virtual CHR getChar(CSTR propertyName);
        virtual I16 getShort(CSTR propertyName);
        virtual I32 getInt32(CSTR propertyName);
        virtual I64 getInt64(CSTR propertyName);
        virtual U8  getUChar(CSTR propertyName);
        virtual U16 getUShort(CSTR propertyName);
        virtual U32 getUInt32(CSTR propertyName);
        virtual U64 getUInt64(CSTR propertyName);
        virtual R4 getSingleReal(CSTR propertyName);
        virtual R8 getDoubleReal(CSTR propertyName);
        virtual ErrorId getString(CSTR propertyName, IString** ppOut);
        
        virtual ErrorId getObject(CSTR propertyName, IDeserialization* pInOut);

        virtual ErrorId get_Output(IString** ppOut) ;
        
        virtual bool get_IsFormatted();
        
        virtual void Reset();
        
    public:
        virtual ErrorId Init(bool formatted = false);
        virtual ErrorId Init(const Value& jsonobject, bool formatted = false);
        
    public:
        // Create an instance of serializer. This can be used to store a serialized json text.
        static ErrorId Create(ISession*,  ISerializedData<>** ppOut, bool formatted = false);
        // Create an instance of serializer from a json serialized text (first parameter). This output (ppOut)
        // then can be used to deserilize the json text to an object.
        static ErrorId Create(ISession*, CSTR jsonstring, ISerializedData<>** ppOut, bool formatted = false);
        
    private:
        Json::Value root;
        bool _formatted;
        Json::Value _tmp;
        I32 _len;

    };
}

#endif	/* CJSONSERIALIZEDDATA_H */

