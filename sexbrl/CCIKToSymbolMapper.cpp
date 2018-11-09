/* 
 * File:   CCIKToSymbolMapper.cpp
 * Author: santony
 * 
 * Created on July 8, 2012, 11:49 PM
 */

#include "sexbrlinternal.h"
#include "CCIKToSymbolMapper.h"
#include <libxml/nanohttp.h>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include <sstream>

#include <boost/mpl/transform.hpp>

#include <boost/tokenizer.hpp>
#include <CSEConnections.h>
#define HOST "njmain"
#define DB "sa"
#include "../external/mariadbpp/mysql++.h"
#include "../external/mariadbpp/ssqls.h"

#include "../seglobal/CString.h"

#define SEDBCONN        CSEConnections::GetMain()



using namespace boost;	
using namespace se::data;
using namespace mysqlpp;
////using namespace log4cxx;
using namespace se::securities;

// sec weburl. replace ~~ with your symbol before querying the site.
//const char* cszSECUrl = "http://www.sec.gov/cgi-bin/browse-edgar?action=getcompany&CIK=~~&type=10-Q&dateb=&owner=exclude&count=40";

CSTR cszSECUrl = "http://www.sec.gov/cgi-bin/browse-edgar?CIK=~~&Find=Search&owner=exclude&action=getcompany";

const char* cszCIKFindPattern="(<link rel=.*)(CIK)(=)(.*)(&.*)(type.*)";


CCIKToSymbolMapper::CCIKToSymbolMapper()// : _logger(Logger::getLogger("se.xbrl.ciksymbolmapper"))
{
}

CCIKToSymbolMapper::CCIKToSymbolMapper(const CCIKToSymbolMapper& orig) 
{
    
}

CCIKToSymbolMapper::~CCIKToSymbolMapper() 
{
}

void CCIKToSymbolMapper::CacheCIKForSymbol(const string& symbol, CIK cik)
{
    //LOG4CXX_INFOINFO(_logger, "Saving CIK = " << cik << " for symbol " << symbol.c_str() << ".");
   
    stringstream strValue;
    strValue << cik;

    unsigned int intcik;
    strValue >> intcik;
   
    I32 flag;
    flag= 0; 
    CSmartConnection conn(SEDBCONN);
    Query q = conn->query();
    q << "select cik from  se_edgar_ciks where cik = " <<  cik ;
    auto sqr = q.store();
    if (sqr.size())
        flag= 1; 
    
    q = conn->query();
    q << "select se_sec_products.product_id, se_edgar_productid_cik.cik from se_sec_products, se_edgar_productid_cik ";
    q << "where se_sec_products.product_id = se_edgar_productid_cik.product_id ";
    q << "and se_sec_products.product_symbol = " << quote << symbol;
    
    bool bNeedInsert = false;
    bool bNeedUpdate = false;
    sqr = q.store();
    if (!sqr.size())
        bNeedInsert = true;
    mysqlpp::Row row;
    CIK tmp;
    long lSecId;
    if (!bNeedInsert)
    {
        row = sqr[0];
        lSecId = row[0];
        tmp = row[1];
        if (tmp == 0)
            bNeedUpdate = true;
    }

    if (!bNeedInsert && !bNeedUpdate) return ;
    if (bNeedInsert)
    {
        // find security id.
        q = conn->query();
        q << "select product_id from se_sec_products where product_symbol = " << quote << symbol;
        auto qRes = q.store();
        if (qRes.size())
        {
            lSecId = qRes[0][0];
            if(intcik !=0)
            {
                q = conn->query();
                q << "INSERT INTO se_edgar_productid_cik (product_id, cik) VALUES (";
                q << lSecId ;
                q << ", ";
                q << cik;
                q << ")";
                q.exec();
                if (flag==0)
                {
                    q = conn->query();
                    q << "INSERT INTO se_edgar_ciks(cik)VALUES (" << cik << ")";
                    q.exec();
                }
            }
            else
            {
                q = conn->query();
                q << "INSERT INTO se_edgar_productid_cik (product_id, cik) VALUES (" ;
                q << lSecId;
                q << ", ";
                q << 0;
                q << ")";
                q.exec();
            }
        }
    }
    else
    {
        q = conn->query();
        q << "UPDATE se_edgar_productid_cik set cik = " << cik << " where product_id = " << lSecId;
        q.exec();
    }
    
    //LOG4CXX_INFOINFO(_logger, "Saved CIK = " << cik << " for symbol " << symbol.c_str() << ".");
    return ;
}

