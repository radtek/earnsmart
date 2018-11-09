/* 
 * File:   CXbrlCalculation.cpp
 * Author: santony
 * 
 * Created on November 25, 2012, 5:30 PM
 */

#include "sexbrlinternal.h"
#include "CXbrlCalculation_old.h"
#include <iostream>
#include <boost/algorithm/string/find_iterator.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/filesystem.hpp>
#include "../sedata/sedataheaders.h"
#include <algorithm>
#include <libxml/xpath.h>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/variant/get.hpp>
#include "../seglobal/CDirectory.h"
#include "../seglobal/CStringToNumber.h"
#include "../seglobal/CDirectory.h"
#include <boost/date_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/gregorian/parsers.hpp>
#include "../seglobal/CStringUtility.h"

using namespace se::data;
using namespace se::conv;

namespace se
{

    namespace xbrl
    {
        

        //TODO : to be configured
        #define XMLDOCSAVELOCATION  "/home/santony/Documents/xbrl/schema/" 

        CXbrlCalculation::CXbrlCalculation(CXbrlStatementParser* parser) : CXbrlStatementModel(parser)
        {
            _logger = log4cxx::Logger::getLogger("se.xbrl.calculation");
        }

        CXbrlCalculation::CXbrlCalculation(const CXbrlCalculation& orig) : CXbrlStatementModel(static_cast<const CXbrlStatementModel&>(orig))
        {

        }

        CXbrlCalculation::~CXbrlCalculation() 
        {
        }

        /* xbrl cal flow 
         * 
         * 1. Load xbrl cal file as an xml document
         * 2. go through the child elements and look for
         *      a. "loc"
         *      b. "calculationArc"
         *      c. roleRef
         * 
         * 3. if "roleref" or "loc"
         *      a. create a map with key and xml doc as value. the key = xsd file name from 
         *           href attribute of roleref or loc element.
         *      b. open the document from internet (use xml's doc) and save locally for caching purpose.
         *      c. hold the xsd files in memory in the map
         *
         * 4. Create a new fresh document for building a calculation tree.
         * 
         * 5. look for calculationLink elements. 
         *      a. take from and to values. these are temporary labels.
         *      b. look within the same xml for "link" where temporary label = real label in xbrl context.
         *      c. Using the xbrl document downloaded, build a hierarchy and into the calculation doc (step 4).
         *      
         */


        ErrorId CXbrlCalculation::Start()
        {
            ErrorId retVal;
            xmlDocPtr docCal = _pParser->get_SourceDoc("cal");
            if (!docCal)
            {
                LOG4CXX_ERROR(_logger, "Start: Calculation xbrl file is missing.");
                return E_FileNotFound;
            }
            
            LOG4CXX_INFO(_logger, "Start: Xbrl calculation parsing started.");
            // get the root node.
            xmlNode* rootNode = xmlDocGetRootElement(docCal);
            // register namespaces
            XmlNs& calNs = RegisterNamespaces(docCal, rootNode, "cal");
            string defCalNsPfx = GetDefaultNamespace(rootNode);
            string xpathExpression = "//" + XPathNsExpr(defCalNsPfx) + "calculationLink";
            LOG4CXX_INFO(_logger, "Processing calculation links");
            EvaluateXPathExpression(
                    (xmlDocPtr) rootNode, 
                    xpathExpression, 
                    "cal", 
                    [this, &xpathExpression, &docCal](xmlNodePtr linkNode, xmlXPathContextPtr xpathContext)-> void
                    {
                        LOG4CXX_INFO(_logger, "Processing calculation link");
                        this->ProcessCalculationLink(xpathExpression, docCal , xpathContext, linkNode);
                    }
                    );
            LOG4CXX_INFO(_logger, "Completed with processing calculation links");
            LOG4CXX_INFO(_logger, "Completed with calculation xbrl file parsing.")
            return retVal;
        }
        
