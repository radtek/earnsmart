/* 
 * File:   CXbrlStatementModel.cpp
 * Author: santony
 * 
 * Created on January 4, 2013, 5:27 AM
 */

#include "CXbrlStatementModel.h"
#include <boost/algorithm/string/find_iterator.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/filesystem.hpp>
#include <algorithm>
#include <boost/variant/get.hpp>
#include "../seglobal/CDirectory.h"
#include "../seglobal/CStringToNumber.h"
#include "../seglobal/CDirectory.h"
#include <boost/date_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/gregorian/parsers.hpp>
#include "../seglobal/CStringUtility.h"

using namespace se::conv;


namespace se
{
    namespace xbrl
    {
        
        CXbrlStatementModel::CXbrlStatementModel(CXbrlStatementParser* parser)  : _pParser(parser)
        {
            _logger = log4cxx::Logger::getLogger("se.xbrl.statementmodel");
        }

        CXbrlStatementModel::CXbrlStatementModel(const CXbrlStatementModel& orig) 
        {
            
        }

        CXbrlStatementModel::~CXbrlStatementModel() 
        {
            // clean up
            xbrlNs.clear(); // namespace file.
            for (auto pair : xbrlRefs)
                xmlFreeDoc(pair.second);
            xbrlRefs.clear();
            for (auto pair : statements)
                delete pair.second;
            statements.clear();
//            for (auto pair : elements)
//                delete pair.second;
            elements.clear();
        }
        
        XmlNs& CXbrlStatementModel::RegisterNamespaces(xmlDocPtr const doc, xmlNodePtr const node, string const& key) 
        {
            string sDefPfx;
            if (node && node->ns && node->ns->prefix)
                sDefPfx = (CSTR) node->ns->prefix;
            auto find = xbrlNs.find(key);
            if (find != xbrlNs.end())
                return find->second;
            xbrlNs.insert(make_pair(key, XmlNs()));
            xmlNsPtr* orig = xmlGetNsList(doc, node);
            xmlNsPtr* namespaces = orig;
            XmlNs& nsmap = xbrlNs[key];
            if (namespaces)
            {
                xmlNsPtr ns = *namespaces;
                while ( ns != NULL)
                {
                    string pfx ;
                    if (!ns->prefix)
                    {
                        if (sDefPfx != "")
                            pfx = sDefPfx;
                        else
                            pfx = "def";
                    }
                    else
                        pfx = (CSTR) ns->prefix;
                    if (nsmap.find(pfx) == nsmap.end())
                    {
                        nsmap.insert(make_pair(pfx, (string)(CSTR)ns->href));
                    }
                    namespaces++;
                    ns = *namespaces;
                }
                delete [] orig;
                
            }
            return nsmap;
        }
        
//        xmlNodePtr CXbrlStatementModel::FindNode()
//        {
//            
//        }
        
        xmlAttrPtr CXbrlStatementModel::FindAttribute(CSTR name, xmlNodePtr node)
        {
            if (!node) return nullptr;
            xmlAttrPtr attr = node->properties;
            for (; attr; attr = attr->next)
                if (!xmlStrcmp(attr->name, BAD_CAST name))
                    return attr;
            return nullptr;
        }
        
        void CXbrlStatementModel::CacheSchema(string& url)
        {
            if (url.empty()) return;
            using namespace boost;
            // we got the url. now we need the file name.
            string fileName = filesystem::path(url).filename().string();
            // now check for this file.
            auto mapfind = xbrlRefs.find(fileName);
            if (mapfind == xbrlRefs.end())
            {
                xmlDocPtr doc;
                string fileLoc = _pParser->get_FilesLocation();
                if (filesystem::exists( fileLoc + fileName))
                {
                    doc = xmlReadFile( fileLoc.c_str() , NULL, 0); 
                    if (doc)
                    {
                        XMLRefItem p(fileName, doc);
                        xbrlRefs.insert(p);
                    }
                }
                else
                {
                    doc = xmlReadFile((STR) url.c_str(), NULL, 0);
                    if (doc)
                    {
                        XMLRefItem p(fileName, doc);
                        xbrlRefs.insert(p);
                        CDirectory::Create(fileLoc.c_str()); // will create only if doesn't exist
                        string importedFile = fileLoc + fileName;
                        xmlSaveFile((const char*) importedFile.c_str(), doc);
                    }
                }
            }    
        }
        
