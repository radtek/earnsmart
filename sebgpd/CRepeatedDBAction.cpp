/* 
 * File:   CRepeatedDBAction.cpp
 * Author: santony
 * 
 * Created on January 20, 2014, 12:08 PM
 */

#include "CRepeatedDBAction.h"

#include <libxml/xmlreader.h>
#include <libxml/xpath.h>
#include "../semysql/CSEConnections.h"
#include <string>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <cmath>
#include "../seglobal/globallogger.h"
#include <exception>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "../senet/CFtpClient.h"
#include <fstream>
#include<boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include "../seeconomy/seeconomapi.h"
#include "../sesecurities/sesecuritiesapi.h"
#include <unordered_set>
#include "../senet/CHttpClient.h"

using namespace se::data;
using namespace mysqlpp;
using namespace std;
using namespace boost;
using namespace boost::algorithm;
using namespace se::net;
using namespace se::economy;
using namespace se::securities;
//
//void PublishNonOtcCompanies(ISession* ps, string symlist, ISecurityExchange* pExch, CSmartConnection& conn)
//{
//    CSTR LOGGER = "sebgpd.sec.nonotc";
//    //LoggerPtr logger = Logger::getLogger("RepopulateListedCompanies_PublishNonOtcCompanies");
//    typedef std::unordered_set<string> FullList;
//    /*
//     * a. Read a line
//     * b. Capture Sector and Sub-sector
//     * c. Attempt to upsert in sec_sics table both sector and sub-sector. 
//     * d. Get sub-sector id.
//     * e. Get the symbol,
//     * f. Query for info from Edgar location. Get cik, address, etc.
//     * g. Now upsert a record in se_sec_products table.
//     * h. Upsert a record in se_edgar_ciks table
//     * j. the relationship between cik and product_id is saved in edgar_product_cik table.
//     * h. 
//     */
//    UID exchange = pExch->get_Id();
//    const string country = "US";
//    ifstream ifs(symlist.c_str(), ifstream::in);
//    if (!ifs.good() || !ifs.is_open())
//    {
//        BERR << "Error reading symbol list file for exchange " << pExch->get_Name();
//        return;
//    }
//    
//    try
//    {
//        //char_separator<char> sep(",");
//        string line;
//        typedef tokenizer< escaped_list_separator<char> > Tokenizer;
//        typedef vector<string> Fields;
//        FullList allsymbols;
//        // skip first line
//        getline(ifs, line);
//        while (getline(ifs, line))
//        {
//            Tokenizer tokz(line);
//            Fields flds;
//            flds.assign(tokz.begin(), tokz.end());
//            // Symbol(0),Name(1),LastSale(2),MarketCap(3),ADR TSO(4),IPOyear(5),Sector(6),industry(7),Summary Quote(8)
//            string& symbol = flds[0];
//            string& sector = flds[6];
//            string& industry = flds[7];
//            string& mktcap = flds[3];
//            string& name = flds[1];
//            string& ipoyear = flds[5];
//            string& adotso = flds[4];
//
//            boost::trim(symbol);
//            boost::trim(sector);
//            boost::trim(industry);
//            boost::trim(mktcap);
//            boost::trim(name);
//            boost::trim(ipoyear);
//            boost::trim(adotso);
//            boost::trim(name);
//
//            if (!mktcap.empty() && mktcap != "-" && mktcap != "n/a")
//            {
//                string mkttmp = "";
//                // fix for the change in marketcap
//                if (mktcap.substr(0, 1) == "$")
//                {
//                    mkttmp = mktcap.substr(1);
//                }
//                else
//                {
//                    // may need a conversion
//                }
//                
//                string scale = mkttmp.substr(mkttmp.size()-1, 1);
//                if (scale == "B" or scale == "M" or scale == "T")
//                {
//                    mkttmp = mkttmp.substr(0, mkttmp.size()-1);
//                    double dmkttmp = strtod(mkttmp.c_str(), nullptr);
//                    if (scale == "B" )
//                        dmkttmp = dmkttmp * 1000000000;
//                    else if (scale == "M")
//                        dmkttmp = dmkttmp * 1000000;
//                    else if (scale == "T")
//                        dmkttmp = dmkttmp * 1000;
//                    ostringstream ostr;
//                    ostr << dmkttmp;
//                    mkttmp = ostr.str();
//                }
//                mktcap = mkttmp;
//            }
//            
//            allsymbols.insert((string)symbol);
//
//            int gicid = 0;
//
//            if (industry != "n/a")
//            {
//                // insert/update GICS
//                Query q = conn->query();
//                q << "select gics_id, gics_name, gics_parent_id from se_sec_gics where gics_name = " << quote << sector;
//                StoreQueryResult sqr = q.store();
//                if (sqr.size())
//                {
//                    int parentgicid = sqr[0][0];
//                    q = conn->query();
//                    q << "select gics_id, gics_name, gics_parent_id from se_sec_gics where gics_name = " << quote << industry;
//                    sqr = q.store();
//                    if (sqr.size())
//                    {
//                        gicid = sqr[0][0];
//                    }
//                    else
//                    {
//                        q = conn->query();
//                        q << "insert se_sec_gics (gics_name, gics_parent_id) values ( " << quote << industry << ", " << parentgicid << " )";
//                       // LOGI(logger, q.str() );
//                        q.exec();
//                        gicid = q.insert_id();
//                    }
//                }
//                else
//                {
//                    q = conn->query();
//                    q << "insert se_sec_gics (gics_name, gics_parent_id) values ( " << quote << sector << ", 0 )";
//                    q.exec();
//                    int newId = q.insert_id();
//                    q = conn->query();
//                    q << "insert se_sec_gics (gics_name, gics_parent_id) values ( " << quote << industry << ", " << newId << " )";
//                   // LOGI(logger, q.str() );
//                    q.exec();
//                    gicid = q.insert_id();
//                }
//            }
//            else
//                gicid = 190;
//
//           // LOGI(logger, "Symbol: " << symbol );
//
//            UID prodId;
//            Query q = conn->query();
//            q << "select product_id, product_symbol from se_sec_products where product_symbol = " << quote << symbol;
//            q << " and product_exchange_id = " << exchange ;
//            q << " and product_country_id = " <<  1;
//            StoreQueryResult sqr = q.store();
//            if (!sqr.size())
//            {
//                q = conn->query();
//                q << "insert into se_sec_products (product_symbol, product_name, product_ipo_year, ";
//                q << "product_gic_id, product_exchange_id, product_security_type_id, product_country_id, product_currency_id, ";
//                q << "product_marketcap, product_adotso )";
//                q << "values (";
//                q << quote << symbol << ", ";
//                q << quote << name << ", ";
//                q << (ipoyear=="n/a"?"NULL":ipoyear) << ", ";
//                q << gicid << ", ";
//                q << exchange << ", ";
//                q << 1 << ", ";
//                q << 1 << ", ";
//                q << 1 << ", ";
//                q << (mktcap=="n/a"?"0":mktcap) << ", ";
//                q << (adotso=="n/a"?"NULL":adotso) ;
//                q << ")";
//               // LOGI(logger, q.str() );
//
//                q.exec();
//                prodId = q.insert_id();
//            }
//            else
//            {
//                prodId = sqr[0][0];
//                q = conn->query();
//                q << "update se_sec_products set product_marketcap = " << (mktcap=="n/a"?"0":mktcap) << ", ";
//                q << "product_adotso = " << (adotso=="n/a"?"NULL":adotso);
//                q << " where product_id = " << prodId;
//               // LOGI(logger, q.str() );
//
//                q.exec();
//            }
//
//            SP<IXbrl> pxbrlApi = (IXbrl*)ps->get_Object(IXbrlName);
//
//            // make an edgar CIK check.
//            SP<IString> cik;
//            if (SUCCEEDED(pxbrlApi->GetCIK(symbol.c_str(), &cik)))
//            {
//                q = conn->query();
//                q << "select cik, companyname from se_edgar_ciks where cik = " << cik->get_Buffer();
//                sqr = q.store();
//                if (sqr.size())
//                {
//                    string coName = (string)sqr[0][1];
//                    if (coName.empty())
//                    {
//                        q = conn->query();
//                        q << "update se_edgar_ciks set companyname = " << quote << name << " where cik = " << cik->get_Buffer();
//                       // LOGI(logger, q.str() );
//                        q.exec();
//                    }
//                    q = conn->query();
//                    q << "select * from se_edgar_productid_cik where product_id = " << prodId;
//                    if (q.store().size())
//                    {
//                        q = conn->query();
//                        q << "update ignore se_edgar_productid_cik set cik = " << cik->get_Buffer() << " where product_id = " << prodId;
//                       // LOGI(logger, q.str() );
//                        q.exec();
//                    }
//                    else
//                    {
//                        q = conn->query();
//                        q << "INSERT INTO se_edgar_productid_cik (product_id, cik) VALUES (";
//                        q << prodId ;
//                        q << ", ";
//                        q <<  cik->get_Buffer();
//                        q << ")";
//                       // LOGI(logger, q.str() );
//                        q.exec();
//                    }
//                }
//                else
//                {
//                    q = conn->query();
//                    q << "insert into se_edgar_ciks (cik, companyname) values (" << cik->get_Buffer() << ", " << quote << name << ")";
//                   // LOGI(logger, q.str() );
//                    q.exec();
//
//                    q = conn->query();
//                    q << "INSERT INTO se_edgar_productid_cik (product_id, cik) VALUES (";
//                    q << prodId ;
//                    q << ", ";
//                    q <<  cik->get_Buffer();
//                    q << ")";
//                   // LOGI(logger, q.str() );
//                    q.exec();
//                }            
//            }
//            else
//            {
//                q = conn->query();
//                q << "delete from se_edgar_productid_cik where product_id = " << prodId;
//               // LOGI(logger, q.str() );
//                q.exec();
//            }
//        }
//        ifs.close();
//
//        Query q = conn->query();
//        q << "update se_sec_products set product_yahoosymbol = product_symbol";
//        q.exec();
//        q = conn->query();
//        q << "update se_sec_products set product_yahoosymbol = replace(product_yahoosymbol, '^', '-P')";
//        q.exec();
//    }
//    catch(const std::exception& ex)
//    {
//        BERR << "Reported an exception: " << ex.what();
//    }
//    catch (...)
//    {
//        BERR << "Reported an unknown exception";
//    }
//    
//}
//
//void PublishOtcCompanies(ISession* ps, string symlist, ISecurityExchange* pExch, CSmartConnection& conn)
//{
//    //LoggerPtr logger = Logger::getLogger("RepopulateListedCompanies_PublishOtcCompanies");
//    typedef std::unordered_set<string> FullList;
//    /*
//     * a. Read a line
//     * b. Capture Sector and Sub-sector
//     * c. Attempt to upsert in sec_sics table both sector and sub-sector. 
//     * d. Get sub-sector id.
//     * e. Get the symbol,
//     * f. Query for info from Edgar location. Get cik, address, etc.
//     * g. Now upsert a record in se_sec_products table.
//     * h. Upsert a record in se_edgar_ciks table
//     * j. the relationship between cik and product_id is saved in edgar_product_cik table.
//     * h. 
//     */
//    UID exchange = pExch->get_Id();
//    const string country = "US";
//    ifstream ifs(symlist.c_str(), ifstream::in);
//    if (!ifs.good() || !ifs.is_open())
//    {
//       // LOGE(logger, "Error reading symbol list file for exchange " << pExch->get_Name());
//        return;
//    }
//    
//    //char_separator<char> sep(",");
//    string line;
//    char_separator<char> sep("|");
//    typedef tokenizer< char_separator<char> > Tokenizer;
//    typedef vector<string> Fields;
//    FullList allsymbols;
//    // skip first line
//    getline(ifs, line);
//    while (getline(ifs, line))
//    {
//        Tokenizer tokz(line, sep);
//        Fields flds;
//        flds.assign(tokz.begin(), tokz.end());
//        
//        // Symbol(0),Name(1),LastSale(2),MarketCap(3),ADR TSO(4),IPOyear(5),Sector(6),industry(7),Summary Quote(8)
//        
//        if (flds.size() < 8)
//            continue;
//        
//        string& symbol = flds[0];
//        string sector = "n/a";
//        string industry = "n/a";
//        string mktcap = "n/a";
//        string& name = flds[2];
//        string ipoyear = "n/a";
//        string adotso = "n/a";
//        
//
//        boost::trim(symbol);
//        boost::trim(sector);
//        boost::trim(industry);
//        boost::trim(mktcap);
//        boost::trim(name);
//        boost::trim(ipoyear);
//        boost::trim(adotso);
//        boost::trim(name);
//
//        allsymbols.insert((string)symbol);
//
//        int gicid = 0;
//
//        if (industry != "n/a")
//        {
//            // insert/update GICS
//            Query q = conn->query();
//            q << "select gics_id, gics_name, gics_parent_id from se_sec_gics where gics_name = " << quote << sector;
//            StoreQueryResult sqr = q.store();
//            if (sqr.size())
//            {
//                int parentgicid = sqr[0][0];
//                q = conn->query();
//                q << "select gics_id, gics_name, gics_parent_id from se_sec_gics where gics_name = " << quote << industry;
//                sqr = q.store();
//                if (sqr.size())
//                {
//                    gicid = sqr[0][0];
//                }
//                else
//                {
//                    q = conn->query();
//                    q << "insert se_sec_gics (gics_name, gics_parent_id) values ( " << quote << industry << ", " << parentgicid << " )";
//                   // LOGI(logger, q.str() );
//                    q.exec();
//                    gicid = q.insert_id();
//                }
//            }
//            else
//            {
//                q = conn->query();
//                q << "insert se_sec_gics (gics_name, gics_parent_id) values ( " << quote << sector << ", 0 )";
//                q.exec();
//                int newId = q.insert_id();
//                q = conn->query();
//                q << "insert se_sec_gics (gics_name, gics_parent_id) values ( " << quote << industry << ", " << newId << " )";
//               // LOGI(logger, q.str() );
//                q.exec();
//                gicid = q.insert_id();
//            }
//        }
//        else
//            gicid = 190;
//        
//       // LOGI(logger, "Symbol: " << symbol );
//
//        UID prodId;
//        Query q = conn->query();
//        q << "select product_id, product_symbol from se_sec_products where product_symbol = " << quote << symbol;
//        q << " and product_exchange_id = " << exchange ;
//        q << " and product_country_id = " <<  1;
//        StoreQueryResult sqr = q.store();
//        if (!sqr.size())
//        {
//            q = conn->query();
//            q << "insert into se_sec_products (product_symbol, product_name, product_ipo_year, ";
//            q << "product_gic_id, product_exchange_id, product_security_type_id, product_country_id, product_currency_id, ";
//            q << "product_marketcap, product_adotso )";
//            q << "values (";
//            q << quote << symbol << ", ";
//            q << quote << name << ", ";
//            q << (ipoyear=="n/a"?"NULL":ipoyear) << ", ";
//            q << gicid << ", ";
//            q << exchange << ", ";
//            q << 1 << ", ";
//            q << 1 << ", ";
//            q << 1 << ", ";
//            q << (mktcap=="n/a"?"0":mktcap) << ", ";
//            q << (adotso=="n/a"?"NULL":adotso) ;
//            q << ")";
//           // LOGI(logger, q.str() );
//
//            q.exec();
//            prodId = q.insert_id();
//        }
//        else
//        {
//            prodId = sqr[0][0];
//            q = conn->query();
//            q << "update se_sec_products set product_marketcap = " << (mktcap=="n/a"?"0":mktcap) << ", ";
//            q << "product_adotso = " << (adotso=="n/a"?"NULL":adotso);
//            q << " where product_id = " << prodId;
//           // LOGI(logger, q.str() );
//
//            q.exec();
//        }
//        
//        SP<IXbrl> pxbrlApi = (IXbrl*)ps->get_Object(IXbrlName);
//
//        // make an edgar CIK check.
//        SP<IString> cik;
//        if (SUCCEEDED(pxbrlApi->GetCIK(symbol.c_str(), &cik)))
//        {
//            q = conn->query();
//            q << "select cik, companyname from se_edgar_ciks where cik = " << cik->get_Buffer();
//            sqr = q.store();
//            if (sqr.size())
//            {
//                string coName = (string)sqr[0][1];
//                if (coName.empty())
//                {
//                    q = conn->query();
//                    q << "update se_edgar_ciks set companyname = " << quote << name << " where cik = " << cik->get_Buffer();
//                   // LOGI(logger, q.str() );
//                    q.exec();
//                }
//                q = conn->query();
//                q << "select * from se_edgar_productid_cik where product_id = " << prodId;
//                if (q.store().size())
//                {
//                    q = conn->query();
//                    q << "update ignore se_edgar_productid_cik set cik = " << cik->get_Buffer() << " where product_id = " << prodId;
//                   // LOGI(logger, q.str() );
//                    q.exec();
//                }
//                else
//                {
//                    q = conn->query();
//                    q << "INSERT INTO se_edgar_productid_cik (product_id, cik) VALUES (";
//                    q << prodId ;
//                    q << ", ";
//                    q <<  cik->get_Buffer();
//                    q << ")";
//                   // LOGI(logger, q.str() );
//                    q.exec();
//                }
//            }
//            else
//            {
//                q = conn->query();
//                q << "insert into se_edgar_ciks (cik, companyname) values (" << cik->get_Buffer() << ", " << quote << name << ")";
//               // LOGI(logger, q.str() );
//                q.exec();
//
//                q = conn->query();
//                q << "INSERT INTO se_edgar_productid_cik (product_id, cik) VALUES (";
//                q << prodId ;
//                q << ", ";
//                q <<  cik->get_Buffer();
//                q << ")";
//               // LOGI(logger, q.str() );
//                q.exec();
//            }            
//        }
//        else
//        {
//            q = conn->query();
//            q << "delete from se_edgar_productid_cik where product_id = " << prodId;
//           // LOGI(logger, q.str() );
//            q.exec();
//        }
//    }
//    ifs.close();
//
//    Query q = conn->query();
//    q << "update se_sec_products set product_yahoosymbol = product_symbol";
//    q.exec();
//    q = conn->query();
//    q << "update se_sec_products set product_yahoosymbol = replace(product_yahoosymbol, '^', '-P')";
//    q.exec();
//    
//}
//
//void RepublishListedCompanies(ISession* ps)
//{
//    //LoggerPtr logger = Logger::getLogger("RepopulateListedCompanies");
//    SP<ISecurityAPI> pSecApi;
//    SP<IXbrl> pxbrlApi;
//    SecOpen(&pSecApi);
//    XbrlOpen(&pxbrlApi);
//    CSmartConnection conn(CSEConnections::GetMain());
//    
//    // enumerate exchanges
//    SP<ISecurityExchangeList> pExchanges;
//    if (FAILED(pSecApi->get_Exchanges(&pExchanges)))
//    {
//       // LOGE(logger, "Error obtaining exchange list from security api.");
//        return;
//    }
//    
//    for (ISecurityExchange* pExch : *pExchanges)
//    {
//       // LOGI(logger, "Downloading security list for exchange " << pExch->get_Name());
//        
//        CSTR seclistlink = pExch->get_Property("symlistlink");
//        // download and save the file.
//        stringstream dest;
//        dest << pExch->get_Name() << CDate::Now() << ".txt";
//        CHttpClient http;
//        if (FAILED(http.GetFile(seclistlink, dest.str().c_str())))
//        {
//           // LOGE(logger, "Error downloading security list from " << seclistlink << " for exchange " << pExch->get_Name());
//            continue;
//        }
//        // file is downloaded, however data format of otc is different from other exchanges.
//        if (pExch->get_Id() == 4) // otc
//        {
//            //PublishOtcCompanies(dest.str(), pExch, conn);
//        }
//        else
//        {
//            //PublishNonOtcCompanies(dest.str(), pExch, conn);
//        }
//        
//        
//       // LOGI(logger, "Successfully published security list for exchange " << pExch->get_Name());
//    }
//
//   // LOGI(logger, "Finally run two stored procedures.");
//   // LOGI(logger, "Running upd_SetGicCodeUsingSicCode.");
//    Query q = conn->query();
//    q << "call upd_SetGicCodeUsingSicCode()";
//    q.execute();
//    while (q.more_results())
//        q.store_next();
//    
//   // LOGI(logger, "Running upd_DisableInvalidProducts.");
//    q = conn->query();
//    q << "call upd_DisableInvalidProducts()";
//    q.execute();
//    while (q.more_results())
//        q.store_next();    
//    
//   // LOGI(logger, "Completed re-publishing all the securities.");
//    
//}

