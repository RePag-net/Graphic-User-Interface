/****************************************************************************
	OTabBasis.cpp
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
#include "OFeld.h"

namespace RePag
{
	namespace GUI
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COTabBasis : public COGrafik
		{
			friend LRESULT CALLBACK WndProc_Tabelle(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);
			friend void __vectorcall frWM_Paint_Kopf(COSchalterGruppe* psgKopf, PAINTSTRUCT& stPaint);
			friend void __vectorcall frAndernSpaltenBreite(HWND hWndTabelle, unsigned char ucSpalte, long lBreite);
			friend void __vectorcall frEingabeZeile_WM_Char_Return(COEditZeile* pezEingabe);
			friend void __vectorcall frEingabeZeile_WM_Char_Escape(COEditZeile* pezEingabe);
			friend void __vectorcall frEingabeBox_WM_Char_ShiftReturn(COEditBox* pebEingabe);
			friend void __vectorcall frEingabeBox_WM_Char_Escape(COEditBox* pebEingabe);
			friend void __vectorcall frEingabeBild_WM_Char_Return(COBild* pbiEingabe);
			friend void __vectorcall frEingabeBild_WM_Char_Escape(COBild* pbiEingabe);
			friend void __vectorcall frEingabeListe_WM_Char_Return(COListBox* plbEingabe);
			friend void __vectorcall frEingabeListe_WM_Char_Escape(COListBox* plbEingabe);
			friend void __vectorcall frEingabeListe_WM_LButtonUp(COListBox* plbEingabe);
			friend void __vectorcall frEingabeDokument_WM_Char_ShiftReturn(CODokument* pdoEingabe);
			friend void __vectorcall frEingabeDokument_WM_Char_Escape(CODokument* pdoEingabe);
			friend void __vectorcall frEingabeAudio_WM_Char_Return(COMedia* pmmEingabe);
			friend void __vectorcall frEingabeAudio_WM_Char_Escape(COMedia* pmmEingabe);
			friend void __vectorcall frEingabeVideo_WM_Char_Return(COMedia* pmmEingabe);
			friend void __vectorcall frEingabeVideo_WM_Char_Escape(COMedia* pmmEingabe);
			friend void __vectorcall frEingabeDatum_WM_Char_Return(CODatum* pedEingabe);
			friend void __vectorcall frEingabeDatum_WM_Char_Escape(CODatum* pedEingabe);
			friend void __vectorcall frEingabeDatum_WM_LButtonUp(CODatum* pdaEingabe);

		private:
			LOGFONT lfSchrift;
			HMENU hMenu;
			COEditZeile* ezEingabe;
			COEditBox* ebEingabe;
			COListBox* lbEingabe;
			COBild* biEingabe;
			CODokument* doEingabe;
			COAudio* maEingabe;
			COVideo* mvEingabe;
			CODatum* daEingabe;
			COStringA* vasNeuerWert;
			COStringA* vasDateiname;
			VMBLOCK vbBool_true;
			VMBLOCK vbBool_false;
			unsigned long ulTabZeilen_max;
			unsigned long ulTabZeilen_blatt;
			bool bAktiv;
			bool bAnsehen;
			bool bSelectiert;
			long lRand_rechts;
			long lRand_unten;
			long lZeichnen_unten;
			unsigned char ucRechte;
			typedef struct STDateifeld
			{
				unsigned long ulBytes;
				VMBLOCK vbDaten;
			} STDateifeld;
			STDateifeld stDateifeld;
			typedef struct STEingabeGrosse
			{
				long lHohe;
				long lBreite;
			} STEingabeGrosse;
			STEingabeGrosse stGrosse_Listbox;
			STEingabeGrosse stGrosse_Textbox;
			STEingabeGrosse stGrosse_Bild;
			STEingabeGrosse stGrosse_Audio;
			STEingabeGrosse stGrosse_Video;
			STEingabeGrosse stGrosse_Datum;
			void __vectorcall WM_Create(HWND hWnd);
			void __vectorcall WM_Size(LPARAM lParam);
			void __vectorcall WM_KeyDown(WPARAM wParam);
			bool __vectorcall WM_Command(WPARAM wParam);
			void __vectorcall WM_ContexMenu(LPARAM lParam);
			void __vectorcall WM_VScroll(WPARAM wParam);
			void __vectorcall WM_HScroll(WPARAM wParam);
			void __vectorcall WM_LButtonUp(LPARAM lParam);
			void __vectorcall WM_MouseWheel(WPARAM wParam, LPARAM lParam);
			void __vectorcall WM_Paint(void);
			void __vectorcall WM_Paint_Komma4(COStringA* vasFeldWert, unsigned long& ulZeile, unsigned char& ucSpalte);
			void __vectorcall WM_Paint_Komma4_80(COStringA* vasFeldWert, unsigned long& ulZeile, unsigned char& ucSpalte);
			void __vectorcall WM_Paint_Zeit(COStringA* vasFeldWert, unsigned long& ulZeile, unsigned char& ucSpalte);
			void __vectorcall WM_Paint_Datum(COStringA* vasFeldWert, unsigned long& ulZeile, unsigned char& ucSpalte);
			void __vectorcall SetzScrollHorz(SCROLLINFO& stScrollHorz);
			void __vectorcall SetzScrollVert(SCROLLINFO& stScrollVert);
			bool __vectorcall AndernWert(bool bEingabe);
			void __vectorcall AndernWert_Komma4(void);
			void __vectorcall AndernWert_Komma4_80(void);
			void __vectorcall AndernWert_Zeit(void);
			void __vectorcall AndernWert_Datum(void);
			void __vectorcall AndernWert_Datei(void);
			void __vectorcall AndernWert_Zeichnen(unsigned long& ulZeile, unsigned char& ucSpalte);
			void __vectorcall FehlerMeldung(COStringA* vasMeldung);
			void __vectorcall FeldWert(void);
			void __vectorcall FeldWert_String(unsigned char ucFeldtyp);
			void __vectorcall FeldWert_Komma4(void);
			void __vectorcall FeldWert_Komma4_80(void);
			void __vectorcall FeldWert_Zeit(void);
			void __vectorcall FeldWert_Datum(void);
			void __vectorcall SelectZelle(unsigned long ulZeileNeu, unsigned char ucSpalteNeu);
			void __vectorcall QuickSort(long lSpalte, unsigned char ucStatusSpalte, long lIndexLinks, long lIndexRechts);
			inline void __vectorcall QSIndexTausch(long lIndex_1, long lIndex_2);
			char __vectorcall FeldVergleich(unsigned char ucStatusSpalte, STFeld& stFeld_1, STFeld& stFeld_2);
			char __vectorcall Komma4Vergleich(char* pcWert_1, char* pcWert_2);
			char __vectorcall Komma4_80Vergleich(char* pcWert_1, char* pcWert_2);
			char __vectorcall ZeitVergleich(char* pcWert_1, char* pcWert_2);
			void __vectorcall DSLoschen(void);
			void __vectorcall DSEinfugen(void);
			void __vectorcall EinfugenRollback(void);
			void __vectorcall AndernRollback(void);
			void __vectorcall LoschEinfugen(void);
			void __vectorcall LoschAndern(void);
			void __vectorcall DateiOffnen(void);
			void __vectorcall DateiSpeichernUnter(void);
			void __vectorcall DateiSpeichern(void);
			void __vectorcall Eingabe_Position(COElement* pElement, SCROLLINFO& stScrollInfo);
			void __vectorcall Eingabe_Return(void);
			void __vectorcall Eingabe_Escape(void);
			void __vectorcall Eingabe_SichtbarFalse(void);
			unsigned char __vectorcall KommaStellen(unsigned char ucSpalte);

		protected:
			VMEMORY vmTabelle;
			unsigned long ulDSAnzahl;
			unsigned char ucSpaltenanzahl;
			unsigned char ucZusatzspalten;
			unsigned long ulZeileSelect;
			unsigned char ucSpalteSelect;
			unsigned char ucMaxSpalte;
			unsigned char ucKopfhohe;
			unsigned char ucZeilenhohe;
			bool bFeldanderung;
			bool bEinfugen;
			COSchalterGruppe* sgKopf;
			STFeld* vstFelderAlt;
			STFeld** vpTabelle;
			typedef struct STSpalte
			{
				long lBreite;
				unsigned short usFormat;
			} STSpalte;
			STSpalte* vstSpalten;
			unsigned char* vucFeldStatus;
			bool* vbFeldGeandert;
			unsigned char* vucDBTabellenSpalte;
			inline unsigned char __vectorcall FeldTyp(unsigned char ucStatusSpalte);
			void __vectorcall NeueTabelle(void);
			void __vectorcall NeueTabelle_Zeichnen(void);
			STFeld* __vectorcall DSErstellen(void);
			void __vectorcall DSAnfugen(STFeld* pstFelder);
			bool(__vectorcall* pfnServerVerbunden)(COTabBasis*);
			bool(__vectorcall* pfnDSSenden)(COTabBasis*);
			void __vectorcall COTabBasisV(VMEMORY vmSpeicher, const char* pcKlassenName, const char* pcFensterName, unsigned int uiIDElementA, unsigned long ulZeilen_Blatt, unsigned char ucSpalten);

		public:
			VMEMORY __vectorcall COFreiV(void);
			void(__vectorcall* pfnEingabeMaske)(COTabBasis*);
			bool(__vectorcall* pfnPrufenWert)(COTabBasis*, COStringA*);
			void(__vectorcall* pfnEingabeListe)(COTabBasis*);
			void(__vectorcall* pfnAndernWert_Zusatz)(unsigned char, COStringA*);
			void(__vectorcall* pfnLeerFeld)(COTabBasis*);
			void(__vectorcall* pfnDatumZeitFormat)(unsigned char, COStringA*, COStringA*);
			void __vectorcall Spaltenhohe(unsigned char ucKopf, unsigned char ucZeile);
			unsigned long __vectorcall SelectedZeile(void);
			unsigned char __vectorcall SelectedSpalte(void);
			unsigned char __vectorcall Spaltenanzahl(void);
			unsigned long __vectorcall Zeilenanzahl(void);
			void __vectorcall LeerFeld(COStringA* vasText, unsigned long ulZeile, unsigned char ucSpalte);
			void __vectorcall LeerFeld(char* pcText, unsigned long ulZeile, unsigned char ucSpalte);
			COStringA* __vectorcall Prufwert(COStringA* pasWert);
			COFeld* __vectorcall FeldWert(COFeld* pfdFeld, unsigned long ulZeile, unsigned char ucSpalte);
			void __vectorcall Einfugen(void);
			void __vectorcall Loschen(unsigned long ulZeile);
			void __vectorcall Andern(COStringA* vasFeldwert, unsigned long ulZeile, unsigned char ucSpalte);
			void __vectorcall Andern(char* pcFeldwert, unsigned long ulZeile, unsigned char ucSpalte);
			void __vectorcall Andern(COStream* psmDatei, char* pcDateiendung, unsigned long ulZeile, unsigned char ucSpalte);
			void __vectorcall Andern(VMBLOCK vbDaten, unsigned long ulBytes, char* pcDateiendung, unsigned long ulZeile, unsigned char ucSpalte);
			void __vectorcall Senden(void);
			void __vectorcall DruckDaten(COStream* psmDrucken);
			void __vectorcall Lese(COStream* psmDaten);
			void __vectorcall Schreibe(VMBLOCK vbDaten, unsigned long ulBytes);
			void __vectorcall Schreibe(COStream* psmDaten);
			bool __vectorcall DateiLesen(char* pcDateinamen);
			bool __vectorcall DateiSchreiben(char* pcDateinamen);
			void __vectorcall Berechtigungen(unsigned char ucRechteA);
			void __vectorcall BoolWertText(char* pcTrue, char* pcFalse);
			void __vectorcall TabellenKopf_Schriftart(LOGFONT& lfSchriftA);
			void __vectorcall TabellenZeilen_Schriftart(LOGFONT& lfSchriftA);
			void __vectorcall EingabeZeile_Zeichenvorgabe(unsigned char ucZeichenvorgabe);
			void __vectorcall EingabeZeile_Zeichenmaske(const char* pcZeichenmaske);
			void __vectorcall EingabeZeile_SetzZeichen_Max(unsigned long ulZeichen);
			void __vectorcall EingabeListe_Grosse(long lHoheA, long lBreiteA);
			void __vectorcall EingabeListe_Text(char* pcText);
			unsigned char __vectorcall EingabeListe_SelectIndex(void);
			bool __vectorcall EingabeListe_SuchUndSetzEintrag(COStringA* vasEintrag, unsigned char& ucIndex);
			void __vectorcall EingabeText_Grosse(long lHoheA, long lBreiteA);
			void __vectorcall EingabeText_SetzZeichen_Max(unsigned long ulZeichen);
			void __vectorcall EingabeBild_Grosse(long lHoheA, long lBreiteA);
			void __vectorcall EingabeAudio_Grosse(long lHoheA, long lBreiteA);
			void __vectorcall EingabeVideo_Grosse(long lHoheA, long lBreiteA);
			void __vectorcall EingabeDatum_Grosse(long lHoheA, long lBreiteA);
			void __vectorcall EingabeBild_Berechtigungen(unsigned char ucRechte);
			void __vectorcall EingabeDokument_Berechtigungen(unsigned char ucRechte);
			void __vectorcall EingabeAudio_Berechtigungen(unsigned char ucRechte);
			void __vectorcall EingabeVideo_Berechtigungen(unsigned char ucRechte);
			void __vectorcall EingabeDatum_DatumZeitFormat(const char* pcFormat_Datum, const char* pcFormat_Zeit, bool bAnordnung_DatumZeit);
			HANDLE heSelect;

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}