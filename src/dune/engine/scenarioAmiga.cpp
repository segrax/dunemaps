#include "stdafx.h"
#include "house.h"
#include "scenario.h"
#include "scenarioAmiga.h"
#include "team.h"

void cScenarioAmiga::iniLoad( string pFile, bool pLocalFile ) {
	size_t size = 0, count = 0;
	bool done = false;

	byte *fileBuffer = g_DuneEngine->resourcesGet()->fileRead( pFile, size, false );
	byte *buffer = fileBuffer;

	count = size;

	while(!done) {

		byte sectionID = *buffer++;
		byte keyID = *buffer++;

		switch(sectionID) {
		
			case 0:				// [BASIC]
				load_Basic( keyID, &buffer );
				break;

			case 1:
				load_Map( keyID, &buffer );
				break;

			case 2:
			case 3:	
			case 4:	
			case 5:
				load_House( sectionID, keyID, &buffer );
				break;

			case 6: {
				sChoam choam;
				choam.mCount = get_word( &buffer );
				choam.mUnitType = g_DuneEngine->resourcesGet()->unitGet( keyID )->Name;
				
				mChoam.push_back( choam );

				break;
					}

			case 7:
				load_Teams( keyID, &buffer );
				break;

			case 8:
				load_Units( keyID, &buffer );
				break;

			case 9:
				load_Structures( keyID, &buffer );
				break;

			case 10:
				load_Reinforcements( keyID, &buffer );
				break;

			case 0xFF:
				done = true;
				break;
		}

	}

	delete fileBuffer;
}

string cScenarioAmiga::get_string( byte **pBuffer ) {

	word		count = readWord(*pBuffer);
	string		str;

	(*pBuffer) += 2;

	//str.append( (char*) *pBuffer, count );

	while(*(*pBuffer)) {
		str.append( 1, (char) **pBuffer );
		(*pBuffer)++;
		--count;
	}
	
	(*pBuffer) += count;

	return str;
}

word cScenarioAmiga::get_word( byte **pBuffer ) {
	byte *value = *pBuffer;

	(*pBuffer) += 2;

	return (value[0] << 8) + value[1];
}

string cScenarioAmiga::get_numberstring( byte **pBuffer ) {
	short int		count = readWord(*pBuffer);
	stringstream	str;

	count += count;

	(*pBuffer) += 2;

	while(count >= 2) {
		count -= 2;

		str << (int) get_word( pBuffer );
		if(count>0)
			str << ",";
	}

	return str.str();
}

void cScenarioAmiga::load_Map( byte keyID, byte **pBuffer ) {
	
	word d0 = keyID - 0x42;

	if( d0 == 0 ) {
		_mapBloom = get_numberstring( pBuffer );

		return;
	}
	
	d0 -= 4;
	// FIELDS
	if( d0 == 0 ) {

		_mapField = get_numberstring( pBuffer );

		return;
	} 

	d0 -= 0x0D;
	// SEED
	if( d0 == 0 ) {
		_mapSeed = get_word( pBuffer );

		mapLoad();
		return;
	}

}

void cScenarioAmiga::load_Basic( byte pKeyID, byte **pBuffer ) {

	switch( pKeyID ) {
		case 0:		// Lose picture
			_pictureLose = get_string( pBuffer );
			break;

		case 1:		// Win
			_pictureWin = get_string( pBuffer );
			break;

		case 2:		// Brief
			_pictureBrief = get_string( pBuffer );
			break;

		case 3:		// Timeout
			_mapTimeOut = get_word( pBuffer );
			break;

		case 4:		// MapScale
			_mapScale = get_word( pBuffer );
			break;

		case 5:		// CursorPos
			_mapCursor = get_word( pBuffer );
			break;

		case 6:		// TacticalPos
			_mapTactical = get_word( pBuffer );
			break;

		case 7:		// Lose Flags
			_mapLoseFlags = get_word( pBuffer );
			break;

		case 8:		// Win Flags
			_mapWinFlags = get_word( pBuffer );
			break;
	}

}

