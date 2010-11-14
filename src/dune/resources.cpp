#include "stdafx.h"
#include <fstream>

// Lib Eastwood
#include "eastwood/IniFile.h"
#include "eastwood/IcnFile.h"
#include "eastwood/PakFile.h"
#include "eastwood/ShpFile.h"
#include "eastwood/PalFile.h"
#include "eastwood/CpsFile.h"
#include "eastwood/WsaFile.h"

cResources::cResources( string pPath ) {
	if( pPath.size() )
		if( pPath.substr( pPath.size(), 1) != "\\" )
			pPath.append("\\");

	_Exe = 0;
	_icons = 0;
	_ini = 0;
	_fileIconICN = 0;
	_fileIconMAP = 0;
	_shp = 0;
	_paletteIBM = 0;
	_palIBM = 0;

	_DataPath = pPath;

	_tileFogWar = _tileBloom = _tileConcrete = _tileWall = _tileLand = 0;

	angleAdjustBuild();
}

cResources::~cResources() {
	map< eHouse, sHouseData* >::iterator	houseIT;
	map< size_t, sUnitData* >::iterator		unitIT;
	map< size_t, sStructureData*>::iterator structIT;
	map< word, SDL_Surface*>::iterator		tileIT;
	map< size_t, sActionData*>::iterator	actionIT;
	map< string, PakFile*>::iterator		pakIT;

	for( houseIT = _dataHouses.begin(); houseIT != _dataHouses.end(); ++houseIT )
		delete houseIT->second;

	for( structIT = _dataStructures.begin(); structIT != _dataStructures.end(); ++structIT )
		delete structIT->second;

	for( unitIT = _dataUnits.begin(); unitIT != _dataUnits.end(); ++unitIT ) 
		delete unitIT->second;
	
	for( tileIT = _Tiles.begin(); tileIT != _Tiles.end(); ++tileIT )
		SDL_FreeSurface( tileIT->second );

	for( actionIT = _dataActions.begin(); actionIT != _dataActions.end(); ++actionIT )
		delete actionIT->second;

	for( pakIT = _Paks.begin(); pakIT != _Paks.end(); ++pakIT )
		delete pakIT->second;

	for( tileIT = _dataSHP.begin(); tileIT != _dataSHP.end(); ++tileIT )
		SDL_FreeSurface( tileIT->second );

	delete _Exe;
	delete _fileIconICN;
	delete _fileIconMAP;
	delete _icons;
	delete _ini;
	delete _shp;

	delete _paletteIBM;
	if(_palIBM)
		delete _palIBM->colors;
	delete _palIBM;
}

void cResources::resourcePrepare() {
	
	_Exe = new cResourceExe( "Dune2.EXE" );

	pakLoad();

	// Load the main palette
	istream *filePalIBM  = fileOpen( "IBM.PAL" );
	_paletteIBM = new PalFile( *filePalIBM );
	_palIBM = _paletteIBM->getPalette();
	delete filePalIBM;

	// Load the tiles
	_fileIconICN = fileRead( fileOpen( "ICON.ICN" ),  _fileIconICNSize );
	_fileIconMAP = fileRead( fileOpen( "ICON.MAP"  ), _fileIconMAPSize );

	_icons = new IcnFile( _fileIconICN, _fileIconICNSize, _fileIconMAP, _fileIconMAPSize, _paletteIBM->getPalette() );

	mapTileIDsLoad();
	shpLoad();
}

byte *cResources::fileRead( string pFile, size_t	&pFileSize ) {
	stringstream	 filePathFinal;
	ifstream		*fileStream;
	byte			*fileBuffer = 0;

	// Build the file path
	filePathFinal << _DataPath;
	filePathFinal << pFile;

	// Attempt to open the file
	fileStream = new ifstream ( filePathFinal.str().c_str(), ios::binary );
	if(fileStream->is_open() != false) {

		// Get file size
		fileStream->seekg(0, ios::end );
		pFileSize = fileStream->tellg();
		fileStream->seekg( ios::beg );

		// Allocate buffer, and read the file into it
		fileBuffer = new byte[ pFileSize ];
		if(fileStream->read( (char*) fileBuffer, pFileSize ) == false) {
			delete fileBuffer;
			fileBuffer = 0;
		}
	}

	// Close the stream
	fileStream->close();
	delete fileStream;

	// All done ;)
	return fileBuffer;
}

