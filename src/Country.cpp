/// @author Jake Weatherhead

#include "Country.h"
#include "WarStage.h"
#include "Superpower.h"
#include "Military.h"
#include "MilitaryState.h"
#include "CountryState.h"
#include "EarlyStrategy.h"
#include "MiddleStrategy.h"
#include "LateStrategy.h"
#include "Strategy.h"
#include "StageContext.h"
#include "StageContextState.h"
#include "Location.h"
#include "LocationObserver.h"
#include "Map.h"
#include <exception>
#include <stdexcept>
#include <vector>

const std::string RESET = "\x1B[0m";
const std::string BOLD = "\x1B[1m";
static std::string strategyList[3] = {"EarlyStrategy", "MiddleStrategy", "LateStrategy"};

///////////////////////////////////////////////////////////
// Country()
///////////////////////////////////////////////////////////

Country::Country()
{
  countryState = new CountryState();
  countryState->locations = NULL;
  strategy = NULL;
  military = NULL;
  locationObservers = new std::vector<LocationObserver *>();
}

///////////////////////////////////////////////////////////
// ~Country()
///////////////////////////////////////////////////////////

Country::~Country()
{
  if (!countryState->isBeingStored && countryState->enemies != NULL)
    for (int i = 0; i < countryState->enemies->size(); i++)
    {
      countryState->enemies->at(i)->removeEnemy(this);
    }
  if (strategy != NULL)
    delete strategy;
  strategy = NULL;
  if (military != NULL)
    delete military;
  military = NULL;
  if (countryState != NULL)
    delete countryState;
  countryState = NULL;
  if (locationObservers != NULL)
  {
    for (LocationObserver *i : *locationObservers)
    {
      if (i != NULL)
        i->updateLocation(colorOfDestroyedBy);
    }
    delete locationObservers;
  }
}

///////////////////////////////////////////////////////////
// Country(std::string)
///////////////////////////////////////////////////////////

Country::Country(std::string _name)
{
  countryState = new CountryState();
  countryState->name = _name;
  military = NULL;
  strategy = NULL;
  countryState->locations = NULL;
  locationObservers = new std::vector<LocationObserver *>();
}

///////////////////////////////////////////////////////////
// takeTurn(Country*)
///////////////////////////////////////////////////////////

void Country::takeTurn(Country *countryB)
{
  setStrategy();
  double strengthRatings[2];
  getCountryRating(countryB, strengthRatings);
}

///////////////////////////////////////////////////////////
// takeTurn(bool *)
///////////////////////////////////////////////////////////

Country *Country::takeTurn(bool *_countryIsDead)
{
  setStrategy();
  double strengthRatings[2];
  // srand((unsigned)time(NULL));
  if (getEnemies()->size() == 0)
  {
    return NULL;
  }
  Country *countryB = getEnemies()->at(rand() % getEnemies()->size());
  getCountryRating(countryB, strengthRatings);
  strategy->takeTurn(strengthRatings, this, countryB);
  _countryIsDead[0] = checkIsDead(this, countryB); // check if countryA is dead
  if (_countryIsDead[0])
  {
    setColorOfDestroyedBy(countryB->getColor());
    std::vector<Location *> *myLocations = getState()->locations;
    for (Location *i : *myLocations)
    {
      i->setIsCapital(false);
      countryB->getState()->locations->push_back(i);
      i->setOwnedBy(countryB);
    }
    getBoost(countryB);
  }
  _countryIsDead[1] = checkIsDead(countryB, this); // check if countryB is dead
  if (_countryIsDead[1])
  {
    countryB->setColorOfDestroyedBy(this->getColor());
    std::vector<Location *> *myLocations = countryB->getState()->locations;
    for (Location *i : *myLocations)
    {
      i->setIsCapital(false);
      this->getState()->locations->push_back(i);
      i->setOwnedBy(this);
    }
    getBoost(this);
  }
  return countryB;
}

///////////////////////////////////////////////////////////
// checkIsDead()
///////////////////////////////////////////////////////////

