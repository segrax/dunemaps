#include "..\stdafx.h"
#include "house.h"
#include "scenario.h"
#include "mapGenerator.h"
#include "map.h"
#include "..\screenPlayfield.h"
#include "objects\object.h"
#include "objects\mapCell.h"

vector<string> splitStr( string pStr ) {
	vector<string> res;
	size_t prevPos = 0, prevPos2;//, num;
	string	value;

	for(  prevPos2 = 0; prevPos2 < pStr.size(); ) {
		prevPos2 = pStr.find(",", prevPos );
		if(prevPos2 == string::npos)
			prevPos2 = pStr.size();

		//num = atoi( pStr.substr(prevPos, prevPos2 - prevPos ).c_str());
		value = pStr.substr(prevPos, prevPos2 - prevPos ).c_str();

		prevPos = prevPos2+1;
		res.push_back( value );

	}

	return res;
}

cScenario::cScenario(  )  {
	_mapGenerator = 0;
	_map = 0;
}


cScenario::~cScenario() {

	delete _map;
	delete _mapGenerator;
}

void cScenario::missionLoad( size_t pScenNumber ) {
	if( pScenNumber < 1 )
		pScenNumber = 1;

	scenarioBegin( pScenNumber );
}

void cScenario::scenarioMapPrepare() {

	_map		  = new cMap( );

	mapLoad();

	// Load the prebuilt units
	teamsLoad();
	unitsLoad();
	structuresLoad();
	
	g_DuneEngine->houseMapPrepare();
}

void cScenario::mapLoad() {
	vector<string> fields = splitStr( _mapField );
	vector<string> blooms = splitStr( _mapBloom );
	vector<string>::iterator	it;

	delete _mapGenerator;

	_mapGenerator = new cMapGenerator( );

	// Generate the map
	_mapGenerator->generate( _mapSeed );

	// Now create the map cells using the tileid array
	_map->mapLoad();

	// Add spice blooms
	for( it = blooms.begin(); it != blooms.end(); ++it ) {
		cMapCell **mapCell = g_DuneEngine->scenarioGet()->mapGet()->mapCellGet( atoi((*it).c_str()) );

		(*mapCell)->tileSetCurrent( 0, g_DuneEngine->resourcesGet()->tileBloom() );
	}

	// Add spice fields
	for( it = fields.begin(); it != fields.end(); ++it ) {
		cMapCell **mapCell = g_DuneEngine->scenarioGet()->mapGet()->mapCellGet( atoi((*it).c_str()) );

		_map->mapRetile( (*mapCell)->mapIndexGet(), 5 );
	}
}

bool cScenario::scenarioBegin( size_t pScenNumber ) {
	sHouseData		*houseData;
	stringstream	 missionScenarioIni;

	houseData = g_DuneEngine->resourcesGet()->houseGet( g_DuneEngine->missionHouseGet() );
	
	// Generate scenario filename
	missionScenarioIni << "SCEN" << houseData->houseLetter;
	missionScenarioIni << setfill('0') << setw(3) << pScenNumber;
	missionScenarioIni << ".INI";

	scenarioLoad( missionScenarioIni.str() );

	return true;
}

void cScenario::scenarioLoad( string pFilename ) {
	
	// Load the ini into the resource manager
	g_DuneEngine->resourcesGet()->IniLoad( pFilename );


	// Read the map details
	_mapScale	= g_DuneEngine->resourcesGet()->IniNumGet("BASIC", "MapScale", 0);
	_mapSeed	= g_DuneEngine->resourcesGet()->IniNumGet("MAP", "Seed", 0);

	_mapField	= g_DuneEngine->resourcesGet()->IniStringGet("MAP", "Field", "");
	_mapBloom	= g_DuneEngine->resourcesGet()->IniStringGet("MAP", "Bloom", "");
	_mapSpecial	= g_DuneEngine->resourcesGet()->IniStringGet("MAP", "Special", "");
	
	_mapCursor  = g_DuneEngine->resourcesGet()->IniNumGet("BASIC", "CursorPos", 0);
	_mapTactical = g_DuneEngine->resourcesGet()->IniNumGet("BASIC", "TacticalPos", 0);

	houseLoad();

	scenarioMapPrepare();
}

