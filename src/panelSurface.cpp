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
#include "frameDuneMaps.h"
#include "panelMinimap.h"

#include "stdafx.h"
#include "screenPlayfield.h"
#include "appDuneMaps.h"
//Do not add custom headers between
//Header Include Start and Header Include End
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End
#include "dune/engine/scenario.h"
#include "dune/map.h"
#include "dune/engine/objects/object.h"
#include "dune/engine/objects/mapCell.h"
#include "dune/engine/objects/unit.h"

//----------------------------------------------------------------------------
// cPanelSurface
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(cPanelSurface, wxPanel)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(cPanelSurface::OnClose)
	EVT_SIZE(cPanelSurface::OnSize)
	EVT_RIGHT_DOWN(cPanelSurface::cPanelSurfaceRightDown)
	EVT_PAINT(cPanelSurface::OnPaint)
	EVT_MOUSE_EVENTS(cPanelSurface::OnMouse)
	EVT_MENU(ID_MNU_UNITROTATE_1003 , cPanelSurface::Mnuunitrotate1003Click)
	EVT_MENU(ID_MNU_ADDBLOOM_1001 , cPanelSurface::Mnuaddbloom1001Click)
	EVT_MENU(ID_MNU_ADDSPICEFIELD_1002 , cPanelSurface::Mnuaddspicefield1002Click)
	EVT_TIMER(inputTimer, cPanelSurface::OnInputTimer)
	EVT_MENU_RANGE(ID_MNU_ORDER_2000, ID_MNU_ORDER_2000 + 14, cPanelSurface::menuActionSet)
END_EVENT_TABLE()
////Event Table End

cPanelSurface::cPanelSurface(wxWindow *parent, wxWindowID id, const wxPoint &position, const wxSize& size)
: wxPanel(parent, id, position, size, wxFRAME_NO_TASKBAR | wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	
	mScale = 2;
	mMouseX = mMouseY = 0;
	mTimer = 0;
	mMapCell = 0;

	mMouseIgnore = false;

	CreateGUIControls();

	menuOrdersBuild();
}

cPanelSurface::~cPanelSurface() {
	mTimer->Stop();
	Sleep(200);

	delete mTimer;
	delete WxPopupMenu1;

}

void cPanelSurface::CreateGUIControls() {
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	mPopupObject = new wxMenu(wxT(""));mPopupObject->Append(ID_MNU_UNITROTATE_1003, wxT("Rotate"), wxT(""), wxITEM_NORMAL);
	wxMenu *ID_MNU_STARTINGORDERS_1004_Obj = new wxMenu();
	ID_MNU_STARTINGORDERS_1004_Obj->Append(ID_MNU_ORDER_2000, wxT("Order"), wxT(""), wxITEM_NORMAL);
	mPopupObject->Append(ID_MNU_STARTINGORDERS_1004, wxT("Orders"), ID_MNU_STARTINGORDERS_1004_Obj);

	WxPopupMenu1 = new wxMenu(wxT(""));WxPopupMenu1->Append(ID_MNU_ADDBLOOM_1001, wxT("Add Spice Bloom"), wxT(""), wxITEM_NORMAL);
	WxPopupMenu1->Append(ID_MNU_ADDSPICEFIELD_1002, wxT("Add Spice Field"), wxT(""), wxITEM_NORMAL);

	SetSize(wxSize(320,334));
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

	playfieldSizeUpdate( mScale, width, height );
}

