/* 
 * File:   CObjectRoot.cpp
 * Author: santony
 * 
 * Created on July 2, 2012, 7:28 AM
 */
#include "../ReturnCodes.h"
#include "CObjectRoot.h"
#include "IObject.h"

std::recursive_mutex CObjectRoot::lock;

CObjectRoot::CObjectRoot(se::ISession* ps)  : _uId(0), _properties(0), _ps(ps)
{
    std::atomic_store(&_rc, (U16)0);
}

CObjectRoot::CObjectRoot(const CObjectRoot& orig) 
{
    //_refCount = 0;
    std::atomic_store(&_rc, (U16)0);
    _uId = orig._uId;
}

CObjectRoot::CObjectRoot(CObjectRoot&& other)
{
//    {
//    __sync_synchronize();
//    _refCount = 0;
//    other._refCount = 0;
//    }
 
    _ps = other._ps;
    std::atomic_store(&_rc, (U16)0);
    _uId = other._uId;
    other._uId = 0;
}

CObjectRoot::~CObjectRoot() 
{
    if (_properties)
        delete _properties;
}

void CObjectRoot::_AddRef()
{
    _rc++;
#ifdef __DEBUG__
    this->OnAdd(_rc);
#endif
}

void CObjectRoot::_DecRef()
{
    _rc--;
#ifdef __DEBUG__
    this->OnDec(_rc);
#endif
    if (0 == _rc)
        delete this;
}

se::OmniType CObjectRoot::_get_Property(const char* key) const
{
    if (!_properties)
        return nopropvalue;
    auto it = _properties->find(key);
    if (it != _properties->end())
        return it->second;
    return nopropvalue;
}


bool CObjectRoot::_set_Property(const char* key, const se::OmniType&  value) 
{
    if (!_properties)
    {
        std::lock_guard<recursive_mutex> g(lock);
        if (!_properties)
            _properties = new Properties();
    }
        
    auto it = _properties->find(key);
    if (it == _properties->end())
    {
        std::lock_guard<std::recursive_mutex> g(lock);
        _properties->insert(make_pair((string)key, value));
        return true;
    }
    
    (*_properties)[key] = value;
    
    return false;
}


bool CObjectRoot::_Equals(se::IObject* other)
{
    return _get_Id() == other->get_Id();
}

ErrorId CObjectRoot::Init()
{
    return S_Success;
}

UID CObjectRoot::_get_Id() const
{
    return _uId;
}

void CObjectRoot::_set_Id(UID id)
{
    _uId = id;
}

se::ISession* CObjectRoot::get_Session()
{
    return _ps;
}

void CObjectRoot::Acquire()
{
    _AddRef();
}

void CObjectRoot::Release()
{
    _DecRef();
}

