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
#include <atomic>

namespace se
{
    class CTimer 
    {
    public:
        // create a timer, provide a function to call on timer timerout.
        // expireon in seconds
        // interval in seconds
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
        //LoggerPtr _logger;
        int _expiresOn;
        int _interval;
        timer_t _timerId;
        bool _bIsTimerSet;
    };


    /* Use this class with CTimer class to verify if a 
     particular time out situation has expired or not*/
    class CTimeOut
    {
    private:
        std::atomic_int _timeout;
        std::atomic<time_t> _clk;
        
    protected:
        void SetInternals(CTimeOut& other);
        
    public:
        static int INIFINTE ;
        
    public:
        CTimeOut();
        CTimeOut(int timeoutSeconds); // create instance. first param is for setting the timeout seconds and 2nd one is for moving the time out forward.
        CTimeOut(const CTimeOut& other);
        CTimeOut(CTimeOut&& other);
        CTimeOut& operator=(const CTimeOut& other);
        CTimeOut& operator=(CTimeOut&& other);
        
        bool HasExpired();  // has the timeout setting expired.
        void Extend();  // extend the timeout setting.
        void Reset(int newTimeOut);   // reset the time out to zero
        
    };
    
}




#endif	/* CTIMER_H */

