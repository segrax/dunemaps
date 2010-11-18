///-----------------------------------------------------------------
///
/// @file      dialogHouses.h
/// @author    Robert
/// Created:   18/11/2010 08:56:20
/// @section   DESCRIPTION
///            cDialogHouses class declaration
///
///------------------------------------------------------------------

#ifndef __CDIALOGHOUSES_H__
#define __CDIALOGHOUSES_H__

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
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/panel.h>
#include <wx/listbox.h>
////Header Include End

////Dialog Style Start
#undef cDialogHouses_STYLE
#define cDialogHouses_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

#include <map>

using namespace std;

struct sHouse {

	short int	 _credits,	_creditQuota, _maxUnits;
	bool		 _human;

	sHouse( short int pCredits = 0, short int pQuota = 0, short int pMaxUnits = 0 ) {
		_credits = pCredits; _creditQuota = pQuota; _maxUnits = pMaxUnits;
		_human = false;
	}
};

class cDialogHouses : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();

		map< int, sHouse* >	 _houses;

		sHouse				*mHouse;

	public:
		cDialogHouses(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("DuneMaps - House Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = cDialogHouses_STYLE);
		virtual ~cDialogHouses();
		void WxListBox1Selected(wxCommandEvent& event);
		void mButtonDoneClick(wxCommandEvent& event);
		void mButtonCancelClick(wxCommandEvent& event);

	private:
		//Do not add custom control declarations between 
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxButton *mButtonCancel;
		wxButton *mButtonDone;
		wxStaticText *WxStaticText3;
		wxStaticText *WxStaticText2;
		wxStaticText *WxStaticText1;
		wxTextCtrl *mEditMaxUnits;
		wxTextCtrl *mEditCredits;
		wxTextCtrl *mEditQuota;
		wxCheckBox *mCheckHuman;
		wxPanel *WxPanel1;
		wxListBox *WxListBox1;
		////GUI Control Declaration End
		
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_MBUTTONCANCEL = 1013,
			ID_MBUTTONDONE = 1011,
			ID_WXSTATICTEXT3 = 1010,
			ID_WXSTATICTEXT2 = 1009,
			ID_WXSTATICTEXT1 = 1008,
			ID_MEDITMAXUNITS = 1007,
			ID_MEDITCREDITS = 1006,
			ID_MEDITQUOTA = 1005,
			ID_MCHECKHUMAN = 1004,
			ID_WXPANEL1 = 1003,
			ID_WXLISTBOX1 = 1002,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_, //don't remove this value unless you have other enum values
			ID_HOUSE = 2000,
		};
	
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
		void updateHouse();
		void updateValues();
};

#endif