void cPanelSurface::OnMouse(wxMouseEvent& event) {
	mMouseIgnore = false;
	int scroll = event.GetWheelRotation();

	if( scroll ) {

		if( scroll > 0 ) {
			if( mScale < 4 )
				playfieldSizeUpdate( mScale + 1 );
			else
				playfieldSizeUpdate( 1 );

		} else {

			if( mScale >= 2 )
				playfieldSizeUpdate( mScale - 1 );
			else
				playfieldSizeUpdate( 4 );
		}

	}

	// Click outside of tile viewer?
	if(event.GetX() < 0 || event.GetY() < 0 || event.GetX() >= this->GetSize().GetWidth() || event.GetY() >= this->GetSize().GetHeight()) {
		mMouseIgnore = true;
		
		return;
	}
	
	// Get the Mouse X/Y, without scale
	mMouseX = event.GetX() / mScale;
	mMouseY = event.GetY() / mScale;

	//
	if( mMouseX == 0 || mMouseY == 0 )
		mMouseIgnore = true;

	// Select the map-cell under the cursor
	if(!mMouseIgnore) {
		word mapIndex = g_DuneEngine->scenarioGet()->mapGet()->posXYtoIndex( g_DuneEngine->screenPlayfieldGet()->mapXGet() + (mMouseX / 16), g_DuneEngine->screenPlayfieldGet()->mapYGet() + (mMouseY / 16) );
		mMapCell = g_DuneEngine->scenarioGet()->mapGet()->mapCellGet( mapIndex );
	} else
		mMapCell = 0;

	// Left Mouse
	if( event.LeftDown() ) {

		// Select the unit, structure or map piece under the cursor
		g_DuneEngine->screenPlayfieldGet()->buttonPressLeft( mMouseX, mMouseY );
		Refresh(false);
	}

	if( scroll ) {
		short int *scale = (short int*) g_DuneEngine->screenPlayfieldGet()->scaleGet();

		short int x = g_DuneEngine->screenPlayfieldGet()->mapXGet() + (mMouseX / 16);
		short int  y = g_DuneEngine->screenPlayfieldGet()->mapYGet() + (mMouseY / 16);
		
		x -= g_DuneEngine->screenTilesMaxX() / 4;
		y -= g_DuneEngine->screenTilesMaxY() / 4;
		
		if( x < scale[0] )
			x = scale[0];

		if( x + g_DuneEngine->screenTilesMaxX() > scale[0] + scale[2] )
			x = scale[2] - (g_DuneEngine->screenTilesMaxX());

		if( y < scale[1] )
			y = scale[1];

		if( y + g_DuneEngine->screenTilesMaxY() > scale[1] + scale[3] )
			y = scale[3] - (g_DuneEngine->screenTilesMaxY());

		if( x >= scale[0] && x <= scale[0] + scale[2] &&
			y >= scale[1] && y <= scale[1] + scale[3] ) {
				// Centre on mouse cursor
				g_DuneEngine->screenPlayfieldGet()->mapTacticalSet( x , y );
				Refresh(false);
		}

	}
}

void cPanelSurface::OnInputTimer(wxTimerEvent& event) {

	if(!mMouseIgnore)
		// Check for movement along the edge of the tile viewer
		if( g_DuneEngine->screenPlayfieldGet()->mouseMove( mMouseX, mMouseY ) )
			Refresh(false);
}

void cPanelSurface::playfieldSizeUpdate( size_t pScale, size_t pWidth, size_t pHeight ) {
	if(!pScale)
		pScale = mScale;

	if( !pWidth )
		pWidth = this->GetSize().GetWidth();

	if( !pHeight )
		pHeight = this->GetSize().GetHeight();

	mScale = pScale;

	g_DuneEngine->screenPlayfieldGet()->widthSet( pWidth );
	g_DuneEngine->screenPlayfieldGet()->heightSet( pHeight );

	g_DuneEngine->screenTilesMaxXSet( pWidth / mScale);
	g_DuneEngine->screenTilesMaxYSet( pHeight / mScale);

	Refresh(false);
	g_DuneEngine->frameGet()->minimapGet()->Refresh(false);
}

/*
 * Mnuaddbloom1001Click : 
 */
