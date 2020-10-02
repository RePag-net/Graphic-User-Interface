/****************************************************************************
  OSchalterGruppe.h
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
#include "OSchalter.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace GUI
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COSchalterGruppe : public COGrafik
		{
			friend LRESULT CALLBACK WndProc_SchalterGruppe(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);
			friend LRESULT CALLBACK WndProc_WechselSchalter(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		private:
			class COWechselSchalter : public COSchalter
			{
				friend LRESULT CALLBACK WndProc_WechselSchalter(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

			private:
				COSchalterGruppe* pSchalterGruppe;
				void __vectorcall WM_LButtonUp(WPARAM wParam, LPARAM lParam);

			protected:

			public:
				void __vectorcall COWechselSchalterV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA, COSchalterGruppe* pSchalterGruppeA);
				bool bGruppe;

			}; COWechselSchalter** vpWechselSchalter;
			unsigned char ucModus;
			unsigned char ucAnzahl;
			unsigned char ucIndex;
			short sMausPos_x;
			unsigned char ucSchalter;
			void __vectorcall WM_LButtonDown(LPARAM lParam);
			void __vectorcall WM_MouseMove(WPARAM wParam, LPARAM lParam);
			void __vectorcall WM_Paint(void);

		protected:

		public:
			void __vectorcall COSchalterGruppeV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA, unsigned char ucSchalteranzahl, unsigned char ucWechselmodus);
			void __vectorcall COTabellenKopfV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA, unsigned char ucSchalteranzahl);
			VMEMORY __vectorcall COFreiV(void);
			void __vectorcall ErstellSchalterInGruppe(unsigned char ucSchalterA, long lHoheA, long lBreiteA, long lPos_x, long lPos_y);
			void __vectorcall SetzAlleSchalter_Aus(void);
			void __vectorcall SetzAlleSchalter_Aktiv(bool bAktiv);
			unsigned char __vectorcall SelectSchalter(void);
			void __vectorcall Sperren(void);
			void __vectorcall Freigeben(void);
			void(__vectorcall* pfnAndernSpaltenBreite)(HWND hWndTabelle, unsigned char ucSpalte, long lBreite);
			void(__vectorcall* pfnWM_Paint)(COSchalterGruppe*, PAINTSTRUCT&);
			void __vectorcall Schalter_pfnWM_LButtonUp(unsigned char ucSchalterA, void(__vectorcall* pfnSchalter_WM_LButtonUp)(COSchalter*));
			void __vectorcall Schalter_pfnWM_LButtonDown(unsigned char ucSchalterA, void(__vectorcall* pfnSchalter_WM_LButtonDown)(COSchalter*));
			void __vectorcall Schalter_Beschriftung(unsigned char ucSchalterA, const char* pcBeschriftung);
			void __vectorcall Schalter_Beschriftung_Ein(unsigned char ucSchalterA, const char* pcBeschriftung);
			void __vectorcall Schalter_Beschriftung_Aus(unsigned char ucSchalterA, const char* pcBeschriftung);
			void __vectorcall Schalter_SetzSchalter(unsigned char ucSchalterA, bool bEinschalten);
			bool __vectorcall Schalter_Aktiv(unsigned char ucSchalterA);
			void __vectorcall Schalter_SetzAktiv(unsigned char ucSchalterA, bool bAktiv);
			void __vectorcall Schalter_SetzGruppe(unsigned char ucSchalterA, bool bGruppe);
			void __vectorcall Schalter_SetzSichtbar(unsigned char ucSchalterA, bool bSichtbar);
			void __vectorcall Schalter_SetzBreite(unsigned char ucSchalterA, long lBreiteA);
			bool __vectorcall Schalter_Eingeschaltet(unsigned char ucSchalterA);
			long& __vectorcall Schalter_Pos_X(unsigned char ucSchalterA, long& lPos_x);
			long& __vectorcall Schalter_Pos_Y(unsigned char ucSchalterA, long& lPos_y);
			void __vectorcall Schalter_NeuePosition(unsigned char ucSchalterA, long lPos_x, long lPos_y);
			void __vectorcall Schalter_AndernPosition(unsigned char ucSchalterA, long lPos_x, long lPos_y);
			void __vectorcall Schalter_Schriftart(unsigned char ucSchalterA, LOGFONT& lfSchrift);
			void __vectorcall Schalter_Hintergrundfarbe(unsigned char ucSchalterA, unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall Schalter_Hintergrundfarbe_1(unsigned char ucSchalterA, unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall Schalter_Hintergrundfarbe_2(unsigned char ucSchalterA, unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall Schalter_Hintergrundfarbe_3(unsigned char ucSchalterA, unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COSchalterGruppe* __vectorcall COSchalterGruppeV(const char* pcFensterName, unsigned int uiIDElement, unsigned char ucSchalteranzahl, unsigned char ucWechselmodus);
		__declspec(dllexport) COSchalterGruppe* __vectorcall COSchalterGruppeV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement, unsigned char ucSchalteranzahl, unsigned char ucWechselmodus);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}