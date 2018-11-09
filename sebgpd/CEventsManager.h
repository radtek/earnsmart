/*
 * File:   CEventsManager.h
 * Author: santony
 *
 * Created on September 6, 2012, 7:02 PM
 */

#ifndef CEVENTSMANAGER_H
#define	CEVENTSMANAGER_H

class CEventsManager
{
public:
    CEventsManager();
    CEventsManager(const CEventsManager& orig);
    virtual ~CEventsManager();
    bool Dispatch();
    void Shutdown();

private:
    bool _bShutdown ;

};

#endif	/* CEVENTSMANAGER_H */

