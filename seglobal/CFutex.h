///* 
// * File:   CFutex.h
// * Author: santony
// *
// * Created on December 8, 2012, 12:57 PM
// */
//
//#ifndef CFUTEX_H
//#define	CFUTEX_H
//
//namespace se
//{
//    namespace threading
//    {
//        class CFutex 
//        {
//        public:
//            CFutex();
//            CFutex(const CFutex& orig);
//            virtual ~CFutex();
//
//            void Lock();
//            void Unlock();
//            bool Trylock();
//
//        private:
//            typedef union mutex
//            {
//                unsigned u;
//                struct
//                {
//                    unsigned char locked;
//                    unsigned char contended;
//                } b;
//            } mutex;        
//            
//            mutex _u;
//            
//        };
//    }
//}
//
//
//#endif	/* CFUTEX_H */
//