        void CXbrlStatementModel::ConstructGraph(
                Graph& graph,
                string statementName, 
                UniqueElements& examined,
                UniqueElementsRev& revExamined,
                ElementPairs& edges,
                std::function<CXbrlElementBase* (const string&, int index,  int degree)> cbvertex
            )
        {
            LOG4CXX_INFO(_logger, "Preparing a adjacent list graph structure.");
            for (auto kvp : edges)
                add_edge(kvp.first, kvp.second, graph);

            // get the edge property map
            typedef property_map<Graph, NameVertexPropertyTag>::type VertexNameMap ;
            VertexNameMap vertexNameMap = get(NameVertexPropertyTag(), graph);

            typedef property_map<Graph, OrderEdgePropertyTag>::type EdgeWeightMap;
            EdgeWeightMap edgeWeightMap = get(OrderEdgePropertyTag(), graph);

            for (auto kvp : examined)
            {
                // then add to the name or order maps
                vertexNameMap[kvp.second] = kvp.first;
            }
            
            typedef property_traits<VertexNameMap>::key_type ElementType;
            typedef property_traits<VertexNameMap>::value_type NameType;

            typedef property_map<Graph, vertex_index_t>::type IndexMap;
            typedef property_map<Graph, edge_index_t>::type EdgeMap;
            IndexMap index = get(vertex_index, graph);
            EdgeMap edgeindex = get(edge_index, graph);

            typedef graph_traits<Graph>::vertex_iterator VertexIter;
            typedef graph_traits<Graph>::adjacency_iterator AdjIter;
            std::pair<VertexIter, VertexIter> vp;
            AdjIter ai, aiend;
            LOG4CXX_INFO(_logger, "Number of vertices : " << num_vertices(graph) );
            typedef graph_traits<Graph>::vertex_descriptor VertexDesc;
            typedef list<VertexDesc> TopoOrderedList;
            TopoOrderedList topoOrder;
            topological_sort(graph, front_inserter(topoOrder));
            string tabs ;
            using namespace se::utils;

            for (TopoOrderedList::iterator i = topoOrder.begin(); i != topoOrder.end(); ++i)
            {
                auto item = *i;
                auto degree = in_degree(item, graph);
                NameType name = get(vertexNameMap, item);
                
                CXbrlElementBase* elmt = cbvertex(name, item, degree);
                
                tabs = CStringUtility::Duplicate('\t', degree);
                LOG4CXX_INFO(_logger, "Adjacent vertex of " << name << " are: ");
                for (tie (ai, aiend) =adjacent_vertices(*i, graph); ai != aiend; ++ai)
                {
                    tabs = CStringUtility::Duplicate('\t', degree + 1);
                    LOG4CXX_INFO(_logger,  tabs << get(vertexNameMap, *ai) );
                    //auto sxbrlsubitem = revExamined[*ai];
                    //LOG4CXX_INFO(_logger, tabs << sxbrlsubitem);
                }
            }            
        }
        
        
//        void CXbrlStatementModel::AddGraph(
//            string statementName, 
//            UniqueElements& examined,
//            UniqueElementsRev& revExamined,
//            ElementPairs& edges
//        )
//        {
//            
////            LOG4CXX_INFO(_logger, "Preparing a adjacent list graph structure.");
////            Graph* pg = new Graph(edges.size());
////            for (auto kvp : edges)
////                add_edge(kvp.first, kvp.second, *pg);
////
////            // get the edge property map
////            typedef property_map<Graph, NameVertexPropertyTag>::type VertexNameMap ;
////            VertexNameMap vertexNameMap = get(NameVertexPropertyTag(), *pg);
////
////            typedef property_map<Graph, OrderEdgePropertyTag>::type EdgeWeightMap;
////            EdgeWeightMap edgeWeightMap = get(OrderEdgePropertyTag(), *pg);
////
////            for (auto kvp : examined)
////            {
////                // then add to the name or order maps
////                vertexNameMap[kvp.second] = kvp.first->get_Name();
////            }
////
////            typedef property_traits<VertexNameMap>::key_type ElementType;
////            typedef property_traits<VertexNameMap>::value_type NameType;
////
////            typedef property_map<Graph, vertex_index_t>::type IndexMap;
////            typedef property_map<Graph, edge_index_t>::type EdgeMap;
////            IndexMap index = get(vertex_index, *pg);
////            EdgeMap edgeindex = get(edge_index, *pg);
////
////            typedef graph_traits<Graph>::vertex_iterator VertexIter;
////            typedef graph_traits<Graph>::adjacency_iterator AdjIter;
////            std::pair<VertexIter, VertexIter> vp;
////            AdjIter ai, aiend;
////            LOG4CXX_INFO(_logger, "Number of vertices : " << num_vertices(*pg) );
////            typedef graph_traits<Graph>::vertex_descriptor VertexDesc;
////            typedef list<VertexDesc> TopoOrderedList;
////            TopoOrderedList topoOrder;
////            topological_sort(*pg, front_inserter(topoOrder));
////            string tabs ;
////            using namespace se::utils;
////
////            for (TopoOrderedList::iterator i = topoOrder.begin(); i != topoOrder.end(); ++i)
////            {
////                auto item = *i;
////                auto degree = in_degree(item, *pg);
////                NameType name = get(vertexNameMap, item);
////                tabs = CStringUtility::Duplicate('\t', degree);
////                auto pXbrlItem = revExamined[*i];
////
////                LOG4CXX_INFO(_logger, "Adjacent vertex of " << name << " are: ");
////                for (tie (ai, aiend) =adjacent_vertices(*i, *pg); ai != aiend; ++ai)
////                {
////                    tabs = CStringUtility::Duplicate('\t', degree + 1);
////                    LOG4CXX_INFO(_logger,  tabs << get(vertexNameMap, *ai) );
////                    auto pXbrlSubItem = revExamined[*ai];
////                    auto contextTie = ((CXbrlMonetaryElement*)pXbrlSubItem)->get_Contexts();
////                    int tabnum = 1;
////                    for (auto ctxtiter = get<0>(contextTie); ctxtiter != get<1>(contextTie); ++ctxtiter)
////                    {
////                        tabs = CStringUtility::Duplicate('\t', degree + 1 + tabnum);
////                        LOG4CXX_INFO(_logger, tabs << (*ctxtiter).first.get_EndDate() << ": " << (*ctxtiter).second );
////                        tabnum ++;
////                    }
////                    //LOG4CXX_INFO(_logger,  '');        
////                }
////            }
////            statements.insert(make_pair(statementName, pg));
//        }
        
