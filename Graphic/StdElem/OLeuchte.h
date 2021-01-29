/****************************************************************************
	OLeuchte.h
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
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace GUI
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COLeuchte : public COGrafik
		{
			friend LRESULT CALLBACK WndProc_Leuchte(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		private:
			STRGBFarbe stLeuchtfarbe;
			unsigned char ucForm;
			unsigned char ucRand;
			void __vectorcall WM_Paint(void);

		protected:

		public:
			void __vectorcall COLeuchteV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);
			void __vectorcall Leuchtfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall Form(unsigned char ucFormA);
			void __vectorcall Rand(unsigned char ucRandA);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COLeuchte* __vectorcall COLeuchteV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllexport) COLeuchte* __vectorcall COLeuchteV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}
