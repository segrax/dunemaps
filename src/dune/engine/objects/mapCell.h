
class cMapCell : public cObject {

private:
	size_t			 _tileID;												// Landscape Tile
	size_t			 _tileTempID;											// Tile to display

	cObject			*_object;

public:
					 cMapCell( word pMapIndex );
					~cMapCell();

	bool			 hasUnit();															// Are we holding a unit
	bool			 hasStructure();													// Are we holding a structure
	
	bool			 isEmpty();															// Is the cell empty
	
	void			 tileMapLoad();

	bool			 objectEnter( cObject	*pObject );									// 'object' enters this->object
	cObject			*objectActivate();													// map-cell has been selected by player
	void			 objectDeActivate();
	inline cObject	*objectGet()							{ return _object; }			// get the 'object'

	void			 tileSetCurrent( cObject *pObject, size_t pTileTemp = 0xFFFFFFFF );	// Set the temporary tile (structure tile for instance)
	inline size_t	 tileGet() {														// Get the tileID
		 
		if( _tileTempID != 0xFFFFFFFF )
			return _tileTempID;

		return _tileID;
	}
	inline void		 tileSet( size_t pNewTile ) {

		_tileID = pNewTile;
	}

};
