class cMapCell;
class cHouse;

class cObject {

private:
	word			 _mapIndex;															// Current map position
	bool			 _objectSelected,		_redraw;
	
protected:
	word			 _objectType;
	word			 _X, _Y, _frame;													// Draw offset from top-left of tile
	cHouse			*_house;
	cVideoSurface	*_surface;															// Current final image

public:
					 cObject( cHouse *pHouse, word pX, word pY);
					 cObject( cHouse *pHouse, word pMapIndex);
					~cObject();

	virtual void	 cycle();
	virtual void	 draw();

	cMapCell	   **mapCellGet();														// Pointer-to-Pointer for our map cell

	bool			 redrawGet()					{ return _redraw; }
	void			 redrawSet( bool val = true )	{ _redraw = val;  }

	inline cHouse	*houseGet()						{ return _house; }
	inline bool		 objectSelected()				{ return _objectSelected; }
	inline void		 objectSelect( bool val = true) { _objectSelected = val; }

	virtual void	 objectDeActivate() = 0;
	virtual cObject	*objectActivate() = 0;									// Object is activated by user
	virtual bool	 objectEnter( cObject *pObject ) = 0;					// Object enters

	inline void		 mapIndexSet( word pMapIndex )	{ _mapIndex = pMapIndex; }	// 
	inline word		 mapIndexGet()					{ return _mapIndex; }		// Current map position

	inline cVideoSurface	*surfaceGet()	{ return _surface; }
	inline word				 typeGet()		{ return _objectType; }
};
