///-----------------------------------------------------------------
///
/// @file      dialogBasics.cpp
/// @author    Robert
/// Created:   18/11/2010 11:22:30
/// @section   DESCRIPTION
///            cDialogBasics class implementation
///
///------------------------------------------------------------------

#include "dialogBasics.h"
#include "stdafx.h"
#include "engine/scenario.h"
#include "screenPlayfield.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// cDialogBasics
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(cDialogBasics,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(cDialogBasics::OnClose)
	EVT_BUTTON(ID_MBUTTONCANCEL,cDialogBasics::mButtonCancelClick)
	EVT_BUTTON(ID_MBUTTONDONE,cDialogBasics::mButtonDoneClick)
END_EVENT_TABLE()
////Event Table End

cDialogBasics::cDialogBasics(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();

	missionLoad();


}

cDialogBasics::~cDialogBasics()
{
} 

void cDialogBasics::missionLoad() {
	int scale = g_DuneEngine->scenarioGet()->mapScaleLevelGet();
	
	stringstream time;
	time << g_DuneEngine->scenarioGet()->mapTimeoutGet( );

	WxEdit1->SetValue( time.str() );

	if( scale == 0 )
		WxRadioButton1->SetValue(true);

	if( scale == 1 )
		WxRadioButton2->SetValue(true);

	flagsSet( WxCheckListBox1, g_DuneEngine->scenarioGet()->mapLoseFlagsGet() );
	flagsSet( WxCheckListBox2, g_DuneEngine->scenarioGet()->mapWinFlagsGet() );

	mPictureLose->SetValue( g_DuneEngine->scenarioGet()->pictureLoseGet() );
	mPictureWin->SetValue( g_DuneEngine->scenarioGet()->pictureWinGet() );
	mPictureBrief->SetValue( g_DuneEngine->scenarioGet()->pictureBriefGet() );
}

byte cDialogBasics::flagsGet( wxCheckListBox *checkList ) {
	wxArrayInt selections;
	byte	   flags = 0;

	for( int i = 0; i < 4; ++i ) 
		if( checkList->IsChecked(i) )
			flags |= (1 << i);

	return flags;
}

void cDialogBasics::flagsSet( wxCheckListBox *checkList, size_t pFlags ) {
	wxArrayInt selections;

	for( int i = 0; i < 4; ++i )  
		if( pFlags & (1 << i) )
			checkList->Check( i, true );

	return;
}

void cDialogBasics::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxStaticText3 = new wxNotebook(this, ID_WXSTATICTEXT3, wxPoint(3, 4), wxSize(412, 282), wxNB_DEFAULT);

	WxNoteBookPage1 = new wxPanel(WxStaticText3, ID_WXNOTEBOOKPAGE1, wxPoint(4, 26), wxSize(404, 252));
	WxStaticText3->AddPage(WxNoteBookPage1, wxT("Mission"));

	WxNoteBookPage2 = new wxPanel(WxStaticText3, ID_WXNOTEBOOKPAGE2, wxPoint(4, 26), wxSize(404, 252));
	WxStaticText3->AddPage(WxNoteBookPage2, wxT("Pictures"));

	WxStaticBox1 = new wxStaticBox(WxNoteBookPage1, ID_WXSTATICBOX1, wxT("Lose Flags"), wxPoint(4, 130), wxSize(195, 104));

	wxArrayString arrayStringFor_WxCheckListBox1;
	WxCheckListBox1 = new wxCheckListBox(WxNoteBookPage1, ID_WXCHECKLISTBOX1, wxPoint(9, 147), wxSize(183, 78), arrayStringFor_WxCheckListBox1, wxLB_SINGLE | wxNO_BORDER, wxDefaultValidator, wxT("WxCheckListBox1"));
	WxCheckListBox1->Append(wxT("AI has no structures"));
	WxCheckListBox1->Append(wxT("Human has structures"));
	WxCheckListBox1->Append(wxT("Human reached credit quota"));
	WxCheckListBox1->Append(wxT("Map timeout reached"));

	WxStaticBox2 = new wxStaticBox(WxNoteBookPage1, ID_WXSTATICBOX2, wxT("Win Flags"), wxPoint(206, 130), wxSize(195, 104));

	wxArrayString arrayStringFor_WxCheckListBox2;
	WxCheckListBox2 = new wxCheckListBox(WxNoteBookPage1, ID_WXCHECKLISTBOX2, wxPoint(211, 147), wxSize(183, 78), arrayStringFor_WxCheckListBox2, wxLB_SINGLE | wxNO_BORDER, wxDefaultValidator, wxT("WxCheckListBox2"));
	WxCheckListBox2->Append(wxT("AI has no structures"));
	WxCheckListBox2->Append(wxT("Human has no structures"));
	WxCheckListBox2->Append(wxT("Human reached credit quota"));
	WxCheckListBox2->Append(wxT("Map timeout reached"));

	WxStaticBox3 = new wxStaticBox(WxNoteBookPage1, ID_WXSTATICBOX3, wxT(""), wxPoint(4, 8), wxSize(396, 113));

	Timeout = new wxStaticText(WxNoteBookPage1, ID_TIMEOUT, wxT("Timeout"), wxPoint(21, 32), wxDefaultSize, 0, wxT("Timeout"));

	WxEdit1 = new wxTextCtrl(WxNoteBookPage1, ID_WXEDIT1, wxT("0"), wxPoint(97, 32), wxSize(67, 22), 0, wxDefaultValidator, wxT("WxEdit1"));

	mButtonDone = new wxButton(this, ID_MBUTTONDONE, wxT("Done"), wxPoint(135, 290), wxSize(69, 22), 0, wxDefaultValidator, wxT("mButtonDone"));

	mButtonCancel = new wxButton(this, ID_MBUTTONCANCEL, wxT("Cancel"), wxPoint(219, 290), wxSize(69, 22), 0, wxDefaultValidator, wxT("mButtonCancel"));

	WxStaticBox4 = new wxStaticBox(WxNoteBookPage1, ID_WXSTATICBOX4, wxT("Map Scale"), wxPoint(235, 26), wxSize(150, 77));

	WxRadioButton1 = new wxRadioButton(WxNoteBookPage1, ID_WXRADIOBUTTON1, wxT("Scale 0"), wxPoint(247, 50), wxSize(81, 12), 0, wxDefaultValidator, wxT("WxRadioButton1"));

	WxRadioButton2 = new wxRadioButton(WxNoteBookPage1, ID_WXRADIOBUTTON2, wxT("Scale 1"), wxPoint(247, 71), wxSize(70, 17), 0, wxDefaultValidator, wxT("WxRadioButton2"));

	WxStaticText1 = new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT1, wxT("Win"), wxPoint(15, 50), wxDefaultSize, 0, wxT("WxStaticText1"));

	WxStaticText2 = new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT2, wxT("Brief"), wxPoint(15, 75), wxDefaultSize, 0, wxT("WxStaticText2"));

	mPictureLose = new wxTextCtrl(WxNoteBookPage2, ID_MPICTURELOSE, wxT(""), wxPoint(65, 25), wxSize(100, 19), 0, wxDefaultValidator, wxT("mPictureLose"));

	mPictureWin = new wxTextCtrl(WxNoteBookPage2, ID_MPICTUREWIN, wxT(""), wxPoint(65, 50), wxSize(100, 19), 0, wxDefaultValidator, wxT("mPictureWin"));

	mPictureBrief = new wxTextCtrl(WxNoteBookPage2, ID_MPICTUREBRIEF, wxT(""), wxPoint(65, 75), wxSize(100, 19), 0, wxDefaultValidator, wxT("mPictureBrief"));

	WxStaticText4 = new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT4, wxT("Lose"), wxPoint(15, 25), wxDefaultSize, 0, wxT("WxStaticText4"));

	SetTitle(wxT("DuneMaps - Scenario Basics"));
	SetIcon(wxNullIcon);
	SetSize(8,8,439,352);
	Center();
	
	////GUI Items Creation End

}

