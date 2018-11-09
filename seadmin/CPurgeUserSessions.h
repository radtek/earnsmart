/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CPurgeUserSessions.h
 * Author: santony
 *
 * Created on November 21, 2016, 7:54 PM
 */

#ifndef CPURGEUSERSESSIONS_H
#define CPURGEUSERSESSIONS_H

#include "../seglobal/CTimerServices.h"

namespace se
{
    namespace admin
    {
        class CPurgeUserSessions : public se::services::CTimerService
        {
        private:
            SP<IApp> _pApp;
            
        public:
            CPurgeUserSessions(IApp* papp);
            CPurgeUserSessions(const CPurgeUserSessions& orig);
            CPurgeUserSessions(CPurgeUserSessions&& orig);
            virtual ~CPurgeUserSessions();
            
            CPurgeUserSessions& operator=(const CPurgeUserSessions& right);
            CPurgeUserSessions& operator=(CPurgeUserSessions&& right);
        protected:
            void SetInternals(CPurgeUserSessions& other);
            void InternalRun() override;


            
        private:

        };
        
    }
}


#endif /* CPURGEUSERSESSIONS_H */

