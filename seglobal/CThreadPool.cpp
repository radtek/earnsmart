/* 
 * File:   CThreadPool.cpp
 * Author: santony
 * 
 * Created on January 5, 2013, 7:23 PM
 */

#include <string.h>
#include "../ReturnCodes.h"
#include "../seglobaltypedefs.h"
#include "exceptions.h"
#include "CThreadPool.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include "seglobalinternal.h"
#include "CDirectory.h"
#include <zthread/Runnable.h>
#include <zthread/ConcurrentExecutor.h>
#include "globallogger.h"

using namespace boost;
using namespace boost::property_tree;

namespace se
{
    namespace threading
    {
        
        CThreadPool::CThreadPool(ISession* ps):CObjectRoot(ps) 
        {
            
        }
        
        CThreadPool::~CThreadPool()
        {
            
        }
        
        // synchronized thread pool
        
        CSThreadPool::CSThreadPool(ISession* ps):CThreadPool(ps) 
        {
            _tp = new SynchronousExecutor();
        }
        
        CSThreadPool::~CSThreadPool()
        {
            delete _tp;
        }
        
        
        // thread pulled from a thread pool and concurrent execution

        CPThreadPool::CPThreadPool(ISession* ps):CThreadPool(ps) 
        {
            _tp = new PoolExecutor(1);
        }
        
        CPThreadPool::~CPThreadPool()
        {
            _tp->wait();
            delete _tp;
        }
        
        // concurrent execution but only a single thread handles the tasks

        CCThreadPool::CCThreadPool(ISession* ps):CThreadPool(ps) 
        {
            _tp = new ConcurrentExecutor();
        }

        CCThreadPool::~CCThreadPool()
        {
            _tp->wait();
            delete _tp;
        }

        ErrorId CThreadPool::Init()
        {
            return S_Success;
        }
        
        ErrorId CPThreadPool::Init()
        {
            try
            {
                RP<IApp> papp = get_Session()->get_App();
                const Json::Value& root =  papp->get_Configurator()->get_ConfigRoot();
                try
                {
                    static_cast<PoolExecutor*>(_tp)->size( root["threading"]["pool"].get("numthreads", 5).asInt() );
                    return S_Success;
                }
                catch (const std::exception& ex)
                {
                    BERR << "'threading.pool.numthreads' element not found in configuration file.";
                    return E_ConfigurationError;
                }
            }
            CATCHCLAUSE("CThreadPool::Init")
        }
        
        void CThreadPool::RunTask(ITask* task)
        {
            // wrap ITask in a Task class
            class TaskRunnable : public Runnable
            {
            private:
                ITask* _task;
            public:
                
                TaskRunnable(ITask* task)
                {
                    _task = task;
                    _task->AddRef();
                }
                
                virtual ~TaskRunnable()
                {
                    _task->DecRef();
                }
                virtual void run()
                {
                    _task->Execute();
                }
            };
            
            TaskRunnable* runner = new TaskRunnable(task);
            _tp->execute(runner);
        }

        void CThreadPool::ExecuteFunction(typename std::function<void(void*) > function, void* state) 
        {
            // wrap ITask in a Task class
            class TaskRunnable : public Runnable
            {
            private:
                std::function<void(void*) > _function;
                POINTER _state;
                
            public:
                
                TaskRunnable(std::function<void(void*) > function, POINTER state) : 
                    _function(function), _state(state)
                {
                }

                virtual void run()
                {
                    _function(_state);
                }
            };

            TaskRunnable* runner = new TaskRunnable(function, state);
            _tp->execute(runner);
        }
    }
}