void cDialogBasics::OnClose(wxCloseEvent& /*event*/)
{
	Show(false);
}

/*
 * mButtonDoneClick
 */
void cDialogBasics::mButtonDoneClick(wxCommandEvent& event) {

	string timeout = WxEdit1->GetValue();

	int scale = 0;

	if( WxRadioButton1->GetValue() )
		scale = 0;

	if( WxRadioButton2->GetValue() )
		scale = 1;

	g_DuneEngine->scenarioGet()->mapScaleLevelSet( scale );
	g_DuneEngine->scenarioGet()->mapTimeoutSet( atoi(timeout.c_str()) );

	g_DuneEngine->scenarioGet()->mapLoseFlagsSet( flagsGet( WxCheckListBox1 ) );
	g_DuneEngine->scenarioGet()->mapWinFlagsSet( flagsGet( WxCheckListBox2 ) );
	
	g_DuneEngine->screenPlayfieldGet()->scaleSet();

	g_DuneEngine->scenarioGet()->pictureLoseSet( string( mPictureLose->GetValue() ) );
	g_DuneEngine->scenarioGet()->pictureWinSet(  string( mPictureWin->GetValue() ) );
	g_DuneEngine->scenarioGet()->pictureBriefSet( string( mPictureBrief->GetValue() ) );

	Close();
}

/*
 * mButtonCancelClick
 */
void cDialogBasics::mButtonCancelClick(wxCommandEvent& event) {

	Close();
}
