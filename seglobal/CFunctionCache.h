/* 
 * File:   CFunctionCache.h
 * Author: Saji Antony
 *
 * Created on October 10, 2013, 8:44 PM
 * 
 * Purpose:
 * There are cases when an expensive function is called repeatedly. If the function's parameters
 * are not going to change and and the data returned by the function is going to be
 * same after each call with the same parameters, then it will be efficient if the return
 * value is stored in a cache and returned quickly to the caller. 
 * 
 * But the memory cache may grow over time. So we also need a policy on which
 * the cached returned value is cleared from memory when calls to the function 
 * with same parameters is not repeated with a given time interval. If the time
 * interval lapses, the memory will be cached. If the caller renews the function call,
 * the function will execute again to reload the returned value and will be cached again.
 *   
 */

#ifndef CFUNCTIONCACHE_H
#define	CFUNCTIONCACHE_H

#include <pthread.h>
#include <unordered_map>
#include <string>
#include <tuple>
#include "TupleHashCombiner.h"
#include "../seglobaltypedefs.h"
#include "CTimer.h"
#include "IObject.h"
#include <time.h>
#include <mutex>

using namespace std;

namespace se
{
    namespace cache
    {
        
        class CFunctionCache 
        {
            typedef std::unordered_map<string, LPVOID> FUNCTIONCACHE;
            
        public:
            static const I32 INFINITETIME;
            
        private:
            
            struct FuncValueTimeWrapperBase
            {
                time_t _clk;
                bool SPType;
                int timeout;
                
                // has expired. if returns true, the cached item will be removed.
                bool HasExpired();
                
                // extends lease expiration of the cache.
                void ExtendLease();
                
                FuncValueTimeWrapperBase(bool sptype);
                
                virtual ~FuncValueTimeWrapperBase();
                

            };
            
            typedef std::unordered_map<size_t, FuncValueTimeWrapperBase* > FUNCMAP;
            
            template<typename TRet>
            struct FuncValueTimeWrapper: public FuncValueTimeWrapperBase
            {
                TRet ReturnValue;
                
                FuncValueTimeWrapper() 
                {
                    FuncValueTimeWrapperBase::timeout = CFunctionCache::DefaultTimeOut;
                }

                FuncValueTimeWrapper(TRet& t, bool sptype) : FuncValueTimeWrapperBase(sptype), ReturnValue(t)
                {
                    FuncValueTimeWrapperBase::timeout = CFunctionCache::DefaultTimeOut;
                }
                
                virtual ~FuncValueTimeWrapper()                
                {
                }
            };

            template<typename TRet>
            struct SPFuncValueTimeWrapper: public FuncValueTimeWrapperBase
            {
                TRet ReturnValue;
                bool SPType;
                
                SPFuncValueTimeWrapper() 
                {
                    FuncValueTimeWrapperBase::timeout = CFunctionCache::DefaultTimeOut;
                }

                SPFuncValueTimeWrapper(TRet& t, bool sptype) : FuncValueTimeWrapperBase(sptype)
                {
                    FuncValueTimeWrapperBase::timeout = CFunctionCache::DefaultTimeOut;
                    ReturnValue = t;
                    ReturnValue->AddRef();
                }
                
                SPFuncValueTimeWrapper(TRet& t, bool sptype, int timeout) : FuncValueTimeWrapperBase(sptype)
                {
                    FuncValueTimeWrapperBase::timeout = timeout;
                    ReturnValue = t;
                    ReturnValue->AddRef();
                }
                
                virtual ~SPFuncValueTimeWrapper()                
                {
                    ReturnValue->DecRef();
                }
            };
            
        private:
            CFunctionCache();
            CFunctionCache operator=(const CFunctionCache& other);
            
            void OnTimerCallback();
            
        public:
            virtual ~CFunctionCache();
            
        public:
            
            // initialize global function cache.
            static void Init();
            
            // static method to obtain a singleton instance of
            // function cache
            static CFunctionCache & Instance() ;
            
            // free cache
            static void Free();
            
