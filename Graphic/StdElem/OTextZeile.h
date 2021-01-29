/****************************************************************************
	OTextZeile.h
	For more information see https://github.com/RePag-net/Graphic-User-Interface
*****************************************************************************/

/****************************************************************************
	The MIT License(MIT)

	Copyright(c) 2021 René Pagel

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
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace GUI
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COTextZeile : public COGrafik
		{
			friend LRESULT CALLBACK WndProc_TextZeile(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		private:

		protected:
			LOGFONT lfSchrift;
			HFONT hFont;
			STRGBFarbe stSchriftfarbe;
			COStringA* vasInhalt;
			long lZeichen_mittel;
			long lZeichen_hohe;
			unsigned char ucSchriftausrichtung;
			void __vectorcall WM_Create(HWND hWnd);
			void __vectorcall WM_Paint(void);
			void __vectorcall ZeichenMetric(HWND hWnd);
			void __vectorcall Schriftausrichtung(HDC hdc, SIZE& stZeichengrosse, POINT& ptText);
			void __vectorcall COTextZeileV(VMEMORY vmSpeicher, const char* pcKlassenName, const char* pcFensterName, unsigned int uiIDElementA);

		public:
			void __vectorcall COTextZeileV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);
			VMEMORY __vectorcall COFreiV(void);
			void __vectorcall Text(char* pcText);
			void __vectorcall Schriftart(LOGFONT& lfSchriftA);
			LOGFONT& __vectorcall Schrift(LOGFONT& lfSchriftA);
			COStringA* __vectorcall Inhalt(COStringA* vasInhaltA);
			void __vectorcall Schriftfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall Schriftausrichtung(unsigned char ucSchriftausrichtungA);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COTextZeile* __vectorcall COTextZeileV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllexport) COTextZeile* __vectorcall COTextZeileV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}
