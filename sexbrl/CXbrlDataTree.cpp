/* 
 * File:   CXbrlDataTree.cpp
 * Author: aashok
 * 
 * Created on July 9, 2013, 8:43 PM
 */

#include "sexbrlinternal.h"
#include "CXbrlDataTree.h"
#include <../semysql/CSEConnections.h>
#include "../external/mariadbpp/mysql++.h"
#include "../external/mariadbpp/ssqls.h"
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/properties.hpp>
#include <map>
#include "../seglobal/CStringUtility.h"
#include "CXbrlDataNode.h"
#include<boost/regex.hpp>
#include <libxml2/libxml/xmlreader.h>
#include <sstream>
#include <iostream>
#include <boost/date_time.hpp>
#include <boost/algorithm/string/split.hpp>
#include "../seglobal/CQuarter.h"
#include "CStatementUtilities.h"

#define SEDBCONN        CSEConnections::GetMain()

using namespace se::xbrl;
using namespace std;
using namespace se;
using namespace boost;	
using namespace se::data;
using namespace mysqlpp;
using namespace se::utils;
//using namespace log4cxx;

const char* Instant="(.*)(xbrli:instant>)(.*)(</xbrli:instant>)";
const char* Period="(.*)(xbrli:endDate>)(.*)(</xbrli:endDate>)";

sql_create_8
(
        DataXbrlLookup, 
        1, 
        8, 
        sql_bigint, xbrl_id,
        sql_varchar, xbrl_prefix,
        sql_varchar, xbrl_name,
        sql_varchar, xbrl_type,
        Null<sql_varchar>, xbrl_balance,
        sql_varchar, xbrl_PeriodType,
        Null<sql_varchar>, xbrl_href,
        Null<sql_text>, xbrl_text
);

//x.xbrl_id, x.label as xbrl_label
sql_create_2
(
        DataXbrlLabels,
        1,
        2,
        sql_bigint, xbrl_id,
        sql_varchar, xbrl_label
);

//xbrl_id, monetaryvalue, stringvalue, numericvalue, contextref
sql_create_7
(
        DataXbrlInstance,
        1,
        7,
        sql_bigint, xbrl_id,
        sql_double, monetaryvalue,
        sql_text, stringvalue,
        sql_int, numericvalue,
        sql_date_null, startdate,
        sql_date_null, enddate,
        sql_smallint, contextref
);

CXbrlDataTree::CXbrlDataTree(ISession* ps) : CObjectRoot(ps) 
{
    
}

CXbrlDataTree::~CXbrlDataTree() 
{
    
}

ErrorId CXbrlDataTree::get_Statements(IROSPList<IXbrlDataNode>** ppOut)
{
    _pStatementList.CopyTo(ppOut);
    return S_Success;
}

RP<se::securities::ISecurityDescription> CXbrlDataTree::get_SecurityDescription() 
{
    return _pSecDesc._ptr;
}
        