            // we will use some neat stuff latest c++ added
            // The tuple.
            // the parameter set is a tuple. That tuple is used to
            // to retrieve return value from cache
            // For each function type, we create an unordered map
            // based on a function name and parameter tuple.
            // That is then added to an unordered map.
            // This unordered map is saved in another unordered map 
            // with function name.


            // we will use some neat stuff latest c++ added
            // The tuple.
            // the parameter set is a tuple. That tuple is used to
            // to retrieve return value from cache
            // For each function type, we create an unordered map
            // based on a function name and parameter tuple.
            // That is then added to an unordered map.
            // This unordered map is saved in another unordered map 
            // with function name.

            /************************0 Params**************************/
            
            template<typename TRet>
            bool TryGetFValue(string const& funcName, TRet& ret)
            {
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                if (itFind != _FuncCache.end())
                {
                    m = static_cast<FUNCMAP*>(itFind->second);
                    auto itNext = m->find(0);
                    if (itNext != m->end())
                    {
                        FuncValueTimeWrapperBase* pBase = (FuncValueTimeWrapperBase*)itNext->second;
                        pBase->ExtendLease();
                        ret = ((FuncValueTimeWrapper<TRet>*)pBase)->ReturnValue;
                        return true;
                    }
                }
                return false;
            }

            template<typename TRet>
            void SetFValue(string const& funcName, TRet& ret)
            {
                
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                if (itFind == _FuncCache.end())
                {
                    std::lock_guard<recursive_mutex> g(_mutex);
                    m = new FUNCMAP();
                    _FuncCache[funcName] = (LPVOID) m;
                }
                else
                    m = static_cast<FUNCMAP*>(itFind->second);
                
                if (m)
                {
                    std::lock_guard<recursive_mutex> g(_mutex);
                    (*m)[0] = new FuncValueTimeWrapper<TRet>(ret, false);
                }
            }

            template<typename TRet>
            bool TrySPGetFValue(string const& funcName, TRet& ret)
            {

                // lock to be added later.
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                if (itFind != _FuncCache.end())
                {
                    m = static_cast<FUNCMAP*>(itFind->second);
                    auto itNext = m->find(0);
                    if (itNext != m->end())
                    {
                        FuncValueTimeWrapperBase* pBase = (FuncValueTimeWrapperBase*)itNext->second;
                        pBase->ExtendLease();
                        ret = ((SPFuncValueTimeWrapper<TRet>*)itNext->second)->ReturnValue;
                        return true;
                    }
                }
                return false;
            }

            template<typename TRet>
            void SPSetFValue(string const& funcName, TRet& ret)
            {
                
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                if (itFind == _FuncCache.end())
                {
                    std::lock_guard<recursive_mutex> g(_mutex);
                    m = new FUNCMAP();
                    _FuncCache[funcName] = (LPVOID) m;
                }
                else
                    m = static_cast<FUNCMAP*>(itFind->second);
                
                if (m)
                {
                    std::lock_guard<recursive_mutex> g(_mutex);
                    (*m)[0] = new SPFuncValueTimeWrapper<TRet>(ret, true);
                }
            }            
            
            template<typename TRet>
            void SPSetFValue(string const& funcName, TRet& ret, int timeout)
            {
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                if (itFind == _FuncCache.end())
                {
                    std::lock_guard<recursive_mutex> g(_mutex);
                    m = new FUNCMAP();
                    _FuncCache[funcName] = (LPVOID) m;
                }
                else
                    m = static_cast<FUNCMAP*>(itFind->second);
                
                if (m)
                {
                    std::lock_guard<recursive_mutex> g(_mutex);
                    (*m)[0] = new SPFuncValueTimeWrapper<TRet>(ret, true, timeout);
                }
            }            
            

            /************************1 Param**************************/
            
            template<typename TParam, typename TRet>
            bool TryGetFValue(string const& funcName, TParam const& param, TRet& ret)
            {
                
                // lock to be added later.
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                if (itFind != _FuncCache.end())
                {
                    std::hash<TParam> h;
                    size_t hs = h(param);
                    m = static_cast<FUNCMAP*>(itFind->second);
                    auto itNext = m->find(hs);
                    if (itNext != m->end())
                    {
                        FuncValueTimeWrapperBase* pBase = (FuncValueTimeWrapperBase*)itNext->second;
                        pBase->ExtendLease();
                        ret = ((FuncValueTimeWrapper<TRet>*)pBase)->ReturnValue;
                        return true;
                    }
                }
                return false;
            }

