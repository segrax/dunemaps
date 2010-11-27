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
#include "panelMinimap.h"
#include "appDuneMaps.h"
#include "dialogHouses.h"
#include "dialogBasics.h"
#include "dialogTeams.h"
#include "dialogReinforcements.h"
#include "dialogNewScenario.h"
#include "dialogChoam.h"

#include "stdafx.h"
#include "dune\engine\objects\object.h"
#include "dune\engine\objects\structure.h"
#include "dune\engine\objects\unit.h"
#include "eastwood\PakFile.h"
#include "dune\engine\scenario.h"

#include <algorithm>
#include "../rev.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
#include "Images/Self_cFrameDuneMaps_XPM.xpm"
////Header Include End

//----------------------------------------------------------------------------
// cFrameDuneMaps
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(cFrameDuneMaps,wxFrame)
	////Manual Code Start
	EVT_TOOL_RANGE(ID_WXTOOLBAR2,ID_WXTOOLBAR2_End, cFrameDuneMaps::WxToolBar2Tool)
	EVT_TOOL_RANGE(ID_WXTOOLBAR1,ID_WXTOOLBAR1_End, cFrameDuneMaps::WxToolBar1Tool)
	EVT_MENU_RANGE(ID_MNU_SCEN, ID_MNU_SCEN_End, cFrameDuneMaps::MnuLoadPak_ScenClick)
	EVT_MOVE(cFrameDuneMaps::OnMove)
	////Manual Code End
	
	EVT_CLOSE(cFrameDuneMaps::OnClose)
	EVT_SIZE(cFrameDuneMaps::OnSize)
	EVT_MENU(ID_MNU_NEWSCENARIO_1005, cFrameDuneMaps::Mnunewscenario1005Click)
	EVT_MENU(ID_MNU_FROMINI_7000, cFrameDuneMaps::Mnuloadscenario1002Click)
	EVT_MENU(ID_MNU_FROMAMIGA_7001, cFrameDuneMaps::Mnufromamiga7001Click)
	EVT_MENU(ID_MNU_SAVESCENARIO_1007, cFrameDuneMaps::Mnusavescenario1007Click)
	EVT_MENU(ID_MNU_LOADSCENARIOPAK_7005, cFrameDuneMaps::Mnuloadscenariopak7005Click)
	EVT_MENU(ID_MNU_QUIT_1006, cFrameDuneMaps::Mnuquit1006Click)
	EVT_MENU(ID_MNU_HARKONNEN_4006, cFrameDuneMaps::MnuHouseChange)
	EVT_MENU(ID_MNU_ATREIDES_4007, cFrameDuneMaps::MnuHouseChange)
	EVT_MENU(ID_MNU_ORDOS_4008, cFrameDuneMaps::MnuHouseChange)
	EVT_MENU(ID_MNU_FREMEN_4009, cFrameDuneMaps::MnuHouseChange)
	EVT_MENU(ID_MNU_SARDAUKA_4010, cFrameDuneMaps::MnuHouseChange)
	EVT_MENU(ID_MNU_MERCENARIES_4011, cFrameDuneMaps::MnuHouseChange)
	EVT_MENU(ID_MNU_BASICS_4003, cFrameDuneMaps::Mnubasics4003Click)
	EVT_MENU(ID_MNU_CHOAM_4017, cFrameDuneMaps::Mnuchoam4017Click)
	EVT_MENU(ID_MNU_HOUSES_4016, cFrameDuneMaps::Mnuhouses4016Click)
	EVT_MENU(ID_MNU_REINFORCEMENTS_4015, cFrameDuneMaps::Mnureinforcements4015Click)
	EVT_MENU(ID_MNU_TEAMS_4014, cFrameDuneMaps::Mnuteams4014Click)
	EVT_MENU(ID_MNU_MINIMAPFOLLOWSWINDOW_7003, cFrameDuneMaps::Mnuminimapfollowswindow7003Click)
END_EVENT_TABLE()
////Event Table End

cFrameDuneMaps::cFrameDuneMaps(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size)
: wxFrame(parent, id, title, position, size)
{
	mHouse = eHouse_Harkonnen;
	CreateGUIControls();

	mMinimapLock = true;
	mScensFromPak = 1;

	string windowTitle = "Dune Maps (SVN:";
	windowTitle.append( SVNREV );
	windowTitle.append( " - " );
	windowTitle.append( SVNDATE );
	windowTitle.append( ")");

	SetTitle( windowTitle );

	// Prepare the tool bars and the PAK load menu
	loadToolbarStructures();
	loadToolbarUnits();
	loadScenariosFromPak();
}