word cResources::actionFind( string pName ) {
	sActionData *action = 0;

	for( size_t i = 0; i < 0x0E; ++i) {
		
		action = actionGet( i );
		if(action->Name == pName)
			return i;
	}

	return 0xFFFF;
}

sActionData *cResources::actionGet( word pIndex ) {
	map< size_t, sActionData* >::iterator actionIT = _dataActions.find( pIndex );

	if( actionIT != _dataActions.end())
		return actionIT->second;

	sExeActionData actionExeData = _Exe->actionGet( pIndex );

	sActionData *actionData = new sActionData();
	actionData->field_0 = actionExeData.field_0;
	actionData->field_1 = actionExeData.field_1;
	actionData->Name	= _Exe->stringGet(actionExeData.Name);
	actionData->field_6 = actionExeData.field_6;
	actionData->field_7 = actionExeData.field_7;
	actionData->sidebarMode = actionExeData.sidebarMode;
	actionData->field_9 = actionExeData.field_9;
	actionData->field_A = actionExeData.field_A;
	actionData->field_B = actionExeData.field_B;
	
	_dataActions.insert( pair< size_t, sActionData*> ( pIndex, actionData ) );
	return actionData;
}

sScriptEmc *cResources::emcGet( string pFilename ) {
	/*< string, sScriptEmc*>::iterator	emcIT;
	
	emcIT = _dataEmc.find( pFilename );
	if(emcIT != _dataEmc.end() )
		return emcIT->second;

	string	  *buffer =	fileRead( pFilename );
	string	   buf = *buffer;
	delete buffer;
	sScriptEmc *emcData = new sScriptEmc( buf );

	_dataEmc.insert( pair< string, sScriptEmc*>( pFilename, emcData ));

	return emcData;*/

	return 0;
}

word cResources::houseFind( string pName ) {
	sHouseData *house = 0;

	for( size_t i = (size_t) eHouse_Harkonnen; i != (size_t) eHouse_End; ++i) {
		
		house = houseGet( (eHouse) i );
		if(house->houseName == pName)
			return i;
	}

	return eHouse_End;
}

sHouseData *cResources::houseGet( eHouse pHouse ) {
	map< eHouse, sHouseData* >::iterator	houseIT = _dataHouses.find( pHouse );

	// Has data already been loaded?
	if(houseIT != _dataHouses.end())
		return houseIT->second;

	sExeHouseData houseExeData = _Exe->houseGet( pHouse );

	sHouseData *houseData = new sHouseData();
	houseData->houseName = _Exe->stringGet( houseExeData.houseName );
	houseData->houseVoice = _Exe->stringGet( houseExeData.houseVoice );
	houseData->buildingDecay = houseExeData.buildingDecay;
	houseData->color = houseExeData.color;
	houseData->FrigateTime = houseExeData.FrigateTime;
	houseData->houseLetter[0] = (char) houseExeData.houseLetter;
	houseData->houseLetter[1] = '\0';
	houseData->LemonFactor = houseExeData.LemonFactor;
	houseData->missionBriefMusic = houseExeData.missionBriefMusic;
	houseData->missionLoseMusic = houseExeData.missionLoseMusic;
	houseData->missionWinMusic = houseExeData.missionWinMusic;
	houseData->PalaceUnitRecharge = houseExeData.PalaceUnitRecharge;
	houseData->PalaceMode = houseExeData.PalaceMode;
	houseData->Weakness = houseExeData.Weakness;

	// Store for later reference
	_dataHouses.insert( pair<eHouse, sHouseData*> ( pHouse, houseData ) );

	return houseData;
}

