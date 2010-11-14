///-----------------------------------------------------------------
///
/// @file      panelSurface.cpp
/// @author    Robert
/// Created:   13/11/2010 16:50:28
/// @section   DESCRIPTION
///            cPanelSurface class implementation
///
///------------------------------------------------------------------

#include "panelSurface.h"
#include "stdafx.h"
#include "screenPlayfield.h"
//Do not add custom headers between
//Header Include Start and Header Include End
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// cPanelSurface
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(cPanelSurface,wxPanel)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(cPanelSurface::OnClose)
	EVT_PAINT(cPanelSurface::OnPaint)
	EVT_SIZE(cPanelSurface::OnSize)
	EVT_MOUSE_EVENTS(cPanelSurface::OnMouse) 
	EVT_TIMER(inputTimer, cPanelSurface::OnInputTimer)


END_EVENT_TABLE()
////Event Table End

cPanelSurface::cPanelSurface(wxWindow *parent, wxWindowID id, const wxPoint &position, const wxSize& size)
: wxPanel(parent, id, position, size) {
	
	mScale = 2;
	mMouseX = mMouseY = 0;
	mTimer = 0;


	CreateGUIControls();
}

cPanelSurface::~cPanelSurface() {
	mTimer->Stop();
	Sleep(200);
	delete mTimer;
}

void cPanelSurface::CreateGUIControls() {
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	SetSize(8,8,320,334);
	Center();
	
	////GUI Items Creation End

	mTimer = new wxTimer(this, inputTimer);
	mTimer->Start(100);	// Milliseconds

}

void cPanelSurface::OnClose(wxCloseEvent& event) {
	Destroy();
}

void cPanelSurface::OnPaint(wxPaintEvent& event) {
	wxBufferedPaintDC tileView(this);

	size_t width = this->GetSize().GetWidth();
	size_t height = this->GetSize().GetHeight();

	cVideoSurface surface(width, height);

	g_DuneEngine->screenPlayfieldGet()->draw( &surface );

	tileView.Clear();
	tileView.DrawBitmap( SDL_To_Bitmap( surface.scaleTo(mScale) ), 0, 0 );

}

void cPanelSurface::OnSize(wxSizeEvent& event) {
	size_t width = this->GetSize().GetWidth();
	size_t height = this->GetSize().GetHeight();

	g_DuneEngine->screenPlayfieldGet()->widthSet( width );
	g_DuneEngine->screenPlayfieldGet()->heightSet( height );

	g_DuneEngine->screenTilesMaxXSet( width / mScale);
	g_DuneEngine->screenTilesMaxYSet( height / mScale);
}

void cPanelSurface::OnMouse(wxMouseEvent& event) {
	// Click outside of tile viewer?
	if(event.GetX() < 0 || event.GetY() < 0 || event.GetX() >= this->GetSize().GetWidth() || event.GetY() >= this->GetSize().GetHeight()) {
		mMouseX = 5;
		mMouseY = 5;
		
		return;
	}
	
	mMouseX = event.GetX() / mScale;
	mMouseY = event.GetY() / mScale;
	if( mMouseX == 0 )
		mMouseX = 5;
	if( mMouseY == 0 )
		mMouseY = 5;

	// Left Mouse
	if( event.LeftDown() ) {

		// Select the unit, structure or map piece under the cursor
		g_DuneEngine->screenPlayfieldGet()->buttonPressLeft( mMouseX, mMouseY );
		this->Refresh(false);
	}

}

void cPanelSurface::OnInputTimer(wxTimerEvent& event) {

	// Check for movement along the edge of the tile viewer
	if( g_DuneEngine->screenPlayfieldGet()->mouseMove( mMouseX, mMouseY ) )
		this->Refresh(false);
}

wxBitmap cPanelSurface::SDL_To_Bitmap(SDL_Surface *surface) {

	return wxBitmap((const char*)surface->pixels, surface->w, surface->h ,surface->format->BitsPerPixel);
}