ErrorId CXbrlDataTree::Init(ISecurityDescription* pSecDesc, DATAPERIOD& period, XFT formType, XDC xbrltype)
{
    _pSecDesc = pSecDesc;
    _period = period;
    _formType = formType;
    
    CSmartConnection conn(SEDBCONN);
    {
        Query q = conn->query();
        q<<"select cik from se_edgar_productid_cik  where product_id ="<<pSecDesc->get_Id();
        auto qRes = q.use();
        auto row = qRes.fetch_row();
        if(row)
        {
            _cik = (string)row[0];
        }
        else
            return E_NotFound;
    }
    
    UID rec_id ;
    CStatementUtilities util(get_Session());
    RETURNIFFAILED(util.GetRecord(conn, _cik.c_str(), period, formType, &rec_id));
    
    {
        Query query1=conn->query(); 
        
        if (_xbrltype == XDC_Calculation)
        {
            query1 << "SELECT distinct y.* \
                from se_fin_statements y inner join se_fin_calculation x \
                on x.statement_id = y.statement_id \
                where y.rec_id = " << rec_id << ";";
        }
        else
        {
            query1 << "SELECT distinct y.* \
                from se_fin_statements y inner join se_fin_presentation x \
                on x.statement_id = y.statement_id \
                where y.rec_id = " << rec_id << ";";
        }
        
        if (!_pStatementList)
            RETFAILED(CObject< CROSPList< IXbrlDataNode > >::Create(get_Session(), &_pStatementList));
    
        if (mysqlpp::StoreQueryResult res = query1.store())
        {
            for (auto it = res.begin(); it != res.end(); ++it)
            {
              auto row = *it;
              SP<IXbrlDataNode> pStmt;
              auto ret = MakeTree(row, rec_id, conn, &pStmt);
              if(ret != S_Success)
              {
                  //LOG4CXX_INFOINFO(_logger, "Error in Make Tree Function");
                  return E_NotFound;
              }
              _pStatementList->Add(pStmt);
            }
        }
    }

    return S_Success;
}

#include "../semysql/CMyqlUtilities.h"
#include "CStatementUtilities.h"
#include "CXbrlUtilities.h"


ErrorId CXbrlDataTree::Init(UID statementId, XDC xbrltype)
{
    // store CIK;
    //LOG4CXX_INFOINFO(_logger, "Init method called with statement id " << statementId);
    CSmartConnection conn(SEDBCONN);

    {
        Query q1 = conn->query();
        q1 << "select cik, form_type_id, filedate from se_edgar_formrecs a inner join se_fin_statements b \
                on a.rec_id = b.rec_id  \
                where b.statement_id = " << statementId << ";";

        auto result = q1.use();
        if (!result) return E_NotFound;
        auto row = result.fetch_row();
        if (!row) return E_NotFound;
        _cik = (string)row[0];
        _formType = (XFT)(I32)row[1];
        
        sql_date dt = row[2];

        CDate filingDate;
        CMyqlUtilities::PopulateDateTime(dt, filingDate);
        CStatementUtilities utils(get_Session());
        _period = utils.DecipherDataPeriod(filingDate, _formType);
        
    }
    

    Query q = conn->query();
    q << "select * from se_fin_statements where statement_id = " << statementId;
    auto queryResult = q.store();
    if (!queryResult || !queryResult.size())
        return E_NotFound;
    auto row = queryResult.front();

    if (!_pStatementList)
        RETFAILED(CObject< CROSPList< IXbrlDataNode > >::Create(get_Session(), &_pStatementList));
    SP<IXbrlDataNode> pStmt;
    RETURNIFFAILED(MakeTree(row, (UID)row["rec_id"], conn, &pStmt));
    _pStatementList->Add(pStmt);
    return S_Success;
}

ErrorId CXbrlDataTree::Init(CIK cik, DATAPERIOD& period, XFT formType, XDC xbrltype)
{
    _xbrltype = xbrltype;
    _cik = cik;
    _period = period;
    _formType = formType;
    CSmartConnection conn(SEDBCONN);
    UID rec_id ;
    CStatementUtilities util(get_Session());
    RETURNIFFAILED(util.GetRecord(conn, _cik.c_str(), period, formType, &rec_id));

    {
//        Query query1=conn->query(); 
//        query1 << "SELECT distinct se_fin_calculation.statement_id  \
//                 from se_fin_calculation inner join se_fin_statements on se_fin_calculation.statement_id = se_fin_statements.statement_id \
//                 where se_fin_statements.rec_id = " << rec_id ;
        
        Query query1 = conn->query();
        
        if (_xbrltype == XDC_Calculation)
        {
            query1 << "select * from se_fin_statements a, \
                (SELECT distinct se_fin_calculation.statement_id \
                from se_fin_statements inner join se_fin_calculation \
                on se_fin_statements.statement_id = se_fin_calculation.statement_id \
                where se_fin_statements.rec_id = " << rec_id << 
                ") b where a.statement_id = b.statement_id;";        
        }
        else
        {
            query1 << "select * from se_fin_statements a, \
                (SELECT distinct se_fin_presentation.statement_id \
                from se_fin_statements inner join se_fin_presentation \
                on se_fin_statements.statement_id = se_fin_presentation.statement_id \
                where se_fin_statements.rec_id = " << rec_id << 
                ") b where a.statement_id = b.statement_id;";        
        }
        if (!_pStatementList)
            RETFAILED(CObject< CROSPList< IXbrlDataNode > >::Create(get_Session(), &_pStatementList));
        if (mysqlpp::StoreQueryResult res = query1.store())
        {
            for (auto it = res.begin(); it != res.end(); ++it)
            {
              auto row = *it;
              SP<IXbrlDataNode> pStmt;
              auto ret = MakeTree(row, rec_id, conn, &pStmt);
              if(ret != S_Success)
              {
                  //LOG4CXX_INFOINFO(_logger, "Error in Make Tree Function");
                  return E_NotFound;
              }
              else
                  _pStatementList->Add(pStmt);
            }
        }
    }
    return S_Success; 
}

