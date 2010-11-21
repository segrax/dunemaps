#include "stdafx.h"
#include "engine/objects/object.h"
#include "engine/house.h"
#include "engine/scenario.h"
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

	for( houseIT = _houses.begin(); houseIT != _houses.end(); ++houseIT )
		delete houseIT->second;

	delete mPlaceObject;
	delete _resources;
	delete _scenario;
	delete _screenPlayfield;
	delete _minimap;
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

void cDune::scenarioLoad( eHouse pHouse, size_t pScenNumber ) {
	houseReset();
	_missionHouse = pHouse;

	delete _scenario;
	
	_scenario = new cScenario( );
	_scenario->missionLoad( pScenNumber );

	delete _screenPlayfield;
	_screenPlayfield = new cScreenPlayfield();

	delete _minimap;
	_minimap = new cMinimap();
}

void cDune::scenarioLoad( string pFilename ) {
	houseReset();
	delete _scenario;

	_scenario = new cScenario( );
	_scenario->scenarioLoad( pFilename, false );

	delete _screenPlayfield;
	_screenPlayfield = new cScreenPlayfield();

	delete _minimap;
	_minimap = new cMinimap();
}

void cDune::houseReset() {
	map< eHouse, cHouse* >::iterator	 houseIT;

	for( houseIT = _houses.begin(); houseIT != _houses.end(); ++houseIT )
		houseIT->second->reset( );
}
