
#include<windows.h>

#include<vector>
#include<string>
#include<map>
#include<sstream>
#include<iostream>
#include<iomanip>

using namespace std;

#include <sdl.h>
#include "types.h"

#include "graphics/scale/scalebit.h"
#include "graphics/surface.h"

class cDune;
extern cDune *g_DuneEngine;

enum eHouse {
	eHouse_Harkonnen = 0,
	eHouse_Atreides,
	eHouse_Ordos,
	eHouse_Fremen,
	eHouse_Sardaukar,
	eHouse_Mercenaries,

	eHouse_End
};

struct sMapPoint {
	word		_X, _Y;

	sMapPoint( word pX = 0, word pY = 0 ) { _X = pX; _Y = pY; }
};

inline word	readWord( const void *buffer ) {
	const byte *wordByte = (const byte *) buffer;
	return (wordByte[0] << 8) + wordByte[1];
}

#include "eastwood/IniFile.h"

#include "resourceExe.h"
#include "resources.h"

#include "dune.hpp"
