class cMapCell;

class cScreenPlayfield {
private:
	cVideoSurface		*_surfaceTileHighlight;
	cVideoSurface		*_surfaceLandscape;							// Current playfield surface
	cVideoSurface		*_surfaceUnits;								// Current unit playfield overlay
	
	size_t				 _width, _height;
	word				 _mapX,		 _mapY;							// New Map X-Y		(Top-Left of the playfield)
	word				 _landscapeX,_landscapeY;					// Currently drawn '_surfaceLandscape' Map X-Y (Top-Left of the playfield)
	word				 _mouseX, _mouseY;

	bool				 _redraw;

	cMapCell		   **_mapCell;									// Current selected map cell									
	word				*_scale;									// Current map scale

			
	void				 drawLandscape();							// Draw landscape to '_surfaceLandscape'
	void				 drawObjects();								// Draw units to '_surfaceUnits'
	void				 drawTileSquares(cVideoSurface *pSurface);							// 
	void				 drawTileSquare( word pMapX, word pMapY );	//
	void				 drawTileSquares( cVideoSurface *pSurface, word pMapIndex, cStructure *pStructure );

	bool				 scrollCheck(size_t X, size_t Y);

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

	inline void			 mapTacticalSet( short int pX, short int pY ) { if(pX < 0) pX = 0; if( pY < 0) pY = 0; _mapX = pX; _mapY = pY;  }		// Set playfield map location

	inline word			 mapXGet()							{ return _mapX; }
	inline word			 mapYGet()							{ return _mapY; }

	inline void			 widthSet( size_t pVal ) {_redraw = true;
													_width = pVal; }

	inline void			 heightSet( size_t pVal ) { _redraw = true;
													_height = pVal; }

	void				 scaleSet();
	inline word			 *scaleGet() { return _scale; }

};
