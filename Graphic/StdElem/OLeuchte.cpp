/****************************************************************************
  OLeuchte.cpp
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

#include "HStdElem.h"
#include "OLeuchte.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COLeuchte* __vectorcall RePag::GUI::COLeuchteV(const char* pcFensterName, unsigned int uiIDElement)
{
 COLeuchte* vLeuchte = (COLeuchte*)VMBlock(VMDialog(), sizeof(COLeuchte));
 vLeuchte->COLeuchteV(VMDialog(), pcFensterName, uiIDElement);
 return vLeuchte;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COLeuchte* __vectorcall RePag::GUI::COLeuchteV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement)
{
 COLeuchte* vLeuchte = (COLeuchte*)VMBlock(vmSpeicher, sizeof(COLeuchte));
 vLeuchte->COLeuchteV(vmSpeicher, pcFensterName, uiIDElement);
 return vLeuchte;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_Leuchte(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
	COLeuchte* pLeuchte;
	switch(uiMessage){
		case WM_PAINT     : ((COLeuchte*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_Paint();
												return NULL;
		case WM_NCDESTROY : pLeuchte = (COLeuchte*)GetWindowLongPtr(hWnd, GWL_USERDATA);
												if(pLeuchte->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pLeuchte->htEffekt_Timer, INVALID_HANDLE_VALUE);
												VMFreiV(pLeuchte);
												return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COLeuchte::COLeuchteV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA)
{
 COGrafikV(vmSpeicher, pcRePag_Leuchte, pcFensterName, uiIDElementA);

 ucForm = LEF_RUND;
 ucRand = 0;
 stLeuchtfarbe.ucRot = stLeuchtfarbe.ucGrun = stLeuchtfarbe.ucBlau = 255;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COLeuchte::WM_Paint(void)
{
 ThreadSicher_Anfang();
 PAINTSTRUCT stPaint; 
 BeginPaint(hWndElement, &stPaint);
 WM_Paint_Hintergrund(stPaint);

 HBRUSH hBrush = CreateSolidBrush(RGB(stLeuchtfarbe.ucRot, stLeuchtfarbe.ucGrun, stLeuchtfarbe.ucBlau));
 HPEN hPen = CreatePen(PS_SOLID, ucRand, RGB(stHintergrundfarbe.ucRot, stHintergrundfarbe.ucGrun, stHintergrundfarbe.ucBlau));
 SelectObject(stPaint.hdc, hPen); SelectObject(stPaint.hdc, hBrush);
 if(ucForm == LEF_RUND) Ellipse(stPaint.hdc, 0, 0, lBreite, lHohe);
 else Rectangle(stPaint.hdc, 0, 0, lBreite, lHohe);

 DeleteObject(hBrush); DeleteObject(hPen);
 EndPaint(hWndElement, &stPaint);
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COLeuchte::Leuchtfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
 ThreadSicher_Anfang();
 stLeuchtfarbe.ucRot = ucRot; stLeuchtfarbe.ucGrun = ucGrun; stLeuchtfarbe.ucBlau = ucBlau;
 UpdateFenster(nullptr, false, false);
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COLeuchte::Form(unsigned char ucFormA)
{
 ThreadSicher_Anfang(); 
 ucForm = ucFormA;
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COLeuchte::Rand(unsigned char ucRandA)
{
 ThreadSicher_Anfang(); 
 ucRand = ucRandA;
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------