        void CXbrlCalculation::ProcessCalculationLink(string basexpath, xmlDocPtr doc, xmlXPathContextPtr ctxt, xmlNodePtr linkNode)
        { 
            // we are ignoring all non element node types
            if (linkNode->type != XML_ELEMENT_NODE)
                return;
            
            LOG4CXX_INFO(_logger, "Entering to evaluate a calculation link node.");
            
            elements.clear();
            ElementPairs graphPairs;
            UniqueElements examined;
            UniqueElementsRev revExamined;
            
            xmlNodePtr rootNode = xmlDocGetRootElement(doc);
            
            // fine attribute role which tells us what this statement is for.
            xmlAttrPtr roleAttr = FindAttribute("role", linkNode);
            // capture its name.
            if (!roleAttr)
            {
                LOG4CXX_ERROR(_logger, "The calculation link doesn't have a role attribute.")
                return;
            }
            string roleAttrNs, roleRef, roleName;
            string defCalNsPfx = GetDefaultNamespace(rootNode);
            if (roleAttr->ns && roleAttr->ns->prefix)
            {
                roleAttrNs = (CSTR) roleAttr->ns->prefix;
            }                
            roleRef = (CSTR)roleAttr->children->content;
            roleName = filesystem::path(roleRef).filename().string();
            LOG4CXX_INFO(_logger, "Captured calculation role name as " << roleName);
         
            // add a root element for our statement. This is a must for creating graph edges
            // and then creating a topology tree.
            CXbrlRootElement* pRootElement = new CXbrlRootElement(roleName.c_str());
            pRootElement->set_StatementType(ParseStatementType(roleName));
            pRootElement->set_RoleName(roleName.c_str());
            //elements.insert(make_pair(roleName, pRootElement));
            
            // add them to maps used for boost graph
            elements.insert(roleName);
            examined.insert(make_pair(roleName, 0));
            revExamined.insert(make_pair(0, roleName));

            // look for all calculationArc nodes.
            string linkbasexpath = basexpath + "[@" + XPathNsExpr(roleAttrNs) + "role='" + roleRef + "']";
            string arcxpathexpr = linkbasexpath + "/" + XPathNsExpr(defCalNsPfx) + "calculationArc" ;
            
            LOG4CXX_INFO(_logger, "Starting to evaluate each calculation arc node.");
            
            EvaluateXPathExpression(
                ctxt, 
                arcxpathexpr, 
                [this, &linkbasexpath, &doc, &roleName, &graphPairs, &examined, &revExamined](xmlNodePtr arcNode, xmlXPathContextPtr pctxt)->void
                {
                    // create edges, vertices and graph here.
                    string fromlbl = (CSTR) FindAttribute("from", arcNode)->children->content;
                    string tolbl = (CSTR) FindAttribute("to", arcNode)->children->content;
                    int nxtpos0, nxtpos1;
                    if ( elements.find(fromlbl) == elements.end())
                    {
                        nxtpos0 = elements.size();
                        elements.insert(fromlbl);
                        examined.insert(make_pair(fromlbl, nxtpos0));
                        revExamined.insert(make_pair(nxtpos0,fromlbl));
                        graphPairs.push_back(make_pair(0, nxtpos0));
                    }
                    else
                    {
                        nxtpos0 = examined[fromlbl];
                    }

                    if (elements.find(tolbl) == elements.end())
                    {
                        nxtpos1  = elements.size();
                        elements.insert(tolbl);
                        examined.insert(make_pair(tolbl, nxtpos1));
                        revExamined.insert(make_pair(nxtpos1,tolbl));
                    }
                    else
                    {
                        nxtpos1 = examined[tolbl];
                    }
                    graphPairs.push_back(make_pair(nxtpos0, nxtpos1));
                }
                );
                
                Graph graph(examined.size());
                ConstructGraph(
                        graph, 
                        roleName, 
                        examined, 
                        revExamined, 
                        graphPairs, 
                        [this](const string& name, int index, int degree)-> CXbrlElementBase* 
                {
                    
                });

            LOG4CXX_INFO(_logger, "Completed evaluating all calculation arc nodes.");

            LOG4CXX_INFO(_logger, "Completed with evaluate a calculation link node.");
        }

