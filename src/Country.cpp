/// @author Jake Weatherhead

#include "Country.h"
#include "WarStage.h"
#include "Superpower.h"
#include "Military.h"
#include "MilitaryState.h"
#include "CountryState.h"
#include "CountryMediator.h"
#include "Strategy.h"

///////////////////////////////////////////////////////////
// Country()
///////////////////////////////////////////////////////////

Country::Country() 
{
}

///////////////////////////////////////////////////////////
// request()
///////////////////////////////////////////////////////////

void Country::request()
{
	return;
}

///////////////////////////////////////////////////////////
// notify()
///////////////////////////////////////////////////////////

void Country::notify()
{
	return;
}

///////////////////////////////////////////////////////////
// attach()
///////////////////////////////////////////////////////////

void Country::attach()
{
	return;
}

///////////////////////////////////////////////////////////
// detach()
///////////////////////////////////////////////////////////

void Country::detach()
{
	return;
}

///////////////////////////////////////////////////////////
// takeTurn()
///////////////////////////////////////////////////////////

void Country::takeTurn(Country* countryB)
{
  setStrategy();
  double* strengthRatings = getCountryRating(countryB);
	strategy->takeTurn(strengthRatings);
}

///////////////////////////////////////////////////////////
// setWarStage()
///////////////////////////////////////////////////////////

void Country::setStrategy()
{
  // set strategy to appropriate strategy based on war stage
}

///////////////////////////////////////////////////////////
// getState()
///////////////////////////////////////////////////////////

CountryState* Country::getState()
{
  // generate current state
  // return current state
  return NULL;
}

///////////////////////////////////////////////////////////
// getNumCitizens()
///////////////////////////////////////////////////////////

int Country::getNumCitizens()
{
  return numCitizens;
}

///////////////////////////////////////////////////////////
// setNumCitizens()
///////////////////////////////////////////////////////////

void Country::setNumCitizens(int _numCitizens)
{
  numCitizens = _numCitizens;
}

///////////////////////////////////////////////////////////
// getMilitaryState()
///////////////////////////////////////////////////////////

MilitaryState* Country::getMilitaryState()
{
  return militaryState;
}

///////////////////////////////////////////////////////////
// setMilitaryState()
///////////////////////////////////////////////////////////

void Country::setMilitaryState(MilitaryState* _militaryState)
{
  militaryState = _militaryState;
}

///////////////////////////////////////////////////////////
// getMapState()
///////////////////////////////////////////////////////////

MapState* Country::getMapState()
{
  return mapState;
}

///////////////////////////////////////////////////////////
// setMapState()
///////////////////////////////////////////////////////////

void Country::setMapState(MapState* _mapState)
{
  mapState = _mapState;
}

///////////////////////////////////////////////////////////
// getCountryRating()
///////////////////////////////////////////////////////////

double* Country::getCountryRating(Country* b)
{
  std::vector<double> strengthScoresA;
  std::vector<double> strengthScoresB;
  std::vector<double> aspectScores;

  aspectScores = compareMilitary(this, b);
  for (double score : aspectScores)
    strengthScoresA.push_back(score);

  aspectScores = compareMilitary(b, this);
  for (double score : aspectScores)
    strengthScoresA.push_back(score);

  aspectScores = compareDomestic(this, b);
  for (double score : aspectScores)
    strengthScoresA.push_back(score);

  aspectScores = compareDomestic(b, this);
  for (double score : aspectScores)
    strengthScoresA.push_back(score);

  double strengthA = 0.0;
  for (double score : strengthScoresA)
    strengthA += score;
  strengthA /= strengthScoresA.size();

  double strengthB = 0.0;
  for (double score : strengthScoresB)
    strengthB += score;
  strengthB /= strengthScoresB.size();

  double scores[2];
  scores[0] = strengthA;
  scores[1] = strengthB;
  return scores;
}

///////////////////////////////////////////////////////////
// compareMilitary()
///////////////////////////////////////////////////////////

std::vector<double> Country::compareMilitary(Country* a, Country* b)
{
  std::vector<double> ratingsA;
  MilitaryState* mA = a->getMilitaryState();
  MilitaryState* mB = b->getMilitaryState();
  ratingsA.push_back(compareAspect(mA->getNumTroops(), mB->getNumTroops()));
  ratingsA.push_back(compareAspect(mA->getNumTanks(), mB->getNumTanks()));
  ratingsA.push_back(compareAspect(mA->getNumPlanes(), mB->getNumPlanes()));
  ratingsA.push_back(compareAspect(mA->getNumShips(), mB->getNumShips()));
  return ratingsA;
}

///////////////////////////////////////////////////////////
// compareDomestic(Country)
///////////////////////////////////////////////////////////

std::vector<double> Country::compareDomestic(Country* a, Country* b)
{
  std::vector<double> ratingsA;
  ratingsA.push_back(compareAspect(a->getPoliticalStability(), b->getPoliticalStability()));
  ratingsA.push_back(compareAspect(a->getDomesticMorale(), b->getDomesticMorale()));
  ratingsA.push_back(compareAspect(a->getBorderStrength(), b->getBorderStrength()));
  ratingsA.push_back(compareAspect(a->getCapitalSafety(), b->getCapitalSafety()));
  ratingsA.push_back(compareAspect(a->getWarSentiment(), b->getWarSentiment()));
  ratingsA.push_back(compareAspect(a->getTradeRouteSafety(), b->getTradeRouteSafety()));
  return ratingsA;
}