void CopyInstanceSegmentData(ISession* ps)
{
    //LoggerPtr logger = Logger::getLogger("CopyInstanceSegmentData");
    CSmartConnection conn(CSEConnections::GetMain());
    try
    {
        Query qInst = conn->query();
        //qInst << "select instance_id, contextref from se_fin_instance where seg_value is null limit " << l0 << ", " << l1-1;
        qInst << "select instance_id, contextref from se_fin_instance where seg_value is null order by instance_id desc limit 10000";
        StoreQueryResult sqr = qInst.store();
        if (!sqr.size()) return;
        UID counter  = 0;
        for (Row& row : sqr)
        {
            const string contextref = (string)row[1];
            // load to an xml buffer
            //xmlDocPtr doc = xmlParseMemory(contextref.c_str(), contextref.size());
            xmlDocPtr doc = xmlReadMemory(contextref.c_str(), contextref.size(), "noname.xml", NULL, XML_PARSE_NOWARNING | XML_PARSE_NOERROR|XML_PARSE_NONET);
            if (!doc)
                continue;

            xmlNodePtr ctxtNode = doc->children;
            if (!ctxtNode)
                continue;

            // loop into entity and then segment
            for (xmlNodePtr n0 = ctxtNode->children; n0; n0 = n0->next)
            {
                if (!xmlStrcmp(xmlStrstr(n0->name, (xmlChar*)"entity"), BAD_CAST "entity"))
                {
                    // loop in
                    for (xmlNodePtr n1 = n0->children; n1; n1 = n1->next)
                    {
                        if (!xmlStrcmp(xmlStrstr(n1->name, (xmlChar*)"segment"), BAD_CAST "segment"))
                        {
                            for (xmlNodePtr n2 = n1->children; n2; n2 = n2->next)
                            {
                                if (xmlStrcmp(xmlStrstr(n2->name, (xmlChar*)"explicitMember"), BAD_CAST "explicitMember"))
                                    continue;

                                /*
                                <xbrli:context id="I2014Q2_us-gaap_FairValueByFairValueHierarchyLevelAxis_us-gaap_FairValueInputsLevel3Member_us-gaap_FairValueByMeasurementFrequencyAxis_us-gaap_FairValueMeasurementsRecurringMember_us-gaap_MajorTypesOfDebtAndEquitySecuritiesAxis_us-gaap_MortgageBackedSecuritiesMember">
                                    <xbrli:entity>
                                      <xbrli:identifier scheme="http://www.sec.gov/CIK">0000940944</xbrli:identifier>
                                      <xbrli:segment>
                                        <xbrldi:explicitMember dimension="us-gaap:FairValueByFairValueHierarchyLevelAxis">us-gaap:FairValueInputsLevel3Member</xbrldi:explicitMember>
                                        <xbrldi:explicitMember dimension="us-gaap:FairValueByMeasurementFrequencyAxis">us-gaap:FairValueMeasurementsRecurringMember</xbrldi:explicitMember>
                                        <xbrldi:explicitMember dimension="us-gaap:MajorTypesOfDebtAndEquitySecuritiesAxis">us-gaap:MortgageBackedSecuritiesMember</xbrldi:explicitMember>
                                      </xbrli:segment>
                                    </xbrli:entity>
                                    <xbrli:period>
                                      <xbrli:instant>2013-11-24</xbrli:instant>
                                    </xbrli:period>
                                  </xbrli:context>                                 
                                 */


                                UID segId = 0, dimId = 0;
                                string segValue = (CSTR)xmlNodeGetContent(n2);
                                string dimension;
                                xmlAttrPtr attrs = n2->properties;
                                if (attrs)
                                {
                                    for (xmlAttrPtr attr = attrs; attr; attr = attr->next)
                                    {
                                        string attrName = (CSTR)attr->name;
                                        xmlNodePtr attrVal = attr->children;
                                        if (attrName == "dimension" && attrVal && attrVal->type == XML_TEXT_NODE)
                                        {
                                            dimension = (CSTR)attrVal->content;
                                            break;
                                        }
                                    }                                    
                                }

                                trim(segValue);
                                trim(dimension);
                                if (!segValue.empty())
                                {
                                    vector<string> sgsplit;
                                    split(sgsplit, segValue, is_any_of(":"));
                                    Query q = conn->query();
                                    q << "select xbrl_id from se_xbrl_lookup where xbrl_prefix = " 
                                            << quote << sgsplit[0] << " and xbrl_name = " << quote << sgsplit[1];
                                    StoreQueryResult qrxbrlid = q.store();
                                    if (qrxbrlid.size())
                                        segId = qrxbrlid[0][0];
                                }
                                if(!dimension.empty())
                                {
                                    vector<string> dimsplit;
                                    split(dimsplit, dimension, is_any_of(":"));
                                    Query q = conn->query();
                                    q << "select xbrl_id from se_xbrl_lookup where xbrl_prefix = " 
                                            << quote << dimsplit[0] << " and xbrl_name = " << quote << dimsplit[1];
                                    StoreQueryResult qrxbrlid = q.store();
                                    if (qrxbrlid.size())
                                        dimId = qrxbrlid[0][0];
                                }
                                if (segId)
                                {
                                    qInst = conn->query();
                                    qInst << "insert ignore into se_fin_instance_context ";
                                    qInst << "(instance_id, segment_id, dimension_id) " ;
                                    qInst << "values (";
                                    qInst << (UID)row[0] << ", " << segId << ", " << dimId ;
                                    qInst << ")";
                                    qInst.execute();
                                }
                            }
                        }
                    }
                }
            }
            
            qInst = conn->query();
            qInst << "update se_fin_instance set seg_value = " << quote << "S" ;
            qInst << " where instance_id = " << (UID)row[0];
            qInst.execute();
            
            //LOG4CXX_INFO(logger, "Updated record " << row[0]);
            xmlFreeDoc(doc);
        }
    }
    catch(const std::exception& ex)
    {
        //LOG4CXX_ERROR(logger, ex.what());
    }
}

