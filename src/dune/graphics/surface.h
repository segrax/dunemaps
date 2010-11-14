struct SDL_Surface;

class cVideoSurface {
private:
	bool			 _changed;

	SDL_Surface		*_surface,	*_surfaceScaled;

public:
	
				 cVideoSurface( size_t pWidth, size_t pHeight );
				~cVideoSurface();

	void		 clear( dword color );
	
	dword		 colorGet( byte pR, byte pG, byte pB );
	void		 colorKeySet( byte pR, byte  pG, byte  pB );
	void		 colorKeySet( size_t pKey );

	SDL_Surface *scaleTo( size_t scaleLevel );

	void		 surfacePut( SDL_Surface *pSource, word pX, word pY, word pSourceX = 0, word pSourceY = 0, word pSourceWidth = 0, word pSourceHeight = 0 );
	void		 surfacePut( cVideoSurface *pSource, word pX, word pY, word pSourceX = 0, word pSourceY = 0, word pSourceWidth = 0, word pSourceHeight = 0, size_t pScale = 0 ) {
		if(!pSource)
			return;

		surfacePut( pSource->scaleTo(pScale), pX, pY, pSourceX, pSourceY , pSourceWidth , pSourceHeight );
	}
	
	inline SDL_Surface *surfaceGet() { return _surface; }

};
