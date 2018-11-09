/* 
 * File:   CParseCompanyProfile.cpp
 * Author: santony
 * 
 * Created on 5 September, 2012, 6:34 AM
 */

#include "internal.h"
#include "../seglobal/stringconversions.h"


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include<boost/tokenizer.hpp>
#include<boost/regex.hpp>
//#include <log4cxx/logger.h>
#include<boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

#include "../ReturnCodes.h"
#include "CParseCompanyProfile.h"
#include "GICSInfo.h"
#include "PublicCompanyInfo.h"
#include <libxml/xmlstring.h>
#include <iconv.h>
#include "../senet/senetapi.h"

using namespace boost::algorithm;
using namespace std;

//using namespace log4cxx;

CParseCompanyProfile::CParseCompanyProfile(ISession* ps, ISecurityDescription* secDesc) :
    _ps(ps),
    _pSecDesc(secDesc),
    _pDoc(0),
    _pNodeQuote(0), 
    _pNodeDetails(0),
    _pNodeSummary(0)
{
}


CParseCompanyProfile::~CParseCompanyProfile() 
{
}

#include "../semysql/CSEConnections.h"
#include "../external/mariadbpp/mysql++.h"
#include "../external/mariadbpp/ssqls.h"
#include <list>
#include <vector>

#define SEDBCONN        CSEConnections::GetMain()

using namespace se;
using namespace se::data;


ErrorId CParseCompanyProfile::Process(CPublicCompanyInfo* pCo)
{    
    // step6: log functions.
    
    try
    {
        //LOG4CXX_INFO(_pLogger, "Starting profile processing....");
        CSmartConnection conn(SEDBCONN);

        UID gicParent;

        // wanted variables for public copmay instnce.
        SP<IGICInfo> gicsInfo;
        I32 gicId = 0;
        bool bNeedWebData = false;
        /* check if db contains information we need. We will check on summary field.*/
        Query qProduct = conn->query();
        qProduct << "select * from se_sec_products where product_id = " << _pSecDesc->get_Id();
        //LOG4CXX_INFO(_pLogger, qProduct.str());
        StoreQueryResult rProduct = qProduct.store();
        if (!rProduct.size())
            return E_NotFound;

        bNeedWebData = false;
        Row& row = rProduct[0];
        // if we don't had address etc, we will get it from yahoo.
        string state = (string)row["product_state"];
        if (state.empty())
        {
            bNeedWebData = true;
        }

        string sector = "";
        string industry = "";

        gicId = row["product_gic_id"];
        // skip this if gics is zero
        if (gicId != 0)
        {
            // we simply need two queries, one for industry and then for sector.
            Query qGics = conn->query();
            qGics << "select gics_id, gics_name, gics_parent_id from se_sec_gics where gics_id = " << gicId;
            StoreQueryResult rGics = qGics.store();
            if (rGics.size())
            {
                industry = (string)rGics[0]["gics_name"];
                gicParent = rGics[0]["gics_parent_id"];
                if (gicParent > 0)
                {
                    qGics.reset();
                    qGics << "select gics_id, gics_name from se_sec_gics where gics_id = " << gicParent;
                    rGics = qGics.store();
                    if (rGics.size() > 0)
                        sector = (string)rGics[0]["gics_name"];
                }
            }
        }
        
        // see if SIC Info is available.
        Query qEdgar = conn->query();
        qEdgar << "select a.cik, trim(c.sic_name) as sic_name from se_edgar_ciks a  \
            inner join se_edgar_productid_cik b on (a.cik = b.cik) \
            inner join se_edgar_sic c on (a.sic_id = c.sic_id) \
            where b.product_id = " << _pSecDesc->get_Id() << ";";

        //LOG4CXX_INFO(_pLogger, qEdgar.str());
        
        StoreQueryResult rEdgar = qEdgar.store();
        if (rEdgar.size())
        {
            pCo->set_CIK(rEdgar[0][0]);
            sql_varchar_null sicName(rEdgar[0][1]);
            if (!sicName.is_null)
                pCo->set_SICInfo((CSTR)sicName.data.c_str());
        }

        // build gics
        RETURNIFFAILED(CObject<CGICSInfo>::Create(_ps,  sector.c_str(), industry.c_str(),  &gicsInfo ));
        gicsInfo->set_Id(gicId);
        pCo->set_GICInfo(gicsInfo);
        pCo->set_Name(((string)row["product_name"]).c_str());
        pCo->set_Id(row["product_id"]);

        SP<IAddress> pAddr;
        RETURNIFFAILED(CreateAddress(_ps, &pAddr));
        if (bNeedWebData)
        {
            RETURNIFFAILED(GetCompany(pCo));
        }
        else
        {
            pAddr->set_City(((string)row["product_city"]).c_str());
            pAddr->set_State(((string)row["product_state"]).c_str());
            pAddr->set_Street(((string)row["product_street"]).c_str());
            pAddr->set_Country(((string)row["product_country"]).c_str());
            pAddr->set_PostalCode(((string)row["product_postalcode"]).c_str());
            pCo->set_Summary(row["product_summary"]);
        }
        pCo->set_Address(pAddr);
        
        
        Query qurl = conn->query();
        qurl << "SELECT * FROM se_sec_productweblinks where product_id = " << pCo->get_Id();
        auto sqrurl = qurl.store();
        if (sqrurl.size())
            pCo->set_UrlNews(sqrurl[0]["newslink"]);
        
        return S_Success;
    }
    catch (std::exception& ex)
    {
        //LOG4CXX_ERROR(_pLogger, "Reported error : " << ex.what());
        return E_UnknownError;
    }
}

