/* 
 * File:   CXbrlStatementParser.cpp
 * Author: santony
 * 
 * Created on January 4, 2013, 5:16 AM
 */

#include "sexbrlinternal.h"
#include <iostream>
#include <boost/algorithm/string/find_iterator.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/filesystem.hpp>
#include "CXbrlPresentation.h"
#include "CXbrlStatementParser.h"
#include <pthread_workqueue.h>
#include "../seglobal/CThreadPool.h"
#include "../seglobal/CDirectory.h"
#include "CXbrlCalculation_old.h"
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>

using namespace boost;

namespace se
{
    namespace xbrl
    {
        log4cxx::LoggerPtr _logger = log4cxx::Logger::getLogger("se.xbrl.statementparser");

        CXbrlStatementParser::CXbrlStatementParser(CSTR fileLocation) 
                : _fileLocation(fileLocation)
        {
            
        }

        CXbrlStatementParser::CXbrlStatementParser(const CXbrlStatementParser& orig) 
        {
        }

        CXbrlStatementParser::~CXbrlStatementParser() 
        {
            
        }

        CSTR CXbrlStatementParser::get_FilesLocation() const
        {
            return _fileLocation.c_str();
        }
        
        XBRLDocEnum CXbrlStatementParser::get_SourceDocs() const
        {
            return XBRLDocEnum(_sourceDocs.begin(), _sourceDocs.end());
        }
        
        xmlDocPtr CXbrlStatementParser::get_SourceDoc(string docId) const
        {
            auto find = _sourceDocs.find(docId);
            if (find != _sourceDocs.end())
                return find->second;
            return nullptr;
        }
        
        ErrorId CXbrlStatementParser::ParseAsync(ParseCompleteFunction completion)
        {
            ErrorId status = E_UnknownError;
            _fCompleted = completion;
         
            // TODO: Enable invoking of "Parse" method in threadpool
            //se::threading::CThreadPool::get_Instance()->Invoke()
         
            status = Parse();
            
            
            return status;
        }
        
        ErrorId CXbrlStatementParser::Parse()
        {
            LOG4CXX_INFO(_logger, "Beginning to parse the statement.");
            
            LIBXML_TEST_VERSION
            
            ErrorId retVal = E_UnknownError;
            try
            {
                EnumerateFiles();

                
                retVal = PrepareFiles();

                /* store here through a CDataTable instance. */

                if (retVal == S_Success)
                    LOG4CXX_INFO(_logger, "Parsing completed successfully.");
            }
            catch(const std::exception& ex)
            {
                LOG4CXX_ERROR(_logger, "Parsing error: " << ex.what());
            }
            return retVal;
            
        }
        
        void CXbrlStatementParser::EnumerateFiles()
        {
            /* Add instance, schema, cal, pre, lab files to the xbrlDocs map*/
            CDirectory::EnumDir(this->_fileLocation.c_str(), [](CSTR name, bool isdir, POINTER state) 
                {
                if (isdir) return true;

                CXbrlStatementParser* p = static_cast<CXbrlStatementParser*>(state);
                // use boost regex to identify different file types and populate
                // all know xbrl docs
                /*xxx.xml, xxx.xsd, xx_pre.xml, xx_cal.xml, xx_lab.xml */
                // "([\\w\\d]+)(-)([\\d]{8})(_)?(pre|cal|lab|def|ref)?(.xml|.xsd)"
                         // match regex
                    string sName = name;
                    boost::cmatch match;
                    const string pattern = "([\\w\\d/]+)(-)([\\d]{8})(_)?(pre|cal|lab|def|ref)?(.xml|.xsd)";
                    if (boost::regex_match(name, match, regex(pattern)))
                    {
                        if (-1 != sName.find("_pre.xml"))
                        {
                            XMLRefItem pr("pre", xmlReadFile(name, NULL, 0));
                            p->_sourceDocs.insert(pr);
                        }
                        else if (-1 != sName.find("_cal.xml"))
                        {
                            XMLRefItem pr("cal", xmlReadFile(name, NULL, 0));
                            p->_sourceDocs.insert(pr);
                        }
                        else if (-1 != sName.find("_lab.xml"))
                        {
                            XMLRefItem pr("lab", xmlReadFile(name, NULL, 0));
                            p->_sourceDocs.insert(pr);
                        }
                        else if (-1 != sName.find("_def.xml"))
                        {
                            XMLRefItem pr("def", xmlReadFile(name, NULL, 0));
                            p->_sourceDocs.insert(pr);
                        }
                        else if (-1 != sName.find(".xml"))
                        {
                            XMLRefItem pr("instance", xmlReadFile(name, NULL, 0));
                            p->_sourceDocs.insert(pr);
                        }
                        else if (-1 != sName.find(".xsd"))
                        {
                            XMLRefItem pr("schema", xmlReadFile(name, NULL, 0));
                            p->_sourceDocs.insert(pr);
                        }
                    }
                    return true;
                }, this);
        }
        
        ErrorId CXbrlStatementParser::PrepareFiles()
        {
            /* parse calculation and presentation separately
             and then merge the results. */
            
            CXbrlCalculation calculation(this);
            ErrorId retVal = calculation.Start();
            
            CXbrlPresentation presentation(this);
            retVal = presentation.Start();
            
            //presentation.Merge(calculation);
            // merge the results into a CDataSet, with each CDataTable respresenting
            // a statement.
            return retVal;
        }
    
    } // namespace xbrl
} // namespace se