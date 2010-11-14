#include "stdafx.h"
#include "minimap.h"
#include "map.h"
#include "engine/mapGenerator.h"
#include "engine/scenario.h"
#include "screenPlayfield.h"

cMinimap::cMinimap( ) {
	_surfaceAreaFrame = 0;
	_mapX = _mapY = 0;
}

bool cMinimap::positionUpdate() {
	//word mapX = g_DuneEngine->screenGet()->playfieldGet()->mapXGet();
	//word mapY = g_DuneEngine->screenGet()->playfieldGet()->mapYGet();

	//if(mapX == _mapX && mapY == _mapY )
		return false;
	
	//redrawSet();

//	_mapX = mapX;
	//_mapY = mapY;

	return true;
}

void cMinimap::draw() {

	positionUpdate();

	for( word mapIndex = 0; mapIndex < 0x1000; ++mapIndex )
		drawPiece( mapIndex );

	// Draw rectangle around selected area
	drawAreaFrame();
}

void cMinimap::drawPiece( word pMapIndex ) {
//	SDL_Surface *minimap = surfaceGet()->surfaceGet();
	word		 Y, X, tileType;
	size_t		 pieceColor;

	// Get the landscape color table
	byte *table = g_DuneEngine->resourcesGet()->mapTerrainSpiceGet();

	// X/Y of current map location
	X = g_DuneEngine->scenarioGet()->mapGet()->posXFromIndex( pMapIndex );
	Y = g_DuneEngine->scenarioGet()->mapGet()->posYFromIndex( pMapIndex );
	
	// Get the dest pixel
	//dword *pixel = ((dword*)minimap->pixels) + (minimap->w * Y) + X;

	// FIXME: Unused:  Get the map scale
	//byte mapScale = g_DuneEngine->scenarioGet()->mapScaleLevelGet();

	// Check if the map has a unit/structure, and use that color if so
	pieceColor = g_DuneEngine->scenarioGet()->mapGet()->mapColorGet( pMapIndex );
	if(!pieceColor) {

		// Get the tile, and its color
		tileType = (size_t) g_DuneEngine->scenarioGet()->mapGet()->mapTileColorGet( pMapIndex );
		pieceColor = table[ (tileType * 0x1C) + 4];
	}

	// Get the pixel color from the palette
	SDL_Color *col = g_DuneEngine->resourcesGet()->colorGet( pieceColor );

	// Draw the pixel 
	//dword	color = SDL_MapRGB( minimap->format, col->r, col->g, col->b );
	//*pixel = color;
	
}

void cMinimap::drawAreaFrame() {

	// Create the frame in buffer
	if(!_surfaceAreaFrame) {
		_surfaceAreaFrame = SDL_CreateRGBSurface(	SDL_SWSURFACE,	17,	12,	 32, 0, 0, 0, 0);

		// This code draws 2 boxes, one smaller to make it look like a clear rectangle
		dword	White = SDL_MapRGB( _surfaceAreaFrame->format, 255, 255, 255 );
		// Outside box
		SDL_Rect dstRect;
		dstRect.x = dstRect.y = 0;
		dstRect.w = 17; dstRect.h = 12;
		SDL_FillRect( _surfaceAreaFrame, &dstRect, White );
		// Inside box
		dstRect.x = dstRect.y = 1;
		dstRect.w = 15; dstRect.h = 10;
		SDL_FillRect( _surfaceAreaFrame, &dstRect, 0 );
		// Set color key so the inside is transparent
		SDL_SetColorKey(	_surfaceAreaFrame, SDL_SRCCOLORKEY, SDL_MapRGB(_surfaceAreaFrame->format, 0, 0, 0)	);
	}

	if(!_mapX)
		_mapX=1;
	if(!_mapY)
		_mapY=1;

//	surfaceGet()->surfacePut( _surfaceAreaFrame, _mapX - 1, _mapY - 1);
}

void cMinimap::buttonPressLeft( size_t pX, size_t pY ) {
	// Dont move the map past the left side of the screen
	if(pX < 0x9)
		pX = 0x9;

	// Dont move above the top of the screen
	if(pY < 0x6)
		pY = 0x6;

	// Adjust to centre the map
	pX -= 0x8;
	pY -= 0x5;

	// Dont go past the bottom
	if( pY >= 0x36 )
		pY = 0x35;

	// Dont go past the right
	if( pX >= 0x31)
		pX = 0x30;

	// Set the new map location
	//g_DuneEngine->screenGet()->playfieldGet()->mapTacticalSet( pX, pY );

	//redrawSet();
}

void cMinimap::buttonClear() {

}

void cMinimap::buttonPressRight(size_t pX, size_t pY ) {
	
}

void cMinimap::buttonReleaseLeft( size_t pX, size_t pY ) {

}

void cMinimap::buttonReleaseRight(size_t pX, size_t pY ) {

}