// bool Country::checkIsDead()
// {
//   double stateSum = 0;
//   stateSum += getBorderStrength();
//   stateSum += getSelfReliance();
//   stateSum += getCapitalSafety();
//   stateSum += getPoliticalStability();
//   stateSum += getTradeRouteSafety();
//   stateSum += getDomesticMorale();
//   stateSum += getWarSentiment();
//   return (stateSum < 1);
// }

///////////////////////////////////////////////////////////
// checkIsDead(Country*, Country*)
///////////////////////////////////////////////////////////

bool Country::checkIsDead(Country *countryA, Country *countryB)
{
  double stateSum = 0;
  stateSum += countryA->getBorderStrength();
  stateSum += countryA->getSelfReliance();
  stateSum += countryA->getCapitalSafety();
  stateSum += countryA->getPoliticalStability();
  stateSum += countryA->getTradeRouteSafety();
  stateSum += countryA->getDomesticMorale();
  stateSum += countryA->getWarSentiment();

  if (stateSum < 1)
  {
    std::string colorB = countryB->getColor();
    std::string sType = strategyList[StageContext::getInstance()->getWarStage()];
    std::cout << "(" << sType << ") " << colorB << countryA->getName() << RESET
              << " was " << BOLD << "captured" << RESET
              << " by " << colorB << countryB->getName()
              << RESET << std::endl;
  }
  return (stateSum < 1);
}

///////////////////////////////////////////////////////////
// checkIsDead(Country*, Country*)
///////////////////////////////////////////////////////////

void Country::getBoost(Country* _country){
  double boostAmount = 0.45;
  if (_country->getBorderStrength() + boostAmount > 1)
    _country->setBorderStrength(1);
  else
    _country->setBorderStrength(_country->getBorderStrength() + boostAmount);

  if (_country->getCapitalSafety() + boostAmount > 1)
    _country->setCapitalSafety(1);
  else
    _country->setCapitalSafety(_country->getCapitalSafety() + boostAmount);

  if (_country->getPoliticalStability() + boostAmount > 1)
    _country->setPoliticalStability(1);
  else
    _country->setPoliticalStability(_country->getPoliticalStability() + boostAmount);

  if (_country->getTradeRouteSafety() + boostAmount > 1)
    _country->setTradeRouteSafety(1);
  else
    _country->setTradeRouteSafety(_country->getTradeRouteSafety() + boostAmount);

  if (_country->getDomesticMorale() + boostAmount > 1)
    _country->setDomesticMorale(1);
  else
    _country->setDomesticMorale(_country->getDomesticMorale() + boostAmount);

  if (_country->getWarSentiment() + boostAmount > 1)
    _country->setWarSentiment(1);
  else
    _country->setWarSentiment(_country->getWarSentiment() + boostAmount);

  _country->getState()->getMilitaryState()->updateNumBattalions(5, true);
  _country->getState()->getMilitaryState()->updateNumPlanes(5, true);
  _country->getState()->getMilitaryState()->updateNumShips(5, true);
  _country->getState()->getMilitaryState()->updateNumTanks(5, true);
  _country->getState()->getMilitaryState()->updateNumTroops(50000, true);
}

///////////////////////////////////////////////////////////
// setStrategy()
///////////////////////////////////////////////////////////

void Country::setStrategy()
{
  if (strategy != NULL)
    delete strategy;

  if (StageContext::getInstance()->getWarStage() == 0)
    strategy = new EarlyStrategy();
  if (StageContext::getInstance()->getWarStage() == 1)
    strategy = new MiddleStrategy();
  if (StageContext::getInstance()->getWarStage() == 2)
    strategy = new LateStrategy();
}

///////////////////////////////////////////////////////////
// getState()
///////////////////////////////////////////////////////////

CountryState *Country::getState()
{
  return countryState;
}

///////////////////////////////////////////////////////////
// getNumCitizens()
///////////////////////////////////////////////////////////

int Country::getNumCitizens()
{
  return countryState->numCitizens;
}

///////////////////////////////////////////////////////////
// setNumCitizens(int)
///////////////////////////////////////////////////////////

void Country::setNumCitizens(int _numCitizens)
{
  countryState->numCitizens = _numCitizens;
}

///////////////////////////////////////////////////////////
// getCountryRating(Country*, double*)
///////////////////////////////////////////////////////////

