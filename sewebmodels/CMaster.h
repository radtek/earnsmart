/* 
 * File:   CMaster.h
 * Author: santony
 *
 * Created on July 3, 2015, 9:00 PM
 */

#ifndef CMASTER_H
#define	CMASTER_H

#include "CModelBase.h"
#include "CHeader.h"
#include "CFooter.h"
#include "CToolbar.h"


class CMaster : public CModelBase
{
public:
    static CHeader Header;
    static CFooter Footer;
    static CToolbar Toolbar;
  
    SP<IUserSession> _pUserSession;
    
public:
    CMaster(const string& title = "Earnsmart");
    CMaster(const CMaster& orig) = delete;
    virtual ~CMaster();


    virtual void serialize(archive& a);

    
    const se::admin::IUserSession* get_UserSession();
    void set_UserSession(IUserSession* p);


};

#endif	/* CMASTER_H */