        /* 
         * Cache and return an xbrl element.
         * These are the conditions to return an existing element from the cache or to create a new one.
         * 1. if the cache already doesn't have it, create a new one and return. Set existing to false
         * 2. if cache has an element with the same 'id' in elementNode xml node, find out what its parent is
         *      using get_Parent() property. If the parent matches parent argument, 
         *      then, check the role string. if they match, then we are returning the same item. set bExisting = true.
         * 3. else, create a new element, and set the parent and rolename properties and return it. bExisting = false.
        */
        CXbrlElementBase* CXbrlStatementModel::CreateXbrlElement(const string& roleName, const CXbrlElementBase * const parent, xmlNodePtr elementNode, bool& existing)
        {
            existing = false;
            CXbrlElementBase* p = NULL;
//            xmlChar* id = FindAttribute("id", elementNode)->children->content;
//            xmlChar* balance = BAD_CAST "";
//            auto balAttr = FindAttribute("balance", elementNode);
//            if (balAttr)
//                    balance = balAttr->children->content;
//            XBRL_BALANCE xbrlBalance = XBRL_BAL_None;
//            if (xmlStrcmp(balance, BAD_CAST "credit"))
//                xbrlBalance = XBRL_BAL_Credit;
//            else if (xmlStrcmp(balance, BAD_CAST "debit"))
//                xbrlBalance = XBRL_BAL_Credit;
//            else
//                xbrlBalance = XBRL_BAL_Debit;
//            // check if id exist
//            auto find = elements.equal_range((CSTR) id );
//            for (auto i = find.first; i != find.second; ++i)
//            {
//                if (i->second->get_Balance() == xbrlBalance)
//                {
//                    // look for the one with matching balance
//                    p = i->second;
//                    existing = true;
//                }
//            }
//            xmlChar* type = FindAttribute("type", elementNode)->children->content;
//            xmlChar* name = FindAttribute("name", elementNode)->children->content;
//            if (xmlStrstr(type, BAD_CAST "monetaryItemType"))
//            {
//                p = new CXbrlMonetaryElement((CSTR)id, (CSTR)name, (CSTR)name, xbrlBalance, false);
//            }
//            else // TODO: for the time being, we are clubbing all non-monetary items into an unknown element. In future more classification will be added.
//            {
//                p = new CXbrlUnknownElement((CSTR)id, (CSTR)name, (CSTR)name, xbrlBalance, false) ;
//            }
//            cout << "Created Pointers: " << p << endl;
//            auto pair = make_pair((string)(CSTR)id, p);
//            elements.insert(pair);
            existing = false;
            return p;
        }
        
