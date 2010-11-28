#include "stdafx.h"

extern "C"
{
#include "graphics/scale/scalebit.h"
}
		
cVideoSurface::cVideoSurface( size_t pWidth, size_t pHeight ) {

	 _surface = SDL_CreateRGBSurface(SDL_SWSURFACE, pWidth, pHeight, 32, 0, 0, 0, 0);
	
	 _surfaceScaled = 0;

	_changed = false;
	
	//surfaceWipe();
}

cVideoSurface::~cVideoSurface() {

	// Free the old surface
	SDL_FreeSurface( _surface );
	if(_surfaceScaled)
		SDL_FreeSurface( _surfaceScaled );
}

void cVideoSurface::clear( dword color ) {
		
	SDL_FillRect( _surface, NULL, color );

	if(_surfaceScaled)
		SDL_FillRect( _surfaceScaled, NULL, color );
}

dword cVideoSurface::colorGet( byte pR, byte pG, byte pB ) {
	
	return SDL_MapRGB( _surface->format, pR, pG, pB );
}

void cVideoSurface::colorKeySet( byte pR, byte  pG, byte  pB ) {

	SDL_SetColorKey( _surface, SDL_SRCCOLORKEY, SDL_MapRGB( _surface->format, pR, pG, pB)	);
}

void cVideoSurface::colorKeySet( size_t pKey ) {

	SDL_SetColorKey( _surface, SDL_SRCCOLORKEY, pKey);
}

SDL_Surface *cVideoSurface::scaleTo( size_t scaleLevel ) {
	// Invalid Scale?
	if(scaleLevel < 2 || scaleLevel > 4)
		return _surface;

	if(!_surfaceScaled) {
		// Set our new dimensions
		_surfaceScaled = SDL_CreateRGBSurface(SDL_SWSURFACE, _surface->w * scaleLevel, _surface->h * scaleLevel, 32, 0, 0, 0, 0);
		_changed = true;
	}

	if(!_changed && _surfaceScaled)
		return _surfaceScaled;

	_changed = false;

	SDL_SetColorKey(	_surfaceScaled, SDL_SRCCOLORKEY, SDL_MapRGB(_surfaceScaled->format, 0, 0, 0)	);

	// Do the scale
	scale(scaleLevel, _surfaceScaled->pixels, _surfaceScaled->pitch, _surface->pixels, _surface->pitch, _surface->format->BytesPerPixel, _surface->w, _surface->h);

	return _surfaceScaled;
}

void cVideoSurface::surfacePut( SDL_Surface *pSource, word pX, word pY, word pSourceX, word pSourceY, word pSourceWidth, word pSourceHeight ) {
	SDL_Rect posSrc, posDst;
	
	_changed = true;

	posSrc.x = pSourceX;
	posSrc.y = pSourceY;

	if(!pSourceWidth)
		posSrc.w = pSource->w;
	else
		posSrc.w = pSourceWidth;
	
	if(!pSourceHeight)
		posSrc.h = pSource->h;
	else
		posSrc.h = pSourceHeight;

	posDst.x = pX;
	posDst.y = pY;

	SDL_BlitSurface( pSource, &posSrc, _surface, &posDst );
	
}