void cScenario::structuresLoad() {
	string tmp;
	vector<string> structDetails;

	// Load the UNITS Section
	g_DuneEngine->resourcesGet()->IniSectionOpen("STRUCTURES");

	for(;;) {
		word	mapIndex = 0, healthPercent = 100;
		tmp = g_DuneEngine->resourcesGet()->IniSectionNext("STRUCTURES");

		// Read map location
		//   For 'GEN3529=Ordos,Concrete'
		if(tmp.substr(0,3) == "GEN")
			mapIndex = atoi ( tmp.substr(3).c_str() );
		
		tmp = g_DuneEngine->resourcesGet()->IniStringGet("STRUCTURES", tmp, "");
		
		// No more lines?
		if(!tmp.size())
			break;

		structDetails = splitStr( tmp );
		if(!structDetails.size())
			break;

		// Atreides,Const Yard,256,1630
		eHouse	house = (eHouse) g_DuneEngine->resourcesGet()->houseFind( structDetails[0].c_str() );	
		word	type = g_DuneEngine->resourcesGet()->structureFind( structDetails[1] );
		
		// if index is defined, we dont have health
		if(!mapIndex) {
			healthPercent  = atoi( structDetails[2].c_str() );
			mapIndex = atoi( structDetails[3].c_str() );
		}

		cHouse *House = g_DuneEngine->houseGet( house );

		cStructure *structure = 0;
		if(House)
			structure = House->structureCreate( type, healthPercent, mapIndex );
	}

	g_DuneEngine->resourcesGet()->IniSectionClose("STRUCTURES");
}

void cScenario::unitsLoad() {
	string tmp;
	vector<string> unitDetails;

	// Load the UNITS Section
	g_DuneEngine->resourcesGet()->IniSectionOpen("UNITS");

	for(;;) {
		tmp = g_DuneEngine->resourcesGet()->IniSectionNext("UNITS");
		tmp = g_DuneEngine->resourcesGet()->IniStringGet("UNITS", tmp, "");
		
		// No more lines?
		if(!tmp.size())
			break;
		//  	house name  hp  map  turnamount
		// "Fremen,Sandworm,256,3800,64,Area Guard"
		unitDetails = splitStr( tmp );
		if(!unitDetails.size())
			break;

		eHouse	house = (eHouse) g_DuneEngine->resourcesGet()->houseFind( unitDetails[0].c_str() );	
		word	type = g_DuneEngine->resourcesGet()->unitFind( unitDetails[1] );
		
		word	healthP  = atoi( unitDetails[2].c_str() );
		word	mapIndex = atoi( unitDetails[3].c_str() );
		byte	angle	 = atoi( unitDetails[4].c_str() );
		byte	action   = g_DuneEngine->resourcesGet()->actionFind( unitDetails[5].c_str() );
		
		cHouse *House = g_DuneEngine->houseGet( house );

		cUnit *unit = 0;
		if(House)
			unit = House->unitCreate( type, healthP, mapIndex, angle, action );

		
	}

	g_DuneEngine->resourcesGet()->IniSectionClose("UNITS");
}

void cScenario::houseLoad() {
	
	for( size_t house = eHouse_Harkonnen; house != eHouse_End; ++house ) {
		
		sHouseData *houseData = g_DuneEngine->resourcesGet()->houseGet( (eHouse) house );
		
		cHouse *House = g_DuneEngine->houseGet( (eHouse ) house );
		
		House->creditQuotaSet( g_DuneEngine->resourcesGet()->IniNumGet( houseData->houseName, "Quota", 0 ) );
		House->creditSet( g_DuneEngine->resourcesGet()->IniNumGet( houseData->houseName, "Credits", 0 ) );
		
		string brain = g_DuneEngine->resourcesGet()->IniStringGet( houseData->houseName, "Brain", "" );
		size_t maxUnits = g_DuneEngine->resourcesGet()->IniNumGet( houseData->houseName, "MaxUnit", 0 );

		if(brain == "Human")
			g_DuneEngine->missionHouseSet( (eHouse) house );

		House->brainSet( brain );
		House->maxUnitSet( maxUnits );
	}

}

void cScenario::teamsLoad() {
	string		   tmp;
	vector<string> teamDetails;

	// Load the STRUCTURES Section
	g_DuneEngine->resourcesGet()->IniSectionOpen("TEAMS");

	for(;;) {
		tmp = g_DuneEngine->resourcesGet()->IniSectionNext("TEAMS");
	
		tmp = g_DuneEngine->resourcesGet()->IniStringGet("TEAMS", tmp, "");
		
		// No more lines?
		if(!tmp.size())
			break;

		teamDetails = splitStr( tmp );
		if(!teamDetails.size())
			break;

		eHouse	house = (eHouse) g_DuneEngine->resourcesGet()->houseFind( teamDetails[0].c_str() );
		size_t	aiMode = g_DuneEngine->resourcesGet()->aiModeFind( teamDetails[1].c_str() );
		size_t	movementType = g_DuneEngine->resourcesGet()->movementNamesGet( teamDetails[2] );
		size_t	unk1 = atoi( teamDetails[3].c_str() );
		size_t  unitsMax	 = atoi( teamDetails[4].c_str() );

		cHouse *House = g_DuneEngine->houseGet( house ) ;

		cTeam *team = House->teamCreate( aiMode, movementType, unk1, unitsMax );
		
	}

	g_DuneEngine->resourcesGet()->IniSectionClose("TEAMS");
}
