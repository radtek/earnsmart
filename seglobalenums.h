#ifndef SEGLOBALENUMS_H_INCLUDED
#define SEGLOBALENUMS_H_INCLUDED

namespace se
{

    typedef enum tagExchanges
    {
        EXCHANGE_ANY        =   0x0,

    } EXCHANGES;

    typedef enum tagCurrencies
    {
        CURRENCY_ANY        = 0x00,
        CURRENCY_USD        = 0x01  // use country code here.
    } CURRENCIES;
    
    typedef enum tagCountries
    {
        COUNTRY_ANY         =   0x000,
        /* other real country ids goes in between*/
        COUNTRY_USA         =   0x001,
        COUNTRY_INDIA       =   0x05B,
        COUNTRY_ALL         =   0xFFF
    } COUNTRIES;

}

#endif // SEGLOBALENUMS_H_INCLUDED
