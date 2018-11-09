/* 
 * File:   CXbrlPresentation.h
 * Author: santony
 *
 * Created on February 23, 2013, 11:22 PM
 */

#ifndef CXBRLPRESENTATION_H
#define	CXBRLPRESENTATION_H

#include "sexbrlinternal.h"
#include "CXbrlStatementModel.h"

namespace se
{

    namespace xbrl
    {
        /* Create a presentation view using the presentation xbrl xml file and
         and the node values calculated by CXbrlCalculation instance. */
        class CXbrlPresentation : public CXbrlStatementModel
        {
        public:
            CXbrlPresentation();
            CXbrlPresentation(const CXbrlPresentation& orig);
            virtual ~CXbrlPresentation();
        private:
        };

    }
}

#endif	/* CXBRLPRESENTATION_H */

