/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CLazy.h
 * Author: santony
 *
 * Created on March 8, 2016, 4:35 PM
 */

#ifndef CLAZY_H
#define CLAZY_H

#include <mutex>


template<typename T>
class CLazy
{
public:
    typedef std::function<T*()> LazyInitializer;
    
private:
    std::mutex _lock;
    T* _lazyObj = nullptr;
    LazyInitializer _finit;
    
public:
    
    CLazy(LazyInitializer finit) : _finit(finit)
    {
        
    }
    
    bool HasValue()
    {
        return _lazyObj != nullptr;
    }
    
    ~CLazy()
    {
        if (_lazyObj)
            delete _lazyObj;
    }
    
    T* Value()
    {
        if (!_lazyObj)
            std::lock_guard<std::mutex> g(_lock);
            if (!_lazyObj)
                _lazyObj = _finit();
        return _lazyObj;
    }
};


#endif /* CLAZY_H */

