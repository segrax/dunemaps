struct sHouseData {
	string houseName;
	string houseVoice;

	word Weakness;
	word LemonFactor;
	word buildingDecay;
	word color;
	word PalaceUnitRecharge;
	word FrigateTime;
	char houseLetter[2];
	word PalaceMode;
	word missionWinMusic;
	word missionLoseMusic;
	word missionBriefMusic;
};

struct sData {
	word typeIndex;
	string Name;
};

struct sStructureData : sData {
	word idShort;
	word idLong;
	string Picture;
	word  buildOpts;
	word  infantrySpawn;
	word  HitPoints;
	word  Sight;    
	word  sidebarIconID;
	word  Cost;
	word  BuildTime;
	word  Techlevel;
	dword PreReqs;
	byte  buildOrder;
	byte CYUpgradesNeeded;
	byte field_22;
	byte field_23;
	byte field_24;
	byte field_25;
	byte field_26;
	byte field_27;
	byte field_28;
	byte field_29;
	byte field_2A;
	word structureID;
	word weaponDamage;
	word weaponDamageAdd;
	byte Owner;
	word RepairFacility;
	word UnitsCanEnter;
	word SpiceStorage;
	word PowerUsage;
	word FoundationSize;
	word structureGfxID;
	word field_3E;
	word field_40;
	word field_42;
	word field_44;
	word field_46;
	word field_48;
	word constructOpt1;
	word constructOpt2;
	word constructOpt3;
	word constructOpt4;
	word constructOpt5;
	word constructOpt6;
	word constructOpt7;
	word constructOpt8;
	word techUpgrade1;
	word techUpgrade2;
	word techUpgrade3;
};

struct sUnitData : sData {
	word stringID;        
	word stringLongID;    
	string Picture;
	word options1;    
	word infantrySpawn;         
	word HitPoints;       
	word Sight;           
	word sidebarIconID;   
	word Cost;            
	word buildTime;       
	word TechLevel;       
	dword PreReqs;        
	byte buildOrder;        
	byte UpgradesNeeded;  
	word sidebarCommand1; 
	word sidebarCommand2; 
	word sidebarCommand3; 
	word sidebarCommand4; 
	byte field_2A;        
	word field_2B;        
	word field_2D;        
	word Aggressivity;    
	byte Owner;           
	word indexMin;        
	word indexMax;        
	word optsFitW;        
	byte field_38;        
	byte field_39;        
	word field_3A;        
	word MovementType;    
	word field_3E;        
	word Speed;
	word turningSpeed; 
	word UnitGfxID;
	word turretGfxID;
	word AICommand;
	word frameAngleMode;
	word deathAnim;
	word weaponRateFire;
	word weaponRange;
	word weaponDamage;
	word field_54;  
	word weaponType;
	word weaponSound;
};

struct sFileData {
	string fileName;
	word  field_4;
	word  field_6;
	word  field_8;
	word  field_A;
	word  field_C;
	word  field_E;
	byte  parentIndex;
	byte  field_11;
	byte  fileType;
};

struct sActionData {
	byte field_0;
	byte field_1;
	string Name;
	byte field_6;
	byte field_7;
	byte sidebarMode;
	byte field_9;
	byte field_A;
	byte field_B;
};

class IniFile;
class IcnFile;
class PakFile;
class PalFile;
class ShpFile;
class WsaFile;

class sScriptEmc;

class cResources {
private:
	string							 _DataPath;											// Path to game data files
	cResourceExe					*_Exe;												// Loaded EXE

	map< eHouse, sHouseData* >		 _dataHouses;										// Loaded hardcoded house data
	map< size_t, sStructureData*>	 _dataStructures;									// Loaded hardcoded structure data
	map< size_t, sUnitData*>		 _dataUnits;										// Loaded hardcoded unit data
	map< size_t, sActionData*>		 _dataActions;
	map< string, sScriptEmc*>		 _dataEmc;
	map< string, WsaFile*>			 _dataWSA;

	map< string, PakFile*>							 _Paks;								// All game PAKs
	map< string, IniFile::KeyListHandle>			 _IniHandles;

	map< word, SDL_Surface* >		 _Tiles;											// In-game tile images
	map< word, SDL_Surface* >		 _dataSHP;											// In-game graphics


	map< size_t, vector<size_t>*>	 _dataIconFrames;									// Loaded frames

	byte							 _angleAdjust[256];

