/* 
 * File:   CXbrlStatement.cpp
 * Author: aashok
 * 
 * Created on September 19, 2013, 2:43 PM
 */
#include "sexbrlinternal.h"
#include "CXbrlStatement.h"
#include "../seglobal/exceptions.h"
#include "../semain/semain.h"
#include "CFinancialItem.h"
#include "CXbrlStatementList.h"
#include <Poco/Mutex.h>
#include <boost/graph/topological_sort.hpp>

using namespace se::data;
using namespace Poco;

#define SEDBCONN        CSEConnections::GetMain()


CXbrlStatement::CXbrlStatement(ISession* ps) : CObjectRoot(ps) ,
        _ceilYear(0), _floorYear(2999)
{
    
}


CXbrlStatement::~CXbrlStatement() 
{
        _root.Clear();
        _pcon.Clear();
}

long unsigned int CXbrlStatement::Init(CIK cik, IFinancialsConsolidated* pconsolidated, unsigned long statementId, const char* title) 
{
    _cik = cik;
    _StatementName = title;
    set_Id(statementId);
    return BuildStatementOnDemand(pconsolidated);
}

long unsigned int CXbrlStatement::Init(CIK cik, unsigned long statementId, CSTR title) 
{
    _cik = cik;
    _StatementName = title;
    try
    {
        set_Id(statementId);
        if (!_root)
        {
            BuildStatementOnDemand();
        }
        return S_OK;
    }
    CATCHCLAUSE("Init")  ;
}

