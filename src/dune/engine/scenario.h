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
	void						 teamsLoad();
	void						 houseLoad();

	inline cMap					*mapGet()			{ return _map; }
	inline cMapGenerator		*mapGeneratorGet()	{ return _mapGenerator; }

	inline size_t				 mapCursorGet()					{ return _mapCursor; }
	inline size_t				 mapScaleLevelGet()				{ return _mapScale; }
	inline size_t				 mapTacticalGet()				{ return _mapTactical; }

};
