/* 
 * File:   CEarningCalendar.cpp
 * Author: santony
 * 
 * Created on May 23, 2014, 12:11 PM
 */

#include "internal.h"
#include <curl/curl.h>
#include <boost/algorithm/string/replace.hpp>
#include "../external/mariadbpp/sql_types.h"
#include <boost/algorithm/string/trim.hpp>
#include "CEarningCalendar.h"
#include "../seglobal/exceptions.h"
#include "../semysql/CMyqlUtilities.h"

using namespace mysqlpp;
using namespace se::data;

namespace se
{
    namespace securities
    {

        #define SEDBCONN        CSEConnections::GetMain()

        CEarningCalendar::CEarningCalendar()  //: _logger(Logger::getLogger("se.securities.earningscalender"))
        {
        }

        CEarningCalendar::CEarningCalendar(const CEarningCalendar& orig) 
        {
            
        }

        CEarningCalendar::~CEarningCalendar() 
        {
            
        }

        long unsigned int CEarningCalendar::Run(PERIOD capturePeriod) 
        {
            try
            {
                SE_CODE ret = S_OK;
                CDate dt = CDate::Now();
                if (capturePeriod == PERIOD_Annually)
                {
                    int year = dt.get_Year();
                    CDate dtStart(year, 1, 1);
                    do
                    {
                        if (dtStart.get_WeekDay() != WD_Sun && dtStart.get_WeekDay() != WD_Sat)
                        {
                            ret = Run(dtStart);
                            if (ret != S_OK)
                            {
                               // LOGE(_logger, "Failed to capture earnings for date " << dtStart);
                            }
                        }
                        dtStart = dtStart.AddSubDays(1);
                    } while (dtStart.get_Year() < (year + 1));
                }
                else if (capturePeriod == PERIOD_Monthly)   // runs for the current month from 1st to last.
                {
                    CDate dtFirst(dt.get_Year(), dt.get_Month(), 1);
                    CDate dtLast(dt.get_Year(), dt.get_Month(), 31);
                    while (dtFirst <= dtLast)
                    {
                        if (dtFirst.get_WeekDay() != WD_Sun && dtFirst.get_WeekDay() != WD_Sat)
                        {
                            ret = Run(dtFirst);
                            if (ret != S_OK)
                            {
                               // LOGE(_logger, "Failed to capture earnings for date " << dtFirst);
                            }
                        }
                        dtFirst = dtFirst.AddSubDays(1);
                    }
                }
                else if (capturePeriod == PERIOD_Daily)
                {
                    if (dt.get_WeekDay() != WD_Sun && dt.get_WeekDay() != WD_Sat)
                    {
                        ret = Run(dt);
                    }
                    else
                    {
                       // LOGIP("Today is a weekend/holiday.");
                    }
                }       
                return ret;
            }
            CATCHCLAUSE("Run");
        }
        
        SE_CODE CEarningCalendar::Run(const CDate& capturedate)
        {
            ostringstream ostr;
            ostr    << "http://biz.yahoo.com/research/earncal/"
                    << capturedate.get_Year() 
                    << std::setw(2) << std::setfill('0') << capturedate.get_Month() 
                    << std::setw(2) << std::setfill('0') << capturedate.get_Day()
                    << ".html";
            return DownloadHtml(capturedate, ostr.str());
        }
        
        uint CEarningCalendar::write_cb(char *in, uint size, uint nmemb, TidyBuffer *out)
        {
          uint r;
          r = size * nmemb;
          tidyBufAppend( out, in, r );
          return(r);
        }        

