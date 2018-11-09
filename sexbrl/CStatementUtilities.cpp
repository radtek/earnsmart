/* 
 * File:   CStatementUtilities.cpp
 * Author: santony
 * 
 * Created on September 29, 2013, 11:38 AM
 */

#include "sexbrlinternal.h"
#include "CStatementUtilities.h"
#include "CXbrlStatement.h"
#include "../seglobal/CString.h"

#define SEDBCONN        CSEConnections::GetMain()

namespace se
{
    namespace xbrl
    {

        CStatementUtilities::CStatementUtilities(ISession* ps):_ps(ps)
        {
        }

        CStatementUtilities::~CStatementUtilities() 
        {
        }
        
        DATAPERIOD CStatementUtilities::DecipherDataPeriod(CDate& filingDate, XFT formType)
        {
            DATAPERIOD dp;
            if(formType == XFT_10Q)
            {
                if (filingDate.get_Month()>=1 && filingDate.get_Month()<=3)
                {
                    dp.Quarter = 4;
                    dp.Year = filingDate.get_Year() - 1;
                }
                else if (filingDate.get_Month()>=4 && filingDate.get_Month()<=6)
                {
                    dp.Quarter = 1;
                    dp.Year = filingDate.get_Year();
                }
                else if (filingDate.get_Month()>=7 && filingDate.get_Month()<=9)
                {
                    dp.Quarter = 2;
                    dp.Year = filingDate.get_Year();
                }
                else
                {
                    dp.Quarter = 3;
                    dp.Year = filingDate.get_Year();
                }
            }
            else
            {
                dp.Quarter = 0;
                dp.Year = filingDate.get_Year() - 1;
            }
            return dp;
        }
        
        ErrorId CStatementUtilities::GetRecord(CSmartConnection &  conn, CIK cik, DATAPERIOD period, XFT formType, UID *pOutRecId)
        {
            
            I32 filingYear;
            I32 startMonth,endMonth;
            mysqlpp::UseQueryResult qres;
            if(formType == XFT_10Q)
            {
                switch (period.Quarter)
                {
                case 1:
                   startMonth =4;   // filing for first quarter usually done after the quarter from april to june
                   endMonth = 6;
                   filingYear = period.Year;
                   break;
                case 2:
                   startMonth =7;   // filing for 2nd quarter usually done after the quarter from july to sept
                   endMonth = 9;
                   filingYear = period.Year;
                   break;
                case 3:
                   startMonth =10;   // filing for 3rd quarter usually done after the quarter from oct to dec
                   endMonth = 12;
                   filingYear = period.Year;
                   break;
                case 4:
                   startMonth =1;
                   endMonth = 3;
                   filingYear = period.Year + 1;    // filing for fourth quarter done next first quarter only.
                   break;
                }
                Query q = conn->query();
                q << "SELECT rec_id FROM se_edgar_formrecs where cik = " << cik 
                        << " and form_type_id = " << formType 
                        << " and YEAR(filedate) = " << filingYear 
                        << " and MONTH(filedate) BETWEEN " << startMonth << " AND " << endMonth << ";";
                qres = q.use();
            }
            else
            {
                filingYear = period.Year + 1;
                Query q = conn->query();
                q << "SELECT rec_id FROM se_edgar_formrecs where cik = " << cik 
                        << " and form_type_id =" << formType 
                        << " and YEAR(filedate) =" << filingYear << ";";
                qres = q.use();
            }

            if (qres)
            {
                auto row = qres.fetch_row();
                if (!row)
                {
                    //LOG4CXX_INFOINFO(_logger, "Cannot Found Rec_id from Given Date");
                    return E_NotFoundRec_id;
                }
                *pOutRecId =(I64)row[0];
                return S_Success;
            }
            //LOG4CXX_INFOINFO(_logger, "Cannot Found Rec_id from Given Date");
            return E_NotFoundRec_id;
        }
        
        ErrorId CStatementUtilities::GetStatementNames(CSmartConnection& conn, CIK cik, DATAPERIOD period, XFT formType, IROSPList<IXbrlStatement>** ppOut)
        {
            UID recId ;
            RETURNIFFAILED(GetRecord(conn, cik, period, formType, &recId));
            Query q = conn->query();
            q << "select distinct a.* from se_fin_statements a, se_fin_calculation b \
                where a.statement_id = b.statement_id \
                and a.rec_id = " << recId << ";";  
            auto result = q.store();
            SP<CROSPList<IXbrlStatement>> pList;
            RETURNIFFAILED(CObject<CROSPList<IXbrlStatement>>::Create(_ps,  &pList));
            for (Row r : result)
            {
                SP<CXbrlStatement> pStmt;
                RETURNIFFAILED(CObject<CXbrlStatement>::Create(_ps, &pStmt));
                pStmt->set_Id((UID)r[0]);
                pStmt->set_Name((CSTR)r["title"]);
                pList->Add((IXbrlStatement*)pStmt._ptr);
            }
            pList.CopyTo(ppOut);
            return S_Success;
        }
        
        ErrorId CStatementUtilities::GetCikFromProductId(CSmartConnection& conn, UID ProductId, IString** ppOutCik)
        {
            Query q = conn->query();
            q << "select cik from se_edgar_productid_cik where product_id = " << ProductId;
            auto result = q.use();
            if (!result)
                return E_NotFound;
            auto row = result.fetch_row();
            if (!row)
                return E_NotFound;
            string s = (string)row[0];
            if (s == "NA" || s.empty())
                return E_NotFound;
            return CObject<CString>::Create(_ps, s.c_str(), ppOutCik);
        }
    }
}
