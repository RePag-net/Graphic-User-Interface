/****************************************************************************
	OMedia.h
	For more information see https://github.com/RePag-net/Graphic-User-Interface
*****************************************************************************/

/****************************************************************************
	The MIT License(MIT)

	Copyright(c) 2020 René Pagel

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this softwareand associated documentation files(the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions :

	The above copyright noticeand this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
******************************************************************************/

#pragma once

namespace RePag
{
	namespace GUI
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COMedia : public COGrafik
		{
			friend void __vectorcall frBalken_WM_LButtonDown(COBalken* pBalken, WPARAM wParam, LPARAM lParam);
			friend void __vectorcall frBalken_WM_MouseMove(COBalken* pBalken, WPARAM wParam, LPARAM lParam);

		private:
			COTextZeile* tzZeit;
			COKnopf* kStartStop;
			COBalken* bkPosition;
			long lLange;

		protected:
			HMENU hMenu;
			HWND hWndMCI;
			HANDLE htPosition;
			LOGFONT lfSchrift;
			STRGBFarbe stSchriftfarbe;
			COStringA* vasInfotext;
			COStringA* vasDateiname;
			COStringA* vasTypEndung;
			bool bGeandert;
			unsigned char ucRechte;
			void __vectorcall WM_Create(HWND hWnd);
			void __vectorcall WM_Size(LPARAM lParam);
			void __vectorcall WM_Char(WPARAM wParam);
			void __vectorcall WM_ContexMenu(LPARAM lParam);
			void __vectorcall MCIWNDM_NotifyMode(LPARAM lParam);
			void __vectorcall DateiEntfernen(void);
			void __vectorcall MCIOffnen(void);
			void __vectorcall COMediaV(VMEMORY vmSpeicher, const char* pcKlassenName, const char* pcFensterName, unsigned int uiIDElementA);

		public:
			VMEMORY __vectorcall COFreiV(void);
			void(__vectorcall* pfnWM_Char_Return)(COMedia*);
			void(__vectorcall* pfnWM_Char_Escape)(COMedia*);
			bool(__vectorcall* pfnWM_Command)(COMedia*, WPARAM);
			void __vectorcall Timer_Position(void);
			void __vectorcall SetzVerfugbar(bool bVerfugbar);
			void __vectorcall Infotext(char* pcText);
			void __vectorcall Schriftart(LOGFONT& lfSchriftA);
			void __vectorcall Schriftfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall Lese(VMBLOCK& vbMedia, unsigned long& ulBytes, COStringA* pasDateiendung);
			void __vectorcall Schreibe(VMBLOCK vbMedia, unsigned long ulBytes, VMBLOCK vbDateiendung, unsigned char ucDateiendung_Lange);
			bool __vectorcall DateiLesen(char* pcDateinamen);
			bool __vectorcall DateiSchreiben(char* pcDateinamen);
			bool __vectorcall Geandert(void);
			void __vectorcall Berechtigungen(unsigned char ucRechteA);

		};
		//-----------------------------------------------------------------------------------------------------------------------------------------
	}
}