void Country::getCountryRating(Country *b, double *strengthRatings)
{
  std::vector<double> *strengthScoresA = new std::vector<double>();
  std::vector<double> *strengthScoresB = new std::vector<double>();
  std::vector<double> *aspectScores = new std::vector<double>();

  compareMilitary(this, b, aspectScores); // get CountryA's military strength scores
  for (int i = 0; i < aspectScores->size(); i++)
    strengthScoresA->push_back(aspectScores->at(i));

  aspectScores->clear();

  compareMilitary(b, this, aspectScores); // get CountryB's military strength scores
  for (int i = 0; i < aspectScores->size(); i++)
    strengthScoresB->push_back(aspectScores->at(i));

  aspectScores->clear();

  compareDomestic(this, b, aspectScores); // get CountryA's domestic strength scores
  for (int i = 0; i < aspectScores->size(); i++)
    strengthScoresA->push_back(aspectScores->at(i));

  aspectScores->clear();

  compareDomestic(b, this, aspectScores); // get CountryB's domestic strength scores
  for (int i = 0; i < aspectScores->size(); i++)
    strengthScoresB->push_back(aspectScores->at(i));

  double strengthA = 0.0;
  for (int i = 0; i < strengthScoresA->size(); i++)
    strengthA += strengthScoresA->at(i);
  if (strengthA != 0)
    strengthA /= strengthScoresA->size(); // CountryA's overall strength

  double strengthB = 0.0;
  for (int i = 0; i < strengthScoresB->size(); i++)
    strengthB += strengthScoresB->at(i);
  if (strengthB != 0)
    strengthB /= strengthScoresB->size(); // CountryB's overall strength

  strengthRatings[0] = strengthA;
  strengthRatings[1] = strengthB;

  delete strengthScoresA;
  delete strengthScoresB;
  delete aspectScores;
}

///////////////////////////////////////////////////////////
// compareMilitary(Country*, Country*, std::vector<double>*)
///////////////////////////////////////////////////////////

void Country::compareMilitary(Country *a, Country *b, std::vector<double> *aspectScores)
{
  MilitaryState *mA = a->countryState->militaryState;
  MilitaryState *mB = b->countryState->militaryState;
  aspectScores->push_back(compareAspect(mA->getNumTroops(), mB->getNumTroops()));
  aspectScores->push_back(compareAspect(mA->getNumTanks(), mB->getNumTanks()));
  aspectScores->push_back(compareAspect(mA->getNumPlanes(), mB->getNumPlanes()));
  aspectScores->push_back(compareAspect(mA->getNumShips(), mB->getNumShips()));
}

///////////////////////////////////////////////////////////
// compareDomestic(Country*, Country*, std::vector<double>*)
///////////////////////////////////////////////////////////

void Country::compareDomestic(Country *a, Country *b, std::vector<double> *aspectScores)
{
  (*aspectScores).push_back(compareAspect(a->getPoliticalStability(), b->getPoliticalStability()));
  (*aspectScores).push_back(compareAspect(a->getDomesticMorale(), b->getDomesticMorale()));
  (*aspectScores).push_back(compareAspect(a->getBorderStrength(), b->getBorderStrength()));
  (*aspectScores).push_back(compareAspect(a->getCapitalSafety(), b->getCapitalSafety()));
  (*aspectScores).push_back(compareAspect(a->getWarSentiment(), b->getWarSentiment()));
  (*aspectScores).push_back(compareAspect(a->getTradeRouteSafety(), b->getTradeRouteSafety()));
}

///////////////////////////////////////////////////////////
// compareAspect(int, int)
///////////////////////////////////////////////////////////

double Country::compareAspect(int countryA, int countryB)
{
  if (countryB == 0)
    return 0;
  return countryA / countryB;
}

///////////////////////////////////////////////////////////
// compareAspect(double, double)
///////////////////////////////////////////////////////////

double Country::compareAspect(double countryA, double countryB)
{
  if (countryB == 0)
    return 0;
  return countryA / countryB;
}

///////////////////////////////////////////////////////////
// getPoliticalStability()
///////////////////////////////////////////////////////////

