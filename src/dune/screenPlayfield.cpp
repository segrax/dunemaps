#include "stdafx.h"
#include "screenPlayfield.h"
#include "engine/objects/object.h"
#include "engine/objects/mapCell.h"
#include "engine/objects/unit.h"
#include "engine/objects/structure.h"

#include "engine/scenario.h"
#include "engine/house.h"
#include "map.h"
#include "minimap.h"

#include "..\panelSurface.h"

cScreenPlayfield::cScreenPlayfield( ) {
	word Tactical = g_DuneEngine->scenarioGet()->mapTacticalGet();
		
	_mapX = g_DuneEngine->scenarioGet()->mapGet()->posXFromIndex( Tactical );
	_mapY = g_DuneEngine->scenarioGet()->mapGet()->posYFromIndex( Tactical );

	_mapCell = g_DuneEngine->scenarioGet()->mapGet()->mapCellGet( g_DuneEngine->scenarioGet()->mapCursorGet() );

	scaleSet();

	_surfaceLandscape = 0;
	_surfaceUnits = 0;
	_surfaceTileHighlight = 0;

	_width = 260;
	_height = 160;

	_redraw = false;
}

cScreenPlayfield::~cScreenPlayfield() {
	
	delete _surfaceLandscape;
	delete _surfaceUnits;
	delete _surfaceTileHighlight;
}

void cScreenPlayfield::scaleSet() {

	_scale = g_DuneEngine->resourcesGet()->mapScaleGet( g_DuneEngine->scenarioGet()->mapScaleLevelGet() << 1);
}

void cScreenPlayfield::screenPrep() {
	// Select the structure
	//if((*_mapCell)->hasStructure())
	//	g_DuneEngine->screenGet()->sidebarGet()->selectStructure( (cStructure*) (*_mapCell)->objectGet() );
}

void cScreenPlayfield::drawLandscape() {
	delete _surfaceLandscape;
	_surfaceLandscape	= new cVideoSurface( _width, _height );
	
	word		  tilesMaxY = g_DuneEngine->screenTilesMaxY();
	word		  tilesMaxX = g_DuneEngine->screenTilesMaxX();
	word		  mapIndex	= 0;
	cMapCell	**mapCell;
	
	// Update the drawn X/Y
	_landscapeX = _mapX;
	_landscapeY = _mapY;

	// Calculate the top-left index
	mapIndex = g_DuneEngine->scenarioGet()->mapGet()->posXYtoIndex(_mapX, _mapY );

	// Loop for each tile of the visible playfield
	for( word Y = 0; Y < tilesMaxY; ++Y, mapIndex += 0x40 ) {

		if( _mapY + Y < _scale[1] )
			continue;

		if( _mapY + Y >= (_scale[3] + _scale[1])  )
			break;

		// Get pointer to tiles
		mapCell = g_DuneEngine->scenarioGet()->mapGet()->mapCellGet( mapIndex );
		
		for( word X = 0; X < tilesMaxX; ++X, mapCell++ ) {

			if( _mapX + X < _scale[0] )
				continue;

			if( _mapX + X >= (_scale[2] + _scale[0]) )
				break;

			// FIXME: Error out here?
			if( !mapCell )
				break;

			// Draw tile to playfield
			SDL_Surface *tile = g_DuneEngine->resourcesGet()->mapTileGet( (*mapCell)->tileGet());
			
			// If there is an object at this tile, use the house palette to draw the tile
			if((*mapCell)->objectGet())
				SDL_SetColors(tile, (*mapCell)->objectGet()->houseGet()->paletteGet()->colors, 0, (*mapCell)->objectGet()->houseGet()->paletteGet()->ncolors);
			
			// Draw the tile
			_surfaceLandscape->surfacePut( tile, (X << 4), (Y << 4) );
		}
	}

}

