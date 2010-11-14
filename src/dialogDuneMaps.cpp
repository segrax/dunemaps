///-----------------------------------------------------------------
///
/// @file      dialogDuneMaps.cpp
/// @author    Robert
/// Created:   14/11/2010 08:04:04
/// @section   DESCRIPTION
///            cDialogDuneMaps class implementation
///
///------------------------------------------------------------------

#include "dialogDuneMaps.h"
#include "panelSurface.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// cDialogDuneMaps
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(cDialogDuneMaps,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(cDialogDuneMaps::OnClose)
	EVT_SIZE(cDialogDuneMaps::OnSize)
END_EVENT_TABLE()
////Event Table End

cDialogDuneMaps::cDialogDuneMaps(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

cDialogDuneMaps::~cDialogDuneMaps()
{
} 

void cDialogDuneMaps::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	SetTitle(wxT("Dune Maps"));
	SetIcon(wxNullIcon);
	SetSize(8,8,740,470);
	Center();
	
	////GUI Items Creation End

	this->SetBackgroundColour( wxColor(0.0L) );

	mTileView = new cPanelSurface( this );
	mTileView->SetPosition( wxPoint( 12, 15 ) );

	SetMinSize( wxSize(400, 300) );
}

void cDialogDuneMaps::OnClose(wxCloseEvent& /*event*/) {
	Destroy();
}

void cDialogDuneMaps::OnSize(wxSizeEvent& event) {
	size_t width = event.GetSize().GetWidth();
	size_t height = event.GetSize().GetHeight();

	mTileView->SetSize( width - 40, height - 65 );
}
