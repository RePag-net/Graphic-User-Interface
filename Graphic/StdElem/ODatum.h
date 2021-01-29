/****************************************************************************
	ODatum.h
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
#include "OEditZeile.h"
#include "OListBox.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace GUI
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) CODatum : public COGrafik
		{
			friend LRESULT CALLBACK WndProc_Datum(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);
			friend LRESULT CALLBACK WndProc_EditDatum(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		private:
			class COEditDatum : public COEditZeile
			{
				friend LRESULT CALLBACK WndProc_EditDatum(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

			private:
				CODatum* pDatum;
				bool __vectorcall WM_Command_EditDatum(WPARAM wParam);
				void __vectorcall WM_Char_EditDatum(WPARAM wParam);
				void __vectorcall WM_LButtonUp(WPARAM wParam, LPARAM lParam);
				void __vectorcall WM_LButtonDBClick(LPARAM lParam);
				void __vectorcall WM_Paint_EditDatum(void);
				inline bool __vectorcall DatumTextzuSYSTEMTIME(SYSTEMTIME& stSystemTime, unsigned char& ucStelle_DatumZeit);
				inline bool __vectorcall ZeitTextzuSYSTEMTIME(SYSTEMTIME& stSystemTime, unsigned char& ucStelle_DatumZeit);
				bool __vectorcall TextzuSYSTEMTIME(void);

			protected:

			public:
				void __vectorcall COEditDatumV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA, CODatum* pDatumA);
				VMEMORY __vectorcall COFreiV(void);
				void __vectorcall DatumZeitText(void);
				COStringA* vasFormat_Datum;
				COStringA* vasFormat_Zeit;
				bool bNeueMaske;

			}; COEditDatum* edDatum;
			COListBox* lbMonat;
			SYSTEMTIME stDatum;
			LOGFONT lfSchrift;
			HFONT hFont;
			STRGBFarbe stSchriftfarbe;
			STRGBFarbe stKopf_Hintergrundfarbe;
			bool bAnordnung_DatumZeit;
			long lBreite_Kalender;
			long lHohe_EditDatum;
			unsigned char ucSpaltenbreite;
			unsigned char ucZeilenhohe;
			unsigned char ucErsterWochentag;
			unsigned char ucLetzterTag;
			unsigned char ucJahr_Pos_x;
			void __vectorcall WM_Create(HWND hWnd);
			void __vectorcall WM_KillFocus(void);
			bool __vectorcall WM_Command(WPARAM wParam);
			bool __vectorcall WM_LButtonUp(LPARAM lParam);
			void __vectorcall WM_Paint(void);
			void __vectorcall SetzWochentag(void);

		protected:

		public:
			void __vectorcall CODatumV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);
			VMEMORY __vectorcall COFreiV(void);
			long& __vectorcall Hohe(long& lHoheA);
			long& __vectorcall Hohe_EditDatum(long& lHoheA);
			long& __vectorcall Breite_EditDatum(long& lBreiteA);
			RECT& __vectorcall Fenster(RECT& rcFenster);
			void __vectorcall NeueFensterPosition(long lPos_x, long lPos_y);
			void __vectorcall NeueFensterPosition(POINT& ptPositionA);
			void __vectorcall NeuesFenster(long lHoheA, long lBreiteA, long lPos_x, long lPos_y);
			void __vectorcall NeueFensterGrosse(long lHoheA, long lBreiteA);
			void __vectorcall NeueFensterHohe(long lHoheA);
			void __vectorcall NeueFensterBreite(long lBreiteA);
			void __vectorcall AndernFensterGrosse(long lHoheA, long lBreiteA);
			void __vectorcall AndernFensterPosition(long lPos_x, long lPos_y);
			void __vectorcall Schriftart(LOGFONT& lfSchriftA);
			LOGFONT& __vectorcall Schrift(LOGFONT& lfSchriftA);
			void __vectorcall Schriftfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall Hintergrundfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall Kopf_Hintergrundfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall SetzVerfugbar(bool bVerfugbar);
			void __vectorcall SetzSichtbar(bool bSichtbar);
			bool __vectorcall Sichtbar(void);
			void(__vectorcall* pfnWM_LButtonUp)(CODatum*);
			void(__vectorcall* pfnWM_Char_Return)(CODatum*);
			void(__vectorcall* pfnWM_Char_Escape)(CODatum*);
			bool(__vectorcall* pfnWM_Command)(CODatum*, WPARAM);
			void __vectorcall ErweiterteBreite_Kalender(long lBreiteA);
			void __vectorcall SetzHohe_EditDatum(long lHoheA);
			void __vectorcall Zeichenvorgabe_EditDatum_ZVSICHTBAR(bool bSichtbar);
			void __vectorcall Schriftausrichtung_EditDatum(unsigned char ucSchriftausrichtung);
			void __vectorcall DatumZeitFormat(const char* pcFormat_Datum, const char* pcFormat_Zeit, bool bAnordnung_DatumZeitA);
			COStringA* __vectorcall Datumformat(COStringA* pasFormat_Datum);
			COStringA* __vectorcall Zeitformat(COStringA* pasFormat_Zeit);
			void __vectorcall SetzDatum(COTime* pzDatum);
			bool __vectorcall SetzDatum(STTime& stZeit);
			void __vectorcall Today(void);
			void __vectorcall Morgen(void);
			void __vectorcall Lese(char* pcInhalt);
			void __vectorcall Schreibe(const char* pcInhalt);
			COStringA* __vectorcall Datum(COStringA* pasDatum);
			COTime* __vectorcall ZEIT(COTime* pzDatum);
			SYSTEMTIME __vectorcall SystemTime(void);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) CODatum* __vectorcall CODatumV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllexport) CODatum* __vectorcall CODatumV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}
