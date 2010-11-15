#include "stdafx.h"


// 'DEMO','1.00','1.07-US','1.07-EU','1.07-HS'
const sGameVersions gameVersions[] = {
	
	// DEMO
	0x36FFE, 0x31470, 0x30CFA, 0x3A14A, 0x324D90F0, 0x3887A, 0x3A39E,0x3A59A,0x3A69A,0x32C80,0x3A55C,0x38D5A,0x32BCE,0x39740,	0x39652,	0x0, 0x0, 0x0, 0x0,
	
	// 1.00
	0x37FA2, 0x30750, 0x2FFDA, 0x3B284, 0x332B8110, 0x0,	 0x0,	 0x0,	0x0,	 0x0,	 0x0,	  0x0, 	0x0,			0x0,	0x0,	0x0, 0x0, 0x0, 0x0,
	
	// 1.07-US
	0x37BB2, 0x2FCB0, 0x2F58A, 0x3AD5C, 0x32EC8500, 0x39576, 0x3AFB4,0x3B1B0,0x3B2B0,0x31600,0x3B172,0x39A1C,0x3154E,0x3A2B6,	0x3A1C8,	0x0, 0x0, 0x0, 0x0,
	
	// 1.07-EU
	0x38132, 0x2FD00, 0x2F5DA, 0x3B878, 0x33447F80, 0x0,	 0x0,	 0x0,	0x0,	 0x0,	 0x0,	  0x0,	0x0,			0x0,	0x0,	0x0, 0x0, 0x0, 0x0, 
	
	// 1.07-HS
	0x380D2, 0x2FCB0, 0x2F58A, 0x3B80C, 0x333E7FE0, 0x3A026, 0x3BA64,0x3BC60,0x3BD60,0x31600,0x3BC22,0x3A4CC,0x3154E,0x3AD66,	0x3AC78, 0x3B2B8, 0x3B35E, 0x3B382, 0x3BBF6, 
	
	0
};


cResourceExe::cResourceExe( const char *pExe ) {
	size_t fileSize = 0;

	_bufferExe = g_DuneEngine->resourcesGet()->fileRead( pExe, fileSize );
	
	if(_bufferExe)
		versionCheck();
}

cResourceExe::~cResourceExe() {

	delete _bufferExe;
}

sExeActionData cResourceExe::actionGet( size_t pIndex ) {
	byte			*actionBuffer = _bufferExe + _version->offActions;
	sExeActionData	 actionData;

	actionBuffer += ((size_t) pIndex * sizeof( sExeActionData ));
	actionData = *((sExeActionData*) actionBuffer);

	return actionData;
}

sExeHouseData cResourceExe::houseGet( eHouse pHouse ) {
	byte			*houseBuffer =  _bufferExe + _version->offHouses;
	sExeHouseData	 houseData;
	
	// Move to the house data
	houseBuffer += ((size_t) pHouse * sizeof( sExeHouseData ));

	// Copy the house data
	houseData = *((sExeHouseData*) houseBuffer);

	return houseData;
}

sExeStructureData cResourceExe::structureGet( size_t pIndex ) {
	byte				*structureBuffer =  _bufferExe + _version->offStructs;
	sExeStructureData	 structureData;

	// Move to the house data
	structureBuffer += ((size_t) pIndex * sizeof( sExeStructureData ));

	// Copy the house data
	structureData = *((sExeStructureData*) structureBuffer);

	return structureData;
}

sExeUnitData cResourceExe::unitGet( size_t pIndex ) {
	byte				*unitBuffer =  _bufferExe + _version->offUnits;
	sExeUnitData		 unitData;
	
	// Move to the house data
	unitBuffer += ((size_t) pIndex * sizeof( sExeUnitData ));

	// Copy the house data
	unitData = *((sExeUnitData*) unitBuffer);

	return unitData;
}

sExeFileData *cResourceExe::fileTableGet() {
	return  (sExeFileData*) (_bufferExe + _version->offFiles);
}

word *cResourceExe::foundationSizesGet( ) {
	return (word*) (_bufferExe + _version->offFoundationSizes);
}

word *cResourceExe::foundationMapModGet( ) {
	return (word*) (_bufferExe + _version->offFoundationMapMod);
}

byte *cResourceExe::mapOffsetsGet( size_t pIndex ) {
	byte				*mapOffsets = _bufferExe + _version->offMapOffsets;

	if(pIndex > 0)
		mapOffsets += 0x15;

	return mapOffsets;
}

byte *cResourceExe::mapScalesGet() {
	return (byte*) (_bufferExe + _version->offMapScales);
}

byte *cResourceExe::mapTerrainSpiceGet() {
	return _bufferExe + _version->offTerrainSpice;
}

byte *cResourceExe::mapSinTableGet() {
	return _bufferExe + _version->offSin;
}

byte *cResourceExe::mapCosTableGet() {
	return _bufferExe + _version->offCos;
}

byte *cResourceExe::mapTileColorGet( ) {
	return _bufferExe + _version->offMapTileColors;
}

byte *cResourceExe::movementNames( ) {
	byte *name = (_bufferExe + _version->offMovementNames);

	return (byte*) (name);
}

byte *cResourceExe::unitAngleFrameAdjustGet() {
	return _bufferExe + _version->offUnitAngleFrameAdjust;
}

byte *cResourceExe::unitFrameAdjustGet() {
	return _bufferExe + _version->offUnitFrameAdjust;
}

byte *cResourceExe::UnitSiegeTurretFrameGet() {
	return _bufferExe + _version->offUnitSiegeTurretFrameAdjust;
}

string cResourceExe::stringGet( size_t	pAddress ) {
	string		tmp;

	if(pAddress == 0x00)
		return "";

	tmp.append( (char*) _bufferExe + (pAddress - _version->offReferences));

	return tmp;
}

bool cResourceExe::versionCheck() {
	string	tmpName;
	size_t	i = 0;

	_version = 0;
	for( const sGameVersions *ver = &gameVersions[0]; gameVersions[i].offName != 0; ver = &gameVersions[++i] ) {

		tmpName.clear();
		tmpName.append( (char*) _bufferExe + ver->offName, 9 );

		if( tmpName == "DUNE2.EXE") {
			_version = ver;
			return true;
		}
	}

	return false;
}
