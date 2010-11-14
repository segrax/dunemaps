//---------------------------------------------------------------------------
//
// Name:        DuneMapsApp.cpp
// Author:      Robert
// Created:     14/11/2010 08:04:04
// Description: 
//
//---------------------------------------------------------------------------

#include "appDuneMaps.h"
#include "..\..\src\dialogDuneMaps.h"
#include "stdafx.h"

IMPLEMENT_APP(cDuneMapsApp)

bool cDuneMapsApp::OnInit() {
	g_DuneEngine = new cDune( "data" );
	g_DuneEngine->resourcesGet()->resourcePrepare();
	g_DuneEngine->scenarioLoad(eHouse_Atreides, 1);

	cDialogDuneMaps* dialog = new cDialogDuneMaps(0);

	SetTopWindow(dialog);
	dialog->Show(true);		

	return true;
}
 
int cDuneMapsApp::OnExit() {

	delete g_DuneEngine;
	g_DuneEngine = 0;

	return 0;
}
