/* 
 * File:   CEventsManager.cpp
 * Author: santony
 * 
 * Created on September 6, 2012, 7:02 PM
 */

#include "CEventsManager.h"

CEventsManager::CEventsManager() : _bShutdown(false)
{
    
}

CEventsManager::CEventsManager(const CEventsManager& orig) 
{
    _bShutdown = orig._bShutdown;
}

CEventsManager::~CEventsManager() 
{
}

void CEventsManager::Shutdown()
{
    _bShutdown = true;
}

bool CEventsManager::Dispatch()
{
    if (_bShutdown)
        return false;
    
    
    
    
    return true;
}