word cResources::unitFind( string pName ) {
	sUnitData	*Unit;

	// Iterate each unit until 'pName' is found
	for( size_t i = 0; i < 0x1B; ++i ) {
		
		Unit = unitGet( i );

		if( pName == Unit->Name )
			return i;
	}

	return 0;
}

sUnitData	*cResources::unitGet( size_t pIndex ) {

	map< size_t, sUnitData* >::iterator	unitIT = _dataUnits.find( pIndex );

	// Has data already been loaded?
	if(unitIT != _dataUnits.end())
		return unitIT->second;

	sExeUnitData	 unitExeData = _Exe->unitGet( pIndex );

	sUnitData		*unitData = new sUnitData( );
	unitData->typeIndex = pIndex;
	unitData->Aggressivity = unitExeData.Aggressivity;
	unitData->buildTime = unitExeData.buildTime;
	unitData->Cost = unitExeData.Cost;
	unitData->deathAnim = unitExeData.deathAnim;
	unitData->buildOrder = unitExeData.buildOrder;
	unitData->field_2A = unitExeData.field_2A;
	unitData->field_2B = unitExeData.field_2B;
	unitData->field_2D = unitExeData.field_2D;
	unitData->field_38 = unitExeData.field_38;
	unitData->field_39 = unitExeData.field_39;
	unitData->field_3A = unitExeData.field_3A;
	unitData->field_3E = unitExeData.field_3E;
	unitData->AICommand = unitExeData.AICommand;
	unitData->frameAngleMode = unitExeData.frameAngleMode;
	unitData->field_54 = unitExeData.field_54;
	unitData->infantrySpawn = unitExeData.infantrySpawn;
	unitData->HitPoints = unitExeData.HitPoints;
	unitData->indexMax = unitExeData.indexMax;
	unitData->indexMin = unitExeData.indexMin;
	unitData->MovementType = unitExeData.MovementType;
	unitData->Name = _Exe->stringGet( unitExeData.Name );
	unitData->optsFitW = unitExeData.optsFitW;
	unitData->options1 = unitExeData.options1;
	unitData->Owner = unitExeData.Owner;
	unitData->Picture = _Exe->stringGet( unitExeData.Picture );
	unitData->PreReqs = unitExeData.PreReqs;
	unitData->sidebarCommand1 = unitExeData.sidebarCommand1;
	unitData->sidebarCommand2 = unitExeData.sidebarCommand2;
	unitData->sidebarCommand3 = unitExeData.sidebarCommand3;
	unitData->sidebarCommand4 = unitExeData.sidebarCommand4;
	unitData->sidebarIconID = unitExeData.sidebarIconID;
	unitData->Sight = unitExeData.Sight;
	unitData->Speed = unitExeData.Speed;
	unitData->stringID = unitExeData.stringID;
	unitData->stringLongID = unitExeData.stringLongID;
	unitData->TechLevel = unitExeData.TechLevel;
	unitData->turningSpeed = unitExeData.turningSpeed;
	unitData->turretGfxID = unitExeData.turretGfxID;
	unitData->UnitGfxID = unitExeData.UnitGfxID;
	unitData->UpgradesNeeded = unitExeData.UpgradesNeeded;
	unitData->weaponDamage = unitExeData.weaponDamage;
	unitData->weaponRange = unitExeData.weaponRange;
	unitData->weaponRateFire = unitExeData.weaponRateFire;
	unitData->weaponSound = unitExeData.weaponSound;
	unitData->weaponType = unitExeData.weaponType;

	_dataUnits.insert( pair<size_t, sUnitData*>( pIndex, unitData ) );

	return unitData;
}

word cResources::structureFind( string pName ) {
	sStructureData *Struct;

	// Iterate each structure until 'pName' is found
	for( size_t i = 0; i < 0x1B; ++i ) {
		
		Struct = structureGet( i );

		if( pName == Struct->Name )
			return i;
	}

	return 0;
}

