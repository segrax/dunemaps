#include "stdafx.h"
#include "house.h"
#include "scenario.h"
#include "mapGenerator.h"
#include "map.h"
#include "screenPlayfield.h"
#include "objects/object.h"
#include "objects/mapCell.h"
#include "objects/structure.h"
#include "objects/unit.h"
#include "team.h"
#include<fstream>

#include "panelSurface.h"

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

	_mapTimeOut = 0;
	_mapSeed = 0;
	_mapScale = 0;
	_mapTactical = 0;
	_mapCursor = 0;

	_mapLoseFlags = 0;
	_mapWinFlags = 0;

}


cScenario::~cScenario() {
	clear();

	delete _map;
	delete _mapGenerator;
}

void cScenario::scenarioMapPrepare() {

	mapLoad();

	// Load the prebuilt units
	
	teamsLoad();
	unitsLoad();
	structuresLoad();

	choamLoad();
	reinforcementsLoad();

	g_DuneEngine->houseMapPrepare();
}

void cScenario::mapLoad() {
	vector<string> fields = splitStr( _mapField );
	vector<string> blooms = splitStr( _mapBloom );
	vector<string> specials = splitStr( _mapSpecial );

	vector<string>::iterator	it;

	if(!_map)
		_map		  = new cMap( );

	delete _mapGenerator;

	_mapGenerator = new cMapGenerator( );

	if( (short int) _mapSeed < 0 )
		_mapSeed |= 0xFFFF0000;
	else
		_mapSeed &= 0xFFFF;

	// Generate the map
	_mapGenerator->generate( _mapSeed );
	
	// Now create the map cells using the tileid array
	_map->mapLoad();

	// Add spice blooms
	for( it = blooms.begin(); it != blooms.end(); ++it ) {
		if(*it == "0")
			continue;

		cMapCell **mapCell = _map->mapCellGet( atoi((*it).c_str()) );

		(*mapCell)->tileSetCurrent( 0, g_DuneEngine->resourcesGet()->tileBloom() );
	}
	
	// Add special
	for( it = specials.begin(); it != specials.end(); ++it ) {
		if(*it == "0")
			continue;

		cMapCell **mapCell = _map->mapCellGet( atoi((*it).c_str()) );

		(*mapCell)->tileSetCurrent( 0, g_DuneEngine->resourcesGet()->tileBloom() );
	}

	// Add spice fields
	for( it = fields.begin(); it != fields.end(); ++it ) {
		if(*it == "0")
			continue;

		cMapCell **mapCell = _map->mapCellGet( atoi((*it).c_str()) );

		_map->mapRetile( (*mapCell)->mapIndexGet(), 5 );
	}

	if(g_DuneEngine->screenPlayfieldGet())
		g_DuneEngine->screenPlayfieldGet()->scaleSet();
}

void cScenario::mapSeedSet( string pSeed ) {
	
	_mapSeed = atoi(pSeed.c_str());

	g_DuneEngine->screenPlayfieldGet()->redrawSet();

	mapLoad();
}

void cScenario::clear() {
	for( size_t house = eHouse_Harkonnen; house != eHouse_End; ++house ) {

		cHouse *House = g_DuneEngine->houseGet( (eHouse ) house );
		House->reset();
	}

	teamsClear();
	mChoam.clear();
	mReinforcements.clear();
}

void cScenario::iniLoad( string pFilename, bool pLocalFile ) {
	
	// Load the ini into the resource manager
	if(!g_DuneEngine->resourcesGet()->IniLoad( pFilename, pLocalFile ))
		return;

	// Read the win/brief/lose pictures
	_pictureLose	= g_DuneEngine->resourcesGet()->IniStringGet("BASIC", "LosePicture", "");
	_pictureBrief	= g_DuneEngine->resourcesGet()->IniStringGet("BASIC", "BriefPicture", "");
	_pictureWin		= g_DuneEngine->resourcesGet()->IniStringGet("BASIC", "WinPicture", "");

	// Read the map details
	_mapScale	= g_DuneEngine->resourcesGet()->IniNumGet("BASIC", "MapScale", 0);
	_mapSeed	= g_DuneEngine->resourcesGet()->IniNumGet("MAP", "Seed", 0);

	_mapField	= g_DuneEngine->resourcesGet()->IniStringGet("MAP", "Field", "");
	_mapBloom	= g_DuneEngine->resourcesGet()->IniStringGet("MAP", "Bloom", "");
	_mapSpecial	= g_DuneEngine->resourcesGet()->IniStringGet("MAP", "Special", "");
	
	_mapTimeOut = g_DuneEngine->resourcesGet()->IniNumGet("BASIC", "TimeOut", 0);

	_mapCursor  = g_DuneEngine->resourcesGet()->IniNumGet("BASIC", "CursorPos", 0);
	_mapTactical = g_DuneEngine->resourcesGet()->IniNumGet("BASIC", "TacticalPos", 0);

	_mapLoseFlags	= g_DuneEngine->resourcesGet()->IniNumGet("BASIC", "LoseFlags", 0);
	_mapWinFlags	= g_DuneEngine->resourcesGet()->IniNumGet("BASIC", "WinFlags", 0);

	houseLoad();

	scenarioMapPrepare();
}