void CXbrlDataTree::Serialize(ISerializedData<IString>* pIn)
{
    if (!pIn)
    {
       // LOGWP("pIn is null, can't serialize.");
        return;
    }
 
   // LOGIP("Starting to serialize xbrl data tree.");
    
    
    
    pIn->Add("Cik", _cik.c_str());
    
    //pIn->Add("_pSec");
    
    pIn->Add("PeriodQuarter", _period.Quarter);
    
    pIn->Add("PeriodYear", _period.Year);
    
    if (_formType == XFT_10K)
        pIn->Add("FormType", "10K");
    else if (_formType == XFT_10Q)
        pIn->Add("FormType", "10Q");
    else
        pIn->Add("FormType", "Not Set");
    
    if (_pStatementList)
        pIn->Add("StatementList", _pStatementList);

   // LOGIP("Completed serializing xbrl data tree.");

}



ErrorId CXbrlDataTree::MakeTree(Row const& statementTableRow, I64 rec_id, CSmartConnection& conn, IXbrlDataNode** pStatement)
{
    
    I64 statement_id = (I64)statementTableRow["statement_id"];

    //LOG4CXX_INFOINFO(_logger, "take xbrl_id from a statement  "<<statement_id<<" recid "<<rec_id);
    map<I64,I32> forwardIndex;

    XbrlLookupDataSet xbrlLookupSet;
    XbrlLabelsDataSet xbrlLabelSet;
    XbrlInstanceDataSet xbrlInstanceSet;
    
    {
        //Query query=conn->query(); 
//        query << "SELECT distinct xbrl_id as xbrlid from se_fin_calculation \
//            where statement_id = " << statement_id << " union SELECT distinct xbrl_parent_id  \
//            as xbrlid from se_fin_calculation where statement_id = " << statement_id ; 

//        // modified this sql to  prevent duplicates after union.
//        query << "select distinct xbrlid from ( SELECT distinct xbrl_id as xbrlid from se_fin_calculation \
//                where statement_id = " << statement_id << " union SELECT distinct xbrl_parent_id \
//                as xbrlid from se_fin_calculation where statement_id = " << statement_id << " ) a ;";
        
        Query querylookup=conn->query(); 
        if (_xbrltype == XDC_Calculation)
        {
            querylookup << "select x.xbrl_id, x.xbrl_prefix, x.xbrl_name, \
                x.xbrl_type, x.xbrl_balance, x.xbrl_periodType, x.xbrl_href, x.xbrl_desc \
                from se_xbrl_lookup x  \
                INNER JOIN (select distinct xbrlid from ( SELECT distinct xbrl_id as xbrlid from se_fin_calculation  \
                where statement_id = " << statement_id 
                << " union SELECT distinct xbrl_parent_id  \
                as xbrlid from se_fin_calculation where statement_id = " << statement_id 
                << " ) a) z ON x.xbrl_id = z.xbrlid;";
        }
        else
        {
            querylookup << "select x.xbrl_id, x.xbrl_prefix, x.xbrl_name, \
                x.xbrl_type, x.xbrl_balance, x.xbrl_periodType, x.xbrl_href, x.xbrl_desc \
                from se_xbrl_lookup x  \
                INNER JOIN (select distinct xbrlid from ( SELECT distinct xbrl_id as xbrlid from se_fin_presentation  \
                where statement_id = " << statement_id 
                << " union SELECT distinct xbrl_parent_id  \
                as xbrlid from se_fin_presentation where statement_id = " << statement_id 
                << " ) a) z ON x.xbrl_id = z.xbrlid;";
        }
        Query querylabels = conn->query();
        if (_xbrltype == XDC_Calculation)
        {
            querylabels << "select x.xbrl_id, x.label as xbrl_label from se_fin_labels x \
                INNER JOIN (select distinct xbrlid from ( SELECT distinct xbrl_id as xbrlid from se_fin_calculation \
                where statement_id = " << statement_id
                << " union SELECT distinct xbrl_parent_id \
                as xbrlid from se_fin_calculation where statement_id = " << statement_id 
                << " ) a) y ON x.xbrl_id = y.xbrlid;";
        }
        else
        {
            querylabels << "select x.xbrl_id, x.label as xbrl_label from se_fin_labels x \
                INNER JOIN (select distinct xbrlid from ( SELECT distinct xbrl_id as xbrlid from se_fin_presentation \
                where statement_id = " << statement_id
                << " union SELECT distinct xbrl_parent_id \
                as xbrlid from se_fin_presentation where statement_id = " << statement_id 
                << " ) a) y ON x.xbrl_id = y.xbrlid;";
        }
        string instanceTableName = CXbrlUtilities::GetInstanceTableName(conn, CXbrlUtilities::GetCikFromRecordId(rec_id));
        
        Query queryinstance = conn->query();
        queryinstance << "select xbrl_id, monetaryvalue, stringvalue, numericvalue, startdate, enddate, contexttype "
                << "from " << instanceTableName << " where rec_id = ";
        queryinstance << rec_id ;
        queryinstance << " order by xbrl_id asc, startdate desc;";
        
       // LOGIP(queryinstance.str());
        
        querylookup.storein_set(xbrlLookupSet);
        if (!xbrlLookupSet.size())
        {
           // LOGEP("Error when populating xbrl lookup. Cannot continue.");
            return E_DBQueryException;
        }
        
        I32 i=0;
        for (auto it = xbrlLookupSet.begin(); it != xbrlLookupSet.end(); ++it)
        {
            const DataXbrlLookup& row = *it;
            I64 xbrl_id = (I64)row.xbrl_id;
            forwardIndex.insert( make_pair(xbrl_id, i) );
            i++;
        }

        querylabels.storein_set(xbrlLabelSet);
        if (!xbrlLabelSet.size())
        {
           // LOGEP("Error when populating xbrl labels. Cannot continue.");
            return E_DBQueryException;
        }
        
        queryinstance.storein_set(xbrlInstanceSet);
        if (!xbrlInstanceSet.size())
        {
           // LOGE(_logger, "Error when populating xbrl instance for rec_id " << rec_id << ". Cannot continue.");
            return E_DBQueryException;
        }
    }
    
    Graph *pCalcGraph = new Graph(forwardIndex.size());
    Query queryCalc = conn->query(); 
    
    
    if (_xbrltype == XDC_Calculation)
    {
        queryCalc << "select xbrl_id, xbrl_parent_id, weight, statement_id from se_fin_calculation where statement_id = " << statement_id;
    }
    else
    {
        queryCalc << "select xbrl_id, xbrl_parent_id, statement_id from se_fin_presentation where statement_id = " << statement_id;
    }
    
    mysqlpp::StoreQueryResult calcResult = queryCalc.store();
    for (auto it = calcResult.begin(); it != calcResult.end(); ++it)
    {
      auto row = *it;
      I32 Parent_id = forwardIndex[row["xbrl_parent_id"]];
      I32 Child_id = forwardIndex[row["xbrl_id"]];
      add_edge(Parent_id, Child_id, *pCalcGraph);
    }
    
    typedef property_map<Graph, NameVertexPropertyTag>::type VertexXbrlMap ;
    VertexXbrlMap vertexXbrlMap = get(NameVertexPropertyTag(), *pCalcGraph);
    for (auto pair : forwardIndex)
    {
        vertexXbrlMap[pair.second] = pair.first;
    }


    typedef property_traits<VertexXbrlMap>::value_type NameType;
    
    typedef graph_traits<Graph>::vertex_iterator VertexIter;
    typedef graph_traits<Graph>::adjacency_iterator AdjIter;
    std::pair<VertexIter, VertexIter> vp;
    AdjIter ai, aiend;
    
    typedef graph_traits<Graph>::vertex_descriptor VertexDesc;
    typedef list<VertexDesc> TopoOrderedList;
    TopoOrderedList topoOrder;
    topological_sort(*pCalcGraph, front_inserter(topoOrder));
    string tabs,tabs1;

    SP<CXbrlDataNode> pStatementNode;
    //CXbrlDataNode* pStatementNode;
    CObject<CXbrlDataNode>::Create(get_Session(), &pStatementNode);
    pStatementNode.CopyTo(pStatement);
    
    pStatementNode->set_Id((UID)statement_id);
    pStatementNode->set_Name((CSTR)statementTableRow["title"]);
   // LOGI(_logger, "Statement: " << pStatementNode->get_Name());
    
    typedef map<I64, CXbrlDataNode*> XbrlNodeMap;
    XbrlNodeMap nodes;
    
    
    for (TopoOrderedList::iterator i = topoOrder.begin(); i != topoOrder.end(); ++i)
    {
        auto item = *i;
        NameType xbrlid = get(vertexXbrlMap, item);
        auto degree = in_degree(item, *pCalcGraph);
        auto findnodeit = nodes.find(xbrlid);
        CXbrlDataNode* pNode ;
        if (findnodeit == nodes.end())
        {
            CObject<CXbrlDataNode>::Create(get_Session(), &pNode); 
            pNode->set_Id((UID)xbrlid);
            SetNodeDetails(rec_id, pNode, conn, statementTableRow, xbrlLookupSet, xbrlLabelSet, xbrlInstanceSet);
            nodes.insert(make_pair((I64)xbrlid, pNode));
        }
        else
        {
            pNode = findnodeit->second;
        }
        if(degree == 0)
            pStatementNode->AppendChildren(pNode);
        
        for (tie (ai, aiend) =adjacent_vertices(item, *pCalcGraph); ai != aiend; ++ai)
        {
            auto xbrlchildid =get(vertexXbrlMap, *ai) ; 
            auto findit = nodes.find(xbrlchildid);
            CXbrlDataNode* pChildNode;
            if (findit == nodes.end())
            {
                CObject<CXbrlDataNode>::Create(get_Session(), &pChildNode); 
                pChildNode->set_Id((UID)xbrlchildid);
                SetNodeDetails(rec_id, pChildNode, conn, statementTableRow, xbrlLookupSet, xbrlLabelSet, xbrlInstanceSet);
                nodes.insert(make_pair((I64)xbrlchildid, pChildNode));
            }
            else
            {
                pChildNode = findit->second;
            }
            assert(pNode);
            auto itfind = find_if(
                    calcResult.begin(), 
                    calcResult.end(),
                    [&pNode, &pChildNode, statement_id](const Row& row)
                    {
                        I64 xbrlid = (I64)row["xbrl_id"];
                        I64 xbrlpid = (I64)row["xbrl_parent_id"];
                        I64 statementid = (I64)row["statement_id"];
                        if (xbrlid == pChildNode->get_Id() && xbrlpid == pNode->get_Id() && statementid == statement_id)
                            return true;
                        return false;
                    }
                    );

            if (XDC_Calculation == _xbrltype && itfind != calcResult.end())
            {
                pChildNode->set_Weight((I32)(*itfind)["weight"]);
            }
            else
            {
                pChildNode->set_Weight(0);
            }

            pNode->AppendChildren(pChildNode);
        }
    }
    //LOG4CXX_INFOINFO(_logger, "End Process");
    return S_Success; 
}

