///-----------------------------------------------------------------
///
/// @file      dialogChoam.h
/// @author    Robert
/// Created:   25/11/2010 17:13:34
/// @section   DESCRIPTION
///            cDialogChoam class declaration
///
///------------------------------------------------------------------

#ifndef __CDIALOGCHOAM_H__
#define __CDIALOGCHOAM_H__

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
#include <wx/menu.h>
#include <wx/button.h>
#include <wx/listctrl.h>
////Header Include End

////Dialog Style Start
#undef cDialogChoam_STYLE
#define cDialogChoam_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class cDialogChoam : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		bool	mCancel;

	public:
		cDialogChoam(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("DuneMaps - Choam"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = cDialogChoam_STYLE);
		virtual ~cDialogChoam();
		void WxListCtrl1ItemActivated(wxListEvent& event);
		void mButtonDoneClick(wxCommandEvent& event);
		void mButtonCancelClick(wxCommandEvent& event);
		void WxButton1Click(wxCommandEvent& event);
	
	private:
		//Do not add custom control declarations between 
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxMenu *WxPopupMenu1;
		wxButton *WxButton1;
		wxButton *mButtonDone;
		wxButton *mButtonCancel;
		wxListCtrl *WxListCtrl1;
		////GUI Control Declaration End
		
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_WXBUTTON1 = 1007,
			ID_MBUTTONDONE = 1006,
			ID_MBUTTONCANCEL = 1005,
			ID_WXLISTCTRL1 = 1001,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
	
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
};

#endif
