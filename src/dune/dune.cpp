#include "stdafx.h"
#include "engine/objects/object.h"
#include "engine/house.h"
#include "engine/scenario.h"
#include "engine/scenarioAmiga.h"
#include "screenPlayfield.h"
#include "minimap.h"

cDune *g_DuneEngine = 0;

cDune::cDune( string pDataPath ) {
	
	screenTilesMaxXSet( 320 );
	screenTilesMaxYSet( 200 );

	_resources = new cResources( pDataPath );
	_screenPlayfield = 0;

	_scenario = 0;
	_minimap = new cMinimap();

	mPlaceObject = 0;
}

cDune::~cDune() {
	map< eHouse, cHouse* >::iterator	 houseIT;
	
	delete _scenario;

	delete mPlaceObject;
	delete _resources;

	delete _screenPlayfield;
	delete _minimap;

	for( houseIT = _houses.begin(); houseIT != _houses.end(); ++houseIT )
		delete houseIT->second;
}

cHouse *cDune::houseGet( eHouse pHouse ) {
	cHouse								*house = 0;
	map< eHouse, cHouse* >::iterator	 houseIT = _houses.find( pHouse );
	
	if(houseIT == _houses.end() ) {
		house = new cHouse( pHouse );
		_houses.insert( pair< eHouse, cHouse*> (pHouse, house ) );
	} else
		house = houseIT->second;

	return house;
}

void cDune::houseMapPrepare() {
	map< eHouse, cHouse* >::iterator	 houseIT;

	for( houseIT = _houses.begin(); houseIT != _houses.end(); ++houseIT )
		houseIT->second->mapPrepare();
}

void cDune::scenarioNew( string pSeed ) {

	delete _scenario;
	_scenario = new cScenario();
	_scenario->mapSeedSet( pSeed );

}

void cDune::scenarioLoad( eHouse pHouse, size_t pScenNumber ) {
	sHouseData		*houseData;
	stringstream	 missionScenarioIni;
	
	_missionHouse = pHouse;

	houseData = g_DuneEngine->resourcesGet()->houseGet( pHouse );
	
	// Generate scenario filename
	missionScenarioIni << "SCEN" << houseData->houseLetter;
	missionScenarioIni << setfill('0') << setw(3) << pScenNumber;
	missionScenarioIni << ".INI";

	scenarioLoad( missionScenarioIni.str(), eScenarioLoad::eLoad_PC_Pak );
}

void cDune::scenarioLoad( string pFilename, eScenarioLoad pLoad ) {
	bool local = false;

	houseReset();

	delete _scenario;
	_scenario = 0;

	switch(pLoad) {

		case eScenarioLoad::eLoad_PC:		// From local INI
			local = true;
			break;

		case eScenarioLoad::eLoad_PC_Pak:	// From INI in PAK
			local = false;
			break;

		case eScenarioLoad::eLoad_Amiga:	// From local Amiga Ini
			local = true;
			_scenario = new cScenarioAmiga( );
			break;
	}

	// Not set? standard pc scenario
	if(!_scenario)
		_scenario = new cScenario( );

	// Load the INI
	_scenario->iniLoad( pFilename, local );

	// Prep the playfield
	delete _screenPlayfield;
	_screenPlayfield = new cScreenPlayfield();

	// Prep minimap
	delete _minimap;
	_minimap = new cMinimap();
}

void cDune::houseReset() {
	map< eHouse, cHouse* >::iterator	 houseIT;

	for( houseIT = _houses.begin(); houseIT != _houses.end(); ++houseIT )
		houseIT->second->reset( );
}
