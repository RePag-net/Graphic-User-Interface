/****************************************************************************
	OFeld.cpp
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
		class __declspec(dllexport) COFeld
		{
		private:
			char c8Feld[8];
			unsigned char ucFeldtyp;

		protected:

		public:
			void __vectorcall COFeldV(const unsigned char ucFeldtypA);
			VMEMORY __vectorcall COFreiV(void);
			void __vectorcall operator =(const STFeld& stFeld);
			void __vectorcall operator =(COStringA& asWert); // lass es stehen an anderer Stelle bessere Lesbarkeit 
			void __vectorcall operator =(const char* pcString);
			unsigned long __vectorcall Lange(void);
			VMBLOCK __vectorcall Wert(void);
			COComma4* __vectorcall KOMMA4(COComma4* pk4Zahl);
			COComma4_80* __vectorcall KOMMA4_80(COComma4_80* pk4gZahl);
			COTime* __vectorcall ZEIT(COTime* pzZeit);

		};
		//---------------------------------------------------------------------------------------------------------------------------------------
		__declspec(dllexport) COFeld* __vectorcall COFeldV(const unsigned char ucFeldtyp);
	}
}