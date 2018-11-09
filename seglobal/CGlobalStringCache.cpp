/* 
 * File:   CGlobalStringCache.cpp
 * Author: santony
 * 
 * Created on January 12, 2013, 5:17 PM
 */

#include "CGlobalStringCache.h"

namespace se
{
    
    CGlobalStringCache* CGlobalStringCache::_pCache = nullptr;
    
    CGlobalStringCache::CGlobalStringCache() 
    {
    }

    CGlobalStringCache::CGlobalStringCache(const CGlobalStringCache& orig) 
    {
    }

    CGlobalStringCache::~CGlobalStringCache() 
    {
    }

    const std::string& CGlobalStringCache::operator[](const std::string& str) 
    {
        auto find = _strcache.find(str);
        if (find == _strcache.end())
        {
            auto inserted = _strcache.insert(str);
            return *inserted.first;
        }
        return *find;
    }
    
    CGlobalStringCache& CGlobalStringCache::Get()
    {
        // TODO: double check lock
        if (!_pCache)
            _pCache = new CGlobalStringCache();
        return *_pCache;
    }

    void CGlobalStringCache::Free() throw()
    {
        delete _pCache ;
        _pCache = nullptr;
    }
    
    
}