void cScreenPlayfield::drawObjects() {
	word		  tilesMaxY = g_DuneEngine->screenTilesMaxY();
	word		  tilesMaxX = g_DuneEngine->screenTilesMaxX();
	word		  mapIndex	= 0;
	cMapCell	**mapCell;

	// Calculate the top-left index
	mapIndex = g_DuneEngine->scenarioGet()->mapGet()->posXYtoIndex(_mapX, _mapY );
	
	delete _surfaceUnits;
	_surfaceUnits		= new cVideoSurface( _width, _height );

	_surfaceUnits->colorKeySet(0xFF);
	_surfaceUnits->clear( 0xFF );

	for( word Y = 0; Y < tilesMaxY; ++Y, mapIndex += 0x40 ) {

		if( _mapY + Y < _scale[1] )
			continue;

		if( _mapY + Y >= (_scale[3] + _scale[1])  )
			break;

		// Get pointer to tiles
		mapCell = g_DuneEngine->scenarioGet()->mapGet()->mapCellGet( mapIndex );

		for( word X = 0; X < tilesMaxX; ++X, ++mapCell ) {
			
			if( _mapX + X < _scale[0] )
				continue;

			if( _mapX + X >= (_scale[2] + _scale[0]) )
				break;

			// Draw unit?
			if( !(*mapCell)->hasUnit() )
				continue;

			cUnit		 *unit = (cUnit*) (*mapCell)->objectGet();
			
			if(!unit)
				continue;

			unit->draw();
			_surfaceUnits->surfacePut( unit->surfaceGet(), (X << 4), (Y << 4) );
		}

	}
}

void cScreenPlayfield::draw( cVideoSurface *pSurface ) {

	// Does the landscape surface need updating?
	if(_landscapeX != _mapX || _landscapeY != _mapY || _redraw )
		drawLandscape();
	
	//
	drawObjects();

	_redraw = false;

	pSurface->surfacePut( _surfaceLandscape, 0, 0 );
	pSurface->surfacePut( _surfaceUnits, 0, 0 );

	cObject *object = g_DuneEngine->mPlaceObjectGet();

	// Draw the rectangle for an object thats about to be placed
	if(object) {
		
		word mapIndex = g_DuneEngine->scenarioGet()->mapGet()->posXYtoIndex( _mapX + _mouseX, _mapY + _mouseY );

		if( typeid(*object) == typeid( cStructure ) )
			drawTileSquares( pSurface, mapIndex, (cStructure*) object );

		else {
			drawTileSquare( 16, 16 );

			// Draw the rectangle to the surface
			pSurface->surfacePut( _surfaceTileHighlight, _mouseX * 16, _mouseY * 16  );
		}
	}

	// Is a structure selected? draw the rectangle on it
	if(!(*_mapCell)->hasStructure())
		return;

	drawTileSquares(pSurface);
}

void cScreenPlayfield::drawTileSquares( cVideoSurface *pSurface ) {
	cStructure  *structure  = 0;
	word		 width		= 1, height = 1;
	word		 mapIndex	= (*_mapCell)->mapIndexGet();
	word		 X, Y;

	// Is this a structure?
	if(!((*_mapCell)->hasStructure()))
		return;

	structure = (cStructure*) (*_mapCell)->objectGet();

	// Get the number of tiles and the layout
	size_t   foundationTiles	= g_DuneEngine->resourcesGet()->foundationSizeGet( structure->dataGet()->FoundationSize );
	word	*foundationLayout	= g_DuneEngine->resourcesGet()->foundationMapModGet( structure->dataGet()->FoundationSize );
	
	for( size_t count = 0; count < foundationTiles; ++count ) {
		
		// Down but not across
		if( (*foundationLayout & 0xC0) && !(*foundationLayout & 0x03))
			height++;

		// Across but not down
		if( (!(*foundationLayout & 0xC0) && *foundationLayout & 0x03))
			width++;

		// Next foundation position
		foundationLayout++;
	}

	// Draw a rectangle
	drawTileSquare( width * 16, height * 16 );

	// Get the top-left cell
	mapIndex = structure->mapIndexGet();

	// Calcualte the screen X/Y of the top-left of the structure
	X = g_DuneEngine->scenarioGet()->mapGet()->posXFromIndex( mapIndex ) - _mapX;
	Y = g_DuneEngine->scenarioGet()->mapGet()->posYFromIndex( mapIndex ) - _mapY;

	// Draw the rectangle to the surface
	pSurface->surfacePut( _surfaceTileHighlight, (X << 4),(Y << 4)  );
}

