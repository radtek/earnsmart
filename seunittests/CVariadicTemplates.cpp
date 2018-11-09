/* 
 * File:   CVariadicTemplates.cpp
 * Author: santony
 * 
 * Created on September 4, 2015, 4:11 PM
 */

#include "CVariadicTemplates.h"
#include <iostream>
#include <functional>

using namespace std;
using namespace std::placeholders;

CVariadicTemplates::CVariadicTemplates() 
{
    
}


CVariadicTemplates::~CVariadicTemplates() 
{
    
}


////////////// static methods/////////////////////////////
// first example continuous adder
template<typename T>
T adder(T v)
{
    return v;
}

template<typename T, typename ...Args>
T adder(T firstArg, Args... args)
{
    return firstArg + adder(args...);
}

//2nd example shows how to compare arguments of same types
template<typename T>
bool compare(T l, T r)
{
    return l == r;
}

template<typename T, typename ...Args>
bool compare(T l, T r, Args... args)
{
    const int n = sizeof...(args);
    cout << "Number of args: " << n << endl;
    return l==r && compare(args...);
}

/// example 2 : how a tuple is constructed
template<class... Ts>
struct Tuple
{
    
};

template<class T, class... Ts>
struct Tuple<T, Ts...> : Tuple<Ts...>
{
    T tail;
    
    Tuple(T t, Ts... ts) : Tuple<Ts...>(ts...), tail(t)
    {
        
    }
};

//template<class T, class... Ts>
//struct elem_type_holder<0, Tuple<T, Ts...> >
//{
//    typedef T type;
//};
//
//template<size_t k, class T, class... Ts>
//struct elem_type_holder<k, Tuple<T, Ts...> >
//{
//    typedef typename elem_type_holder<k-1, Tuple<Ts...> >::type type;
//};


template<typename... funcs>
void DeclareFunction(funcs... fs)
{
    auto sz = sizeof...(fs);
    //std::function<_Res(_ArgTypes...)> Fs[sz] = {fs...};
}

void Test(int x, int  y)
{
    
}

void CVariadicTemplates::Run() 
{
    double sum = adder(1.0, 1.2, 2.3, 1.2);
    cout << "Variadic template adder: " << sum << endl;;

    bool issame = compare<const string&>("saji", "saji", "saji", "saji");
    cout << issame << endl;;
    issame = compare<const string&>("saji", "biji", "jim", "dolly");
    cout << issame << endl;;
    

//    DeclareFunction(
//            (void*)std::bind(&Test, _1, _2)
//            );
    
    
}
