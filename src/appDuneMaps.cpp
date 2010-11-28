//---------------------------------------------------------------------------
//
// Name:        DuneMapsApp.cpp
// Author:      Robert
// Created:     14/11/2010 08:04:04
// Description: 
//
//---------------------------------------------------------------------------

#include "appDuneMaps.h"
#include "frameDuneMaps.h"
#include "stdafx.h"

IMPLEMENT_APP(cDuneMapsApp)

bool cDuneMapsApp::OnInit() {
	g_DuneEngine = new cDune( "data" );

	g_DuneEngine->resourcesGet()->resourcePrepare();
	g_DuneEngine->scenarioLoad(eHouse_Atreides, 1);

	cFrameDuneMaps* dialog = new cFrameDuneMaps(0);

	g_DuneEngine->frameSet( dialog );

	SetTopWindow(dialog);
	dialog->Show(true);		

	return true;
}
 
int cDuneMapsApp::OnExit() {

	delete g_DuneEngine;
	g_DuneEngine = 0;

	return 0;
}

wxBitmap SDL_To_Bitmap(SDL_Surface *surface) {

#ifdef WIN32
	return wxBitmap((const char*)surface->pixels, surface->w, surface->h ,surface->format->BitsPerPixel);
#else
	wxImage image(surface->w, surface->h, true);

	for(int y = 0 ; y < surface->h; y++) {
		for(int x = 0; x < surface->w; x++) {
			Uint32 color = *((Uint32*) ((unsigned char*) surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel));
			Uint8 r,g,b;
			SDL_GetRGB(color, surface->format, &r, &g, &b);
			image.SetRGB(x,y,r, g, b);
		}
	}

	return wxBitmap(image);
#endif
}
