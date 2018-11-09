
#include "xbrldatawork.h"
#include <libxml/xmlreader.h>
#include <libxml/xpath.h>
#include "../semysql/CSEConnections.h"
#include <string>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <cmath>
//#include <log4cxx/logger.h>
//#include <log4cxx/propertyconfigurator.h>
#include <exception>
#include <fstream>
#include <sstream>
#include <boost/tokenizer.hpp>
#include "../sesecurities/sesecuritiesapi.h"
#include "../sexbrl/sexbrlapi.h"
#include <boost/regex.hpp>
#include <unordered_set>
#include <boost/unordered/unordered_set.hpp>

using namespace se::data;
using namespace mysqlpp;
using namespace std;
using namespace boost;
//using namespace log4cxx;
using namespace boost;
using namespace se;
using namespace se::securities;
using namespace se::xbrl;

void CopyInstanceSegmentData()
{
    
    ////LoggerPtr logger = Logger::getLogger("CopyInstanceSegmentData");
    
    
    CSmartConnection conn(CSEConnections::GetMain());
    

        // use a cursor to loop
        //Query qInst = conn->query();
        //qInst << "select count(0) from se_fin_instance";
        //StoreQueryResult sr = qInst.store();
        //U64 count = sr[0][0];
        //if (count)
        while (true)
        {
            try
            {
                //ldiv_t dvr = ldiv(count, 100);
                //for (U64 l = 0; dvr.quot; l+=100)
                //U64 l0 = 0;
                //U64 l1 = 0;
                //for(;;)
        //        {
        //            l0 = l1;
        //            if (l0 > dvr.quot)
        //            {
        //                if (!dvr.rem)
        //                    break;
        //                else
        //                    l1 = l1 + dvr.rem;
        //            }
        //            else
        //                l1 += 100;

                    Query qInst = conn->query();
                    //qInst << "select instance_id, contextref from se_fin_instance where seg_xbrl_id is null limit " << l0 << ", " << l1-1;
                    qInst << "select instance_id, contextref from se_fin_instance where seg_xbrl_id is null limit 100000"; 
                    StoreQueryResult sqr = qInst.store();
                    if (!sqr.size())
                        break;
                    //UseQueryResult uqr = qInst.use();
                    //if (!uqr)
                      //  break;

                    //while (Row row = uqr.fetch_row())
                    UID counter  = 0;
                    for (Row& row : sqr)
                    {
                        if (0 == (counter % 1001))
                            sleep(2);

                        UID segId = 0, dimId = 0;
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
                                             <xbrli:entity>
                                            <xbrli:identifier scheme="http://www.sec.gov/CIK">0000066740</xbrli:identifier>
                                            <xbrli:segment>
                                            <xbrldi:explicitMember dimension="us-gaap:PropertyPlantAndEquipmentByTypeAxis">us-gaap:LandMember</xbrldi:explicitMember>
                                            </xbrli:segment>
                                            </xbrli:entity>
                                             */


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

                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        qInst = conn->query();
                        qInst << "update se_fin_instance set seg_xbrl_id = " << quote << segId ;
                        qInst << ", seg_dim_xbrl_id = " << quote << dimId ;
                        qInst << " where instance_id = " << row[0];

                       qInst.execute();

                       //LOG4CXX_INFO(logger, "Updated record " << row[0]);


                        xmlFreeDoc(doc);
                        
                        counter ++;

                    }
            }
            catch(const std::exception& ex)
            {
                ////LOG4CXX_ERROR(logger, ex.what());
            }
            sleep(1);
        }
}

typedef std::unordered_set<string> FullList;

void PurgeUnlistedCompanies(FullList freshList, int exchange);