            template<typename TParam, typename TRet>
            void SetFValue(string const& funcName, TParam const& param, TRet& ret)
            {

                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                if (itFind == _FuncCache.end())
                {
                    std::lock_guard<recursive_mutex> g(_mutex);
                    m = new FUNCMAP();
                    _FuncCache[funcName] = (LPVOID) m;
                }
                else
                    m = static_cast<FUNCMAP*>(itFind->second);
                if (m)
                {
                    std::hash<TParam> h;
                    size_t hs = h(param);
                    std::lock_guard<recursive_mutex> g(_mutex);
                    (*m)[hs] = new FuncValueTimeWrapper<TRet>(ret, false);
                }
            }
            
            template<typename TParam, typename TRet>
            bool TrySPGetFValue(string const& funcName, TParam const& param, TRet& ret)
            {
                
                // lock to be added later.
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                if (itFind != _FuncCache.end())
                {
                    std::hash<TParam> h;
                    size_t hs = h(param);
                    m = static_cast<FUNCMAP*>(itFind->second);
                    auto itNext = m->find(hs);
                    if (itNext != m->end())
                    {
                        FuncValueTimeWrapperBase* pBase = (FuncValueTimeWrapperBase*)itNext->second;
                        pBase->ExtendLease();
                        ret = ((SPFuncValueTimeWrapper<TRet>*)itNext->second)->ReturnValue;
                        return true;
                    }
                }
                return false;
            }

            template<typename TParam, typename TRet>
            void SPSetFValue(string const& funcName, TParam const& param, TRet& ret)
            {
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                if (itFind == _FuncCache.end())
                {
                    std::lock_guard<recursive_mutex> g(_mutex);
                    m = new FUNCMAP();
                    _FuncCache[funcName] = (LPVOID) m;
                }
                else
                    m = static_cast<FUNCMAP*>(itFind->second);
                if (m)
                {
                    std::hash<TParam> h;
                    size_t hs = h(param);
                    std::lock_guard<recursive_mutex> g(_mutex);
                    (*m)[hs] = new SPFuncValueTimeWrapper<TRet>(ret, true);
                }
            }            
            
            template<typename TParam, typename TRet>
            void SPSetFValue(string const& funcName, TParam const& param, TRet& ret, int timeout)
            {
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                if (itFind == _FuncCache.end())
                {
                    std::lock_guard<recursive_mutex> g(_mutex);
                    m = new FUNCMAP();
                    _FuncCache[funcName] = (LPVOID) m;
                }
                else
                    m = static_cast<FUNCMAP*>(itFind->second);
                if (m)
                {
                    std::hash<TParam> h;
                    size_t hs = h(param);
                    std::lock_guard<recursive_mutex> g(_mutex);
                    (*m)[hs] = new SPFuncValueTimeWrapper<TRet>(ret, true, timeout);
                }
            }            
            
            
            /************************2 Params**************************/
            
            template<typename TParam0, typename TParam1, typename TRet>
            bool TryGetFValue(string const& funcName, TParam0 const& param0, TParam1 const& param1, TRet& ret)
            {
                
                // lock to be added later.
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                auto param = make_tuple(param0, param1);
                if (itFind != _FuncCache.end())
                {
                    std::hash<tuple<TParam0, TParam1>> h;
                    size_t hs = h(param);
                    m = static_cast<FUNCMAP*>(itFind->second);
                    auto itNext = m->find(hs);
                    if (itNext != m->end())
                    {
                        FuncValueTimeWrapperBase* pBase = (FuncValueTimeWrapperBase*)itNext->second;
                        pBase->ExtendLease();
                        ret = ((FuncValueTimeWrapper<TRet>*)pBase)->ReturnValue;
                        return true;
                    }
                }
                return false;
            }
           
