///-----------------------------------------------------------------
///
/// @file      panelMinimap.cpp
/// @author    Robert
/// Created:   21/11/2010 09:20:57
/// @section   DESCRIPTION
///            cPanelMinimap class implementation
///
///------------------------------------------------------------------
#include "appDuneMaps.h"
#include "panelSurface.h"
#include "panelMinimap.h"
#include "stdafx.h"

#include "dune/minimap.h"
#include "dune/screenPlayfield.h"
#include "dune/engine/scenario.h"
#include <wx/dcbuffer.h>

//Do not add custom headers between
//Header Include Start and Header Include End
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// cPanelMinimap
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(cPanelMinimap,wxFrame)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(cPanelMinimap::OnClose)
	EVT_PAINT(cPanelMinimap::OnPaint)
	EVT_MOUSE_EVENTS(cPanelMinimap::OnMouse)
END_EVENT_TABLE()
////Event Table End

cPanelMinimap::cPanelMinimap(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxFrame(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

cPanelMinimap::~cPanelMinimap()
{
}

void cPanelMinimap::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	SetSize(wxSize(134,156));
	Center();
	
	////GUI Items Creation End

	this->SetBackgroundColour( wxColor(0.0L) );


}

void cPanelMinimap::OnClose(wxCloseEvent& event)
{
	Show(false);
}

void cPanelMinimap::OnPaint(wxPaintEvent& event) {
	wxBufferedPaintDC tileView(this);

	cVideoSurface surface(64, 64);

	g_DuneEngine->minimapGet()->draw( &surface );

	tileView.Clear();
	tileView.DrawBitmap( SDL_To_Bitmap( surface.scaleTo(2) ), 0, 0 );
}

void cPanelMinimap::OnMouse(wxMouseEvent& event) {
	word *scale = g_DuneEngine->screenPlayfieldGet()->scaleGet();

	if( !event.LeftDown() )
		return;

	int mouseX = event.GetX() / 2;
	int mouseY = event.GetY() / 2;
	
	mouseX -= (g_DuneEngine->screenTilesMaxX() / 2);
	mouseY -= (g_DuneEngine->screenTilesMaxY() / 2);

	if( mouseY  < scale[1] )
		mouseY = scale[1];

	if( mouseY + g_DuneEngine->screenTilesMaxY() > scale[3] + scale[1] )
		mouseY = (scale[3] + scale[1]) - (g_DuneEngine->screenTilesMaxY() - 1);

	if( mouseX < scale[0] )
		mouseX = scale[0];

	if( mouseX + g_DuneEngine->screenTilesMaxX() > scale[2] + scale[0] )
		mouseX = (scale[2] + scale[0]) - (g_DuneEngine->screenTilesMaxX() - 1);

	// Set the new map location
	g_DuneEngine->screenPlayfieldGet()->mapTacticalSet( mouseX, mouseY );

	g_DuneEngine->mSurfaceGet()->Refresh(false);
	Refresh(false);
}
