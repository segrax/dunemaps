///-----------------------------------------------------------------
///
/// @file      dialogTeams.h
/// @author    Robert
/// Created:   19/11/2010 08:50:10
/// @section   DESCRIPTION
///            cDialogTeams class declaration
///
///------------------------------------------------------------------

#ifndef __CDIALOGTEAMS_H__
#define __CDIALOGTEAMS_H__

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
#undef cDialogTeams_STYLE
#define cDialogTeams_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxCLOSE_BOX
////Dialog Style End

class cDialogTeams : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		cDialogTeams(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("DuneMaps - Teams"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = cDialogTeams_STYLE);
		virtual ~cDialogTeams();
		void mButtonCancelClick(wxCommandEvent& event);
		void mButtonDoneClick(wxCommandEvent& event);
		void WxListCtrl1RightClick(wxListEvent& event);
		void Mnunewteam1005Click(wxCommandEvent& event);
		void Mnudeleteteam1006Click(wxCommandEvent& event);
		void mButtonNewClick(wxCommandEvent& event);
	
	private:
		//Do not add custom control declarations between 
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxButton *mButtonNew;
		wxMenu *mPopupTeams;
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
			ID_MBUTTONNEW = 1007,
			ID_MNU_NEWTEAM_1005 = 1005,
			ID_MNU_DELETETEAM_1006 = 1006,
			
			ID_MBUTTONDONE = 1004,
			ID_MBUTTONCANCEL = 1003,
			ID_WXLISTCTRL1 = 1001,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
	
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
};

#endif
