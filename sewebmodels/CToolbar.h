/* 
 * File:   CToolbar.h
 * Author: santony
 *
 * Created on July 3, 2015, 9:08 PM
 */

#ifndef CTOOLBAR_H
#define	CTOOLBAR_H

#include "CModelBase.h"


class CToolbar : public CModelBase
{
private:
    static bool IsLoaded;
    
public:
    Json::Value RootMenu;
    
public:
    
    CToolbar();
    CToolbar(const CToolbar& orig) = delete;
    virtual ~CToolbar();
    
    void InitMenus();
    
    
    virtual void serialize(archive& a);
    
private:

};

#endif	/* CTOOLBAR_H */

