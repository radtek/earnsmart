#include "models.h"
#include <cppcms/http_cookie.h>
#include <boost/tokenizer.hpp>
#include <set>
#include <boost/algorithm/string.hpp>
#include "../seglobal/Object.h"
#include "../seglobal/CList.h"

using namespace cppcms::http;
using namespace boost;

namespace sg
{
    namespace web
    {
        namespace models
        {
            
            const string RECENTFINDS = "recentfinds";

            RecentFinds::RecentFinds()
            {
                
            }
            
            RecentFinds::RecentFinds(cppcms::http::context& ctxt) : BaseContent(ctxt)
            {
                
            }
            
            void RecentFinds::operator +=(int productid)
            {
                if (!getContext().session().is_set(RECENTFINDS))
                {
                    getContext().session()[RECENTFINDS] = "";
                }
                stringstream str ;
                str << getContext().session()[RECENTFINDS];
                if (str.str().empty())
                        str << productid;
                else
                {
                    str << "," << productid;
                    set<string> items;
                    typedef tokenizer<char_separator<char>> tokenizer;
                    char_separator<char> sep(",", "", keep_empty_tokens);
                    string recentids = str.str();
                    tokenizer tok(recentids, sep);
                    for (string s : tok)
                    {
                        trim(s);
                        if (s == "" || s.empty())
                            continue;
                        if (items.find(s) == items.end())
                            items.insert(s);
                    }
                    str.clear();
                    str.str("");
                    for (const string& s : items)
                    {
                        str << s << ",";
                    }
                }
                getContext().session()[RECENTFINDS] = str.str();
            }
            
            void RecentFinds::Clear()
            {
                getContext().session().erase(RECENTFINDS);
            }
            
            ErrorId RecentFinds::get_RecentFinds(ISPList<ISecurityDescription>** ppOut)
            {
                if (!getContext().session().is_set(RECENTFINDS) ||
                        getContext().session()[RECENTFINDS] == "")
                    return S_False;

                SP<ISPList<ISecurityDescription>> pList;
                RETURNIFFAILED(CObject<CSPList<ISecurityDescription>>::Create(&pList));
                SP<ISecurityAPI> pApi;
                RETURNIFFAILED(SecOpen(&pApi));
                // get ids from the tokenized string.
                typedef tokenizer<char_separator<char>> tokenizer;
                char_separator<char> sep(",", "", keep_empty_tokens);
                string recentids = getContext().session()[RECENTFINDS];
                tokenizer tok(recentids, sep);
                for (string s : tok)
                {
                    trim(s);
                    if (s == "" || s.empty())
                        continue;
                    SP<ISecurityDescription> pSec;
                    RETURNIFFAILED(pApi->get_SecurityDescription((UID)atoi(s.c_str()), &pSec));
                    pList->Add(pSec);
                }
                pList.CopyTo(ppOut);
                return S_Success;
            }
            
            bool RecentFinds::get_HasRecentFinds()
            {
                return (getContext().session().is_set(RECENTFINDS) && getContext().session()[RECENTFINDS] != "");
            }
            
            string RecentFinds::get_RecentFinds()
            {
                if (!getContext().session().is_set(RECENTFINDS) ||
                        getContext().session()[RECENTFINDS] == "")
                    return "<tr><td class='recentfinds'>None</td></tr>";
                
                SP<ISecurityAPI> pApi;
                if (SecOpen(&pApi) != S_Success)
                    return "<tr><td class='recentfinds'>None</td></tr>";
                
                // get ids from the tokenized string.
                typedef tokenizer<char_separator<char>> tokenizer;
                char_separator<char> sep(",", "", keep_empty_tokens);
                string recentids = getContext().session()[RECENTFINDS];
                tokenizer tok(recentids, sep);
                stringstream str;
                for (string s : tok)
                {
                    trim(s);
                    if (s == "" || s.empty())
                        continue;
                    SP<ISecurityDescription> pSec;
                    if (SUCCEEDED(pApi->get_SecurityDescription((UID)atoi(s.c_str()), &pSec)))
                    {
                        str << "<tr><td class='recentfinds'>";
                        str << "<a href=\"" << "/fundamentals/p/summary/" << s << "\">" ;
                        str << filters::escape(pSec->get_Name()) << "</a></td></tr>";
                    }
                }
                return str.str();
            }
        }
    }
}



