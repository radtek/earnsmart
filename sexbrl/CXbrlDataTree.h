/* 
 * File:   CXbrlDataTree.h
 * Author: aashok
 *
 * Created on July 9, 2013, 8:43 PM
 */

#ifndef CXBRLDATATREE_H
#define	CXBRLDATATREE_H

#include "sexbrlapi.h"
#include "sexbrlinternal.h"
#include "CXbrlDataNode.h"

#include <boost/graph/directed_graph.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/properties.hpp>
////#include <log4cxx/logger.h>

#include <unordered_set>
#include <set>

#include "../semysql/CSEConnections.h"



using namespace se;
using namespace se::xbrl;
using namespace se::securities;
using namespace std;
using namespace se::data;
using namespace boost;	

struct DataXbrlInstance;
struct DataXbrlLabels;
struct DataXbrlLookup;

typedef struct 
{
    bool operator()(const Row& rowa, const Row& rowb)
    {
        I64 xbrlida = (I64)rowa["xbrl_id"];
        I64 xbrlidb = (I64)rowb["xbrl_id"];
        return xbrlida - xbrlidb;
    }

} COMPAREXBRLIDS;

template<typename T>
struct CompareDataXbrls
{
    T t;
    CompareDataXbrls(){}
    CompareDataXbrls(const T& c)
    {
        t = c;
    }
    
    bool operator()(const T& rowa, const T& rowb) const
    {
        return rowa.xbrl_id < rowb.xbrl_id;
    }

    bool operator()(const T& other) const
    {
        return t.xbrl_id == other.xbrl_id;
    }
} ;


typedef std::unordered_set<DataXbrlLookup, CompareDataXbrls<DataXbrlLookup> > XbrlLookupDataSet;
typedef std::unordered_set<DataXbrlLabels, CompareDataXbrls<DataXbrlLabels> > XbrlLabelsDataSet;
typedef std::unordered_multiset<DataXbrlInstance, CompareDataXbrls<DataXbrlInstance> > XbrlInstanceDataSet;

class CXbrlDataTree : public CObjectRoot , public IXbrlDataTree
{
private:
    
    
    
public:
    CXbrlDataTree(ISession*);
    CXbrlDataTree(const CXbrlDataTree& orig)=delete;
    virtual ~CXbrlDataTree();
    
    virtual ErrorId get_Statements(IROSPList<IXbrlDataNode>** ppOut);
    
    virtual RP<se::securities::ISecurityDescription> get_SecurityDescription();
      
    virtual ErrorId Init(ISecurityDescription* pSecDesc, DATAPERIOD& period, XFT formType, XDC);
    
    virtual ErrorId Init(CIK cik, DATAPERIOD& period, XFT formType, XDC);
    
    virtual ErrorId Init(UID statementId, XDC xbrltype);
    
    virtual void Serialize(ISerializedData<IString>* pIn);
       
private:
    
    ErrorId MakeTree(Row const& row, I64  rec_id, CSmartConnection& conn, IXbrlDataNode** ppStmt);

    void SetNodeDetails(
        I64 recid, 
        CXbrlDataNode* pNode, 
        CSmartConnection& conn,
        Row const& statementRow,
        XbrlLookupDataSet const& lookupset,
        XbrlLabelsDataSet const& labelset,
        XbrlInstanceDataSet const& instanceset);
    
    bool ParseContextRef(const string& contextRef, CXbrlDataNode*& pNode);
    
    struct OrderEdgePropertyTag
    {
        typedef edge_property_tag kind;
    };

    struct NameVertexPropertyTag
    {
        typedef vertex_property_tag kind;
    };

    typedef property<OrderEdgePropertyTag, int> EdgeOrderProperty;
    typedef property<NameVertexPropertyTag, int> XbrlIdProperty;
    typedef adjacency_list<vecS, vecS, bidirectionalS, XbrlIdProperty, EdgeOrderProperty> Graph;
    

private:
    ////LoggerPtr _logger;
    SP<CXbrlDataNode> _pStatementNode;
    string _cik;
    SP<ISecurityDescription> _pSecDesc;
    DATAPERIOD _period;
    XFT _formType;
    SP<CROSPList<IXbrlDataNode>> _pStatementList;
    I64 _selectYear,_prevYear;
    I32 _yearFlag,_isCurrent = 0,_selectQtr,_prvQtr,_qtrFlag;
    XDC _xbrltype;

};

#endif	/* CXBRLDATATREE_H */

