/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CTimerServices.cpp
 * Author: santony
 * 
 * Created on November 21, 2016, 6:21 PM
 */

#include "CTimerServices.h"
#include "Object.h"
#include "CThreadPool.h"
#include "globallogger.h"

using namespace se::services;

CTimerServices::CTimerServices() : timer(60, 60)
{
    timer.SetState(this);
    timer.SetCallback(CTimerServices::OnTimeOut);
}

CTimerServices::~CTimerServices()
{
    timer.StopTimer();
    for(auto p : services)
    {
        delete p;
    }
    services.clear();
}

long unsigned int CTimerServices::Init()
{
    auto ret = CObject<se::threading::CCThreadPool>::Create((ISession*)nullptr, &tp);
    if (FAILED(ret))
        return ret;
    timer.StartTimer();
    return S_OK;
}


void CTimerServices::OnTimeOut(const CTimer* timer, LPVOID state)
{
    CTimerServices* p = static_cast<CTimerServices*>(state);
    std::lock_guard<std::recursive_mutex> g(p->lock);
    for(auto service : p->services)
    {
        if (!service->IsBusy() && service->HasExpired())
        {
            p->tp->ExecuteFunction([&service](LPVOID){service->Run();}, nullptr);
        }
    }
}


void CTimerServices::Add(CTimerService* service)
{
    std::lock_guard<std::recursive_mutex> g(lock);
    services.emplace_back(service);
}


/////////////// CTimerService /////////////////

CTimerService::CTimerService()
{
}

CTimerService::CTimerService(const CTimerService& orig)
{
    SetInternals(const_cast<CTimerService&>(orig));
}

CTimerService::CTimerService(CTimerService&& orig)
{
    SetInternals(orig);
}

void CTimerService::SetInternals(CTimerService& other)
{
    this->bIsBusy = other.bIsBusy;
    CTimeOut::SetInternals(other);
}


CTimerService& CTimerService::operator =(const CTimerService& right)
{
    if (this == &right) // Same object?
        return *this; // Yes, so skip assignment, and just return *this.
    SetInternals(const_cast<CTimerService&>(right));
    return *this;
}

CTimerService& CTimerService::operator =(CTimerService&& right)
{
    if (this == &right) // Same object?
        return *this; // Yes, so skip assignment, and just return *this.
    SetInternals(right);
    return *this;
}

bool CTimerService::IsBusy()
{
    return this->bIsBusy;
}

void CTimerService::SetIsBusy()
{
    bIsBusy = true;
}

void CTimerService::UnsetIsBusy()
{
    bIsBusy = false;
}

void CTimerService::Run()
{
    this->SetIsBusy();
    try
    {
        this->InternalRun();
    }
    catch(const std::exception& ex)
    {
        BERR << "Service " << Name << " failed to run with error " << ex.what();
    }
    catch(...)
    {
        BERR << "Service " << Name << " failed to run. Unknown error.";
    }
    this->UnsetIsBusy();
}