void cScenarioAmiga::load_House( byte sectionID, byte keyID, byte **pBuffer ) {
	word	 houseIDLoad[8] = { 1, 0, 0, 1, 2, 4, 3, 0 };

	cHouse  *house = g_DuneEngine->houseGet( (eHouse) houseIDLoad[ sectionID ] );

	word d4 = 0;

	for( int i = 0; i < 4; ++i ) {
		word d0 = keyID;

		d0 -= 0x42;
		if( d0 == 0 ) {
			//02D7DE0
			d4 = get_word( pBuffer );

		} else {

			d0 -= 1;

			if( d0 == 0 ) {
				//02D7DE8
				house->creditSet( get_word( pBuffer ) );

			} else {

				d0 -= 0x0A;
				if( d0 == 0 ) {
					//02D7DF6
					
					house->maxUnitSet( get_word( pBuffer ) );
				} else {
					d0 -= 0x04;

					if( d0 == 0 ) {
						//2D7E04
						house->creditQuotaSet( get_word( pBuffer ) );

					}
				}
			}
		}

		//2D7E10
		keyID = get_word( pBuffer ) & 0xFF;
	}

	//2D7E1E
	if( d4 == 'H' ) {

		house->brainSet("Human");
		g_DuneEngine->missionHouseSet( house->houseIDGet() );

	} else
		house->brainSet("CPU");

	(*pBuffer) -= 2;
}

void cScenarioAmiga::load_Units( byte keyID, byte **pBuffer ) {
	word arg0 = get_word( pBuffer );
	word arg1 = get_word( pBuffer );
	word d0 = get_word( pBuffer );
	word d1 = get_word( pBuffer );
	word d2 = get_word( pBuffer );
	word d3 = get_word( pBuffer );

	cHouse *House = g_DuneEngine->houseGet( (eHouse) arg0 );

	cUnit *unit = 0;
	if(House)
		unit = House->unitCreate( arg1, d0, d1, d2, d3 );

}

void cScenarioAmiga::load_Structures( byte keyID, byte **pBuffer ) {
	word arg0 = get_word( pBuffer );
	word arg1 = get_word( pBuffer );
	word d0 = get_word( pBuffer );
	word d1 = 0;
	word d2 = 0;

	if( keyID == 'G' ) {
		// Health
		d1 = 256;

		// Map Index
		d2 = arg0;
	} else {
		d1 = get_word( pBuffer );
		d2 = get_word( pBuffer );
	}

	cHouse *House = g_DuneEngine->houseGet( (eHouse) arg1 );

	cStructure *structure = 0;
	if(House)
		structure = House->structureCreate( d0, d1, d2 );


}

void cScenarioAmiga::load_Teams( byte keyID, byte **pBuffer ) {
	word house = get_word( pBuffer );
	word findMode = get_word( pBuffer );

	word foundMode = 0;

	bool found = false;
	
	for( int i = 0; i < 5; ++i ) {
		string mode = g_DuneEngine->resourcesGet()->aiModeGet( i );

		if( mode[0] == (char) findMode ) {
			found = true;
			foundMode = i;
			break;
		}
	}
	if(!found)
		return;

	found = false;
	word findMovement = get_word( pBuffer );
	word foundMove = 0;

	for( int i = 0; i < 6; ++i ) {
		string move = g_DuneEngine->resourcesGet()->movementNameGet(i);

		if( move[0] == (char) findMovement ) {
			foundMove = i;
			found = true;
			break;
		}
	}
	if(!found)
		return;

	word min = get_word( pBuffer );
	word max = get_word( pBuffer );
	
	mTeams.push_back( new cTeam( g_DuneEngine->houseGet( (eHouse) house ), foundMode, foundMove, min, max ));
}

void cScenarioAmiga::load_Choam( byte keyID, byte **pBuffer ) {

}

void cScenarioAmiga::load_Reinforcements( byte keyID, byte **pBuffer ) {
	sReinforcement reinforce;
	
	reinforce.mHouse = (eHouse) get_word( pBuffer );
	reinforce.mUnitType = get_word( pBuffer );
	reinforce.mDirection = get_word( pBuffer );

	reinforce.mTime = *(*pBuffer)++;
	byte repeat = *(*pBuffer)++;
	
	if( repeat == '+' )
		reinforce.mRepeat = true;
	else
		reinforce.mRepeat = false;

	mReinforcements.push_back( reinforce );
}
