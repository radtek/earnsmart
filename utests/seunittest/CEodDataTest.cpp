/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CEodDataTest.cpp
 * Author: santony
 * 
 * Created on December 31, 2015, 2:08 PM
 */

#include "CEodDataTest.h"

CEodDataTest::CEodDataTest() { }

CEodDataTest::CEodDataTest(const CEodDataTest& orig) { }

CEodDataTest::~CEodDataTest() { }

void CEodDataTest::testEodDataExchanges()
{
    RP<ISessionInterface> psession = NonInteractiveUserLogin("admin", "963051");
    RP<IEODData> peod = psession->get_EodDataApi();
    peod = psession->get_EodDataApi();
    SP<IWSClientSession> pws;
    if (ISOK(peod->LoginToWSClient("vze2rdgy", "Run37Dir", &pws)))
    {
        SP<se::eoddata::IExchangeList> pexchs;
        if (ISOK(pws->GetExchanges(&pexchs)))
        {
            SP<IEnumerator1 < se::eoddata::IExchange>> penum;
            pexchs->get_Enumerator(&penum);
            while (penum->MoveNext())
            {
                printf("%s\n", penum->get_Current()->get_Code());
            }
        }
    }

    CPPUNIT_ASSERT_MESSAGE("eod ws session is not initialized", pws != nullptr);
    
}
