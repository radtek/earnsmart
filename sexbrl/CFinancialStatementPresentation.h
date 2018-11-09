/* 
 * File:   CFinancialStatementPresentation.h
 * Author: santony
 *
 * Created on January 27, 2014, 7:08 PM
 */

#ifndef CFINANCIALSCONSOLIDATED_H
#define	CFINANCIALSCONSOLIDATED_H

////#include <log4cxx/logger.h>
#include <set>
#include "CFinancialItem.h"
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include "../semysql/CSEConnections.h"
#include "CXbrlStatementList.h"


using namespace std;
//using namespace log4cxx;
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

typedef property<EdgePropertyTag, I64> EdgeProperty;
typedef property<NameVertexPropertyTag, I64> FinItemNodeProperty;
typedef adjacency_list<listS, vecS, bidirectionalS, FinItemNodeProperty, EdgeProperty> Graph;

typedef std::map<UID, string> StatementMap;
typedef std::map<UID, Graph> StatementGraph;

class CXbrlStatementList;

class CFinancialStatementPresentation : 
        public CObjectRoot, 
        public IFinancialStatementPresentation
{
public:
    CFinancialStatementPresentation();
    CFinancialStatementPresentation(const CFinancialStatementPresentation& orig);
    virtual ~CFinancialStatementPresentation();
    
    virtual void Serialize(ISerializedData<IString>* pIn);
    
    SE_CODE Init(CIK cik);
    
    virtual long unsigned int get_Statements(IXbrlStatementList**);

    virtual long unsigned int get_Statement(STATEMENTTYPES statementType, IXbrlStatement**);


    
private:
    void SubmitToDb();
    
    SE_CODE BuildStatementOnDemand(UID statementId);
    
private:
    //LoggerPtr _logger;
    SP<IFinancialsConsolidated> _pcon;
    
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
    */

    class bfs_time_visitor : public default_bfs_visitor 
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
    
    SP<CXbrlStatementList> _pstatements;
    
};

#endif	/* CFINANCIALSCONSOLIDATED_H */

