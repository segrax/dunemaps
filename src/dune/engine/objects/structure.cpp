#include "../../stdafx.h"
#include "object.h"
#include "structure.h"
#include "mapCell.h"
#include "../house.h"

cStructure::cStructure(  cHouse *pHouse, word pType, word pMapIndex, word pHealthPercent ) : cObject(pHouse, pMapIndex, pHealthPercent ) {

	_buildingItemIndex = 0;
	_frame = _frames = 0;
	_foundationTiles = _tileCount = 0;

	_objectType = pType;
	_structData = g_DuneEngine->resourcesGet()->structureGet( pType );

	frameLoad();
	
	if(mapIndexGet() == 0xFFFF)
		return;

	_frame = _frames - 1;
	if((short int)_frame < 0)
		_frame=  0;

	mapTilesSetup( false );
}

cStructure::~cStructure() {
	mapTilesSetup( true );
}

void cStructure::frameLoad() {
	if(_structData->structureGfxID == 0xFFFF)
		return;

	_tileCount		 = g_DuneEngine->resourcesGet()->mapIcnCount( _structData->structureGfxID );
	_tileIDs		 = g_DuneEngine->resourcesGet()->mapIcnFrameSetGet( _structData->structureGfxID );
	
	_foundationTiles = g_DuneEngine->resourcesGet()->foundationSizeGet( _structData->FoundationSize );

	_frames			 = _tileCount / _foundationTiles;
}

void cStructure::buildingItemSet( size_t pIndex ) {
	_buildingItemIndex = pIndex;
}

void cStructure::frameChange( size_t pFrame ) {
	
	if(pFrame < _frames) {
		_frame = pFrame;
		mapTilesSetup( false );
	}

}

void cStructure::mapTilesSetup( bool pClear ) {
	size_t		  tileStart		= _frame * _foundationTiles;
	cMapCell	**mapCell		= 0;
	word		 *foundation	= 0;

	if(!pClear && _structData->structureGfxID == 0xFFFF)
		return;

	houseGet()->buildingsHave( _structData->typeIndex, true );

	foundation	= g_DuneEngine->resourcesGet()->foundationMapModGet( _structData->FoundationSize );
	
	// Put each structure tile in its respective map position
	for( size_t tileCount = tileStart; tileCount < (tileStart + _foundationTiles); ++tileCount ) {
		
		mapCell  = mapCellGet();
		mapCell += *foundation;
		
		++foundation;

		if( pClear )
			(*mapCell)->tileSetCurrent( 0 );

		else {

			if( typeGet() != 1) 
				(*mapCell)->tileSetCurrent( this, (*_tileIDs)[tileCount] );
			else
				(*mapCell)->tileSetCurrent( this, g_DuneEngine->resourcesGet()->tileConcrete() );
		}

	}	

}

bool cStructure::objectEnter( cObject	*pObject ) {				// 

	return true;
}

cObject *cStructure::objectActivate() {											//
	
	objectSelect(true);
//	g_DuneEngine->screenGet()->sidebarGet()->selectStructure( this );
	draw();

	return this;
}

void cStructure::objectDeActivate() {
	objectSelect(false);
	//g_DuneEngine->screenGet()->sidebarGet()->selectClear();
	draw();
}