            template<typename TParam0, typename TParam1, typename TRet>
            void SetFValue(string const& funcName, TParam0 const& param0, TParam1 const& param1, TRet& ret)
            {

                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                if (itFind == _FuncCache.end())
                {
                    std::lock_guard<recursive_mutex> g(_mutex);
                    m = new FUNCMAP();
                    _FuncCache[funcName] = (LPVOID) m;
                }
                else
                    m = static_cast<FUNCMAP*>(itFind->second);
                if (m)
                {
                    std::hash<tuple<TParam0, TParam1>> h;
                    auto tpl = make_tuple(param0, param1);
                    size_t hs = h(tpl);
                    std::lock_guard<recursive_mutex> g(_mutex);
                    (*m)[hs] = new FuncValueTimeWrapper<TRet>(ret, false);
                }
            }
            
            template<typename TParam0, typename TParam1, typename TRet>
            bool TrySPGetFValue(string const& funcName, TParam0 const& param0, TParam1 const& param1, TRet& ret)
            {
                
                
                // lock to be added later.
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                auto param = make_tuple(param0, param1);
                if (itFind != _FuncCache.end())
                {
                    std::hash<tuple<TParam0, TParam1>> h;
                    size_t hs = h(param);
                    m = static_cast<FUNCMAP*>(itFind->second);
                    auto itNext = m->find(hs);
                    if (itNext != m->end())
                    {
                        FuncValueTimeWrapperBase* pBase = (FuncValueTimeWrapperBase*)itNext->second;
                        pBase->ExtendLease();
                        ret = ((SPFuncValueTimeWrapper<TRet>*)itNext->second)->ReturnValue;
                        return true;
                    }
                }
                return false;
            }
           
            template<typename TParam0, typename TParam1, typename TRet>
            void SPSetFValue(string const& funcName, TParam0 const& param0, TParam1 const& param1, TRet& ret)
            {
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                if (itFind == _FuncCache.end())
                {
                    std::lock_guard<recursive_mutex> g(_mutex);
                    m = new FUNCMAP();
                    _FuncCache[funcName] = (LPVOID) m;
                }
                else
                    m = static_cast<FUNCMAP*>(itFind->second);
                if (m)
                {
                    std::hash<tuple<TParam0, TParam1>> h;
                    auto tpl = make_tuple(param0, param1);
                    size_t hs = h(tpl);
                    std::lock_guard<recursive_mutex> g(_mutex);
                    (*m)[hs] = new SPFuncValueTimeWrapper<TRet>(ret, true);
                }
            }            
            
            template<typename TParam0, typename TParam1, typename TRet>
            void SPSetFValue(string const& funcName, TParam0 const& param0, TParam1 const& param1, TRet& ret, int timeout)
            {
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                if (itFind == _FuncCache.end())
                {
                    std::lock_guard<recursive_mutex> g(_mutex);
                    m = new FUNCMAP();
                    _FuncCache[funcName] = (LPVOID) m;
                }
                else
                    m = static_cast<FUNCMAP*>(itFind->second);
                if (m)
                {
                    std::hash<tuple<TParam0, TParam1>> h;
                    auto tpl = make_tuple(param0, param1);
                    size_t hs = h(tpl);
                    std::lock_guard<recursive_mutex> g(_mutex);
                    (*m)[hs] = new SPFuncValueTimeWrapper<TRet>(ret, true, timeout);
                }
            }            
            
            /************************3 Params**************************/
            
            
            template<typename TParam0, typename TParam1, typename TParam2, typename TRet>
            bool TryGetFValue(string const& funcName, TParam0 const& param0, TParam1 const& param1, TParam2 const& param2, TRet& ret)
            {
                
                
                // lock to be added later.
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                auto param = make_tuple(param0, param1, param2);
                if (itFind != _FuncCache.end())
                {
                    std::hash<tuple<TParam0, TParam1, TParam2>> h;
                    size_t hs = h(param);
                    m = static_cast<FUNCMAP*>(itFind->second);
                    auto itNext = m->find(hs);
                    if (itNext != m->end())
                    {
                        FuncValueTimeWrapperBase* pBase = (FuncValueTimeWrapperBase*)itNext->second;
                        pBase->ExtendLease();
                        ret = ((FuncValueTimeWrapper<TRet>*)pBase)->ReturnValue;
                        return true;
                    }
                }
                return false;
            }
            