cFrameDuneMaps::~cFrameDuneMaps()
{
	delete WxToolBar2;
	delete mTileView;
	delete WxOpenFileDialog1;
	delete WxSaveFileDialog1;
	delete mMinimap;

} 

void cFrameDuneMaps::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxToolBar1 = new wxToolBar(this, ID_WXTOOLBAR1, wxPoint(0, 0), wxSize(740, 28));

	WxOpenFileDialog1 =  new wxFileDialog(this, wxT("Choose a file"), wxT(""), wxT(""), wxT("SCEN*.INI"), wxFD_OPEN);

	WxMenuBar1 = new wxMenuBar();
	wxMenu *ID_MNU_FILE_1001_Mnu_Obj = new wxMenu(0);
	ID_MNU_FILE_1001_Mnu_Obj->Append(ID_MNU_NEWSCENARIO_1005, wxT("&New Scenario"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILE_1001_Mnu_Obj->AppendSeparator();
	
	wxMenu *ID_MNU_LOADSCENARIO_1002_Mnu_Obj = new wxMenu(0);
	ID_MNU_LOADSCENARIO_1002_Mnu_Obj->Append(ID_MNU_FROMINI_7000, wxT("From Ini"), wxT(""), wxITEM_NORMAL);
	ID_MNU_LOADSCENARIO_1002_Mnu_Obj->Append(ID_MNU_FROMAMIGA_7001, wxT("From Amiga"), wxT(""), wxITEM_NORMAL);
	ID_MNU_LOADSCENARIO_1002_Mnu_Obj->AppendSeparator();
	
	wxMenu *ID_MNU_LOADSCENFROMPAK_Mnu_Obj = new wxMenu(0);
	ID_MNU_LOADSCENFROMPAK_Mnu_Obj->Append(ID_MNU_SCEN, wxT("SCEN"), wxT(""), wxITEM_NORMAL);
	ID_MNU_LOADSCENARIO_1002_Mnu_Obj->Append(ID_MNU_LOADSCENFROMPAK, wxT("From Pak"), ID_MNU_LOADSCENFROMPAK_Mnu_Obj);
	ID_MNU_FILE_1001_Mnu_Obj->Append(ID_MNU_LOADSCENARIO_1002, wxT("&Load Scenario"), ID_MNU_LOADSCENARIO_1002_Mnu_Obj);
	ID_MNU_FILE_1001_Mnu_Obj->Append(ID_MNU_SAVESCENARIO_1007, wxT("&Save Scenario"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILE_1001_Mnu_Obj->AppendSeparator();
	ID_MNU_FILE_1001_Mnu_Obj->Append(ID_MNU_LOADSCENARIOPAK_7005, wxT("Load Scenario &PAK"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILE_1001_Mnu_Obj->Append(ID_MNU_QUIT_1006, wxT("Quit"), wxT(""), wxITEM_NORMAL);
	WxMenuBar1->Append(ID_MNU_FILE_1001_Mnu_Obj, wxT("&File"));
	
	wxMenu *ID_MNU_HOUSE_4005_Mnu_Obj = new wxMenu(0);
	ID_MNU_HOUSE_4005_Mnu_Obj->Append(ID_MNU_HARKONNEN_4006, wxT("Harkonnen"), wxT(""), wxITEM_RADIO);
	ID_MNU_HOUSE_4005_Mnu_Obj->Check(ID_MNU_HARKONNEN_4006,true);
	ID_MNU_HOUSE_4005_Mnu_Obj->Append(ID_MNU_ATREIDES_4007, wxT("Atreides"), wxT(""), wxITEM_RADIO);
	ID_MNU_HOUSE_4005_Mnu_Obj->Check(ID_MNU_ATREIDES_4007,false);
	ID_MNU_HOUSE_4005_Mnu_Obj->Append(ID_MNU_ORDOS_4008, wxT("Ordos"), wxT(""), wxITEM_RADIO);
	ID_MNU_HOUSE_4005_Mnu_Obj->Check(ID_MNU_ORDOS_4008,false);
	ID_MNU_HOUSE_4005_Mnu_Obj->Append(ID_MNU_FREMEN_4009, wxT("Fremen"), wxT(""), wxITEM_RADIO);
	ID_MNU_HOUSE_4005_Mnu_Obj->Check(ID_MNU_FREMEN_4009,false);
	ID_MNU_HOUSE_4005_Mnu_Obj->Append(ID_MNU_SARDAUKA_4010, wxT("Sardauka"), wxT(""), wxITEM_RADIO);
	ID_MNU_HOUSE_4005_Mnu_Obj->Check(ID_MNU_SARDAUKA_4010,false);
	ID_MNU_HOUSE_4005_Mnu_Obj->Append(ID_MNU_MERCENARIES_4011, wxT("Mercenaries"), wxT(""), wxITEM_RADIO);
	ID_MNU_HOUSE_4005_Mnu_Obj->Check(ID_MNU_MERCENARIES_4011,false);
	WxMenuBar1->Append(ID_MNU_HOUSE_4005_Mnu_Obj, wxT("&House"));
	
	wxMenu *ID_MNU_SCENARIO_4001_Mnu_Obj = new wxMenu(0);
	ID_MNU_SCENARIO_4001_Mnu_Obj->Append(ID_MNU_BASICS_4003, wxT("Basics"), wxT(""), wxITEM_NORMAL);
	ID_MNU_SCENARIO_4001_Mnu_Obj->Append(ID_MNU_CHOAM_4017, wxT("Choam"), wxT(""), wxITEM_NORMAL);
	ID_MNU_SCENARIO_4001_Mnu_Obj->Append(ID_MNU_HOUSES_4016, wxT("Houses"), wxT(""), wxITEM_NORMAL);
	ID_MNU_SCENARIO_4001_Mnu_Obj->Append(ID_MNU_REINFORCEMENTS_4015, wxT("Reinforcements"), wxT(""), wxITEM_NORMAL);
	ID_MNU_SCENARIO_4001_Mnu_Obj->Append(ID_MNU_TEAMS_4014, wxT("Teams"), wxT(""), wxITEM_NORMAL);
	WxMenuBar1->Append(ID_MNU_SCENARIO_4001_Mnu_Obj, wxT("&Scenario"));
	
	wxMenu *ID_MNU_OPTIONS_7002_Mnu_Obj = new wxMenu(0);
	ID_MNU_OPTIONS_7002_Mnu_Obj->Append(ID_MNU_MINIMAPFOLLOWSWINDOW_7003, wxT("Minimap Lock"), wxT(""), wxITEM_CHECK);
	ID_MNU_OPTIONS_7002_Mnu_Obj->Check(ID_MNU_MINIMAPFOLLOWSWINDOW_7003,true);
	WxMenuBar1->Append(ID_MNU_OPTIONS_7002_Mnu_Obj, wxT("Options"));
	SetMenuBar(WxMenuBar1);

	WxSaveFileDialog1 =  new wxFileDialog(this, wxT("Choose a file"), wxT(""), wxT(""), wxT("SCEN*.INI"), wxFD_SAVE);

	WxToolBar1->Realize();
	SetToolBar(WxToolBar1);
	SetTitle(wxT("Dune Maps"));
	SetIcon(Self_cFrameDuneMaps_XPM);
	SetSize(8,8,756,550);
	
	////GUI Items Creation End

	wxMenuItem *menuItem = WxMenuBar1->FindItem(ID_MNU_MINIMAPFOLLOWSWINDOW_7003);
	menuItem->SetCheckable(true);
	menuItem->Check(true);

	this->SetBackgroundColour( wxColor(0.0L) );

	mTileView = new cPanelSurface( this );
	mMinimap = new cPanelMinimap( this );

	g_DuneEngine->mSurfaceSet( mTileView );

	mTileView->SetPosition( wxPoint( 12, 10 ) );
	mTileView->SetSize( GetSize().GetWidth() - 40, GetSize().GetHeight() - 126 );
	
	SetMinSize( wxSize(756, 550) );

	WxToolBar2 = new wxToolBar(this, ID_WXTOOLBAR2, wxPoint(0, 460), wxSize(800, 28), wxTB_BOTTOM);
	WxToolBar2->Realize();

	mMinimap->Show();
}

void cFrameDuneMaps::loadScenariosFromPak() {

	wxMenuItem *mnuItemPak = FindItemInMenuBar( ID_MNU_LOADSCENFROMPAK );
	wxMenu *mnuPakScenarios = mnuItemPak->GetSubMenu();

	// Cleanup the PAK menu
	for( size_t i = 0; i < mScensFromPak; ++i )
		mnuPakScenarios->Delete(ID_MNU_SCEN + i);

	PakFile *pak = g_DuneEngine->resourcesGet()->pakGet("scenario.pak");
	mScensFromPak = pak->getNumFiles();

	int count = 0;

	// Loop the entries in the PAK, and add any beginning with 'scen' to the pak load menu
	for( size_t i = 0; i < mScensFromPak; ++i ) {
	
		string name = pak->getFileName( i );

		std::transform( name.begin(), name.end(), name.begin(), tolower );

		if( name.find("scen") != string::npos ) {
			mnuPakScenarios->Insert(mnuPakScenarios->GetMenuItemCount(), ID_MNU_SCEN + count, name );
			++count;
		}
	}

	mScensFromPak = count;
}

void cFrameDuneMaps::loadToolbarUnits() {
	cVideoSurface surface(32, 24);

	for( int id = 0; id < 18; ++id ) {
		
		sUnitData *unitData = g_DuneEngine->resourcesGet()->unitGet( id );
		SDL_Surface *shp = g_DuneEngine->resourcesGet()->shpGet( unitData->sidebarIconID, 0 );

		surface.surfacePut( shp, 0, 0 );
		
		WxToolBar2->AddTool(ID_WXTOOLBAR2 + id, wxT(""), SDL_To_Bitmap(surface.scaleTo(1)), wxNullBitmap, wxITEM_NORMAL, unitData->Name, wxT("") );
	}

	// Sandworm
	sUnitData *unitData = g_DuneEngine->resourcesGet()->unitGet( 0x19 );
	SDL_Surface *shp = g_DuneEngine->resourcesGet()->shpGet( unitData->sidebarIconID, 0 );

	surface.surfacePut( shp, 0, 0 );
		
	WxToolBar2->AddTool(ID_WXTOOLBAR2 + 0x19, wxT(""), SDL_To_Bitmap(surface.scaleTo(1)), wxNullBitmap, wxITEM_NORMAL, unitData->Name, wxT("") );

	WxToolBar2->Realize();
}

void cFrameDuneMaps::loadToolbarStructures() {

	cVideoSurface surface(32, 24);

	for( int id = 0; id < 19; ++id ) {
		
		sStructureData *buildingData = g_DuneEngine->resourcesGet()->structureGet( id );
		SDL_Surface *shp = g_DuneEngine->resourcesGet()->shpGet( buildingData->sidebarIconID, 0 );

		surface.surfacePut( shp, 0, 0 );
		
		WxToolBar1->AddTool(ID_WXTOOLBAR1 + id, wxT(""), SDL_To_Bitmap(surface.scaleTo(1)), wxNullBitmap, wxITEM_NORMAL, buildingData->Name, wxT("") );
	}

	WxToolBar1->Realize();
}

void cFrameDuneMaps::OnClose(wxCloseEvent& /*event*/) {
	Destroy();
}

void cFrameDuneMaps::OnMove(wxMoveEvent& event) {
	if(mMinimapLock) {
		wxPoint a( GetPosition().x + GetSize().GetWidth(), GetPosition().y );
		mMinimap->SetPosition( a );
	}
}

void cFrameDuneMaps::OnSize(wxSizeEvent& event) {
	size_t width = event.GetSize().GetWidth();
	size_t height = event.GetSize().GetHeight();
	
	if(mMinimapLock) {
		wxPoint a( GetPosition().x + GetSize().GetWidth(), GetPosition().y );
		mMinimap->SetPosition( a );
	}

	if(mTileView)
		mTileView->SetSize( width - 40, height - 130 );

	WxToolBar2->SetPosition( wxPoint( 0, height - 126 ) );
	WxToolBar2->Refresh();
	Refresh(false);
}

/*
 * Mnuloadscenario1002Click
 */
void cFrameDuneMaps::Mnuloadscenario1002Click(wxCommandEvent& event) {
	WxOpenFileDialog1->SetTitle("Load Scenario");
	WxOpenFileDialog1->ShowModal();

	string filename = WxOpenFileDialog1->GetPath();

	if(!filename.size())
		return;

	g_DuneEngine->scenarioLoad( filename, eLoad_PC );
	mTileView->playfieldSizeUpdate();
}

/*
 * Mnusavescenario1007Click
 */
void cFrameDuneMaps::Mnusavescenario1007Click(wxCommandEvent& event) {
	WxSaveFileDialog1->SetTitle("Save Scenario");
	WxSaveFileDialog1->ShowModal();

	string filename = WxSaveFileDialog1->GetPath();

	g_DuneEngine->scenarioGet()->iniSave( filename );
}

/*
 * Mnunewscenario1005Click
 */
void cFrameDuneMaps::Mnunewscenario1005Click(wxCommandEvent& event) {
	cDialogNewScenario *scenario = new cDialogNewScenario(this);

	scenario->Show();
}

/*
 * Mnufromamiga7001Click
 */
void cFrameDuneMaps::Mnufromamiga7001Click(wxCommandEvent& event) {
	WxOpenFileDialog1->SetTitle("Load Amiga Scenario");
	WxOpenFileDialog1->ShowModal();

	string filename = WxOpenFileDialog1->GetPath();
	if(!filename.size())
		return;

	g_DuneEngine->scenarioLoad( filename, eLoad_Amiga );
	mTileView->playfieldSizeUpdate();
}

/*
 * MnuLoadPak_ScenClick
 */
void cFrameDuneMaps::MnuLoadPak_ScenClick(wxCommandEvent& event) {
	wxMenuItem *item = WxMenuBar1->FindItem( event.GetId() );

	string filename = item->GetItemLabel();

	g_DuneEngine->scenarioLoad( filename, eLoad_PC_Pak );

	mTileView->playfieldSizeUpdate();
}

/*
 * Mnuloadscenariopak7005Click
 */
void cFrameDuneMaps::Mnuloadscenariopak7005Click(wxCommandEvent& event) {
	WxOpenFileDialog1->SetTitle("Load Scenario PAK");

	WxOpenFileDialog1->SetWildcard(wxT("*.PAK"));
	WxOpenFileDialog1->ShowModal();
	WxOpenFileDialog1->SetWildcard(wxT("SCEN*.INI"));
	
	string filename = WxOpenFileDialog1->GetPath();
	if(!filename.size())
		return;

	g_DuneEngine->resourcesGet()->pakUnload("scenario.pak");
	g_DuneEngine->resourcesGet()->pakLoad( filename, false, "scenario.pak" );

	loadScenariosFromPak();
}

/*
 * Mnuquit1006Click
 */
void cFrameDuneMaps::Mnuquit1006Click(wxCommandEvent& event) {
	Close();
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

	g_DuneEngine->mPlaceObjectSet( new cStructure( house, structID, -1, 256) );

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

	g_DuneEngine->mPlaceObjectSet( new cUnit( house, unitID, -1, 0, 0, 256 ) );
}

/*
 * MnuHouseChange
 */
void cFrameDuneMaps::MnuHouseChange(wxCommandEvent& event) {
	int houseID = event.GetId() - ID_MNU_HARKONNEN_4006;

	mHouse = (eHouse) houseID;

	wxMenuItem *menuItem = WxMenuBar1->FindItem( ID_MNU_HARKONNEN_4006 + houseID );

	menuItem->Check();
}

/*
 * Mnuteams4014Click
 */
void cFrameDuneMaps::Mnuteams4014Click(wxCommandEvent& event) {
	cDialogTeams *Teams = new cDialogTeams(this);

	Teams->ShowModal();

	delete Teams;
}

/*
 * Mnureinforcements4015Click
 */
void cFrameDuneMaps::Mnureinforcements4015Click(wxCommandEvent& event) {
	cDialogReinforcements *Reinforcements = new cDialogReinforcements(this);

	Reinforcements->ShowModal();

	delete Reinforcements;
}

/*
 * Mnubasics4003Click
 */
void cFrameDuneMaps::Mnubasics4003Click(wxCommandEvent& event) {
	cDialogBasics *Basics = new cDialogBasics(this);

	Basics->ShowModal();

	delete Basics;
	mTileView->playfieldSizeUpdate();
	mMinimap->Refresh(false);
}

/*
 * Mnuhouses4016Click
 */
void cFrameDuneMaps::Mnuhouses4016Click(wxCommandEvent& event) {
	cDialogHouses *Houses = new cDialogHouses(this);

	Houses->ShowModal();

	delete Houses;
}
/*
 * Mnuminimapfollowswindow7003Click
 */
void cFrameDuneMaps::Mnuminimapfollowswindow7003Click(wxCommandEvent& event) {
	wxMenuItem *item = WxMenuBar1->FindItem( ID_MNU_MINIMAPFOLLOWSWINDOW_7003 );

	if( mMinimapLock )
		mMinimapLock = false;
	else 
		mMinimapLock = true;

	item->Check( mMinimapLock );
}

/*
 * Mnuchoam4017Click
 */
void cFrameDuneMaps::Mnuchoam4017Click(wxCommandEvent& event) {
	cDialogChoam *choam = new cDialogChoam( this );

	choam->ShowModal();
	delete choam;

}
