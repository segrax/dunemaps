enum eStructureOptions {
	eStructureOptions_HasChoam			= 2,
	eStructureOptions_4					= 4,
	eStructureOptions_NoConcreteNeeded  = 8,
	eStructureOptions_UnitDeploy		= 0x10,
	eStructureOptions_20				= 0x20,
	eStructureOptions_40				= 0x40,
	eStructureOptions_CanCapture		= 0x80
};


class cStructure : public cObject {

private:
	sStructureData		*_structData;
	
	word				 _foundationTiles;												// number of tiles per frame 
	word				 _frames;														// number of frames

	word				 _buildingItemIndex;											// Index of current building item

	word				 _tileCount;

	vector<size_t>		*_tileIDs;														// tile ids of each frame

	void				 frameLoad();

public:
						 cStructure(  cHouse *pHouse, word pType, word pMapIndex, word pHealth );
						~cStructure();

	void				 buildingItemSet( size_t pIndex );
	inline word			 buildingItemGet()					{ return _buildingItemIndex; }

	void				 frameChange( size_t pFrame );
	
	void				 mapTilesSetup( bool pClear );

	bool				 objectEnter( cObject	*pObject );									// 'object' enters this->object
	cObject				*objectActivate();													// map-cell has been selected by player
	void				 objectDeActivate();

	inline sStructureData *dataGet()		{ return _structData; }
};
