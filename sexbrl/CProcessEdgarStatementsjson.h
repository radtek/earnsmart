/* 
 * File:   CProcessEdgarStatements.h
 * Author: aashok
 *
 * Created on June 29, 2013, 7:12 AM
 */

#ifndef CPROCESSEDGARSTATEMENTS_H
#define	CPROCESSEDGARSTATEMENTS_H

////#include <log4cxx/logger.h>
#include "../external/mariadbpp/mysql++.h"
#include "../external/mariadbpp/ssqls.h"
#include <string>
#include <map>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include "../semysql/CSEConnections.h"
#include <Poco/NamedMutex.h>
#include <atomic>

////using namespace log4cxx;
using namespace std;
using namespace se::data;
using namespace se::xbrl;
using namespace Poco;

class CProcessEdgarStatements 
{
private:
    typedef map<string, xmlDocPtr> XMLRefs;
    typedef pair<string, xmlDocPtr> XMLRefItem;
    
    typedef map<string, string> XmlNs;
    typedef pair<string, string> StringPair;
    typedef map<string, XmlNs> RegisteredNs;
    typedef pair<string, XmlNs> NsPair;

    typedef std::map<string, xmlNodePtr> Contexts;
    typedef std::map<string, xmlNodePtr> Units;
    typedef std::multimap<string, xmlNodePtr> Elements;
    typedef std::map<string, xmlNodePtr> Miscs;
    
    struct properties
    {
        string fieldDate;
        string ChangeDate; 
        string CompanyName; 
        string IncState;    //incorporated state
        string cikValue;
        std::pair<int, string> sic;
        string taxid; //rename this to taxid
        string FYearEnd;
        string TYPE;
        string BStreet1;
        string MStreet1;
        string  BStreet2;
        string  MStreet2;
        string  City;
        string  MCity;
        string  State;
        string  MState;
        string  ZIP;
        string  MZIP;
        string  Phone;
        string  MPhone;
        string  XSD_Fname;
        string  XSD_Fname1;
        string XML_Instance;
        XMLRefs xbrlDocs;
        RegisteredNs   nsDocs;
        I64 recid;
        
        properties()
        {
            fieldDate=ChangeDate=CompanyName=IncState=IncState=cikValue=taxid=FYearEnd=FYearEnd=TYPE=BStreet1=MStreet1=BStreet2=MStreet2=City=MCity=State=MState=ZIP=MZIP=Phone=MPhone="";
        }
        
        ~properties()
        {
            clear();
        }
        
        void clear()
        {
            if (xbrlDocs.size())
            {
                // free all xml documents
                for (auto pr : xbrlDocs)
                {
                    if (pr.second)
                            xmlFreeDoc(pr.second);
                }
                xbrlDocs.clear();
            }
//            for (auto pr : nsDocs)
//            {
//                xmlFreeNs(&pr.second);
//            }
            if (nsDocs.size())
                nsDocs.clear();
            
            if (!cikValue.empty())
                fieldDate=ChangeDate=CompanyName=IncState=IncState=cikValue=taxid=FYearEnd=FYearEnd=TYPE=BStreet1=MStreet1=BStreet2=MStreet2=City=MCity=State=MState=ZIP=MZIP=Phone=MPhone="";
        }
    };

private:
    ISession* _ps;
    
public:
    CProcessEdgarStatements(ISession*);
    CProcessEdgarStatements(const CProcessEdgarStatements& orig) = delete;
    virtual ~CProcessEdgarStatements();
    
