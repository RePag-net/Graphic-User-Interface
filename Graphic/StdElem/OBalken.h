/****************************************************************************
  OBalken.h
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
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace GUI
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COBalken : public COGrafik
		{
			friend LRESULT CALLBACK WndProc_Balken(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

		private:
			unsigned long ulPosition_max;
			unsigned long ulPosition_min;
			unsigned long ulPosition;
			double dSchritt;
			STRGBFarbe stFullfarbe;
			void __vectorcall WM_Size(LPARAM lParam);
			void __vectorcall WM_LButtonDown(WPARAM wParam, LPARAM lParam);
			void __vectorcall WM_MouseMove(WPARAM wParam, LPARAM lParam);
			void __vectorcall WM_Paint(void);

		protected:

		public:
			void __vectorcall COBalkenV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);
			void __vectorcall SetzPosition_Max(unsigned long ulPosition);
			void __vectorcall SetzPosition_Min(unsigned long ulPosition);
			void __vectorcall SetzPosition(unsigned long ulPositionA);
			void __vectorcall Fullfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void(__vectorcall* pfnWM_LButtonDown)(COBalken*, WPARAM, LPARAM);
			void(__vectorcall* pfnWM_MouseMove)(COBalken*, WPARAM, LPARAM);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COBalken* __vectorcall COBalkenV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllexport) COBalken* __vectorcall COBalkenV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}