            template<typename TParam0, typename TParam1, typename TParam2, typename TRet>
            void SetFValue(string const& funcName, TParam0 const& param0, TParam1 const& param1, TParam2 const& param2, TRet& ret)
            {
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                if (itFind == _FuncCache.end())
                {
                    std::lock_guard<recursive_mutex> g(_mutex);
                    m = new FUNCMAP();
                    _FuncCache[funcName] = (LPVOID) m;
                }
                else
                    m = static_cast<FUNCMAP*>(itFind->second);
                if (m)
                {
                    std::hash<tuple<TParam0, TParam1, TParam2>> h;
                    auto tpl = make_tuple(param0, param1, param2);
                    size_t hs = h(tpl);
                    std::lock_guard<recursive_mutex> g(_mutex);
                    (*m)[hs] = new FuncValueTimeWrapper<TRet>(ret, false);
                }
            }

           template<typename TParam0, typename TParam1, typename TParam2, typename TRet>
            bool TrySPGetFValue(string const& funcName, TParam0 const& param0, TParam1 const& param1, TParam2 const& param2, TRet& ret)
            {
               
                // lock to be added later.
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                auto param = make_tuple(param0, param1, param2);
                if (itFind != _FuncCache.end())
                {
                    std::hash<tuple<TParam0, TParam1, TParam2>> h;
                    size_t hs = h(param);
                    m = static_cast<FUNCMAP*>(itFind->second);
                    auto itNext = m->find(hs);
                    if (itNext != m->end())
                    {
                        FuncValueTimeWrapperBase* pBase = (FuncValueTimeWrapperBase*)itNext->second;
                        pBase->ExtendLease();
                        ret = ((SPFuncValueTimeWrapper<TRet>*)itNext->second)->ReturnValue;
                        return true;
                    }
                }
                return false;
            }
            

            template<typename TParam0, typename TParam1, typename TParam2, typename TRet>
            void SPSetFValue(string const& funcName, TParam0 const& param0, TParam1 const& param1, TParam2 const& param2, TRet& ret)
            {
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                if (itFind == _FuncCache.end())
                {
                    std::lock_guard<recursive_mutex> g(_mutex);
                    m = new FUNCMAP();
                    _FuncCache[funcName] = (LPVOID) m;
                }
                else
                    m = static_cast<FUNCMAP*>(itFind->second);
                if (m)
                {
                    std::hash<tuple<TParam0, TParam1, TParam2>> h;
                    auto tpl = make_tuple(param0, param1, param2);
                    size_t hs = h(tpl);
                    std::lock_guard<recursive_mutex> g(_mutex);
                    (*m)[hs] = new SPFuncValueTimeWrapper<TRet>(ret, true);
                }
            }

            template<typename TParam0, typename TParam1, typename TParam2, typename TRet>
            void SPSetFValue(string const& funcName, TParam0 const& param0, TParam1 const& param1, TParam2 const& param2, TRet& ret, int timeout)
            {
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                if (itFind == _FuncCache.end())
                {
                    std::lock_guard<recursive_mutex> g(_mutex);
                    m = new FUNCMAP();
                    _FuncCache[funcName] = (LPVOID) m;
                }
                else
                    m = static_cast<FUNCMAP*>(itFind->second);
                if (m)
                {
                    std::hash<tuple<TParam0, TParam1, TParam2>> h;
                    auto tpl = make_tuple(param0, param1, param2);
                    size_t hs = h(tpl);
                    std::lock_guard<recursive_mutex> g(_mutex);
                    (*m)[hs] = new SPFuncValueTimeWrapper<TRet>(ret, true, timeout);
                }
            }
            
