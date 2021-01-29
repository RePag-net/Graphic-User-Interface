/****************************************************************************
	OAudio.h
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
#include "OMedia.h"

namespace RePag
{
	namespace GUI
	{
		//-----------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COAudio : public COMedia
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
		__declspec(dllexport) COAudio* __vectorcall COAudioV(const char* pcFensterName, unsigned int uiIDElement);
		__declspec(dllexport) COAudio* __vectorcall COAudioV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
		//-----------------------------------------------------------------------------------------------------------------------------------------
	}
}