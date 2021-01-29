/****************************************************************************
	OSchalter.h
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
#include "OKnopf.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace GUI
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COSchalter : public COKnopf
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
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COSchalter* __vectorcall COSchalterV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllexport) COSchalter* __vectorcall COSchalterV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}
