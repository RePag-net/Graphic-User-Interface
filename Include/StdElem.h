// up Fileversion 3.2.3.1
#ifndef StdElemH
#define StdElemH
#ifndef HPROGDLG
#include "ProgDlg.h"
#endif
//---------------------------------------------------------------------------------------------------------------------------------------
/*
Zeichenmaske:
  A -> nur Grossbuchstaben
  a -> nur Kleinbuchstaben
  B -> alle Buchstaben und Sonderzeichen
  b -> nur Sonderzeichen
	X -> nur Hex-Zahlen
  Z -> alle Zeichen
  9 -> nur Ziffern
  8 -> Ziffern, Vorzeichen, Leerzeichen
  '' -> Selbstdarstellung

Datumformat:
  d    -> Tag ohne führende Null
  dd   -> Tag mit führende Null
  ddd  -> Wochentag zwei Buchstaben
  dddd -> Wochentag ganzes Wort
  M    -> Monat ohne führende Null
  MM   -> Monat mit führende Null
  MMM  -> Monat drei Buchstaben
  MMMM -> Monat ganzes Wort
	y    -> Jahr 2-stellig ohne führende Null
	yy   -> Jahr 2-stellig mit führende Null
	yyyy -> Jahr 4-stellig
  ''   -> Selbstdarstellung

Zeitformat:
  h    -> 12-Stunden-Uhr ohne führende Null
  hh   -> 12-Stunden-Uhr mit führende Null
  H    -> 24-Stunden-Uhr ohne führende Null
  HH   -> 24-Stunden-Uhr mit führende Null
  m    -> Minuten ohne führende Null
  mm   -> Minuten mit führende Null
  s    -> Sekunden ohne führende Null
  ss   -> Sekunden mit führende Null
	t    -> Zeitmarke A oder P 
	tt   -> Zeitmarke AM oder PM
  ''   -> Selbstdarstellung
*/
namespace RePag
{
	namespace GUI
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		#define TXA_OBEN 0
		#define TXA_LINKS 1
		#define TXA_RECHTS 2
		#define TXA_UNTEN 4
		#define TXA_MITTEVERTICAL 8
		#define TXA_MITTEHORIZONTAL 16

		#define ZV_KEINE 0
		#define ZV_BUCHSTABEN 1
		#define ZV_ZIFFERN 2
		#define ZV_SONDERBUCHSTABEN 4
		#define ZV_SONDERZEICHEN 8
		#define ZV_UBERLANGE 16
		#define ZV_SICHTBAR 32
		#define ZV_ALLE 15

		#define SGM_KEINWECHSEL 0
		#define SGM_EINAUS 1
		#define SGM_EINAUSINAKTIV 2

		#define BI_PIXEL 0
		#define BI_MILLIMETER 1