    ErrorId Process();
    ErrorId Process(UID recId);
    
private:
    void Init();
    ErrorId DownloadUsGaapSchemas();
    ErrorId ProcessRecord(CSmartConnection &conn, mysqlpp::Row& row );
    ErrorId AquireRecord(CSmartConnection &conn, Row& outRow);
    ErrorId AquireRecord(CSmartConnection &conn, UID recId, Row& outRow);
    ErrorId SaveProcessStatus(CSmartConnection &conn, I64 recId, ErrorId retCode);
    ErrorId ParseEdgarHtmlData(CSmartConnection &conn,CSTR path,properties &ppt1,CSTR Fpath);
    ErrorId IdentifyXbrlFiles(CSmartConnection &conn,CSTR path, CSTR Fpath, properties &ppt1);
    ErrorId StoreEdgarHtmlData(CSmartConnection& conn, properties &ppt1);
    UID FindProductId(CSmartConnection& conn, const string& companyName);
    ErrorId CIKMissingEdgarHtmlData(CSmartConnection& conn, UID, properties &ppt1,CSTR isID,CSTR bsID,CSTR msID);
    ErrorId ProcessStatement(CSmartConnection& conn, mysqlpp::Row &row,CSTR path,properties &ppt1);
    ErrorId ProcessPresentation(CSmartConnection& conn, mysqlpp::Row &row,CSTR path,properties &ppt1);
    ErrorId ProcessLabel(CSmartConnection& conn, mysqlpp::Row &row,CSTR path,properties &ppt1);

    void RegisterNamespaces(xmlDocPtr, XmlNs&);
    void RegisterNamespace(string nsname, string nsurl, XmlNs&);
    
    ErrorId CacheSchema(CSmartConnection &conn,string& url, properties &ppt1);
    
    // register for xpathcontext
    ErrorId RegisterNsInXpathContext(xmlXPathContextPtr context, const XmlNs& nsdoc);
    string GetDocDefaultNamespace(const xmlNodePtr);
    
    ErrorId ProcessRoleRef(CSmartConnection &conn, const xmlDocPtr doc, const xmlNodePtr roleRefNode,mysqlpp::Row& row,CSTR path,properties &ppt1, STM doctype, xmlXPathContextPtr preContext);
    ErrorId ProcessSchema(CSmartConnection &conn,mysqlpp::Row& row,CSTR path,properties &ppt1,CSTR fileName,CSTR xsd_Id);
    I64 getFnElement(CSmartConnection &conn, mysqlpp::Row& row,CSTR path,properties &ppt1,CSTR href,const xmlNodePtr roleRefNode);
    ErrorId ProcessCalculation(CSmartConnection& conn, mysqlpp::Row &row,CSTR path,properties &ppt1);
    ErrorId ProcessCalRoleRef(CSmartConnection &conn, const xmlDocPtr doc, const xmlNodePtr roleRefNode,mysqlpp::Row& row,CSTR path,properties &ppt1, STM doctype, xmlXPathContextPtr preContext);
    ErrorId ProcessInstance(CSmartConnection& conn, mysqlpp::Row& row, const char* path, properties& ppt1);
    //ErrorId InsertInstance(CSmartConnection& conn, CIK identifier, CSTR contextref,const xmlNodePtr RefNode, CSTR startDate, CSTR endDate, vector<UID>&, vector<UID>&, xmlXPathContextPtr xpathContext,properties &ppt1);
    ErrorId InsertInstance(
        CSmartConnection& conn, 
        const xmlNodePtr elementNode, 
        const xmlNodePtr contextNode, 
        const xmlNodePtr unitNode, 
        properties &ppt1);
    ErrorId InsertLabel(CSmartConnection& conn, CSTR XbrHref, CSTR Label, xmlXPathContextPtr xpathContext, properties &ppt1, mysqlpp::Row &recrow);
    ErrorId DeleteIfFailed(CSmartConnection& conn, UID rec_id);
    
    int getLastError();
    
private:
    static std::mutex _mutex;
    string defNsPfx;
    RegisteredNs xbrlNs;
    XMLRefs xbrlRefs ;
    string XMLDOCSAVELOCATION;
    string content;
    string _cacheSchemaPath ;

    UID _lastRecId;
    string _sqlRecord;
    CSTR _deleteInstcontext;
    CSTR _deleteInst;
    const string _deleteCalc;
    const string _deletePre;
    const string _deleteStmt;
    static bool _bFixConcurrency;
    string _instTableName;
    string _ctxtTableName;
    U64 _cik;
    string temporaryDir;
    string edgarUrl;
};

#endif	/* CPROCESSEDGARSTATEMENTS_H */

