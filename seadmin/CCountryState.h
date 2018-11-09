/* 
 * File:   CCountryState.h
 * Author: santony
 *
 * Created on January 2, 2014, 2:39 PM
 */

#ifndef CCOUNTRYSTATE_H
#define	CCOUNTRYSTATE_H

namespace se
{
    namespace admin
    {
        class CCountryState : public CObjectRoot, public IState 
        {
        public:
            CCountryState(ISession*);
            CCountryState(const CCountryState& orig)=delete;
            virtual ~CCountryState();
            ErrorId Init(UID id, const string& name, const string& code, I16 countryId);

            virtual CSTR get_Name();
            virtual CSTR get_Code() ;
            virtual ErrorId get_Country(ICountry**) ;

            virtual void set_Name(CSTR name);
            void set_Code(const string& v);
            void set_CountryId(I16 v);
            virtual void Serialize(ISerializedData<IString>* pIn);
            
        private:
            string _name, _code;
            I16 _cid;
        };
        
    }
}

#endif	/* CCOUNTRYSTATE_H */

