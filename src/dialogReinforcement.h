///-----------------------------------------------------------------
///
/// @file      dialogReinforcement.h
/// @author    Robert
/// Created:   19/11/2010 21:54:34
/// @section   DESCRIPTION
///            cDialogReinforcement class declaration
///
///------------------------------------------------------------------

#ifndef __CDIALOGREINFORCEMENT_H__
#define __CDIALOGREINFORCEMENT_H__

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
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/stattext.h>
////Header Include End

////Dialog Style Start
#undef cDialogReinforcement_STYLE
#define cDialogReinforcement_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

#include<string>
using namespace std;

struct sReinforce {
	string mRepeat;
	string mTimer;
	string mDirection;
	string mUnitType;
	string mHouse;
};

class cDialogReinforcement : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		bool	mCancel;

		sReinforce		mReinforce;

	public:
		cDialogReinforcement(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("DuneMaps - Reinforcement"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = cDialogReinforcement_STYLE);
		virtual ~cDialogReinforcement();
		sReinforce	 *reforcementGet()			{ if(mCancel) return 0; return &mReinforce; }

		void	 reinforcementSet( sReinforce *pReinforcement );
		void mButtonDoneClick(wxCommandEvent& event);
		void mButtonCancelClick(wxCommandEvent& event);

	private:
		//Do not add custom control declarations between 
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxTextCtrl *mTimer;
		wxCheckBox *mRepeat;
		wxStaticText *WxStaticText4;
		wxChoice *mChoiceDirection;
		wxStaticText *WxStaticText3;
		wxStaticText *WxStaticText2;
		wxChoice *mChoiceUnit;
		wxButton *mButtonCancel;
		wxButton *mButtonDone;
		wxChoice *mChoiceHouse;
		wxStaticText *WxStaticText1;
		////GUI Control Declaration End
		
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_MTIMER = 1019,
			ID_MREPEAT = 1018,
			ID_WXSTATICTEXT4 = 1017,
			ID_MCHOICEDIRECTION = 1016,
			ID_WXSTATICTEXT3 = 1015,
			ID_WXSTATICTEXT2 = 1013,
			ID_MCHOICEUNIT = 1012,
			ID_MBUTTONCANCEL = 1010,
			ID_MBUTTONDONE = 1009,
			ID_MCHOICEHOUSE = 1008,
			ID_WXSTATICTEXT1 = 1007,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
	
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
};

#endif
