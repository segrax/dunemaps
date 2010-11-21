///-----------------------------------------------------------------
///
/// @file      dialogNewScenario.cpp
/// @author    Robert
/// Created:   20/11/2010 11:19:39
/// @section   DESCRIPTION
///            cDialogNewScenario class implementation
///
///------------------------------------------------------------------

#include "dialogNewScenario.h"

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

	SetTitle(wxT("DuneMaps - New Scenario"));
	SetIcon(wxNullIcon);
	SetSize(8,8,329,306);
	Center();
	
	////GUI Items Creation End
}

void cDialogNewScenario::OnClose(wxCloseEvent& /*event*/)
{
	Destroy();
}