bool CCIKToSymbolMapper::CheckGetCachedCIK(const string& symbol, string& outCik)
{
    //LOG4CXX_INFOINFO(_logger, "Checking if the CIK is alrGetSymbolFromCIKeady cached for symbol " << symbol.c_str() << ".");
    CSmartConnection conn(SEDBCONN);
    Query q = conn->query();
    q << "select cik from se_sec_products, se_edgar_productid_cik ";
    q << "where se_sec_products.product_id = se_edgar_productid_cik.product_id ";
    q << "and se_sec_products.product_symbol = " << quote << symbol ;
    
    auto qres = q.use();
    if (qres)
    {
        auto row = qres.fetch_row();
        if (row)
        {
            outCik = static_cast<string>(row[0]);
            if (outCik != "NA")
            {
                //LOG4CXX_INFOINFO(_logger, "Found the CIK = " << outCik << " for symbol " << symbol.c_str() << ".");
                return true;
            }
        }
    }
    outCik = "0";
    //LOG4CXX_INFOINFO(_logger, "Couldn't find the CIK for symbol " << symbol.c_str() << ".");
    return false;
}

ErrorId CCIKToSymbolMapper::GetCompanyDetailsFromEdgar(CSTR symbolOrCik, IPublicCompanyInfo** ppOut)
{
    
    
    return E_NotImpl;
}

SE_CODE CCIKToSymbolMapper::GetCIKFromSymbol(CSTR symbol, IString**ppOut, bool cache)
{
    xmlNanoHTTPInit();
    std::string cSymbol = symbol;
    string uiCik ;
    if (cache)
    {
        if (CheckGetCachedCIK(cSymbol, uiCik))
        {
            CacheCIKForSymbol(cSymbol,uiCik.c_str());
            if (uiCik == "0")
                return E_NotFound;
            else
            {
                return CString::Create(uiCik.c_str(), ppOut);
            }
        }
        return E_NotFound;
    }
    // open internet connection, download html content from 
    // sec site and obtain CIK.
    std::string szurl = cszSECUrl;
    size_t posOfstop = cSymbol.find(".");
    if (posOfstop != string::npos)
    {
        cSymbol.insert(posOfstop, "\\");
    }
    replace_first(szurl, "~~", cSymbol);
    // query.
    char* contentType = 0;
    void* httpCtxt = xmlNanoHTTPOpen(szurl.c_str(), &contentType);
    if (httpCtxt == 0)
    {
        return E_NotFound;
    }

    ostringstream os;
    char buffer[0x800];
    int Count ;
    while ( (Count = xmlNanoHTTPRead(httpCtxt, buffer, 0x800)))
    {
        os << buffer;
    }
    
    //xmlFree(contentType);
    xmlNanoHTTPClose(httpCtxt);
    xmlNanoHTTPCleanup();
    
    // using the output stream, parse using regex and 
    // obtain CIK
    if (!os.good())
        return 0;

    istringstream is(os.str()) ;
    
    regex regexPattern(cszCIKFindPattern, regex::extended); 
    smatch what; 
    if(!is.good())
        return E_NotFound;
    
    string str;
    uiCik = "0";
    while(!is.eof())
    {
        getline(is, str);
        bool isMatchFound = boost::regex_match(str, what, regexPattern); 
        if (isMatchFound) 
        { 
            for (unsigned int i=0; i < what.size(); i++) 
            { 
                if(i==4)
                {
                      //uiCik = atoi( what.str(4).c_str() ) ; 
                    uiCik = what.str(4).c_str(); 
                    
                }
            } 
        }
    }
   
    if (cache)
    {
        string cik = string(uiCik);
        CacheCIKForSymbol(cSymbol, cik.c_str());
        /*
        if (uiCik > 0)
        {

            SP<ISecurityAPI> pSecApi;
            SecOpen(&pSecApi);
            SP<ISecurityDescription> pSecDesc ;
            pSecApi->CreateSecurityDescription(&pSecDesc);
            pSecDesc->set_Symbol(symbol);http://www.sec.gov/cgi-bin/browse-edgar?CIK=aapl&Find=Search&owner=exclude&action=getcompany
            pSecApi->SynchronizeSecurityDescription(pSecDesc);
            SecClose(pSecApi);

            CacheCIKForSymbol(cSymbol, uiCik);

        }
        */
    }
    if (uiCik == "0")
        return E_NotFound;
    else
        return CString::Create(uiCik.c_str(), ppOut);
}

