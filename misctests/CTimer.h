/* 
 * File:   CTimer.h
 * Author: santony
 *
 * Created on October 12, 2013, 10:23 PM
 */

#ifndef CTIMER_H
#define	CTIMER_H

#include <functional>
#include <signal.h>
#include <time.h>
#include "../seglobaltypedefs.h"
#include <log4cxx/logger.h>

using namespace log4cxx;

namespace se
{
    class CTimer 
    {
    public:
        // create a timer, provide a function to call on timer timerout.
        // expireon in Mseconds
        // interval in mseconds
        CTimer(int expireon, int interval);
        CTimer(const CTimer& orig);
        virtual ~CTimer();
        
        bool StartTimer();
        void StopTimer();
        void SetState(LPVOID state);
        void SetCallback(std::function<void(CTimer const*, LPVOID)> timercallback);
        
        static void NativeTimerHandler(int sig, siginfo_t* si, void* uc);

    private:
        std::function<void(CTimer const*, LPVOID)> _callback;
        LPVOID _state;
        LoggerPtr _logger;
        int _expiresOn;
        int _interval;
        timer_t _timerId;
        bool _bIsTimerSet;
    };
    
}


#endif	/* CTIMER_H */

