/****************************************************************************
	OKnopf.h
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
#include "OTextZeile.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace GUI
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COKnopf : public COTextZeile
		{
			friend LRESULT CALLBACK WndProc_Knopf(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		private:
			STRGBFarbe stSchriftfarbe_1;
			STRGBFarbe stSchriftfarbe_Focus;

		protected:
			bool bAktiv;
			STRGBFarbe stHintergrundfarbe_1;
			STRGBFarbe stHintergrundfarbe_2;
			STRGBFarbe stHintergrundfarbe_3;
			void __vectorcall WM_SetFocus(void);
			void __vectorcall WM_KillFocus(void);
			void __vectorcall WM_Char(WPARAM wParam);
			void __vectorcall WM_LButtonDown(void);
			void __vectorcall WM_LButtonUp(WPARAM wParam, LPARAM lParam);
			void __vectorcall COKnopfV(VMEMORY vmSpeicher, const char* pcKlassenName, const char* pcFensterName, unsigned int uiIDElementA);

		public:
			void __vectorcall COKnopfV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);
			void(__vectorcall* pfnWM_LButtonDown)(COKnopf*);
			void(__vectorcall* pfnWM_LButtonUp)(COKnopf*);
			void __vectorcall Schriftfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall Schriftfarbe_Focus(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall Hintergrundfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall Hintergrundfarbe_1(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall Hintergrundfarbe_2(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall Hintergrundfarbe_3(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall Beschriftung(const char* pcBeschriftung);
			void __vectorcall SetzAktiv(bool bAktivA);
			bool __vectorcall Aktiv(void);
			void __vectorcall Sperren(void);
			void __vectorcall Freigeben(void);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COKnopf* __vectorcall COKnopfV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllexport) COKnopf* __vectorcall COKnopfV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}