        bool CXbrlStatementModel::EvaluateXPathExpression(xmlDocPtr pNodeToEvalFrom, string xpathExpression, string nsdata, std::function< void (xmlNodePtr, xmlXPathContextPtr) > cbpernode)
        {
            auto find = xbrlNs.find(nsdata);
            if (find == xbrlNs.end())
                return false;
            xmlXPathContextPtr context = xmlXPathNewContext(pNodeToEvalFrom) ;
            if (!context)
                return false;
            for (auto it : find->second)
            {
                if ( 0 != xmlXPathRegisterNs(context, BAD_CAST it.first.c_str(), BAD_CAST it.second.c_str()) )
                {
                    xmlXPathFreeContext(context);
                    return false;
                }
            }
            bool bRet = EvaluateXPathExpression(context, xpathExpression, cbpernode);
            xmlXPathFreeContext(context);
            return bRet;
        }
        
        bool CXbrlStatementModel::EvaluateXPathExpression(xmlXPathContextPtr pContext, string xpathExpression, std::function< void (xmlNodePtr, xmlXPathContextPtr) > cbpernode)
        {
            auto xpathLocObj = xmlXPathEvalExpression(BAD_CAST xpathExpression.c_str(), pContext);
            if (!xpathLocObj || !xpathLocObj->nodesetval || !xpathLocObj->nodesetval->nodeNr)
                return false;
            for (int i = 0; i != xpathLocObj->nodesetval->nodeNr; ++i)
                cbpernode(xpathLocObj->nodesetval->nodeTab[i], pContext);
            xmlXPathFreeObject(xpathLocObj);
            return true;
        }        
        
        // NOTE: a string is returned using new RVO feature of C11.
        string CXbrlStatementModel::GetDefaultNamespace(xmlNodePtr node)
        {
            if (node->ns && node->ns->prefix)
                return (string)(CSTR)node->ns->prefix;
            return (string)"def"; // casting to ensure RVO is carried out.
        }
    }
}

