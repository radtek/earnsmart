/* 
 * File:   CCountry.h
 * Author: santony
 *
 * Created on January 1, 2014, 3:44 PM
 */

#ifndef CCOUNTRY_H
#define	CCOUNTRY_H

#include "seadminapi.h"
#include "../seglobal/CROList.h"


namespace se
{
    namespace admin
    {

        class CCountry : public CObjectRoot, public ICountry
        {
        public:
            CCountry(ISession*);
            CCountry(const CCountry& orig) = delete;
            virtual ~CCountry();
            
            ErrorId Init(
                I16 id,
                const string& name,
                const string& threecharcode,
                const string& twocharcode, 
                bool isppavailable);
            
            virtual CSTR get_Name();
            virtual const char* get_ThreeCharCode();
            virtual const char* get_TwoCharCode();
            virtual bool get_IsPaypalAvailable();
            
            virtual void set_Name(CSTR name);
            void set_ThreeCharCode(const string& code);
            void set_TwoCharCode(const string& code);
            void set_IsPaypalAvailable(bool b);
            
            virtual void Serialize(ISerializedData<IString>* pIn);

            
        private:
            string _name, _twocode, _threecode;
            bool _bisppavailable;

        };
    }
}


#endif	/* CCOUNTRY_H */

