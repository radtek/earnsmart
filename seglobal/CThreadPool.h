/* 
 * File:   CThreadPool.h
 * Author: santony
 *
 * Created on January 5, 2013, 7:23 PM
 */

#ifndef CTHREADPOOL_H
#define	CTHREADPOOL_H

#include <pthread_workqueue.h>
#include "exceptions.h"
#include "CObjectRoot.h"
#include <functional>
#include "IObject.h"
#include <zthread/PoolExecutor.h>
#include <zthread/SynchronousExecutor.h>
#include <zthread/Task.h>

using namespace ZThread;

namespace se
{
    namespace threading
    {
        
        class CThreadPool : public CObjectRoot, public IThreadPool
        {

        public:
            CThreadPool(ISession*);
            virtual ~CThreadPool();
            virtual ErrorId Init();
            virtual void RunTask(ITask* task);
            virtual void ExecuteFunction(typename std::function<void(POINTER)> function , POINTER state);
            
        protected:
            Executor* _tp;

        };        
        
        class CSThreadPool : public CThreadPool
        {

        public:
            CSThreadPool(ISession*);
            virtual ~CSThreadPool();
            

        };        
        
        class CPThreadPool : public CThreadPool
        {

        public:
            CPThreadPool(ISession*);
            virtual ~CPThreadPool();
            virtual ErrorId Init();
            

        };                

        class CCThreadPool : public CThreadPool
        {

        public:
            CCThreadPool(ISession*);
            virtual ~CCThreadPool();

        };                
        
    }
}


#endif	/* CTHREADPOOL_H */

