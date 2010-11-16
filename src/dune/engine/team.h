
class cTeam {
private:
	cHouse							*_house;

	word							 _movementType;
	word							 _aiMode;
	word							 _unk1, _unitsMax;

public:
									 cTeam( cHouse *pHouse, size_t pAiMode, size_t pMovementType, word arg_6, word arg_8 );

	inline word						 aiModeGet() { return _aiMode; }
	inline void						 aiModeSet( word pMode ) { _aiMode = pMode; }

	inline void						 unk1Set( word pValue )	{ _unk1 = pValue; }
	inline word						 unk1Get()				{ return _unk1; }

	inline void						 unitsMaxSet( word pValue )	{ _unitsMax = pValue; }
	inline word						 unitsMaxGet()				{ return _unitsMax; }

	inline cHouse					*houseGet() { return _house; }

	inline word						 movementTypeGet()  { return _movementType; }
};
