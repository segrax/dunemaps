#ifndef EASTWOOD_CPSFILE_H
#define EASTWOOD_CPSFILE_H

#include "Decode.h"

struct SDL_Palette;
struct SDL_Surface;

class CpsFile : public Decode
{
public:
	CpsFile(std::istream &stream, SDL_Palette *palette = NULL);
	virtual ~CpsFile();

	SDL_Surface *getSurface();
};

#endif // EASTWOOD_CPSFILE_H
