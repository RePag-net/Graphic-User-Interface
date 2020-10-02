/****************************************************************************
	OBildBox.h
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
#include "OBild.h"

namespace RePag
{
	namespace GUI
	{
		//-----------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COBildBox : public COGrafik
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
		__declspec(dllexport) COBildBox* __vectorcall COBildBoxV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllexport) COBildBox* __vectorcall COBildBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//-----------------------------------------------------------------------------------------------------------------------------------------
	}
}
