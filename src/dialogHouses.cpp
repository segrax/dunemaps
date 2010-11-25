///-----------------------------------------------------------------
///
/// @file      dialogHouses.cpp
/// @author    Robert
/// Created:   18/11/2010 08:56:20
/// @section   DESCRIPTION
///            cDialogHouses class implementation
///
///------------------------------------------------------------------

#include "dialogHouses.h"
#include "stdafx.h"
#include "dune/engine/house.h"
#include <algorithm>

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
#include "Images/Self_cDialogHouses_XPM.xpm"
////Header Include End

//----------------------------------------------------------------------------
// cDialogHouses
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(cDialogHouses,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(cDialogHouses::OnClose)
	EVT_BUTTON(ID_MBUTTONCANCEL,cDialogHouses::mButtonCancelClick)
	EVT_BUTTON(ID_MBUTTONDONE,cDialogHouses::mButtonDoneClick)
	EVT_LISTBOX(ID_WXLISTBOX1,cDialogHouses::WxListBox1Selected)
END_EVENT_TABLE()
////Event Table End

cDialogHouses::cDialogHouses(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
	mHouse = 0;

	for( int i = eHouse_Harkonnen; i < eHouse_End; ++i ) {
		cHouse *house = g_DuneEngine->houseGet( (eHouse) i );

		_houses.insert( make_pair( i, new sHouse( house->creditGet(), house->creditQuotaGet(), house->maxUnitGet()) ));

		string brain = house->brainGet();
		std::transform( brain.begin(), brain.end(), brain.begin(), tolower );

		if( brain.compare( "human" ) == 0 )
			_houses[i]->_human = true;

		wxString name = house->houseDataGet()->houseName;

		WxListBox1->InsertItems( 1, &name, i );
	}

	mHouse = _houses[0];
	updateValues();
}

cDialogHouses::~cDialogHouses() {

	for( int i = eHouse_Harkonnen; i < eHouse_End; ++i ) { 
		delete _houses[i];
	}

} 

void cDialogHouses::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	wxArrayString arrayStringFor_WxListBox1;
	WxListBox1 = new wxListBox(this, ID_WXLISTBOX1, wxPoint(4, 5), wxSize(179, 120), arrayStringFor_WxListBox1, wxLB_SINGLE);

	WxPanel1 = new wxPanel(this, ID_WXPANEL1, wxPoint(192, 7), wxSize(217, 118));

	mCheckHuman = new wxCheckBox(WxPanel1, ID_MCHECKHUMAN, wxT("Is Human"), wxPoint(5, 93), wxSize(75, 17), 0, wxDefaultValidator, wxT("mCheckHuman"));

	mEditQuota = new wxTextCtrl(WxPanel1, ID_MEDITQUOTA, wxT("0"), wxPoint(90, 7), wxSize(120, 20), 0, wxDefaultValidator, wxT("mEditQuota"));

	mEditCredits = new wxTextCtrl(WxPanel1, ID_MEDITCREDITS, wxT("1000"), wxPoint(90, 37), wxSize(120, 21), 0, wxDefaultValidator, wxT("mEditCredits"));

	mEditMaxUnits = new wxTextCtrl(WxPanel1, ID_MEDITMAXUNITS, wxT("25"), wxPoint(90, 67), wxSize(120, 20), 0, wxDefaultValidator, wxT("mEditMaxUnits"));

	WxStaticText1 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT1, wxT("Quota"), wxPoint(5, 7), wxDefaultSize, 0, wxT("WxStaticText1"));

	WxStaticText2 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT2, wxT("Credits"), wxPoint(5, 37), wxDefaultSize, 0, wxT("WxStaticText2"));

	WxStaticText3 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT3, wxT("Max Units"), wxPoint(5, 67), wxDefaultSize, 0, wxT("WxStaticText3"));

	mButtonDone = new wxButton(this, ID_MBUTTONDONE, wxT("Done"), wxPoint(121, 133), wxSize(60, 22), 0, wxDefaultValidator, wxT("mButtonDone"));

	mButtonCancel = new wxButton(this, ID_MBUTTONCANCEL, wxT("Cancel"), wxPoint(197, 133), wxSize(60, 22), 0, wxDefaultValidator, wxT("mButtonCancel"));

	SetTitle(wxT("DuneMaps - House Editor"));
	SetIcon(Self_cDialogHouses_XPM);
	SetSize(8,8,430,191);
	Center();
	
	////GUI Items Creation End
}

void cDialogHouses::OnClose(wxCloseEvent& event) {
	Show(false);
}

void cDialogHouses::updateHouse() {
	
	// Update Current Selected House
	mHouse->_creditQuota	= atoi(mEditQuota->GetValue());
	mHouse->_credits		= atoi(mEditCredits->GetValue());
	mHouse->_maxUnits		= atoi(mEditMaxUnits->GetValue());
	mHouse->_human			= mCheckHuman->GetValue();
}

void cDialogHouses::updateValues() {
	stringstream val;
	val << mHouse->_creditQuota;
	mEditQuota->SetValue( val.str() );

	val.str("");
	val << mHouse->_credits;
	mEditCredits->SetValue( val.str() );

	val.str("");
	val << mHouse->_maxUnits;
	mEditMaxUnits->SetValue( val.str() );

	mCheckHuman->SetValue( mHouse->_human );
}

/*
 * WxListBox1Selected
 */
void cDialogHouses::WxListBox1Selected(wxCommandEvent& event) {
	int id = event.GetSelection();

	updateHouse();

	mHouse = _houses[ id ];

	updateValues();
}

/*
 * mButtonDoneClick
 */
void cDialogHouses::mButtonDoneClick(wxCommandEvent& event) {
	updateHouse();

	for( int i = eHouse_Harkonnen; i < eHouse_End; ++i ) {
		cHouse *house = g_DuneEngine->houseGet( (eHouse) i );
		
		house->creditSet( _houses[i]->_credits );
		house->creditQuotaSet( _houses[i]->_creditQuota );
		house->maxUnitSet( _houses[i]->_maxUnits );
		
		if( _houses[i]->_human )
			house->brainSet("Human");
		else
			house->brainSet( "CPU" );
	}

	Close();
}

/*
 * mButtonCancelClick
 */
void cDialogHouses::mButtonCancelClick(wxCommandEvent& event) {
	Close();
}
