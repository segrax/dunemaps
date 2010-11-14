#include "..\stdafx.h"
#include "house.h"
#include "scenario.h"
#include "mapGenerator.h"
#include "map.h"
#include "..\screenPlayfield.h"

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

}


cScenario::~cScenario() {

	delete _map;
}

void cScenario::missionLoad( size_t pScenNumber ) {
	if( pScenNumber < 1 )
		pScenNumber = 1;

	missionLoadIni( pScenNumber );

	vector<string> fields = splitStr( _mapField );
	vector<string> blooms = splitStr( _mapBloom );

	// Add the spice fields/blooms
	_mapGenerator->fieldsAdd( fields );
	_mapGenerator->bloomsAdd( blooms );

	// Genereate the map
	_mapGenerator->generate( _mapSeed );

	// Now create the map cells using the tileid array
	_map->mapLoad();

	// Load the prebuilt units
	unitsLoad();
	structuresLoad();

	g_DuneEngine->houseMapPrepare();
}

bool cScenario::missionLoadIni( size_t pScenNumber ) {
	sHouseData		*houseData;
	stringstream	 missionScenarioIni;

	houseData = g_DuneEngine->resourcesGet()->houseGet( g_DuneEngine->missionHouseGet() );
	
	// Generate scenario filename
	missionScenarioIni << "SCEN" << houseData->houseLetter;
	missionScenarioIni << setfill('0') << setw(3) << pScenNumber;
	missionScenarioIni << ".INI";

	// Load the ini into the resource manager
	g_DuneEngine->resourcesGet()->IniLoad( missionScenarioIni.str() );

	// Read the map details
	_mapScale	= g_DuneEngine->resourcesGet()->IniNumGet("BASIC", "MapScale", 0);
	_mapSeed	= g_DuneEngine->resourcesGet()->IniNumGet("MAP", "Seed", 0);

	_mapField	= g_DuneEngine->resourcesGet()->IniStringGet("MAP", "Field", "");
	_mapBloom	= g_DuneEngine->resourcesGet()->IniStringGet("MAP", "Bloom", "");
	_mapSpecial	= g_DuneEngine->resourcesGet()->IniStringGet("MAP", "Special", "");
	
	_mapCursor  = g_DuneEngine->resourcesGet()->IniNumGet("BASIC", "CursorPos", 0);
	_mapTactical = g_DuneEngine->resourcesGet()->IniNumGet("BASIC", "TacticalPos", 0);
	
	_mapGenerator = new cMapGenerator( );
	_map		  = new cMap( );

	return true;
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
