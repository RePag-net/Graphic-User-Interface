// up Fileversion 3.2.3.0
#ifndef MediaH
#define MediaH
#include "StdElem.h"

namespace RePag
{
	namespace GUI
	{
		//-----------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllimport) COMedia : public COGrafik
		{
			friend void CALLBACK frBalken_WM_LButtonDown(COBalken* pBalken, WPARAM wParam, LPARAM lParam);
			friend void CALLBACK frBalken_WM_MouseMove(COBalken* pBalken, WPARAM wParam, LPARAM lParam);

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
		class __declspec(dllimport) COVideo : public COMedia
		{
			friend LRESULT CALLBACK WndProc_Video(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		private:
			bool bBildformatErhalten;
			void __vectorcall WM_Paint(void);
			bool __vectorcall WM_Command(WPARAM wParam);
			void __vectorcall DateiOffnen(void);
			void __vectorcall DateiSpeichernUnter(void);

		protected:

		public:
			void __vectorcall COVideoV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);

		};
		//-----------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllimport) COVideo* __vectorcall COVideoV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllimport) COVideo* __vectorcall COVideoV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//-----------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllimport) COAudio : public COMedia
		{
			friend LRESULT CALLBACK WndProc_Audio(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		private:
			void __vectorcall WM_Paint(void);
			bool __vectorcall WM_Command(WPARAM wParam);
			void __vectorcall DateiOffnen(void);
			void __vectorcall DateiSpeichernUnter(void);

		protected:

		public:
			void __vectorcall COAudioV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);

		};
		//-----------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllimport) COAudio* __vectorcall COAudioV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllimport) COAudio* __vectorcall COAudioV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//-----------------------------------------------------------------------------------------------------------------------------------------
	}
}
#endif