bool CXbrlDataTree::ParseContextRef(const string& ctxtref, CXbrlDataNode*& pNode)
{
    auto doc = xmlReadMemory(ctxtref.c_str(), ctxtref.size(), "noname.xml", NULL, XML_PARSE_NOWARNING | XML_PARSE_NOERROR|XML_PARSE_NONET);
    if (doc == NULL) 
    {
       // LOGEP("Failed to parse context ref xml data.");
        return false;
    } 

    // enumerate children
    /*
    <xbrli:context id="D2012Q1">
        <xbrli:entity>
          <xbrli:identifier scheme="http://www.sec.gov/CIK">0000018230</xbrli:identifier>
        </xbrli:entity>
        <xbrli:period>
          <xbrli:startDate>2012-01-01</xbrli:startDate>
          <xbrli:endDate>2012-03-31</xbrli:endDate>
        </xbrli:period>
      </xbrli:context>                     
     * OR
    <xbrli:context id="I2013Q1">
        <xbrli:entity>
          <xbrli:identifier scheme="http://www.sec.gov/CIK">0000005272</xbrli:identifier>
        </xbrli:entity>
        <xbrli:period>
          <xbrli:instant>2013-03-31</xbrli:instant>
        </xbrli:period>
      </xbrli:context>
     *                      
     */

    // parse children
    using namespace boost::gregorian;
    auto root = xmlDocGetRootElement(doc);
    xmlNode *curnode = root->children;
    xmlChar *value = NULL;
    for (curnode = root->children; curnode; curnode = curnode->next) 
    {
        if (curnode->type == XML_ELEMENT_NODE) 
        {
            // entity with segment, then we don't need for calculation. it is used for presentation only.
            if (!xmlStrcmp(xmlStrstr(curnode->name, (xmlChar*)"entity"), (xmlChar*)"entity") )
            {
                for(xmlNode* nextlevel = curnode->children; nextlevel; nextlevel = nextlevel->next)
                {
                    if (nextlevel->type == XML_ELEMENT_NODE) 
                    {
                        //LOGI(_logger, "Entity child: " << (CSTR)nextlevel->name);
                        if (!xmlStrcmp( xmlStrstr( nextlevel->name, (xmlChar*)"segment"), (xmlChar*)"segment" ) )
                        {
                            goto FAILEDPARSE;
                        }
                    }
                }            
            }
            
            // check if it is period
            if (!xmlStrcmp(xmlStrstr(curnode->name, (xmlChar*)"period"), (xmlChar*)"period") )
            {
                // dig to see if it is instance or duration child
                for(xmlNode* nextlevel = curnode->children; nextlevel; nextlevel = nextlevel->next)
                {
                    if (nextlevel->type == XML_ELEMENT_NODE) 
                    {
                        //LOGI(_logger, "Period child: " << (CSTR)nextlevel->name);
                        if (!xmlStrcmp( xmlStrstr( nextlevel->name, (xmlChar*)"instant"), (xmlChar*)"instant" ) )
                        {
                            if (pNode->get_PeriodType() != XPT_Instant)
                            {
                               // LOGWP("Period type is not instant as understood from xbrl_lookup table. Enforcing instant.");
                                pNode->set_PeriodType(XPT_Instant);
                            }
                            value = xmlNodeGetContent(nextlevel);
                            CSTR czvalue = (CSTR)value;
                            date dt(from_string(czvalue));
                            xmlFreeFunc(value);
                            if (_formType == XFT_10K && dt.year() == _period.Year+1)
                            {
                                _yearFlag = 0;
                                _selectYear = dt.year();
                                goto SUCCESSPARSE;
                            }   
                            else if(_formType == XFT_10K && dt.year() == _period.Year)
                            {
                                _yearFlag = 1;
                                _prevYear = dt.year();
                                goto SUCCESSPARSE;
                            }        
                            else if (_formType == XFT_10Q && dt.year() == _period.Year)
                            {
                                // make sure quarter is right
                                CDate dtQ(dt.year(), dt.month(),dt.day());
                                CQuarter q(dtQ);
//                                if ( q.get_Quarter() >= _period.Quarter)
//                                {
//                                    goto SUCCESSPARSE;
//                                }
                                auto lret = (I32)q.get_Quarter();
                                auto rret = (I32)_period.Quarter;
                                if (lret > rret)
                                {
                                    _qtrFlag =0;
                                    _selectQtr = lret;
                                    goto SUCCESSPARSE;
                                }
                                else if (lret == rret)
                                {
                                    _qtrFlag =1;
                                    _prvQtr = lret;
                                    goto SUCCESSPARSE;
                                }
                            }
                            break;
                        }
                        /*
                        else if (!xmlStrcmp(xmlStrstr(nextlevel->name, (xmlChar*)"startDate"), (xmlChar*)"startDate") )
                        {
                            if (pNode->get_PeriodType() != XPT_Duration)
                            {
                               // LOGWP("Period type is not duration (Start and end dates) as understood from xbrl_lookup table. Enforcing duration.");
                                pNode->set_PeriodType(XPT_Duration);
                            }
                            value = xmlNodeGetContent(nextlevel);
                            CSTR czvalue = (CSTR)value; 
                            date dt(from_string(czvalue));      // the date is in ISO format.


                            xmlFreeFunc(value);
                            continue;
                        }
                         * */
                        else if (!xmlStrcmp(xmlStrstr(nextlevel->name, (xmlChar*)"endDate"), (xmlChar*)"endDate") )
                        {
                            value = xmlNodeGetContent(nextlevel);
                            CSTR czvalue = (CSTR)value;
                            date dt(from_string(czvalue));
                            xmlFreeFunc(value);
                            if (_formType == XFT_10K && dt.year() == _period.Year+1)
                            {
                                _yearFlag = 0;
                                _selectYear = dt.year();
                                goto SUCCESSPARSE;
                            }   
                            else if(_formType == XFT_10K && dt.year() == _period.Year)
                            {
                                _yearFlag = 1;
                                _prevYear = dt.year();
                                goto SUCCESSPARSE;
                            }        
                            else if (_formType == XFT_10Q && dt.year() == _period.Year)
                            {
                                // make sure quarter is right
                                CDate dtQ(dt.year(), dt.month(),dt.day());
                                CQuarter q(dtQ);
                                auto lret = (I32)q.get_Quarter();
                                auto rret = (I32)_period.Quarter;
//                                if (lret >= rret)
//                                    goto SUCCESSPARSE;
                                if (lret > rret)
                                {
                                    _qtrFlag =0;
                                    _selectQtr = lret;
                                    goto SUCCESSPARSE;
                                }
                                else if (lret == rret)
                                {
                                    _qtrFlag =1;
                                    _prvQtr = lret;
                                    goto SUCCESSPARSE;
                                }
                                    
                            }
                            break;
                        }
                    }
                }
            }
        }
    }

FAILEDPARSE:    
    xmlFreeDoc(doc);                    
    return false;
    

SUCCESSPARSE:
    xmlFreeDoc(doc);                    
    return true;


}

