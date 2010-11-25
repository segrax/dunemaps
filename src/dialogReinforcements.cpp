///-----------------------------------------------------------------
///
/// @file      dialogReinforcements.cpp
/// @author    Robert
/// Created:   19/11/2010 18:00:35
/// @section   DESCRIPTION
///            cDialogReinforcements class implementation
///
///------------------------------------------------------------------

#include "dialogReinforcements.h"
#include "dialogReinforcement.h"
#include "stdafx.h"
#include "engine/scenario.h"
#include "engine/house.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
#include "Images/Self_cDialogReinforcements_XPM.xpm"
////Header Include End

//----------------------------------------------------------------------------
// cDialogReinforcements
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(cDialogReinforcements,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(cDialogReinforcements::OnClose)
	EVT_BUTTON(ID_MBUTTONDONE,cDialogReinforcements::mButtonDoneClick)
	EVT_BUTTON(ID_MBUTTONCANCEL,cDialogReinforcements::mButtonCancelClick)
	
	EVT_LIST_ITEM_ACTIVATED(ID_WXLISTCTRL1,cDialogReinforcements::WxListCtrl1ItemActivated)
END_EVENT_TABLE()
////Event Table End

cDialogReinforcements::cDialogReinforcements(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();

	reinforcementsLoad();
}

cDialogReinforcements::~cDialogReinforcements()
{
} 

void cDialogReinforcements::reinforcementsLoad() {
	vector<sReinforcement> *reinforcements = g_DuneEngine->scenarioGet()->reinforcementsGet();
	vector<sReinforcement>::iterator	reIT;

	for( reIT = reinforcements->begin(); reIT != reinforcements->end(); ++reIT ) {
		sHouseData *house = g_DuneEngine->resourcesGet()->houseGet( reIT->mHouse );
		string unit = g_DuneEngine->resourcesGet()->unitGet( reIT->mUnitType )->Name;
		string direction = g_DuneEngine->resourcesGet()->directionGet( reIT->mDirection );
		stringstream timer;
		
		string repeat;
		
		if( reIT->mRepeat )
			repeat = "True";
		else
			repeat = "False";

		timer << reIT->mTime;

		int i = WxListCtrl1->InsertItem( WxListCtrl1->GetItemCount(), house->houseName );
		WxListCtrl1->SetItem( i, 1, unit );
		WxListCtrl1->SetItem( i, 2, direction );
		WxListCtrl1->SetItem( i, 3, timer.str() );
		WxListCtrl1->SetItem( i, 4, repeat );

	}

}

void cDialogReinforcements::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	mButtonDone = new wxButton(this, ID_MBUTTONDONE, wxT("Done"), wxPoint(134, 215), wxSize(69, 22), 0, wxDefaultValidator, wxT("mButtonDone"));

	mButtonCancel = new wxButton(this, ID_MBUTTONCANCEL, wxT("Cancel"), wxPoint(240, 215), wxSize(69, 22), 0, wxDefaultValidator, wxT("mButtonCancel"));

	WxListCtrl1 = new wxListCtrl(this, ID_WXLISTCTRL1, wxPoint(8, 5), wxSize(442, 202), wxLC_REPORT, wxDefaultValidator, wxT("WxListCtrl1"));
	WxListCtrl1->InsertColumn(0,wxT("Repeat"),wxLIST_FORMAT_LEFT,60 );
	WxListCtrl1->InsertColumn(0,wxT("Timer"),wxLIST_FORMAT_LEFT,60 );
	WxListCtrl1->InsertColumn(0,wxT("Direction"),wxLIST_FORMAT_LEFT,100 );
	WxListCtrl1->InsertColumn(0,wxT("Unit Type"),wxLIST_FORMAT_LEFT,100 );
	WxListCtrl1->InsertColumn(0,wxT("House"),wxLIST_FORMAT_LEFT,100 );

	SetTitle(wxT("DuneMaps - Reinforcements"));
	SetIcon(Self_cDialogReinforcements_XPM);
	SetSize(8,8,474,277);
	Center();
	
	////GUI Items Creation End
}

void cDialogReinforcements::OnClose(wxCloseEvent& /*event*/) {
	Show(false);
}

/*
 * mButtonCancelClick
 */
void cDialogReinforcements::mButtonCancelClick(wxCommandEvent& event) {
	Close();
}

/*
 * mButtonDoneClick
 */
void cDialogReinforcements::mButtonDoneClick(wxCommandEvent& event) {

	g_DuneEngine->scenarioGet()->reinforcementsClear();

	// Add all teams
	for( int i =0; i < WxListCtrl1->GetItemCount(); ++i ) {
		
		string houseName = WxListCtrl1->GetItemText( i, 0 );
		string unitName= WxListCtrl1->GetItemText( i, 1 );
		string direction= WxListCtrl1->GetItemText( i, 2 );
		string timer= WxListCtrl1->GetItemText( i, 3 );
		string repeats = WxListCtrl1->GetItemText( i, 4 );

		bool repeat = false;

		if( repeats == "True" )
			repeat = true;

		g_DuneEngine->scenarioGet()->reinforcementLoad( houseName, unitName, direction, timer, repeat );
	}

	Close();
}

/*
 * WxListCtrl1ItemActivated
 */
void cDialogReinforcements::WxListCtrl1ItemActivated(wxListEvent& event) {
	int id = event.GetItem().GetId();
	
	cDialogReinforcement *reinforcement = new cDialogReinforcement(this);
	sReinforce *reinforce = new sReinforce();

	reinforce->mHouse =  WxListCtrl1->GetItemText( id, 0 );
	reinforce->mUnitType =  WxListCtrl1->GetItemText( id, 1 );
	reinforce->mDirection =  WxListCtrl1->GetItemText( id, 2 );
	reinforce->mTimer =  WxListCtrl1->GetItemText( id, 3 );
	reinforce->mRepeat =  WxListCtrl1->GetItemText( id, 4 );

	reinforcement->reinforcementSet( reinforce );
	reinforcement->ShowModal();

	delete reinforce;

	reinforce = reinforcement->reforcementGet();

	WxListCtrl1->SetItem(id, 0, reinforce->mHouse );
	WxListCtrl1->SetItem( id, 1, reinforce->mUnitType );
	WxListCtrl1->SetItem( id, 2, reinforce->mDirection );
	WxListCtrl1->SetItem( id, 3, reinforce->mTimer );
	WxListCtrl1->SetItem( id, 4, reinforce->mRepeat );

	delete reinforcement;
}
