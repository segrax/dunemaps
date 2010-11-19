#include "../../stdafx.h"
#include "../mapGenerator.h"
#include "object.h"
#include "structure.h"
#include "unit.h"
#include "mapCell.h"
#include "../scenario.h"

cMapCell::cMapCell(  word pMapIndex ) : cObject( 0, pMapIndex, 0 ) {

	tileMapLoad();

	// No temp (eg. structure tile)
	_tileTempID = 0xFFFFFFFF;

	_house = 0;
	_object = 0;
}

cMapCell::~cMapCell() {

}

void cMapCell::tileMapLoad() {

	// Read our tile from the generator
	_tileID = *g_DuneEngine->scenarioGet()->mapGeneratorGet()->mapGet( mapIndexGet() );

}
void cMapCell::tileSetCurrent( cObject *pObject, size_t pTileTemp ) {

	// Set a temporary tile, and its pointer
	_object		= pObject;
	_tileTempID = pTileTemp;

	redrawSet();
}

bool cMapCell::objectEnter( cObject	*pObject ) {				// 
	
	// Pass the enter action to the held object, unless its being cleared
	if(_object && pObject)
		return _object->objectEnter( pObject );
	
	_object = pObject;
	return true;
}

cObject *cMapCell::objectActivate() {											//
	
	if(_object) {
		
		return _object->objectActivate();
	}// else
//		g_DuneEngine->screenGet()->sidebarGet()->selectClear();

	return 0;
}

void cMapCell::objectDeActivate() {
	
	if(_object)
		_object->objectDeActivate();

}

bool cMapCell::hasUnit() {
	
	if( _object && typeid( *_object ) == typeid( cUnit ) )
		return true;

	return false;
}

bool cMapCell::hasStructure() {

	if(_object && typeid( *_object ) == typeid( cStructure ) )
		return true;

	return false;
}

bool cMapCell::isEmpty() { 

	if(_object)
		return false;

	return true;
}
