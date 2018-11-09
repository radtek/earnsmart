/* 
 * File:   CUserType.h
 * Author: aashok
 *
 * Created on August 31, 2013, 2:43 PM
 */

#ifndef CUSERTYPE_H
#define	CUSERTYPE_H

#include "seadminapi.h"
#include <string>

using namespace std;

namespace se
{
    namespace admin
    {
       class CUserType : public CObjectRoot, public IUserType
       {
            public: 
                CUserType(ISession*);
                CUserType(const CUserType& orig)=delete;
                virtual ~CUserType();
                
                virtual CSTR get_Name();
                virtual void set_Name(CSTR value);
                virtual void Serialize(ISerializedData<IString>* pIn) ;
                virtual bool get_IsAdmin() ;
                virtual void set_IsAdmin(bool);

            private:
                string _name;
                bool _isAdmin;
        }; 
    }
}




#endif	/* CUSERTYPE_H */

