
struct sGameVersions {
	size_t	offName;							// 'DUNE2.EXE' location
	size_t	offUnits, offStructs, offHouses;	// Units / Structures/ Houses

	size_t  offReferences;						// Subtraction from string references
	
	size_t	offMapOffsets, offTerrainSpice;		// Terrain Generation / Spice Generation data
	size_t	offSin, offCos;						// Precalculated sin/cos 
	size_t	offFiles;							// File Table
	size_t	offMapScales;						// Map scale levels

	size_t	offMapTileColors;					// Minimap Tile Colors
	size_t	offActions;							// Unit Action Table
	size_t  offFoundationSizes;					// Foundation Sizes
	size_t  offFoundationMapMod;				// Map-Foundation Modifier (structure tile placement)

	size_t	offUnitAngleFrameAdjust;			// Unit-gfx Angle Frame adjustment
	size_t  offUnitFrameAdjust;					// Unit-gfx frame adjust
	size_t	offUnitSiegeTurretFrameAdjust;

	size_t	offMovementNames;
};

struct sExeHouseData {
	#pragma pack(1)
	dword houseName;
	word Weakness;
	word LemonFactor;
	word buildingDecay;
	word color;
	word PalaceUnitRecharge;
	word FrigateTime;
	word houseLetter;
	word PalaceMode;
	word missionWinMusic;
	word missionLoseMusic;
	word missionBriefMusic;
	dword houseVoice;
};

struct sExeStructureData {
	#pragma pack(1)
	word idShort;
	dword Name;
	word idLong;
	dword Picture;
	word  buildOpts;
	word  infantrySpawn;			// Chance to spawn infantry on building destruction
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
struct sExeUnitData {
	#pragma pack(1)
	word stringID;        
	dword Name;
	word stringLongID;    
	dword Picture;
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

struct sExeFileData {
	#pragma pack(1)
	dword fileName;
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

struct sExeActionData {
	#pragma pack(1)
	byte field_0;
	byte field_1;
	dword Name;
	byte field_6;
	byte field_7;
	byte sidebarMode;
	byte field_9;
	byte field_A;
	byte field_B;
};

class cResourceExe {
private:
	byte					*_bufferExe;
	const sGameVersions		*_version;
	
	bool					 versionCheck();

public:
							 cResourceExe( const char *pExe );
							~cResourceExe();

	sExeActionData			 actionGet( size_t pIndex );
	sExeHouseData			 houseGet( eHouse pHouse );								// Get house data at index
	sExeStructureData		 structureGet( size_t pIndex );							// Get structure data at index
	sExeUnitData			 unitGet( size_t pIndex);								// Get unit data at index
	

	sExeFileData			*fileTableGet( );

	word					*foundationSizesGet( );
	word					*foundationMapModGet( );

	byte					*mapOffsetsGet( size_t pIndex );						// Get the map generator offsets
	byte					*mapTerrainSpiceGet();
	
	byte					*mapSinTableGet();
	byte					*mapCosTableGet();

	byte					*mapTileColorGet();
	byte					*mapScalesGet();
	
	byte					*movementNames();

	byte					*unitAngleFrameAdjustGet();
	byte					*unitFrameAdjustGet();
	byte					*UnitSiegeTurretFrameGet();

	string					 stringGet( size_t	pAddress );							// Takes an internal string reference ptr and returns the string
};