void cScenario::structuresLoad() {
	string tmp;
	vector<string> structDetails;

	// Load the UNITS Section
	g_DuneEngine->resourcesGet()->IniSectionOpen("STRUCTURES");

	for(;;) {
		word	mapIndex = 0, healthPercent = 256;
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

		teamCreate(teamDetails[0], teamDetails[1], teamDetails[2], teamDetails[3], teamDetails[4]);
		
	}

	g_DuneEngine->resourcesGet()->IniSectionClose("TEAMS");
}

void cScenario::reinforcementsLoad() {
	vector<string>	reinforcementDetails;
	string			tmp;

	mReinforcements.clear();

	// Load the REINFORCEMENT Section
	g_DuneEngine->resourcesGet()->IniSectionOpen("REINFORCEMENTS");

	for(;;) {
		tmp = g_DuneEngine->resourcesGet()->IniSectionNext("REINFORCEMENTS");
		tmp = g_DuneEngine->resourcesGet()->IniStringGet("REINFORCEMENTS", tmp, "");
		
		// No more lines?
		if(!tmp.size())
			break;

		reinforcementDetails = splitStr( tmp );
		if(!reinforcementDetails.size())
			break;

		bool repeat= false;

		if( reinforcementDetails[3].find("+") != string::npos )
			repeat = true;

		reinforcementLoad( reinforcementDetails[0], reinforcementDetails[1], reinforcementDetails[2], reinforcementDetails[3], repeat );

	}

	g_DuneEngine->resourcesGet()->IniSectionClose("REINFORCEMENTS");
}

void cScenario::choamLoad() {

	mChoam.clear();

	for(int i = 0; i < 18; ++i) {
		sChoam choam;
		
		choam.mUnitType = g_DuneEngine->resourcesGet()->unitGet( i )->Name;
		choam.mCount = g_DuneEngine->resourcesGet()->IniNumGet("CHOAM", choam.mUnitType, 0 );

		if(choam.mCount)
			mChoam.push_back( choam );
	}

}

void cScenario::reinforcementsClear() {
	mReinforcements.clear();
}

void cScenario::reinforcementLoad(string pHouse, string pUnitType, string pDirection, string pTimer, bool pRepeat ) {

	eHouse house = (eHouse) g_DuneEngine->resourcesGet()->houseFind( pHouse );
	int unitType = g_DuneEngine->resourcesGet()->unitFind(pUnitType );
	int direction = g_DuneEngine->resourcesGet()->directionGet( pDirection );
	int time = atoi(pTimer.c_str());

	sReinforcement reinforce;
	reinforce.mHouse = house;
	reinforce.mDirection = direction;
	reinforce.mTime = time;
	reinforce.mUnitType = unitType;

	reinforce.mRepeat = pRepeat;

	mReinforcements.push_back( reinforce );
}

void cScenario::teamCreate(string pHouseName, string pAiMode, string pMovementType, string pUnitsMin, string pUnitsMax ) {
	eHouse	house = (eHouse) g_DuneEngine->resourcesGet()->houseFind( pHouseName.c_str() );
	size_t	aiMode = g_DuneEngine->resourcesGet()->aiModeFind( pAiMode.c_str() );
	size_t	movementType = g_DuneEngine->resourcesGet()->movementNamesGet( pMovementType );
	size_t	unk1 = atoi( pUnitsMin.c_str() );
	size_t  unitsMax	 = atoi( pUnitsMax.c_str() );

	cHouse *House = g_DuneEngine->houseGet( house ) ;

	mTeams.push_back( new cTeam(House, aiMode, movementType, unk1, unitsMax ));
}

void cScenario::teamsClear() {
	vector< cTeam* >::iterator					teamIT;
		
	for( teamIT = mTeams.begin(); teamIT != mTeams.end(); ++teamIT )
		delete (*teamIT);

	mTeams.clear();
}

