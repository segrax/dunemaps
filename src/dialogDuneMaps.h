///-----------------------------------------------------------------
///
/// @file      dialogDuneMaps.h
/// @author    Robert
/// Created:   14/11/2010 08:04:04
/// @section   DESCRIPTION
///            cDialogDuneMaps class declaration
///
///------------------------------------------------------------------

#ifndef __CDIALOGDUNEMAPS_H__
#define __CDIALOGDUNEMAPS_H__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/dialog.h>
#else
	#include <wx/wxprec.h>
#endif

//Do not add custom headers between 
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
////Header Include End

////Dialog Style Start
#undef cDialogDuneMaps_STYLE
#define cDialogDuneMaps_STYLE wxFULL_REPAINT_ON_RESIZE | wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class cPanelSurface;

class cDialogDuneMaps : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		cDialogDuneMaps(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Dune Maps"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = cDialogDuneMaps_STYLE);
		virtual ~cDialogDuneMaps();
	
	private:
		//Do not add custom control declarations between 
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		////GUI Control Declaration End
		
		cPanelSurface		*mTileView;

	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
	
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
		void OnSize(wxSizeEvent& event);
};

#endif