sStructureData  *cResources::structureGet( size_t pIndex ) {
	
	map< size_t, sStructureData* >::iterator	structIT = _dataStructures.find( pIndex );
	
	// Has data already been loaded?
	if(structIT != _dataStructures.end())
		return structIT->second;

	sExeStructureData	 structExeData = _Exe->structureGet( pIndex );

	sStructureData		*structData = new sStructureData( );

	structData->typeIndex = pIndex;
	structData->buildOpts = structExeData.buildOpts;
	structData->buildOrder = structExeData.buildOrder;
	structData->BuildTime = structExeData.BuildTime;
	structData->constructOpt1 = structExeData.constructOpt1;
	structData->constructOpt2 = structExeData.constructOpt2;
	structData->constructOpt3 = structExeData.constructOpt3;
	structData->constructOpt4 = structExeData.constructOpt4;
	structData->constructOpt5 = structExeData.constructOpt5;
	structData->constructOpt6 = structExeData.constructOpt6;
	structData->constructOpt7 = structExeData.constructOpt7;
	structData->constructOpt8 = structExeData.constructOpt8;
	structData->Cost = structExeData.Cost;
	structData->CYUpgradesNeeded = structExeData.CYUpgradesNeeded;
	structData->field_22 = structExeData.field_22;
	structData->field_23 = structExeData.field_23;
	structData->field_24 = structExeData.field_24;
	structData->field_25 = structExeData.field_25;
	structData->field_26 = structExeData.field_26;
	structData->field_27 = structExeData.field_27;
	structData->field_28 = structExeData.field_28;
	structData->field_29 = structExeData.field_29;
	structData->field_2A = structExeData.field_2A;
	structData->field_3E = structExeData.field_3E;
	structData->field_40 = structExeData.field_40;
	structData->field_42 = structExeData.field_42;
	structData->field_44 = structExeData.field_44;
	structData->field_46 = structExeData.field_46;
	structData->field_48 = structExeData.field_48;
	structData->infantrySpawn = structExeData.infantrySpawn;
	structData->FoundationSize = structExeData.FoundationSize;
	structData->HitPoints = structExeData.HitPoints;
	structData->idLong = structExeData.idLong;
	structData->idShort = structExeData.idShort;
	structData->Name = _Exe->stringGet( structExeData.Name );
	structData->Owner = structExeData.Owner;
	structData->Picture = _Exe->stringGet( structExeData.Picture );
	structData->PowerUsage = structExeData.PowerUsage;
	structData->PreReqs = structExeData.PreReqs;
	structData->RepairFacility = structExeData.RepairFacility;
	structData->sidebarIconID = structExeData.sidebarIconID;
	structData->Sight = structExeData.Sight;
	structData->SpiceStorage = structExeData.SpiceStorage;
	structData->structureGfxID = structExeData.structureGfxID;
	structData->structureID = structExeData.structureID;
	structData->Techlevel = structExeData.Techlevel;
	structData->techUpgrade1 = structExeData.techUpgrade1;
	structData->techUpgrade2 = structExeData.techUpgrade2;
	structData->techUpgrade3 = structExeData.techUpgrade3;
	structData->UnitsCanEnter = structExeData.UnitsCanEnter;
	structData->weaponDamage = structExeData.weaponDamage;
	structData->weaponDamageAdd = structExeData.weaponDamageAdd;

	_dataStructures.insert( pair<size_t, sStructureData*>( pIndex, structData ) );

	return structData;
}

sFileData *cResources::fileTableGet( size_t pIndex ) {
	sFileData		 *fileData		 = new sFileData();
	sExeFileData	 *fileExeData	 = _Exe->fileTableGet();

	fileExeData += pIndex;

	fileData->field_11 = fileExeData->field_11;
	fileData->field_4 = fileExeData->field_4;
	fileData->field_6 = fileExeData->field_6;
	fileData->field_8 = fileExeData->field_8;
	fileData->field_A = fileExeData->field_A;
	fileData->field_C = fileExeData->field_C;
	fileData->field_E = fileExeData->field_E;
	fileData->fileName = _Exe->stringGet( fileExeData->fileName );
	fileData->fileType = fileExeData->fileType;
	fileData->parentIndex = fileExeData->parentIndex;

	return fileData;
}

