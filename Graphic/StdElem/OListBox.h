/****************************************************************************
	OListBox.h
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
#include "OTextBox.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace GUI
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COListBox : public COTextBox
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
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COListBox* __vectorcall COListBoxV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllexport) COListBox* __vectorcall COListBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}
