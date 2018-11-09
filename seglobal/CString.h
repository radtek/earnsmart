/* 
 * File:   CString.h
 * Author: santony
 *
 * Created on September 22, 2012, 10:11 PM
 */

#ifndef CSTRING_H
#define	CSTRING_H

#include <string>

using namespace std;
using namespace se;

class CString : public CObjectRoot, public se::IString
{
public:
    CString(ISession*);
    CString(const CString& orig);
    virtual ~CString();
    virtual ErrorId Init(CSTR str = "");
    virtual CSTR get_Buffer() ;
    virtual void set_Buffer(CSTR str) ;
    virtual SIZE get_Length() ;
    virtual bool get_IsEmpty() ;
    virtual void Clear() ;
    virtual void Assign(CSTR str) override ;
    virtual void Replace(CSTR replace, CSTR with) ;
    virtual SIZE Find(CSTR str, SIZE startPosition = 0) ;
    virtual SIZE ReverseFind(CSTR str, SIZE startPosition = 0) ;
    virtual ErrorId Substring(SIZE pos, SIZE count, IString** outVal) ;
    virtual I32 Compare(CSTR otherString) ;

    virtual void Append(CSTR str) ;
    /* this is an expensive operation when using stringstream and then copy to string variable. 
     We will have to find a better way.*/
//    virtual void Append(I8 data) ;
//    virtual void Append(U8 data) ;
//    virtual void Append(I16 data) ;
//    virtual void Append(U16 data) ;
//    virtual void Append(I32 data) ;
//    virtual void Append(U32 data) ;
//    virtual void Append(I64 data) ;
//    virtual void Append(U64 data) ;
//    virtual void Append(R4 data) ;
//    virtual void Append(R8 data) ;
    
    string GetInternalBuffer();
    
public:
    //static ErrorId Create(IString** ppOut, const char* def = 0) ;
    
private:
    string _buffer;
};

#endif	/* CSTRING_H */

