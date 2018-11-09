/* 
 * File:   CFunctionCache.cpp
 * Author: santony
 * 
 * Created on October 10, 2013, 8:44 PM
 */

#include "CFunctionCache.h"
#include "exceptions.h"
#include <unordered_map>
#include <malloc.h>
#include "globallogger.h"

namespace se
{
    namespace cache
    {
        
        CFunctionCache* CFunctionCache::_pCache = nullptr; 
        int CFunctionCache::DefaultTimeOut = 600;     //in seconds => 10 mins (60 secs * 10)
        bool CFunctionCache::_bIsInit = false;
        std::recursive_mutex CFunctionCache::_mutex;
        const I32 CFunctionCache::INFINITETIME =  -1;
        
        CFunctionCache::CFunctionCache() : 
                _FuncCache(FUNCTIONCACHE()),
                _timer(60, 60)
        {
            _timer.SetState(this);
            _timer.SetCallback([](CTimer const* timer, LPVOID state)
            {
                static_cast<CFunctionCache*>(state)->OnTimerCallback();
            });
            _timer.StartTimer();
        }

        void CFunctionCache::OnTimerCallback()
        {
            if (!_bIsInit) 
                return;
            
            std::lock_guard<recursive_mutex> g(_mutex);
            for (auto it0 = _FuncCache.begin(); it0 != _FuncCache.end();)
            {
                auto funcMap = (std::unordered_map<size_t, FuncValueTimeWrapperBase*> *)it0->second;
                for (auto it1 = funcMap->begin(); it1 != funcMap->end();)
                {
                    bool b = it1->second->HasExpired();
                    if (b)
                    {
                        //LOG4CXX_INFO(_logger,  "Function " << it0->first << " has  expired. Removing it from cache memory.");
                        delete it1->second;
                        it1 = funcMap->erase(it1);
                    }
                    else
                    {
                        //LOG4CXX_INFO(_logger, "Function " << it0->first << " has not expired.");
                        it1++;
                    }
                }
                if (!funcMap->size())
                {
                    it0 = _FuncCache.erase(it0);
                    delete funcMap;
                    // free some memory
                    malloc_trim(0);
                }
                else
                {
                    it0++;
                }
            }
        }
        
        CFunctionCache CFunctionCache::operator =(const CFunctionCache& other)
        {
            
        }

        CFunctionCache::~CFunctionCache() 
        {
            _timer.StopTimer();
            std::lock_guard<recursive_mutex> g(_mutex);
            for (auto p : _FuncCache)
            {
                //LOG4CXX_INFO(_logger, "Freeing " << p.first);
                auto funcMap = (std::unordered_map<size_t, FuncValueTimeWrapperBase*> *)p.second;
                for (auto p1 : *funcMap)
                {
                    delete p1.second;
                }
                delete (std::unordered_map<size_t, FuncValueTimeWrapperBase*> *)p.second;
            }
        }
        
        CFunctionCache & CFunctionCache::Instance() 
        {
            if (!_bIsInit)
            {
                throw seexception("Function cache is not intialized. Ensure that an instance of IApp is created using SeGetApp method at your process startup method such as main method.");
            }
            // double check lock
            if (!_pCache)
            {
                // lock 
                //pthread_mutex_lock(&_lock);
                std::lock_guard<recursive_mutex> g(_mutex);
                // check agagin
                if (!_pCache)
                {
                    _pCache = new CFunctionCache();
                }
                //pthread_mutex_unlock(&_lock);
            }
            return *_pCache;
        }
        
        void CFunctionCache::Init()
        {
            if (!_bIsInit)
            {
                // initialize mutex
                //pthread_mutexattr_settype(&_attr, PTHREAD_MUTEX_RECURSIVE);
                //pthread_mutex_init(&_lock, &_attr);
                _bIsInit = true;
            }
        }
        
        void CFunctionCache::Free()
        {
            if (_bIsInit)
            {
                
                // delete is already thread-safe
                delete _pCache;
                
                // destroy lock and attr;
                //pthread_mutex_destroy(&_lock);
                //pthread_mutexattr_destroy(&_attr);
                
                
                _bIsInit = false;
            }
        }
        
        // funcvaluetimewrapperbase
        
        CFunctionCache::FuncValueTimeWrapperBase::FuncValueTimeWrapperBase(bool sptype) 
                : SPType(sptype)
        {
            ExtendLease();
        }
        
        CFunctionCache::FuncValueTimeWrapperBase::~FuncValueTimeWrapperBase()
        {
            
        }
        
        void CFunctionCache::FuncValueTimeWrapperBase::ExtendLease()
        {
            std::lock_guard<recursive_mutex> g(_mutex);
            _clk = time(0);
        }
        
        bool CFunctionCache::FuncValueTimeWrapperBase::HasExpired()
        {
            if (timeout == INFINITETIME)
                return false;
            
            // if the difference is more than 10 mins, return true.
            double d = difftime( time(0), _clk );
            return d >= (double)timeout;
        }
    }
}