        void CXbrlCalculation::ProcessCalculationArc(string basexpath, xmlDocPtr doc, const string& roleName, xmlXPathContextPtr ctxt, xmlNodePtr arcNode)
        {
            // we will ignore all non element node type.
            if (arcNode->type != XML_ELEMENT_NODE)
                return ;
            
            LOG4CXX_INFO(_logger, "Entering to process an arc node.");
            /* calculationArc 
             * has from and to
             * get from or to, and then read the label.
             * locate "loc" with the above label and then capture the file name form "href".
             * use this file name and obtain the xmldoc from xbrlRefs map (xsd file)
             * locate the element from xsd file, and create an instance of CXbrlElement(Base).
             * Now using this value, go to instance document and read monetary values set to the CXbrlElement instance.
             * Create an edge using "from" and "to" CXbrlElements
             * Add to the edge vector.
             */
            
            // variables for CXbrlElement(Base)
            I8 weight ;
            I8 priority;
            string use ;
            R4 order ;
            string edgeStr[2];
            string locExpression ;

            xmlNodePtr rootNode = xmlDocGetRootElement(doc);
            string defCalNsPfx = GetDefaultNamespace(rootNode);
            string attrPfx;
            
            xmlAttrPtr attr = arcNode->properties;
            for (xmlAttrPtr a = attr; a ; a = a->next)
            {
                if (!xmlStrcmp(BAD_CAST "weight", a->name))
                {
                    // convert xmlChar* to double
                    weight = CConvertString::To<I8>( (CSTR) a->children->content);
                    
                }
                else if (!xmlStrcmp(BAD_CAST "order", a->name))
                {
                    order = CConvertString::To<R4>( (CSTR) a->children->content);
                }
                else if (!xmlStrcmp(BAD_CAST "from", a->name))
                {
                   edgeStr[0] = (CSTR) a->children->content;
                   if (attrPfx.empty() && a->ns && a->ns->prefix)
                        attrPfx = (CSTR) a->ns->prefix;
                }
                else if (!xmlStrcmp(BAD_CAST "to", a->name))
                {
                    edgeStr[1]  = (CSTR) a->children->content;
                    if (attrPfx.empty() && a->ns && a->ns->prefix)
                        attrPfx = (CSTR) a->ns->prefix;
                }
                else if (!xmlStrcmp(BAD_CAST "priority", a->name))
                {
                    priority = CConvertString::To<I8>( (CSTR) a->children->content);
                }
                else if (!xmlStrcmp(BAD_CAST "use", a->name))
                {
                    use = (CSTR) a->children->content;
                }
            }
            // if either from to to are missing, we can't proceed
            if (edgeStr[0].empty() || edgeStr[1].empty()) 
            {
                LOG4CXX_WARN(_logger, "ProcessCalculationArc: Either from or to is missing from the calculation arc node.")
                return ;
            }
            
            CXbrlElementBase* edgePointers[2] ;
            
            for (int i=0; i!=2; ++i)
            {
                locExpression = basexpath + "/"  + XPathNsExpr(defCalNsPfx) + "loc[@" + XPathNsExpr(attrPfx) + "label='" + edgeStr[i] + "']";
                xmlDocPtr calcLink = (xmlDocPtr) arcNode->parent;
                EvaluateXPathExpression(
                        ctxt, 
                        locExpression, 
                        [this, &edgePointers, i, &roleName](xmlNodePtr locNode, xmlXPathContextPtr context)->void
                        {
                    xmlAttrPtr lochref = FindAttribute("href", locNode);
                    if (!lochref) return;
                    string xsdfileuri = (CSTR) lochref->children->content;
                    // cache it for future use
                    vector<string> splits;
                    split(splits, xsdfileuri, is_any_of("#"));
                    string sSchemaFileUri = splits[0];
                    string sxbrltag = splits[1];
                    CacheSchema(sSchemaFileUri);
                    auto sSchemaFile = filesystem::path(sSchemaFileUri).filename().string();
                    // this file may be in xbrlDocs or xbrlRefs map
                    // first check by name in xbrlRefs and then 
                    // check if it is same as company's schema extension file.
                    string schemaStoreName ;
                    xmlDocPtr pSchemaFile = nullptr;
                    bool failed = false;
                    auto findSchemaFile = xbrlRefs.find(sSchemaFile);
                    if (findSchemaFile == xbrlRefs.end())
                    {
                        pSchemaFile = _pParser->get_SourceDoc("schema");
                        if (pSchemaFile == nullptr)
                            failed = true;
                        else
                            schemaStoreName = "schema";
                    }
                    else
                    {
                        pSchemaFile = findSchemaFile->second;
                        schemaStoreName = findSchemaFile->first;
                    }
                    if (failed) return;
                    // get the root node
                    xmlNodePtr schemaDocRoot = xmlDocGetRootElement(pSchemaFile);
                    RegisterNamespaces(pSchemaFile, schemaDocRoot, schemaStoreName);
                    string schemaDefNs = GetDefaultNamespace(schemaDocRoot);
                    string elmtxpath = "//" + XPathNsExpr(schemaDefNs) + "element[@id='" + sxbrltag + "']";
                    CXbrlElementBase* pEdgeVertex = edgePointers[i];
                    EvaluateXPathExpression(pSchemaFile, elmtxpath, schemaStoreName, 
                            [this, &pEdgeVertex, &sxbrltag, &roleName] (xmlNodePtr elmtNode, xmlXPathContextPtr pctxt) mutable ->void 
                    {
                        xmlDocPtr instanceDoc = _pParser->get_SourceDoc("instance");
                        xmlNodePtr instanceRoot = xmlDocGetRootElement(instanceDoc);
                        RegisterNamespaces(instanceDoc, instanceRoot, "instance");
                        string defInstanceNs = GetDefaultNamespace(instanceRoot);
                        string findns, findtag ;
                        vector<string> splits;
                        split(splits, sxbrltag, is_any_of("_"));
                        findns = splits[0];
                        findtag = splits[1];
                        string elmtxpath = "//" + XPathNsExpr(findns) + findtag;
                        EvaluateXPathExpression(instanceDoc, elmtxpath, "instance", 
                                [this, &pEdgeVertex, &roleName](xmlNodePtr instnode, xmlXPathContextPtr)-> void
                        {
                            bool bExisting = false;
                            pEdgeVertex = CreateXbrlElement(roleName, nullptr, instnode, bExisting);
                            if (!pEdgeVertex) return ;
                            
                        }
                                );
                        
                    }
                            );  // schema evaluatexpathexpression end
                    
                        }
                    );  // loc node evaluateexpression end.
            }                    
                    
            LOG4CXX_INFO(_logger, "Completing with processing an arc node.");
        }

    }
}