/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CTimerServices.h
 * Author: santony
 *
 * Created on November 21, 2016, 6:21 PM
 */

#ifndef CTIMERSERVICES_H
#define CTIMERSERVICES_H

#include "CTimer.h"
#include "IObject.h"
#include "CObjectRoot.h"
#include <mutex>
#include <vector>
#include <Poco/ThreadPool.h>

namespace se
{
    namespace services
    {
        class CTimerService : public se::CTimeOut
        {
        public:
            std::string Name = "TimerService";
            
        private:
            bool bIsBusy = false;
            
        protected:
            void SetIsBusy();
            void UnsetIsBusy();
            void SetInternals(CTimerService& other);
            virtual void InternalRun(){};
            
        public:
            CTimerService();
            CTimerService(const CTimerService& orig);
            CTimerService(CTimerService&& orig);
            CTimerService& operator=(const CTimerService& right);
            CTimerService& operator=(CTimerService&& right);

            bool IsBusy();
            void Run();
            
        };
        
        typedef std::vector<CTimerService*> TimerSericeList;
        
        class CTimerServices : CObjectRoot
        {
        private:
            CTimer timer;
            TimerSericeList services;
            std::recursive_mutex lock;
            SP<se::threading::IThreadPool> tp;
            
        private:
            static void OnTimeOut(const CTimer* timer, LPVOID state);
            
        public:
            CTimerServices();
            CTimerServices(const CTimerServices& orig) = delete;
            virtual ~CTimerServices();
            SE_CODE Init();
            void Add(CTimerService* service);

        };
        
    }
}

#endif /* CTIMERSERVICES_H */

