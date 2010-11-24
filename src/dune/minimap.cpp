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

cMinimap::~cMinimap() {
	SDL_FreeSurface( _surfaceAreaFrame );
}

bool cMinimap::positionUpdate() {
	word mapX = g_DuneEngine->screenPlayfieldGet()->mapXGet();
	word mapY = g_DuneEngine->screenPlayfieldGet()->mapYGet();

	if(mapX == _mapX && mapY == _mapY )
		return false;

	_mapX = mapX;
	_mapY = mapY;

	return true;
}

void cMinimap::draw( cVideoSurface *pSurface ) {
	word	*scale = g_DuneEngine->screenPlayfieldGet()->scaleGet();

	positionUpdate();

	for( word Y = scale[1]; Y < scale[1] + scale[3]; ++Y )
		for( word X = scale[0]; X < scale[2] + scale[0]; ++X )
			drawPiece( g_DuneEngine->scenarioGet()->mapGet()->posXYtoIndex( X, Y ), pSurface );

	//for( word mapIndex = 0; mapIndex < 0x1000; ++mapIndex )
	//	drawPiece( mapIndex, pSurface );

	// Draw rectangle around selected area
	drawAreaFrame( pSurface );
}

void cMinimap::drawPiece( word pMapIndex,  cVideoSurface *pSurface ) {
	SDL_Surface *minimap = pSurface->surfaceGet();
	word		 Y, X, tileType;
	size_t		 pieceColor;

	// Get the landscape color table
	byte *table = g_DuneEngine->resourcesGet()->mapTerrainSpiceGet();

	// X/Y of current map location
	X = g_DuneEngine->scenarioGet()->mapGet()->posXFromIndex( pMapIndex );
	Y = g_DuneEngine->scenarioGet()->mapGet()->posYFromIndex( pMapIndex );

	word	*scale = g_DuneEngine->screenPlayfieldGet()->scaleGet();
	
	X -= scale[0];
	Y -= scale[1];
	
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
	dword	color = SDL_MapRGB( minimap->format, col->r, col->g, col->b );

	size_t scaleLevel = g_DuneEngine->scenarioGet()->mapScaleLevelGet() + 1;
	// Get the dest pixel
	dword *pixel = 0;
	
	pixel = ((dword*)minimap->pixels) + (minimap->w * (Y * scaleLevel)) + (X * scaleLevel);

	*pixel++ = color;

	if(scaleLevel>1) {
		*pixel = color;

		pixel += minimap->w - 1;
		*pixel++ = color;
		*pixel = color;
	}


}

void cMinimap::drawAreaFrame( cVideoSurface *pSurface ) {
	word scalelevel = g_DuneEngine->scenarioGet()->mapScaleLevelGet();

	// Create the frame in buffer
	SDL_FreeSurface( _surfaceAreaFrame );

	word frameSizeX = ((scalelevel + 1) * g_DuneEngine->screenTilesMaxX());
	word frameSizeY = ((scalelevel + 1) * g_DuneEngine->screenTilesMaxY());

	if(frameSizeX > 63)
		frameSizeX = 63;

	if(frameSizeY > 63)
		frameSizeY = 63;
	_surfaceAreaFrame = SDL_CreateRGBSurface(	SDL_SWSURFACE,	frameSizeX,	frameSizeY,	 32, 0, 0, 0, 0);

	// This code draws 2 boxes, one smaller to make it look like a clear rectangle
	dword	White = SDL_MapRGB( _surfaceAreaFrame->format, 255, 255, 255 );
	// Outside box
	SDL_Rect dstRect;
	dstRect.x = dstRect.y = 0;
	dstRect.w = frameSizeX; dstRect.h = frameSizeY;
	SDL_FillRect( _surfaceAreaFrame, &dstRect, White );
	// Inside box
	dstRect.x = dstRect.y = 1;
	dstRect.w = frameSizeX - 2; dstRect.h = frameSizeY - 2;
	SDL_FillRect( _surfaceAreaFrame, &dstRect, 0 );
	// Set color key so the inside is transparent
	SDL_SetColorKey(	_surfaceAreaFrame, SDL_SRCCOLORKEY, SDL_MapRGB(_surfaceAreaFrame->format, 0, 0, 0)	);

	word drawX, drawY;


	drawX = _mapX;
	drawY = _mapY;


	word	*scale = g_DuneEngine->screenPlayfieldGet()->scaleGet();


	if(scalelevel > 0) {
		drawX -= scale[0];
		drawY -= scale[1];
	} else {
		--drawX;
		--drawY;
	}

	if(drawY + g_DuneEngine->screenTilesMaxY() > 64 / (scalelevel + 1))
		drawY = (64 / (scalelevel + 1)) - g_DuneEngine->screenTilesMaxY();
	
	if(drawX + g_DuneEngine->screenTilesMaxX() > 64 / (scalelevel + 1))
		drawX = (64 / (scalelevel + 1)) - g_DuneEngine->screenTilesMaxX();

	drawX *= (scalelevel + 1);
	drawY *= (scalelevel + 1);

	pSurface->surfacePut( _surfaceAreaFrame, drawX, drawY);
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