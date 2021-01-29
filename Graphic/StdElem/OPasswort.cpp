/****************************************************************************
	OPasswort.cpp
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

#include "HStdElem.h"
#include "OPasswort.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COPasswort* __vectorcall RePag::GUI::COPasswortV(const char* pcFensterName, unsigned int uiIDElement)
{
 COPasswort* vPasswort = (COPasswort*)VMBlock(VMDialog(), sizeof(COPasswort));
 vPasswort->COPasswortV(VMDialog(), pcFensterName, uiIDElement);
 return vPasswort;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COPasswort* __vectorcall RePag::GUI::COPasswortV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement)
{
 COPasswort* vPasswort = (COPasswort*)VMBlock(vmSpeicher, sizeof(COPasswort));
 vPasswort->COPasswortV(vmSpeicher, pcFensterName, uiIDElement);
 return vPasswort;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_Passwort(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
	COPasswort* pPasswortA;
	switch(uiMessage){
		case WM_CREATE      : ((COPasswort*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create(hWnd);
													return NULL;
		case WM_SIZE        : pPasswortA = (COPasswort*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pPasswortA) pPasswortA->WM_Size_Element(hWnd, lParam);
													else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													return NULL;
		case WM_SETFOCUS    : ((COPasswort*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_SetFocus();
													return NULL;
		case WM_KILLFOCUS   : ((COPasswort*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_KillFocus();
													return NULL;
		case WM_CHAR        :	((COPasswort*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_Char(wParam);
													return NULL;
		case WM_CONTEXTMENU : ((COPasswort*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_ContexMenu(lParam);
													return NULL;
		case WM_LBUTTONDOWN : ((COPasswort*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonDown(lParam);
													return NULL;
		case WM_PAINT       : ((COPasswort*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_Paint();
													return NULL;
		case WM_NCDESTROY   : pPasswortA = (COPasswort*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pPasswortA->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pPasswortA->htEffekt_Timer, INVALID_HANDLE_VALUE);
													VMFreiV(pPasswortA);
													return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COPasswort::COPasswortV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA)
{
 COEditZeileV(vmSpeicher, pcRePag_Passwort, pcFensterName, uiIDElementA);

 vasPasswort = COStringAV(vmSpeicher);

 ucSchriftausrichtung = TXA_MITTEVERTICAL | TXA_MITTEHORIZONTAL;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::GUI::COPasswort::COFreiV(void)
{
	VMFreiV(vasPasswort);
	return ((COEditZeile*)this)->COFreiV();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COPasswort::WM_Paint(void)
{
 ThreadSicher_Anfang();
 PAINTSTRUCT stPaint; TEXTMETRIC stTextmetric; SIZE stZeichengrosse;
 BeginPaint(hWndElement, &stPaint);
 WM_Paint_Hintergrund(stPaint);

 SelectObject(stPaint.hdc, hFont);
 GetTextMetrics(stPaint.hdc, &stTextmetric);
 GetTextExtentPoint32(stPaint.hdc, vasPasswort->c_Str(), vasPasswort->Length(), &stZeichengrosse);
 POINT ptText = {0,0};
 if(ucSchriftausrichtung & TXA_RECHTS) ptText.x = lBreite - stZeichengrosse.cx;
 if(ucSchriftausrichtung & TXA_MITTEHORIZONTAL) ptText.x = (lBreite - stZeichengrosse.cx) / 2;
 if(ucSchriftausrichtung & TXA_UNTEN) ptText.y = lHohe - stTextmetric.tmHeight + stTextmetric.tmExternalLeading;
 if(ucSchriftausrichtung & TXA_MITTEVERTICAL) ptText.y = (lHohe - stTextmetric.tmHeight + stTextmetric.tmExternalLeading) / 2;
 if(lTextPos || stZeichengrosse.cx > lBreite) ptText.x = lTextPos *-1;

 SetBkMode(stPaint.hdc, TRANSPARENT);
 SetTextColor(stPaint.hdc, RGB(stSchriftfarbe.ucRot, stSchriftfarbe.ucGrun, stSchriftfarbe.ucBlau));
 ExtTextOut(stPaint.hdc, ptText.x, ptText.y, ETO_CLIPPED, &stPaint.rcPaint, vasPasswort->c_Str(), vasPasswort->Length(), nullptr);

 EndPaint(hWndElement, &stPaint);
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COPasswort::WM_SetFocus(void)
{
 ThreadSicher_Anfang();
 TEXTMETRIC stTextmetric; SIZE stZeichengrosse;
 HDC hdc = GetDC(hWndElement);
 SelectObject(hdc, hFont);
 GetTextMetrics(hdc, &stTextmetric);
 CreateCaret(hWndElement, nullptr, 0, stTextmetric.tmHeight);
 GetTextExtentPoint32(hdc, vasPasswort->c_Str(), vasPasswort->Length(), &stZeichengrosse);
 POINT ptText = {0,0};
 if(ucSchriftausrichtung & TXA_RECHTS) ptText.x = lBreite - stZeichengrosse.cx;
 if(ucSchriftausrichtung & TXA_MITTEHORIZONTAL) ptText.x = (lBreite - stZeichengrosse.cx) / 2;
 if(ucSchriftausrichtung & TXA_UNTEN) ptText.y = lHohe - stTextmetric.tmHeight + stTextmetric.tmExternalLeading;
 if(ucSchriftausrichtung & TXA_MITTEVERTICAL) ptText.y = (lHohe - stTextmetric.tmHeight + stTextmetric.tmExternalLeading) / 2;

 EnableMenuItem(hMenu, IDM_KOPIEREN, MF_BYCOMMAND | MF_GRAYED);
 EnableMenuItem(hMenu, IDM_AUSSCHNEIDEN, MF_BYCOMMAND | MF_GRAYED);
 EnableMenuItem(hMenu, IDM_EINFUGEN, MF_BYCOMMAND | MF_GRAYED);
 ptCaret.y = ptText.y;
 ptCaret.x = ptText.x + stZeichengrosse.cx;
 ulZeichenPos = vasPasswort->Length();
 ReleaseDC(hWndElement, hdc);
 ShowCaret(hWndElement); SetCaretPos(ptCaret.x, ptCaret.y);
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COPasswort::WM_Char(WPARAM wParam)
{ 
 HDC hdc; SIZE stZeichengrosse;
 switch(wParam){
	  case VK_BACK   : ThreadSicher_Anfang();
			               if(ulZeichenPos){ 
											 vasInhalt->Delete(--ulZeichenPos, 1); vasPasswort->Delete(ulZeichenPos, 1);
                       if(ucSchriftausrichtung & (1 << 0)) ptCaret.x -= 8;
											 else if(ucSchriftausrichtung & (1 << 3)){
												 hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
										 		 GetTextExtentPoint32(hdc, vasPasswort->c_Str(), vasPasswort->Length(), &stZeichengrosse);
												 ptCaret.x = (lBreite - stZeichengrosse.cx) / 2;
												 GetTextExtentPoint32(hdc, vasPasswort->c_Str(), ulZeichenPos, &stZeichengrosse);
                         ptCaret.x += stZeichengrosse.cx;
												 ReleaseDC(hWndElement, hdc);
											 }
											 UpdateFenster(nullptr, true, false);
										 }
										 SetCaretPos(ptCaret.x, ptCaret.y);
										 ThreadSicher_Ende();
								     break;
	  case VK_RETURN : ThreadSicher_Anfang();
			               if(pfnWM_Char_Return) pfnWM_Char_Return(this);
										 ThreadSicher_Ende();
		                 break;
	  case VK_ESCAPE : ThreadSicher_Anfang();
			               if(pfnWM_Char_Escape) pfnWM_Char_Escape(this);
										 ThreadSicher_Ende();
								     break;
	  default        : ThreadSicher_Anfang();
			               if(ZeichenVorgabe(wParam)){
											 if(ulZeichenPos < ulZeichen_max){
												 if(ulZeichenPos == vasInhalt->Length()){ *vasInhalt += (char*)&wParam; *vasPasswort += "*";}
												 else{ vasInhalt->Insert((char*)&wParam, ulZeichenPos); vasPasswort->Insert("*", ulZeichenPos); }
												 ulZeichenPos++;
												 hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
												 if(ucSchriftausrichtung & TXA_LINKS){
													 GetTextExtentPoint32(hdc, vasPasswort->c_Str(), ulZeichenPos, &stZeichengrosse);
													 ptCaret.x = stZeichengrosse.cx;
												 }
												 else if(ucSchriftausrichtung & TXA_MITTEVERTICAL){
											 		 GetTextExtentPoint32(hdc, vasPasswort->c_Str(), vasPasswort->Length(), &stZeichengrosse);
													 ptCaret.x = (lBreite - stZeichengrosse.cx) / 2;
													 GetTextExtentPoint32(hdc, vasPasswort->c_Str(), ulZeichenPos, &stZeichengrosse);
													 ptCaret.x += stZeichengrosse.cx;
												 }
												 ReleaseDC(hWndElement, hdc);
												 UpdateFenster(nullptr, true, false);
											 }
										 }
										 SetCaretPos(ptCaret.x, ptCaret.y);
										 ThreadSicher_Ende();
								     break;
 }
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COPasswort::WM_LButtonDown(LPARAM lParam)
{
 ThreadSicher_Anfang();
 if(hWndElement != GetFocus()) SetFocus(hWndElement);

 SIZE stZeichengrosse;
 HDC hdc = GetDC(hWndElement); 
 SelectObject(hdc, hFont);
 ulZeichenPos = 0;
 if(vasPasswort->Length()){
	 if(lTextPos || ucSchriftausrichtung & (1 << 0)){
		 do{ GetTextExtentPoint32(hdc, vasPasswort->c_Str(), ++ulZeichenPos, &stZeichengrosse); }
		 while(stZeichengrosse.cx - lTextPos < LOWORD(lParam) && ulZeichenPos < (int)vasPasswort->Length());
		 ptCaret.x = stZeichengrosse.cx - lTextPos;
	 }
	 else if(ucSchriftausrichtung & (1 << 3)){
		 GetTextExtentPoint32(hdc, vasPasswort->c_Str(), vasPasswort->Length(), &stZeichengrosse);
		 ptCaret.x = (lBreite - stZeichengrosse.cx) / 2;

		 if(LOWORD(lParam) > ptCaret.x){
			 do{ GetTextExtentPoint32(hdc, vasPasswort->c_Str(), ++ulZeichenPos, &stZeichengrosse); }
			 while(stZeichengrosse.cx + ptCaret.x < LOWORD(lParam) && ulZeichenPos < (int)vasPasswort->Length());
			 ptCaret.x += stZeichengrosse.cx;
		 }
	 }
 }
 ReleaseDC(hWndElement, hdc);
 SetCaretPos(ptCaret.x, ptCaret.y);
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COPasswort::Text(char* pcText)
{ 
 ThreadSicher_Anfang();
 *vasInhalt = pcText; *vasPasswort = NULL;
 for(BYTE ucZeichen = 0; ucZeichen < vasInhalt->Length(); ucZeichen++) *vasPasswort += "*";
 UpdateFenster(nullptr, true, false);
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------