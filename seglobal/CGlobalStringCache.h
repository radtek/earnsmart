/* 
 * File:   CGlobalStringCache.h
 * Author: santony
 *
 * Created on January 12, 2013, 5:17 PM
 */

#ifndef CGLOBALSTRINGCACHE_H
#define	CGLOBALSTRINGCACHE_H

#include "../seglobaltypedefs.h"
#include <string>
#include <unordered_set>

namespace se
{

    /* A global cache of reusable string values. A std::string is with the same value is used
     more than once in the application, then utilize this cache to conserve memory. */
    class CGlobalStringCache 
    {
    public:
        CGlobalStringCache();
        CGlobalStringCache(const CGlobalStringCache& orig);
        virtual ~CGlobalStringCache();
        // add a new string and return its index. Note that
        // if a string with same value exists, no effort is made to find it.
        const std::string& operator[](const std::string& str);
        static CGlobalStringCache& Get();
        static void Free() throw();

    private:
        static CGlobalStringCache* _pCache ;
        std::unordered_set<std::string> _strcache;

    };

}
#endif	/* CGLOBALSTRINGCACHE_H */

