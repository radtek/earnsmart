/* 
 * File:   CObjectRoot.h
 * Author: santony
 *
 * Created on July 2, 2012, 7:28 AM
 */

#ifndef COBJECTROOT_H
#define	COBJECTROOT_H

#include "../seglobaltypedefs.h"
#include "IObject.h"
#include <unordered_map>
#include <atomic>
#include <mutex>

using namespace std;

class CObjectRoot 
{
public:
    CObjectRoot(){}
    CObjectRoot(se::ISession*);
    CObjectRoot(const CObjectRoot& orig);
    virtual ~CObjectRoot();
    
    // move constructor
    CObjectRoot(CObjectRoot&& other);

protected:
    std::atomic<U16> _rc;

private:
    UID _uId;
    // pointer to properties
    typedef unordered_map<string, se::OmniType> Properties;
    Properties* _properties;
    se::ISession* _ps = nullptr;
    const se::OmniType nopropvalue;
    static recursive_mutex lock;
    
public:
    void Acquire();
    void Release();

protected:
    /* internal unique identifier which usually maps to 
     primary key in the table*/
    UID _get_Id() const;
    void _set_Id(UID id);
    
    se::ISession* get_Session();

#ifdef __DEBUG__
    virtual void _AddRef();
    virtual void _DecRef();
    
    virtual void OnAdd(U16 rc){};
    virtual void OnDec(U16 rc){};
    
#else
    void _AddRef();
    void _DecRef();
#endif
    
    bool _set_Property(CSTR key, const se::OmniType&  value);
    se::OmniType _get_Property(CSTR key) const;
    
    bool _Equals(se::IObject* other);
    
    virtual ErrorId Init();
};

template <class T = CObjectRoot>
class Sharer
{
private:
    T _sharable;
public:
    Sharer(T sharable) : _sharable(sharable)
    {
        _sharable->Acquire();
    }
    ~Sharer()
    {
        _sharable->Release();
    }
};

#endif	/* COBJECTROOT_H */