///////////////////////////////////////////////////////////
// compareAspect(int, int)
///////////////////////////////////////////////////////////

double Country::compareAspect(int countryA, int countryB)
{
  return countryA/countryB;
}

///////////////////////////////////////////////////////////
// compareAspect(double, double)
///////////////////////////////////////////////////////////

double Country::compareAspect(double countryA, double countryB)
{
  return countryA/countryB;
}

///////////////////////////////////////////////////////////
// getPoliticalStability()
///////////////////////////////////////////////////////////

double Country::getPoliticalStability()
{
  return politicalStability;
}

///////////////////////////////////////////////////////////
// setPoliticalStability()
///////////////////////////////////////////////////////////

void Country::setPoliticalStability(double _politicalStability)
{
  if (_politicalStability < 0)
  {
    throw std::invalid_argument("_politicalStability must be greater than 0");
    return;
  }
  politicalStability = _politicalStability;
}

///////////////////////////////////////////////////////////
// getDomesticMorale()
///////////////////////////////////////////////////////////

double Country::getDomesticMorale()
{
  return domesticMorale;
}

///////////////////////////////////////////////////////////
// setDomesticMorale()
///////////////////////////////////////////////////////////

void Country::setDomesticMorale(double _domesticMorale)
{
  if (_domesticMorale < 0)
  {
    throw std::invalid_argument("_domesticMorale must be greater than 0");
    return;
  }
  domesticMorale = _domesticMorale;
}

///////////////////////////////////////////////////////////
// getSelfReliance()
///////////////////////////////////////////////////////////

double Country::getSelfReliance()
{
  return selfReliance;
}

///////////////////////////////////////////////////////////
// setSelfReliance()
///////////////////////////////////////////////////////////

void Country::setSelfReliance(double _selfReliance)
{
  if (_selfReliance < 0)
  {
    throw std::invalid_argument("_selfReliance must be greater than 0");
    return;
  }
  selfReliance = _selfReliance;
}

///////////////////////////////////////////////////////////
// getBorderStrength()
///////////////////////////////////////////////////////////

double Country::getBorderStrength() 
{
  return borderStrength;
}

///////////////////////////////////////////////////////////
// setBorderStrength()
///////////////////////////////////////////////////////////

void Country::setBorderStrength(double _borderStrength)
{
  if (_borderStrength < 0)
  {
    throw std::invalid_argument("_borderStrength must be greater than 0");
    return;
  }
  borderStrength = _borderStrength;
}

///////////////////////////////////////////////////////////
// getCapitalSafety()
///////////////////////////////////////////////////////////

double Country::getCapitalSafety()
{
  return capitalSafety;
}

///////////////////////////////////////////////////////////
// setCapitalSafety()
///////////////////////////////////////////////////////////

void Country::setCapitalSafety(double _capitalSafety)
{
  if (_capitalSafety < 0)
  {
    throw std::invalid_argument("_capitalSafety must be greater than 0");
    return;
  }
  capitalSafety = _capitalSafety;
}

///////////////////////////////////////////////////////////
// getWarSentiment()
///////////////////////////////////////////////////////////

double Country::getWarSentiment()
{
  return warSentiment;
}

///////////////////////////////////////////////////////////
// setWarSentiment()
///////////////////////////////////////////////////////////

void Country::setWarSentiment(double _warSentiment)
{
  if (_warSentiment < 0)
  {
    throw std::invalid_argument("_warSentiment must be greater than 0");
    return;
  }
  warSentiment = _warSentiment;
}

///////////////////////////////////////////////////////////
// getTradeRouteSafety()
///////////////////////////////////////////////////////////

double Country::getTradeRouteSafety()
{
  return tradeRouteSafety;
}

///////////////////////////////////////////////////////////
// setTradeRouteSafety()  
///////////////////////////////////////////////////////////

void Country::setTradeRouteSafety(double _tradeRouteSafety)
{
  if (_tradeRouteSafety < 0)
  {
    throw std::invalid_argument("_tradeRouteSafety must be greater than 0");
    return;
  }
  tradeRouteSafety = _tradeRouteSafety;
}

///////////////////////////////////////////////////////////
// getMilitaryState()
///////////////////////////////////////////////////////////

MilitaryState* Country::getMilitaryState()
{
  return militaryState;
}

///////////////////////////////////////////////////////////
// setMilitaryState()
///////////////////////////////////////////////////////////

void Country::setMilitaryState(MilitaryState* _militaryState)
{
  militaryState = _militaryState;
}

///////////////////////////////////////////////////////////
// getMapState()
///////////////////////////////////////////////////////////

MapState* Country::getMapState()
{
  return mapState;
}

///////////////////////////////////////////////////////////
// setMapState()
///////////////////////////////////////////////////////////

void Country::setMapState(MapState* _mapState)
{
  mapState = _mapState;
}

///////////////////////////////////////////////////////////
// getCountryState()
///////////////////////////////////////////////////////////

CountryState* Country::getCountryState()
{
  return countryState;
}

///////////////////////////////////////////////////////////
// setCountryState()
///////////////////////////////////////////////////////////

void Country::setCountryState(CountryState* _countryState)
{
  countryState = _countryState;
}