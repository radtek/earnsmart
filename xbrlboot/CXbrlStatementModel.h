/* 
 * File:   CXbrlStatementModel.h
 * Author: santony
 *
 * Created on January 4, 2013, 5:27 AM
 */

#ifndef CXBRLSTATEMENTMODEL_H
#define	CXBRLSTATEMENTMODEL_H

#include <string>
#include <map>
#include <vector>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/properties.hpp>
#include "CXbrlElement.h"
#include "CXbrlStatementParser.h"
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>

using namespace std;
using namespace boost;


namespace se
{
    
    namespace xbrl
    {

        #define    XBRLELMTUSE_NONE                ""
        #define    XBRLELMTUSE_OPTIONAL            "optional"
        #define    XBRLELMTUSE_PROHIBITED          "prohibited"

        struct OrderEdgePropertyTag
        {
            typedef edge_property_tag kind;
        };

        struct WeightEdgePropertyTag
        {
            typedef edge_property_tag kind;
        };

        struct NameVertexPropertyTag
        {
            typedef vertex_property_tag kind;
        };

        typedef property<OrderEdgePropertyTag, int> EdgeOrderProperty;
        //typedef property<WeightEdgePropertyTag, int, EdgeOrderProperty> EdgeWeightProperty; // Note the nesting of edge properties
        typedef property<NameVertexPropertyTag, string> VertexNameProperty;
        typedef adjacency_list<vecS, vecS, bidirectionalS, VertexNameProperty, EdgeOrderProperty> Graph;

        typedef map<string, Graph*> Statements;

        //typedef multimap<string, CXbrlElementBase*> ElementByNames; // caches already created elements by name. Upto two records for the same can be created, one for Debit and other for Credit (if available)
        typedef set<string> ElementByNames;

        typedef map<string, string> XmlNs;
        typedef pair<string, string> StringPair;
        typedef pair<string, XmlNs> NsPair;
        
        typedef map<string, XmlNs> RegisteredNs;
        typedef map<string, xmlDocPtr> XMLRefs;
        typedef pair<string, xmlDocPtr> XMLRefItem;

        typedef pair<int, int> ElementPairsPair;
        typedef list<ElementPairsPair> ElementPairs;

        typedef map<string, int> UniqueElements;
        typedef map<int, string> UniqueElementsRev;

        #define XPathNsExpr(ns) (ns.empty() ? "" : ns + ":")
        
        
        class CXbrlStatementModel 
        {
        public:
            CXbrlStatementModel(CXbrlStatementParser* parser) ;
            virtual ~CXbrlStatementModel();

            virtual ErrorId Start() = 0;
            
        protected:
            CXbrlStatementModel(const CXbrlStatementModel& orig);
            
            XmlNs& RegisterNamespaces(xmlDocPtr const doc, xmlNodePtr const node, string const& key);
//            xmlNodePtr FindNode();
            xmlAttrPtr FindAttribute(CSTR name, xmlNodePtr node);
            void CacheSchema(string& url);
            virtual void ConstructGraph(
                Graph& graph,
                string statementName, 
                UniqueElements& examined,
                UniqueElementsRev& revExamined,
                ElementPairs& edges,
                std::function<CXbrlElementBase* (const string&, int index, int degree)> cbvertex
            );
            CXbrlElementBase* CreateXbrlElement(const string& roleName, const CXbrlElementBase * const parent, xmlNodePtr elementNode, bool& existing);
            
            // Evaluate an xpath expression. pNodeToEvalFrom can be either xmlDocPtr or xmlNodePtr.
            bool EvaluateXPathExpression(xmlDocPtr pNodeToEvalFrom, string xpathExpression, string nsdata, std::function< void (xmlNodePtr, xmlXPathContextPtr) > cbpernode);
            bool EvaluateXPathExpression(xmlXPathContextPtr pContext, string xpathExpression, std::function< void (xmlNodePtr, xmlXPathContextPtr) > cbpernode);

            string GetDefaultNamespace(xmlNodePtr node);
            
        protected:
            CXbrlStatementParser* _pParser;
            RegisteredNs xbrlNs;
            XMLRefs xbrlRefs;
            ElementByNames elements;
            Statements statements;
            log4cxx::LoggerPtr _logger;
        };
        
    }
}

#endif	/* CXBRLSTATEMENTMODEL_H */

