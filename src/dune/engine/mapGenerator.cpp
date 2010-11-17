#include "../stdafx.h"
#include "mapGenerator.h"
#include "scenario.h"
#include "../map.h"

cMapGenerator::cMapGenerator(  )  {
	_Seed = 0;

	_SeedRand = 98761234;
}

void cMapGenerator::generate( size_t pSeed ) {
	byte terrain[0x111];

	word di, rand;

	byte *offsetArray		= g_DuneEngine->resourcesGet()->mapOffsetsGet(0);
	byte *offsetArray2		= g_DuneEngine->resourcesGet()->mapOffsetsGet(1);

	signed char al;
	short int counter, index;

	_cMap = g_DuneEngine->scenarioGet()->mapGet();

	_Seed		= pSeed;
	_SeedRand	= pSeed;

	memset(&_map, 0, sizeof(_map));

	for( di = 0; di < 0x110; ++di ) {

		terrain[di] = seedRandomGet() & 0x0F;
		if(terrain[di] <= 0x0A)
			continue;

		terrain[di] = 0x0A;
	}

	for( counter = seedRandomGet() & 0x0F; counter >= 0; --counter ) {
		rand = seedRandomGet() & 0xFF;

		for( byte i = 0; i < 0x15; ++i ) {
			al = *(offsetArray + i);
			index = (rand + al);

			if(index < 0)
				index = 0;
			if(index > 0x110)
				index = 0x110;

			terrain[index] = (terrain[index] + (seedRandomGet() & 0x0F)) & 0x0F;
		}
	}

	for( counter = seedRandomGet() & 0x03; counter >= 0; --counter ) {
		rand = seedRandomGet() & 0xFF;

		for( byte i = 0; i < 0x15; ++i ) {
			al = *(offsetArray + i);
			index = (rand + al);

			if(index < 0)
				index = 0;
			if(index > 0x110)
				index = 0x110;

			terrain[index] = (seedRandomGet() & 0x03);
		}
	}

	word X, Y;

	for( Y = 0, di = 0;	Y < 0x40; Y+=4)
		for( X = 0;	X < 0x40; X+=4, ++di) {

			_map[ _cMap->posXYtoIndex( X, Y ) ] = terrain[ di ] & 0x0F;
		}

	word si = 0, ax = 0;

	for( Y = 0;	Y < 0x40; Y+=4) {
		for( X = 0;	X < 0x40; X+=4) {
			
			if(si != 0)
				ax = 0;
			else
				ax = 1;

			si = ax * 0x15;
			di = si;

			for( ; (si + 0x15) > di ; ++di ) {	
				word mapPos;
				signed short int  mapPoint[2];

				index = (di << 3);

				mapPoint[0] = _cMap->posXYtoIndex( (X + offsetArray2[index]),		(Y + offsetArray2[index + 2]) );
				mapPoint[1] = _cMap->posXYtoIndex( (X + offsetArray2[index + 4]),  (Y + offsetArray2[index + 6]) );
				mapPos = (mapPoint[1] + mapPoint[0]) >> 1;
				if(mapPos >= 0x1000)
					continue;	

				mapPoint[0] = _cMap->posXMaxYtoIndex( (X + offsetArray2[index]),		(Y + offsetArray2[index + 2]) );
				mapPoint[1] = _cMap->posXMaxYtoIndex( (X + offsetArray2[index + 4]),   (Y + offsetArray2[index + 6]) );

				_map[ mapPos ] = (_map[mapPoint[1]] + _map[mapPoint[0]] + 1) >> 1;
			}

		}	// X
	}	// Y


	generateTerrainAverage();
	generateTerrainBuild();

	generateSpiceAdd();
	generateFinalise();
	generateToTiles();
}

