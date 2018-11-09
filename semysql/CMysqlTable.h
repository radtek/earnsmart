/* 
 * File:   CMysqlTable.h
 * Author: santony
 *
 * Created on September 24, 2014, 7:58 PM
 */

#ifndef CMYSQLTABLE_H
#define	CMYSQLTABLE_H


namespace se
{
    namespace data
    {

        template<typename TTable, typename T>
        class CMysqlTable : CObjectRoot, public IList<T>
        {
        public:
            CMysqlTable(ISession* ps) : CObjectRoot(ps) {}

            virtual ~CMysqlTable()
            {
            }
            

            long unsigned int Init(const TTable& vec)
            {
                std::swap(_vector, vec);
                return S_OK;
            }


            virtual long unsigned int get_Count();

            virtual void Add(T item)
            {
                
            }

            virtual bool Remove(T item)
            {
                
            }

            virtual long unsigned int get_Item(long unsigned int index, T* pOut)
            {
                
            }

            virtual typename se::IList<T>::VECTOR::iterator begin()
            {
                return _vector.begin();
            }

            virtual typename se::IList<T>::VECTOR::iterator end()
            {
                return _vector.end();
            }

            virtual bool HasItem(T item)
            {
                
            }
//            
//            static SE_CODE Create(typename se::IList<T>::VECTOR& rawtable, CMysqlTable<T> **ppOut)
//            {
//                //return CObject<CMysqlTable<T>::Create(rawtable, ppOut);
//                return E_NotImpl;
//            }

        private:
            typename IList<T>::VECTOR _vector;

        };
        
        
    }
}

#endif	/* CMYSQLTABLE_H */

