class cResources;
class cScenario;
class cHouse;
class cScreenPlayfield;
class cStructure;
class cUnit;
class cObject;
class cMinimap;
class cPanelSurface;
class cFrameDuneMaps;

enum eScenarioLoad {
	eLoad_PC			= 0,
	eLoad_PC_Pak,
	eLoad_Amiga,
};

class cDune {

	size_t					 mTilesMaxY, mTilesMaxX;

	cMinimap				*_minimap;
	cResources				*_resources;
	cScenario				*_scenario;	
	cScreenPlayfield		*_screenPlayfield;
	cFrameDuneMaps			*mFrame;

	map< eHouse, cHouse* >	 _houses;

	eHouse					 _missionHouse;
	cObject					*mPlaceObject;
	
	cPanelSurface			*mSurface;
					
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

	void			 scenarioNew( string pSeed );
	void			 scenarioLoad( eHouse pHouse, size_t pScenNumber );
	void			 scenarioLoad( string pFilename, eScenarioLoad pLoad );

	inline cMinimap			*minimapGet()		{ return _minimap; }
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

	inline cPanelSurface	*mSurfaceGet() { return mSurface; }
	inline void				 mSurfaceSet( cPanelSurface	*pVal ) { mSurface = pVal; }


	inline cFrameDuneMaps	*frameGet() { return mFrame; }
	inline 	void frameSet( cFrameDuneMaps *pVal ) { mFrame = pVal; }
};
