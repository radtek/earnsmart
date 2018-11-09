/* 
 * File:   CFibnacciQueue.h
 * Author: santony
 *
 * Created on November 2, 2012, 10:47 AM
 */

#ifndef CFIBNACCIQUEUE_H
#define	CFIBNACCIQUEUE_H

#include <math.h>

namespace se
{
    namespace queues
    {

        template<class T>
        class CFibonacciQueue 
        {
            template<class T1>
            struct PriorityNode
            {
                U32 Degree;
                U32 Priority;
                T1 Data;
                PriorityNode<T1> Parent;
                PriorityNode<T1> Child;
                PriorityNode<T1> Left;
                PriorityNode<T1> Right;
                bool Mark;

                PriorityNode(T1 data, U32 priority)
                {
                    Data = data;
                    Priority = priority;
                    Degree = 0;
                    Parent = Child = 0;
                    Left = Right = this;
                    Mark = false;
                }

            } ;

            typedef PriorityNode<T> N, *PN;

        public:
            CFibonacciQueue() : _rootList(0), _minItem(0)
            {

            }

            CFibonacciQueue(const CFibonacciQueue& orig)
            {

            }

            ~CFibonacciQueue()
            {

            }

            /* Add an item to the queue with a given priority value */
            void Add(T data, U32 priority)
            {
                auto node = new N(data, priority);
                Insert(node);
                _length++;
            }

            U32 get_Length()
            {
                return _length;
            }

            void MergeQueues(CFibonacciQueue<T>* otherQueue)
            {
                MergeQueue(otherQueue->_rootList, otherQueue->_length);
            }

            bool Remove(T* pData, U32* pPriority)
            {
                auto node = DeleteMin();
                if (!node)
                {
                    *pData = T();
                    *pPriority = 0;
                    return false;
                }
                *pData = node->Data;
                *pPriority = node->Priority;
                return true;
            }

            bool Remote(T* pData)
            {
                U32 priority ;
                return Remove(pData, &priority);
            }

            T GetMinimum()
            {
                return _minItem->Data;
            }

        private:
            void Insert(PN node)
            {
                if (!_rootList)
                {
                    _rootList = _minItem = node;
                }
                else
                {
                    auto l = _rootList->Left;
                    l->Right = node;
                    _rootList->Left = node;
                    node->Left = l;
                    node->Right = _rootList;
                    if (node->Priority < _minItem->Priority)
                        _minItem = node;
                }
            }

            /* Link with another priority heap to build a larger heap */
            PN LinkHeaps(PN h1, PN h2)
            {
                if (h2->Priority < h1->Priority)
                    return LinkHeaps(h2, h1);

                h2->Parent = h1;
                PN child = h1->Child;
                h1->Child = h2;
                if (child)
                {
                    h2->Left = child->Left;
                    h2->Left->Right = h2;
                    h2->Right = child;
                    h2->Right->Left = h2;
                }
                else
                {
                    h2->Left = h2;
                    h2->Right = h2;
                }
                h1->Degree++;
                h2->Mark = false;
                return h1;
            }

            /* Merge to queues. Useful for multi-threaded scenario*/
            void MergeQueue(PN otherRoot, U32 length)
            {
                if (!_rootList)
                {
                    _rootList = otherRoot;
                    _minItem = otherRoot;
                    _length = length;
                }
                else
                {
                    PN n1 = _rootList->Left;
                    PN n2 = otherRoot->Left;
                    _rootList->Left = n2;
                    n2->Right = _rootList;
                    otherRoot->Left = n1;
                    n1->Right = otherRoot;

                    _length += length;

                    if (otherRoot->Priority < _minItem->Priority)
                    {
                        _minItem = otherRoot;
                    }
                }
            }

            PN DeleteFirst()
            {
                if (_rootList)
                {
                    PN r = _rootList;
                    if (r == r->Right)
                        _rootList = 0;
                    else
                    {
                        _rootList = r->Right;
                        r->Left->Right = r->Right;
                        r->Right->Left = r->Left;
                        r->Right = r;
                        r->Left = r;
                    }
                    _length?_length--:0;
                    return r;
                }
                else
                    return 0;
            }