void cScreenPlayfield::drawTileSquares( cVideoSurface *pSurface, word pMapIndex, cStructure *pStructure ) {
	cStructure  *structure  = 0;
	word		 width		= 1, height = 1;
	word		 X, Y;

	cMapCell **mapCell = g_DuneEngine->scenarioGet()->mapGet()->mapCellGet( pMapIndex );
	
	// Get top-left
	if((*mapCell)->objectGet())
		mapCell = ((cStructure*) (*mapCell)->objectGet())->mapCellGet();

	if( (*mapCell)->mapIndexGet() != 0xFFFF ) 
		pMapIndex = (*mapCell)->mapIndexGet();

	// Get the number of tiles and the layout
	size_t   foundationTiles	= g_DuneEngine->resourcesGet()->foundationSizeGet( pStructure->dataGet()->FoundationSize );
	word	*foundationLayout	= g_DuneEngine->resourcesGet()->foundationMapModGet( pStructure->dataGet()->FoundationSize );
	
	for( size_t count = 0; count < foundationTiles; ++count ) {
		
		// Down but not across
		if( (*foundationLayout & 0xC0) && !(*foundationLayout & 0x03))
			height++;

		// Across but not down
		if( (!(*foundationLayout & 0xC0) && *foundationLayout & 0x03))
			width++;

		// Next foundation position
		foundationLayout++;
	}

	// Draw a rectangle
	drawTileSquare( width * 16, height * 16 );

	// Calcualte the screen X/Y of the top-left of the structure
	X = g_DuneEngine->scenarioGet()->mapGet()->posXFromIndex( pMapIndex ) - _mapX;
	Y = g_DuneEngine->scenarioGet()->mapGet()->posYFromIndex( pMapIndex ) - _mapY;

	// Draw the rectangle to the surface
	pSurface->surfacePut( _surfaceTileHighlight, (X << 4),(Y << 4)  );
}

void cScreenPlayfield::drawTileSquare( word pWidth, word pHeight ) {
	dword	White = SDL_MapRGB( _surfaceLandscape->surfaceGet()->format, 255, 255, 255 );
	dword	Black = SDL_MapRGB( _surfaceLandscape->surfaceGet()->format, 0, 0, 0 );

	delete _surfaceTileHighlight;
	_surfaceTileHighlight = new cVideoSurface( pWidth, pHeight );

	_surfaceTileHighlight->colorKeySet( 0 );
	_surfaceTileHighlight->clear(0);

	SDL_Rect dstRect;
	dstRect.x = 0;
	dstRect.y = 0;
	dstRect.w = pWidth;
	dstRect.h = pHeight;
	SDL_FillRect(  _surfaceTileHighlight->surfaceGet(), &dstRect, White);
	
	dstRect.x = 1;
	dstRect.y = 1;
	dstRect.w = pWidth - 2;
	dstRect.h = pHeight - 2;
	SDL_FillRect(  _surfaceTileHighlight->surfaceGet(), &dstRect, Black);
}


bool cScreenPlayfield::mapInScreen( word pMapIndex ) {
	sMapPoint Point = g_DuneEngine->scenarioGet()->mapGet()->mapIndexToXY( pMapIndex );

	return mapInScreen( Point._X, Point._Y );
}