/* This method returns the symbol if cik is cached earlier. */

bool CCIKToSymbolMapper::GetSymbolFromCIK(CSTR cik, IString** symbol)
{
    //LOG4CXX_INFOINFO(_logger, "Getting symbol for the cik " << cik << ".");

    CSmartConnection conn(SEDBCONN);
    Query q = conn->query();
    q << "SELECT se_sec_products.product_symbol from se_sec_products, se_edgar_productid_cik ";
    q << "where se_sec_products.product_id = se_edgar_productid_cik.product_id ";
    q << "and se_edgar_productid_cik.cik = " << cik;
    auto qres = q.use();
    if (qres)
    {
        auto row = qres.fetch_row();
        if (row)
        {
            ErrorId err = CString::Create((CSTR)row[0], symbol);
            return true;
        }
    }
    
    //LOG4CXX_INFOINFO(_logger, "Failed to get symbol for the cik " << cik << ".");
    return false;
}

    /*
     * 1. Create a query which contains securities_id which is not in se_symbols_to_ciks table.
     * 2. loop through the query resultset 
     * 3. Call GetCIK method for each symbol
     * 4. Insert the CIK along with security_id into the se_symbols_to_ciks table.
     * 5. return success (zero).
     * 
     */
 
sql_create_2(
        se_edgar_productid_cik, 
        1, 
        2,
        sql_int, product_id,
        sql_bigint, cik
        );


void  CCIKToSymbolMapper::UpdateCIKForAllSymbols()
{
    //LOG4CXX_INFOINFO(_logger, "Entering UpdateCIKsForAllSymbols method.");
    string symbol;
    try
    {
         //LOG4CXX_INFOINFO(_logger, "UpdateCIKsForAllSymbols: opening db connection.");
         CSmartConnection conn(SEDBCONN);
         try
         {
             
            // first get a list of ciks we haven't already mapped from se_edgar_formrecs
            Query qCiks = conn->query();
            qCiks << "select distinct cik from se_edgar_formrecs where cik not in (select cik from se_edgar_ciks order by cik)";
             
             // update ciks table with details.
            conn.BeginTransaction();

            Query::RowCountInsertPolicy<> policy(25);
            typedef vector<se_edgar_productid_cik> LIST;
            LIST list;

            se_edgar_productid_cik symbols;
            symbols.instance_table("se_edgar_productid_cik");

            //LOG4CXX_INFOINFO(_logger, "UpdateCIKsForAllSymbols: Quering for the delta.");
            
            Query q = conn->query();
            q << "select product_symbol,product_id from se_sec_products where ";
            q << "product_id not in (select product_id from se_edgar_productid_cik) limit 25";

           if (mysqlpp::StoreQueryResult res = q.store())
           {
                //LOG4CXX_INFOINFO(_logger, "UpdateCIKsForAllSymbols: Query successful.");
                //LOG4CXX_INFOINFO(_logger, "UpdateCIKsForAllSymbols: Found " << res.size() << " items.");

               for (auto it = res.begin(); it != res.end(); ++it)
               {
                   auto row = *it;
                   symbol = string(row[0]);
                   SP<se::IString> cik;
                   if (S_OK == GetCIKFromSymbol(symbol.c_str(), &cik))
                   {
                        symbols.product_id = atoi(row[1]);
                        symbols.cik = !cik ? 0 : strtol(cik->get_Buffer(), nullptr, 10);
                        list.push_back(symbols); 
                   }
               } 
           }

           //LOG4CXX_INFOINFO(_logger, "UpdateCIKsForAllSymbols: Reading file completed. Sending the vector to the database.");
           //LOG4CXX_INFOINFO(_logger, "UpdateCIKsForAllSymbols: Inserting rows.");
           
           Query insertQuery = conn->query();
           insertQuery.insertfrom(list.begin(), list.end(), policy);
           conn.CommitTransaction();
           
           //LOG4CXX_INFOINFO(_logger, "UpdateCIKsForAllSymbols: Successfully updated the database.");
         }
         catch(std::exception&)
         {
             conn.RollbackTransaction();
             throw;
         }
    }
    catch (std::exception& er)
    {
        //LOG4CXX_INFOERROR(_logger, "Error: " << er.what());
    }
    //LOG4CXX_INFOINFO(_logger, "Exiting UpdateCIKsForAllSymbols method.");
    return ; 

}