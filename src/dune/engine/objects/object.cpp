#include "../../stdafx.h"
#include "object.h"
#include "..//scenario.h"
#include "../../map.h"
#include "mapCell.h"

cObject::cObject( cHouse *pHouse, word pX, word pY ) {
	_mapIndex = g_DuneEngine->scenarioGet()->mapGet()->posXYtoIndex( pX, pY );

	_objectSelected = false;
	_redraw = false;
	_surface = 0;
	_house = pHouse;
	_X = _Y = 0;
	_objectType = 0;
	_frame = 0;
}

cObject::cObject( cHouse *pHouse, word pMapIndex) {
	_mapIndex = pMapIndex;

	_objectSelected = false;
	_redraw = false;
	_surface = 0;
	_house = pHouse;
	_X = _Y = 0;
	_objectType = 0;
	_frame = 0;
}

cObject::~cObject() {
	delete _surface;
}

void cObject::cycle() {

}

void cObject::draw() {

}

cMapCell **cObject::mapCellGet() {
	// is the unit on the playfield?
	if(_mapIndex == 0xFFFF)
		return 0;

	return g_DuneEngine->scenarioGet()->mapGet()->mapCellGet( _mapIndex );
}
