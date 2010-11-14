#include "stdafx.h"
#include "engine/house.h"
#include "engine/objects/object.h"
#include "engine/objects/mapCell.h"
#include "engine/mapGenerator.h"
#include "map.h"
#include "engine/scenario.h"

cMap::cMap(  ) {
	
	for( size_t count = 0; count < 0x1000; ++count )
		_mapCells[count] = 0;

}

cMap::~cMap() {

	for( size_t count = 0; count < 0x1000; ++count )
		delete _mapCells[count];

}

void cMap::mapLoad() {
	// Load each piece of the map
	for( size_t count = 0; count < 0x1000; ++count )
		_mapCells[count] = new cMapCell( count );

}

size_t cMap::mapColorGet( word pMapIndex ) {
	cObject *object = _mapCells[ pMapIndex ]->objectGet();

	if( object )
		return object->houseGet()->colorGet();

	return 0;
}

size_t cMap::mapTileColorGet( word pMapIndex ) {
	if(_mapCells[ pMapIndex ] == 0 )
		return 0;

	cObject *object = _mapCells[ pMapIndex ]->objectGet();

	if( object )
		return object->houseGet()->colorGet();
	
	int tile = _mapCells[ pMapIndex ]->tileGet() & 0x1FF;

	if( tile == g_DuneEngine->resourcesGet()->tileConcrete() )
		return 0x0A;

	if( tile == g_DuneEngine->resourcesGet()->tileBloom() || tile == g_DuneEngine->resourcesGet()->tileBloom() + 1 )
		return 0x0E;
	
	if( tile > g_DuneEngine->resourcesGet()->tileWall() && (g_DuneEngine->resourcesGet()->tileWall() + 0x4B) > tile )
		return 0x0B;

	dword tile2 = tile & 0xFF;
	tile2 >>= 1;
	tile2 &= 0x7F;
	if( tile2 == g_DuneEngine->resourcesGet()->tileWall())
		return 0x0D;

	// Check map piece for building here
	
	//if(has_structure)
	//	return 0x0C;

	if(tile - g_DuneEngine->resourcesGet()->tileLand() < 0)
		return 0x04;

	tile -= g_DuneEngine->resourcesGet()->tileLand();

	if( tile <= 0x52 ) {
		
		return g_DuneEngine->resourcesGet()->mapTileColorGet( tile );
	}

	return 0x04;
}

sMapPoint cMap::mapIndexToXY( word mapPos ) {
	sMapPoint	points;
	points._X = ((mapPos & 0x3F) << 8) | 0x80;
	points._Y = ((mapPos << 2) & 0xFF00) | 0x80;
	return points;
}