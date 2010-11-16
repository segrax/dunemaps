///-----------------------------------------------------------------
///
/// @file      frameDuneMaps.cpp
/// @author    Robert
/// Created:   14/11/2010 08:04:04
/// @section   DESCRIPTION
///            cFrameDuneMaps class implementation
///
///------------------------------------------------------------------

#include "frameDuneMaps.h"
#include "panelSurface.h"
#include "appDuneMaps.h"

#include "stdafx.h"
#include "dune\engine\objects\object.h"
#include "dune\engine\objects\structure.h"
#include "dune\engine\objects\unit.h"
#include "eastwood\PakFile.h"

#include <algorithm>

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// cFrameDuneMaps
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(cFrameDuneMaps,wxFrame)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(cFrameDuneMaps::OnClose)
	EVT_SIZE(cFrameDuneMaps::OnSize)
	EVT_MENU(ID_MNU_NEWSCENARIO_1005, cFrameDuneMaps::Mnunewscenario1005Click)
	EVT_MENU(ID_MNU_QUIT_1006, cFrameDuneMaps::Mnuquit1006Click)
	EVT_TOOL_RANGE(ID_WXTOOLBAR2,ID_WXTOOLBAR2_End, cFrameDuneMaps::WxToolBar2Tool)
	EVT_TOOL_RANGE(ID_WXTOOLBAR1,ID_WXTOOLBAR1_End, cFrameDuneMaps::WxToolBar1Tool)
	EVT_MENU_RANGE(ID_MNU_SCEN, ID_MNU_SCEN_End, cFrameDuneMaps::MnuLoadPak_ScenClick)

END_EVENT_TABLE()
////Event Table End

cFrameDuneMaps::cFrameDuneMaps(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size)
: wxFrame(parent, id, title, position, size)
{
	mHouse = eHouse_Atreides;
	CreateGUIControls();
}

cFrameDuneMaps::~cFrameDuneMaps()
{
} 