            void AssertNode(PN n) throw()
            {
                if (!n)
                    return;
                auto n1 = n->Right;
                while (n != n1)
                {
                    if (n1->Child && n1 != n1->Child->Parent)
                    {
                        throw "Parent-child pointers incorrect.";
                    }
                    if (n1 != n1->Right->Left)
                    {
                        throw "Right-left relation incorrect";
                    }
                    if (n1 != n1->Left->Right)
                    {
                        throw "Left-right relation incorrect.";
                    }
                }
            }

            void Consolidate()
            {
                U32 len = 2 * (U32)log2((double)_length) + 1;
                PN orderedTree[] = new N[len];
                PN n = 0;
                while ( (n = DeleteFirst()) != 0 )
                {
                    PN n1 = 0;
                    while ( (n1 = orderedTree[n->Degree]) != 0 )
                    {
                        orderedTree[n->Degree] = 0;
                        n = LinkHeaps(n, n1);
                    }
                    orderedTree[n->Degree] = n;
                }
                _rootList = _minItem = 0;
                for (U32 i = 0; i != len; ++i)
                {
                    if (orderedTree[i] != 0)
                    {
                        Insert(orderedTree[i]);
                    }
                }
            }

            PN DeleteMin()
            {
                if (!_rootList) return 0;
                auto minNode = _minItem;
                if (_length == 1)
                {
                    _rootList = _minItem = 0;
                    _length = 0;
                }
                else
                {
                    U32 len = _length;
                    if (_minItem == _minItem->Right)
                    {
                        _rootList = _minItem = 0;
                    }
                    else
                    {
                        _rootList = _minItem->Right;
                    }
                    minNode->Left->Right = minNode->Right;
                    minNode->Right->Left = minNode->Left;
                    minNode->Left = minNode ;
                    if (minNode->Child)
                    {
                        auto n = minNode->Child;
                        do
                        {
                            n->Parent = 0;
                            n->Mark = false;
                            n =  n->Right;
                        } while (n != minNode->Child) ;

                        if (_rootList)
                        {
                            auto l1 = _rootList->Left;
                            auto l2 = n->Left;
                            l1->Right = n;
                            n->Left = l1;
                            l2->Right = _rootList;
                            _rootList->Left = l2;
                        }
                        else
                        {
                            _rootList = n;
                        }
                    }

                    _length = len - 1;
                    Consolidate();
                }
                return minNode;
            }

            CFibonacciQueue<T>* Split(PN n)
            {
              if (!n->Parent)  
                  return this;

              n->Parent->Degree--;
              if (n->Parent->Child == n)
              {
                  if (n->Right == n)
                      n->Parent->Child = 0;
                  else
                      n->Parent->Child = n->Right;
              }
              n->Parent = 0;
              n->Right->Left = n->Left;
              n->Left->Right = n->Right;
              n->Right = _rootList;
              n->Left = _rootList->Left;
              _rootList->Left->Right = n;
              _rootList->Left = n;
              _rootList = n;
              n->Mark = false;
              return this;
            }

            CFibonacciQueue<T>* Delete(PN n)
            {
                if (n->Child)
                {
                    auto c = n->Child;
                    auto e = n->Child;
                    do 
                    {
                        auto r = c->Right;
                        Split(c);
                        c = r;
                    } while (c != e) ;
                    if (n->Parent)
                    {
                        Split(n);
                    }
                    if ( n = n->Right)
                    {
                        _minItem = 0;
                        _rootList = 0;
                    }
                    else
                    {
                        if (_rootList = n)
                            _rootList = n->Right;
                        if (_minItem == n)
                        {
                            auto n1 = n->Right;
                            _minItem = n1;
                            do 
                            {
                                if (n1->Priority < _minItem->Priority)
                                {
                                    _minItem = n1;
                                }
                                n1 = n1->Right;
                            } while (n1 != n);
                        }

                        n->Right->Left = n->Left;
                        n->Left->Right = n->Right;
                        n->Left = n;
                        n->Right = n;
                    }
                }
                _length --;
                return this;
            }


        private:

            PN _rootList, _minItem;
            U32 _length;
        };
        
    }
}


#endif	/* CFIBNACCIQUEUE_H */

