class cMapCell;

class cScreenPlayfield {
private:
	cVideoSurface		*_surfaceTileHighlight;
	cVideoSurface		*_surfaceLandscape;							// Current playfield surface
	cVideoSurface		*_surfaceUnits;								// Current unit playfield overlay
	
	size_t				 _width, _height;
	word				 _mapX,		 _mapY;							// New Map X-Y		(Top-Left of the playfield)
	word				 _landscapeX,_landscapeY;					// Currently drawn '_surfaceLandscape' Map X-Y (Top-Left of the playfield)

	bool				 _redraw;

	cMapCell		   **_mapCell;									// Current selected map cell									
	word				*_scale;									// Current map scale

			
	void				 drawLandscape();							// Draw landscape to '_surfaceLandscape'
	void				 drawObjects();								// Draw units to '_surfaceUnits'
	void				 drawTileSquares(cVideoSurface *pSurface);							// 
	void				 drawTileSquare( word pMapX, word pMapY );	//

public:

						 cScreenPlayfield();
						~cScreenPlayfield();

			void		 screenPrep();
	virtual void		 draw( cVideoSurface *pSurface );

	virtual bool		 mouseMove( size_t pX, size_t pY );

			void		 buttonClear();
	virtual void		 buttonPressLeft( size_t pX, size_t pY );
	virtual void		 buttonPressRight(size_t pX, size_t pY );
	virtual void		 buttonReleaseLeft( size_t pX, size_t pY );
	virtual void		 buttonReleaseRight(size_t pX, size_t pY );

	bool				 mapInScreen( word mapIndex );
	bool				 mapInScreen( size_t pX, size_t pY);

	inline void			 mapTacticalSet( word pX, word pY ) { _mapX = pX; _mapY = pY;  }		// Set playfield map location

	inline word			 mapXGet()							{ return _mapX; }
	inline word			 mapYGet()							{ return _mapY; }

	inline void			 widthSet( size_t pVal ) {	if(_width != pVal)
														_redraw = true;
													_width = pVal; }

	inline void			 heightSet( size_t pVal ) { if(_height != pVal)
														_redraw = true;
													_height = pVal; }
};
