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
	bool fresh = false;

	if(_mapCells[0] == 0 )
		fresh = true;

	// Load each piece of the map
	for( size_t count = 0; count < 0x1000; ++count )
		if( fresh )
			_mapCells[count] = new cMapCell( count );
		else
			_mapCells[count]->tileMapLoad();
}

void cMap::mapRetile(word pMapIndex, short int pArg_2 ) {
	
	if( !pArg_2 )
		return;

	word posX = posXFromIndex( pMapIndex );
	word posY = posYFromIndex( pMapIndex );

	short int counter = -pArg_2;

	for( ; counter <= pArg_2  ; ++counter ) {

		short int counter2 = -pArg_2;

		for( ; counter2 <= pArg_2; ++counter2 ) {

			word ax = posY + counter;
			word ax2 = posX + counter2;

			word mapIndex = posXYtoIndex( ax2, ax );

			short int var_C = sub_1D7E0( pMapIndex, mapIndex );

			// 15B0
			if( var_C  >= pArg_2 ) {
				if( var_C != pArg_2 )
					continue;
				
				word ax = g_DuneEngine->scenarioGet()->mapGeneratorGet()->seedRandomGet();
				if(!(ax & 1) )
					continue;
			}

			ax = mapTileTypeGet( mapIndex );

			if( ax == 8 )
				continue;

			sub_5700A( mapIndex, 1 );
		}

	}

	// 15FB
	sub_5700A( pMapIndex, 1 );
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

word cMap::sub_1D7E0( word pMapIndex1, word pMapIndex2 ) {
	short int ax = pMapIndex1;
	short int bx = pMapIndex2;
	
	ax &= 0x3F;
	bx &= 0x3F;

	ax -= bx;
	if( ax < 0 )
		ax = -ax;

	bx = pMapIndex1;
	short int dx = pMapIndex2;

	bx = ((word) bx) >> 6;
	dx = ((word) dx) >> 6;

	bx -= dx;
	if( bx < 0)
		bx = -bx;

	if( ax <= bx ) {
		short int cx = ax;
		ax = bx;
		bx = cx;
	}

	bx >>= 1;
	ax += bx;

	return ax;
}

word cMap::mapTileTypeGet( word pMapIndex ) {
	if( pMapIndex >= 0x1000 )
		return 0;

	if(_mapCells[ pMapIndex ] == 0 )
		return 0;
	
	short int tile = _mapCells[ pMapIndex ]->tileGet() & 0x1FF;

	if( tile == g_DuneEngine->resourcesGet()->tileConcrete() )
		return 0x0A;

	if( tile == g_DuneEngine->resourcesGet()->tileBloom() || tile == g_DuneEngine->resourcesGet()->tileBloom() + 1 )
		return 0x0E;
	
	if( tile > g_DuneEngine->resourcesGet()->tileWall() && (g_DuneEngine->resourcesGet()->tileWall() + 0x4B) > tile )
		return 0x0B;

	cObject *object = _mapCells[ pMapIndex ]->objectGet();

	if( object && _mapCells[ pMapIndex ]->hasStructure() )
		return 0x0C;

	dword tile2 = tile & 0xFF;
	tile2 >>= 1;
	tile2 &= 0x7F;
	if( tile2 == g_DuneEngine->resourcesGet()->tileWall())
		return 0x0D;

	tile -= g_DuneEngine->resourcesGet()->tileLand();

	if(tile < 0 )
		return 0x4;

	if( tile <= 0x52 ) {

		return g_DuneEngine->resourcesGet()->mapTileColorGet( tile);
	}

	return 0x04;
}

word cMap::sub_5700A( word pMapIndex, short int pArg_2 ) {
	if(pMapIndex >= 0x1000)
		return 0;

	if( pArg_2 == 0 )
		return 0;

	word newTileType, tileType = mapTileTypeGet( pMapIndex );
	
	if( tileType != TERRAIN_SPICELOTS || pArg_2 <= 0 ) {

		if( tileType == TERRAIN_SPICE || tileType == TERRAIN_SPICELOTS || pArg_2 >= 0 ) {
			
			if( tileType != TERRAIN_SAND && tileType != TERRAIN_DUNES && tileType != TERRAIN_SPICE && pArg_2 > 0 )
				return 0;

			//ovr194:0B55 
			if( pArg_2 > 0 ) {
				if( tileType == TERRAIN_SPICE )
					tileType = TERRAIN_SPICELOTS;
				else
					tileType = TERRAIN_SPICE;

			} else {
				//ovr194:0B6F
				if( tileType == TERRAIN_SPICELOTS )
					tileType = TERRAIN_SPICE;
				else
					tileType = TERRAIN_SAND;
			}
		
			if( tileType == TERRAIN_SPICELOTS )
				tileType = TERRAIN_SPICE;

			//ovr194:0B81
			newTileType = TERRAIN_SAND;
			if( tileType == TERRAIN_SPICE )
				newTileType = 0x31;

			if( tileType == TERRAIN_SPICELOTS )
				newTileType = 0x41;

			word *tiles = (word*) g_DuneEngine->resourcesGet()->mapIconPtrsGet( 9 );
			
			word *tile = (word*) g_DuneEngine->resourcesGet()->mapIconPtrsGet( (*tiles) +  newTileType );
			
			// ovr194:0BB3
			newTileType = *tile;

			_mapCells[ pMapIndex ]->tileSet( newTileType & 0x1FF );
			
			sub_57146( pMapIndex );
			sub_57146( pMapIndex + 1 );
			sub_57146( pMapIndex - 1 );
			sub_57146( pMapIndex - 0x40 );
			sub_57146( pMapIndex + 0x40 );
		}
	}

	return 0;
}

word cMap::sub_57146( word pMapIndex ) {
	short int word_45972[4] = { -0x40, 0x01, 0x40, -1 };
	short int di = pMapIndex;
	word si = 0;
	
	di &= 0xFFF;

	word tileType = mapTileTypeGet( di );
	if( tileType == TERRAIN_SPICE || tileType == TERRAIN_SPICELOTS ) {
		
		word var_2 = 0;

		for( var_2 = 0; var_2 < 4; ++var_2 ) {
			
			short mapIndex = di + word_45972[ var_2 ];

			short int posX = posXFromIndex( mapIndex );
			short int posY = posYFromIndex( mapIndex );
	
			word landTile;

			if( posX < 0 || posX > 0x40 || posY < 0 || posY > 0x40 ) {

				if( tileType == TERRAIN_SPICE || tileType == TERRAIN_SPICELOTS )
					si |= (1 << var_2);
				
				continue;

			} else {
				
				landTile = mapTileTypeGet( mapIndex );
				if( tileType == TERRAIN_SPICE ) {
					
					if( landTile == TERRAIN_SPICE )
						si |= (1 << var_2);

					if( landTile == TERRAIN_SPICELOTS )
						si |= (1 << var_2);

				} else {
				
					if( landTile != TERRAIN_SPICELOTS )
						continue;

					si |= (1 << var_2);
				}
			}

		}	// while

		if( tileType == TERRAIN_SPICE )
			si += 0x31;
		else 
			si += 0x41;

		word *tiles = (word*) g_DuneEngine->resourcesGet()->mapIconPtrsGet( 9 );
		word *tile = (word*) g_DuneEngine->resourcesGet()->mapIconPtrsGet( (*tiles) +  si );

		_mapCells[ di ]->tileSet( *tile & 0x1FF );
	}

	return 0;
}