		#define LEF_RUND 0
		#define LEF_ECKIG 1
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllimport) COTextZeile : public COGrafik
		{
			friend LRESULT CALLBACK WndProc_TextZeile(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam);

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
		__declspec(dllimport) COTextZeile* __vectorcall COTextZeileV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllimport) COTextZeile* __vectorcall COTextZeileV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllimport) COEditZeile : public COTextZeile
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
		__declspec(dllimport) COEditZeile* __vectorcall COEditZeileV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllimport) COEditZeile* __vectorcall COEditZeileV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllimport) COTextBox : public COEditZeile
		{
			friend LRESULT CALLBACK WndProc_TextBox(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		private:
			void __vectorcall Text_Create(HWND hWnd);

		protected:
			COList* vliText;
			long lBreitesteZeile;
			long lRand_rechts;
			long lRand_unten;
			void __vectorcall WM_Create(HWND hWnd);
			void __vectorcall WM_Size(LPARAM lParam);
			void __vectorcall WM_VScroll(WPARAM wParam);
			void __vectorcall WM_HScroll(WPARAM wParam);
			void __vectorcall WM_LButtonDown(void);
			void __vectorcall WM_MouseWheel(WPARAM wParam, LPARAM lParam);
			void __vectorcall WM_Paint(void);
			void __vectorcall SetzScrollHorz(SCROLLINFO& stScrollHorz);
			void __vectorcall SetzScrollVert(SCROLLINFO& stScrollVert);
			void __vectorcall DeSelect(void);
			void __vectorcall COTextBoxV(VMEMORY vmSpeicher, const char* pcKlassenName, const char* pcFensterName, unsigned int uiIDElementA);

		public:
			void __vectorcall COTextBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);
			VMEMORY __vectorcall COFreiV(void);
			void __vectorcall Text(char* pcText);
			void __vectorcall Text_NeueZeile(char* pcText);
			unsigned long __vectorcall Zeilenanzahl(void);
			long __vectorcall BreitesteZeile(void);
			void __vectorcall Scroll_Anfang(void);
			void __vectorcall Scroll_Ende(void);
			void __vectorcall Scroll_Zeile(bool bAbwarts);

		};
		__declspec(dllimport) COTextBox* __vectorcall COTextBoxV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllimport) COTextBox* __vectorcall COTextBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllimport) COEditBox : public COTextBox
		{
			friend LRESULT CALLBACK WndProc_EditBox(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		private:
			long lZeile;
			void* pvZeile;
			void __vectorcall WM_SetFocus(void);
			void __vectorcall WM_LButtonDown(LPARAM lParam);
			void __vectorcall WM_KeyDown(WPARAM wParam, LPARAM lParam);
			void __vectorcall WM_Char(WPARAM wParam);
			bool __vectorcall WM_Command(WPARAM wParam);
			void __vectorcall WM_ContexMenu(LPARAM lParam);
			void __vectorcall WM_MouseMove(WPARAM wParam, LPARAM lParam);
			void __vectorcall BreitesteZeile(HDC hdc);
			void __vectorcall Select_Loschen(HDC hdc);

		protected:

		public:
			void __vectorcall COEditBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);
			void(__vectorcall* pfnWM_Char_ShiftReturn)(COEditBox*);
			void(__vectorcall* pfnWM_Char_Escape)(COEditBox*);
			void(__vectorcall* pfnWM_KillFocus)(COEditBox*);
			bool(__vectorcall* pfnWM_Command)(COEditBox*, WPARAM);
			COStringA* __vectorcall Inhalt(COStringA* vasInhaltA);

		};
		__declspec(dllimport) COEditBox* __vectorcall COEditBoxV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllimport) COEditBox* __vectorcall COEditBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllimport) COListBox : public COTextBox
		{
			friend LRESULT CALLBACK WndProc_ListBox(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		private:
			unsigned char ucIndex;
			void __vectorcall SelectZeile(SCROLLINFO& stScrollVert);

		protected:
			void __vectorcall WM_LButtonUp(LPARAM lParam);
			void __vectorcall WM_KeyDown(WPARAM wParam);
			void __vectorcall WM_Char(WPARAM wParam);
			void __vectorcall COListBoxV(VMEMORY vmSpeicher, const char* pcKlassenName, const char* pcFensterName, unsigned int uiIDElementA);

		public:
			void __vectorcall COListBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);
			VMEMORY __vectorcall COFreiV(void);
			void(__vectorcall* pfnWM_LButtonDown)(COListBox*);
			void(__vectorcall* pfnWM_LButtonUp)(COListBox*);
			void(__vectorcall* pfnWM_Char_Return)(COListBox*);
			void(__vectorcall* pfnWM_Char_Escape)(COListBox*);
			void(__vectorcall* pfnWM_KillFocus)(COListBox*);
			bool(__vectorcall* pfnWM_Command)(COListBox*, WPARAM);
			unsigned char __vectorcall SelectIndex(void);
			COStringA* __vectorcall SelectEintrag(COStringA* vasEintrag);
			bool __vectorcall SetzSelectIndex(unsigned char ucIndexA);
			COStringA* __vectorcall SetzUndSuchEintrag(unsigned char ucIndexA, COStringA* vasEintrag);
			bool __vectorcall SuchEintrag(COStringA* vasEintrag, unsigned char& ucIndexA);
			bool __vectorcall SuchUndSetzEintrag(COStringA* vasEintrag, unsigned char& ucIndexA);
			unsigned long __vectorcall AnzahlEintrage(void);
			void __vectorcall DeSelectEintrag(void);

		};
		__declspec(dllimport) COListBox* __vectorcall COListBoxV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllimport) COListBox* __vectorcall COListBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllimport) COPasswort : public COEditZeile
		{
			friend LRESULT CALLBACK WndProc_Passwort(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		private:
			COStringA* vasPasswort;
			void __vectorcall WM_SetFocus(void);
			void __vectorcall WM_Char(WPARAM wParam);
			void __vectorcall WM_LButtonDown(LPARAM lParam);
			void __vectorcall WM_Paint(void);

		protected:

		public:
			void __vectorcall COPasswortV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);
			VMEMORY __vectorcall COFreiV(void);
			void __vectorcall Text(char* pcText);

		};
		__declspec(dllimport) COPasswort* __vectorcall COPasswortV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllimport) COPasswort* __vectorcall COPasswortV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllimport) COKnopf : public COTextZeile
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
		__declspec(dllimport) COKnopf* __vectorcall COKnopfV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllimport) COKnopf* __vectorcall COKnopfV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllimport) COKlappBox : public COListBox
		{
			friend LRESULT CALLBACK WndProc_KlappBox(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);
			friend LRESULT CALLBACK WndProc_EintragZeile(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		private:
			class COEintragZeile : public COKnopf
			{
				friend LRESULT CALLBACK WndProc_EintragZeile(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

			private:
				COKlappBox* pKlappBox;
				void __vectorcall WM_LButtonUp(WPARAM wParam, LPARAM lParam);
				void __vectorcall WM_Paint(void);

			protected:

			public:
				void __vectorcall COEintragZeileV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA, COKlappBox* pKlappBoxA);

			}; COEintragZeile* ezEintrag;
			unsigned char ucZeilenhohe_EintragZeile;
			void __vectorcall WM_Create_KlappBox(HWND hWnd);
			void __vectorcall WM_LButtonUp_KlappBox(void);

		protected:

		public:
			void __vectorcall COKlappBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);
			VMEMORY __vectorcall COFreiV(void);
			long& __vectorcall Hohe(long& lHoheA);
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
			void __vectorcall Zeilenhohe_EintragZeile(unsigned char ucHohe);
			void __vectorcall Schriftfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall Hintergrundfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall Sperren(void);
			void __vectorcall Freigeben(void);
			void __vectorcall Text(char* pcText);
			bool __vectorcall SetzSelectIndex(unsigned char ucIndex);
			bool __vectorcall SuchUndSetzEintrag(COStringA* vasEintrag, unsigned char& ucIndexA);

		};
		__declspec(dllimport) COKlappBox* __vectorcall COKlappBoxV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllimport) COKlappBox* __vectorcall COKlappBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllimport) COSchalter : public COKnopf
		{
			friend LRESULT CALLBACK WndProc_Schalter(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		private:
			bool bEingeschaltet;
			void __vectorcall WM_LButtonUp(WPARAM wParam, LPARAM lParam);

		protected:
			COStringA* vasBeschriftung_ein;
			COStringA* vasBeschriftung_aus;
			void __vectorcall Schalten(bool bEinschalten);
			void __vectorcall COSchalterV(VMEMORY vmSpeicher, const char* pcKlassenName, const char* pcFensterName, unsigned int uiIDElementA);

		public:
			void __vectorcall COSchalterV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);
			VMEMORY __vectorcall COFreiV(void);
			void(__vectorcall* pfnWM_LButtonDown)(COSchalter*);
			void(__vectorcall* pfnWM_LButtonUp)(COSchalter*);
			bool __vectorcall Eingeschaltet(void);
			void __vectorcall SetzSchalter(bool bEinschalten);
			void __vectorcall Beschriftung(const char* pcBeschriftung);
			void __vectorcall Beschriftung_Ein(const char* pcBeschriftung);
			void __vectorcall Beschriftung_Aus(const char* pcBeschriftung);

		};
		__declspec(dllimport) COSchalter* __vectorcall COSchalterV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllimport) COSchalter* __vectorcall COSchalterV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllimport) COSchalterGruppe : public COGrafik
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
		__declspec(dllimport) COSchalterGruppe* __vectorcall COSchalterGruppeV(const char* pcFensterName, unsigned int uiIDElement, unsigned char ucSchalteranzahl, unsigned char ucWechselmodus);
		__declspec(dllimport) COSchalterGruppe* __vectorcall COSchalterGruppeV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement, unsigned char ucSchalteranzahl, unsigned char ucWechselmodus);
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllimport) COBalken : public COGrafik
		{
			friend LRESULT CALLBACK WndProc_Balken(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		private:
			unsigned long ulPosition_max;
			unsigned long ulPosition_min;
			unsigned long ulPosition;
			double dSchritt;
			STRGBFarbe stFullfarbe;
			void __vectorcall WM_Size(LPARAM lParam);
			void __vectorcall WM_LButtonDown(WPARAM wParam, LPARAM lParam);
			void __vectorcall WM_MouseMove(WPARAM wParam, LPARAM lParam);
			void __vectorcall WM_Paint(void);

		protected:

		public:
			void __vectorcall COBalkenV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);
			void __vectorcall SetzPosition_Max(unsigned long ulPosition);
			void __vectorcall SetzPosition_Min(unsigned long ulPosition);
			void __vectorcall SetzPosition(unsigned long ulPositionA);
			void __vectorcall Fullfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void(__vectorcall* pfnWM_LButtonDown)(COBalken*, WPARAM, LPARAM);
			void(__vectorcall* pfnWM_MouseMove)(COBalken*, WPARAM, LPARAM);

		};
		__declspec(dllimport) COBalken* __vectorcall COBalkenV(const char* pcName, unsigned int uiIDElement);
		__declspec(dllimport) COBalken* __vectorcall COBalkenV(VMEMORY vmSpeicher, const char* pcName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllimport) CODatum : public COGrafik
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
		__declspec(dllimport) CODatum* __vectorcall CODatumV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllimport) CODatum* __vectorcall CODatumV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllimport) COLeuchte : public COGrafik
		{
			friend LRESULT CALLBACK WndProc_Leuchte(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		private:
			STRGBFarbe stLeuchtfarbe;
			unsigned char ucForm;
			unsigned char ucRand;
			void __vectorcall WM_Paint(void);

		protected:

		public:
			void __vectorcall COLeuchteV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);
			void __vectorcall Leuchtfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall Form(unsigned char ucFormA);
			void __vectorcall Rand(unsigned char ucRandA);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllimport) COLeuchte* __vectorcall COLeuchteV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllimport) COLeuchte* __vectorcall COLeuchteV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}
#endif