/* 
 * File:   CUser.h
 * Author: santony
 *
 * Created on March 18, 2013, 9:57 PM
 */

#ifndef CUSER_H
#define	CUSER_H

#include "../seglobal/CROList.h"
#include "seadminapi.h"
#include <string>

using namespace std;

namespace se
{
    namespace admin
    {
        class CUser : public CObjectRoot, public IUser
        {
        public:
            CUser(ISession*);
            CUser(const CUser& orig)=delete;
            virtual ~CUser();
            
            virtual bool get_IsActive() ;
            virtual void set_Active(bool val) ;
            virtual CSTR get_UserId() ;
            virtual void set_UserId(CSTR val) ;
            virtual CSTR get_FirstName() ;
            virtual void set_FirstName(CSTR val) ;
            virtual CSTR get_LastName() ;
            virtual void set_LastName(CSTR val) ;
            virtual CSTR get_FullName() ;
            virtual ErrorId get_Address(IAddress** ppVal) ;
            virtual void Serialize(ISerializedData<IString>* pIn) ;
            virtual ErrorId Deserialize(ISerializedData<IString>* pIn) ;
            
            virtual ErrorId set_Address(IAddress* pVal);
            

            virtual UserRole get_Roles();

            virtual void add_Role(UserRole roleToAdd);

            virtual void rem_Role(UserRole roleToRemove);

            bool get_IsInRole(UserRole role) override;


            virtual CTimeStamp& get_RegisteredOn();
            virtual void set_RegisteredOn(CTimeStamp ts);
            virtual CSTR get_ActivationKey();
            virtual void set_ActivationKey(CSTR key);


            REGSTAGES get_RegistrationStage() override;
            void set_RegistrationStage(REGSTAGES) override;

            CSTR get_CurrentSessionId() override;

            void set_CurrentSessionId(CSTR);
            
            virtual long unsigned int store_CurrentRoles();

            virtual long unsigned int store_CurrentStatus();


            
            virtual SE_CODE Store();
            virtual SE_CODE Delete();
            
            
            
            
        public:
            // internal
//            virtual ErrorId Init();
            
        public: //public fields
            UID _addrId;
            
        private:
            
            bool _bactive;
            string _userId, _firstname, _lastname, _currentSessionId;
            CTimeStamp _registeredOn;
            SP<IAddress> _inputAddress;
            I32 _roles = (I32)UserRole::Anonymous;
            REGSTAGES _currentRegStage = REGSTAGES::UserTypeAndAddress;
        };
    }
}


#endif	/* CUSER_H */

