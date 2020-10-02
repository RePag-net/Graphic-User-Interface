/****************************************************************************
	OBalken.cpp
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
#include "OBalken.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COBalken* __vectorcall RePag::GUI::COBalkenV(const char* pcFensterName, unsigned int uiIDElement)
{
 COBalken* vBalken = (COBalken*)VMBlock(VMDialog(), sizeof(COBalken));
 vBalken->COBalkenV(VMDialog(), pcFensterName, uiIDElement);
 return vBalken;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COBalken* __vectorcall RePag::GUI::COBalkenV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement)
{
 COBalken* vBalken = (COBalken*)VMBlock(vmSpeicher, sizeof(COBalken));
 vBalken->COBalkenV(vmSpeicher, pcFensterName, uiIDElement);
 return vBalken;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_Balken(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
	COBalken* pBalken;
	switch(uiMessage){
		case WM_SIZE        : pBalken = (COBalken*)GetWindowLongPtr(hWnd, GWL_USERDATA);
													if(pBalken) pBalken->WM_Size(lParam);
													else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													return NULL;
		case WM_LBUTTONDOWN : ((COBalken*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_LButtonDown(wParam, lParam);
													return NULL;
		case WM_LBUTTONUP   : PostMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWnd, GWLP_ID), wParam), lParam);
													return NULL;
		case WM_MOUSEMOVE   : ((COBalken*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_MouseMove(wParam, lParam);
													return NULL;
		case WM_PAINT       : ((COBalken*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_Paint();
													return NULL;
		case WM_NCDESTROY   : pBalken = (COBalken*)GetWindowLongPtr(hWnd, GWL_USERDATA);
													if(pBalken->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pBalken->htEffekt_Timer, INVALID_HANDLE_VALUE);
													VMFreiV(pBalken);
													return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBalken::COBalkenV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA)
{
 COGrafikV(vmSpeicher, pcRePag_Balken, pcFensterName, uiIDElementA);

 stFullfarbe.ucRot = 140; stFullfarbe.ucGrun = 85; stFullfarbe.ucBlau = 0;
 dSchritt = ulPosition_max = ulPosition_min = ulPosition = 0;

 pfnWM_LButtonDown = nullptr;
 pfnWM_MouseMove = nullptr;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBalken::WM_Paint(void)
{
 ThreadSicher_Anfang();
 PAINTSTRUCT stPaint; 
 BeginPaint(hWndElement, &stPaint);
 WM_Paint_Hintergrund(stPaint);
 HBRUSH hBrush = CreateSolidBrush(RGB(stFullfarbe.ucRot, stFullfarbe.ucGrun, stFullfarbe.ucBlau));
 RECT rcKnopf; rcKnopf.right = ulPosition; rcKnopf.left = 0; rcKnopf.top = 0; rcKnopf.bottom = lHohe;
 FillRect(stPaint.hdc, &rcKnopf, hBrush);
 DeleteObject(hBrush);
 EndPaint(hWndElement, &stPaint);
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBalken::WM_Size(LPARAM lParam)
{
 ThreadSicher_Anfang();
 WM_Size_Element(hWndElement, lParam);

 if(ulPosition_max > ulPosition_min){ dSchritt = (double)lBreite / (double)(ulPosition_max - ulPosition_min); }
 else dSchritt = 0;
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBalken::WM_LButtonDown(WPARAM wParam, LPARAM lParam)
{
 ThreadSicher_Anfang();
 if(ulPosition_max){
	 ulPosition = LOWORD(lParam);
	 UpdateFenster(nullptr, true, false);
 }
 if(pfnWM_LButtonDown) pfnWM_LButtonDown(this, wParam, lParam);
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBalken::WM_MouseMove(WPARAM wParam, LPARAM lParam)
{
 ThreadSicher_Anfang();
 if(ulPosition_max && wParam == MK_LBUTTON){
	 ulPosition = LOWORD(lParam);
	 UpdateFenster(nullptr, true, false);
 }
 if(pfnWM_MouseMove) pfnWM_MouseMove(this, wParam, lParam);
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBalken::SetzPosition_Max(unsigned long ulPosition)
{
 ThreadSicher_Anfang();
 ulPosition_max = ulPosition;
 if(ulPosition_max > ulPosition_min){ dSchritt = (double)lBreite / (double)(ulPosition_max - ulPosition_min);}
 else dSchritt = 0;
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBalken::SetzPosition_Min(unsigned long ulPosition)
{
 ThreadSicher_Anfang();
 ulPosition_min = ulPosition;
 if(ulPosition_max > ulPosition_min){ dSchritt = (double)lBreite / (double)(ulPosition_max - ulPosition_min);}
 else dSchritt = 0;
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBalken::SetzPosition(unsigned long ulPositionA)
{
 ThreadSicher_Anfang();
 ulPosition = (double)ulPositionA * dSchritt;
 UpdateFenster(nullptr, true, false);
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBalken::Fullfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
 ThreadSicher_Anfang();
 stFullfarbe.ucRot = ucRot; stFullfarbe.ucGrun = ucGrun; stFullfarbe.ucBlau = ucBlau;
 ThreadSicher_Ende();
}