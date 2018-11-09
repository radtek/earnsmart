/* 
 * File:   CTestEodData.cpp
 * Author: santony
 * 
 * Created on August 26, 2015, 9:15 AM
 */

#include <booster/log.h>

#include "CTestEodData.h"

#define BERR BOOSTER_ERROR(__FUNCTION__)
#define BINF BOOSTER_INFO(__FUNCTION__)
#define BDBG BOOSTER_DEBUG(__FUNCTION__)
#define BWRN BOOSTER_WARNING(__FUNCTION__)

#define uid  "vze2rdgy"
#define pwd  "Run37Dir"

CTestEodData::CTestEodData() {

}

CTestEodData::~CTestEodData() {

}

void CTestEodData::Run() {
    const int runall = 0x1 | 0x2 | 0x4 | 0x8;
    //int runblock = runall; // run all 
    int runblock = 0x8;

    SE_CODE rCode;


    SP<IEODData> pEd;
    rCode = EodDataOpen(&pEd);
    if (FAILED(rCode)) {
        BERR << "Error opening eoddata api";
        return;
    }
    SP<IWSClientSession> pws;
    rCode = pEd->LoginToWSClient(uid, pwd, &pws);
    if (FAILED(rCode)) {
        BERR << "Login failed.";
        return;
    }

    if (0x1 == (runblock & 0x1)) {
        SP<IExchangeList> pExchs;
        rCode = pws->GetExchanges(&pExchs);
        if (FAILED(rCode)) {
            BERR << "Failed to get exchanges";
            return;
        }

        SP<IEnumerator1 < IExchange>> pExchEnum;
        rCode = pExchs->get_Enumerator(&pExchEnum);
        while (pExchEnum->MoveNext()) {
            IExchange* pExch = pExchEnum->get_Current();
            BINF << "Exchange Info: " << pExch->get_Id() << ", " << pExch->get_Name() << ", " << pExch->get_Code() << ", " << pExch->get_Country() << endl;
        }
    }

    if ((0x2 == (runblock & 0x2))) {
        SP<ISymbolList> pSymbols;
        CSTR exch = "NYSE";
        rCode = pws->GetSymbols(exch, &pSymbols);
        if (FAILED(rCode)) {
            BERR << "Failed to get symbols for nyse";
            return;
        }
        int i = 0, max = 10;
        SP<IEnumerator1 < se::securities::ISecurityDescription>> pSymListEnum;
        rCode = pSymbols->get_Enumerator(&pSymListEnum);
        if (FAILED(rCode)) {
            BERR << "Failed to get symbol list enumerator";
            return;
        }
        while (pSymListEnum->MoveNext()) {
            se::securities::ISecurityDescription* pSecDesc =
                    pSymListEnum->get_Current();
            BINF << "Security: Code=" << pSecDesc->get_Symbol() <<
                    " Name=" << pSecDesc->get_Name() << endl;
        }
    }
    if ((0x4 == (runblock & 0x4))) {
        SP<se::securities::ISecurityDescription> pDesc;
        SP<ISecurityDetails> pDets;
        se::securities::ISecurityDescription::Create(&pDesc);
        pDesc->set_Symbol("IBM");
        pDesc->set_Exchange("NYSE");
        rCode = pws->GetSecurityDetails(pDesc, &pDets);
        if (FAILED(rCode)) {
            BERR << "Failed to get sec details";
            return;
        }
    }

    if ((0x8 == (runblock & 0x8))) {
        SP<se::securities::ISecurityDescription> pDesc;
        SP<ISecurityDetails> pDets;
        se::securities::ISecurityDescription::Create(&pDesc);
        pDesc->set_Symbol("IBM");
        pDesc->set_Exchange("NYSE");
        CDate dtStart(2015, 8, 1);
        CDate dtEnd(2015, 8, 28);
        SP<ISecurityQuoteList> pQuotes;
        rCode = pws->GetSymbolHistoryDaily(pDesc, dtStart, dtEnd, &pQuotes);
        if (FAILED(rCode)) {
            BERR << "Failed to get sec price history";
            return;
        }
        SP<IEnumerator1 < ISecurityQuote>> pEnum;
        rCode = pQuotes->get_Enumerator(&pEnum);
        if (FAILED(rCode)) {
            BERR << "Failed to get sec price history enum";
            return;
        }
        while (pEnum->MoveNext()) {
            ISecurityQuote* pQ = pEnum->get_Current();
            BINF << "Security Price for " << pQ->get_DateTime() << std::endl;
            BINF << "Ask: " << pQ->get_Close() << std::endl;
        }
    }

    // Test publishing
    rCode = pws->PublishEodData();
    if (FAILED(rCode)) {
        BERR << "Failed to publish data";
        return;
    }

    return;
}


