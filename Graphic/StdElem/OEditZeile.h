/****************************************************************************
  OEditZeile.h
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
#include "OTextZeile.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace GUI
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COEditZeile : public COTextZeile
		{
			friend LRESULT CALLBACK WndProc_EditZeile(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		private:
			COStringA* vasZeichenMaske;
			bool __vectorcall ZeichenMaske_Einfugen(WPARAM wParam);
			bool __vectorcall ZeichenMaske_Einfugen_Prufen(WPARAM wParam, VMBLOCK vbZeichen_Maske);
			bool __vectorcall ZeichenMaske_Loschen(void);
			bool __vectorcall ZeichenMaske_FestRechts(void);
			bool __vectorcall ZeichenMaske_FestLinks(void);
			void __vectorcall DeSelect(void);
			void __vectorcall Select_Loschen(HDC hdc);

		protected:
			HMENU hMenu;
			POINT ptCaret;
			char cSelect;
			RECT rcSelect;
			long lTextPos;
			unsigned long ulZeichen_max;
			unsigned char ucZeichenVorgabe;
			unsigned long ulZeichenPos;
			unsigned long ulSelectPos;
			STRGBFarbe stSelectSchriftfarbe;
			STRGBFarbe stSelectHintergrundfarbe;
			void __vectorcall WM_Create(HWND hWnd);
			void __vectorcall WM_SetFocus(void);
			void __vectorcall WM_KillFocus(void);
			void __vectorcall WM_KeyDown(WPARAM wParam, LPARAM lParam);
			void __vectorcall WM_Char(WPARAM wParam);
			bool __vectorcall WM_Command(WPARAM wParam);
			void __vectorcall WM_ContexMenu(LPARAM lParam);
			void __vectorcall WM_LButtonUp(WPARAM wParam, LPARAM lParam);
			void __vectorcall WM_LButtonDown(WPARAM wParam, LPARAM lParam);
			void __vectorcall WM_MouseMove(WPARAM wParam, LPARAM lParam);
			void __vectorcall WM_LButtonDBClick(WPARAM wParam, LPARAM lParam);
			void __vectorcall WM_Paint(void);
			bool __vectorcall ZeichenVorgabe(WPARAM wParam);
			void __vectorcall COEditZeileV(VMEMORY vmSpeicher, const char* pcKlassenName, const char* pcFensterName, unsigned int uiIDElementA);

		public:
			void __vectorcall COEditZeileV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);
			VMEMORY __vectorcall COFreiV(void);
			void(__vectorcall* pfnWM_Char_Return)(COEditZeile*);
			void(__vectorcall* pfnWM_Char_Escape)(COEditZeile*);
			void(__vectorcall* pfnWM_KillFocus)(COEditZeile*);
			bool(__vectorcall* pfnWM_Command)(COEditZeile*, WPARAM);
			void(__vectorcall* pfnWM_LButtonDown)(COEditZeile*, WPARAM, LPARAM);
			void(__vectorcall* pfnWM_LButtonUp)(COEditZeile*, WPARAM, LPARAM);
			void(__vectorcall* pfnWM_LButtonDBClick)(COEditZeile*, WPARAM, LPARAM);
			void __vectorcall SetzVerfugbar(bool bVerfugbar);
			void __vectorcall Text(char* pcText);
			void __vectorcall SelectSchriftfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall SelectHintergrundfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall SetzZeichen_Max(unsigned long ulZeichen);
			unsigned long __vectorcall Zeichen_Max(void);
			void __vectorcall Zeichenvorgabe(unsigned char ucZeichenvorgabeA);
			void __vectorcall Zeichenmaske(const char* pcZeichenmaske);
			COStringA* __vectorcall Zeichenmaske(COStringA* pasZeichenmaske);
			void __vectorcall SelectAlles(void);
			void __vectorcall SelectEntfernen(void);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COEditZeile* __vectorcall COEditZeileV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllexport) COEditZeile* __vectorcall COEditZeileV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}