ErrorId CParseCompanyProfile::GetCompany(CPublicCompanyInfo* pCo)
{
    xmlBufferPtr nodeBuffer = 0;
    xmlChar *key = 0;
    xmlDocPtr doc = 0;
    xmlNodePtr cur = 0;
    try
    {
        string url = "http://finance.yahoo.com/q/pr?s=" + (string)_pSecDesc->get_Symbol();
        nodeBuffer = xmlBufferCreate();
        doc = htmlParseFile (url.c_str(), NULL);
        cur = xmlDocGetRootElement(doc);
        cur = cur->xmlChildrenNode;
        cur = cur->next;
        xmlNodeDump(nodeBuffer, doc, cur, 0, 1);
        key=nodeBuffer->content;
        _sHtmlText = (CSTR)key;
        xmlBufferFree(nodeBuffer);
        xmlFreeDoc(doc);

    }
    catch(std::exception &ex)
    {
        if (nodeBuffer)
            xmlBufferFree(nodeBuffer);
        
        if (doc)
            xmlFreeDoc(doc);

        //LOG4CXX_ERROR(_pLogger, "Failed to read company profile from remote site. Error : " << ex.what());
        return E_HttpError;
    }
    
    //LOG4CXX_INFO(_pLogger, "Processing completed."); 
    CSmartConnection conn(SEDBCONN);
    
    string addresStr = ReadAddress();
    std::vector<std::string> strs;
    boost::split(strs, addresStr, boost::is_any_of(":"));
    
    if (strs.size() < 6)
        return E_NotFound;

    
    string street, city, state, postalcode, country;
    street = strs[1];
    city = strs[2];
    state = strs[3];
    postalcode = strs[4];
    country = strs[5];

    SP<IAddress> address;
    RETURNIFFAILED(CreateAddress(_ps, &address));

    address->set_Street(street.c_str());
    address->set_City(city.c_str());
    address->set_State(state.c_str());
    address->set_PostalCode(postalcode.c_str());
    address->set_Country(country.c_str());
    pCo->set_Address(address);
    string summary = ReadSummary();
    pCo->set_Summary(summary.c_str());
    
    Query q = conn->query();
    q << "update se_sec_products set \
            product_street = " << quote << street <<
            ", product_city = " << quote << city << 
            ", product_state = " << quote << state << 
            ", product_country = " << quote << country << 
            ", product_postalcode = " << quote << postalcode << 
            ", product_summary = " << quote << summary << 
            " where product_id = " << _pSecDesc->get_Id();
    q.exec();
    //LOG4CXX_INFO(_pLogger, "proruct updated sucessfully"); 
    return S_Success;
}

