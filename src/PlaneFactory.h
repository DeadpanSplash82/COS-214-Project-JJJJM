/// @author Jake Mahloko

#ifndef __PlaneFactory_h__
#define __PlaneFactory_h__

#include "VehicleFactory.h"
#include <exception>

/**
 *brief PlaneFactory Inherits from the VehicleFactory class which it uses the factory method
 *to create plane objects and return pointer to the object
 **/
class PlaneFactory : public VehicleFactory
{

public:
	/**
	 * @brief construct a new PlaneFactory object
	 **/
	PlaneFactory();
	/**
	 * @brief destroy a PlaneFactory object
	 **/
	~PlaneFactory();
	/**
	 * @brief creates Plane object which inherits from Vehicle
	 * @return Vehicle* but this class will send a Plane object
	 **/
	Vehicle *manufactureVehicle();
	// Vehicle *manufactureVehicle(int, int, int);

	/**
	 * @brief Create a deep copy of the current object
	 *
	 * @return PlaneFactory*
	 */
	PlaneFactory *clone();
};

#endif
