//---------------------------------------------------------------------------
//
// Name:        DuneMapsApp.h
// Author:      Robert
// Created:     14/11/2010 08:04:04
// Description: 
//
//---------------------------------------------------------------------------

#ifndef __cDuneMapsApp_h__
#define __cDuneMapsApp_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
#endif

class cDuneMapsApp : public wxApp
{
	public:
		bool OnInit();
		int OnExit();
};

struct SDL_Surface;
wxBitmap SDL_To_Bitmap(SDL_Surface *surface);

#endif
