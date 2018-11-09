/* 
 * File:   CEarningCalendar.h
 * Author: santony
 * Retrieves earning calendar information from yahoo website. http://biz.yahoo.com/research/earncal/20140523.html
 * See calendarnotes.txt in resources folder for data to be parsed from the above link.
 * where date is replaced for any given day.
 * Created on May 23, 2014, 12:11 PM
 */

#ifndef CEARNINGCALENDAR_H
#define	CEARNINGCALENDAR_H

#include <tidy/tidy.h>
#include <tidy/buffio.h>


namespace se
{
    namespace securities
    {
        
        class CEarningCalendar 
        {
        public:
            CEarningCalendar();
            CEarningCalendar(const CEarningCalendar& orig);
            virtual ~CEarningCalendar();
            
            SE_CODE Run(PERIOD capturePeriod);
            SE_CODE Run(const CDate& capturedate);
            
        private:
            static uint write_cb(char *in, uint size, uint nmemb, TidyBuffer *out);
            SE_CODE DownloadHtml(const CDate& date, const string& url);
            SE_CODE ParseHtml(const CDate& date, TidyDoc doc, TidyNode tnod, int indent);
            SE_CODE ParseHtmlTable(const CDate& date, TidyDoc doc, TidyNode tnod);
            CSTR get_CodeMessage(SE_CODE);
            
        private:
            //LoggerPtr _logger;

        };
        
    }
}


#endif	/* CEARNINGCALENDAR_H */

