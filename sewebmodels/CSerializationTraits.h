/* 
 * File:   CSerializationTraits.h
 * Author: santony
 *
 * Created on August 20, 2015, 11:51 AM
 */

#ifndef CSERIALIZATIONTRAITS_H
#define	CSERIALIZATIONTRAITS_H

#include <cppcms/serialization.h>
#include <memory>

namespace cppcms
{
    // specialized template to handle booleans
    // specialized from cppcms/serialization_classes.h
    template<>  
    struct archive_traits<bool, void>
    {  
        static void save(bool const &obj, archive &tgt)  
        {  
            const char* b = obj?"1":"0";
            tgt.write_chunk(b, sizeof(char));
        }  
        static void load(bool &obj, archive &src)  
        {  
            char data[1];
            src.read_chunk(&data, sizeof(char));
            obj = !strcmp((const char*)&data, "1");
        }  
    };


}
#endif	/* CSERIALIZATIONTRAITS_H */

