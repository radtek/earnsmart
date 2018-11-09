/* 
 * File:   CXbrlStatement.h
 * Author: aashok
 *
 * Created on September 19, 2013, 2:43 PM
 */

#ifndef CXBRLSTATEMENT_H
#define	CXBRLSTATEMENT_H

#include <string>

#include "../seglobaltypedefs.h"
#include "sexbrlapi.h"
#include "../seglobal/CROList.h"
#include "CFinancialItem.h"
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/breadth_first_search.hpp>

using namespace se;
using namespace se::xbrl;
using namespace boost;
using namespace boost::graph;
using namespace se::data;

struct EdgePropertyTag
{
    typedef edge_property_tag kind;
};

struct NameVertexPropertyTag
{
    typedef vertex_property_tag kind;
};

//struct XbrlEdgeProperty
//{
//    R8 Order;
//    R8 Priority;
//    R8 Depth;
//};

typedef property<EdgePropertyTag, I64> EdgeProperty;
typedef property<NameVertexPropertyTag, I64> FinItemNodeProperty;
typedef adjacency_list<listS, vecS, bidirectionalS, FinItemNodeProperty, EdgeProperty> Graph;
//typedef directed_graph<
//    IXbrlAbstractRecord*,
//    XbrlEdgeProperty
//    > DGraph;

//typedef adjacency_list<listS, vecS, bidirectionalS, FinItemNodeProperty, EdgeProperty> Graph;

    /*
    adjacency_list
     * <
     * class OutEdgeListS=vecS,
     * class VertexListS=vecS,
     * class DirectedS=directedS,
     * class VertexProperty=no_property,
     * class EdgeProperty=no_property,
     * class GraphProperty=no_property,
     * class EdgeListS=listS>
     * 
     * class bfs_time_visitor : public default_bfs_visitor 
        {
        public:
          bfs_time_visitor(CFinancialStatementPresentation& outer):_outer(outer)
          {
          }

          template < typename V, typename G >
          void discover_vertex(V u, const G & g) const
          {
              //cout << u << endl;
          }
        private:
            CFinancialStatementPresentation& _outer;
        };    

        friend class bfs_time_visitor;

    */


class CXbrlStatement : public CObjectRoot, public IXbrlStatement
{
public:
    CXbrlStatement(ISession*);
    CXbrlStatement(const CXbrlStatement& orig)=delete;
    virtual ~CXbrlStatement();
    virtual long unsigned int Init(){}
    SE_CODE Init(CIK cik, UID statementId, CSTR title);
    SE_CODE Init(CIK cik, IFinancialsConsolidated* pconsolidated, UID statementId, CSTR title);
    virtual CSTR get_Name();
    virtual void set_Name(CSTR value);
    virtual void Serialize(ISerializedData<IString>* pIn);
    virtual long unsigned int get_Tree(IXbrlAbstractRecord** ppOut);
    virtual short get_HighestYear();
    virtual short get_LowerYear();


private:
    typedef Graph::vertex_descriptor Vd;
    typedef property_map<Graph, vertex_index_t>::type IndexMap;
    typedef std::map<UID, SP<CXbrlAbstractRecord> > XbrlRecords;
    typedef std::vector<UID> Index;

private:
    SE_CODE BuildStatementOnDemand(IFinancialsConsolidated* = nullptr);
    void WalkGraph(const Vd& vdes, const property_map<Graph, vertex_index_t>::type& gmap, const Graph& g, const XbrlRecords& xrecs, const Index& xindex, IXbrlAbstractRecord** ppRec);
//    void WalkDGraph(
//        DGraph::vertex_descriptor vdes, 
//        const DGraph& g
//        );
    
private:
    string _cik;
    string _StatementName;
    ////LoggerPtr _logger;
    SP<CXbrlRecordList> _root;
    SP<IFinancialsConsolidated> _pcon;
    I16 _floorYear, _ceilYear;
};

#endif	/* CXBRLSTATEMENT_H */