const char* CParseCompanyProfile::ReadQuote()
{
    string &answer = _sHtmlText;
    //const string pattern="(.*)(>Index Membership:</td><td class=)(.*)(>Sector:</td><td)(.*)(>)(.*)(</a></td></tr><tr><td)(.*)(>Industry:</td><td)(.*)(>)(.*)(</a></td></tr><tr><td)(.*)(>)(.*)(</td></tr></table></td></tr></table><br/><table)(.*)(>Business Summary</span></th><th)(.*)";  
    const string pattern="(.*)(</td></tr></table><br/><div class=)(.*)(>)(.*)(</span></span><span class=)(.*)";  
    boost::regex regexPattern(pattern, boost::regex::extended);
    boost::smatch what;
    string quote;
    bool isMatchFound = boost::regex_match(answer, what, regexPattern);  
     
        if (isMatchFound)
            {
                  for (unsigned int i=0; i < what.size(); i++)
                  {
                    if(i==1)
                    {
                        quote=what[5];
                    }     /*  
    const string pattern5="(.*)(<tr><td><big><b>Profile</b></big></td><td)(.*)(>Map</a><br/>)(Phone:)(.*)(<br/>)(Fax:)(.*)(<br/>Website:)(.*)(</a><br/><br/><table width=)(.*)(>Details</span></th><th align=)(.*)";
    boost::regex regexPattern5(pattern5, boost::regex::extended);
    boost::smatch what5;
    
    const string pattern6="(.*)(<tr><td><big><b>Profile</b></big></td><td)(.*)(>Map</a><br/>)(Phone:)(.*)(<br/>)(.*)(Website:)(.*)(</a><br/><br/><table width=)(.*)(>Details</span></th><th align=)(.*)";
    boost::regex regexPattern6(pattern6, boost::regex::extended);
    boost::smatch what6;*/
                  }
            }
    
    return quote.c_str();
}

const char* CParseCompanyProfile::ReadAddress()
{   
    string &answer = _sHtmlText;
    const string pattern2="(.*)(<tr><td><big><b>Profile</b></big></td><td)(.*)(><b>)(.*)(</b><br/>)(.*)(<br/>)(.*)(,)(.*)(<br/>)(.*)( - )(<a href=)(.*)(>Map</a><br/>)(.*)(>)(.*)(</a><br/><br/><table width=)(.*)(>Details</span></th><th align=)(.*)";
    boost::regex regexPattern2(pattern2, boost::regex::extended);
    boost::smatch what2;
  /*  
    const string pattern5="(.*)(<tr><td><big><b>Profile</b></big></td><td)(.*)(>Map</a><br/>)(Phone:)(.*)(<br/>)(Fax:)(.*)(<br/>Website:)(.*)(</a><br/><br/><table width=)(.*)(>Details</span></th><th align=)(.*)";
    boost::regex regexPattern5(pattern5, boost::regex::extended);
    boost::smatch what5;
    
    const string pattern6="(.*)(<tr><td><big><b>Profile</b></big></td><td)(.*)(>Map</a><br/>)(Phone:)(.*)(<br/>)(.*)(Website:)(.*)(</a><br/><br/><table width=)(.*)(>Details</span></th><th align=)(.*)";
    boost::regex regexPattern6(pattern6, boost::regex::extended);
    boost::smatch what6;*/
    
    int flag1=0; 
    string profile="";
    bool isMatchFound2 = boost::regex_match(answer, what2, regexPattern2);   
        if (isMatchFound2)
            {
                  for (unsigned int k=0; k < what2.size(); k++)
                  {
                    if(k==5){
                 
                                 string str=what2[11];
                                 trim(str);
                                 istringstream iss(str);
                                 int i=0;
                                 std::string state;
                                 std::string postalcode;    
                                 do
                                 {
                                        string sub;
                                        iss >> sub;
                                        if(i==0)
                                                state=sub;    
                                        else if(i==1)
                                                postalcode=sub;    
                                        i++;
                                 } while (iss);
                                 //cout<<what2[13]<<endl;
                                 profile=what2[5]+":"+what2[7]+":"+what2[9]+":"+state+":"+postalcode+":"+what2[13];
                      }
                  }
            }
   
    
  /*  bool isMatchFound5 = boost::regex_match(answer, what5, regexPattern5);   
        if (isMatchFound5)
            {
                  flag1=1;
                  for (unsigned int k=0; k < what5.size(); k++)
                  {
                    if(k==2){                      
                        profile=profile+"\n"+what5[6]+"\n"+what5[9];
                        cout<<endl;
                     }
                  }
            }
   
    if(flag1==0)
    {
        bool isMatchFound6 = boost::regex_match(answer, what6, regexPattern6);   
            if (isMatchFound6)
                {
                      for (unsigned int k=0; k < what6.size(); k++)
                      {
                        if(k==2){                      
                            profile=profile+"\n"+what6[6];
                            cout<<endl;
                         }
                      }
                }
    }*/
      return profile.c_str();
}