word *cResources::foundationMapModGet( size_t pIndex ) {
	word *found = _Exe->foundationMapModGet();

	found += (pIndex * 0x9);

	return found;
}

word cResources::foundationSizeGet( size_t pIndex ) {
	word *found = _Exe->foundationSizesGet();
	found += pIndex;

	return *found;
}

byte *cResources::mapOffsetsGet( size_t pIndex ) {
	
	return _Exe->mapOffsetsGet(pIndex);
}

byte *cResources::mapTerrainSpiceGet() {
	return _Exe->mapTerrainSpiceGet();
}

byte *cResources::mapSinTableGet() {
	return _Exe->mapSinTableGet();
}

byte *cResources::mapCosTableGet() {
	return _Exe->mapCosTableGet();
}

void cResources::pakLoad() {
	PakFile		*tmpFile;
	sFileData	*fileData = 0;
	istream		*stream;
	byte		*pakData = 0;
	size_t		 pakSize = 0;

	// Load all packs
	for( size_t i = 0; i < 0x0F; ++i ) {
		fileData = fileTableGet( i );
		string filename = fileData->fileName;
		delete fileData;

		pakData = fileRead( filename, pakSize );
		if(!pakData)
			continue;

		stream = new istringstream( std::string((const char*)pakData, pakSize ));
		tmpFile = new PakFile( stream );

		delete pakData;

		_Paks.insert( pair< string, PakFile*>( filename, tmpFile ) );
	}

}

string *cResources::fileRead( string pFilename) {
	PakFile *pak = 0;
	map< string, PakFile*>::iterator	pakIT;

	// Iterate over each PAK looking for 'filename'
	for( pakIT = _Paks.begin(); pakIT != _Paks.end(); ++pakIT ) {

		pak = pakIT->second;
		try {

			return pak->getFile( pFilename );
		} catch(...) {

		}
	}

	return 0;
}

istream	*cResources::fileOpen( string pFilename ) {
	PakFile *pak = 0;
	map< string, PakFile*>::iterator	pakIT;

	// Iterate over each PAK looking for 'filename'
	for( pakIT = _Paks.begin(); pakIT != _Paks.end(); ++pakIT ) {

		pak = pakIT->second;
		try {

			return pak->getFileStream( pFilename );
		} catch(...) {

		}
	}

	return 0;
}

byte *cResources::fileRead( istream *pStream, size_t &pSize) {
	byte *buffer = 0;

	pStream->seekg( 0, ios::end );
	pSize = pStream->tellg();
	pStream->seekg(ios::beg);

	if(pSize != 0) {
		buffer = new byte[pSize];
		pStream->read( (char*) buffer, pSize );
	}

	delete pStream;
	return buffer;
}

word *cResources::mapScaleGet( word pIndex ) {
	byte *mapScaleLevel = _Exe->mapScalesGet();

	// Multiply by 2 because there is X and Y
	return (word*) (mapScaleLevel + (pIndex << 2));
}

byte *cResources::mapIconPtrsGet( word pIndex ) {
	return _fileIconMAP + (pIndex * 2);
}

void cResources::mapTileIDsLoad() {
	word	tmpPtr = *(_fileIconMAP + 0x0E) << 1;
	_tileFogWar =	*(_fileIconMAP + tmpPtr + 0x20);

	tmpPtr = *(_fileIconMAP + 0x14) << 1;
	_tileBloom =	*(_fileIconMAP + tmpPtr);

	tmpPtr = *(_fileIconMAP + 0x10) << 1;
	_tileConcrete = *(_fileIconMAP + tmpPtr + 0x04);
	
	tmpPtr = *(_fileIconMAP + 0x0C) << 1;
	_tileWall =		*(_fileIconMAP + tmpPtr);
	
	tmpPtr = *(_fileIconMAP + 0x12) << 1;
	_tileLand =		*(_fileIconMAP + tmpPtr);
}

