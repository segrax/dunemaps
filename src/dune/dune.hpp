class cResources;
class cScenario;
class cHouse;
class cScreenPlayfield;
class cStructure;
class cUnit;
class cObject;

class cDune {

	size_t					 mTilesMaxY, mTilesMaxX;

	cResources				*_resources;
	cScenario				*_scenario;	
	cScreenPlayfield		*_screenPlayfield;

	map< eHouse, cHouse* >	 _houses;

	eHouse					 _missionHouse;
	cObject					*mPlaceObject;

public:
					 cDune( string pDataPath );
					~cDune();

	inline void				 missionHouseSet( eHouse pHouse ) { _missionHouse = pHouse; }

	inline cObject			*mPlaceObjectGet() { return mPlaceObject; }
	inline void				 mPlaceObjectSet( cObject *pStructure ) { mPlaceObject = pStructure; }

	bool			 execute();												// Main execution loop
	
	cHouse			*houseGet( eHouse pHouse );
	void			 houseMapPrepare();
	void			 houseReset();

	void			 scenarioLoad( eHouse pHouse, size_t pScenNumber );
	void			 scenarioLoad( string pFilename );

	inline eHouse			 missionHouseGet()  { return _missionHouse; }

	inline cScenario		*scenarioGet()		{ return _scenario; }				// Get the scenario
	inline cScreenPlayfield	*screenPlayfieldGet() { return _screenPlayfield; }

	inline cResources		*resourcesGet()		{ return _resources; }				// Get the resources

	inline void				 screenTilesMaxXSet( size_t pScreenWidth ) {
		mTilesMaxX = (pScreenWidth / 16) + 1;
	}

	inline void			 screenTilesMaxYSet( size_t pScreenHeight ) {
		mTilesMaxY = (pScreenHeight / 16) + 1;
	}

	inline size_t			 screenTilesMaxY() { return mTilesMaxY; }
	inline size_t			 screenTilesMaxX() { return mTilesMaxX; }
};
