/// @author Jacob Mahloko

#include "Tank.h"
#include "TankFactory.h"

using namespace std;

TankFactory::TankFactory(){}

TankFactory::~TankFactory(){}

Vehicle *TankFactory::manufactureVehicle()
{
	return new Tank();
}

TankFactory *TankFactory::clone()
{
	return new TankFactory();
}