double Country::getPoliticalStability()
{
  return countryState->politicalStability;
}

///////////////////////////////////////////////////////////
// setPoliticalStability(double)
///////////////////////////////////////////////////////////

void Country::setPoliticalStability(double _politicalStability)
{
  if (_politicalStability < 0)
  {
    throw std::invalid_argument("_politicalStability must be greater than 0");
    return;
  }
  countryState->politicalStability = _politicalStability;
}

///////////////////////////////////////////////////////////
// getDomesticMorale()
///////////////////////////////////////////////////////////

double Country::getDomesticMorale()
{
  return countryState->domesticMorale;
}

///////////////////////////////////////////////////////////
// setDomesticMorale(double)
///////////////////////////////////////////////////////////

void Country::setDomesticMorale(double _domesticMorale)
{
  if (_domesticMorale < 0)
  {
    throw std::invalid_argument("_domesticMorale must be greater than 0");
    return;
  }
  countryState->domesticMorale = _domesticMorale;
}

///////////////////////////////////////////////////////////
// getSelfReliance()
///////////////////////////////////////////////////////////

double Country::getSelfReliance()
{
  return countryState->selfReliance;
}

///////////////////////////////////////////////////////////
// setSelfReliance(double)
///////////////////////////////////////////////////////////

void Country::setSelfReliance(double _selfReliance)
{
  if (_selfReliance < 0)
  {
    throw std::invalid_argument("_selfReliance must be greater than 0");
    return;
  }
  countryState->selfReliance = _selfReliance;
}

///////////////////////////////////////////////////////////
// getBorderStrength()
///////////////////////////////////////////////////////////

double Country::getBorderStrength()
{
  return countryState->borderStrength;
}

///////////////////////////////////////////////////////////
// setBorderStrength(double)
///////////////////////////////////////////////////////////

void Country::setBorderStrength(double _borderStrength)
{
  if (_borderStrength < 0)
  {
    throw std::invalid_argument("_borderStrength must be greater than 0");
    return;
  }
  countryState->borderStrength = _borderStrength;
}

///////////////////////////////////////////////////////////
// getCapitalSafety()
///////////////////////////////////////////////////////////

double Country::getCapitalSafety()
{
  return countryState->capitalSafety;
}

///////////////////////////////////////////////////////////
// setCapitalSafety(double)
///////////////////////////////////////////////////////////

void Country::setCapitalSafety(double _capitalSafety)
{
  if (_capitalSafety < 0)
  {
    throw std::invalid_argument("_capitalSafety must be greater than 0");
    return;
  }
  countryState->capitalSafety = _capitalSafety;
}

///////////////////////////////////////////////////////////
// getWarSentiment()
///////////////////////////////////////////////////////////

double Country::getWarSentiment()
{
  return countryState->warSentiment;
}

///////////////////////////////////////////////////////////
// setWarSentiment(double)
///////////////////////////////////////////////////////////

void Country::setWarSentiment(double _warSentiment)
{
  if (_warSentiment < 0)
  {
    throw std::invalid_argument("_warSentiment must be greater than 0");
    return;
  }
  countryState->warSentiment = _warSentiment;
}

///////////////////////////////////////////////////////////
// getTradeRouteSafety()
///////////////////////////////////////////////////////////

double Country::getTradeRouteSafety()
{
  return countryState->tradeRouteSafety;
}

///////////////////////////////////////////////////////////
// setTradeRouteSafety(double)
///////////////////////////////////////////////////////////

void Country::setTradeRouteSafety(double _tradeRouteSafety)
{
  if (_tradeRouteSafety < 0)
  {
    throw std::invalid_argument("_tradeRouteSafety must be greater than 0");
    return;
  }
  countryState->tradeRouteSafety = _tradeRouteSafety;
}

///////////////////////////////////////////////////////////
// getCountryState()
///////////////////////////////////////////////////////////

CountryState *Country::getCountryState()
{
  return countryState;
}

///////////////////////////////////////////////////////////
// setCountryState(CountryState*)
///////////////////////////////////////////////////////////

void Country::setCountryState(CountryState *_countryState)
{
  if (countryState != NULL)
    delete countryState;
  countryState = _countryState;
}