long unsigned int CXbrlStatement::BuildStatementOnDemand(IFinancialsConsolidated* pcon) 
{
   // LOGIP("Begin-BuildStatementOnDemand");
    if (!_root)
    {
        SP<IXbrl> pxbrl;
        RETURNIFFAILED(XbrlOpen(get_Session(), &pxbrl));
        if (!pcon)
        {
            RETURNIFFAILED(pxbrl->GetConsolidatedFinancials(_cik.c_str(), &_pcon));
        }
        else
            _pcon = pcon;
        
        CSmartConnection conn(SEDBCONN);
       // LOGI(_logger, "Building the presentation statement tree for statement id " << get_Id());
        Query q = conn->query();
        q << "call get_PresentationTables(" << _cik << ", " << get_Id() << ");";
       // LOGIP(q.str());
        StoreQueryResult sqr = q.store();
       // LOGI(_logger, "Retrieved presentation tree for statement : " << this->get_Name());
        try
        {
            
            /* Experimental DirectedGraph*/
            /*
            DGraph dg;
            for (const Row& r : sqr)
            {
                I32 ip, ic;
                UID pxid = r["xbrl_parent_id"];
                UID xid = r["xbrl_id"];
                typename DGraph::edge_property_type edgeProp;
                edgeProp = { (R8)r["se_order"], (R8)r["priority"], (R8)r["depth"] };
                SP<IXbrlAbstractRecord> ppxarec;
                SP<IXbrlRecord> ppxrec;
                if (FAILED(_pcon->get_Record(pxid, &ppxrec)))
                {
                    RETURNIFFAILED(CObject<CXbrlAbstractRecord>::Create(pxid, r["pxname"], "", &ppxarec));
                }
                else
                    ppxarec = ppxrec;
                
                SP<IXbrlAbstractRecord> pxarec;
                SP<IXbrlRecord> pxrec;
                if (FAILED(_pcon->get_Record(xid, &pxrec)))
                {
                    RETURNIFFAILED(CObject<CXbrlAbstractRecord>::Create(xid, r["xname"], "", &pxarec));
                }
                else
                    pxarec = pxrec;
                add_edge(ppxarec, pxarec, edgeProp, dg);
            }

            if (dg.num_edges())
            {
                SP<IXbrlAbstractRecord> prec((IXbrlAbstractRecord*)*vertices(dg).first);
                WalkDGraph(prec, dg);
                _root->Add(prec);
            }
             * */
            
            //typedef std::map<R8, SP<CXbrlAbstractRecord> > OrderedRecords;
            
            typedef std::map<UID, I32> IndexMap;
            typedef std::map<UID, SP<CXbrlAbstractRecord> > XbrlRecords;
            typedef std::vector<UID> Index;
            Index index;
            IndexMap map;
            XbrlRecords xrecords;
            Graph g;
            SP<IString> plabel;
            for (const Row& r : sqr)
            {
                I32 ip, ic;
                UID pxid = r["xbrl_parent_id"];
                UID xid = r["xbrl_id"];
                
                IndexMap::const_iterator find = map.find(pxid);
                if (find != map.end())
                    ip = find->second;
                else
                {
                    ip = index.size();
                    map.insert(make_pair(pxid, ip));
                    index.push_back(pxid);
                    pxbrl->get_Label(&plabel, pxid, _cik.c_str());
                    //LOGI(_logger, "Node Name: " << r["pxname"] << " and Label : " << plabel->get_Buffer());
                    SP<CXbrlAbstractRecord> pxrec;
                    RETURNIFFAILED(CObject<CXbrlAbstractRecord>::Create(get_Session(), pxid, r["pxname"], (plabel ? plabel->get_Buffer() : "") , &pxrec));
                   // LOGI(_logger, "Node Name: " << pxrec->get_Name() << " and Label : " << pxrec->get_XbrlLabel());
                    pxrec->set_Property("priority", (R8)r["priority"]);
                    pxrec->set_Property("order", (R8)r["se_order"]);
                    pxrec->set_Property("depth", (R8)r["depth"]);
                    /*
                    sql_varchar_null preferredlabel = r["preferredlabel"];
                    if (preferredlabel.is_null)
                        pxrec->set_Property("preferredlabel", "");
                    else
                        pxrec->set_Property("preferredlabel", preferredlabel.data.c_str());
                     * */
                    xrecords.insert(make_pair(pxid, pxrec));
                }
                
                find = map.find(xid);
                if (find != map.end())
                    ic = find->second;
                else
                {
                    ic = index.size();
                    map.insert(make_pair(xid, ic));
                    index.push_back(xid);
                    pxbrl->get_Label(&plabel, xid, _cik.c_str());
                    SP<CXbrlAbstractRecord> pxrec;
                    RETURNIFFAILED(CObject<CXbrlAbstractRecord>::Create(get_Session(), xid, r["xname"], (plabel ? plabel->get_Buffer() : ""), &pxrec));
                   // LOGI(_logger, "Node Name: " << pxrec->get_Name() << " and Label : " << pxrec->get_XbrlLabel());
                    pxrec->set_Property("priority", (R8)r["priority"]);
                    pxrec->set_Property("order", (R8)r["se_order"]);
                    pxrec->set_Property("depth", (R8)r["depth"]);
                    /*
                    sql_varchar_null preferredlabel = r["preferredlabel"];
                    if (preferredlabel.is_null)
                        pxrec->set_Property("preferredlabel", "");
                    else
                        pxrec->set_Property("preferredlabel", (CSTR)preferredlabel.data.c_str());
                     * */
                    xrecords.insert(make_pair(xid, pxrec));
                }
                plabel.Clear();
                //add_edge(g, ip, ic, Weight((R8)r["se_order"]));
                add_edge(ip, ic, g);
            }
            while(q.more_results()) q.store_next();

           // LOGI(_logger, "Now building a adjacency list graph for statement " << get_Name());
            
            typename Graph::vertex_iterator vstart, vend;
            typename Graph::out_edge_iterator ostart, oend;
            typename Graph::adjacency_iterator astart, aend;
            property_map<Graph, vertex_index_t>::type indexmap = get(vertex_index, g);
            
            if (g.m_edges.size())
            {
                RETURNIFFAILED(CObject<CXbrlRecordList>::Create(get_Session(), _pcon._ptr, &_root ));
            }
            
            typedef list<Graph::vertex_descriptor> TopOrder;
            TopOrder ordered;
            topological_sort(g, std::front_inserter(ordered));
            
            if (ordered.size())
            {
                SP<IXbrlAbstractRecord> prec;
                WalkGraph(ordered.front(), indexmap, g, xrecords, index, &prec);
                _root->Add(prec);
            }
            
            /*
             * 
            //for (tie(vstart, vend) = vertices(g); vstart != vend; ++vstart)
            for (auto i : ordered)
            {
                UID xbrlId = index[get(indexmap, i)];
                auto outdigree = degree(i, g);
                //UID xbrlId = index[get(indexmap, *vstart)];
                SP<IXbrlAbstractRecord> pRec;
                pRec.Assign(xrecords.find(xbrlId)->second);
               // LOGI(_logger, pRec->get_Name());
                SP<IXbrlRecord> pFinRec;
                if (ISOK(_pcon->get_Record(xbrlId, &pFinRec)))
                {
                   // LOGI(_logger, "Year Start: " 
                            << pFinRec->get_LowerYear() << ", "
                            << "Year End: " << pFinRec->get_HighestYear());
                }
                else
                {
                    
                }
                tie(astart, aend) = adjacent_vertices(i, g);
                //tie(astart, aend) = adjacent_vertices(*vstart, g);
                OrderedRecords children;
                if (astart == aend)
                {
                   // LOGI(_logger, " has no children");
                }
                else
                {
                   // LOGI(_logger, "Children are : ");
                    // sort by order
                    for(; astart != aend; ++astart)
                    {
                        UID cxbrlId = index[get(indexmap, *astart)];
                        SP<CXbrlAbstractRecord> pRec1 = xrecords.find(cxbrlId)->second;
                        children.insert(make_pair((R8)pRec1->get_Property("order"), pRec1));
                    }
                }
                for (auto pr : children)
                {
                    SP<CXbrlAbstractRecord> pRec1 = pr.second;
                   // LOGI(_logger, "        " << pRec1->get_Name());
                }
            }
             * 
             */
            
           // LOGI(_logger, "Completed adjacency list graph building for statement " << get_Name());
        }
        catch(const std::exception& ex)
        {
            while(q.more_results()) q.store_next();
           // LOGI(_logger, "Init error: " << ex.what());
        }
    }
   // LOGIP("End-BuildStatementOnDemand");
    return S_OK;
}