            /************************4 Params**************************/
            
            
            template<typename TParam0, typename TParam1, typename TParam2, typename TParam3, typename TRet>
            bool TryGetFValue(string const& funcName, TParam0 const& param0, TParam1 const& param1, TParam2 const& param2, TParam3 const& param3, TRet& ret)
            {
                
                // lock to be added later.
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                auto param = make_tuple(param0, param1, param2, param3);
                if (itFind != _FuncCache.end())
                {
                    std::hash<tuple<TParam0, TParam1, TParam2, TParam3>> h;
                    size_t hs = h(param);
                    m = static_cast<FUNCMAP*>(itFind->second);
                    auto itNext = m->find(hs);
                    if (itNext != m->end())
                    {
                        FuncValueTimeWrapperBase* pBase = (FuncValueTimeWrapperBase*)itNext->second;
                        pBase->ExtendLease();
                        ret = ((FuncValueTimeWrapper<TRet>*)pBase)->ReturnValue;
                        return true;
                    }
                }
                return false;
            }
            

            template<typename TParam0, typename TParam1, typename TParam2, typename TParam3, typename TRet>
            void SetFValue(string const& funcName, TParam0 const& param0, TParam1 const& param1, TParam2 const& param2, TParam3 const& param3, TRet& ret)
            {
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                if (itFind == _FuncCache.end())
                {
                    std::lock_guard<recursive_mutex> g(_mutex);
                    m = new FUNCMAP();
                    _FuncCache[funcName] = (LPVOID) m;
                }
                else
                    m = static_cast<FUNCMAP*>(itFind->second);
                if (m)
                {
                    std::hash<tuple<TParam0, TParam1, TParam2, TParam3>> h;
                    auto tpl = make_tuple(param0, param1, param2, param3);
                    size_t hs = h(tpl);
                    std::lock_guard<recursive_mutex> g(_mutex);
                    (*m)[hs] = new FuncValueTimeWrapper<TRet>(ret, false);
                }
            }
            
            
            template<typename TParam0, typename TParam1, typename TParam2, typename TParam3, typename TRet>
            bool TrySPGetFValue(string const& funcName, TParam0 const& param0, TParam1 const& param1, TParam2 const& param2, TParam3 const& param3, TRet& ret)
            {
                
                // lock to be added later.
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                auto param = make_tuple(param0, param1, param2, param3);
                if (itFind != _FuncCache.end())
                {
                    std::hash<tuple<TParam0, TParam1, TParam2, TParam3>> h;
                    size_t hs = h(param);
                    m = static_cast<FUNCMAP*>(itFind->second);
                    auto itNext = m->find(hs);
                    if (itNext != m->end())
                    {
                        FuncValueTimeWrapperBase* pBase = (FuncValueTimeWrapperBase*)itNext->second;
                        pBase->ExtendLease();
                        ret = ((SPFuncValueTimeWrapper<TRet>*)itNext->second)->ReturnValue;
                        return true;
                    }
                }
                return false;
            }
            

            template<typename TParam0, typename TParam1, typename TParam2, typename TParam3, typename TRet>
            void SPSetFValue(string const& funcName, TParam0 const& param0, TParam1 const& param1, TParam2 const& param2, TParam3 const& param3, TRet& ret)
            {
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                if (itFind == _FuncCache.end())
                {
                    std::lock_guard<recursive_mutex> g(_mutex);
                    m = new FUNCMAP();
                    _FuncCache[funcName] = (LPVOID) m;
                }
                else
                    m = static_cast<FUNCMAP*>(itFind->second);
                if (m)
                {
                    std::hash<tuple<TParam0, TParam1, TParam2, TParam3>> h;
                    auto tpl = make_tuple(param0, param1, param2, param3);
                    size_t hs = h(tpl);
                    std::lock_guard<recursive_mutex> g(_mutex);
                    (*m)[hs] = new SPFuncValueTimeWrapper<TRet>(ret, true);
                }
            }            
            
            /************************5 Params**************************/
            
            template<typename TParam0, typename TParam1, typename TParam2, typename TParam3, typename TParam4, typename TRet>
            bool TryGetFValue(string const& funcName, TParam0 const& param0, TParam1 const& param1, TParam2 const& param2, TParam3 const& param3, TParam4 const& param4, TRet& ret)
            {
                
                // lock to be added later.
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                auto param = make_tuple(param0, param1, param2, param3, param4);
                if (itFind != _FuncCache.end())
                {
                    std::hash<tuple<TParam0, TParam1, TParam2, TParam3, TParam4>> h;
                    size_t hs = h(param);
                    m = static_cast<FUNCMAP*>(itFind->second);
                    auto itNext = m->find(hs);
                    if (itNext != m->end())
                    {
                        FuncValueTimeWrapperBase* pBase = (FuncValueTimeWrapperBase*)itNext->second;
                        pBase->ExtendLease();
                        ret = ((FuncValueTimeWrapper<TRet>*)pBase)->ReturnValue;
                        return true;
                    }
                }
                return false;
            }
            