///////////////////////////////////////////////////////////
// getName()
///////////////////////////////////////////////////////////

std::string Country::getName()
{
  return countryState->name;
}

///////////////////////////////////////////////////////////
// setName(std::string)
///////////////////////////////////////////////////////////

void Country::setName(std::string _name)
{
  countryState->name = _name;
}

///////////////////////////////////////////////////////////
// getMilitary()
///////////////////////////////////////////////////////////

Military *Country::getMilitary()
{
  return military;
}

///////////////////////////////////////////////////////////
// getCapital()
///////////////////////////////////////////////////////////

Location *Country::getCapital()
{
  return countryState->capital;
}

///////////////////////////////////////////////////////////
// setCapital(Location*)
///////////////////////////////////////////////////////////

void Country::setCapital(Location *_capital)
{
  countryState->capital = _capital;
}

///////////////////////////////////////////////////////////
// getLocations()
///////////////////////////////////////////////////////////

std::vector<Location *> *Country::getLocations()
{
  return countryState->locations;
}

///////////////////////////////////////////////////////////
// setLocations(std::vector<Location*>*)
///////////////////////////////////////////////////////////

void Country::setLocations(std::vector<Location *> *_locations)
{
  countryState->locations = _locations;
  for (int i = 0; i < countryState->locations->size(); i++)
    if (!countryState->locations->at(i)->getIsCapital())
      countryState->locations->at(i)->setColor(countryState->color); // Set location to be the same color as country
    else
      countryState->locations->at(i)->setColor("\x1B[40m"); // Set capital color to black
}

///////////////////////////////////////////////////////////
// setColor(std::string)
///////////////////////////////////////////////////////////

void Country::setColor(std::string _color)
{
  countryState->color = _color;
}

///////////////////////////////////////////////////////////
// getColor()
///////////////////////////////////////////////////////////

std::string Country::getColor()
{
  return countryState->color;
}

///////////////////////////////////////////////////////////
// getEnemies()
///////////////////////////////////////////////////////////

std::vector<Country *> *Country::getEnemies()
{
  return countryState->enemies;
}

///////////////////////////////////////////////////////////
// setEnemies(std::vector<Country *> *)
///////////////////////////////////////////////////////////

void Country::setEnemies(std::vector<Country *> *_enemies)
{
  if (countryState->enemies != NULL)
    delete countryState->enemies;
  countryState->enemies = _enemies;
}

///////////////////////////////////////////////////////////
// getMilitaryState()
///////////////////////////////////////////////////////////

MilitaryState *Country::getMilitaryState()
{
  return countryState->militaryState;
}

///////////////////////////////////////////////////////////
// setMilitaryState(MilitaryState *)
///////////////////////////////////////////////////////////

void Country::setMilitaryState(MilitaryState *_militaryState)
{
  if (countryState->militaryState != NULL)
    delete countryState->militaryState;
  countryState->militaryState = _militaryState;
}

///////////////////////////////////////////////////////////
// setState(CountryState *)
///////////////////////////////////////////////////////////

void Country::setState(CountryState *_state)
{
  if (countryState != NULL)
    delete countryState;
  countryState = _state;
}

///////////////////////////////////////////////////////////
// attachObserver(LocationObserver *)
///////////////////////////////////////////////////////////

void Country::attachObserver(LocationObserver *_lObserver)
{
  if (locationObservers == NULL)
  {
    locationObservers = new std::vector<LocationObserver *>();
  }

  if (_lObserver == NULL)
    throw std::invalid_argument("_lObserver must not be NULL");

  locationObservers->push_back(_lObserver);
}

///////////////////////////////////////////////////////////
// detachObserver(LocationObserver *)
///////////////////////////////////////////////////////////

void Country::detachObserver(LocationObserver *_lObserver)
{
  if (locationObservers == NULL)
  {
    throw std::logic_error("locationObservers was not initialized thus it can not have elements remove from it");
  }
  for (int i = 0; i < locationObservers->size(); i++)
  {
    if (_lObserver == locationObservers->at(i))
      locationObservers->erase(locationObservers->begin() + i);
  }
}

