/* 
 * File:   CUserType.cpp
 * Author: aashok
 * 
 * Created on August 31, 2013, 2:43 PM
 */

#include "internal.h"
#include "CUserType.h"

namespace se
{
    namespace admin
    {
        CUserType::CUserType(ISession* ps) : CObjectRoot(ps)
        {
            
        }

        CUserType::~CUserType() 
        {
            
        }
        
        
        CSTR CUserType::get_Name()
        {
           return _name.c_str(); 
        }
        
        void CUserType::set_Name(CSTR value)
        {
            
           _name = value;
        }
        
        void CUserType::Serialize(ISerializedData<IString>* pIn)
        {
            pIn->Add("Name",_name.c_str());
        }
        
        ErrorId IUserType::Create(ISession* ps, IUserType** ppOut)
        {
            return CObject<CUserType>::Create(ps, ppOut);
        }
        
        bool CUserType::get_IsAdmin()
        {
            return _isAdmin;
        }
        
        void CUserType::set_IsAdmin(bool value)
        {
            _isAdmin = value;
        }
    }
}




