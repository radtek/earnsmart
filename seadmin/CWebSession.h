/* 
 * File:   CWebSession.h
 * Author: santony
 *
 * Created on August 5, 2014, 10:35 PM
 */

#ifndef CWEBSESSION_H
#define	CWEBSESSION_H

#include "seadminapi.h"
#include <unordered_set>

namespace se
{
    namespace admin
    {
        class CWebSession : public CObjectRoot, public IWebSession
        {
        public:
            CWebSession(ISession*);
            CWebSession(const CWebSession& orig)=delete;
            virtual ~CWebSession();
            

            virtual long unsigned int get_ProductFinds(ISecurityDescriptionList** ppOut, short limitTo);

            virtual long unsigned int set_NewProductFound(unsigned long ProductId);

            virtual const char* get_SessionId();

            virtual unsigned long get_UserId();
            

            virtual void set_UserId(unsigned long uid);

            virtual void set_SessionId(const char* sid);
            
            virtual RP<IUser> get_User();
            
        private:
            SP<IUser> _pUser;
            SP<ISecurityDescriptionList> _pPicks;
            string _sessionId;
            UID _userId = 0;
            I16 _pickLimit = 0;
            typedef unordered_set<UID> ProductIds;
            ProductIds _pids;
        };
    }
    
}

#endif	/* CWEBSESSION_H */

