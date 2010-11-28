///-----------------------------------------------------------------
///
/// @file      dialogTeams.cpp
/// @author    Robert
/// Created:   19/11/2010 08:50:10
/// @section   DESCRIPTION
///            cDialogTeams class implementation
///
///------------------------------------------------------------------

#include "dialogTeams.h"
#include "dialogTeam.h"
#include "stdafx.h"
#include "engine/scenario.h"
#include "engine/house.h"
#include "engine/team.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
#include "Images/Self_cDialogTeams_XPM.xpm"
////Header Include End

//----------------------------------------------------------------------------
// cDialogTeams
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(cDialogTeams,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(cDialogTeams::OnClose)
	EVT_MENU(ID_MNU_NEWTEAM_1005 , cDialogTeams::Mnunewteam1005Click)
	EVT_MENU(ID_MNU_DELETETEAM_1006 , cDialogTeams::Mnudeleteteam1006Click)
	EVT_BUTTON(ID_MBUTTONNEW,cDialogTeams::mButtonNewClick)
	EVT_BUTTON(ID_MBUTTONDONE,cDialogTeams::mButtonDoneClick)
	EVT_BUTTON(ID_MBUTTONCANCEL,cDialogTeams::mButtonCancelClick)
	
	EVT_LIST_ITEM_ACTIVATED(ID_WXLISTCTRL1,cDialogTeams::WxListCtrl1ItemActivated)
	EVT_LIST_ITEM_RIGHT_CLICK(ID_WXLISTCTRL1,cDialogTeams::WxListCtrl1RightClick)
	EVT_LIST_COL_RIGHT_CLICK(ID_WXLISTCTRL1,cDialogTeams::WxListCtrl1RightClick)
END_EVENT_TABLE()
////Event Table End

cDialogTeams::cDialogTeams(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();

	vector<cTeam*>				*teams = g_DuneEngine->scenarioGet()->teamsGet();
	vector<cTeam*>::iterator	 teamIT;

	for( teamIT = teams->begin(); teamIT != teams->end(); ++teamIT ) {
		cTeam *team = (*teamIT);
		
		string mode = g_DuneEngine->resourcesGet()->aiModeGet(team->aiModeGet());
		string type = g_DuneEngine->resourcesGet()->movementNameGet(team->movementTypeGet());

		stringstream unk1, maxUnits;

		unk1 << team->unk1Get();
		maxUnits << team->unitsMaxGet();

		int i = WxListCtrl1->InsertItem( WxListCtrl1->GetItemCount(), wxString(team->houseGet()->houseDataGet()->houseName.c_str(), wxConvUTF8) );

		WxListCtrl1->SetItem( i, 1, wxString(mode.c_str(), wxConvUTF8) );
		WxListCtrl1->SetItem( i, 2, wxString(type.c_str(), wxConvUTF8) );
		WxListCtrl1->SetItem( i, 3, wxString(unk1.str().c_str(), wxConvUTF8) );
		WxListCtrl1->SetItem( i, 4, wxString(maxUnits.str().c_str(), wxConvUTF8) );
	}

}

cDialogTeams::~cDialogTeams()
{
} 

void cDialogTeams::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	mPopupTeams = new wxMenu(wxT(""));mPopupTeams->Append(ID_MNU_NEWTEAM_1005, wxT("New Team"), wxT(""), wxITEM_NORMAL);
	mPopupTeams->Append(ID_MNU_DELETETEAM_1006, wxT("Delete Team"), wxT(""), wxITEM_NORMAL);

	mButtonNew = new wxButton(this, ID_MBUTTONNEW, wxT("New"), wxPoint(10, 218), wxSize(37, 22), 0, wxDefaultValidator, wxT("mButtonNew"));

	mButtonDone = new wxButton(this, ID_MBUTTONDONE, wxT("Done"), wxPoint(149, 221), wxSize(69, 22), 0, wxDefaultValidator, wxT("mButtonDone"));

	mButtonCancel = new wxButton(this, ID_MBUTTONCANCEL, wxT("Cancel"), wxPoint(240, 221), wxSize(69, 22), 0, wxDefaultValidator, wxT("mButtonCancel"));

	WxListCtrl1 = new wxListCtrl(this, ID_WXLISTCTRL1, wxPoint(7, 5), wxSize(449, 201), wxLC_REPORT | wxLC_SINGLE_SEL, wxDefaultValidator, wxT("WxListCtrl1"));
	WxListCtrl1->InsertColumn(0,wxT("Max Units"),wxLIST_FORMAT_LEFT,70 );
	WxListCtrl1->InsertColumn(0,wxT("Min Units"),wxLIST_FORMAT_LEFT,90 );
	WxListCtrl1->InsertColumn(0,wxT("Unit Type"),wxLIST_FORMAT_LEFT,90 );
	WxListCtrl1->InsertColumn(0,wxT("AI Type"),wxLIST_FORMAT_LEFT,90 );
	WxListCtrl1->InsertColumn(0,wxT("House"),wxLIST_FORMAT_LEFT,100 );

	SetTitle(wxT("DuneMaps - Teams"));
	SetIcon(Self_cDialogTeams_XPM);
	SetSize(8,8,479,287);
	Center();
	
	////GUI Items Creation End
}

