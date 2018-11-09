/* 
 * File:   CWebSession.cpp
 * Author: santony
 * 
 * Created on August 5, 2014, 10:35 PM
 */

#include "internal.h"
#include "CWebSession.h"
#include <algorithm>
#include <functional>

namespace se
{
    namespace admin
    {


        CWebSession::CWebSession(ISession* ps) : CObjectRoot(ps)
        {
        }

        CWebSession::~CWebSession() 
        {
        }


        long unsigned int CWebSession::get_ProductFinds(ISecurityDescriptionList** ppOut, short limitTo) 
        {
//            if (limitTo != _pickLimit || !_pPicks)
//            {
//                _pickLimit = limitTo;
//                SP<IAdminAPI> papi;
//                RETURNIFFAILED(AdminOpen(get_Session(), &papi));
//                RETURNIFFAILED(papi->get_RecentProductPicks(this, limitTo, &_pPicks));
//                if (_pPicks)
//                {
//                    for (ISecurityDescription* p : *_pPicks)
//                        _pids.insert(p->get_Id());
//                }
//            }
//            _pPicks.CopyTo(ppOut);
            return S_OK;
        }

        long unsigned int CWebSession::set_NewProductFound(unsigned long ProductId) 
        {
//            if (_pids.find(ProductId) == _pids.end())
//            {
//                _pPicks.Clear();
//                _pids.clear();
//                // update record in the table.
//                // add new item to the list
//                // if the list exceeds 'limitTo', then pop the excess item.
//                SP<IAdminAPI> papi;
//                RETURNIFFAILED(AdminOpen(get_Session(), &papi));
//                return papi->set_RecentProductPick(this, ProductId);
//            }
            return S_OK;
        }

        const char* CWebSession::get_SessionId() 
        {
            return _sessionId.c_str();
        }

        unsigned long CWebSession::get_UserId() 
        {
            return _userId;
        }

        RP<IUser> CWebSession::get_User() 
        {
            if (!_userId)
            {
                THRWITHBOOSTERLOG(IUser::CreateAnonymous(get_Session(), &_pUser), "Failed to initialize anon user.");
            }
            else
            {
                SP<IAdminAPI> papi;
                THRWITHBOOSTERLOG(AdminOpen(get_Session(), &papi), "Failed to open admin api");
                THRWITHBOOSTERLOG(papi->get_User(_userId, &_pUser), "Failed to initialize user");
            }
            return _pUser; // ensure LHS uses smart pointer
        }


        void CWebSession::set_SessionId(const char* sid) 
        {
            _sessionId = sid;
        }

        void CWebSession::set_UserId(unsigned long uid) 
        {
            _userId = uid;
        }
        
    }
    
}