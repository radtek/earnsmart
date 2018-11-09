/* 
 * File:   CROList.cpp
 * Author: aashok
 * 
 * Created on July 9, 2013, 8:54 PM
 */

#include "IObject.h"
#include "CObjectRoot.h"
#include "Object.h"
#include "CROList.h"
#include "CList.h"

namespace se
{
//
//template<typename T>
//ErrorId ListCreator::CreateList(IROSPList<T>** ppOut)
//{
//    return CObject<CROSPList<T>>::Create(ppOut);
//}
//
//template<typename T>
//ErrorId ListCreator::CreateList(ISPList<T>** ppOut)
//{
//    return CObject<CSPList<T>>::Create(ppOut);
//}
    
    CIdNameList::CIdNameList(ISession* ps):CROSPList<IIdName, IIdNameList> (ps)
    {

    }

    CIdNameList::~CIdNameList() 
    {

    }

}