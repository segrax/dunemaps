#include "../stdafx.h"
#include "objects/object.h"
#include "objects/unit.h"
#include "objects/structure.h"
#include "house.h"

cHouse::cHouse(  eHouse pHouse )  {
	
	_houseID	= pHouse;
	_houseData	= g_DuneEngine->resourcesGet()->houseGet( pHouse );

	_palette	= 0;

	palletePrep();
}

cHouse::~cHouse() {
	reset();

	if(_palette)
		delete _palette->colors;
	delete _palette;
}

void cHouse::palletePrep() {
	SDL_Palette *palette = g_DuneEngine->resourcesGet()->paletteGet();

	_palette			= new SDL_Palette();
	_palette->colors	= new SDL_Color[palette->ncolors];
	_palette->ncolors	= palette->ncolors;

	memcpy( _palette->colors, palette->colors, _palette->ncolors * sizeof(SDL_Color) );

	size_t color = colorGet();

	for(size_t i = 0; i < 0x10; ++i) {
		_palette->colors[ 0x90 + i ].b = _palette->colors[ color + i].b;
		_palette->colors[ 0x90 + i ].g = _palette->colors[ color + i].g;
		_palette->colors[ 0x90 + i ].r = _palette->colors[ color + i].r;
	}
}

cUnit *cHouse::unitCreate(  word pType, word pHealthPercent, word pMapIndex, byte pAngle, byte pAction ) {
	cUnit		*created = 0;
	sUnitData	*pUnitData = g_DuneEngine->resourcesGet()->unitGet( pType );

	if(!pUnitData)
		return 0;
	
	created = new cUnit( this, pType, pMapIndex, pAngle, pAction );

	_units.insert( pair< size_t, cUnit*> ( pType, created ) );

	return created;
}

cStructure *cHouse::structureCreate(word pType, word pHealthPercent, word pMapIndex ) {
	cStructure		*created = 0;
	sStructureData	*pStructData = g_DuneEngine->resourcesGet()->structureGet( pType );

	if(!pStructData)
		return 0;
	
	created = new cStructure( this, pType, pMapIndex );

	_structures.insert( pair< size_t, cStructure*> ( pType, created ) );

	return created;
}

void cHouse::reset() {
	multimap< size_t, cUnit* >::iterator		unitIT;
	multimap< size_t, cStructure* >::iterator	structIT;

	for( unitIT = _units.begin(); unitIT != _units.end(); ++unitIT ) 
		delete unitIT->second;

	for( structIT = _structures.begin(); structIT != _structures.end(); ++structIT ) 
		delete structIT->second;
}

void cHouse::mapPrepare() {
	multimap< size_t, cUnit* >::iterator		unitIT;
	multimap< size_t, cStructure* >::iterator	structIT;

	for( unitIT = _units.begin(); unitIT != _units.end(); ++unitIT ) 
		;

	for( structIT = _structures.begin(); structIT != _structures.end(); ++structIT ) {
		structIT->second->frameChange(2);
		
	}

}

void cHouse::buildingsHave( word pTypeIndex, bool pOn )	{ 

	pTypeIndex = 1 << pTypeIndex;	

	if(pOn)		
		_buildingsHave |= pTypeIndex;
	else
		_buildingsHave &= ~pTypeIndex;
}