	size_t							 _fileIconICNSize,	_fileIconMAPSize, _fileIniSize;	// Size of loaded files
	byte							*_fileIconICN, *_fileIconMAP;						// Ptr to ICON.ICN/MAP byte buffers

	IniFile							*_ini;												// Current loaded 'INI' scenario
	IcnFile							*_icons;											// Loaded map Icons
	ShpFile							*_shp;												// Current SHP 
	PalFile							*_paletteIBM;										// IBM Palette
	SDL_Palette						*_palIBM;

	word							 _tileFogWar, _tileBloom, _tileConcrete, _tileWall;	// Tile Reference IDs
	word							 _tileLand;											// 

	void							 angleAdjustBuild();
	string							 languageAdd( string pFilename );

	void							 pakLoad();														// Load all PAKs into '_Paks' map
	void							 mapTileIDsLoad();												// Load the Map-Tiles IDs into '_tile' variables

	void							 shpLoad();														// Load the 4 SHP files into _dataSHP
	void							 shpLoad( string Filename  );// Load the SHP file into _dataSHP

public:

									 cResources( string pPath );
									~cResources();

	size_t							 aiModeFind( string pName ) ;
	size_t							 movementNamesGet( string pName );

	SDL_Surface						*CpsGet( string fileName ) ;

	string							*fileRead( string pFileName);
	byte							*fileRead( string pFile, size_t	&pFileSize );		// Read from a file on disk
	byte							*fileRead( istream *pStream, size_t &pSize );		// Read from an istream
	istream							*fileOpen( string pFilename );						
	
	void							 IniLoad( string fileName );						// Load an ini in

	string							 IniStringGet( string pSection, string pVariable, string pDefault);
	size_t							 IniNumGet( string pSection, string pVariable, size_t pDefault );

	void							 IniSectionOpen( string pSection );
	string							 IniSectionNext( string pSection );
	void							 IniSectionClose(string pSection );

	word							 actionFind( string pName );
	sActionData						*actionGet( word pIndex );

	sScriptEmc						*emcGet( string pFilename );

	word							 houseFind( string pName );
	sHouseData						*houseGet( eHouse pHouse );

	word							 structureFind( string pName );
	sStructureData					*structureGet( size_t pIndex );

	word							 unitFind( string pName );
	sUnitData						*unitGet( size_t pIndex );
	
	word							*foundationMapModGet( size_t pIndex );
	word							 foundationSizeGet( size_t pIndex );
	sFileData						*fileTableGet( size_t pIndex );								// 

	byte							*mapOffsetsGet( size_t pIndex );
	byte							*mapTerrainSpiceGet();
	byte							*mapSinTableGet();
	byte							*mapCosTableGet();

	word							*mapScaleGet( word pIndex );
	
	size_t							 mapIcnCount( word pIndex );
	vector<size_t>					*mapIcnFrameSetGet( word pindex );

	byte							*mapIconPtrsGet( word pIndex );
	SDL_Surface						*mapTileGet( word pTileIndex );
	
	word							*unitAngleFrameAdjustGet( size_t pIndex );
	word							*unitAngleFrame3AdjustGet( size_t pIndex );

	word							*unitAngleFrameFlagsGet( size_t pIndex ) ;
	word							*unitAngleFrame3FlagsGet( size_t pIndex );

	byte							*unitFrameAdjustGet( size_t pIndex );
	word							 unitSiegeTurretFrameGet( size_t pIndex, bool addOne );
	word							 unitDevastatorFrameGet( size_t pIndex, bool addOne );

	SDL_Palette						*paletteGet() { return _palIBM; }
	SDL_Surface						*shpGet( word pIndex );									// Don't free these surfaces

	SDL_Surface						*wsaGet( string pFileName, size_t pFrame );

	void							 resourcePrepare();

	inline SDL_Color				*colorGet( word color ) { return &_palIBM->colors[color]; }

	inline word						 tileFogWar()						{ return _tileFogWar;	}
	inline word						 tileBloom()						{ return _tileBloom;	}
	inline word						 tileConcrete()						{ return _tileConcrete; }
	inline word						 tileWall()							{ return _tileWall;		}
	inline word						 tileLand()							{ return _tileLand;		}

	inline byte						 mapTileColorGet( word pTile )		{ return *(_Exe->mapTileColorGet() + (pTile << 1)); }

	inline byte						 angleAdjustGet( size_t pIndex )	{ return  _angleAdjust[ pIndex & 0xFF ] & 0x07; }

	PakFile							*pakGet( string pPakName );
};