//void CXbrlStatement::WalkDGraph(DGraph::vertex_descriptor vdes, const DGraph& g) 
//{
//    typename DGraph::out_edge_iterator ostart, oend;
//    SP<IXbrlAbstractRecord> p((IXbrlAbstractRecord*)vdes);
//    if (ostart != oend)
//    {
//        SP<CXbrlRecordList> pkids;
//        CObject<CXbrlRecordList>::Create(_pcon, &pkids);
//        for (; ostart != oend; ++ostart)
//        {
//            SP<IXbrlAbstractRecord> pkid( (IXbrlAbstractRecord*)target(*ostart, g));
//            WalkDGraph(pkid, g);
//            pkids->Add(pkid);
//        }
//        if ( ((IXbrlAbstractRecord*)vdes)->get_IsAbstract())
//        {
//           ((CXbrlAbstractRecord*)vdes)->set_Children(pkids);
//        }
//        else
//        {
//           ((CXbrlRecord*)vdes)->set_Children(pkids);
//        }
//    }
//}

void CXbrlStatement::WalkGraph(const Vd& vdes, const property_map<Graph, vertex_index_t>::type& gmap, const Graph& g, const XbrlRecords& xrecs, const Index& xindex, IXbrlAbstractRecord** ppRec) 
{
    typename Graph::out_edge_iterator ostart, oend;

    UID xbrlId = xindex[get(gmap, vdes)];
   // LOGIP("Looking for financial record....")
    SP<IXbrlAbstractRecord> pRec;
    SP<IXbrlRecord> pFinRec;
    if (ISOK(_pcon->get_Record(xbrlId, &pFinRec)))
    {
       // LOGIP("Found financial record....")
        _ceilYear = max(_ceilYear, pFinRec->get_HighestYear());
        _floorYear = min(_floorYear, pFinRec->get_LowerYear());
        pFinRec.CopyTo(ppRec);
    }
    else
    {
       // LOGIP("Not found financial record. Assuming it as an abstract record....");
        pRec = xrecs.find(xbrlId)->second;
        pRec.CopyTo(ppRec);
    }
    
    tie(ostart, oend) = out_edges(vdes, g);
    if (ostart != oend)
    {
        SP<CXbrlRecordList> pkids;
        CObject<CXbrlRecordList>::Create(get_Session(), _pcon._ptr, pRec ? pRec._ptr : pFinRec._ptr, &pkids);
        for (; ostart != oend; ++ostart)
        {
            Vd tg = target(*ostart, g);
            SP<IXbrlAbstractRecord> pkid;
            WalkGraph(tg, gmap, g, xrecs, xindex, &pkid);
            pkids->Add(pkid);
        }
        if (pFinRec)
        {
            ((CXbrlRecord*)pFinRec._ptr)->set_Children(pkids);
        }
        else if (pRec)
        {
            ((CXbrlAbstractRecord*)pRec._ptr)->set_Children(pkids);
        }                
    }
}


CSTR CXbrlStatement::get_Name()
{
    return _StatementName.c_str();
}
void CXbrlStatement::set_Name(CSTR value)
{
    _StatementName = value;
}

long unsigned int CXbrlStatement::get_Tree(IXbrlAbstractRecord** ppOut) 
{
    if (_root)
    {
        return _root->get_Item(0, ppOut);
    }
    return _root ? S_OK : S_False;
}


void CXbrlStatement::Serialize(ISerializedData<IString>* pIn)
{
    pIn->Add("CIK", _cik.c_str());
    pIn->Add("Statement Id", get_Id());
    pIn->Add("Name", get_Name());
    pIn->Add("DataAvailableFrom", get_LowerYear());
    pIn->Add("DataAvailableTo", get_HighestYear());
    pIn->Add("Name", get_Name());
    SP<IXbrlAbstractRecord> pRoot;
    if (ISOK(get_Tree(&pRoot)))
        pIn->Add("Root Account", pRoot);
}

short CXbrlStatement::get_HighestYear() 
{
    return _ceilYear;
}

short CXbrlStatement::get_LowerYear() 
{
    return _floorYear;
}
