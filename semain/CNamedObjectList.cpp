/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CNamedObjectList.cpp
 * Author: santony
 * 
 * Created on December 29, 2015, 7:08 PM
 */

#include <mutex>

#include "semaininternal.h"
#include "CNamedObjectList.h"

CNamedObjectList::CNamedObjectList(ISession* ps) : CObjectRoot(ps)
{
}

CNamedObjectList::~CNamedObjectList()
{
}

RP<INamedObject> CNamedObjectList::get(NamedObjectNamingType name)
{
    auto find = _map.find(name);
    if (find != _map.end())
    {
        // lock set method in case iterator get modified.
        std::lock_guard<std::recursive_mutex> g(this->get_Session()->get_LockObject());
        return find->second;
    }
    throw seexception(E_NotFound, "not found.");
}

long unsigned int CNamedObjectList::set(NamedObjectNamingType name, INamedObject* instance)
{
    // lock if not found
    auto find = _map.find(name);
    if (find == _map.end())
    {
        std::lock_guard<std::recursive_mutex> g(this->get_Session()->get_LockObject());
        _map.insert(make_pair(name, instance));
        return S_OK;
    }
    return S_DataExists;
}

long unsigned int CNamedObjectList::remove(NamedObjectNamingType name)
{
    std::unique_lock<std::recursive_mutex> g(this->get_Session()->get_LockObject(), try_to_lock_t());
    if (g.try_lock())
    {
        auto find = _map.find(name);
        if (find != _map.end())
        {
            _map.erase(find);
        }    
        this->get_Session()->get_LockObject().unlock();
        return S_OK;
    }
    return E_MutexLock;
}