const char* Fyear = "(<FISCAL-YEAR-END>)(.*)";


void FixFinancialYearEnd(ISession* ps)
{
    //LoggerPtr logger = Logger::getLogger("FixFinancialYearEnd");

    //LOG4CXX_INFO(logger, "Opening FixFinancialYearEnd. ");

    CSmartConnection conn(CSEConnections::GetMain());
    try
    {

        //LOG4CXX_INFO(logger, "Query table. ");
        
        Query q = conn->query();
        q << "select * from se_edgar_formrecs where finyearend is null";
        StoreQueryResult qr = q.store();
        for (const Row& row : qr)
        {
            I64 recId = row["rec_id"];
            string location = (CSTR)row["location"];
            string ftpUrl = "ftp://ftp.sec.gov/" ;
            ftpUrl += location;
            ftpUrl += "/*";
            I32 pos =0;
            string token;
            string delimiter = "/";
            while ((pos = location.find(delimiter)) != std::string::npos) 
            {
               token = location.substr(0, pos);
               location.erase(0, pos + delimiter.length());
            }

            string first,middle,last;
            first = location;
            middle = location;
            last = location;
            first.erase(10);
            middle.erase(0,10);
            last = middle;
            middle.erase(2);
            last.erase(0,2);

            struct passwd* pPwd = getpwuid(getuid()); 
            string strDest = pPwd->pw_dir;
            char tmpFileBuff[TMP_MAX];
            strDest += tmpnam(tmpFileBuff);            
            CFtpClient ftp;
            ftp.GetFiles(ftpUrl.c_str(), strDest.c_str(), "(.*)(-index.htm)");
            string filepath=strDest+"/"+first+"-"+middle+"-"+last+"-index.htm";
            ifstream fin;
            fin.open(filepath);
            string line;
            while (getline(fin, line))
            {
                regex regexPattern6(Fyear, regex::extended);  
                smatch what6;
                bool isMatchFound6 = boost::regex_match(line, what6, regexPattern6);
                if (isMatchFound6) 
                { 
                    for (unsigned int i=0; i < what6.size(); i++) 
                    { 
                        if(i==1)
                        {
                            string FYearEnd = what6.str(2).c_str();
                            trim(FYearEnd);
                            Query qUpd = conn->query();
                            qUpd << "update se_edgar_formrecs set finyearend = " << quote << FYearEnd << " where rec_id = " << recId;
                            //LOG4CXX_INFO(logger, qUpd.str());
                            qUpd.exec();
                            goto BREAKLOOP;
                        }
                    } 
                } 
            }     
            BREAKLOOP:
            fin.close();
            sleep(1);
        }
    }
    catch(const std::exception& ex)
    {
        //LOG4CXX_ERROR(logger, ex.what());
    }
}

