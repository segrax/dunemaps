#include "../../stdafx.h"
#include "object.h"
#include "unit.h"
#include "../../map.h"
#include "mapCell.h"
#include "../house.h"
#include "../../screenPlayfield.h"

cUnit::cUnit(  cHouse *pHouse, word pType, word pMapIndex, byte pAngle, word pAction  ) : cObject(pHouse, pMapIndex) {

	_surface = new cVideoSurface( 32, 32 );
	_surface->colorKeySet( 0xFF );

	_objectType = pType;
	_UnitData = g_DuneEngine->resourcesGet()->unitGet( _objectType );

	_actionCurrent = pAction;

	// Put the unit in the mapcell
	if(pMapIndex)
		(*mapCellGet())->objectEnter( this );

	angleSet(pAngle, false, &_angleBase);
	angleSet(pAngle, false, &_angleTop);

	_X = 0;
	_Y = 0;
}

void cUnit::cycle() {
	
}

void cUnit::draw() {

	if(mapIndexGet() == 0xFFFF)
		return;

	_surface->clear( 0xFF );
	
	if( _UnitData->UnitGfxID != 0xFFFF)
		drawBase();

	// Is there a turret?
	if(_UnitData->turretGfxID != 0xFFFF)
		drawTurret();

	// If this object is selected, draw the selected cursor
	if(objectSelected()) {
		SDL_Surface *surface = g_DuneEngine->resourcesGet()->shpGet( 6, 0 );
		_surface->surfacePut( surface, _X, _Y );
	}
}

void cUnit::drawBase() {
	byte		 angle		= g_DuneEngine->resourcesGet()->angleAdjustGet( _angleBase._Current );
	word		*adjust		= 0,	*flag = 0;

	word		 frameAdd	= 0, flags = 0;
	SDL_Surface *surface	= 0;
	
	word destX, destY;

	destX = _X;
	destY = _Y;

	switch( _UnitData->frameAngleMode ) {

	case 0:
		break;
	case 1:
	case 2: {
		if( _UnitData->MovementType == 5)		// Underground
			break;
			
		adjust = g_DuneEngine->resourcesGet()->unitAngleFrameAdjustGet( angle );
		flag   = g_DuneEngine->resourcesGet()->unitAngleFrameFlagsGet( angle );

		frameAdd = *adjust;
		flags = *flag;
		break;
			}

	case 3: {
		adjust = g_DuneEngine->resourcesGet()->unitAngleFrame3AdjustGet( angle );
		flag   = g_DuneEngine->resourcesGet()->unitAngleFrame3FlagsGet( angle );

		frameAdd = (*adjust) * 3;
		frameAdd += (*g_DuneEngine->resourcesGet()->unitFrameAdjustGet( _frame & 0x03 ));

		flags = *flag;
		break;
			}
	case 4:
		adjust = g_DuneEngine->resourcesGet()->unitAngleFrame3AdjustGet( angle );
		flag   = g_DuneEngine->resourcesGet()->unitAngleFrame3FlagsGet( angle );

		frameAdd = (*adjust) << 2;
		frameAdd += (*g_DuneEngine->resourcesGet()->unitFrameAdjustGet( _frame & 0x03 ));

		flags = *flag;
		break;
	}

	surface = g_DuneEngine->resourcesGet()->shpGet( _UnitData->UnitGfxID + frameAdd, flags & 1 );
	SDL_SetColors(surface, _house->paletteGet()->colors, 0, _house->paletteGet()->ncolors);
	
	if(surface->w == 8) {
		destX += 4;
		destY += 4;
	}

	_surface->surfacePut( surface, destX, destY);
}

void cUnit::drawTurret() {
	byte		 angle = 0;
	word		*adjust = 0;
	short int	 destX = 0, destY = 0;

	// Can this turret rotate seperate from the base?
	if( _UnitData->options1 & _UnitOpts_TurretHasAngle )
		angle		= g_DuneEngine->resourcesGet()->angleAdjustGet( _angleTop._Current );
	else
		angle		= g_DuneEngine->resourcesGet()->angleAdjustGet( _angleBase._Current );

	adjust = g_DuneEngine->resourcesGet()->unitAngleFrameAdjustGet( angle );
	word flag  = *g_DuneEngine->resourcesGet()->unitAngleFrameFlagsGet( angle );

	// This doesn't match the diassembly perfectly, not sure how the images where lined up... but this works for now
	switch( _UnitData->turretGfxID ) {
		case 0x7E:		// Siege Tank

			destX = g_DuneEngine->resourcesGet()->unitSiegeTurretFrameGet( *adjust, false );
			destY = g_DuneEngine->resourcesGet()->unitSiegeTurretFrameGet( *adjust, true );
			break;

		case 0x88:		// Devastator
			destX = g_DuneEngine->resourcesGet()->unitDevastatorFrameGet( *adjust, false );
			destY = g_DuneEngine->resourcesGet()->unitDevastatorFrameGet( *adjust, true);
			break;

		case 0x8D:		// Sonic Tank
			destY = -2;
			break;

		case 0x92:		// Launcher
			destY = -3;
			break;

		default:;
	}

	SDL_Surface *surface = g_DuneEngine->resourcesGet()->shpGet( _UnitData->turretGfxID + *adjust, flag & 1 );
	SDL_SetColors(surface, _house->paletteGet()->colors, 0, _house->paletteGet()->ncolors);
	
	if(surface->w == 0xa) {
		destX += 3;
		destY += 3;
	}

	_surface->surfacePut( surface, destX, destY );
}

void cUnit::angleSet( byte pAngle, bool pTurnTo, cUnitAngle *pAngles ) {
	short int	angle;

	pAngles->_TurnTo = 0;
	pAngles->_Target = pAngle;
	
	if(!pTurnTo) {
		pAngles->_Current = pAngle;
		return;
	}

	if( pAngles->_Current == pAngle )
		return;

	pAngles->_TurnTo = (_UnitData->turningSpeed << 4);
	
	angle = pAngle;
	angle -= pAngles->_Current;

	// FIXME: this is probably slightly wrong...
	// seg031:1F3F
	if(angle > -128) {
		if( angle > 0 )
			if(angle <= 128)
				return;

	} else
		if(angle <= 128)
			return;

	pAngles->_TurnTo = !pAngles->_TurnTo;
}

bool cUnit::objectEnter( cObject	*pObject ) {

	return false;
}

cObject *cUnit::objectActivate() {
	
	// Play sound here 'unit reporting', etc

	objectSelect(true);
	//g_DuneEngine->screenGet()->sidebarGet()->selectUnit( this );
	draw();

	return this;
}

void cUnit::objectDeActivate() {
	objectSelect(false);
	//g_DuneEngine->screenGet()->sidebarGet()->selectClear();
	draw();
}
