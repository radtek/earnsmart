/* 
 * File:   CSession.h
 * Author: santony
 *
 * Created on August 31, 2013, 10:31 AM
 */

#ifndef CSESSION_H
#define	CSESSION_H

#include <string>
#include <cppcms/application.h>
#include "../seglobal/IObject.h"
#include "../seadmin/seadminapi.h"
#include <cppcms/serialization.h>
//#include <log4cxx/logger.h>

using namespace std;
using namespace cppcms;
using namespace se;
using namespace se::admin;
//using namespace log4cxx;

namespace sg
{
    namespace web
    {
        
        typedef std::tuple<string, string> SymbolAndName;
        typedef std::vector<SymbolAndName> RecentPicks;
        
        class CSession : public serializable
        {
        public:
            CSession();
            CSession(const CSession& orig);
            virtual ~CSession();
            void Init(const string& sessionid);
            void Clear();
            
            // Means, a user has been authenticated and authorized,
            // either from a previously stored session or a newly 
            // created session.
            bool get_IsAuthenticated();        
            bool Authenticate(string const& userId, string const& password);
            
            virtual void serialize(archive& a);
            
            static void BuildUserSession(http::context& , IUserSession*&);
            
            void set_UserSession(IUserSession*);
            
        public:
            string _sessionId;
            UID _userId;
            SP<IUserSession> _pUserSession;
            RecentPicks Picks;
            
        public:

        };

    }
}

#endif	/* CSESSION_H */

