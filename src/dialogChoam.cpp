///-----------------------------------------------------------------
///
/// @file      dialogChoam.cpp
/// @author    Robert
/// Created:   25/11/2010 17:13:34
/// @section   DESCRIPTION
///            cDialogChoam class implementation
///
///------------------------------------------------------------------

#include "dialogChoam.h"
#include "stdafx.h"
#include "dune/engine/scenario.h"
#include <wx/numdlg.h>

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// cDialogChoam
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(cDialogChoam,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(cDialogChoam::OnClose)
	EVT_BUTTON(ID_WXBUTTON1,cDialogChoam::WxButton1Click)
	EVT_BUTTON(ID_MBUTTONDONE,cDialogChoam::mButtonDoneClick)
	EVT_BUTTON(ID_MBUTTONCANCEL,cDialogChoam::mButtonCancelClick)
	
	EVT_LIST_ITEM_ACTIVATED(ID_WXLISTCTRL1,cDialogChoam::WxListCtrl1ItemActivated)
END_EVENT_TABLE()
////Event Table End

cDialogChoam::cDialogChoam(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	mCancel = false;
	CreateGUIControls();

	// Add all units
	for( int i = 0; i < 18; ++i ) {
		sUnitData *unit = g_DuneEngine->resourcesGet()->unitGet(i);

		WxListCtrl1->InsertItem( WxListCtrl1->GetItemCount(), wxString(unit->Name.c_str(), wxConvUTF8) );
		WxListCtrl1->SetItem( i, 1, wxT("0") );
	}


	vector<sChoam> *choams = g_DuneEngine->scenarioGet()->choamGet();
	vector<sChoam>::iterator	reIT;

	// Sert the count for each unit
	for( reIT = choams->begin(); reIT != choams->end(); ++reIT ) {
		stringstream count;
		count << reIT->mCount;

		int i = WxListCtrl1->FindItem(0, wxString(reIT->mUnitType.c_str(), wxConvUTF8) );

		WxListCtrl1->SetItem( i, 1, wxString(count.str().c_str(), wxConvUTF8) );
	}
}

cDialogChoam::~cDialogChoam() {

} 

void cDialogChoam::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxPopupMenu1 = new wxMenu(wxT(""));

	WxButton1 = new wxButton(this, ID_WXBUTTON1, wxT("New"), wxPoint(5, 222), wxSize(35, 18), 0, wxDefaultValidator, wxT("WxButton1"));

	mButtonDone = new wxButton(this, ID_MBUTTONDONE, wxT("Done"), wxPoint(59, 220), wxSize(69, 22), 0, wxDefaultValidator, wxT("mButtonDone"));

	mButtonCancel = new wxButton(this, ID_MBUTTONCANCEL, wxT("Cancel"), wxPoint(157, 220), wxSize(69, 22), 0, wxDefaultValidator, wxT("mButtonCancel"));

	WxListCtrl1 = new wxListCtrl(this, ID_WXLISTCTRL1, wxPoint(10, 9), wxSize(252, 205), wxLC_REPORT, wxDefaultValidator, wxT("WxListCtrl1"));
	WxListCtrl1->InsertColumn(0,wxT("Count"),wxLIST_FORMAT_LEFT,90 );
	WxListCtrl1->InsertColumn(0,wxT("Unit Type"),wxLIST_FORMAT_LEFT,150 );

	SetTitle(wxT("DuneMaps - Choam"));
	SetIcon(wxNullIcon);
	SetSize(8,8,289,284);
	Center();
	
	////GUI Items Creation End
}

void cDialogChoam::OnClose(wxCloseEvent& /*event*/) {
	Show(false);
}

/*
 * WxListCtrl1ItemActivated
 */
void cDialogChoam::WxListCtrl1ItemActivated(wxListEvent& event) {

	int units = wxGetNumberFromUser( wxT("Number of available Units"), wxT("Units"), wxT("Choam Units") );

	if(units < 0)
		return;

	wxListItem selected = event.GetItem();

	int id = selected.GetId();

	stringstream unit;
	unit << units;

	WxListCtrl1->SetItem(id, 1, wxString(unit.str().c_str(), wxConvUTF8) );
}

/*
 * mButtonDoneClick
 */
void cDialogChoam::mButtonDoneClick(wxCommandEvent& event) {
	mCancel = false;

	vector<sChoam> *choams = g_DuneEngine->scenarioGet()->choamGet();

	choams->clear();

	for( int i = 0; i < WxListCtrl1->GetItemCount(); ++i ) {
		int count = atoi( WxListCtrl1->GetItemText( i).mb_str() );

		if(count != 0 ) {
			sChoam choam;
			choam.mCount = count;
			choam.mUnitType = g_DuneEngine->resourcesGet()->unitGet(i)->Name;

			choams->push_back(choam);
		}
	}

	Close();
}

/*
 * mButtonCancelClick
 */
void cDialogChoam::mButtonCancelClick(wxCommandEvent& event) {
	mCancel = true;
	Close();
}

/*
 * WxButton1Click
 */
void cDialogChoam::WxButton1Click(wxCommandEvent& event) {
	
}
