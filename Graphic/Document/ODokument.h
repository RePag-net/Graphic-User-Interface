/****************************************************************************
	ODokument.h
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
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) CODokument : public COKnopf
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
		__declspec(dllexport) CODokument* __vectorcall CODokumentV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllexport) CODokument* __vectorcall CODokumentV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}