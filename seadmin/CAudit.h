/* 
 * File:   CAudit.h
 * Author: aashok
 *
 * Created on August 31, 2013, 2:27 PM
 */

#ifndef CAUDIT_H
#define	CAUDIT_H

#include "seadminapi.h"
#include "../semysql/CSEConnections.h"

using namespace se::data;



namespace se
{
    namespace admin
    {
        class CAudit : public CObjectRoot, public IAudit
        {
        public:
            CAudit(ISession*);
            virtual ~CAudit();
            
            virtual void Critical(CSTR user, CSTR category, CSTR message);
            virtual void Moderate(CSTR user, CSTR category, CSTR message);
            virtual void Normal(CSTR user, CSTR category, CSTR message);
            
        private:
            CSmartConnection _conn;

        };
    }
}


#endif	/* CAUDIT_H */