SDL_Surface	*cResources::mapTileGet( word pTileIndex ) {
	map< word, SDL_Surface* >::iterator		 tileIT;
	SDL_Surface								*surface;

	tileIT = _Tiles.find( pTileIndex );

	if(tileIT == _Tiles.end() ) {
		surface = _icons->getSurface( pTileIndex );
		_Tiles.insert( pair< word, SDL_Surface* >( pTileIndex, surface ));
	} else
		surface = tileIT->second;

	return surface;
}

word *cResources::unitAngleFrameAdjustGet( size_t pIndex ) {
	byte *adjust =  _Exe->unitAngleFrameAdjustGet();
	
	adjust += (pIndex << 2);

	return (word*) adjust;
}

word *cResources::unitAngleFrameFlagsGet( size_t pIndex ) {
	byte *adjust =  _Exe->unitAngleFrameAdjustGet();
	adjust+=2;

	adjust += (pIndex << 2);

	return (word*) adjust;
}

word *cResources::unitAngleFrame3AdjustGet( size_t pIndex ) {
	byte *adjust =  _Exe->unitAngleFrameAdjustGet();
	
	adjust += 0x20 + (pIndex << 2);

	return (word*) adjust;
}

word *cResources::unitAngleFrame3FlagsGet( size_t pIndex ) {
	byte *adjust =  _Exe->unitAngleFrameAdjustGet();
	adjust+=2;

	adjust += 0x20 + (pIndex << 2);

	return (word*) adjust;
}

byte *cResources::unitFrameAdjustGet( size_t pIndex ) {
	byte *adjust = _Exe->unitFrameAdjustGet();

	adjust += pIndex;

	return adjust;
}

word cResources::unitSiegeTurretFrameGet( size_t pIndex, bool addOne ) {
	byte *adjust = _Exe->UnitSiegeTurretFrameGet();
	if(addOne)
		adjust+=2;

	adjust += (pIndex << 2);

	return *((word*)adjust);
}

word cResources::unitDevastatorFrameGet( size_t pIndex, bool addOne ) {
	byte *adjust = _Exe->UnitSiegeTurretFrameGet();
	if(addOne)
		adjust+=2;
	
	adjust += 0x20;

	adjust += (pIndex << 2);

	return *((word*)adjust);
}

void cResources::angleAdjustBuild() {
	word tmp, tmp2;

	// Build the unit gfx angle adjustment table
	for( word count = 0; count < 0x100; ++count ) {
		tmp = count;
		tmp += 0x10;
		tmp &= 0xE0;
		tmp >>= 5;

		tmp2 = count;
		tmp2 += 0x08;
		tmp2 &= 0xF0;
		tmp2 >>= 4;

		tmp |= (tmp2 & 0xFF) << 4;

		_angleAdjust[count] = tmp;
	}

}

string cResources::languageAdd( string pFilename ) {
/*	switch( g_DuneEngine->languageGet() ) {

		case 0:
			pFilename.append("ENG");
			break;

		case 1:

			break;

		case 2:	
			break;
	}*/
	pFilename.append("ENG");
	return pFilename;
}

void cResources::shpLoad() {
	string	choamFile	= languageAdd("CHOAM.");
	string  bttnFile	= languageAdd("BTTN.");

	shpLoad("MOUSE.SHP");
	shpLoad(bttnFile);
	shpLoad("SHAPES.SHP");		
	shpLoad("UNITS2.SHP");
	shpLoad("UNITS1.SHP");
	shpLoad("UNITS.SHP");	
	shpLoad(choamFile);
}

