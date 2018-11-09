/* 
 * File:   cppcmsfilters.h
 * Author: santony
 *
 * Created on July 31, 2015, 10:24 AM
 */

#ifndef CPPCMSFILTERS_H
#define	CPPCMSFILTERS_H

#include <cppcms/filters.h>
#include "CModelBase.h"

using namespace cppcms;
namespace cppcms
{
    namespace filters
    {

        ///
        /// \brief Filter to render a validation error widget
        ///
        /// On post-submission of a page, verifies if certain controls
        /// have required value and if not displays a error label next to the 
        /// control
        ///
        class CPPCMS_API validationwidget 
        {
        public:
                validationwidget();
                validationwidget(validationwidget const &other);
                validationwidget const &operator=(validationwidget const &other);
                ~validationwidget();

                // Accepts a content field name and the associated html control name and regex pattern
                // Verifies if the
                validationwidget(streamable const &controlvalue, std::string const& controlname, std::string const& regexpattern, std::string const& warningtext);

                void operator()(std::ostream &out) const;

        private:
            streamable _controlValue;
            std::string _rgPattern;
            std::string _controlName;
            std::string _warningtext;
            streamable _widget;
        };

        inline std::ostream &operator<<(std::ostream &out, validationwidget const &obj)
        {
            obj(out);
            return out;
        }
    }
}
#endif	/* CPPCMSFILTERS_H */

