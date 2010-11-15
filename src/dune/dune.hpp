class cResources;
class cScenario;
class cHouse;
class cScreenPlayfield;
class cStructure;
class cUnit;

class cDune {

	size_t					 mTilesMaxY, mTilesMaxX;

	cResources				*_resources;
	cScenario				*_scenario;	
	cScreenPlayfield		*_screenPlayfield;

	map< eHouse, cHouse* >	 _houses;

	eHouse					 _missionHouse;
	cStructure				*mPlaceStructure;
	cUnit					*mPlaceUnit;

public:
					 cDune( string pDataPath );
					~cDune();

	inline cStructure*		 mPlaceStructureGet() { return mPlaceStructure; }
	inline void		 mPlaceStructureSet( cStructure *pStructure ) { mPlaceStructure = pStructure; }
	
	inline cUnit*		 mPlaceUnitGet() { return mPlaceUnit; }
	inline void		 mPlaceUnitSet( cUnit *pUnit ) { mPlaceUnit = pUnit; }

	bool			 execute();												// Main execution loop
	
	cHouse			*houseGet( eHouse pHouse );
	void			 houseMapPrepare();

	void			 scenarioLoad( eHouse pHouse, size_t pScenNumber );

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
