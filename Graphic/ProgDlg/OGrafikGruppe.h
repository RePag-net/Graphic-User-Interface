/****************************************************************************
	OGrafikGruppe.h
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
#include "OGrafik.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace GUI
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COGrafikGruppe : public COGrafik
		{
			friend LRESULT CALLBACK WndProc_GrafikGruppe(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);
			friend DWORD WINAPI thWM_Command_GrafikGruppe(void* pvParam);

		private:
			void __vectorcall WM_Paint(void);
			void __vectorcall WM_Command_GrafikGruppe(unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		protected:

		public:
			void __vectorcall COGrafikGruppeV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);
			void __vectorcall ErstellFensterInRahmen(COGrafik* pGrafik);
			void __vectorcall ErstellFensterInGruppe(COGrafik* pGrafik, long lHoheA, long lBreiteA, long lPos_x, long lPos_y);
			void(__vectorcall* pfnWM_Paint)(COGrafikGruppe*, PAINTSTRUCT&);
			void(__vectorcall* pfnWM_Command)(HWND, unsigned int, WPARAM, LPARAM);
			void __vectorcall SetzSichtbar(bool bSichtbar);
		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COGrafikGruppe* __vectorcall COGrafikGruppeV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllexport) COGrafikGruppe* __vectorcall COGrafikGruppeV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}