void CXbrlDataTree::SetNodeDetails(
        I64 , 
        CXbrlDataNode* pNode, 
        CSmartConnection&,
        Row const& ,
        XbrlLookupDataSet const& lookupset,
        XbrlLabelsDataSet const& labelset,
        XbrlInstanceDataSet const& instanceset)
{
    
    DataXbrlLabels lbl(pNode->get_Id());
    XbrlLabelsDataSet::const_iterator itLabel = labelset.find(lbl);
            
    if (itLabel != labelset.end())
        pNode->set_label( (*itLabel).xbrl_label.c_str());

    DataXbrlLookup lkp(pNode->get_Id());
    XbrlLookupDataSet::const_iterator itLookupFind;
    itLookupFind = lookupset.find(lkp);
            
    if (itLookupFind == lookupset.end())        
    {
       // LOGEP("Xbrl item not found Xbrl lookup table");
        return;
    }
            
    const DataXbrlLookup& row = *itLookupFind;

    XDT type;
    string period_Type = row.xbrl_PeriodType;
    if(period_Type == "instant")
        pNode->set_PeriodType(XPT_Instant);
    else if(period_Type == "duration")
        pNode->set_PeriodType(XPT_Duration);

    
    pNode->set_Name((row.xbrl_prefix + ":" + row.xbrl_name).c_str());

    string szType = row.xbrl_type;
    if (szType.find("decimal") != string::npos ||
            szType.find("nonZeroDecimal") != string::npos ||
            szType.find("monetaryItemType") != string::npos)
    {
        pNode->set_Type(XDT_Monetary);
    }
    else if (szType.find("sharesItemType") != string::npos)
    {
        pNode->set_Type(XDT_Numeric);
    }
    else
    {
        pNode->set_Type(XDT_Text);
    }
    R8 selctMontary;
    I64 selctNumeric;
    string selctText;
    _selectYear = 0;
    _prevYear = 0;
    _selectQtr = 0;
    _prvQtr = 0;
    
    DataXbrlInstance inst(pNode->get_Id());
    pair<XbrlInstanceDataSet::const_iterator, XbrlInstanceDataSet::const_iterator> equalRange
            = instanceset.equal_range(inst);
    
    type = pNode->get_Type();
    for (auto it = equalRange.first; it != equalRange.second; ++it)
    {
            
        const DataXbrlInstance& row = *it;
        sql_date_null verifydate;
        if(pNode->get_PeriodType() == XPT_Instant && row.contextref == 0) // instance
            verifydate = row.startdate;
        else if(pNode->get_PeriodType() == XPT_Duration && row.contextref == 1)
            verifydate = row.enddate;
        if (!verifydate.is_null)
        {
            if (_formType == XFT_10K)
            {
                if (_period.Year == verifydate.data.year())
                {
                    switch (type)
                    {
                        case XDT_Monetary:
                            selctMontary = row.monetaryvalue;
                            break;
                        case XDT_Numeric:
                            selctNumeric = row.numericvalue;
                            break;
                        default:
                            selctText = row.stringvalue;
                            break;
                    }
                }
            }
            else
            {
                CDate dtQ(verifydate.data.year(), verifydate.data.month(), verifydate.data.day());
                CQuarter q(dtQ);        
                CDate& dt = q.get_EndDate();
                if (q.get_Quarter() == _period.Quarter && dt.get_Year() == _period.Year)
                {
                    switch (type)
                    {
                        case XDT_Monetary:
                            selctMontary = row.monetaryvalue;
                            break;
                        case XDT_Numeric:
                            selctNumeric = row.numericvalue;
                            break;
                        default:
                            selctText = row.stringvalue;
                            break;
                    }
                    break;
                }
            }

        }
    }
    switch (type)
    {
        case XDT_Monetary:
            pNode->set_Monetary(selctMontary);
            break;
        case XDT_Numeric:
            pNode->set_Numeric(selctNumeric);
            break;
        default:
            pNode->set_Text(selctText.c_str());
            break;
    }
    
}