void RunPerformancePersistance(ISession* ps)
{
    //LoggerPtr logger = Logger::getLogger("sebgpd.RunPerformancePersistance");

    RP<IXbrl> pxbrl = (IXbrl*)ps->get_Object(IXbrlName)._ptr;
    SE_CODE retCode = pxbrl->PersistPerformanceData();
    if (FAILED(retCode))
    {
       // LGE(logger, "Failed to execute performance persistance function");
        return;
    }
    
    // run stored procedure to build quick access table.
   // LGI(logger, "Creating/Recreating se_fin_perf_latest table....");
    try
    {
        CSmartConnection conn(CSEConnections::GetMain());
        conn.set_ConnectionTimeout(7200);
        Query q = conn->query();
        q << "call upd_LatestPerfRatioValues(0);";
        try
        {
            q.execute();
            while (q.more_results())
                q.store_next();
           // LGI(logger, "se_fin_perf_latest table is (re)created successfully.");
            conn.RestoreTimeout();
        }
        catch(...)
        {
            while (q.more_results())
                q.store_next();
            conn.RestoreTimeout();
            throw;
        }
    }
    catch(const std::exception& ex)    
    {
       // LGE(logger, "Procedure upd_LatestPerfRatioValues failed to execute with error : " << ex.what());
    }
}

