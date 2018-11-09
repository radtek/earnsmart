/* 
 * File:   CAudit.cpp
 * Author: aashok
 * 
 * Created on August 31, 2013, 2:27 PM
 */

#include "internal.h"
#include "CAudit.h"
#include "../seglobal/exceptions.h"
#define SEDBCONN        CSEConnections::GetMain()

namespace se
{
    namespace admin
    {
        CAudit::CAudit(ISession* ps) : CObjectRoot(ps), _conn(SEDBCONN)
        {
        }

        CAudit::~CAudit() 
        {
        }
        
        /*
         * Table: audits
            Columns:
            audit_id	bigint(20) PK 
            createdon	timestamp 
            action	varchar(45) 
            category	varchar(45) 
            importance	tinyint(4) 
         */

        void CAudit::Critical(CSTR user, CSTR category, CSTR message)
        {
            if (!_conn->connected())
                throw seexception("DB Connection error.");
            if (!user || !strlen(user))
                throw seexception("user parameter cannot be null or empty. Provide a session or logged-in user name.");
            if (!message || !strlen(message))
                throw seexception("message parameter cannot be null or empty.");
            
            // use the connection to add a critical message.
            auto query = _conn->query();
            query << "INSERT INTO audits (username, action, category, importance) "
                    << "values (" << quote << user << ", " << quote << message << ", " << quote << (!category || !strlen(category) ? "" : category) 
                    << ", " << (I16) AUDIMP_Critical << ")";
            query.exec();
        }
        
        void CAudit::Moderate(CSTR user, CSTR category, CSTR message)
        {
            if (!_conn->connected())
                throw seexception("DB Connection error.");
            if (!user || !strlen(user))
                throw seexception("user parameter cannot be null or empty. Provide a session or logged-in user name.");
            if (!message || !strlen(message))
                throw seexception("message parameter cannot be null or empty.");
            // use the connection to add a moderate message.
            auto query = _conn->query();
            query << "INSERT INTO audits (username, action, category, importance) "
                    << "values (" << quote << user << ", " << quote << message << ", " << quote << (!category || !strlen(category) ? "" : category) 
                    << ", " << (I16) AUDIMP_Moderate << ")";
            query.exec();
        }
        
        void CAudit::Normal(CSTR user, CSTR category, CSTR message)
        {
            if (!_conn->connected())
                throw seexception("DB Connection error.");
            if (!user || !strlen(user))
                throw seexception("user parameter cannot be null or empty. Provide a session or logged-in user name.");
            if (!message || !strlen(message))
                throw seexception("message parameter cannot be null or empty.");
            // use the connection to add a normal message.
            auto query = _conn->query();
            query << "INSERT INTO audits (username, action, category, importance) "
                    << "values (" << quote << user << ", " << quote << message << ", " << quote << (!category || !strlen(category) ? "" : category) 
                    << ", " << (I16) AUDIMP_Normal << ")";
            query.exec();
        }
    }
}
