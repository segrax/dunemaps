///-----------------------------------------------------------------
///
/// @file      dialogTeam.h
/// @author    Robert
/// Created:   19/11/2010 11:31:24
/// @section   DESCRIPTION
///            cDialogTeam class declaration
///
///------------------------------------------------------------------

#ifndef __CDIALOGTEAM_H__
#define __CDIALOGTEAM_H__

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
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/choice.h>
////Header Include End

////Dialog Style Start
#undef cDialogTeam_STYLE
#define cDialogTeam_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

#include <string>
using namespace std;

struct sTeam {
	string mAIMode,  mHouse, mUnitType, mMinUnits, mMaxUnits;
};

class cDialogTeam : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		sTeam			 mTeam;
		bool			 mCancel;

	public:
		cDialogTeam(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("DuneMaps - Team"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = cDialogTeam_STYLE);
		virtual ~cDialogTeam();
		void mButtonDoneClick(wxCommandEvent& event);
		void mButtonCancelClick(wxCommandEvent& event);
	
		inline sTeam			*teamGet() { if(mCancel) return 0; return &mTeam; }
		void				 teamSet( sTeam *pTeam );

	private:
		//Do not add custom control declarations between 
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxButton *mButtonDone;
		wxButton *mButtonCancel;
		wxStaticText *WxStaticText5;
		wxStaticText *WxStaticText4;
		wxStaticText *WxStaticText3;
		wxStaticText *WxStaticText2;
		wxTextCtrl *mTextMaxUnits;
		wxTextCtrl *mTextMinUnits;
		wxChoice *mChoiceUnitType;
		wxChoice *mChoiceAI;
		wxStaticText *WxStaticText1;
		wxChoice *mChoiceHouse;
		////GUI Control Declaration End
		
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_MBUTTONDONE = 1014,
			ID_MBUTTONCANCEL = 1013,
			ID_WXSTATICTEXT5 = 1011,
			ID_WXSTATICTEXT4 = 1010,
			ID_WXSTATICTEXT3 = 1009,
			ID_WXSTATICTEXT2 = 1008,
			ID_MTEXTMAXUNITS = 1006,
			ID_MTEXTMINUNITS = 1005,
			ID_MCHOICEUNITTYPE = 1004,
			ID_MCHOICEAI = 1003,
			ID_WXSTATICTEXT1 = 1002,
			ID_MCHOICEHOUSE = 1001,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
	
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
};

#endif
