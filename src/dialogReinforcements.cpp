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
	EVT_BUTTON(ID_WXBUTTON1,cDialogReinforcements::WxButton1Click)
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

		int i = WxListCtrl1->InsertItem( WxListCtrl1->GetItemCount(), wxString(house->houseName.c_str(), wxConvUTF8) );
		WxListCtrl1->SetItem( i, 1, wxString(unit.c_str(), wxConvUTF8) );
		WxListCtrl1->SetItem( i, 2, wxString(direction.c_str(), wxConvUTF8) );
		WxListCtrl1->SetItem( i, 3, wxString(timer.str().c_str(), wxConvUTF8) );
		WxListCtrl1->SetItem( i, 4, wxString(repeat.c_str(), wxConvUTF8) );

	}

}

void cDialogReinforcements::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxButton1 = new wxButton(this, ID_WXBUTTON1, wxT("New"), wxPoint(15, 214), wxSize(45, 19), 0, wxDefaultValidator, wxT("WxButton1"));

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
		wxListItem listItem;
		listItem.SetId(i);
		listItem.SetMask(wxLIST_MASK_TEXT);

		listItem.SetColumn(0);
		WxListCtrl1->GetItem(listItem);
		string houseName = string(listItem.GetText().mb_str());

		listItem.SetColumn(1);
		WxListCtrl1->GetItem(listItem);
		string unitName = string(listItem.GetText().mb_str());

		listItem.SetColumn(2);
		WxListCtrl1->GetItem(listItem);
		string direction = string(listItem.GetText().mb_str());

		listItem.SetColumn(3);
		WxListCtrl1->GetItem(listItem);
		string timer = string(listItem.GetText().mb_str());

		listItem.SetColumn(4);
		WxListCtrl1->GetItem(listItem);
		string repeats = string(listItem.GetText().mb_str());

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

	wxListItem listItem;
	listItem.SetId(id);
	listItem.SetMask(wxLIST_MASK_TEXT);

	listItem.SetColumn(0);
	reinforce->mHouse = string(listItem.GetText().mb_str());

	listItem.SetColumn(1);
	reinforce->mUnitType = string(listItem.GetText().mb_str());

	listItem.SetColumn(2);
	reinforce->mDirection = string(listItem.GetText().mb_str());

	listItem.SetColumn(3);
	reinforce->mTimer = string(listItem.GetText().mb_str());

	listItem.SetColumn(4);
	reinforce->mRepeat = string(listItem.GetText().mb_str());

	reinforcement->reinforcementSet( reinforce );
	reinforcement->ShowModal();

	delete reinforce;

	reinforce = reinforcement->reforcementGet();
	if(!reinforce)
		return;

	listItem.SetColumn(0);
	listItem.SetText(wxString(reinforce->mHouse.c_str(), wxConvUTF8));
	WxListCtrl1->SetItem(listItem);

	listItem.SetColumn(1);
	listItem.SetText(wxString(reinforce->mUnitType.c_str(), wxConvUTF8));
	WxListCtrl1->SetItem(listItem);

	listItem.SetColumn(2);
	listItem.SetText(wxString(reinforce->mDirection.c_str(), wxConvUTF8));
	WxListCtrl1->SetItem(listItem);

	listItem.SetColumn(3);
	listItem.SetText(wxString(reinforce->mTimer.c_str(), wxConvUTF8));
	WxListCtrl1->SetItem(listItem);

	listItem.SetColumn(4);
	listItem.SetText(wxString(reinforce->mRepeat.c_str(), wxConvUTF8));
	WxListCtrl1->SetItem(listItem);

	delete reinforcement;
}

/*
 * WxButton1Click
 */
void cDialogReinforcements::WxButton1Click(wxCommandEvent& event) {

	cDialogReinforcement *reinforcement = new cDialogReinforcement(this);
	reinforcement->ShowModal();

	sReinforce *reinforce = reinforcement->reforcementGet();
	if(!reinforce)
		return;

	int id = WxListCtrl1->InsertItem( WxListCtrl1->GetItemCount(), wxString(reinforce->mHouse.c_str(), wxConvUTF8) );

	wxListItem listItem;
	listItem.SetId(id);
	listItem.SetMask(wxLIST_MASK_TEXT);

	listItem.SetColumn(1);
	listItem.SetText(wxString(reinforce->mUnitType.c_str(), wxConvUTF8));
	WxListCtrl1->SetItem(listItem);

	listItem.SetColumn(2);
	listItem.SetText(wxString(reinforce->mDirection.c_str(), wxConvUTF8));
	WxListCtrl1->SetItem(listItem);

	listItem.SetColumn(3);
	listItem.SetText(wxString(reinforce->mTimer.c_str(), wxConvUTF8));
	WxListCtrl1->SetItem(listItem);

	listItem.SetColumn(4);
	listItem.SetText(wxString(reinforce->mRepeat.c_str(), wxConvUTF8));
	WxListCtrl1->SetItem(listItem);

	delete reinforcement;

}
