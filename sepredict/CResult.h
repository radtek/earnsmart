/* 
 * File:   CResult.h
 * Author: santony
 *
 * Created on May 14, 2015, 8:23 PM
 */

#ifndef CRESULT_H
#define	CRESULT_H

namespace se
{
    namespace predict
    {
        class CResult : public CObjectRoot, public IResult
        {
        public:
            CResult(ISession* );
            CResult(const CResult& orig) = delete;
            virtual ~CResult();
            

            virtual OmniType& Value();

        private:
            OmniType _value;
        };
    }
}


#endif	/* CRESULT_H */

