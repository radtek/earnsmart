/* 
 * File:   CXbrlCalculation.h
 * Author: santony
 *
 * Created on November 25, 2012, 5:30 PM
 */

#ifndef CXBRLCALCULATION_H
#define	CXBRLCALCULATION_H

#include "CXbrlElement.h"
#include "CXbrlStatementModel.h"
#include "CXbrlStatementParser.h"
#include "sexbrlinternal.h"


#include <string>
#include <map>
#include <vector>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <boost/graph/directed_graph.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/properties.hpp>

#include <log4cxx/logger.h>

using namespace std;
using namespace boost;

namespace se
{
    
    namespace xbrl
    {


        class CXbrlCalculation  : public CXbrlStatementModel
        {

        private:



        public:
            CXbrlCalculation(CXbrlStatementParser* parser);
            CXbrlCalculation(const CXbrlCalculation& orig);
            virtual ~CXbrlCalculation();
            virtual ErrorId Start();
            
        private:
            void ProcessCalculationLink(string, xmlDocPtr, xmlXPathContextPtr, xmlNodePtr);
            void ProcessCalculationArc(string, xmlDocPtr, const string& , xmlXPathContextPtr, xmlNodePtr);
            //CXbrlElementBase* SetupXbrlElement()

        private:
            string defNsPfx;     
            string location;
            XMLRefs xbrlRefs ;
            XMLRefs xbrlDocs;   // a map of quarterly or annual documents submitted by the company.
            Statements statements;
            
            log4cxx::LoggerPtr _logger;

        };

    }
}

#endif	/* CXBRLCALCULATION_H */