void RepopulateListedCompanies(std::initializer_list<std::string> files)
{
    SP<ISecurityAPI> pSecApi;
    SP<IXbrl> pxbrlApi;
    SecOpen(&pSecApi);
    XbrlOpen(&pxbrlApi);
    
    CSmartConnection conn(CSEConnections::GetMain());
    
    for(string file : files)
    {
        /*
         * a. Read a line
         * b. Capture Sector and Sub-sector
         * c. Attempt to upsert in sec_sics table both sector and sub-sector. 
         * d. Get sub-sector id.
         * e. Get the symbol,
         * f. Query for info from Edgar location. Get cik, address, etc.
         * g. Now upsert a record in se_sec_products table.
         * h. Upsert a record in se_edgar_ciks table
         * j. the relationship between cik and product_id is saved in edgar_product_cik table.
         * h. 
         */
        
        int exchange;
        if (file.find("amex") != string::npos)
            exchange = 3;
        else if (file.find("nasdaq") != string::npos)
            exchange = 2;
        else if (file.find("nyse") != string::npos)
            exchange = 1;
        const string country = "US";
        ifstream ifs(file.c_str(), ifstream::in);
        if (!ifs.good() || !ifs.is_open())
            continue;
        //char_separator<char> sep(",");
        string line;
        typedef tokenizer< escaped_list_separator<char> > Tokenizer;
        typedef vector<string> Fields;
        FullList allsymbols;
        // skip first line
        getline(ifs, line);
        while (getline(ifs, line))
        {
            Tokenizer tokz(line);
            Fields flds;
            flds.assign(tokz.begin(), tokz.end());
            // Symbol(0),Name(1),LastSale(2),MarketCap(3),ADR TSO(4),IPOyear(5),Sector(6),industry(7),Summary Quote(8)
            string& symbol = flds[0];
            string& sector = flds[6];
            string& industry = flds[7];
            string& mktcap = flds[3];
            string& name = flds[1];
            string& ipoyear = flds[5];
            string& adotso = flds[4];

            boost::trim(symbol);
            boost::trim(sector);
            boost::trim(industry);
            boost::trim(mktcap);
            boost::trim(name);
            boost::trim(ipoyear);
            boost::trim(adotso);
            boost::trim(name);
            
            allsymbols.insert((string)symbol);

            int gicid = 0;
            
            if (industry != "n/a")
            {
                // insert/update GICS
                Query q = conn->query();
                q << "select gics_id, gics_name, gics_parent_id from se_sec_gics where gics_name = " << quote << sector;
                StoreQueryResult sqr = q.store();
                if (sqr.size())
                {
                    int parentgicid = sqr[0][0];
                    q = conn->query();
                    q << "select gics_id, gics_name, gics_parent_id from se_sec_gics where gics_name = " << quote << industry;
                    sqr = q.store();
                    if (sqr.size())
                    {
                        gicid = sqr[0][0];
                    }
                    else
                    {
                        q = conn->query();
                        q << "insert se_sec_gics (gics_name, gics_parent_id) values ( " << quote << industry << ", " << parentgicid << " )";
                        cout << q.str() << endl;
                        q.exec();
                        gicid = q.insert_id();
                    }
                }
                else
                {
                    q = conn->query();
                    q << "insert se_sec_gics (gics_name, gics_parent_id) values ( " << quote << sector << ", 0 )";
                    q.exec();
                    int newId = q.insert_id();
                    q = conn->query();
                    q << "insert se_sec_gics (gics_name, gics_parent_id) values ( " << quote << industry << ", " << newId << " )";
                    cout << q.str() << endl;
                    q.exec();
                    gicid = q.insert_id();
                }
            }
            cout << "Symbol: " << symbol << endl;
            UID prodId;
            Query q = conn->query();
            q << "select product_id, product_symbol from se_sec_products where product_symbol = " << quote << symbol;
            q << " and product_exchange_id = " << exchange ;
            q << " and product_country_id = " <<  1;
            StoreQueryResult sqr = q.store();
            if (!sqr.size())
            {
                q = conn->query();
                q << "insert into se_sec_products (product_symbol, product_name, product_ipo_year, ";
                q << "product_gic_id, product_exchange_id, product_security_type_id, product_country_id, product_currency_id, ";
                q << "product_marketcap, product_adotso )";
                q << "values (";
                q << quote << symbol << ", ";
                q << quote << name << ", ";
                q << (ipoyear=="n/a"?"NULL":ipoyear) << ", ";
                q << gicid << ", ";
                q << exchange << ", ";
                q << 1 << ", ";
                q << 1 << ", ";
                q << 1 << ", ";
                q << (mktcap=="n/a"?"0":mktcap) << ", ";
                q << (adotso=="n/a"?"NULL":adotso) ;
                q << ")";
                cout << q.str() << endl;
                q.exec();
                prodId = q.insert_id();
            }
            else
            {
                prodId = sqr[0][0];
                q = conn->query();
                q << "update se_sec_products set product_marketcap = " << (mktcap=="n/a"?"0":mktcap) << ", ";
                q << "product_adotso = " << (adotso=="n/a"?"NULL":adotso);
                q << " where product_id = " << prodId;
                cout << q.str() << endl;
                q.exec();
            }
            
            // make an edgar CIK check.
            SP<IString> cik;
            if (SUCCEEDED(pxbrlApi->GetCIK(symbol.c_str(), &cik)))
            {
                q = conn->query();
                q << "select cik, companyname from se_edgar_ciks where cik = " << cik->get_Buffer();
                sqr = q.store();
                if (sqr.size())
                {
                    string coName = (string)sqr[0][1];
                    if (coName.empty())
                    {
                        q = conn->query();
                        q << "update se_edgar_ciks set companyname = " << quote << name << " where cik = " << cik->get_Buffer();
                        cout << q.str() << endl;
                        q.exec();
                    }
                    q = conn->query();
                    q << "select * from se_edgar_productid_cik where product_id = " << prodId;
                    if (q.store().size())
                    {
                        q = conn->query();
                        q << "update ignore se_edgar_productid_cik set cik = " << cik->get_Buffer() << " where product_id = " << prodId;
                        cout << q.str() << endl;
                        q.exec();
                    }
                    else
                    {
                        q = conn->query();
                        q << "INSERT INTO se_edgar_productid_cik (product_id, cik) VALUES (";
                        q << prodId ;
                        q << ", ";
                        q <<  cik->get_Buffer();
                        q << ")";
                        cout << q.str() << endl;
                        q.exec();
                    }
                }
                else
                {
                    q = conn->query();
                    q << "insert into se_edgar_ciks (cik, companyname) values (" << cik->get_Buffer() << ", " << quote << name << ")";
                    cout << q.str() << endl;
                    q.exec();

                    q = conn->query();
                    q << "INSERT INTO se_edgar_productid_cik (product_id, cik) VALUES (";
                    q << prodId ;
                    q << ", ";
                    q <<  cik->get_Buffer();
                    q << ")";
                    cout << q.str() << endl;
                    q.exec();
                }            
            }
            else
            {
                q = conn->query();
                q << "delete from se_edgar_productid_cik where product_id = " << prodId;
                cout << q.str() << endl;
                q.exec();
            }
        }
        ifs.close();
        
        Query q = conn->query();
        q << "update se_sec_products set product_yahoosymbol = product_symbol";
        q.exec();
        q = conn->query();
        q << "update se_sec_products set product_yahoosymbol = replace(product_yahoosymbol, '^', '-P')";
        q.exec();
        
    }
}