void RunEconomicNewsStorageProvider(ISession* ps)
{
    //LoggerPtr logger = Logger::getLogger("sebgpd.RunEconomicNewsStorageProvider");
    RP<IEconomyApi> api = (IEconomyApi*)ps->get_Object(IEconomyApiName)._ptr;
    if (FAILED(api->ProcessEconomicNewsFeeds()))
    {
        return;
    }

}

void RunGicsRevenueGrowthPersistance(ISession* ps)
{
    //LoggerPtr logger = Logger::getLogger("sebgpd.RunGicsRevenueGrowthPersistance");

   // LGI(logger, "Started.");
    RP<IXbrl> pxbrl = (IXbrl*)ps->get_Object(IXbrlName)._ptr;
    
    CSmartConnection conn(CSEConnections::GetMain());
    Query q = conn->query();
    q << "SELECT * FROM sa.se_sec_gics;";
    for (const Row& row : q.store())
    {
        SE_CODE retCode;
        UID gicid = row["gics_id"];
        retCode = pxbrl->GetGicsRevenueGrowth(true, gicid, PERIOD_Annually, nullptr, nullptr);
        if (FAILED(retCode))
        {
           // LGE(logger, "Failed to execute annual performance persistance function for gic id " << gicid);
        }
        retCode = pxbrl->GetGicsRevenueGrowth(true, gicid, PERIOD_Quarterly, nullptr, nullptr);
        if (FAILED(retCode))
        {
           // LGE(logger, "Failed to execute quarterly performance persistance function for gic id " << gicid);
        }
        sleep(2);
    }
   // LGI(logger, "Done.");
}

