/* 
 * File:   CSessionActivity.h
 * Author: aashok
 *
 * Created on August 31, 2013, 4:06 PM
 */

#ifndef CUSERSESSION_H
#define	CUSERSESSION_H

#include <string>

using namespace std;

namespace se
{
    class CSessionActivity: public CObjectRoot, public ISessionActivity
    {
        public:
            CSessionActivity(ISession*);
            CSessionActivity(const CSessionActivity& orig)=delete;
            virtual ~CSessionActivity();

            virtual DATETIME& get_Time();
            virtual void set_Time(DATETIME& ts);


            virtual CSTR get_UserAgent();
            virtual CSTR get_RemoteAddress();
            virtual CSTR get_RemoteUser();
            virtual CSTR get_RemoteIdent();
            virtual CSTR get_RemoteHost();
            virtual CSTR get_HttpReferer();

            virtual void set_UserAgent(CSTR);
            virtual void set_RemoteAddress(CSTR);
            virtual void set_RemoteUser(CSTR);
            virtual void set_RemoteIdent(CSTR);
            virtual void set_RemoteHost(CSTR);
            virtual void set_HttpReferer(CSTR);

        private:
            DATETIME _time;
            UST _activity;
            string _useragent, _httpReferer;
            string _remoteAddr, _remoteHost, _remoteUser, _remoteIdent;

    };
}


#endif	/* CUSERSESSION_H */