void cMapGenerator::generateTerrainAverage() {
	short int	mapLeft, mapHere, mapRight, mapUpLeft, mapUpHere, mapUpRight, mapDownRight, mapDown, mapDownLeft;
	word		mapRow[0x40], mapRowPrevious[0x40];
	word		start = 0;

	memset( &mapRow, 0, sizeof(mapRow) );

	// Loop for each column
	for( word Y = 0; Y < 0x40; ++Y, start += 0x40 ) {
		
		// move current row buffer into previous buffer, and new row into current row buffer
		for( word X = 0; X < 0x40; ++X ) {
			mapRowPrevious[ X ] = mapRow[ X ];
			mapRow[ X ] = _map[ start + X ];
		}

		// Loop for each row
		for( word X = 0; X < 0x40; ++X ) {
			
			// Read all map piece values in tiles around this tile
			mapHere		= mapRow[X];
			mapUpHere	= mapRowPrevious[X];
			
			mapUpRight	= mapRowPrevious[X+1];
			mapRight	= mapRow[X+1];

			mapDownRight= _map[ start + X + 0x41 ]; // 0x104 ];
			mapDown		= _map[ start + X + 0x40 ]; // 0x100 ];
			mapDownLeft	= _map[ start + X + 0x3F ]; // 0xFC ];

			// Don't get left when X is already 0
			if(X) {
				if(Y)
					mapUpLeft	= mapRowPrevious[X-1];

				mapLeft	= mapRow[X-1];
			}

			// No previous row?
			if(!Y)
				mapUpLeft = mapUpHere = mapUpRight = mapHere;

			// No previous column?
			if(!X)
				mapUpLeft = mapLeft = mapDownLeft = mapHere;
			
			// Last Row?
			if(Y == 0x3F)
				mapDownLeft = mapDown = mapDownRight = mapHere;
			
			// Last Column?
			if(X == 0x3F)
				mapUpRight = mapRight = mapDownRight = mapHere;
	
			// Set map piece to the average
			_map[ start + X] = (mapUpHere + mapUpRight + mapRight + mapDownRight + mapDown + mapDownLeft + mapLeft + mapUpLeft + mapHere) / 9;
		}

	}
}

void cMapGenerator::generateTerrainBuild() {
	word minRock, minMountain, minDune, mapCurrent;
	byte mapTerrain;

	minRock = seedRandomGet() & 0xF;
	if (minRock < 8) 
		minRock = 8;

	else if (minRock > 0xC) 
		minRock = 0xC;

	minMountain = minRock + 4;
	minDune = (seedRandomGet() & 0x3) - 1;

	// Loop for the entire map
	for( word counter = 0; counter < 0x1000; ++counter ) {
		mapCurrent = _map[counter];

		// Default is sand
		mapTerrain = TERRAIN_SAND;

		if (mapCurrent > minMountain)   
			mapTerrain = TERRAIN_MOUNTAIN;

		// Set to rock
		else if (mapCurrent >= minRock)  
			mapTerrain = TERRAIN_ROCK;

		// 
		else if (mapCurrent <= minDune) 
			mapTerrain = TERRAIN_DUNES;

		// Set final terrain piece
		_map[counter] = mapTerrain;
   }
}

void cMapGenerator::generateSpiceAdd() {
	short						 mapIndex;
	word						 posX, posY, mapPos, piecesMax, max;
	byte						*terrainSpiceTable = g_DuneEngine->resourcesGet()->mapTerrainSpiceGet();

	for( word counter = seedRandomGet() & 0x2F; counter; --counter) {
		posY = seedRandomGet() & 0x3F;
		posX = seedRandomGet() & 0x3F;

		mapPos = _cMap->posXYtoIndex( posX, posY );

		if( !terrainSpiceTable[ _map[ mapPos ] * 0x1C ] ) {
			++counter;
			continue;
		}
		
		piecesMax = seedRandomGet() & 0x1F;
		for( word pieceCount = 0; pieceCount < piecesMax; ) {
			max = seedRandomGet() & 0x3f;
			
			sMapPoint Point = _cMap->mapIndexToXY( mapPos );
			Point = mapXYCorrect( Point, max, 1 );
			mapIndex = _cMap->mapIndexFromXY( Point );
			
			if(mapIndex >= 0x1000 || mapIndex < 0)
				continue;
			
			generateSpiceAddTo(mapIndex);
			++pieceCount;
		}
	}
}

