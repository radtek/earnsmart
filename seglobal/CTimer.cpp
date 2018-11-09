/* 
 * File:   CTimer.cpp
 * Author: santony
 * 
 * Created on October 12, 2013, 10:23 PM
 */

#include <signal.h>
#include <string.h>


#include "CTimer.h"

namespace se
{

    CTimer::CTimer(int expireon, int interval) : 
    _expiresOn(expireon),
            _interval(interval),
        _callback(nullptr),
            _state(nullptr),
            _bIsTimerSet(false)
            //_logger(Logger::getLogger("se.timer"))
    {
    }

    CTimer::CTimer(const CTimer& orig) {
    }

    CTimer::~CTimer() 
    {
        StopTimer();
    }
    
    void CTimer::SetState(LPVOID state)
    {
        _state = state;
    }
    
    void CTimer::SetCallback(std::function<void(CTimer const*,LPVOID)> timercallback)
    {
        _callback = timercallback;
    }

    bool CTimer::StartTimer()
    {
        if (_bIsTimerSet) return true;
        
        // ensure that callback is available. state is not important.
        if (!_callback)
            //LOG4CXX_WARN(_logger, "Timer callback is not set. Timer will be started, but notifications will not be delivered.");
        
        if (_interval == 0)
        {
            //LOG4CXX_ERROR(_logger, "Interval is set to " << _interval << " which is not valid. It must be a positive number.");
            return false;
        }
        
        struct sigevent se;
        struct itimerspec its;
        struct sigaction sa;
        int sigNo = SIGRTMIN;
        // signal handler.
        sa.sa_flags = SA_SIGINFO;
        sa.sa_sigaction = CTimer::NativeTimerHandler;
        sigemptyset(&sa.sa_mask);
        if (sigaction(sigNo, &sa, NULL) == -1)
        {
            //LOG4CXX_ERROR(_logger, "Failed to setup signal handler");
            return false;
        }
        
        // enable timer
        se.sigev_notify = SIGEV_SIGNAL;
        se.sigev_signo = sigNo;
        se.sigev_value.sival_ptr = this;
        if (-1 == timer_create(CLOCK_REALTIME, &se, &_timerId))
        {
            //LOG4CXX_ERROR(_logger, "Failed to create timer");
            return false;
        }
        
        its.it_interval.tv_sec = _interval;
        its.it_interval.tv_nsec = 0; // _interval * 1000; //000;
        its.it_value.tv_sec = _expiresOn;
        its.it_value.tv_nsec =  0 ;//_expiresOn * 1000; //000;
        if (-1 == timer_settime(_timerId, 0, &its, NULL))
        {
            //LOG4CXX_ERROR(_logger, "Failed to setup timer time");
            return false;
        }
        _bIsTimerSet = true;
        return true;
    }
    
    void CTimer::StopTimer()
    {
        if (_bIsTimerSet)
        {
            struct itimerspec its;
            memset(&its, 0, sizeof (struct itimerspec));
            timer_settime(_timerId, 0, &its, NULL);
            timer_delete(_timerId);
            memset(&_timerId, 0, sizeof(_timerId));
            _bIsTimerSet = false;
        }
    }
    
    void CTimer::NativeTimerHandler(int sig, siginfo_t* si, void* uc)
    {
        // get object
        CTimer* pTimer = static_cast<CTimer*>(si->si_value.sival_ptr);
        pTimer->_callback(pTimer, pTimer->_state);
    }
    
   
    /////////// CTimeOut //////////////////
    
    int CTimeOut::INIFINTE = -1;

    CTimeOut::CTimeOut()
    {
    }
    
    CTimeOut::CTimeOut(int timeoutSeconds) : _timeout(timeoutSeconds)
    {
        Extend();   
    }
    
    CTimeOut::CTimeOut(const CTimeOut& other)
    {
        SetInternals(const_cast<CTimeOut&>(other));
    }
    
    CTimeOut::CTimeOut(CTimeOut&& other)
    {
        SetInternals(const_cast<CTimeOut&>(other));
    }
    
    CTimeOut& CTimeOut::operator =(const CTimeOut& other)
    {
        if (this == &other) 
            return *this; 
        SetInternals(const_cast<CTimeOut&>(other));
        return *this;
    }
    
    CTimeOut& CTimeOut::operator =(CTimeOut&& other)
    {
        if (this == &other) 
            return *this; 
        SetInternals(other);
        return *this;
    }
    
    void CTimeOut::SetInternals(CTimeOut& other)
    {
        this->_timeout.store(other._timeout);
        this->_clk.store(other._clk);
    }
    
    void CTimeOut::Reset(int newTimeOut) 
    {
        _timeout.store(newTimeOut);
        Extend();
    }

    void CTimeOut::Extend() 
    {
        _clk.store(time(0));
    }

    
    bool CTimeOut::HasExpired() 
    {
        if (_timeout == CTimeOut::INIFINTE)
        {
            return false;
        }
        double d = difftime(time(0), _clk);
        return d >= (double)_timeout;
    }

    
}