/* 
 * File:   CBsonSerializedData.cpp
 * Author: santony
 * 
 * Created on May 18, 2015, 8:35 PM
 */

#include "CBsonSerializedData.h"

using namespace se;

CBsonSerializedData::CBsonSerializedData(ISession* ps):CObjectRoot(ps) {
}

CBsonSerializedData::~CBsonSerializedData() {
}

const char* CBsonSerializedData::get_TypeName() {
    return "";
}

void CBsonSerializedData::set_TypeName(const char*) {

}


void CBsonSerializedData::Add(const char* propertyName, const bool propertyValue) {

}

void CBsonSerializedData::Add(CSTR propertyName, CSTR propertyValue) {}
void CBsonSerializedData::Add(CSTR propertyName, const CHR propertyValue) {}
void CBsonSerializedData::Add(CSTR propertyName, const I16 propertyValue) {}
void CBsonSerializedData::Add(CSTR propertyName, const I32 propertyValue) {}
void CBsonSerializedData::Add(CSTR propertyName, const I64 propertyValue) {}
void CBsonSerializedData::Add(CSTR propertyName, const U8 propertyValue) {}
void CBsonSerializedData::Add(CSTR propertyName, const U16 propertyValue) {}
void CBsonSerializedData::Add(CSTR propertyName, const U32 propertyValue) {}
void CBsonSerializedData::Add(CSTR propertyName, const U64 propertyValue) {}
void CBsonSerializedData::Add(CSTR propertyName, const R4 propertyValue) {}
void CBsonSerializedData::Add(CSTR propertyName, const R8 propertyValue) {}
void CBsonSerializedData::Add(CSTR propertyName, const IString* propertyValue) {}
void CBsonSerializedData::Add(CSTR propertyName, ISerialization* propertyValue) {}


void CBsonSerializedData::AddList(const char* propertyName, ISerialization* propertyValue) {

}

void CBsonSerializedData::AddListItem(ISerialization* value) {

}


BOOL CBsonSerializedData::getBool(CSTR propertyName){}
CSTR CBsonSerializedData::getString(CSTR propertyName){}
CHR CBsonSerializedData::getChar(CSTR propertyName){}
I16 CBsonSerializedData::getShort(CSTR propertyName){}
I32 CBsonSerializedData::getInt32(CSTR propertyName){}
I64 CBsonSerializedData::getInt64(CSTR propertyName){}
U8  CBsonSerializedData::getUChar(CSTR propertyName){}
U16 CBsonSerializedData::getUShort(CSTR propertyName){}
U32 CBsonSerializedData::getUInt32(CSTR propertyName){}
U64 CBsonSerializedData::getUInt64(CSTR propertyName){}
R4 CBsonSerializedData::getSingleReal(CSTR propertyName){}
R8 CBsonSerializedData::getDoubleReal(CSTR propertyName){}
ErrorId CBsonSerializedData::getString(CSTR propertyName, IString** ppOut){}

ErrorId CBsonSerializedData::getObject(CSTR propertyName, IDeserialization* pInOut){}

ErrorId CBsonSerializedData::get_Output(IString** ppOut) {}
        
bool CBsonSerializedData::get_IsFormatted(){}

void CBsonSerializedData::Reset(){}



