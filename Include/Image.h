// up Fileversion 3.2.3.0
#ifndef BildH
#define BildH
#include "ProgDlg.h"
#include "gdiplus.h"
using namespace Gdiplus;

namespace RePag
{
	namespace GUI
	{
		//-----------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllimport) COBild : public COGrafik
		{
			friend LRESULT CALLBACK WndProc_Bild(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		private:
			HMENU hMenu;
			Image* imBild;
			bool bBildformatErhalten;
			LOGFONT lfSchrift;
			STRGBFarbe stSchriftfarbe;
			COStringA* vasInfotext;
			bool bGeandert;
			unsigned char ucRechte;
			void __vectorcall WM_Create(HWND hWnd);
			void __vectorcall WM_Size(LPARAM lParam);
			void __vectorcall WM_KeyDown(WPARAM wParam);
			bool __vectorcall WM_Command(WPARAM wParam);
			void __vectorcall WM_ContexMenu(LPARAM lParam);
			void __vectorcall WM_Paint(void);
			bool __vectorcall BildKonvertierenZu(GUID guidImage, VMBLOCK& vbBild, unsigned long& ulGrosse);
			void __vectorcall DateiOffnen(void);
			void __vectorcall DateiSpeichernUnter(void);
			void __vectorcall DateiSpeichern(char* pcDateiname);
			void __vectorcall DateiEntfernen(void);

		protected:

		public:
			void __vectorcall COBildV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);
			VMEMORY __vectorcall COFreiV(void);
			void(__vectorcall* pfnWM_Paint)(COBild*, PAINTSTRUCT&);
			void(__vectorcall* pfnWM_Char_Return)(COBild*);
			void(__vectorcall* pfnWM_Char_Escape)(COBild*);
			void(__vectorcall* pfnWM_KillFocus)(COBild*);
			bool(__vectorcall* pfnWM_Command)(COBild*, WPARAM);
			void __vectorcall SetzVerfugbar(bool bVerfugbar);
			void __vectorcall Infotext(char* pcText);
			void __vectorcall Schriftart(LOGFONT& lfSchriftA);
			void __vectorcall Schriftfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			bool __vectorcall Lese(VMBLOCK& vbBild, unsigned long& ulBytes, COStringA* pasDateiendung);
			void __vectorcall Schreibe(VMBLOCK vbBild, unsigned long ulBytes);
			bool __vectorcall DateiLesen(char* pcDateinamen);
			bool __vectorcall DateiSchreiben(char* pcDateinamen);
			bool __vectorcall Geandert(void);
			void __vectorcall Berechtigungen(unsigned char ucRechteA);

		};
		//-----------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllimport) COBild* __vectorcall COBildV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllimport) COBild* __vectorcall COBildV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//-----------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllimport) COBildBox : public COGrafik
		{
			friend LRESULT CALLBACK WndProc_BildBox(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		private:
			COBild* biBild;
			long lBildhohe;
			long lBildbreite;
			long lRand_rechts;
			long lRand_unten;
			void __vectorcall WM_Create(HWND hWnd);
			void __vectorcall WM_Size(LPARAM lParam);
			void __vectorcall WM_VScroll(WPARAM wParam);
			void __vectorcall WM_HScroll(WPARAM wParam);
			void __vectorcall WM_MouseWheel(WPARAM wParam, LPARAM lParam);
			void __vectorcall WM_Paint(void);
			void __vectorcall SetzScrollHorz(SCROLLINFO& stScrollHorz);
			void __vectorcall SetzScrollVert(SCROLLINFO& stScrollVert);
			int __vectorcall MMzuPixel(int iIndex, int iMillimeter);

		protected:

		public:
			void __vectorcall COBildBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);
			void __vectorcall ErstellBildInBox(long lHoheA, long lBreiteA, bool bMillimeter);
			void __vectorcall Bild_Hintergrundfarbe(BYTE ucRot, BYTE ucGrun, BYTE ucBlau);
			void __vectorcall Bild_AndernGrosse(int iHoheA, int iBreiteA, bool bMillimeter);
			SIZE& __vectorcall Bild_Grosse_Aktuell(SIZE& stGrosse);
			SIZE& __vectorcall Bild_Grosse_Original(SIZE& stGrosse);
			void __vectorcall Bild_pfnWM_Paint(void(__vectorcall* pfnWM_Paint_Bild)(COBild*, PAINTSTRUCT&));

		};
		//-----------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllimport) COBildBox* __vectorcall COBildBoxV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllimport) COBildBox* __vectorcall COBildBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//-----------------------------------------------------------------------------------------------------------------------------------------
	}
}
#endif