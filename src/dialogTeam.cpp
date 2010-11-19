///-----------------------------------------------------------------
///
/// @file      dialogTeam.cpp
/// @author    Robert
/// Created:   19/11/2010 11:31:24
/// @section   DESCRIPTION
///            cDialogTeam class implementation
///
///------------------------------------------------------------------

#include "dialogTeam.h"
#include "stdafx.h"
#include "dune/engine/house.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// cDialogTeam
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(cDialogTeam,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(cDialogTeam::OnClose)
	EVT_BUTTON(ID_MBUTTONDONE,cDialogTeam::mButtonDoneClick)
	EVT_BUTTON(ID_MBUTTONCANCEL,cDialogTeam::mButtonCancelClick)
END_EVENT_TABLE()
////Event Table End

cDialogTeam::cDialogTeam(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
	mCancel = false;

	for( int i = eHouse_Harkonnen; i < eHouse_End; ++i ) {
		cHouse *house = g_DuneEngine->houseGet( (eHouse) i );

		mChoiceHouse->Insert( house->houseDataGet()->houseName, i );
	}

	for( int i = 0; i < 6; ++i ) {
		string movement = g_DuneEngine->resourcesGet()->movementNameGet(i);

		mChoiceUnitType->Insert( movement, i );
	}

	for( int i = 0; i < 0x5; ++i) {
		string modeName = g_DuneEngine->resourcesGet()->aiModeGet(i);
		
		mChoiceAI->Insert( modeName, i );
	}
}

cDialogTeam::~cDialogTeam()
{
} 

void cDialogTeam::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	mButtonDone = new wxButton(this, ID_MBUTTONDONE, wxT("Done"), wxPoint(46, 149), wxSize(69, 22), 0, wxDefaultValidator, wxT("mButtonDone"));

	mButtonCancel = new wxButton(this, ID_MBUTTONCANCEL, wxT("Cancel"), wxPoint(137, 149), wxSize(69, 22), 0, wxDefaultValidator, wxT("mButtonCancel"));

	WxStaticText5 = new wxStaticText(this, ID_WXSTATICTEXT5, wxT("Maximum Units"), wxPoint(137, 113), wxDefaultSize, wxALIGN_CENTRE, wxT("WxStaticText5"));

	WxStaticText4 = new wxStaticText(this, ID_WXSTATICTEXT4, wxT("Minimum Units"), wxPoint(14, 112), wxDefaultSize, wxALIGN_CENTRE, wxT("WxStaticText4"));

	WxStaticText3 = new wxStaticText(this, ID_WXSTATICTEXT3, wxT("Unit Type"), wxPoint(14, 81), wxDefaultSize, 0, wxT("WxStaticText3"));

	WxStaticText2 = new wxStaticText(this, ID_WXSTATICTEXT2, wxT("AI Mode"), wxPoint(14, 48), wxDefaultSize, 0, wxT("WxStaticText2"));

	mTextMaxUnits = new wxTextCtrl(this, ID_MTEXTMAXUNITS, wxT("8"), wxPoint(204, 119), wxSize(43, 17), 0, wxDefaultValidator, wxT("mTextMaxUnits"));

	mTextMinUnits = new wxTextCtrl(this, ID_MTEXTMINUNITS, wxT("6"), wxPoint(80, 119), wxSize(43, 18), 0, wxDefaultValidator, wxT("mTextMinUnits"));

	wxArrayString arrayStringFor_mChoiceUnitType;
	mChoiceUnitType = new wxChoice(this, ID_MCHOICEUNITTYPE, wxPoint(80, 79), wxSize(166, 23), arrayStringFor_mChoiceUnitType, 0, wxDefaultValidator, wxT("mChoiceUnitType"));
	mChoiceUnitType->SetSelection(-1);

	wxArrayString arrayStringFor_mChoiceAI;
	mChoiceAI = new wxChoice(this, ID_MCHOICEAI, wxPoint(80, 44), wxSize(166, 23), arrayStringFor_mChoiceAI, 0, wxDefaultValidator, wxT("mChoiceAI"));
	mChoiceAI->SetSelection(-1);

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, wxT("House"), wxPoint(14, 9), wxDefaultSize, 0, wxT("WxStaticText1"));

	wxArrayString arrayStringFor_mChoiceHouse;
	mChoiceHouse = new wxChoice(this, ID_MCHOICEHOUSE, wxPoint(80, 6), wxSize(166, 23), arrayStringFor_mChoiceHouse, 0, wxDefaultValidator, wxT("mChoiceHouse"));
	mChoiceHouse->SetSelection(-1);

	SetTitle(wxT("DuneMaps - Team"));
	SetIcon(wxNullIcon);
	SetSize(8,9,271,207);
	Center();
	
	////GUI Items Creation End
}

void cDialogTeam::OnClose(wxCloseEvent& /*event*/)
{
	Show(false);
}

/*
 * mButtonDoneClick
 */
void cDialogTeam::mButtonDoneClick(wxCommandEvent& event) {

	mTeam.mHouse = mChoiceHouse->GetStringSelection();
	mTeam.mAIMode = mChoiceAI->GetStringSelection();
	mTeam.mUnitType = mChoiceUnitType->GetStringSelection();

	mTeam.mMaxUnits = mTextMaxUnits->GetValue();
	mTeam.mMinUnits = mTextMinUnits->GetValue();

	mCancel = false;
	Close();
}	

/*
 * mButtonCancelClick
 */
void cDialogTeam::mButtonCancelClick(wxCommandEvent& event) {
	mCancel = true;
	Close();
}

void cDialogTeam::teamSet( sTeam *pTeam ) {

	mChoiceHouse->Select( mChoiceHouse->FindString( pTeam->mHouse ) );
	mChoiceAI->Select( mChoiceAI->FindString( pTeam->mAIMode ) );
	mChoiceUnitType->Select( mChoiceUnitType->FindString( pTeam->mUnitType ) );

	mTextMaxUnits->SetValue( pTeam->mMaxUnits );
	mTextMinUnits->SetValue( pTeam->mMinUnits );
}