void cScenario::iniSave( string pFile ) {
	unsigned char *buffer =  new unsigned char[1];

	IniFile ini( buffer, 0 );
	delete buffer;

	// [BASIC] Section
	ini.setStringValue("BASIC", "LosePicture", _pictureLose );
	ini.setStringValue("BASIC", "WinPicture", _pictureWin );
	ini.setStringValue("BASIC", "BriefPicture", _pictureBrief );
	ini.setIntValue("BASIC", "TimeOut", _mapTimeOut );
	ini.setIntValue("BASIC", "MapScale", _mapScale );
	ini.setIntValue("BASIC", "CursorPos", mapCursorGet() );
	ini.setIntValue("BASIC", "TacticalPos", mapTacticalGet() );
	ini.setIntValue("BASIC", "LoseFlags", _mapLoseFlags  );
	ini.setIntValue("BASIC", "WinFlags", _mapWinFlags  );

	// [MAP] Section
	ini.setStringValue("MAP", "Field", _mapField );
	ini.setStringValue("MAP", "Bloom", _mapBloom );
	ini.setStringValue("MAP", "Special", _mapSpecial );

	ini.setIntValue("MAP", "Seed", _mapSeed );

	// [HOUSENAME] Section
	for( int i = eHouse_Harkonnen; i != eHouse_End; ++i ) {
		cHouse *house = g_DuneEngine->houseGet( (eHouse) i );
		string houseName = house->houseDataGet()->houseName;

		if(house->maxUnitGet() == 0)
			continue;

		ini.setIntValue(houseName, "Quota", house->creditQuotaGet() );
		ini.setIntValue(houseName, "Credits", house->creditGet() );
		ini.setStringValue(houseName, "Brain", house->brainGet() );
		ini.setIntValue(houseName, "MaxUnit", house->maxUnitGet() );
	}

	// [CHOAM] Section
	vector< sChoam >::iterator		choamIT;

	for( choamIT = mChoam.begin(); choamIT != mChoam.end(); ++choamIT ) {
		stringstream	output;

		output << choamIT->mCount;
		ini.setStringValue("CHOAM", choamIT->mUnitType, output.str() );
	}

	// [TEAMS] Section
	vector< cTeam* >::iterator		teamIT;
	int count = 1;

	for(  teamIT = mTeams.begin(); teamIT != mTeams.end(); ++teamIT, ++count ) {

		stringstream	output, counter;

		output << (*teamIT)->houseGet()->houseDataGet()->houseName;
		output << ",";
		output << g_DuneEngine->resourcesGet()->aiModeGet((*teamIT)->aiModeGet());
		output << ",";
		output << g_DuneEngine->resourcesGet()->movementNameGet( (*teamIT)->movementTypeGet() );
		output << ",";
		output << (*teamIT)->unk1Get();
		output << ",";
		output << (*teamIT)->unitsMaxGet();

		counter << count;

		ini.setStringValue("TEAMS", counter.str(), output.str() );
	}

	multimap< size_t, cUnit* >::iterator	unitIT;
	multimap< size_t, cStructure*>::iterator structIT;


	// [UNITS] Section
	int id = 0;
	for( int i = eHouse_Harkonnen; i != eHouse_End; ++i ) {
		cHouse *house = g_DuneEngine->houseGet( (eHouse) i );
		string houseName = house->houseDataGet()->houseName;

		multimap< size_t, cUnit* >		*units = house->unitsGet();
		multimap< size_t, cStructure*>  *structs = house->structsGet();

		for( structIT = structs->begin(); structIT != structs->end(); ++structIT ) {
			stringstream output;
			stringstream counter;

			output << houseName;
			output << ",";
			output << structIT->second->dataGet()->Name;
			if( structIT->second->dataGet()->typeIndex != 0 && structIT->second->dataGet()->typeIndex != 1 &&
				structIT->second->dataGet()->typeIndex != 14 ) {
				output << ",";
				output << structIT->second->healthGet();
				output << ",";
				output << structIT->second->mapIndexGet();
				counter << "ID";
				counter << setw(3) << setfill('0') << id;
				++id;

			} else {
				counter << "GEN";
				counter << structIT->second->mapIndexGet();
			}

			ini.setStringValue("STRUCTURES", counter.str(), output.str() );
		}

		for( unitIT = units->begin(); unitIT != units->end(); ++unitIT ) {
			stringstream output;
			stringstream counter;

			output << houseName;
			output << ",";
			output << unitIT->second->dataGet()->Name;
			output << ",";
			output << unitIT->second->healthGet();
			output << ",";
			output << unitIT->second->mapIndexGet();
			output << ",";
			output << (int) unitIT->second->angleBaseGet()->_Current;
			output << ",";
			output << g_DuneEngine->resourcesGet()->actionGet( unitIT->second->actionGet() )->Name;
			
			counter << "ID";
			counter << setw(3) << setfill('0') << id;
			++id;

			ini.setStringValue("UNITS", counter.str(), output.str() );
		}
	}

	vector<sReinforcement>::iterator reIT;

	id = 1;

	// [REINFORCEMENTS] Section
	for( reIT = mReinforcements.begin(); reIT != mReinforcements.end(); ++reIT ) {
			stringstream output;
			stringstream counter;
			
			cHouse *house = g_DuneEngine->houseGet( reIT->mHouse );
			sUnitData *unit = g_DuneEngine->resourcesGet()->unitGet( reIT->mUnitType );

			output << house->houseDataGet()->houseName;
			output << ",";

			output <<  unit->Name;
			output << ",";

			output << g_DuneEngine->resourcesGet()->directionGet( reIT->mDirection );
			output << ",";

			output << reIT->mTime;
			if( reIT->mRepeat)
				output << "+";
			
			counter << id;
			++id;

			ini.setStringValue("REINFORCEMENTS", counter.str(), output.str() );
	}


	ini.SaveChangesTo( pFile );
}
