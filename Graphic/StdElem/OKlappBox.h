/****************************************************************************
	OKlappBox.h
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
#include "OListBox.h"
#include "OKnopf.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace GUI
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COKlappBox : public COListBox
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
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COKlappBox* __vectorcall COKlappBoxV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllexport) COKlappBox* __vectorcall COKlappBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}
