///-----------------------------------------------------------------
///
/// @file      dialogBasics.h
/// @author    Robert
/// Created:   18/11/2010 11:22:30
/// @section   DESCRIPTION
///            cDialogBasics class declaration
///
///------------------------------------------------------------------

#ifndef __CDIALOGBASICS_H__
#define __CDIALOGBASICS_H__

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
#include <wx/radiobut.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/checklst.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/notebook.h>
////Header Include End

////Dialog Style Start
#undef cDialogBasics_STYLE
#define cDialogBasics_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class cDialogBasics : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		
		void	missionLoad();

	public:
		cDialogBasics(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("DuneMaps - Map Basics"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = cDialogBasics_STYLE);
		virtual ~cDialogBasics();
		void mButtonDoneClick(wxCommandEvent& event);
		void mButtonCancelClick(wxCommandEvent& event);
		byte flagsGet( wxCheckListBox *checkList );
		void flagsSet( wxCheckListBox *checkList, size_t pFlags );

	private:
		//Do not add custom control declarations between 
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxRadioButton *WxRadioButton2;
		wxRadioButton *WxRadioButton1;
		wxStaticBox *WxStaticBox4;
		wxButton *mButtonCancel;
		wxButton *mButtonDone;
		wxTextCtrl *WxEdit1;
		wxStaticText *Timeout;
		wxStaticBox *WxStaticBox3;
		wxCheckListBox *WxCheckListBox2;
		wxStaticBox *WxStaticBox2;
		wxCheckListBox *WxCheckListBox1;
		wxStaticBox *WxStaticBox1;
		wxPanel *WxNoteBookPage2;
		wxPanel *WxNoteBookPage1;
		wxNotebook *WxNotebook1;
		////GUI Control Declaration End
		
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_WXRADIOBUTTON2 = 1020,
			ID_WXRADIOBUTTON1 = 1019,
			ID_WXSTATICBOX4 = 1017,
			ID_MBUTTONCANCEL = 1015,
			ID_MBUTTONDONE = 1014,
			ID_WXEDIT1 = 1011,
			ID_TIMEOUT = 1010,
			ID_WXSTATICBOX3 = 1009,
			ID_WXCHECKLISTBOX2 = 1008,
			ID_WXSTATICBOX2 = 1007,
			ID_WXCHECKLISTBOX1 = 1006,
			ID_WXSTATICBOX1 = 1005,
			ID_WXNOTEBOOKPAGE2 = 1003,
			ID_WXNOTEBOOKPAGE1 = 1002,
			ID_WXNOTEBOOK1 = 1001,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
	
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
};

#endif
