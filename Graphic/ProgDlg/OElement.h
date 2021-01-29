/****************************************************************************
	OElement.h
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
		typedef struct STRGBFarbe
		{
			unsigned char ucRot;
			unsigned char ucBlau;
			unsigned char ucGrun;
		} STRGBFarbe;
		//---------------------------------------------------------------------------------------------------------------------------------------
		class __declspec(dllexport) COElement
		{
		private:
			struct STThread
			{
				HANDLE heElement[2];
				DWORD dwHaupt;
				DWORD dwAktiv;
				unsigned char ucFunktion;
				unsigned char ucWarten;
			};
			STThread stThread;
			HBITMAP hbmBild;
			bool bHintergrund;
			unsigned char ucHintergrundeffekt;
			STRGBFarbe stHintergrundeffektfarbe;
			void __vectorcall Hintergrund(RECT& rcZeichnen);
			void __vectorcall Hintergrund_Effekt(RECT* prcZeichnen);
			void __vectorcall Effekt_Beleuchtung_Horizontal(void);
			void __vectorcall Effekt_Beleuchtung_Vertical(void);
			void __vectorcall Effekt_Rand_Horizontal(void);
			void __vectorcall Effekt_Rand_Vertical(void);

		protected:
			VMEMORY vmSpeicher;
			HWND hWndElement;
			HANDLE htEffekt_Timer;
			long lHohe;
			long lBreite;
			POINT ptPosition;
			STRGBFarbe stHintergrundfarbe;
			RECT rcEffektrand;
			unsigned char ucRahmenbreite;
			void __vectorcall WM_Size_Element(HWND hWnd, LPARAM lParam);
			void __vectorcall WM_Paint_Hintergrund(PAINTSTRUCT& stPaint);
			inline void __vectorcall ThreadSicher_Anfang(void);
			inline void __vectorcall ThreadSicher_Ende(void);
			void __vectorcall COElementV(VMEMORY vmSpeicherA);

		public:
			VMEMORY __vectorcall COFreiV(void);
			void(__vectorcall* pfnWM_Size)(COElement*, WPARAM, LPARAM);
			HWND __vectorcall HWND_Element(void);
			HWND __vectorcall HWND_Haupt(void);
			long& __vectorcall Breite(long& lBreiteA);
			long& __vectorcall Hohe(long& lHoheA);
			long& __vectorcall Pos_X(long& lPos_x);
			long& __vectorcall Pos_Y(long& lPos_y);
			POINT& __vectorcall Position(POINT& ptPositionA);
			RECT& __vectorcall Fenster(RECT& rcFenster);
			void __vectorcall NeueFensterPosition(long lPos_x, long lPos_y);
			void __vectorcall NeueFensterPosition(POINT& ptPositionA);
			void __vectorcall NeuesFenster(long lHoheA, long lBreiteA, long lPos_x, long lPos_y);
			void __vectorcall NeueFensterGrosse(long lHoheA, long lBreiteA);
			void __vectorcall NeueFensterHohe(long lHoheA);
			void __vectorcall NeueFensterBreite(long lBreiteA);
			void __vectorcall AndernFensterGrosse(long lHoheA, long lBreiteA);
			void __vectorcall AndernFensterPosition(long lPos_x, long lPos_y);
			void __vectorcall Hintergrundfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall Hintergrundeffekt(unsigned char ucEffekt);
			void __vectorcall Hintergrundeffektfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
			void __vectorcall Hintergrundeffektrand(long lOben, long lUnten, long lLinks, long lRechts);
			void __vectorcall Effekt_Timer(bool bStarten, unsigned long ulInterval_msek, void CALLBACK fnEffekt_Timer(COElement*, bool bTimerOrWaitFired));
			void __vectorcall UpdateFenster(RECT* prcFenster, bool bUpdateHintergrund, bool bUpdateHintergrundeffekt);
			void __vectorcall SetzHintergrund(bool bMitHintergrund);
			void __vectorcall SetzVerfugbar(bool bVerfugbar);
			bool __vectorcall Verfugbar(void);
			void __vectorcall Loschen(void);
		};
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}