            template<typename TParam0, typename TParam1, typename TParam2, typename TParam3, typename TParam4, typename TRet>
            void SetFValue(string const& funcName, TParam0 const& param0, TParam1 const& param1, TParam2 const& param2, TParam3 const& param3, TParam4 const& param4, TRet& ret)
            {
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                if (itFind == _FuncCache.end())
                {
                    std::lock_guard<recursive_mutex> g(_mutex);
                    m = new FUNCMAP();
                    _FuncCache[funcName] = (LPVOID) m;
                }
                else
                    m = static_cast<FUNCMAP*>(itFind->second);
                if (m)
                {
                    std::hash<tuple<TParam0, TParam1, TParam2, TParam3, TParam4>> h;
                    auto tpl = make_tuple(param0, param1, param2, param3, param4);
                    size_t hs = h(tpl);
                    std::lock_guard<recursive_mutex> g(_mutex);
                    (*m)[hs] = new FuncValueTimeWrapper<TRet>(ret, false);
                }
            }
            
            
            template<typename TParam0, typename TParam1, typename TParam2, typename TParam3, typename TParam4, typename TRet>
            bool TrySPGetFValue(string const& funcName, TParam0 const& param0, TParam1 const& param1, TParam2 const& param2, TParam3 const& param3, TParam4 const& param4, TRet& ret)
            {
                
                // lock to be added later.
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                auto param = make_tuple(param0, param1, param2, param3, param4);
                if (itFind != _FuncCache.end())
                {
                    std::hash<tuple<TParam0, TParam1, TParam2, TParam3, TParam4>> h;
                    size_t hs = h(param);
                    m = static_cast<FUNCMAP*>(itFind->second);
                    auto itNext = m->find(hs);
                    if (itNext != m->end())
                    {
                        FuncValueTimeWrapperBase* pBase = (FuncValueTimeWrapperBase*)itNext->second;
                        pBase->ExtendLease();
                        ret = ((SPFuncValueTimeWrapper<TRet>*)itNext->second)->ReturnValue;
                        return true;
                    }
                }
                return false;
            }
            

            template<typename TParam0, typename TParam1, typename TParam2, typename TParam3, typename TParam4, typename TRet>
            void SPSetFValue(string const& funcName, TParam0 const& param0, TParam1 const& param1, TParam2 const& param2, TParam3 const& param3, TParam4 const& param4, TRet& ret)
            {
                auto itFind = _FuncCache.find(funcName);
                FUNCMAP *m = 0;
                if (itFind == _FuncCache.end())
                {
                    std::lock_guard<recursive_mutex> g(_mutex);
                    m = new FUNCMAP();
                    _FuncCache[funcName] = (LPVOID) m;
                }
                else
                    m = static_cast<FUNCMAP*>(itFind->second);
                if (m)
                {
                    std::hash<tuple<TParam0, TParam1, TParam2, TParam3, TParam4>> h;
                    auto tpl = make_tuple(param0, param1, param2, param3, param4);
                    size_t hs = h(tpl);
                    std::lock_guard<recursive_mutex> g(_mutex);
                    (*m)[hs] = new SPFuncValueTimeWrapper<TRet>(ret, true);
                }
            }            
            
        private:
            static bool _bIsInit;
            static std::recursive_mutex _mutex;
            static CFunctionCache* _pCache;
            FUNCTIONCACHE _FuncCache;
            CTimer _timer;
        
        public:
            static int DefaultTimeOut;       // default cache timeout value in milliseconds
        };

    }
}

#define FC (se::cache::CFunctionCache::Instance())

#endif	/* CFUNCTIONCACHE_H */

