#include <exception>
using namespace std;

#ifndef __WarStage_h__
#define __WarStage_h__

// #include "Country.h"

class Country;
class WarStage;

class WarStage
{
	public: 
    Country* countries;
    virtual void handle() = 0;
};

#endif