void cMapGenerator::generateSpiceAddTo( word pMapIndex ) {
	word	 mapTile = _map[ pMapIndex ];
	byte	*terrainSpiceTable = g_DuneEngine->resourcesGet()->mapTerrainSpiceGet();

	if( mapTile == TERRAIN_SPICE ) {
		_map[ pMapIndex ] = TERRAIN_SPICELOTS;
		generateSpiceAddTo( pMapIndex );
		return;
	} 

	if( mapTile == TERRAIN_SPICELOTS ) {
		word	posX, posY;
		short	mapRowIndex;
		
		for( short Y = -1; Y <= 1; ++Y ) 
			for( short X = -1; X <= 1; ++X ) {
				posY = Y + _cMap->posYFromIndex( pMapIndex );
				posX = X + _cMap->posXFromIndex( pMapIndex );
			
				mapRowIndex = _cMap->posXYtoIndex( posX, posY );
				if( mapRowIndex < 0 || mapRowIndex >= 0x1000)
					continue;

				if( !terrainSpiceTable[ _map[ mapRowIndex ] * 0x1C ] ) {
					_map[ pMapIndex ] = TERRAIN_SPICE;
					continue;
				}

				if( _map[ mapRowIndex ] == TERRAIN_SPICELOTS )
					continue;

				_map[ mapRowIndex ] = TERRAIN_SPICE;
			}

		return;
	}

	if( !terrainSpiceTable[ mapTile * 0x1C ] )
		return;

	_map[ pMapIndex ] = TERRAIN_SPICE;
}

void cMapGenerator::generateFinalise() {
	word		mapRow[0x40], mapRowPrevious[0x40], start = 0;
	byte		mapTileFinal = 0;
	short int	mapLeft, mapHere, mapRight, mapDown, mapUp;
	short int	X, Y;


	for( X = 0; X < 0x40; ++X  ) 
		mapRow[ X ] = (word) _map[ X ];

	for( Y = 0; Y < 0x40; ++Y, start += 0x40 ) {
		
		// Copy old row out, copy new row in
		for( X = 0; X < 0x40; ++X ) {

			mapRowPrevious[ X ] = mapRow[ X ];
			mapRow[ X ]			= (word) _map[ start + X ];

		}
		
		// Loop every column in this row, calculate the tile id for each
		for( X = 0; X < 0x40; ++X, mapTileFinal = 0 ) {
			mapHere = _map [ start + X];
			
			if(Y)
				mapUp = mapRowPrevious[ X ];
			else
				mapUp = mapHere;
			
			if(X)
				mapLeft = mapRow[ X - 1];
			else
				mapLeft = mapHere;

			if(X != 0x3F)
				mapRight = mapRow[ X + 1 ];
			else
				mapRight = mapHere;

			if(Y != 0x3F)
				mapDown = _map[ start + X + 0x40 ]; // 0x100 ];
			else
				mapDown = mapHere;
			
			switch( mapHere ) {
				case TERRAIN_ROCK:
					if( mapUp == mapHere || mapUp == TERRAIN_MOUNTAIN ) 
						mapTileFinal |= 0x01; 
					if( mapRight == mapHere || mapRight == TERRAIN_MOUNTAIN )
						mapTileFinal |= 0x02;
					if( mapDown == mapHere || mapDown == TERRAIN_MOUNTAIN )
						mapTileFinal |= 0x04;
					if( mapLeft == mapHere || mapLeft == TERRAIN_MOUNTAIN )
						mapTileFinal |= 0x08;

					break;

				case TERRAIN_SPICE:
					if( mapUp == mapHere || mapUp == TERRAIN_SPICELOTS ) 
						mapTileFinal |= 0x01; 
					if( mapRight == mapHere || mapRight == TERRAIN_SPICELOTS )
						mapTileFinal |= 0x02;
					if( mapDown == mapHere || mapDown == TERRAIN_SPICELOTS )
						mapTileFinal |= 0x04;
					if( mapLeft == mapHere || mapLeft == TERRAIN_SPICELOTS )
						mapTileFinal |= 0x08;
					
					break;

				default:
					if (mapUp == mapHere) 
						mapTileFinal |= 0x01;
					if (mapRight == mapHere) 
						mapTileFinal |= 0x02;
					if (mapDown	== mapHere) 
						mapTileFinal |= 0x04;
					if (mapLeft	== mapHere) 
						mapTileFinal |= 0x08;

					break;

			}	// switch( mapHere )
		
			switch (mapHere) {
				case TERRAIN_SAND:      mapTileFinal = 0;	  break;
				case TERRAIN_ROCK:      mapTileFinal++;		  break;
				case TERRAIN_DUNES:     mapTileFinal += 0x11; break;
				case TERRAIN_MOUNTAIN:  mapTileFinal += 0x21; break;
				case TERRAIN_SPICE:     mapTileFinal += 0x31; break;
				case TERRAIN_SPICELOTS: mapTileFinal += 0x41; break;
			}

			_map[ start + X ] = mapTileFinal;
		}

	}	// for( y )
}