///////////////////////////////////////////////////////////
// printSummary()
///////////////////////////////////////////////////////////

void Country::printSummary()
{
  std::cout << "---------------------------------\n";
  std::cout << "Summary of " << getName() << std::endl;
  std::cout << "---------------------------------\n";
  std::cout << "Population: " << countryState->numCitizens << std::endl;
  std::cout << "Political Stability: " << countryState->politicalStability << std::endl;
  std::cout << "Domestic Morale: " << countryState->domesticMorale << std::endl;
  std::cout << "Self Reliance: " << countryState->selfReliance << std::endl;
  std::cout << "Border Strength: " << countryState->borderStrength << std::endl;
  std::cout << "Capital Safety: " << countryState->capitalSafety << std::endl;
  std::cout << "War Sentiment: " << countryState->warSentiment << std::endl;
  std::cout << "Trade Route Safety: " << countryState->tradeRouteSafety << std::endl;

  MilitaryState *thisMilitary = countryState->getMilitaryState();
  std::cout << "Number Of Battalions " << thisMilitary->getNumBattalions() << std::endl;
  std::cout << "Number of Tanks: " << thisMilitary->getNumTanks() << std::endl;
  std::cout << "Number Of Ships: " << thisMilitary->getNumShips() << std::endl;
  std::cout << "Number Of Planes: " << thisMilitary->getNumPlanes() << std::endl;
  std::cout << "---------------------------------\n";
}

///////////////////////////////////////////////////////////
// resetLocations(Map*)
///////////////////////////////////////////////////////////

void Country::resetLocations(Map *_map)
{
  std::vector<Location *> *locs = new std::vector<Location *>();
  for (int i = 0; i < countryState->locations->size(); i++)
  {
    Location *l = _map->getLocation(countryState->locations->at(i)->getX(), countryState->locations->at(i)->getY());
    l->setIsCapital(countryState->locations->at(i)->getIsCapital());
    l->setOwnedBy(this);
    locs->push_back(l);
    delete countryState->locations->at(i);
  }

  delete countryState->locations;
  countryState->locations = locs;
}

///////////////////////////////////////////////////////////
// clone()
///////////////////////////////////////////////////////////

Country *Country::clone()
{
  Country *c = new Country(this->getName());
  c->setBorderStrength(getBorderStrength());
  c->setCapitalSafety(getCapitalSafety());
  c->setColor(getColor());
  c->setDomesticMorale(getDomesticMorale());
  c->setNumCitizens(getNumCitizens());
  c->setPoliticalStability(getPoliticalStability());
  c->setSelfReliance(getSelfReliance());
  c->setTradeRouteSafety(getTradeRouteSafety());
  c->setWarSentiment(getWarSentiment());
  c->getState()->enemies = NULL;

  return c;
}

///////////////////////////////////////////////////////////
// resetEnemies(std::vector<Country *> *)
///////////////////////////////////////////////////////////

void Country::resetEnemies(std::vector<Country *> *_enemies)
{
  std::vector<Country *> *newEnemies = new std::vector<Country *>();

  for (int i = 0; i < _enemies->size(); i++)
  {
    newEnemies->push_back(_enemies->at(i));
  }

  for (int i = 0; i < getState()->enemies->size(); i++)
  {
    delete getState()->enemies->at(i);
  }

  delete getState()->enemies;
  getState()->enemies = newEnemies;
}

///////////////////////////////////////////////////////////
// removeEnemy(Country *)
///////////////////////////////////////////////////////////

void Country::removeEnemy(Country *_enemy)
{
  if (countryState->enemies != NULL)
  {
    for (int i = 0; i < countryState->enemies->size(); i++)
    {
      if (countryState->enemies->at(i) == _enemy)
      {
        countryState->enemies->erase(countryState->enemies->begin() + i);
        return;
      }
    }
    throw std::out_of_range("Country not on enemy list");
  }
}

///////////////////////////////////////////////////////////
// setColorOfDestroyed(std::string)
///////////////////////////////////////////////////////////

void Country::setColorOfDestroyedBy(std::string _newColorOfDestroyedBy)
{
  colorOfDestroyedBy = _newColorOfDestroyedBy;
}