void cDialogTeams::OnClose(wxCloseEvent& /*event*/)
{
	Show(false);
}

/*
 * mButtonCancelClick
 */
void cDialogTeams::mButtonCancelClick(wxCommandEvent& event)
{
	Close();
}

/*
 * mButtonDoneClick
 */
void cDialogTeams::mButtonDoneClick(wxCommandEvent& event) {

	g_DuneEngine->scenarioGet()->teamsClear();

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
		string aiMode = string(listItem.GetText().mb_str());

		listItem.SetColumn(2);
		WxListCtrl1->GetItem(listItem);
		string movementType = string(listItem.GetText().mb_str());

		listItem.SetColumn(3);
		WxListCtrl1->GetItem(listItem);
		string UnitsMin = string(listItem.GetText().mb_str());

		listItem.SetColumn(4);
		WxListCtrl1->GetItem(listItem);
		string UnitsMax = string(listItem.GetText().mb_str());

		g_DuneEngine->scenarioGet()->teamCreate( houseName, aiMode, movementType, UnitsMin, UnitsMax );

	}

	Close();
}

/*
 * WxListCtrl1RightClick
 */
void cDialogTeams::WxListCtrl1RightClick(wxListEvent& event) {
	PopupMenu( mPopupTeams );
}

/*
 * Mnunewteam1005Click
 */
void cDialogTeams::Mnunewteam1005Click(wxCommandEvent& event) {

	mButtonNewClick(event);
}

/*
 * Mnudeleteteam1006Click
 */
void cDialogTeams::Mnudeleteteam1006Click(wxCommandEvent& event) {
	int id = event.GetSelection();

	if( id < 0 )
		return;

	WxListCtrl1->DeleteItem( id );
}

/*
 * mButtonNewClick
 */
void cDialogTeams::mButtonNewClick(wxCommandEvent& event) {
	
	cDialogTeam *team = new cDialogTeam(this);

	team->ShowModal();

	sTeam *t = team->teamGet();

	if(t) {
		int i =	WxListCtrl1->InsertItem( WxListCtrl1->GetItemCount(), wxString(t->mHouse.c_str(), wxConvUTF8) );

		WxListCtrl1->SetItem( i, 1, wxString(t->mAIMode.c_str(), wxConvUTF8) );
		WxListCtrl1->SetItem( i, 2, wxString(t->mUnitType.c_str(), wxConvUTF8) );
		WxListCtrl1->SetItem( i, 3, wxString(t->mMinUnits.c_str(), wxConvUTF8) );
		WxListCtrl1->SetItem( i, 4, wxString(t->mMaxUnits.c_str(), wxConvUTF8) );
	}

	delete team;
}

/*
 * WxListCtrl1ItemActivated
 */
void cDialogTeams::WxListCtrl1ItemActivated(wxListEvent& event) {
	int id = event.GetItem().GetId();
	
	cDialogTeam *team = new cDialogTeam(this);

	sTeam *t = new sTeam();

	wxListItem listItem;
	listItem.SetId(id);
	listItem.SetMask(wxLIST_MASK_TEXT);

	listItem.SetColumn(0);
	WxListCtrl1->GetItem(listItem);
	t->mHouse = string(listItem.GetText().mb_str());

	listItem.SetColumn(1);
	WxListCtrl1->GetItem(listItem);
	t->mAIMode = string(listItem.GetText().mb_str());

	listItem.SetColumn(2);
	WxListCtrl1->GetItem(listItem);
	t->mUnitType = string(listItem.GetText().mb_str());

	listItem.SetColumn(3);
	WxListCtrl1->GetItem(listItem);
	t->mMinUnits = string(listItem.GetText().mb_str());

	listItem.SetColumn(4);
	WxListCtrl1->GetItem(listItem);
	t->mMaxUnits = string(listItem.GetText().mb_str());

	team->teamSet( t );

	team->ShowModal();

	delete t;

	t = team->teamGet();

	WxListCtrl1->SetItem( id, 0, wxString(t->mHouse.c_str(), wxConvUTF8) );
	WxListCtrl1->SetItem( id, 1, wxString(t->mAIMode.c_str(), wxConvUTF8) );
	WxListCtrl1->SetItem( id, 2, wxString(t->mUnitType.c_str(), wxConvUTF8) );
	WxListCtrl1->SetItem( id, 3, wxString(t->mMinUnits.c_str(), wxConvUTF8) );
	WxListCtrl1->SetItem( id, 4, wxString(t->mMaxUnits.c_str(), wxConvUTF8) );

	delete team;
}
