// up Fileversion 3.2.2.0
#ifndef DokumentH
#define DokumentH
#include "StdElem.h"

namespace RePag
{
	namespace GUI
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllimport) CODokument : public COKnopf
		{
			friend LRESULT CALLBACK WndProc_Dokument(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		private:
			HMENU hMenu;
			COStringA* vasDateiname;
			COStream* vsmDatei;
			COStringA* vasTypEndung;
			bool bGeandert;
			unsigned char ucRechte;
			void __vectorcall WM_Char(WPARAM wParam);
			bool __vectorcall WM_Command(WPARAM wParam);
			void __vectorcall WM_ContexMenu(LPARAM lParam);
			void __vectorcall WM_LButtonUp(WPARAM wParam, LPARAM lParam);
			void __vectorcall DateiLaden(void);
			void __vectorcall DateiOffnen(void);
			void __vectorcall DateiSpeichernUnter(void);
			void __vectorcall DateiSpeichern(void);
			void __vectorcall DateiEntfernen(void);
			void __vectorcall Schreibe(VMBLOCK vbDokument, unsigned long& ulBytes);

		protected:

		public:
			void __vectorcall CODokumentV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);
			VMEMORY __vectorcall COFreiV(void);
			void(__vectorcall* pfnWM_Char_ShiftReturn)(CODokument*);
			void(__vectorcall* pfnWM_Char_Escape)(CODokument*);
			void(__vectorcall* pfnWM_KillFocus)(CODokument*);
			bool(__vectorcall* pfnWM_Command)(CODokument*, WPARAM);
			void __vectorcall Lese(VMBLOCK& vbDokument, unsigned long& ulBytes, COStringA* pasDateiendung);
			void __vectorcall Schreibe(VMBLOCK vbDokument, unsigned long ulBytes, VMBLOCK vbDateiendung, unsigned char ucDateiendung_Lange);
			bool __vectorcall DateiLesen(char* pcDateinamen);
			bool __vectorcall DateiSchreiben(char* pcDateinamen);
			void __vectorcall SetzVerfugbar(bool bVerfugbar);
			bool __vectorcall Geandert(void);
			void __vectorcall Berechtigungen(unsigned char ucRechteA);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllimport) CODokument* __vectorcall CODokumentV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllimport) CODokument* __vectorcall CODokumentV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
#endif