void cPanelSurface::Mnuaddbloom1001Click(wxCommandEvent& event) {
	stringstream	bloom;
	string			blooms = g_DuneEngine->scenarioGet()->mapBloomGet();
	
	word			mapIndex = g_DuneEngine->scenarioGet()->mapGet()->posXYtoIndex( g_DuneEngine->screenPlayfieldGet()->mapXGet() + (mMouseX / 16), g_DuneEngine->screenPlayfieldGet()->mapYGet() + (mMouseY / 16) );

	bloom << mapIndex;
	if(blooms.size())
		blooms.append(",");
	blooms.append( bloom.str() );

	g_DuneEngine->scenarioGet()->mapBloomSet(blooms);
	g_DuneEngine->scenarioGet()->mapLoad();

	playfieldSizeUpdate();
}

/*
 * Mnuaddspicefield1002Click
 */
void cPanelSurface::Mnuaddspicefield1002Click(wxCommandEvent& event) {
	stringstream	field;
	string			fields = g_DuneEngine->scenarioGet()->mapFieldGet();
	
	word			mapIndex = g_DuneEngine->scenarioGet()->mapGet()->posXYtoIndex( g_DuneEngine->screenPlayfieldGet()->mapXGet() + (mMouseX / 16), g_DuneEngine->screenPlayfieldGet()->mapYGet() + (mMouseY / 16) );

	field << mapIndex;
	if(fields.size())
		fields.append(",");
	fields.append( field.str() );

	g_DuneEngine->scenarioGet()->mapFieldSet( fields );
	g_DuneEngine->scenarioGet()->mapLoad();

	playfieldSizeUpdate();
}

/*
 * cPanelSurfaceRightDown : Right Click on the panel, display the popup menu
 */
void cPanelSurface::cPanelSurfaceRightDown(wxMouseEvent& event) {
	
	OnMouse( event );

	// No unit in selected cell, display the map menu
	if(!(*mMapCell)->hasUnit())
		PopupMenu( WxPopupMenu1 );

	else {
		// Reset orders menu
		menuOrdersReset();

		// Check the action thats in use by the unit
		cUnit *unit = (cUnit*) (*mMapCell)->objectGet();
		word actionID = unit->actionGet();
		wxMenuItem		*item = mPopupObject->FindItem(ID_MNU_ORDER_2000 + actionID);
		item->Check();

		// Show the unit menu
		PopupMenu( mPopupObject );
		
	}
}

/*
 * Menu: Rotate unit
 */
void cPanelSurface::Mnuunitrotate1003Click(wxCommandEvent& event) {

	if(!(*mMapCell)->hasUnit())
		return;

	cUnit *unit = (cUnit*) (*mMapCell)->objectGet();

	cUnitAngle *angle = unit->angleBaseGet();

	size_t newAngle = angle->_Current;
	newAngle += 64;

	angle->_Current = newAngle;

	playfieldSizeUpdate();
}

// Menu: Uncheck all actions
void cPanelSurface::menuOrdersReset() {
	
	for( int i = 0; i < 14; ++i ) {

		wxMenuItem		*item = mPopupObject->FindItem(ID_MNU_ORDER_2000 + i);
		item->Check(false);
	}
}

// Menu: Build a list of actions
void cPanelSurface::menuOrdersBuild() {

	wxMenuItem		*item = mPopupObject->FindItem(ID_MNU_ORDER_2000);

	wxMenu			*menu = item->GetMenu();
	menu->Remove( item );

	for( int i = 0; i < 14; ++i ) {
		sActionData *action = g_DuneEngine->resourcesGet()->actionGet(i);

		menu->InsertCheckItem(i, ID_MNU_ORDER_2000 + i, action->Name );
	}
}

// Menu: Set an unit with a starting action
void cPanelSurface::menuActionSet(wxCommandEvent& event) {

	if(!(*mMapCell)->hasUnit())
		return;

	int actionID = event.GetId() - ID_MNU_ORDER_2000;

	cUnit *unit = (cUnit*) (*mMapCell)->objectGet();

	unit->actionSet( actionID );
}
