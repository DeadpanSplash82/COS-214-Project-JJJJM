#include "Military.h"
#include "MilitaryState.h"

///////////////////////////////////////////////////////////
// MilitaryState()
///////////////////////////////////////////////////////////

MilitaryState::MilitaryState()
{
  numTroops = 0;
  ships = NULL;
  tanks = NULL;
  planes = NULL;
  battalions = NULL;
  vehicleFactories = NULL;
}

///////////////////////////////////////////////////////////
// ~MilitaryState()
///////////////////////////////////////////////////////////

MilitaryState::~MilitaryState(){}

///////////////////////////////////////////////////////////
// setShips()
///////////////////////////////////////////////////////////

void MilitaryState::setShips(std::vector<Ship*>* _ships)
{
  ships = _ships;
}

///////////////////////////////////////////////////////////
// setPlanes()
///////////////////////////////////////////////////////////

void MilitaryState::setPlanes(std::vector<Plane*>* _planes)
{
  planes = _planes;
}

///////////////////////////////////////////////////////////
// setTanks()
///////////////////////////////////////////////////////////

void MilitaryState::setTanks(std::vector<Tank*>* _tanks)
{
  tanks = _tanks;
}

///////////////////////////////////////////////////////////
// setBattalions()
///////////////////////////////////////////////////////////

void MilitaryState::setBattalions(std::vector<Battalion*>* _battalions)
{
  battalions = _battalions;
}

///////////////////////////////////////////////////////////
// setVehicleFactories()
///////////////////////////////////////////////////////////

void MilitaryState::setVehicleFactories(std::vector<VehicleFactory*>* _vehicleFactories)
{
  vehicleFactories = _vehicleFactories;
}

///////////////////////////////////////////////////////////
// getNumTroops()
///////////////////////////////////////////////////////////

int MilitaryState::getNumTroops()
{
  return numTroops;
}

///////////////////////////////////////////////////////////
// updateNumTroops()
///////////////////////////////////////////////////////////

void MilitaryState::updateNumTroops(int _numTroops, bool isAddition)
{
  if (isAddition)
  {
    numTroops = numTroops + _numTroops;
    return;
  }
  // subtract
  if (numTroops - _numTroops < 0)
    throw std::runtime_error("new numTroops cannot be less than 0");
  else
    numTroops = numTroops - _numTroops;
}

///////////////////////////////////////////////////////////
// getNumTanks()
///////////////////////////////////////////////////////////

int MilitaryState::getNumTanks()
{
  return tanks->size();
}

///////////////////////////////////////////////////////////
// updateNumTanks()
///////////////////////////////////////////////////////////

void MilitaryState::updateNumTanks(int _numTanks, bool isAddition)
{
  if (isAddition)
  {
    tanks->resize(tanks->size() + _numTanks);
    return;
  }
  // subtract
  if (tanks->size() - _numTanks < 0)
    throw std::runtime_error("new numTanks cannot be less than 0");
  else
    tanks->resize(tanks->size() - _numTanks);
}

///////////////////////////////////////////////////////////
// getNumShips()
///////////////////////////////////////////////////////////

int MilitaryState::getNumShips()
{
  return ships->size();
}

///////////////////////////////////////////////////////////
// setNumShips()
///////////////////////////////////////////////////////////

void MilitaryState::updateNumShips(int _numShips, bool isAddition)
{
  if (isAddition)
  {
    ships->resize(ships->size() + _numShips);
    return;
  }
  // subtract
  if (ships->size() - _numShips < 0)
    throw std::runtime_error("new numShips cannot be less than 0");
  else
    ships->resize(ships->size() - _numShips);
}

///////////////////////////////////////////////////////////
// getNumPlanes()
///////////////////////////////////////////////////////////

int MilitaryState::getNumPlanes()
{
  return planes->size();
}

///////////////////////////////////////////////////////////
// setNumPlanes()
///////////////////////////////////////////////////////////

void MilitaryState::updateNumPlanes(int _numPlanes, bool isAddition)
{
  if (isAddition)
  {
    planes->resize(planes->size() + _numPlanes);
    return;
  }
  // subtract
  if (planes->size() - _numPlanes < 0)
    throw std::runtime_error("new numPlanes cannot be less than 0");
  else
    planes->resize(planes->size() - _numPlanes);
}

///////////////////////////////////////////////////////////
// getNumBattalions()
///////////////////////////////////////////////////////////

int MilitaryState::getNumBattalions()
{
  return battalions->size();
}

///////////////////////////////////////////////////////////
// updateNumBattalions()
///////////////////////////////////////////////////////////

void MilitaryState::updateNumBattalions(int _numBattalions, bool isAddition)
{
  if (isAddition)
  {
    battalions->resize(battalions->size() + _numBattalions);
    return;
  }
  // subtract
  if (battalions->size() - _numBattalions < 0)
    throw std::runtime_error("new numBattalions cannot be less than 0");
  else
    battalions->resize(battalions->size() - _numBattalions);
}