        long unsigned int CEarningCalendar::DownloadHtml(const CDate& date, const string& url) 
        {
            SE_CODE retCode;
            CURL* pcurl;
            char curl_errbuf[CURL_ERROR_SIZE];
            TidyDoc tdoc;
            TidyBuffer docbuf = {0};
            TidyBuffer tidy_errbuf = {0};
            int err;
            pcurl = curl_easy_init();
            curl_easy_setopt(pcurl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(pcurl, CURLOPT_ERRORBUFFER, curl_errbuf);
            curl_easy_setopt(pcurl, CURLOPT_NOPROGRESS, 0L);
            //curl_easy_setopt(pcurl, CURLOPT_VERBOSE, 1L);
            curl_easy_setopt(pcurl, CURLOPT_WRITEFUNCTION, write_cb);
            
            tdoc = tidyCreate();
            tidyOptSetBool(tdoc, TidyForceOutput, yes); /* try harder */ 
            tidyOptSetInt(tdoc, TidyWrapLen, 4096);
            tidySetErrorBuffer( tdoc, &tidy_errbuf );
            tidyBufInit(&docbuf);            
            
            curl_easy_setopt(pcurl, CURLOPT_WRITEDATA, &docbuf);
            err = curl_easy_perform(pcurl);
            if (!err)
            {
                err = tidyParseBuffer(tdoc, &docbuf);
                if (err >= 0)
                {
                    err = tidyCleanAndRepair(tdoc);
                    if (err >= 0)
                    {
                        err = tidyRunDiagnostics(tdoc);
                        if (err >= 0)
                        {
                            if (FAILED(ParseHtml(date, tdoc, tidyGetRoot(tdoc), 0 )))
                            {
                               // LOGE(_logger, "Tidy Error: " << tidy_errbuf.bp);
                                retCode = E_HtmlParseError;
                                goto CLEANEXIT;
                            }
                            else
                            {
                                retCode = S_OK;
                                goto CLEANEXIT;
                            }
                        }
                    }
                }
            }
            else
            {
               // LOGE(_logger, "Curl Error: " << curl_errbuf);
                retCode = E_HtmlParseError;
                goto CLEANEXIT;
            }
            
        CLEANEXIT:
            curl_easy_cleanup(pcurl);
            tidyBufFree(&docbuf);
            tidyBufFree(&tidy_errbuf);
            tidyRelease(tdoc);
            return retCode;
        }
        

        const string findstr = "Earnings Announcements for";
        long unsigned int CEarningCalendar::ParseHtml(const CDate& date, TidyDoc doc, TidyNode tnod, int indent) 
        {
            TidyNode child;
            for ( child = tidyGetChild(tnod); child; child = tidyGetNext(child) )
            {
                ctmbstr name = tidyNodeGetName( child );
                if ( name )
                {
                    /* if it has a name, then it's an HTML tag ... */ 
                    //TidyAttr attr;
                    //printf( "%*.*s%s ", indent, indent, "<", name);
                    /* walk the attribute list */ 
                    
                    /*
                    for ( attr=tidyAttrFirst(child); attr; attr=tidyAttrNext(attr) ) 
                    {
                      //printf(tidyAttrName(attr));
                      tidyAttrValue(attr)?printf("=\"%s\" ",
                                                 tidyAttrValue(attr)):printf(" ");
                    }
                    printf( ">\n");
                     */
                }
                else 
                {
                  /* if it doesn't have a name, then it's probably text, cdata, etc... */ 
                  TidyBuffer buf;
                  tidyBufInit(&buf);
                  tidyNodeGetText(doc, child, &buf);
                  if (buf.bp)
                  {
                      string strText = (CSTR)buf.bp;
                      if (strText.size() > findstr.size())
                      {
                        string substr = strText.substr(0, findstr.size());
                        if (substr == findstr)
                        {
                            TidyNode tbl = tidyGetParent(child);
                            tbl = tidyGetParent(tbl);
                            tbl = tidyGetParent(tbl);
                            tbl = tidyGetParent(tbl);
                            string istable = tidyNodeGetName( tbl );
                            if (istable == "table")
                            {
                                return ParseHtmlTable(date, doc, tbl);
                            }
                        }
                      }
                  }
                  //printf("%*.*s\n", indent, indent, buf.bp?(char *)buf.bp:"");
                  tidyBufFree(&buf);
                }
                ParseHtml(date, doc, child, indent + 4 ); /* recursive */ 
            }            
            return S_OK;
        }
        
        /*
         * Table: se_sec_earningscalendar
            Columns:
            product_id	bigint(20) PK
            cal_date	date
            cal_time	varchar(45)
         */
        
        sql_create_3(se_sec_earningscalendar, 3, 0, sql_bigint, product_id, sql_date, cal_date, sql_varchar_null, cal_time);

        SE_CODE CEarningCalendar::ParseHtmlTable(const CDate& date, TidyDoc doc, TidyNode tnod) 
        {
            
            TidyNode tr;
            int descend = 0;    // descend up to third to get the first valid row.
            for ( tr = tidyGetChild(tnod); tr; tr = tidyGetNext(tr) )
            {
                if (descend == 2)
                    break;
                descend++;
            }
            
            const string timemsgs[3] = {"Time Not Supplied", "Before Market Open", "After Market Close"};
            // a third way of showing time is :  02:00 am ET

            SP<IString> pdtstr;
            date.ToString(&pdtstr);
            CSmartConnection conn(SEDBCONN);
            typedef vector<se_sec_earningscalendar> Cal;
            Cal cal;
            for (; tr; tr = tidyGetNext(tr) )
            {
                string symbol, company, timestr;
                string strText;
                TidyBuffer buf;
                TidyNode txt;
                TidyNode td = tidyGetChild(tr);
                
                if (!td)
                    continue;
                
                // get the text
                txt = tidyGetChild(td);
                if (txt)
                {
                    tidyBufInit(&buf);
                    tidyNodeGetText(doc, txt, &buf);
                    if (buf.bp)
                    {
                        strText = (CSTR)buf.bp;
                        replace_all(strText, "\n", "");
                        company = strText;
                    }
                    tidyBufFree(&buf);
                    buf = {0};
                }
                
                td = tidyGetNext(td);
                if (td)
                {
                    TidyNode a = tidyGetChild(td);
                    if (a)
                    {
                        txt = tidyGetChild(a);
                        if (!txt)
                            continue;
                        tidyBufInit(&buf);
                        tidyNodeGetText(doc, txt, &buf);
                        if (buf.bp)
                        {
                            strText = (CSTR)buf.bp;
                            replace_all(strText, "\n", "");
                            symbol = strText;
                        }
                        tidyBufFree(&buf);
                        buf = {0};
                    }
                    td = tidyGetNext(td);   //eps estimate columns comes here
                    td = tidyGetNext(td);
                    if (td)
                    {
                        TidyNode sm = tidyGetChild(td);
                        if (sm)
                        {
                            txt = tidyGetChild(sm);
                            if (txt)
                            {
                                tidyBufInit(&buf);
                                tidyNodeGetText(doc, txt, &buf);
                                if (buf.bp)
                                {
                                    strText = (CSTR)buf.bp;
                                    replace_all(strText, "\n", "");
                                    timestr = strText;
                                }
                                tidyBufFree(&buf);            
                                buf = {0};
                            }
                        }
                    }
                    trim(symbol);
                    trim(timestr);
                    // store available values
                    Query q = conn->query();
                    q << "select product_id from se_sec_products where product_yahoosymbol = " << quote << symbol ;
                    UseQueryResult uqr = q.use();
                    Row row;
                    if (uqr && (row = uqr.fetch_row()))
                    {
                        // add to vector.
                        se_sec_earningscalendar citem;
                        citem.cal_date = (sql_date)pdtstr->get_Buffer();
                        if (timestr == timemsgs[0])
                            timestr = "NA";
                        else if (timestr == timemsgs[1])
                            timestr = "Before";
                        else if (timestr == timemsgs[2])
                            timestr = "After";
                        citem.cal_time = timestr;
                        citem.product_id = row[0];
                        
                        cal.push_back(citem);
                    }
                }
                if (company == "<small>&nbsp;</small>")
                    break;
                
               // LOGI(_logger, "Symbol: " << symbol << ", Company: " << company << ", Time: " << timestr);
            }
            
            if (cal.size())
            {
                Query q = conn->query();
                q.replace(cal.begin(), cal.end()).execute();
            }
            return S_OK;
        }

        
        const char* CEarningCalendar::get_CodeMessage(long unsigned int) 
        {

        }
      
    }
}