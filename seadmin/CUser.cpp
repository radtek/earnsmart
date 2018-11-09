/* 
 * File:   CUser.cpp
 * Author: santony
 * 
 * Created on March 18, 2013, 9:57 PM
 */

#include "internal.h"
#include "CUser.h"
#include "../semysql/CSEConnections.h"

#define SEDBCONN        CSEConnections::GetMain()

using namespace se::data;

namespace se
{
    namespace admin
    {
        CUser::CUser(ISession* ps) : CObjectRoot(ps)
        {

        }


        CUser::~CUser() 
        {
        
        }
        
        ErrorId IUser::Create(ISession* ps, IUser** ppOut)
        {
            return CObject<CUser>::Create(ps, ppOut);
        }

        bool CUser::get_IsActive() 
        {
            return _bactive;
        }

        void CUser::set_Active(bool val) 
        {
            _bactive =val;
        }

        CSTR CUser::get_UserId() 
        {
            return _userId.c_str();
        }

        void CUser::set_UserId(CSTR val) 
        {
            _userId = val;
        }

        CSTR CUser::get_FirstName() 
        {
            return _firstname.c_str();
        }

        void CUser::set_FirstName(CSTR val) 
        {
            _firstname = val;
        }

        CSTR CUser::get_LastName()
        {
            return _lastname.c_str();
        }

        void CUser::set_LastName(CSTR val) 
        {
            _lastname = val;
        }

        CSTR CUser::get_FullName() 
        {
            return (_firstname + " " + _lastname).c_str();
        }

        ErrorId CUser::get_Address(IAddress** ppVal) 
        {
            if (_inputAddress)
            {
                _inputAddress.CopyTo(ppVal);
                return S_Success;
            }

            if (_addrId)
            {
                SP<IAdminAPI> api;
                RETURNIFFAILED(AdminOpen(get_Session(), &api));
                return api->get_Address(_addrId, ppVal);
            }
            
            return S_Optional;
        }

        void CUser::Serialize(ISerializedData<IString>* pIn) 
        {
            pIn->set_TypeName("se::admin::IUser");
            pIn->Add("Active", _bactive);
            pIn->Add("UserId", _userId.c_str());
            pIn->Add("FirstName", _firstname.c_str());
            pIn->Add("LastName", _lastname.c_str());
            SP<IAddress> pAddr;
            if (SUCCEEDED(get_Address(&pAddr)))
                pIn->Add("Address", pAddr);
        }

        ErrorId CUser::Deserialize(ISerializedData<IString>* pIn)
        {
            _bactive = pIn->getBool("Active");
            _userId = pIn->getString("UserId");
            _firstname = pIn->getString("FirstName");
            _lastname = pIn->getString("LastName");
            //pIn->getObject("Address", _pAddr._ptr);
            return S_Success;
        }

        ErrorId CUser::set_Address(IAddress* pVal)
        {
            _inputAddress = pVal;
        }

        UserRole CUser::get_Roles() 
        {
            return (UserRole)_roles;
        }

        void CUser::add_Role(UserRole roleToAdd) 
        {
            //_roles |= (I32)roleToAdd;     // commented because we are not following adding role approach any more.
            _roles = (I32)roleToAdd;
        }

        void CUser::rem_Role(UserRole roleToRemove) 
        {
            //_roles &~ (I32)roleToRemove; // THERE IS NO SUCH THING AS REMVOE ROLE ANY MORE.
        }

        bool CUser::get_IsInRole(UserRole role) 
        {
            bool b = (I32)role == ((I32)_roles & (I32)role);
            return b;
        }

        
        CTimeStamp& CUser::get_RegisteredOn()
        {
            return _registeredOn;
        }
        
        void CUser::set_RegisteredOn(CTimeStamp ts)
        {
            _registeredOn = ts;
        }
        
        CSTR CUser::get_ActivationKey()
        {
            return _currentSessionId.c_str();
        }
        
        void CUser::set_ActivationKey(CSTR key)
        {
            _currentSessionId = key;
        }

        
        REGSTAGES CUser::get_RegistrationStage() {
            return _currentRegStage;
        }

        void CUser::set_RegistrationStage(REGSTAGES val) {
            _currentRegStage = val;
        }
        

        long unsigned int CUser::store_CurrentRoles() 
        {
            SP<IAdminAPI> papi;
            RETURNIFFAILED(AdminOpen(get_Session(), &papi));
            return papi->set_UserRoles(get_Id(), (UserRole)_roles);
        }

        long unsigned int CUser::store_CurrentStatus() 
        {
            SP<IAdminAPI> papi;
            RETURNIFFAILED(AdminOpen(get_Session(), &papi));
            return papi->set_UserStatus(get_UserId(), _bactive);
        }


        long unsigned int CUser::Store() 
        {
            SP<IAdminAPI> papi;
            RETURNIFFAILED(AdminOpen(get_Session(), &papi));
            return papi->set_User(this);
        }

        long unsigned int CUser::Delete() 
        {
            BWRN << "Not Implemented";
            return E_NotImpl;
        }

        const char* CUser::get_CurrentSessionId()
        {
            return _currentSessionId.c_str();
        }

        void CUser::set_CurrentSessionId(const char* val)
        {
            _currentSessionId = val;
        }


        
        /////////////// Static Methods ////////////////////////

        long unsigned int IUser::CreateAnonymous(ISession* ps, IUser** ppOut) 
        {
            SP<CUser> pUser;
            RETURNIFFAILED(CObject<CUser>::Create(ps, &pUser));
            //pUser->add_UserType(UserRole::Anonymous); // this is default anyway
            pUser->set_FirstName("");
            pUser->set_LastName("");
            pUser.CopyTo(ppOut);
            return S_OK;
        }

        long unsigned int IUser::CreateBasic(ISession* ps, IUser** ppOut) 
        {
            SP<CUser> pUser;
            RETURNIFFAILED(CObject<CUser>::Create(ps, &pUser));
            pUser->add_Role(UserRole::Standard); 
            pUser.CopyTo(ppOut);
            return S_OK;
        }

        long unsigned int IUser::CreateLicensed(ISession* ps, IUser** ppOut) 
        {
            SP<CUser> pUser;
            RETURNIFFAILED(CObject<CUser>::Create(ps, &pUser));
            pUser->add_Role(UserRole::Licensed); 
            pUser.CopyTo(ppOut);
            return S_OK;
        }
    }
}

