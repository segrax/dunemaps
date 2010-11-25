///-----------------------------------------------------------------
///
/// @file      dialogReinforcement.cpp
/// @author    Robert
/// Created:   19/11/2010 21:54:34
/// @section   DESCRIPTION
///            cDialogReinforcement class implementation
///
///------------------------------------------------------------------

#include "dialogReinforcement.h"
#include "stdafx.h"
#include "engine/house.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
#include "Images/Self_cDialogReinforcement_XPM.xpm"
////Header Include End

//----------------------------------------------------------------------------
// cDialogReinforcement
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(cDialogReinforcement,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(cDialogReinforcement::OnClose)
	EVT_BUTTON(ID_MBUTTONCANCEL,cDialogReinforcement::mButtonCancelClick)
	EVT_BUTTON(ID_MBUTTONDONE,cDialogReinforcement::mButtonDoneClick)
END_EVENT_TABLE()
////Event Table End

cDialogReinforcement::cDialogReinforcement(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();

	mCancel = false;

	for( int i = eHouse_Harkonnen; i < eHouse_End; ++i ) {
		cHouse *house = g_DuneEngine->houseGet( (eHouse) i );

		mChoiceHouse->Insert( house->houseDataGet()->houseName, i );
	}

	for( int i = 0; i < 18; ++i ) {
		string unit = g_DuneEngine->resourcesGet()->unitGet(i)->Name;

		mChoiceUnit->Insert( unit, i );
	}

	for( int i = 0; i < 8; ++i ) {
		string dir = g_DuneEngine->resourcesGet()->directionGet( i );

		mChoiceDirection->Insert( dir, i );
	}
}

cDialogReinforcement::~cDialogReinforcement()
{
} 

void cDialogReinforcement::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	mTimer = new wxTextCtrl(this, ID_MTIMER, wxT("10"), wxPoint(80, 119), wxSize(40, 19), 0, wxDefaultValidator, wxT("mTimer"));

	mRepeat = new wxCheckBox(this, ID_MREPEAT, wxT("Repeat Delivery"), wxPoint(79, 153), wxSize(122, 19), 0, wxDefaultValidator, wxT("mRepeat"));

	WxStaticText4 = new wxStaticText(this, ID_WXSTATICTEXT4, wxT("Timer"), wxPoint(14, 119), wxDefaultSize, 0, wxT("WxStaticText4"));

	wxArrayString arrayStringFor_mChoiceDirection;
	mChoiceDirection = new wxChoice(this, ID_MCHOICEDIRECTION, wxPoint(80, 81), wxSize(166, 23), arrayStringFor_mChoiceDirection, 0, wxDefaultValidator, wxT("mChoiceDirection"));
	mChoiceDirection->SetSelection(-1);

	WxStaticText3 = new wxStaticText(this, ID_WXSTATICTEXT3, wxT("Direction"), wxPoint(14, 83), wxDefaultSize, 0, wxT("WxStaticText3"));

	WxStaticText2 = new wxStaticText(this, ID_WXSTATICTEXT2, wxT("Unit Type"), wxPoint(14, 46), wxDefaultSize, 0, wxT("WxStaticText2"));

	wxArrayString arrayStringFor_mChoiceUnit;
	mChoiceUnit = new wxChoice(this, ID_MCHOICEUNIT, wxPoint(80, 43), wxSize(166, 23), arrayStringFor_mChoiceUnit, 0, wxDefaultValidator, wxT("mChoiceUnit"));
	mChoiceUnit->SetSelection(-1);

	mButtonCancel = new wxButton(this, ID_MBUTTONCANCEL, wxT("Cancel"), wxPoint(146, 179), wxSize(69, 22), 0, wxDefaultValidator, wxT("mButtonCancel"));

	mButtonDone = new wxButton(this, ID_MBUTTONDONE, wxT("Done"), wxPoint(55, 179), wxSize(69, 22), 0, wxDefaultValidator, wxT("mButtonDone"));

	wxArrayString arrayStringFor_mChoiceHouse;
	mChoiceHouse = new wxChoice(this, ID_MCHOICEHOUSE, wxPoint(80, 9), wxSize(166, 23), arrayStringFor_mChoiceHouse, 0, wxDefaultValidator, wxT("mChoiceHouse"));
	mChoiceHouse->SetSelection(-1);

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, wxT("House"), wxPoint(14, 12), wxDefaultSize, 0, wxT("WxStaticText1"));

	SetTitle(wxT("DuneMaps - Reinforcement"));
	SetIcon(Self_cDialogReinforcement_XPM);
	SetSize(8,8,286,241);
	Center();
	
	////GUI Items Creation End
}

void cDialogReinforcement::OnClose(wxCloseEvent& /*event*/)
{
	Show(false);
}

void cDialogReinforcement::reinforcementSet( sReinforce *pReinforcement ) {
		
	mChoiceHouse->Select( mChoiceHouse->FindString( pReinforcement->mHouse ) );
	mChoiceUnit->Select( mChoiceUnit->FindString( pReinforcement->mUnitType ) );
	mChoiceDirection->Select( mChoiceDirection->FindString( pReinforcement->mDirection ) );

	mTimer->SetValue( pReinforcement->mTimer );

	if( pReinforcement->mRepeat == "True" )
		mRepeat->SetValue( true );
	else
		mRepeat->SetValue( false );
}

/*
 * mButtonDoneClick
 */
void cDialogReinforcement::mButtonDoneClick(wxCommandEvent& event) {

	mReinforce.mHouse = mChoiceHouse->GetStringSelection();
	mReinforce.mUnitType = mChoiceUnit->GetStringSelection();
	mReinforce.mDirection = mChoiceDirection->GetStringSelection();
	mReinforce.mTimer = mTimer->GetValue();

	if( mRepeat->GetValue() == true )
		mReinforce.mRepeat = "True";
	else
		mReinforce.mRepeat = "False";

	mCancel = false;
	Close();
}

/*
 * mButtonCancelClick
 */
void cDialogReinforcement::mButtonCancelClick(wxCommandEvent& event) {

	mCancel = true;
	Close();
}
