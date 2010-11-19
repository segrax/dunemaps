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

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
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
END_EVENT_TABLE()
////Event Table End

cDialogReinforcements::cDialogReinforcements(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

cDialogReinforcements::~cDialogReinforcements()
{
} 

void cDialogReinforcements::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	mButtonDone = new wxButton(this, ID_MBUTTONDONE, wxT("Done"), wxPoint(105, 215), wxSize(69, 22), 0, wxDefaultValidator, wxT("mButtonDone"));

	mButtonCancel = new wxButton(this, ID_MBUTTONCANCEL, wxT("Cancel"), wxPoint(196, 215), wxSize(69, 22), 0, wxDefaultValidator, wxT("mButtonCancel"));

	WxListCtrl1 = new wxListCtrl(this, ID_WXLISTCTRL1, wxPoint(8, 5), wxSize(364, 202), wxLC_REPORT, wxDefaultValidator, wxT("WxListCtrl1"));
	WxListCtrl1->InsertColumn(0,wxT("Timer"),wxLIST_FORMAT_LEFT,60 );
	WxListCtrl1->InsertColumn(0,wxT("Direction"),wxLIST_FORMAT_LEFT,100 );
	WxListCtrl1->InsertColumn(0,wxT("Unit Type"),wxLIST_FORMAT_LEFT,100 );
	WxListCtrl1->InsertColumn(0,wxT("House"),wxLIST_FORMAT_LEFT,100 );

	SetTitle(wxT("DuneMaps - Reinforcements"));
	SetIcon(wxNullIcon);
	SetSize(8,8,393,277);
	Center();
	
	////GUI Items Creation End
}

void cDialogReinforcements::OnClose(wxCloseEvent& /*event*/)
{
	Destroy();
}

/*
 * mButtonCancelClick
 */
void cDialogReinforcements::mButtonCancelClick(wxCommandEvent& event)
{
	// insert your code here
}

/*
 * mButtonDoneClick
 */
void cDialogReinforcements::mButtonDoneClick(wxCommandEvent& event)
{
	// insert your code here
}
