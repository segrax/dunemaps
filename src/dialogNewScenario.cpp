///-----------------------------------------------------------------
///
/// @file      dialogNewScenario.cpp
/// @author    Robert
/// Created:   20/11/2010 11:19:39
/// @section   DESCRIPTION
///            cDialogNewScenario class implementation
///
///------------------------------------------------------------------
#include "appDuneMaps.h"
#include "dialogNewScenario.h"
#include "frameDuneMaps.h"
#include "panelMinimap.h"

#include "stdafx.h"
#include "dune/minimap.h"
#include "dune/engine/scenario.h"

#include <wx/dcbuffer.h>
//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// cDialogNewScenario
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(cDialogNewScenario,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(cDialogNewScenario::OnClose)
	
	EVT_TEXT(ID_WXEDIT1,cDialogNewScenario::WxEdit1Updated)
END_EVENT_TABLE()
////Event Table End

cDialogNewScenario::cDialogNewScenario(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

cDialogNewScenario::~cDialogNewScenario()
{
} 

void cDialogNewScenario::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxEdit1 = new wxTextCtrl(this, ID_WXEDIT1, wxT("123456789"), wxPoint(115, 26), wxSize(93, 21), 0, wxDefaultValidator, wxT("WxEdit1"));
	WxEdit1->SetMaxLength(9);

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, wxT("Map Seed"), wxPoint(25, 28), wxDefaultSize, 0, wxT("WxStaticText1"));

	SetTitle(wxT("DuneMaps - New Scenario"));
	SetIcon(wxNullIcon);
	SetSize(8,8,280,240);
	Center();
	
	////GUI Items Creation End
}

void cDialogNewScenario::OnClose(wxCloseEvent& /*event*/)
{
	Show(false);
	Destroy();
}

void cDialogNewScenario::OnPaint(wxPaintEvent& event) {
	wxBufferedPaintDC tileView(this);

	cVideoSurface surface(64, 64);

	g_DuneEngine->minimapGet()->draw( &surface );

	tileView.Clear();
	tileView.DrawBitmap( SDL_To_Bitmap( surface.scaleTo(2) ), 0, 0 );
}

/*
 * WxEdit1Updated
 */
void cDialogNewScenario::WxEdit1Updated(wxCommandEvent& event) {
	string seed =  WxEdit1->GetValue();

	g_DuneEngine->scenarioGet()->scenarioNew( seed );

	g_DuneEngine->frameGet()->Refresh(false);
	g_DuneEngine->frameGet()->minimapGet()->Refresh(false);
}
