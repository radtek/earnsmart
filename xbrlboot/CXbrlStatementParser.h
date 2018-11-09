/* 
 * File:   CXbrlStatementParser.h
 * Author: santony
 *
 * Created on January 4, 2013, 5:16 AM
 */

#ifndef CXBRLSTATEMENTPARSER_H
#define	CXBRLSTATEMENTPARSER_H

#include <map>
#include <string>
#include <libxml/tree.h>
#include <functional>
#include "sexbrlinternal.h"
#include "CXbrlCalculation.h"
#include "cxbrl"
#include "CXbrlPresentation.h"

using namespace std;

namespace se
{
    namespace xbrl
    {

        #define XBRLDocuments map<string, xmlDocPtr> 
        #define XBRLDocEnum pair<XBRLDocuments::const_iterator, XBRLDocuments::const_iterator>
        #define ParseCompleteFunction std::function<void (ErrorId)>
        
        class CXbrlStatementParser 
        {
            friend class CXbrlStatementModel;
            
        private:
            CXbrlStatementParser(const CXbrlStatementParser& orig);
            
            // enumerate all the xbrl documents and register them in the _sourceDocs map.
            void EnumerateFiles();
            
            // start preparing the documents, start with calculation and then presentation.
            ErrorId PrepareFiles();
            
            ErrorId Merge(CXbrlPresentation& presentation, CXbrlCalculation& calculation);
            
        public:
            CXbrlStatementParser(CSTR fileLocation);
            virtual ~CXbrlStatementParser();

            CSTR get_FilesLocation() const;
            XBRLDocEnum get_SourceDocs() const;
            
            xmlDocPtr get_SourceDoc(string docId) const;
            
            ErrorId ParseAsync(ParseCompleteFunction completion);
            ErrorId Parse();


        private:
            string _fileLocation;       // the local server directory where the xbrl files are downloaded from edgar ftp server.
            XBRLDocuments _sourceDocs;  // downloaded xbrl xml files for the given period from edgar ftp server.
            XBRLDocuments _cachedRefDocs;       // other cached documents for reference purpose while building statements.
            ParseCompleteFunction _fCompleted;  // the caller of the "ParseAsync" method is notified when parsing is completed.

            
            
        };
    }

}

#endif	/* CXBRLSTATEMENTPARSER_H */

