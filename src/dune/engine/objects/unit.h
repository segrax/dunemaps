enum eUnitOptions {
	_UnitOpts_AirUnitShadow			= 0x1,
	_UnitOpts_2						= 0x2,
	_UnitOpts_4						= 0x4,
	_UnitOpts_8						= 0x8,
	_UnitOpts_10					= 0x10,
	_UnitOpts_WormCamo				= 0x20,
	_UnitOpts_TurretHasAngle		= 0x40,
	_UnitOpts_80					= 0x80,
	_UnitOpts_CarryAllPickupRepairs	= 0x100,
	_UnitOpts_NoAttackedSound		= 0x200,
	_UnitOpts_ForceScriptCycles		= 0x800,
	_UnitOpts_1000					= 0x1000,
	_UnitOpts_TargetAirUnit			= 0x2000
};

class cUnitAngle {
public:
	byte					 _TurnTo,	_Target,	_Current;

							 cUnitAngle() {
								_TurnTo = _Target = _Current;
							 }
};

class cUnit : public cObject {

private:
	sUnitData				*_UnitData;

	cUnitAngle				 _angleBase,	_angleTop;

	word					 _actionCurrent;

public:
							 cUnit(  cHouse *pHouse, word pType, word pMapIndex, byte pAngle, word pAction );

	void					 cycle();
	void					 draw();
	void					 drawBase();
	void					 drawTurret();

	void					 angleSet( byte pAngle, bool pTurnTo, cUnitAngle *pAngles );
	
	cUnitAngle				*angleTopGet() { return &_angleTop; }
	cUnitAngle				*angleBaseGet() { return &_angleBase; }

	inline void				 actionSet( word pAction ) { _actionCurrent = pAction; }
	inline word				 actionGet() { return _actionCurrent; }

	bool					 objectEnter( cObject	*pObject );									// 'object' enters this->object
	cObject					*objectActivate();													// map-cell has been selected by player
	void					 objectDeActivate();

	inline sUnitData		*dataGet()		{ return _UnitData; }
};
