/* 
 * File:   CXbrlIdList.cpp
 * Author: santony
 * 
 * Created on May 1, 2014, 4:29 PM
 */

#include "sexbrlinternal.h"
#include "CXbrlIdList.h"

namespace se
{
    namespace xbrl
    {
        CXbrlIdList::CXbrlIdList(ISession* ps) : CList< UID, IXbrlIdList >(ps)
        {
        }

        CXbrlIdList::~CXbrlIdList() 
        {
        }

        unsigned long CXbrlIdList::GetSerializedvalue(ISerializedData<IString>* pIn, const char* propName) 
        {
            return 0;
        }

        void CXbrlIdList::SetSerializedValue(ISerializedData<IString>* pIn, const char* PropName, const unsigned long& value) 
        {

        }

#ifdef __DEBUG__
        
        void CXbrlIdList::_AddRef() 
        {
            CList< UID, IXbrlIdList >::_AddRef();
            //LoggerPtr logger = Logger::getLogger("se.xbrl.xbrlidlist.addref");
           // LOGI(logger, "refId: " << _rc);
        }

        void CXbrlIdList::_DecRef() 
        {
            CList< UID, IXbrlIdList >::_DecRef();
            //LoggerPtr logger = Logger::getLogger("se.xbrl.xbrlidlist.decref");
           // LOGI(logger, "refId: " << _rc);
        }

#endif

        ///////////////////////////// static Create //////////////////////////////////////
        
        SE_CODE XBRLEXP IXbrlIdList::Create(ISession* ps, IXbrlIdList** ppOut)
        {
            return CObject<CXbrlIdList>::Create(ps, ppOut);
        }
        
    }
}

