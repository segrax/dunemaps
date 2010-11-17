class cMap;
class cMapGenerator;
class cUnit;
class cHouse;

class cScenario {
private:
	cMap						*_map;
	cMapGenerator				*_mapGenerator;
	
	size_t						 _mapSeed, _mapScale, _mapTactical, _mapCursor;
	string						 _mapField, _mapBloom, _mapSpecial;

	void						 scenarioMapPrepare();
	bool						 scenarioBegin(  size_t pScenNumber );
	

	void						 unitsLoad();
	void						 structuresLoad();

public:
								 cScenario( );												// Constructor
								~cScenario();																// Destructor

	void						 missionLoad( size_t pScenNumber );
	void						 scenarioLoad( string pFilename );

	void						 mapLoad();
	void						 teamsLoad();
	void						 houseLoad();

	inline cMap					*mapGet()			{ return _map; }
	inline cMapGenerator		*mapGeneratorGet()	{ return _mapGenerator; }

	inline size_t				 mapScaleLevelGet()				{ return _mapScale; }	
	inline void					 mapScaleLevelSet( size_t pVal ){ _mapScale = pVal; }

	inline size_t				 mapSeedGet()					{ return _mapSeed; }
	inline void					 mapSeedSet( size_t pVal )		{ _mapSeed = pVal; }
	
	inline size_t				 mapTacticalGet()				{ return _mapTactical; }
	inline void					 mapTacticalSet( size_t pVal )	{ _mapTactical = pVal; }

	inline size_t				 mapCursorGet()					{ return _mapCursor; }
	inline void					 mapCursorSet( size_t pVal )	{ _mapCursor = pVal; }

	inline string				 mapFieldGet()					{ return _mapField; }
	inline void					 mapFieldSet( string pVal )		{ _mapField = pVal; }

	inline string				 mapBloomGet()					{ return _mapBloom; }
	inline void					 mapBloomSet( string pVal )		{ _mapBloom = pVal; }

	inline string				 mapSpecialGet()					{ return _mapSpecial; }
	inline void					 mapSpecialSet( string pVal )		{ _mapSpecial = pVal; }
};
