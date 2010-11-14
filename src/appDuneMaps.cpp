//---------------------------------------------------------------------------
//
// Name:        DuneMapsApp.cpp
// Author:      Robert
// Created:     14/11/2010 08:04:04
// Description: 
//
//---------------------------------------------------------------------------

#include "appDuneMaps.h"
#include "..\..\src\frameDuneMaps.h"
#include "stdafx.h"

IMPLEMENT_APP(cDuneMapsApp)

bool cDuneMapsApp::OnInit() {
	g_DuneEngine = new cDune( "data" );
	g_DuneEngine->resourcesGet()->resourcePrepare();
	g_DuneEngine->scenarioLoad(eHouse_Atreides, 1);

	cFrameDuneMaps* dialog = new cFrameDuneMaps(0);

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

	return wxBitmap((const char*)surface->pixels, surface->w, surface->h ,surface->format->BitsPerPixel);
}
