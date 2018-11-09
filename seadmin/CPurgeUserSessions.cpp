/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CPurgeUserSessions.cpp
 * Author: santony
 * 
 * Created on November 21, 2016, 7:54 PM
 */

#include "internal.h"
#include "../semysql/CSEConnections.h"
#include "CPurgeUserSessions.h"

#define SEDBADM         CSEConnections::GetAdmin()

using namespace se::admin;
using namespace se::data;

// USE BELOW CODE LOAD FROM SE.JSON FILE; SEE Reset(300) in constructor below
//    RP<se::config::IConfigurator> pCfg = this->get_Session()->get_Configs();
//    auto cfg = pCfg->get_ConfigRoot();
//    int purgeTimeout = 0; // 5 mins
//    if (cfg.isMember("timerserver"))
//    {
//        purgeTimeout = cfg["timerserver"].get("purgesessions", 300).asInt(); // 5 mins is defualt
//    }
//    _pPurgeSessionTimeout = new CTimeOut(purgeTimeout);
//    _timer.StartTimer();


CPurgeUserSessions::CPurgeUserSessions(IApp* papp)
{
    this->_pApp = papp;
    this->Name = "PurgeUserSessions";
    Reset(300);
}

CPurgeUserSessions::CPurgeUserSessions(const CPurgeUserSessions& orig) : CTimerService(orig)
{
    SetInternals(const_cast<CPurgeUserSessions&>(orig));
}

CPurgeUserSessions::CPurgeUserSessions(CPurgeUserSessions&& orig) : CTimerService(orig)
{
    SetInternals(orig);
}

CPurgeUserSessions::~CPurgeUserSessions()
{
}

void CPurgeUserSessions::SetInternals(CPurgeUserSessions& other)
{
    se::services::CTimerService::SetInternals(other);
}

CPurgeUserSessions& CPurgeUserSessions::operator=(const CPurgeUserSessions& right)
{
    if (this == &right)
        return *this;
    SetInternals(const_cast<CPurgeUserSessions&>(right));
    return *this;
}

CPurgeUserSessions& CPurgeUserSessions::operator=(CPurgeUserSessions&& right)
{
    if (this == &right)
        return *this;
    
    return *this;
}



void CPurgeUserSessions::InternalRun()
{
    CSmartConnection conn(SEDBADM);
    Query q = conn->query("call spPurgeExpiredSessions");
    q.execute();
    while (q.more_results());
}
