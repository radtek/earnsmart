/* 
 * File:   CString.cpp
 * Author: santony
 * 
 * Created on September 22, 2012, 10:11 PM
 */

#include "seglobalinternal.h"
#include "IObject.h"
#include "CObjectRoot.h"
#include "Object.h"
#include "CString.h"

CString::CString(ISession* ps):CObjectRoot(ps)
{
}

CString::CString(const CString& orig) : CObjectRoot(orig)
{
    _buffer = orig._buffer;
}

CString::~CString() 
{
}

ErrorId CString::Init(const char* str)
{
    _buffer = str;
    return S_Success;
}

CSTR CString::get_Buffer() 
{
    return _buffer.c_str();
}

string CString::GetInternalBuffer()
{
    return std::move(_buffer);
}

void CString::set_Buffer(CSTR str) 
{
    _buffer = str;
}

SIZE CString::get_Length() 
{
    return _buffer.length();
}

bool CString::get_IsEmpty() 
{
    return _buffer.empty();
}

void CString::Clear() 
{
    _buffer.clear();
}

void CString::Assign(CSTR str) 
{
    _buffer = str;
}

void CString::Replace(CSTR replace, CSTR with) 
{
    _buffer.replace(_buffer.begin(), _buffer.end(), replace, with);
}

SIZE CString::Find(CSTR str, SIZE startPosition) 
{
    return _buffer.find(str, startPosition);
}

SIZE CString::ReverseFind(CSTR str, SIZE startPosition) 
{
    return _buffer.rfind(str, startPosition);
}

ErrorId CString::Substring(SIZE pos, SIZE count, IString** outVal) 
{
     auto str = _buffer.substr(pos, count);
     return CObject<CString>::Create(get_Session(), str.c_str(), outVal);
}

I32 CString::Compare(CSTR otherString) 
{
    return _buffer.compare(otherString);
}

void CString::Append(CSTR str) 
{
    _buffer.append(str);
}

//void CString::Append(I8 data) 
//{
//    
//}
//
//void CString::Append(U8 data) 
//{
//    
//}
//
//void CString::Append(I16 data) 
//{
//    
//}
//
//void CString::Append(U16 data) 
//{
//    
//}
//
//void CString::Append(I32 data) 
//{
//    
//}
//
//void CString::Append(U32 data) 
//{
//    
//}
//
//void CString::Append(I64 data) 
//{
//    
//}
//
//void CString::Append(U64 data) 
//{
//    
//}
//
//void CString::Append(R4 data) 
//{
//    
//}
//
//void CString::Append(R8 data) 
//{
//    
//}

ErrorId IString::Create(CSTR defvalue, IString** ppOut)
{
    return CObject<CString>::Create(nullptr, defvalue, ppOut);
}