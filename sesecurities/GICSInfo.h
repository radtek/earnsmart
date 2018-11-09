#ifndef CGICSINFO_H
#define CGICSINFO_H

#include <string>

using namespace std;
using namespace se;
using namespace se::securities;


class CGICSInfo : public CObjectRoot, public IGICInfo
{
    public:
        CGICSInfo(ISession*);
        virtual ~CGICSInfo();
        
        //virtual ErrorId Init() { return S_Success; }
        virtual ErrorId Init(CSTR sector, CSTR industry);
        
        virtual CSTR get_Sector() ;
        virtual CSTR get_Industry() ;
        

        virtual long unsigned int get_Companies(IPublicCompanyInfoList** ppOut);

        
        virtual void Serialize(ISerializedData<IString>* pIn);
    protected:
    private:
        
        string _sSector, _sIndustry ;
};

#endif // CGICSINFO_H