void cResources::shpLoad( string pFileName ) {
	string			*str		= fileRead( pFileName );
	string			 st = *str;
	istringstream	 is(st);

	_shp = new ShpFile( is ,  _paletteIBM->getPalette() );
	
	word shpFiles =	_shp->getNumFiles();

	word count2 = _dataSHP.size();

	for( word count = 0; count < shpFiles; ++count, ++count2) {
		SDL_Surface *surface = _shp->getSurface( count );
		_dataSHP.insert( pair< size_t, SDL_Surface* >( count2, surface ));

	}

	delete str;
	delete _shp;
	_shp = 0;
}

SDL_Surface *cResources::shpGet( word pIndex ) {
	map< word, SDL_Surface* >::iterator		 shapeIT;
	SDL_Surface								*surface;

	shapeIT = _dataSHP.find( pIndex );

	if(shapeIT == _dataSHP.end() )
		return 0;
	else
		surface = shapeIT->second;

	return surface;
}

SDL_Surface *cResources::wsaGet( string pFileName, size_t pFrame ) {
	map<string, WsaFile*>::iterator		wsaIT = _dataWSA.find( pFileName );

	if( wsaIT != _dataWSA.end() )
		return wsaIT->second->getSurface( pFrame );
	
	string			*str		= fileRead( pFileName );
	string			 st = *str;
	istringstream	 is(st);

	delete str;

	WsaFile *wsa = new WsaFile( is, _paletteIBM->getPalette(), 0, 0 );
	
	_dataWSA.insert( pair< string, WsaFile*> (pFileName, wsa ));
	
	return wsa->getSurface( pFrame );
}

size_t cResources::mapIcnCount( word pIndex ) {
	size_t tileCount = _icons->getTileCount( pIndex );

	return tileCount;
}

vector<size_t> *cResources::mapIcnFrameSetGet( word pIndex ) {
	map< size_t, vector<size_t>*>::iterator		icnIT = _dataIconFrames.find( pIndex );

	if( icnIT != _dataIconFrames.end() )
		return icnIT->second;
	
	vector<size_t> *tiles = new vector<size_t>(_icons->getTileIDs( pIndex ));
	_dataIconFrames.insert( pair< size_t, vector<size_t> *>( pIndex, tiles ) );
	return tiles;
}

SDL_Surface *cResources::CpsGet( string fileName ) {
	size_t fileSize = 0;

	string		*str		= fileRead(fileName );
	string		st = *str;
	istringstream is(st);

	CpsFile		*Cps		= new CpsFile( is, _paletteIBM->getPalette() );
	SDL_Surface *surface	= Cps->getSurface();

	delete Cps;
	delete str;

	return surface;
}

void cResources::IniLoad( string fileName ) {
	istream		*stream		= fileOpen( fileName );
	byte		*ini		= fileRead( stream, _fileIniSize );

	delete _ini;
	_ini 		= new IniFile( ini, _fileIniSize );
	
	delete ini;
}

string cResources::IniStringGet(  string pSection, string pVariable, string pDefault ) {
	if(!_ini)
		return "";
	return _ini->getStringValue( pSection, pVariable, pDefault );
}

size_t cResources::IniNumGet( string pSection, string pVariable, size_t pDefault ) {
	if(!_ini)
		return 0;

	return _ini->getIntValue( pSection, pVariable, pDefault );
}

void cResources::IniSectionOpen( string pSection ) {
	if(!_ini)
		return;

	_IniHandles.insert( pair< string, IniFile::KeyListHandle> ( pSection, _ini->KeyList_Open(pSection) ));
}

string cResources::IniSectionNext( string pSection ) {
	map<string, IniFile::KeyListHandle>::iterator	keyIT = _IniHandles.find(pSection);

	if(keyIT == _IniHandles.end())
		return "";

	
	if( _ini->KeyList_EOF( keyIT->second ) )
		return "";

	return _ini->KeyList_GetNextKey( &keyIT->second );
}

void cResources::IniSectionClose(string pSection ) {
		map<string, IniFile::KeyListHandle>::iterator	keyIT = _IniHandles.find(pSection);

	if(keyIT == _IniHandles.end())
		return;

	_ini->KeyList_Close( &keyIT->second );
	_IniHandles.erase( keyIT );
}