void RunEconomicsIndicatorUpdate(ISession* ps)
{
    //LoggerPtr logger = Logger::getLogger("sebgpd.RunEconomicsIndicatorUpdate");
    RP<IEconomyApi> peapi = (IEconomyApi*)ps->get_Object(IEconomyApiName)._ptr;
    if (FAILED(peapi->SourceEconomicData())) return ;
}

void UpdateEarningsCalendar(ISession* ps)
{
    //LoggerPtr logger = Logger::getLogger("sebgpd.UpdateEarningsCalendar");
    RP<ISecurityAPI> papi = (ISecurityAPI*)ps->get_Object(ISecurityAPIName)._ptr;
    if (FAILED(papi->ProcessEarningCalendar(PERIOD_Monthly)))
    {
       // LGE(logger, "Processing earnings calendar failed.");
    }
    return;
}

void ProcessProductKeyStats(ISession* ps)
{
    //LoggerPtr logger = Logger::getLogger("sebgpd.UpdateAverageReturnsAndRisk");
    try
    {
        RP<IXbrl> px = (IXbrl*)ps->get_Object(IXbrlName)._ptr;
        px->ProcessProductKeyStats();
    }
    catch(const std::exception& ex)
    {
       // LGE(logger, "Failed to run return risk update processor.")
    }
}





