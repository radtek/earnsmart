/* 
 * File:   CSiteMenu.h
 * Author: santony
 *
 * Created on September 23, 2013, 11:09 AM
 */

#ifndef CSITEMENU_H
#define	CSITEMENU_H

#include <cppcms/serialization.h>
#include <string>
#include <vector>

using namespace std;
using namespace cppcms;

namespace sg
{
    namespace web
    {
        namespace models
        {
            struct Menu;
            
            struct MenuList : public serializable
            {
                typedef vector<Menu> ITEMS;
                
                ITEMS MenuItems;
                
                virtual void serialize(archive& a)
                {
                    a & MenuItems;
                }
            };
            
            struct Menu : public serializable
            {
                string Name;
                string Tooltip;
                string Link;
                MenuList Children;
                
            
                virtual void serialize(archive& ar)
                {
                    ar  & Name
                        & Tooltip
                        & Link
                        & Children;
                }
            };
        }
    }
}


#endif	/* CSITEMENU_H */

