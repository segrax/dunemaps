///-----------------------------------------------------------------
///
/// @file      frameDuneMaps.h
/// @author    Robert
/// Created:   14/11/2010 08:04:04
/// @section   DESCRIPTION
///            cFrameDuneMaps class declaration
///
///------------------------------------------------------------------

#ifndef __cFrameDuneMaps_H__
#define __cFrameDuneMaps_H__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/frame.h>
#else
	#include <wx/wxprec.h>
#endif

//Do not add custom headers between 
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/filedlg.h>
#include <wx/menu.h>
#include <wx/toolbar.h>
////Header Include End

class cPanelSurface;
class cHouse;
enum eHouse;

class cFrameDuneMaps : public wxFrame
{
	private:
		DECLARE_EVENT_TABLE();
		eHouse				mHouse;

	public:
		cFrameDuneMaps(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Dune Maps"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize );
		virtual ~cFrameDuneMaps();

		void Mnuquit1006Click(wxCommandEvent& event);
		void Mnunewscenario1005Click(wxCommandEvent& event);
		void Mnuloadscenario1002Click(wxCommandEvent& event);
		void WxToolBar1Tool(wxCommandEvent& event);
		void MnuLoadPak_ScenClick(wxCommandEvent& event);
		void WxToolBar2Tool(wxCommandEvent& event);
		void Mnufromini4002Click(wxCommandEvent& event);
		void MnuHouseChange(wxCommandEvent& event);
		void Mnuteams4014Click(wxCommandEvent& event);
		void Mnureinforcements4015Click(wxCommandEvent& event);
		void Mnubasics4003Click(wxCommandEvent& event);
		void Mnuhouses4016Click(wxCommandEvent& event);
	private:
		void Mnusavescenario1007Click(wxCommandEvent& event);
		//Do not add custom control declarations between 
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxFileDialog *WxOpenFileDialog1;
		wxFileDialog *WxSaveFileDialog1;
		wxMenuBar *WxMenuBar1;
		wxToolBar *WxToolBar2;
		wxToolBar *WxToolBar1;
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
			ID_MNU_FILE_1001 = 1001,
			ID_MNU_NEWSCENARIO_1005 = 1005,
			ID_MNU_LOADSCENARIO_1002 = 1002,
			ID_MNU_FROMINI_7000 = 7000,
			ID_MNU_LOADSCENFROMPAK = 1010,
			ID_MNU_SCEN = 6000,
			ID_MNU_SAVESCENARIO_1007 = 1007,
			ID_MNU_QUIT_1006 = 1006,
			ID_MNU_HOUSE_4005 = 4005,
			ID_MNU_HARKONNEN_4006 = 4006,
			ID_MNU_ATREIDES_4007 = 4007,
			ID_MNU_ORDOS_4008 = 4008,
			ID_MNU_FREMEN_4009 = 4009,
			ID_MNU_SARDAUKA_4010 = 4010,
			ID_MNU_MERCENARIES_4011 = 4011,
			ID_MNU_SCENARIO_4001 = 4001,
			ID_MNU_BASICS_4003 = 4003,
			ID_MNU_HOUSES_4016 = 4016,
			ID_MNU_TEAMS_4014 = 4014,
			ID_MNU_REINFORCEMENTS_4015 = 4015,
			
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_, //don't remove this value unless you have other enum values
			ID_WXTOOLBAR,
			ID_WXTOOLBAR1 = 3000,
			ID_WXTOOLBAR1_End = 3019,
			ID_WXTOOLBAR2 = 3020,
			ID_WXTOOLBAR2_End = 3039,
			ID_MNU_SCEN_End = 6100,
		};
	 
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
		void OnSize(wxSizeEvent& event);

		void loadToolbarStructures();
		void loadToolbarUnits();
		void loadScenariosFromPak();

};

#endif