const char* CParseCompanyProfile::ReadDetails()
{
    string &answer = _sHtmlText;
    const string pattern3="(.*)(>Index Membership:</td><td class=\")(.*)(>)(.*)(</td></tr><tr><td)(.*)(>Sector:</td><td)(.*)";
    boost::regex regexPattern3(pattern3, boost::regex::extended);
    boost::smatch what3;
    
    const string pattern="(.*)(>Index Membership:</td><td class=)(.*)(>Sector:</td><td)(.*)(>)(.*)(</a></td></tr><tr><td)(.*)(>Industry:</td><td)(.*)(>)(.*)(</a></td></tr><tr><td)(.*)(>)(.*)(</td></tr></table></td></tr></table><br/><table)(.*)(>Business Summary</span></th><th)(.*)";  
    boost::regex regexPattern(pattern, boost::regex::extended);
    boost::smatch what;
        
    const string pattern4="(.*)(>Index Membership:</td><td class=\")(yfnc_tabledata1\")(><a href=\")(http)(://)(finance.yahoo.com/)(q)(\\?)(s=%)(5EDJA)(&)(amp)(;)(d=t)(\")(>)(.*)(</a><br/)(><a href=\")(http)(://)(finance.yahoo.com/)(q)(\\?)(s=%)(5EDJI)(&)(amp)(;)(d=t)(\")(>)(.*)(</a>)(.*)(>Sector:</td><td)(.*)";
    boost::regex regexPattern4(pattern4, boost::regex::extended);
    boost::smatch what4;
    
    string details="";
    int flag=0;  
    bool isMatchFound4 = boost::regex_match(answer, what4, regexPattern4);  
    if (isMatchFound4)
            {
            flag=1;
                  for (unsigned int i=0; i < what4.size(); i++)
                  {
                    if(i==1){
                        //cout<<endl;
                        details="Index Membership:\t"+what4[18]+"\n\t\t\t"+what4[34];
                        //cout<<endl;
                     }
                  }
            }
    
      if(flag==0)
    {
        bool isMatchFound3 = boost::regex_match(answer, what3, regexPattern3);   
            if (isMatchFound3)
                {
                      for (unsigned int i=0; i < what3.size(); i++)
                      {
                        if(i==1){
                            details="Index Membership:\t"+what3[5];
                            //cout<<endl;
                         }
                      }
                }
        //cout<<"end"<<endl;
    }
     bool isMatchFound = boost::regex_match(answer, what, regexPattern);  
     
    if (isMatchFound)
        {
              for (unsigned int i=0; i < what.size(); i++)
              {
                if(i==1){
                    //details=details+"\n"+"Sector:\t\t\t"+what[7]+"\nIndustry:\t\t"+what[13]+"\nFull Time Employees:\t"+what[17];
                    details=what[13]+":"+what[7];
                    //cout<<endl;
                 }
              }
        }

     return details.c_str();  
}

const char* CParseCompanyProfile::ReadSummary()
{
    string &answer = _sHtmlText;
    const string pattern1="(.*)(</tr></table><p>)(.*)(</p><p><a href=)(.*)";
    boost::regex regexPattern1(pattern1, boost::regex::extended);
    boost::smatch what1;
    bool isMatchFound1 = boost::regex_match(answer, what1, regexPattern1);   
           if (isMatchFound1)
               {
                     for (unsigned int j=0; j < what1.size(); j++)
                     {
                       if(j==3)
                       {
                           //cout<<endl;
                           _sInfoText=what1[3];
                           break;
                       }
                     }
               }  
   return _sInfoText.c_str();
}

