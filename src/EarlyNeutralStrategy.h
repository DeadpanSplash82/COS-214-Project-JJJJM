#ifndef EN_STRATEGY
#define EN_STRATEGY

#include "Strategy.h"

class Country;

class EarlyNeutralStrategy : public Strategy
{
public:
    EarlyNeutralStrategy();
    ~EarlyNeutralStrategy();
    virtual void takeTurn(Country* countryB);
};


#endif