void cMapGenerator::generateToTiles() {
	byte	*Icons			=		  g_DuneEngine->resourcesGet()->mapIconPtrsGet(0x9);
	word	*IconsLandscape = (word*) g_DuneEngine->resourcesGet()->mapIconPtrsGet(0);

	short	 tile;

	IconsLandscape += (*Icons);

	for( word count = 0; count < 0x1000; ++count ) {
		tile = _map[ count ];
		tile = IconsLandscape[tile];
		
		_map[ count ] &= 0xFE00;
		_map[ count ] |= tile;
	}

}

sMapPoint cMapGenerator::mapXYCorrect( sMapPoint pPoint, word pRandMax, bool pAdd80) {
	sMapPoint	Point;
	word	rand, rand2;

	signed char	*sinTable = (signed char*) g_DuneEngine->resourcesGet()->mapSinTableGet();
	signed char	*cosTable = (signed char*) g_DuneEngine->resourcesGet()->mapCosTableGet();
	
	Point = pPoint;

	if(!pRandMax)
		return pPoint;

	rand = seedRandomGet() & 0xFF;
	while(rand > pRandMax)
		rand >>= 1;				// SI

	rand2 = seedRandomGet() & 0xFF;

	Point._X += (((sinTable[rand2] * rand) >> 7) << 4);
	Point._Y += (((((-1) * cosTable[rand2]) * rand) >> 7) << 4);

	if(Point._X > 0x4000 || Point._Y > 0x4000)
		return pPoint;

	if(pAdd80)
		Point = _cMap->posAdjust( Point );

	return Point;
}

size_t cMapGenerator::seedRandomGet() {
	byte *seed = (byte*) &_SeedRand;

	byte al = seed[0];
	byte c, n;

	// Shift right 2 bits, keeping carry status of 2nd
	al >>= 1; c = al & 0x01; al >>= 1;
	
	// Roll left 1 bit, keeping bit7, use carry for new bit0
	n = (seed[2] & 0x80) >> 7;
	seed[2] <<= 1;
	seed[2] |= c;
	c = n;			

	// Roll left 1 bit, keeping bit7, use carry for new bit0
	n = (seed[1] & 0x80) >> 7;
	seed[1] <<= 1;
	seed[1] |= c;
	c = n;

	// invert carry
	c = !c;

	// 
	al -= (seed[0] + c);
	c = al & 0x01;

	//al >>= 1;
	seed[0] >>= 1;
	seed[0] |= (c << 7);

	// xor 
	al = seed[0] ^ seed[1];

	return al;
}