void cFrameDuneMaps::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxToolBar1 = new wxToolBar(this, ID_WXTOOLBAR1, wxPoint(0, 0), wxSize(732, 28));

	WxMenuBar1 = new wxMenuBar();
	wxMenu *ID_MNU_FILE_1001_Mnu_Obj = new wxMenu(0);
	ID_MNU_FILE_1001_Mnu_Obj->Append(ID_MNU_NEWSCENARIO_1005, wxT("&New Scenario"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILE_1001_Mnu_Obj->AppendSeparator();
	
	wxMenu *ID_MNU_LOADSCENARIO_1002_Mnu_Obj = new wxMenu(0);
	
	wxMenu *ID_MNU_LOADSCENFROMPAK_Mnu_Obj = new wxMenu(0);
	ID_MNU_LOADSCENFROMPAK_Mnu_Obj->Append(ID_MNU_SCEN, wxT("SCEN"), wxT(""), wxITEM_NORMAL);
	ID_MNU_LOADSCENARIO_1002_Mnu_Obj->Append(ID_MNU_LOADSCENFROMPAK, wxT("From Pak"), ID_MNU_LOADSCENFROMPAK_Mnu_Obj);
	ID_MNU_LOADSCENARIO_1002_Mnu_Obj->AppendSeparator();
	ID_MNU_FILE_1001_Mnu_Obj->Append(ID_MNU_LOADSCENARIO_1002, wxT("&Load Scenario"), ID_MNU_LOADSCENARIO_1002_Mnu_Obj);
	ID_MNU_FILE_1001_Mnu_Obj->Append(ID_MNU_QUIT_1006, wxT("Quit"), wxT(""), wxITEM_NORMAL);
	WxMenuBar1->Append(ID_MNU_FILE_1001_Mnu_Obj, wxT("&File"));
	SetMenuBar(WxMenuBar1);

	WxToolBar1->Realize();
	SetToolBar(WxToolBar1);
	SetTitle(wxT("Dune Maps"));
	SetIcon(wxNullIcon);
	SetSize(8,8,758,477);
	
	////GUI Items Creation End

	this->SetBackgroundColour( wxColor(0.0L) );

	mTileView = new cPanelSurface( this );

	mTileView->SetPosition( wxPoint( 12, 10 ) );
	mTileView->SetSize( GetSize().GetWidth() - 40, GetSize().GetHeight() - 126 );
	
	SetMinSize( wxSize(400, 300) );

	
	WxToolBar2 = new wxToolBar(this, ID_WXTOOLBAR2, wxPoint(0, 460), wxSize(404, 28), wxTB_BOTTOM);
	WxToolBar2->Realize();

	loadToolbarStructures();
	loadToolbarUnits();
	loadScenariosFromPak();
}

void cFrameDuneMaps::loadScenariosFromPak() {

	wxMenuItem *mnuItemPak = FindItemInMenuBar( ID_MNU_LOADSCENFROMPAK );
	wxMenu *mnuPakScenarios = mnuItemPak->GetSubMenu();

	mnuPakScenarios->Delete(ID_MNU_SCEN);

	PakFile *pak = g_DuneEngine->resourcesGet()->pakGet("SCENARIO.PAK");

	int count = pak->getNumFiles();

	for( int i = 0; i < count; ++i ) {
	
		string name = pak->getFileName( i );

		std::transform( name.begin(), name.end(), name.begin(), tolower );

		if( name.find("scen") != string::npos )
			mnuPakScenarios->Insert(mnuPakScenarios->GetMenuItemCount(), ID_MNU_SCEN + i, name );
	}

}

void cFrameDuneMaps::loadToolbarUnits() {
	cVideoSurface surface(32, 24);

	for( int id = 0; id < 18; ++id ) {
		
		sUnitData *unitData = g_DuneEngine->resourcesGet()->unitGet( id );
		SDL_Surface *shp = g_DuneEngine->resourcesGet()->shpGet( unitData->sidebarIconID );

		surface.surfacePut( shp, 0, 0 );
		
		WxToolBar2->AddTool(ID_WXTOOLBAR2 + id, wxT(""), SDL_To_Bitmap(surface.scaleTo(1)), wxNullBitmap, wxITEM_NORMAL, unitData->Name, wxT("") );
	}

	WxToolBar2->Realize();
}

void cFrameDuneMaps::loadToolbarStructures() {

	cVideoSurface surface(32, 24);

	for( int id = 0; id < 19; ++id ) {
		
		sStructureData *buildingData = g_DuneEngine->resourcesGet()->structureGet( id );
		SDL_Surface *shp = g_DuneEngine->resourcesGet()->shpGet( buildingData->sidebarIconID );

		surface.surfacePut( shp, 0, 0 );
		
		WxToolBar1->AddTool(ID_WXTOOLBAR1 + id, wxT(""), SDL_To_Bitmap(surface.scaleTo(1)), wxNullBitmap, wxITEM_NORMAL, buildingData->Name, wxT("") );
	}

	WxToolBar1->Realize();
}

void cFrameDuneMaps::OnClose(wxCloseEvent& /*event*/) {
	Destroy();
}

void cFrameDuneMaps::OnSize(wxSizeEvent& event) {
	size_t width = event.GetSize().GetWidth();
	size_t height = event.GetSize().GetHeight();

	if(mTileView)
		mTileView->SetSize( width - 40, height - 130 );

	WxToolBar2->SetPosition( wxPoint( 0, height - 126 ) );
	WxToolBar2->Refresh();
	Refresh(false);
}

/*
 * Mnufile1001Click
 */
void cFrameDuneMaps::Mnufile1001Click(wxCommandEvent& event)
{
	// insert your code here
}

/*
 * Mnuloadscenario1002Click
 */
void cFrameDuneMaps::Mnuloadscenario1002Click(wxCommandEvent& event)
{
	// insert your code here
}

/*
 * Mnunewscenario1005Click
 */
void cFrameDuneMaps::Mnunewscenario1005Click(wxCommandEvent& event)
{
	// insert your code here
}

/*
 * Mnuquit1006Click
 */
void cFrameDuneMaps::Mnuquit1006Click(wxCommandEvent& event)
{
	// insert your code here
}

/*
 * WxToolBar1Tool
 */
void cFrameDuneMaps::WxToolBar1Tool(wxCommandEvent& event) {
	int structID = event.GetId() - ID_WXTOOLBAR1;

	cHouse *house = g_DuneEngine->houseGet( mHouse );
	cObject *object = g_DuneEngine->mPlaceObjectGet();

	// Delete any structure attempting to be placed
	if(object)
		delete object;

	g_DuneEngine->mPlaceObjectSet( new cStructure( house, structID, 0) );

}

/*
 * WxToolBar2Tool
 */
void cFrameDuneMaps::WxToolBar2Tool(wxCommandEvent& event) {
	int unitID = event.GetId() - ID_WXTOOLBAR2;

	cHouse *house = g_DuneEngine->houseGet( mHouse );
	cObject *object = g_DuneEngine->mPlaceObjectGet();

	// Delete any unit attempting to be placed
	if(object)
		delete object;

	g_DuneEngine->mPlaceObjectSet( new cUnit( house, unitID, 0, 0, 0 ) );
}

/*
 * MnuLoadPak_ScenClick
 */
void cFrameDuneMaps::MnuLoadPak_ScenClick(wxCommandEvent& event) {
	wxMenuItem *item = WxMenuBar1->FindItem( event.GetId() );

	string filename = item->GetText();

	g_DuneEngine->scenarioLoad( filename );

	mTileView->playfieldSizeUpdate();
}