bool cScreenPlayfield::mapInScreen( size_t pX, size_t pY ) {
	size_t	screenX = _mapX, screenY = _mapY;

	screenX <<= 4;
	screenY >>= 2;

	pX >>= 4;
	pY >>= 4;

	pX -= screenX;
	pY -= screenY;

	if( pX < 0xFFF0 || pX > 100 || pY < 0xFFF0)
		return false;

	if( pY <= 0xB0 )
		return true;
	
	return false;
}

void cScreenPlayfield::buttonClear() {

}

void cScreenPlayfield::buttonPressLeft( size_t pX, size_t pY ) {
	cObject		*object =  g_DuneEngine->mPlaceObjectGet();

	// Deactive the current map cell
	(*_mapCell)->objectDeActivate();

	// Convert screen X/Y into Tiles position
	pX >>= 4;
	pY >>= 4;

	// Get the map cell for the position we have clicked
	_mapCell = g_DuneEngine->scenarioGet()->mapGet()->mapCellGet( _mapX + pX, _mapY + pY );

	if( object ) {
		if( typeid(*object) == typeid( cStructure ) ) {
			// Add to the house
			object->houseGet()->structureCreate( object->typeGet(), 256, (*_mapCell)->mapIndexGet() );
		} else {

			object->houseGet()->unitCreate( object->typeGet(), 256, (*_mapCell)->mapIndexGet(), 64, g_DuneEngine->resourcesGet()->actionFind( "Area Guard" ));
		}

		delete object;
		g_DuneEngine->mPlaceObjectSet(0);
		_redraw = true;
	}

	// Activate the map cell
	(*_mapCell)->objectActivate();	
}

void cScreenPlayfield::buttonPressRight( size_t pX, size_t pY ) {

}
void cScreenPlayfield::buttonReleaseLeft( size_t pX, size_t pY ) {

}

void cScreenPlayfield::buttonReleaseRight(size_t pX, size_t pY ) {

}

bool cScreenPlayfield::scrollCheck(size_t X, size_t Y) {
	word		 tilesMaxY = g_DuneEngine->screenTilesMaxY();
	word		 tilesMaxX = g_DuneEngine->screenTilesMaxX();
	bool		 redraw = false;

	// Get mouse column/row (these are half the size of the playfield tiles)
	X /= 2;
	Y /= 2;

	// Left of play field
	if(X < 2)
		if(_mapX > _scale[0]) {
			--_mapX;
			redraw = true;
		}

	// Top of playfield
	if(Y < 2)
		if(_mapY > _scale[1]) {
			--_mapY;
			redraw = true;
		}

	// Right of playfield
	if(X >= ((tilesMaxX-1) * 8))
		if(_mapX < (_scale[2] + _scale[0]) - (tilesMaxX-1)) {
			_mapX++;
			redraw = true;
		}

	// Bottom of playfield
	if(Y >= ((tilesMaxY-1) * 8))
		if(_mapY < (_scale[3] + _scale[1]) - (tilesMaxY-1) ) {
			_mapY++;
			redraw = true;
		}

	if( redraw )
		mapTacticalSet( _mapX, _mapY );

	return redraw;
}

bool cScreenPlayfield::mouseMove(size_t X, size_t Y) {
	bool redraw = false;

	redraw = scrollCheck(X, Y);

	// If the mouse has moved, and we're placing...
	if(g_DuneEngine->mPlaceObjectGet())
		if( _mouseX != X / 16 || _mouseY != Y / 16 )
			redraw = true;

	_mouseX = X / 16;
	_mouseY = Y / 16;

	return redraw;
}

void cScreenPlayfield::mapTacticalSet( short int pX, short int pY ) { 
	if(pX < 0) 
		pX = 0; 
	
	if( pY < 0)
		pY = 0;
	
	_mapX = pX; 
	_mapY = pY;  

	g_DuneEngine->scenarioGet()->mapTacticalSet( g_DuneEngine->scenarioGet()->mapGet()->posXYtoIndex( _mapX, _mapY ) );

}	

				