/****************************************************************************
	OBild.h
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

namespace RePag
{
	namespace GUI
	{
		//-----------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COBild : public COGrafik
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
		__declspec(dllexport) COBild* __vectorcall COBildV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllexport) COBild* __vectorcall COBildV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//-----------------------------------------------------------------------------------------------------------------------------------------
	}
}