void PurgeUnlistedCompanies(FullList freshList, int exchange)
{
    /* TODO:
     * Purge all CIK records in formrecs, presentatino/calculation,instance/label where the company either
     doesn't exist or not publicly listed in any of the american exchanges .
     */
   
    // BELOW CODE ONLY REMOVED SYMBOLS WHICH ARE NOT IN NEWLY PUBLISHED LIST
    // based not in freshList
    CSmartConnection conn(CSEConnections::GetMain());
    Query q = conn->query();
    q << "select product_id, product_symbol from se_sec_products where product_security_type_id = 1 and product_exchange_id = " << exchange;
    StoreQueryResult sqr = q.store();
    cout << "Starting to purge for exchagne " << exchange << endl;
    for (const Row& r : sqr)
    {
        string product_id = (string)r[0];
        string symbol = (string)r[1];
        trim(symbol);
        if (freshList.find(symbol) == freshList.end())
        {
            cout << "Deleting " << symbol << endl;
            q = conn->query();
            q << "delete from se_edgar_productid_cik where product_id = " << product_id;
            q.exec();
            q = conn->query();
            q << "delete from se_sec_eq_pricehistory where product_id = " << product_id;
            q.exec();
            q = conn->query();
            q << "delete from se_sec_products where product_id = " << product_id;
            q.exec();
            cout << "Deleted " << symbol << endl;
        }
    }
    
    
    /////////TODO////// TEST BELOW SP CALL THROUGHLY ///////////////
    q = conn->query();
    q << "call upd_DisableInvalidProducts();";
    q.exec();
    
    cout << "Completed purging for exchagne " << exchange << endl;
}

void UploadSecurityWebLinks(const string& file)
{
    CSmartConnection conn(CSEConnections::GetMain());

    ifstream ifs(file.c_str(), ifstream::in);
    if (!ifs.good() || !ifs.is_open())
        return;

    string line;
    typedef tokenizer< escaped_list_separator<char> > Tokenizer;
    typedef vector<string> Fields;
    // skip first line
    getline(ifs, line);
    while (getline(ifs, line))
    {
        Tokenizer tokz(line);
        Fields flds;
        flds.assign(tokz.begin(), tokz.end());
        //product_id	newspage	About Company
        string& prodid = flds[0];
        string& newspage = flds[1];
        string& about = flds[2];
        boost::trim(prodid);
        boost::trim(newspage);
        boost::trim(about);
        replace_all(about, "\r", "");
        replace_all(newspage, "**", "");
        replace_all(about, "**", "");
        
        if (newspage.empty() && newspage.empty())
            continue;
        
        Query q = conn->query();
        q << "insert ignore into se_sec_productweblinks (product_id, newslink, aboutlink) ";
        q << "VALUES (";
        q << prodid << ", ";
        q << quote << newspage << ", ";
        q << quote << about;
        q << ")";
        cout << q.str() << endl;
        q.exec();
    }
    
}