/////////////////////////////////////////////////////////////////////////////////////////////

CRepeatedDBAction::CRepeatedDBAction(ISession* ps, const string& param) : 
        CTimedFunction(),
        //_logger(Logger::getLogger("sebgpd::CRepeatedDBAction")),
        _param(param)
{
    _pSession = ps;
}


CRepeatedDBAction::~CRepeatedDBAction() 
{
}

void CRepeatedDBAction::run() 
{
    BoolCompletion busy(_bIsBusy);
    if (_param == "publishseclist")
    {
        //RepublishListedCompanies(_pSession);
    }
    else if (_param == "segment")
        CopyInstanceSegmentData(_pSession);
    else if (_param == "fixfinyear")
        FixFinancialYearEnd(_pSession);
    else if(_param == "updateperformance")
        RunPerformancePersistance(_pSession);
    else if (_param == "economicnewsprocessor")
        RunEconomicNewsStorageProvider(_pSession);
    else if (_param == "sourceeconomicsdata")
        RunEconomicsIndicatorUpdate(_pSession);
    else if (_param == "gicsrevenuegrowth")
        RunGicsRevenueGrowthPersistance(_pSession);
    else if (_param == "updateearningscalendar")
        UpdateEarningsCalendar(_pSession);
    else if (_param == "processproductkeystats")
        ProcessProductKeyStats(_pSession);
}

