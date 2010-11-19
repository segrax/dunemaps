#include "../stdafx.h"
#include "objects/object.h"
#include "objects/unit.h"
#include "objects/structure.h"
#include "team.h"

cTeam::cTeam( cHouse *pHouse, size_t pAiMode, size_t pMovementType, word arg_6, word arg_8 ) {
	
	_house = pHouse;
	_aiMode = pAiMode;

	_movementType = pMovementType;

	_unk1 = arg_6;
	_unitsMax = arg_8;

}
