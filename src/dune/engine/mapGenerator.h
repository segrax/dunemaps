enum eTerrain {
	TERRAIN_SAND		= 0,
	TERRAIN_DUNES		= 2,
	TERRAIN_ROCK		= 4,
	TERRAIN_MOUNTAIN	= 6,
	TERRAIN_SPICE		= 8,
	TERRAIN_SPICELOTS	= 9,
};

class cMap;

class cMapGenerator  {
private:
	size_t				 _Seed,		_SeedRand;
	byte				 _map[65 * 65];

	cMap				*_cMap;											// Ptr to main map

	void				generateFinalise();								// Finish the map by converting to the 'icon.map' tile ids
	void				generateSpiceAdd();								// Add spice to the array
	void				generateSpiceAddTo( word mapIndex );			// Add spice .to map location
	void				generateTerrainAverage();						// Average cells with their 8 neighbouring cells
	void				generateTerrainBuild();							// Turn the map array into the terrain ids
	void				generateToTiles();								// Convert the map to the tile icon ids

public:
						 cMapGenerator(  );

	void				 generate( size_t pSeed );						// Generate the map based on 'pSeed'
	
	size_t				 seedRandomGet();								// Generate a ''random'' number from the seed, and update seed

	inline byte			*mapGet( word pMapIndex ) { return &_map[ pMapIndex ]; }
	sMapPoint				 mapXYCorrect( sMapPoint pPoint, word pRandMax, bool pAdd80 );

};
