/****************************************************************************
	OEditZeile.cpp
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
#include "OEditZeile.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COEditZeile* __vectorcall RePag::GUI::COEditZeileV(const char* pcFensterName, unsigned int uiIDElement)
{
 COEditZeile* vEditZeile = (COEditZeile*)VMBlock(VMDialog(), sizeof(COEditZeile));
 vEditZeile->COEditZeileV(VMDialog(), pcFensterName, uiIDElement);
 return vEditZeile;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COEditZeile* __vectorcall RePag::GUI::COEditZeileV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement)
{
 COEditZeile* vEditZeile = (COEditZeile*)VMBlock(vmSpeicher, sizeof(COEditZeile));
 vEditZeile->COEditZeileV(vmSpeicher, pcFensterName, uiIDElement);
 return vEditZeile;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_EditZeile(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
	COEditZeile* pEditZeile;
	switch(uiMessage){
		case WM_CREATE        : ((COEditZeile*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create(hWnd);
														return NULL;
		case WM_SIZE          : pEditZeile = (COEditZeile*)GetWindowLongPtr(hWnd, GWL_USERDATA);
														if(pEditZeile) pEditZeile->WM_Size_Element(hWnd, lParam);
														else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
														return NULL;
		case WM_SETFOCUS      : ((COEditZeile*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_SetFocus();
														return NULL;
		case WM_KILLFOCUS     : ((COEditZeile*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_KillFocus();
														return NULL;
		case WM_KEYDOWN       :	((COEditZeile*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_KeyDown(wParam, lParam);
														return NULL;
		case WM_CHAR          :	((COEditZeile*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_Char(wParam);
														return NULL;
		case WM_COMMAND       : pEditZeile = (COEditZeile*)GetWindowLongPtr(hWnd, GWL_USERDATA);
														if(!pEditZeile->WM_Command(wParam)) return NULL;
														else if(pEditZeile->pfnWM_Command){
															pEditZeile->ThreadSicher_Anfang();
															if(!pEditZeile->pfnWM_Command(pEditZeile, wParam)){ pEditZeile->ThreadSicher_Ende(); return NULL; }
															pEditZeile->ThreadSicher_Ende();
														}
														else PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
														break;
		case WM_CONTEXTMENU   : ((COEditZeile*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_ContexMenu(lParam);
														return NULL;
		case WM_MOUSEMOVE     : ((COEditZeile*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_MouseMove(wParam, lParam);
														return NULL;
		case WM_LBUTTONDOWN   : ((COEditZeile*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_LButtonDown(wParam, lParam);
														return NULL;
		case WM_LBUTTONUP     : ((COEditZeile*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_LButtonUp(wParam, lParam);
														return NULL;
		case WM_LBUTTONDBLCLK : ((COEditZeile*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_LButtonDBClick(wParam, lParam);
														return NULL;
		case WM_PAINT         : ((COEditZeile*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_Paint();
														return NULL;
		case WM_NCDESTROY     : pEditZeile = (COEditZeile*)GetWindowLongPtr(hWnd, GWL_USERDATA);
														if(pEditZeile->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pEditZeile->htEffekt_Timer, INVALID_HANDLE_VALUE);
														VMFreiV(pEditZeile);
														return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::COEditZeileV(VMEMORY vmSpeicher, const char* pcKlassenName, const char* pcFensterName, unsigned int uiIDElementA)
{
 COTextZeileV(vmSpeicher, pcKlassenName, pcFensterName, uiIDElementA);

 pfnWM_Char_Return = nullptr;
 pfnWM_Char_Escape = nullptr;
 pfnWM_KillFocus = nullptr;
 pfnWM_Command = nullptr;
 pfnWM_LButtonDown = nullptr;
 pfnWM_LButtonUp = nullptr;
 pfnWM_LButtonDBClick = nullptr;
 ptCaret.x = 0;
 ptCaret.y = 0;
 cSelect = 0;
 rcSelect.left = rcSelect.right = rcSelect.top = rcSelect.bottom = 0;
 lTextPos = 0;
 ulZeichenPos = 0;
 ulSelectPos = 0;
 ulZeichen_max = 0x7fffffff;
 ucZeichenVorgabe = ZV_ALLE; 
 stSelectSchriftfarbe.ucRot = stSelectSchriftfarbe.ucGrun = stSelectSchriftfarbe.ucBlau = 0;
 stSelectHintergrundfarbe.ucRot = 0; stSelectHintergrundfarbe.ucGrun = 220; stSelectHintergrundfarbe.ucBlau = 220;

 vasZeichenMaske = COStringAV(vmSpeicher);

 hMenu = CreatePopupMenu();
 AppendMenu(hMenu, MF_STRING, IDM_AUSSCHNEIDEN, "Ausschneiden		Strg+X");
 AppendMenu(hMenu, MF_STRING, IDM_KOPIEREN, "Kopieren		Strg+C");
 AppendMenu(hMenu, MF_STRING, IDM_EINFUGEN, "Einfügen		Strg+V");
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::COEditZeileV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA)
{
 COEditZeileV(vmSpeicher, pcRePag_EditZeile, pcFensterName, uiIDElementA);
}
//---------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::GUI::COEditZeile::COFreiV(void)
{
 VMFreiV(vasZeichenMaske);
 DestroyMenu(hMenu);
 return ((COTextZeile*)this)->COFreiV();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::WM_Paint(void)
{
 ThreadSicher_Anfang();
 PAINTSTRUCT stPaint; SIZE stZeichengrosse; POINT ptText = {0,0};
 BeginPaint(hWndElement, &stPaint);
 WM_Paint_Hintergrund(stPaint);

 SelectObject(stPaint.hdc, hFont);
 Schriftausrichtung(stPaint.hdc, stZeichengrosse, ptText);
 if(lTextPos || stZeichengrosse.cx > lBreite) ptText.x = lTextPos *-1; 
 
 if(!cSelect){ SetBkMode(stPaint.hdc, TRANSPARENT); SetTextColor(stPaint.hdc, RGB(stSchriftfarbe.ucRot, stSchriftfarbe.ucGrun, stSchriftfarbe.ucBlau));
	 ExtTextOut(stPaint.hdc, ptText.x, ptText.y, ETO_CLIPPED, &stPaint.rcPaint, vasInhalt->c_Str(), vasInhalt->Length(), nullptr);
 }
 else if(rcSelect.left <= stPaint.rcPaint.left && rcSelect.right >= stPaint.rcPaint.right){
	 SetBkMode(stPaint.hdc, OPAQUE);
	 SetBkColor(stPaint.hdc, RGB(stSelectHintergrundfarbe.ucRot, stSelectHintergrundfarbe.ucGrun, stSelectHintergrundfarbe.ucBlau ));
	 SetTextColor(stPaint.hdc, RGB(stSelectSchriftfarbe.ucRot, stSelectSchriftfarbe.ucGrun, stSelectSchriftfarbe.ucBlau));
	 ExtTextOut(stPaint.hdc, ptText.x, ptText.y, ETO_CLIPPED, &stPaint.rcPaint, vasInhalt->c_Str(), vasInhalt->Length(), nullptr);
 }
 else{ RECT rcZeichnen; rcZeichnen = rcSelect;
   SetBkMode(stPaint.hdc, OPAQUE);
	 SetBkColor(stPaint.hdc, RGB(stSelectHintergrundfarbe.ucRot, stSelectHintergrundfarbe.ucGrun, stSelectHintergrundfarbe.ucBlau ));
	 SetTextColor(stPaint.hdc, RGB(stSelectSchriftfarbe.ucRot, stSelectSchriftfarbe.ucGrun, stSelectSchriftfarbe.ucBlau));
	 ExtTextOut(stPaint.hdc, ptText.x, ptText.y, ETO_CLIPPED, &rcZeichnen, vasInhalt->c_Str(), vasInhalt->Length(), nullptr);

	 SetBkMode(stPaint.hdc, TRANSPARENT); SetTextColor(stPaint.hdc, RGB(stSchriftfarbe.ucRot, stSchriftfarbe.ucGrun, stSchriftfarbe.ucBlau));
	 if(rcSelect.left > stPaint.rcPaint.left){
		 rcZeichnen.right = rcSelect.left; rcZeichnen.left = stPaint.rcPaint.left;
		 ExtTextOut(stPaint.hdc, ptText.x, ptText.y, ETO_CLIPPED, &rcZeichnen, vasInhalt->c_Str(), vasInhalt->Length(), nullptr);
	 }
	 if(rcSelect.right < stPaint.rcPaint.right){
		 rcZeichnen.right = stPaint.rcPaint.right; rcZeichnen.left = rcSelect.right;
		 ExtTextOut(stPaint.hdc, ptText.x, ptText.y, ETO_CLIPPED, &rcZeichnen, vasInhalt->c_Str(), vasInhalt->Length(), nullptr);
	 }
 }
 EndPaint(hWndElement, &stPaint);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::WM_Create(HWND hWnd)
{
 ZeichenMetric(hWnd); 
 rcSelect.bottom = lHohe;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::WM_SetFocus(void)
{
 ThreadSicher_Anfang();
 SIZE stZeichengrosse; POINT ptText = {0,0};
 HDC hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
 CreateCaret(hWndElement, nullptr, 0, lZeichen_hohe);

 Schriftausrichtung(hdc, stZeichengrosse, ptText);
 ptCaret.y = ptText.y;
 if(vasZeichenMaske->Length()){
	 ptCaret.x = ptText.x; 
	 if(!cSelect) ulZeichenPos = 0;
	 EnableMenuItem(hMenu, IDM_AUSSCHNEIDEN, MF_BYCOMMAND | MF_GRAYED);
	 EnableMenuItem(hMenu, IDM_EINFUGEN, MF_BYCOMMAND | MF_GRAYED);
 }
 else{ ptCaret.x = ptText.x + stZeichengrosse.cx;
	 if(!cSelect) ulZeichenPos = vasInhalt->Length();
 }
 ReleaseDC(hWndElement, hdc);

 if(!cSelect){ ShowCaret(hWndElement); SetCaretPos(ptCaret.x, ptCaret.y); }
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::WM_KillFocus(void)
{
 ThreadSicher_Anfang();
 HideCaret(hWndElement); DestroyCaret();
 if(pfnWM_KillFocus) pfnWM_KillFocus(this);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::WM_KeyDown(WPARAM wParam, LPARAM lParam)
{
 HDC hdc; SIZE stZeichengrosse; VMBLOCK vbZeichen; RECT rcZeichnen;
 switch(wParam){
		case VK_HOME   : ThreadSicher_Anfang();
			               hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
			               if(cSelect) DeSelect();
			               ulZeichenPos = 0;
										 if(lTextPos){ lTextPos = 0; ptCaret.x = 0; UpdateFenster(nullptr, true, false); }
										 else if(ucSchriftausrichtung & TXA_LINKS) ptCaret.x = 0; 
										 else{ 
											 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
											 if(stZeichengrosse.cx > lBreite) ptCaret.x = 0;
											 else if(ucSchriftausrichtung & TXA_RECHTS) ptCaret.x = lBreite - stZeichengrosse.cx;
											 else ptCaret.x = (lBreite - stZeichengrosse.cx) / 2;
										 } 
										 SetCaretPos(ptCaret.x, ptCaret.y);
										 ReleaseDC(hWndElement, hdc);
										 ThreadSicher_Ende();
										 break;
		case VK_END    : ThreadSicher_Anfang();
			               hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
			               if(cSelect) DeSelect();
										 ulZeichenPos = vasInhalt->Length();
		                 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
										 if(lTextPos){ ptCaret.x = lBreite; lTextPos = stZeichengrosse.cx - lBreite; UpdateFenster(nullptr, true, false); }
										 else if(ucSchriftausrichtung & TXA_LINKS) ptCaret.x = stZeichengrosse.cx;
										 else if(stZeichengrosse.cx > lBreite){ ptCaret.x = lBreite; lTextPos = stZeichengrosse.cx - lBreite; UpdateFenster(nullptr, true, false); }
										 else if(ucSchriftausrichtung & TXA_RECHTS) ptCaret.x = lBreite;
										 else ptCaret.x = (lBreite - stZeichengrosse.cx) / 2 + stZeichengrosse.cx; 
										 SetCaretPos(ptCaret.x, ptCaret.y);
										 ReleaseDC(hWndElement, hdc);
										 ThreadSicher_Ende();
										 break;
		case VK_LEFT   : ThreadSicher_Anfang();
			               hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
			               if(ulZeichenPos){ rcZeichnen.right = ptCaret.x;
											 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), --ulZeichenPos, &stZeichengrosse);
											 if(lTextPos && !ptCaret.x){
												 rcZeichnen.top = 0; rcZeichnen.bottom = lHohe;
												 rcZeichnen.left = 0; rcZeichnen.right = lBreite - lTextPos + stZeichengrosse.cx;
												 ScrollWindow(hWndElement, lTextPos - stZeichengrosse.cx, 0, &rcZeichnen, nullptr);
												 rcZeichnen.right = lTextPos - stZeichengrosse.cx;
												 lTextPos -= rcZeichnen.right;
												 if(cSelect) rcSelect.right += rcZeichnen.right;
												 UpdateFenster(&rcZeichnen, true, false);
											 }
											 else if(stZeichengrosse.cx - lTextPos < 0){
												 rcZeichnen.top = 0; rcZeichnen.bottom = lHohe;
												 rcZeichnen.left = 0; rcZeichnen.right = lBreite - lTextPos + stZeichengrosse.cx;
												 ScrollWindow(hWndElement, lTextPos - stZeichengrosse.cx, 0, &rcZeichnen, nullptr);
												 rcZeichnen.right = ptCaret.x + lTextPos - stZeichengrosse.cx;
												 if(cSelect) rcSelect.right += lTextPos - stZeichengrosse.cx;
												 lTextPos -= lTextPos - stZeichengrosse.cx;
												 UpdateFenster(&rcZeichnen, true, false);
												 ptCaret.x = 0;												 	 
											 }
											 else if(ucSchriftausrichtung & TXA_LINKS){
												 if(vasZeichenMaske->Length()){
													 if(ZeichenMaske_FestLinks()) GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
												 }
                         ptCaret.x = stZeichengrosse.cx - lTextPos;
											 }
											 else{
												 if(vasZeichenMaske->Length()){
													 if(ZeichenMaske_FestLinks()){
													   GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
														 (ucSchriftausrichtung & TXA_RECHTS ? ptCaret.x = lBreite - stZeichengrosse.cx : ptCaret.x = (lBreite - stZeichengrosse.cx) / 2);
														 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
														 ptCaret.x += stZeichengrosse.cx;
													 }
													 else{
														 vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
														 GetTextExtentPoint32(hdc, vbZeichen, 1, &stZeichengrosse); VMFrei(vbZeichen);
														 ptCaret.x -= stZeichengrosse.cx;
													 }
												 }
												 else{
													 vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
													 GetTextExtentPoint32(hdc, vbZeichen, 1, &stZeichengrosse); VMFrei(vbZeichen);
													 ptCaret.x -= stZeichengrosse.cx;
												 }
											 }

											 if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
												 if(cSelect < 0) rcSelect.left = ptCaret.x;
												 else if(cSelect > 0){
													 if(ulSelectPos < ulZeichenPos) rcSelect.right = ptCaret.x;
													 else{ cSelect = 0; ShowCaret(hWndElement); }
												 }
												 else{ cSelect = -1;
													 ulSelectPos = ulZeichenPos + 1;
 													 rcSelect.left = ptCaret.x; rcSelect.right = rcZeichnen.right;
													 HideCaret(hWndElement);
												 }
												 rcZeichnen.top = 0; rcZeichnen.bottom = lHohe; rcZeichnen.left = ptCaret.x;
												 UpdateFenster(&rcZeichnen, true, false);
											 }
											 else if(cSelect) DeSelect();
										 }
										 SetCaretPos(ptCaret.x, ptCaret.y);
 										 ReleaseDC(hWndElement, hdc);
										 ThreadSicher_Ende();
										 break;
		case VK_RIGHT  : ThreadSicher_Anfang();
			               hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
			               if(ulZeichenPos < vasInhalt->Length()){ rcZeichnen.left = ptCaret.x;
			                 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ++ulZeichenPos, &stZeichengrosse);
											 if(stZeichengrosse.cx - lTextPos > lBreite){
											   rcZeichnen.top = 0; rcZeichnen.bottom = lHohe;
												 rcZeichnen.left = stZeichengrosse.cx - lTextPos - lBreite; rcZeichnen.right = lBreite;
												 ScrollWindow(hWndElement, rcZeichnen.left *-1, 0, &rcZeichnen, nullptr);
												 lTextPos += rcZeichnen.left;
												 (!cSelect ? rcSelect.left = ptCaret.x - rcZeichnen.left : rcSelect.left -= rcZeichnen.left);
												 rcZeichnen.left = lBreite - rcZeichnen.left; 
                         ptCaret.x = rcZeichnen.right = lBreite;
												 UpdateFenster(&rcZeichnen, true, false);
												 rcZeichnen.left = rcSelect.left;
											 }
											 else if(ucSchriftausrichtung & TXA_LINKS){
												 if(vasZeichenMaske->Length()){
													 if(ZeichenMaske_FestRechts()) GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
												 }
												 ptCaret.x = stZeichengrosse.cx - lTextPos;
											 }
											 else{
 												 if(vasZeichenMaske->Length()){
													 if(ZeichenMaske_FestRechts()){
													   GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
														 (ucSchriftausrichtung & TXA_RECHTS ? ptCaret.x = lBreite - stZeichengrosse.cx : ptCaret.x = (lBreite - stZeichengrosse.cx) / 2);
													   GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
													   ptCaret.x += stZeichengrosse.cx;
													 }
	 												 else{
														 vasInhalt->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
														 GetTextExtentPoint32(hdc, vbZeichen, 1, &stZeichengrosse); VMFrei(vbZeichen);
														 ptCaret.x += stZeichengrosse.cx;
												   }
												 }
												 else{
													 vasInhalt->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
													 GetTextExtentPoint32(hdc, vbZeichen, 1, &stZeichengrosse); VMFrei(vbZeichen);
													 ptCaret.x += stZeichengrosse.cx;
												 }
											 }

											 if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){ 
												 if(cSelect > 0) rcSelect.right = ptCaret.x;
												 else if(cSelect < 0){
													 if(ulSelectPos > ulZeichenPos) rcSelect.left = ptCaret.x;
													 else{ cSelect = 0; ShowCaret(hWndElement);}
												 }
												 else{ cSelect = 1;
													 ulSelectPos = ulZeichenPos - 1; 
													 rcSelect.left = rcZeichnen.left; rcSelect.right = ptCaret.x;
													 HideCaret(hWndElement);
												 }											 
												 rcZeichnen.top = 0; rcZeichnen.bottom = lHohe; rcZeichnen.right = ptCaret.x;
												 UpdateFenster(&rcZeichnen, true, false);
											 }
											 else if(cSelect) DeSelect();
										 }
										 SetCaretPos(ptCaret.x, ptCaret.y);
										 ReleaseDC(hWndElement, hdc);
										 ThreadSicher_Ende();
										 break;
		case VK_DELETE : ThreadSicher_Anfang();
										 if(!ucZeichenVorgabe){ ThreadSicher_Ende(); break; }
			               if(!vasZeichenMaske->Length()){
											 hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
			                 if(cSelect) Select_Loschen(hdc);
											 else if(ulZeichenPos < vasInhalt->Length()){ rcZeichnen.top = 0; rcZeichnen.bottom = lHohe;
												 if(ucSchriftausrichtung & TXA_LINKS){ 
													 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
													 if(stZeichengrosse.cx - lTextPos > lBreite) rcZeichnen.right = lBreite;
													 else rcZeichnen.right = stZeichengrosse.cx - lTextPos + 1;
													 vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
													 GetTextExtentPoint32(hdc, vbZeichen, 1, &stZeichengrosse); VMFrei(vbZeichen);
													 if(ptCaret.x + stZeichengrosse.cx > lBreite){ SetCaretPos(ptCaret.x, ptCaret.y); ReleaseDC(hWndElement, hdc); ThreadSicher_Ende(); break; }
													 rcZeichnen.left = ptCaret.x + stZeichengrosse.cx;
													 ScrollWindow(hWndElement, stZeichengrosse.cx *-1, 0, &rcZeichnen, nullptr);
													 rcZeichnen.left = rcZeichnen.right - stZeichengrosse.cx;
													 vasInhalt->Delete(ulZeichenPos, 1);
												 }
												 else if(ucSchriftausrichtung & TXA_RECHTS){ 
													 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
													 if(stZeichengrosse.cx - lTextPos > lBreite){
														 vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
														 GetTextExtentPoint32(hdc, vbZeichen, 1, &stZeichengrosse); VMFrei(vbZeichen);
														 if(stZeichengrosse.cx > lBreite - ptCaret.x){ SetCaretPos(ptCaret.x, ptCaret.y); ReleaseDC(hWndElement, hdc); ThreadSicher_Ende(); break; }
														 rcZeichnen.right = lBreite; rcZeichnen.left = ptCaret.x + stZeichengrosse.cx;
														 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
														 if(stZeichengrosse.cx - lBreite - lTextPos > rcZeichnen.left - ptCaret.x){
															 ScrollWindow(hWndElement, ptCaret.x - rcZeichnen.left, 0, &rcZeichnen, nullptr);
															 rcZeichnen.left = lBreite - rcZeichnen.left + ptCaret.x;
														 }
														 else{
															 lTextPos = stZeichengrosse.cx - lBreite - rcZeichnen.left + ptCaret.x;
															 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
															 ptCaret.x = stZeichengrosse.cx - lTextPos;
															 rcZeichnen.left = 0; rcZeichnen.right = lBreite;
														 }
														 vasInhalt->Delete(ulZeichenPos, 1);
													 }
													 else{
														 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
														 rcZeichnen.left = ptCaret.x - stZeichengrosse.cx + lTextPos;
														 vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
														 GetTextExtentPoint32(hdc, vbZeichen, 1, &stZeichengrosse); VMFrei(vbZeichen);
														 rcZeichnen.right = ptCaret.x;
														 ScrollWindow(hWndElement, stZeichengrosse.cx, 0, &rcZeichnen, nullptr);
														 rcZeichnen.right = rcZeichnen.left + stZeichengrosse.cx;
														 ptCaret.x += stZeichengrosse.cx;
														 if(lTextPos) lTextPos -= stZeichengrosse.cx;
														 vasInhalt->Delete(ulZeichenPos, 1);
													 }
												 }
												 else{ 
													 vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
													 GetTextExtentPoint32(hdc, vbZeichen, 1, &stZeichengrosse); VMFrei(vbZeichen);
													 if(ptCaret.x + stZeichengrosse.cx > lBreite){ SetCaretPos(ptCaret.x, ptCaret.y); ReleaseDC(hWndElement, hdc); ThreadSicher_Ende(); break; }
													 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
													 if(lTextPos){ rcZeichnen.left = 0; rcZeichnen.right = lBreite;}
													 else{ rcZeichnen.left = (lBreite - stZeichengrosse.cx) / 2; rcZeichnen.right = rcZeichnen.left + stZeichengrosse.cx;}
													 vasInhalt->Delete(ulZeichenPos, 1);
													 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
													 ptCaret.x = (lBreite - stZeichengrosse.cx) / 2;
													 if(lTextPos) lTextPos = ptCaret.x *-1;
													 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
													 ptCaret.x += stZeichengrosse.cx;	
												 }
												 UpdateFenster(&rcZeichnen, true, false);
											 }
											 SetCaretPos(ptCaret.x, ptCaret.y);
											 ReleaseDC(hWndElement, hdc);
											 ThreadSicher_Ende();
										 }
										 PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), WM_CHAR);
										 break;
		default        : break;
 }
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::WM_Char(WPARAM wParam)
{
 HDC hdc; VMBLOCK vbZeichen; SIZE stZeichengrosse; RECT rcZeichnen;
 switch(wParam){
	  case VK_TAB    : ThreadSicher_Anfang();
										 if(!ucZeichenVorgabe){ ThreadSicher_Ende(); break; }
			               rcZeichnen.right = 0;
			               do{ SendMessage(hWndElement, WM_CHAR, ' ', NULL); }
								     while(++rcZeichnen.right < 4);
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
	  case VK_BACK   : ThreadSicher_Anfang();
			               if(!ucZeichenVorgabe){ ThreadSicher_Ende(); break; }
			               if(ulZeichenPos || !ulZeichenPos && cSelect){ 
											 hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
											 rcZeichnen.top = 0; rcZeichnen.bottom = lHohe;
			                 if(!vasZeichenMaske->Length()){
												 if(cSelect) Select_Loschen(hdc);
												 else{
													 if(ucSchriftausrichtung & TXA_LINKS){ 
														 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
														 (stZeichengrosse.cx - lTextPos > lBreite ?	rcZeichnen.right = lBreite : rcZeichnen.right = stZeichengrosse.cx - lTextPos + 1);
														 vasInhalt->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
														 GetTextExtentPoint32(hdc, vbZeichen, 1, &stZeichengrosse); VMFrei(vbZeichen);
														 if(ptCaret.x < stZeichengrosse.cx){ ReleaseDC(hWndElement, hdc); ThreadSicher_Ende(); break; }
														 rcZeichnen.left = ptCaret.x;
														 ScrollWindow(hWndElement, stZeichengrosse.cx *-1, 0, &rcZeichnen, nullptr);
														 rcZeichnen.left = rcZeichnen.right - stZeichengrosse.cx;
														 ptCaret.x -= stZeichengrosse.cx;
														 vasInhalt->Delete(--ulZeichenPos, 1);
													 }
													 else if(ucSchriftausrichtung & TXA_RECHTS){ 
														 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
														 if(stZeichengrosse.cx - lTextPos > lBreite){
															 rcZeichnen.right = stZeichengrosse.cx - lTextPos - lBreite;
															 vasInhalt->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
															 GetTextExtentPoint32(hdc, vbZeichen, 1, &stZeichengrosse); VMFrei(vbZeichen);
															 if(stZeichengrosse.cx > ptCaret.x){ ReleaseDC(hWndElement, hdc); ThreadSicher_Ende(); break; }
															 if(rcZeichnen.right > stZeichengrosse.cx){
																 rcZeichnen.left = ptCaret.x; rcZeichnen.right = lBreite;
																 ScrollWindow(hWndElement, stZeichengrosse.cx *-1, 0, &rcZeichnen, nullptr);
																 rcZeichnen.left = lBreite - stZeichengrosse.cx;
																 ptCaret.x -= stZeichengrosse.cx;
																 vasInhalt->Delete(--ulZeichenPos, 1);
															 }
															 else{
																 vasInhalt->Delete(--ulZeichenPos, 1);
																 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
																 lTextPos = stZeichengrosse.cx - lBreite;
																 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
																 ptCaret.x = stZeichengrosse.cx - lTextPos;
																 rcZeichnen.left = 0; rcZeichnen.right = lBreite;
															 }
														 }
														 else{
															 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
															 rcZeichnen.left = ptCaret.x - stZeichengrosse.cx + lTextPos;
															 vasInhalt->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
															 GetTextExtentPoint32(hdc, vbZeichen, 1, &stZeichengrosse); VMFrei(vbZeichen);
															 if(stZeichengrosse.cx > ptCaret.x){ ReleaseDC(hWndElement, hdc); ThreadSicher_Ende(); break; }
															 rcZeichnen.right = ptCaret.x - stZeichengrosse.cx;
															 ScrollWindow(hWndElement, stZeichengrosse.cx, 0, &rcZeichnen, nullptr);
															 rcZeichnen.right = rcZeichnen.left + stZeichengrosse.cx;
															 vasInhalt->Delete(--ulZeichenPos, 1);

															 if(lTextPos){
																 (lTextPos > stZeichengrosse.cx ? lTextPos -= stZeichengrosse.cx : lTextPos = 0);
															 }
														 }
													 }
													 else{
														 vasInhalt->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
														 GetTextExtentPoint32(hdc, vbZeichen, 1, &stZeichengrosse); VMFrei(vbZeichen);
														 if(ptCaret.x < stZeichengrosse.cx){ ReleaseDC(hWndElement, hdc); ThreadSicher_Ende(); break ;}
														 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
														 if(lTextPos){ if(ptCaret.x <= 0){ ReleaseDC(hWndElement, hdc); ThreadSicher_Ende(); break; }
															 rcZeichnen.left = 0; rcZeichnen.right = lBreite; }
														 else{ rcZeichnen.left = (lBreite - stZeichengrosse.cx) / 2; rcZeichnen.right = rcZeichnen.left + stZeichengrosse.cx; }
														 vasInhalt->Delete(--ulZeichenPos, 1);

														 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
														 ptCaret.x = (lBreite - stZeichengrosse.cx) / 2;
														 if(lTextPos) lTextPos = ptCaret.x *-1;
														 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
														 ptCaret.x += stZeichengrosse.cx;	
													 }
													 UpdateFenster(&rcZeichnen, true, false);
												 }
											 }
											 else{
												 if(cSelect){
													 if(cSelect < 0) ulZeichenPos = ulSelectPos;
													 DeSelect();
													 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
													 ptCaret.x = stZeichengrosse.cx;	
												 }
												 if(ZeichenMaske_Loschen()){
													 vasInhalt->Delete(--ulZeichenPos, 1);
													 (ucZeichenVorgabe & ZV_SICHTBAR ? vasInhalt->Insert("_", ulZeichenPos) : vasInhalt->Insert(" ", ulZeichenPos));
                           rcZeichnen.right = 1;
												 }
												 else rcZeichnen.right = 0;

												 if(ucSchriftausrichtung & TXA_LINKS) ptCaret.x = 0;	
												 else if(ucSchriftausrichtung & TXA_RECHTS){
													 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
													 ptCaret.x = lBreite - stZeichengrosse.cx;
												 }
												 else{
													 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
													 ptCaret.x = (lBreite - stZeichengrosse.cx) / 2;
												 }
												 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
												 ptCaret.x += stZeichengrosse.cx;

												 if(rcZeichnen.right){ UpdateFenster(nullptr, true, false); }
											 }
											 ReleaseDC(hWndElement, hdc);
								     } 
										 SetCaretPos(ptCaret.x, ptCaret.y);
										 ThreadSicher_Ende();
										 PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), WM_CHAR);
								     break;
		default        : ThreadSicher_Anfang();
			               if(ZeichenVorgabe(wParam) && ulZeichenPos < ulZeichen_max){
											 hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
											 if(!vasZeichenMaske->Length()){
												 if(cSelect) Select_Loschen(hdc);
												 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
												 rcZeichnen.right = stZeichengrosse.cx - lTextPos;
												 GetTextExtentPoint32(hdc, (LPSTR)&wParam, 1, &stZeichengrosse);
												 rcZeichnen.right += stZeichengrosse.cx;
												 if(rcZeichnen.right <= lBreite){
													 (ulZeichenPos == vasInhalt->Length() ? *vasInhalt += (char*)&wParam : vasInhalt->Insert((char*)&wParam, ulZeichenPos));
													 rcZeichnen.top = 0; rcZeichnen.bottom = lHohe;
													 if(ucSchriftausrichtung & TXA_LINKS){ 
														 rcZeichnen.left = ptCaret.x; rcZeichnen.right = lBreite - stZeichengrosse.cx;
														 ScrollWindow(hWndElement, stZeichengrosse.cx, 0, &rcZeichnen, nullptr);
														 ptCaret.x += stZeichengrosse.cx; rcZeichnen.right = ptCaret.x;
													 }
													 else if(ucSchriftausrichtung & TXA_RECHTS){ 
														 rcZeichnen.left = stZeichengrosse.cx; rcZeichnen.right = ptCaret.x;
														 ScrollWindow(hWndElement, stZeichengrosse.cx *-1, 0, &rcZeichnen, nullptr);
														 rcZeichnen.left = ptCaret.x - stZeichengrosse.cx;
													 }
													 else{ 
														 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
														 rcZeichnen.left = (lBreite - stZeichengrosse.cx) / 2;
														 rcZeichnen.right = rcZeichnen.left + stZeichengrosse.cx;
														 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos + 1, &stZeichengrosse);
														 ptCaret.x = rcZeichnen.left + stZeichengrosse.cx;
													 }
													 ulZeichenPos++;
													 UpdateFenster(&rcZeichnen, true, false);
												 }
												 else if(ucZeichenVorgabe & (1 << 4)){ 
													 (ulZeichenPos == vasInhalt->Length() ? *vasInhalt += (char*)&wParam : vasInhalt->Insert((char*)&wParam, ulZeichenPos));
													 rcZeichnen.top = 0; rcZeichnen.bottom = lHohe;
													 if(ucSchriftausrichtung & TXA_LINKS){
														 if(ptCaret.x + stZeichengrosse.cx <= lBreite){
															 rcZeichnen.left = ptCaret.x; rcZeichnen.right = lBreite - stZeichengrosse.cx;
															 ScrollWindow(hWndElement, stZeichengrosse.cx, 0, &rcZeichnen, nullptr);
															 ptCaret.x += stZeichengrosse.cx; rcZeichnen.right = ptCaret.x;
														 }
														 else if(ptCaret.x < lBreite){
															 rcZeichnen.left = ptCaret.x - lBreite + stZeichengrosse.cx; rcZeichnen.right = ptCaret.x;
															 ScrollWindow(hWndElement, rcZeichnen.left *-1, 0, &rcZeichnen, nullptr);
															 rcZeichnen.left = rcZeichnen.right - stZeichengrosse.cx; 
															 lTextPos += ptCaret.x - lBreite + stZeichengrosse.cx;
                               ptCaret.x = rcZeichnen.right = lBreite;																 
														 }
														 else{
															 rcZeichnen.left = stZeichengrosse.cx; rcZeichnen.right = ptCaret.x;
															 ScrollWindow(hWndElement, stZeichengrosse.cx *-1, 0, &rcZeichnen, nullptr);
															 rcZeichnen.left = rcZeichnen.right - stZeichengrosse.cx; 
															 lTextPos += stZeichengrosse.cx;
														 }
													 }
													 else if(ucSchriftausrichtung & TXA_RECHTS){
														 (!lTextPos ? lTextPos += rcZeichnen.right - lBreite : lTextPos += stZeichengrosse.cx);
                             rcZeichnen.left = stZeichengrosse.cx; rcZeichnen.right = ptCaret.x;
														 ScrollWindow(hWndElement, stZeichengrosse.cx *-1, 0, &rcZeichnen, nullptr);
                             rcZeichnen.left = ptCaret.x - stZeichengrosse.cx;
													 }
													 else{ 
                             if(ptCaret.x + stZeichengrosse.cx <= lBreite){
															 rcZeichnen.left = stZeichengrosse.cx / 2; rcZeichnen.right = ptCaret.x;
															 ScrollWindow(hWndElement, rcZeichnen.left *-1, 0, &rcZeichnen, nullptr);
															 rcZeichnen.right = lBreite - stZeichengrosse.cx + rcZeichnen.left;
															 rcZeichnen.left = ptCaret.x; 
															 ScrollWindow(hWndElement, lBreite - rcZeichnen.right, 0, &rcZeichnen, nullptr);
															 rcZeichnen.left = ptCaret.x - stZeichengrosse.cx / 2;
															 rcZeichnen.right = rcZeichnen.left + stZeichengrosse.cx;
															 ptCaret.x = rcZeichnen.right;
															 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos + 1, &stZeichengrosse);
															 lTextPos = stZeichengrosse.cx - ptCaret.x;
														 }
														 else{
															 rcZeichnen.left = stZeichengrosse.cx; rcZeichnen.right = ptCaret.x;
															 ScrollWindow(hWndElement, stZeichengrosse.cx *-1, 0, &rcZeichnen, nullptr);
															 rcZeichnen.left = rcZeichnen.right - stZeichengrosse.cx; 
															 lTextPos += stZeichengrosse.cx;
														 }
													 }
													 ulZeichenPos++;
													 UpdateFenster(&rcZeichnen, true, false);
												 } 
											 }
											 else{ 
 												 if(cSelect){
													 if(cSelect > 0){ ulZeichenPos = ulSelectPos;
														 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
														 ptCaret.x = stZeichengrosse.cx;	
													 }
													 DeSelect();
												 }
												 if(ZeichenMaske_Einfugen(wParam)){
													 vasInhalt->Delete(ulZeichenPos, 1); vasInhalt->Insert((char*)&wParam, ulZeichenPos++);
													 if(ucSchriftausrichtung & TXA_LINKS) ptCaret.x = 0;	
													 else if(ucSchriftausrichtung & TXA_RECHTS){
														 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
														 ptCaret.x = lBreite - stZeichengrosse.cx;
													 }
													 else{
														 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
														 ptCaret.x = (lBreite - stZeichengrosse.cx) / 2;
													 }
													 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
													 ptCaret.x += stZeichengrosse.cx;
													 UpdateFenster(nullptr, true, false);
												 }
											 }
											 ReleaseDC(hWndElement, hdc);
										 }
										 SetCaretPos(ptCaret.x, ptCaret.y);
										 ThreadSicher_Ende();
										 PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), WM_CHAR);
										 break;
 }
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COEditZeile::WM_Command(WPARAM wParam)
{
 HDC hdc; VMBLOCK vbZeichen; HGLOBAL hGlobal; char* pcAblage; ULONG ulZeichen; SIZE stZeichengrosse; RECT rcZeichnen;
 switch(LOWORD(wParam)){
		case IDM_KOPIEREN     : ThreadSicher_Anfang();
			                      OpenClipboard(hWndElement); EmptyClipboard(); 
														if(cSelect > 0) ulZeichen = vasInhalt->SubString(vbZeichen, ulSelectPos + 1, ulZeichenPos);
														else ulZeichen = vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulSelectPos);
														hGlobal = GlobalAlloc(GMEM_MOVEABLE, ulZeichen + 1); 
														pcAblage = (char*)GlobalLock(hGlobal); 
														MemCopy(pcAblage, vbZeichen, ulZeichen); VMFrei(vbZeichen);
														pcAblage[ulZeichen] = 0;
														GlobalUnlock(hGlobal); 
										 				SetClipboardData(CF_TEXT, hGlobal); CloseClipboard();
														ThreadSicher_Ende();
			                      return false;
		case IDM_AUSSCHNEIDEN : ThreadSicher_Anfang();
														if(vasZeichenMaske->Length() || !ucZeichenVorgabe){ ThreadSicher_Ende(); return false; }
			                      OpenClipboard(hWndElement); EmptyClipboard(); 
														DeSelect();
														hdc = GetDC(hWndElement);	SelectObject(hdc, hFont);
														GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
														if(ulSelectPos <= ulZeichenPos){
															ulZeichen = vasInhalt->SubString(vbZeichen, ulSelectPos + 1, ulZeichenPos);
                              vasInhalt->Delete(ulSelectPos, ulZeichen);
                              ulZeichenPos = ulSelectPos;
														}
														else{
															ulZeichen = vasInhalt->SubString(vbZeichen, ulZeichenPos + 1, ulSelectPos);
															vasInhalt->Delete(ulZeichenPos, ulZeichen);
														}

														hGlobal = GlobalAlloc(GMEM_MOVEABLE, ulZeichen + 1); 
														pcAblage = (char*)GlobalLock(hGlobal); 
														MemCopy(pcAblage, vbZeichen, ulZeichen); VMFrei(vbZeichen);
														pcAblage[ulZeichen] = 0;
														GlobalUnlock(hGlobal); 
										 				SetClipboardData(CF_TEXT, hGlobal); CloseClipboard(); 
	
														rcZeichnen.top = 0; rcZeichnen.bottom = lHohe;
														if(ucSchriftausrichtung & TXA_LINKS){
														  rcZeichnen.left = rcSelect.right;
 														  (stZeichengrosse.cx - lTextPos >= lBreite ? rcZeichnen.right = lBreite : rcZeichnen.right = stZeichengrosse.cx - lTextPos);
														  stZeichengrosse.cx = rcSelect.right - rcSelect.left;
														  if(ulSelectPos == ulZeichenPos) ptCaret.x -= stZeichengrosse.cx;
															if(rcZeichnen.left == rcZeichnen.right) UpdateFenster(&rcSelect, true, false);
														  else{
															  ScrollWindow(hWndElement, stZeichengrosse.cx *-1, 0, &rcZeichnen, nullptr);
															  if(stZeichengrosse.cx > rcZeichnen.right - rcZeichnen.left) rcZeichnen.left = rcSelect.left + rcZeichnen.right - rcZeichnen.left;
															  else rcZeichnen.left = lBreite - stZeichengrosse.cx;
																UpdateFenster(&rcZeichnen, true, false);
														  }
														}
														else if(ucSchriftausrichtung & TXA_RECHTS){
														  if(stZeichengrosse.cx - lTextPos > lBreite){
															  rcZeichnen.left = stZeichengrosse.cx - lTextPos - lBreite; 
															  stZeichengrosse.cx = rcSelect.right - rcSelect.left;
															  if(rcZeichnen.left > stZeichengrosse.cx){
																  rcZeichnen.left = rcSelect.right; rcZeichnen.right = lBreite;
																  if(ulSelectPos == ulZeichenPos) ptCaret.x -= stZeichengrosse.cx;
																  ScrollWindow(hWndElement, stZeichengrosse.cx *-1, 0, &rcZeichnen, nullptr);
																  rcZeichnen.left = lBreite - stZeichengrosse.cx;
																	UpdateFenster(&rcZeichnen, true, false);
															  }
															  else{
																  GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
																  lTextPos = stZeichengrosse.cx - lBreite;
																  GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
																  ptCaret.x = stZeichengrosse.cx - lTextPos;
																	UpdateFenster(nullptr, true, false);
															  }
														  }
														  else{
															  rcZeichnen.right = rcSelect.left;
															  (!lTextPos ? rcZeichnen.left = lBreite - stZeichengrosse.cx : rcZeichnen.left = 0);
															  stZeichengrosse.cx = rcSelect.right - rcSelect.left;
															  if(ulSelectPos != ulZeichenPos) ptCaret.x += stZeichengrosse.cx;
																if(rcZeichnen.left == rcZeichnen.right) UpdateFenster(&rcSelect, true, false);
															  else{ ScrollWindow(hWndElement, stZeichengrosse.cx, 0, &rcZeichnen, nullptr);
																  if(stZeichengrosse.cx > rcZeichnen.right - rcZeichnen.left){ 
																	  rcZeichnen.right = rcSelect.right - rcZeichnen.right + rcZeichnen.left;
																	  rcZeichnen.left = rcSelect.left;
																		UpdateFenster(&rcZeichnen, true, false);
																  }
															  }

															  if(lTextPos){
																  rcZeichnen.right = stZeichengrosse.cx;
																  (lTextPos <= stZeichengrosse.cx ? lTextPos = 0 : lTextPos -= stZeichengrosse.cx);
                                  UpdateFenster(&rcZeichnen, true, false);
															  }
														  }
														}
														else{
															GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
															ptCaret.x = (lBreite - stZeichengrosse.cx) / 2;
															(stZeichengrosse.cx > lBreite ? lTextPos = ptCaret.x *-1 : lTextPos = 0);
															GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
															ptCaret.x += stZeichengrosse.cx;
															UpdateFenster(nullptr, true, false);
														}
														ReleaseDC(hWndElement, hdc);
														SetCaretPos(ptCaret.x, ptCaret.y);
														ThreadSicher_Ende();
														return false;
		case IDM_EINFUGEN     :	ThreadSicher_Anfang();
			                      if(!IsClipboardFormatAvailable(CF_TEXT) || !ucZeichenVorgabe){ ThreadSicher_Ende(); return false; }
														if(vasZeichenMaske->Length()){ ThreadSicher_Ende(); return false; }
														hdc = GetDC(hWndElement);	SelectObject(hdc, hFont);
														if(cSelect) Select_Loschen(hdc);
														rcZeichnen.top = 0; rcZeichnen.bottom = lHohe;
														GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
														if(ucSchriftausrichtung & TXA_LINKS) rcZeichnen.right = stZeichengrosse.cx;
														else if(ucSchriftausrichtung & TXA_RECHTS) rcZeichnen.left = lBreite - stZeichengrosse.cx;
														rcZeichnen.top = stZeichengrosse.cx;

														OpenClipboard(hWndElement); 
														hGlobal = GetClipboardData(CF_TEXT);
														pcAblage = (char*)GlobalLock(hGlobal);
														ulZeichen = StrLength(pcAblage);
														GlobalUnlock(hGlobal); 
														CloseClipboard(); 

														GetTextExtentPoint32(hdc, pcAblage, ulZeichen, &stZeichengrosse);
                            if(stZeichengrosse.cx + rcZeichnen.top < lBreite || ucZeichenVorgabe & (1 << 4) && vasInhalt->Length() + ulZeichen < ulZeichen_max){
															vasInhalt->Insert(pcAblage, ulZeichenPos); rcZeichnen.top = 0;			
															if(ucSchriftausrichtung & TXA_LINKS){	
																if(ptCaret.x + stZeichengrosse.cx > lBreite){
																	rcZeichnen.left = ptCaret.x + stZeichengrosse.cx - lBreite; rcZeichnen.right = ptCaret.x;
																	ScrollWindow(hWndElement, rcZeichnen.left *-1, 0, &rcZeichnen, nullptr);
																	lTextPos += rcZeichnen.left;
																	rcZeichnen.left = ptCaret.x - rcZeichnen.left; rcZeichnen.right = rcZeichnen.left + stZeichengrosse.cx;
																}
																else{ 
																	rcZeichnen.left = ptCaret.x;
																	ScrollWindow(hWndElement, stZeichengrosse.cx, 0, &rcZeichnen, nullptr);
																	rcZeichnen.right = ptCaret.x + stZeichengrosse.cx;
																}
																ulZeichenPos += ulZeichen;
																UpdateFenster(&rcZeichnen, true, false);
																ptCaret.x = rcZeichnen.right;
															}
															else if(ucSchriftausrichtung & TXA_RECHTS){
																if(rcZeichnen.left < 0) rcZeichnen.left = 0;
																if(rcZeichnen.left < stZeichengrosse.cx) lTextPos += stZeichengrosse.cx - rcZeichnen.left;
																rcZeichnen.right = ptCaret.x;
																ScrollWindow(hWndElement, stZeichengrosse.cx *-1, 0, &rcZeichnen, nullptr);
																rcZeichnen.left = ptCaret.x - stZeichengrosse.cx;

																ulZeichenPos += ulZeichen;
																UpdateFenster(&rcZeichnen, true, false);
																ptCaret.x = rcZeichnen.right;
															}
															else{
																GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
																ptCaret.x = (lBreite - stZeichengrosse.cx) / 2;
																(stZeichengrosse.cx > lBreite ? lTextPos = ptCaret.x *-1 : lTextPos = 0);
																GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
																ptCaret.x += stZeichengrosse.cx;
																ulZeichenPos += ulZeichen;
																GetTextExtentPoint32(hdc, pcAblage, ulZeichen, &stZeichengrosse);
																ptCaret.x += stZeichengrosse.cx;
																if(ptCaret.x > lBreite || ptCaret.x < 0){
																	ulZeichenPos = lTextPos / lZeichen_mittel + 1;
																	GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
																	ptCaret.x = stZeichengrosse.cx - lTextPos;
																}
																UpdateFenster(nullptr, true, false);
															}
															ReleaseDC(hWndElement, hdc);
															SetCaretPos(ptCaret.x, ptCaret.y);
															ThreadSicher_Ende();
															return false;
														}
		default               : return true;
 }
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::WM_ContexMenu(LPARAM lParam)
{
 ThreadSicher_Anfang();
 if(vasZeichenMaske->Length()){
	 EnableMenuItem(hMenu, IDM_AUSSCHNEIDEN, MF_BYCOMMAND | MF_GRAYED);
	 EnableMenuItem(hMenu, IDM_EINFUGEN, MF_BYCOMMAND | MF_GRAYED);
 }
 else{
	 EnableMenuItem(hMenu, IDM_AUSSCHNEIDEN, MF_BYCOMMAND | MF_ENABLED);
	 EnableMenuItem(hMenu, IDM_EINFUGEN, MF_BYCOMMAND | MF_ENABLED);
 }

 POINT ptPosition;
 ptPosition.x = LOWORD(lParam); ptPosition.y = HIWORD(lParam);
 if(ptPosition.x == USHRT_MAX && ptPosition.y == USHRT_MAX) ClientToScreen(GetParent(hWndElement), &Position(ptPosition));
 TrackPopupMenuEx(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, ptPosition.x, ptPosition.y, hWndElement, nullptr);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::WM_MouseMove(WPARAM wParam, LPARAM lParam)
{
 if(hWndElement == GetFocus() && wParam == MK_LBUTTON){
	 ThreadSicher_Anfang();
	 if((short)LOWORD(lParam) < ptCaret.x - lZeichen_mittel) SendMessage(hWndElement, WM_KEYDOWN, VK_LEFT, NULL);
	 else if((short)LOWORD(lParam) > ptCaret.x + lZeichen_mittel) SendMessage(hWndElement, WM_KEYDOWN, VK_RIGHT, NULL);
	 ThreadSicher_Ende();
 }
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::WM_LButtonDown(WPARAM wParam, LPARAM lParam)
{
 SetCapture(hWndElement);
 ThreadSicher_Anfang();
 SetFocus(hWndElement);
 if(cSelect) DeSelect();

 SIZE stZeichengrosse;
 HDC hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
 ulZeichenPos = 0;
 if(vasInhalt->Length()){
	 if(lTextPos || ucSchriftausrichtung & TXA_LINKS){
		 do{ GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ++ulZeichenPos, &stZeichengrosse); }
		 while(stZeichengrosse.cx - lTextPos < LOWORD(lParam) && ulZeichenPos < vasInhalt->Length());
		 ptCaret.x = stZeichengrosse.cx - lTextPos;
		
		 if(vasZeichenMaske->Length()){
			 if(ZeichenMaske_FestRechts()){
		     GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
			   ptCaret.x = stZeichengrosse.cx;
			 }
		 }
	 }
	 else{
		 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
		 if(stZeichengrosse.cx > lBreite){
			 do{ GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ++ulZeichenPos, &stZeichengrosse); }
			 while(stZeichengrosse.cx < LOWORD(lParam) && ulZeichenPos < vasInhalt->Length());
			 ptCaret.x = stZeichengrosse.cx;
		 }
		 else if(ucSchriftausrichtung & TXA_RECHTS) ptCaret.x = lBreite - stZeichengrosse.cx;
		 else ptCaret.x = (lBreite - stZeichengrosse.cx) / 2;

		 if(LOWORD(lParam) > ptCaret.x){
			 do{ GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ++ulZeichenPos, &stZeichengrosse);}
			 while(stZeichengrosse.cx + ptCaret.x < LOWORD(lParam) && ulZeichenPos < vasInhalt->Length());
			 
			 if(vasZeichenMaske->Length()){
				 if(ZeichenMaske_FestRechts()) GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
			 }
			 ptCaret.x += stZeichengrosse.cx;
		 } 
	 }
 }
 ReleaseDC(hWndElement, hdc);
 SetCaretPos(ptCaret.x, ptCaret.y);

 if(pfnWM_LButtonDown) pfnWM_LButtonDown(this, wParam, lParam);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::WM_LButtonUp(WPARAM wParam, LPARAM lParam)
{
 ReleaseCapture();
 ThreadSicher_Anfang();
 if(pfnWM_LButtonUp) pfnWM_LButtonUp(this, wParam, lParam);
 else PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), WM_LBUTTONUP);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::WM_LButtonDBClick(WPARAM wParam, LPARAM lParam)
{
 ThreadSicher_Anfang();
 SelectAlles();
 if(pfnWM_LButtonDBClick) pfnWM_LButtonDBClick(this, wParam, lParam);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::SetzVerfugbar(bool bVerfugbar)
{
 if(bVerfugbar){ 
	 if(!IsWindowVisible(hWndElement)) ShowWindow(hWndElement, SW_SHOW);
	 if(!IsWindowEnabled(hWndElement)) EnableWindow(hWndElement, bVerfugbar);
	 else if(hWndElement == GetFocus()) SendMessage(hWndElement, WM_SETFOCUS, NULL, NULL);
 }
 else{ if(hWndElement == GetFocus()) SetFocus(GetParent(hWndElement)); EnableWindow(hWndElement, bVerfugbar); }
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COEditZeile::ZeichenVorgabe(WPARAM wParam)
{
 for(BYTE ucBit = 0; ucBit < 4; ucBit++){
	 switch(ucBit){
			case 0   : if(ucZeichenVorgabe & (1 << ucBit)){
				           if(wParam >= 0x41 && wParam <= 0x5a || wParam >= 0x61 && wParam <= 0x7a || wParam == 0x20) return true;
								 } break;
			case 1   : if(ucZeichenVorgabe & (1 << ucBit)){ 
									 if(wParam >= 0x30 && wParam <= 0x39) return true;
								 } break;
			case 2   : if(ucZeichenVorgabe & (1 << ucBit)){
									 if(wParam >= 0x80 && wParam <= 0xff) return true;
								 } break;
			case 3   : if(ucZeichenVorgabe & (1 << ucBit)){ 
									 if(wParam >= 0x20 && wParam <= 0x2f || wParam >= 0x3a && wParam <= 0x40 || 
											wParam >= 0x5b && wParam <= 0x60 || wParam >= 0x7b && wParam <= 0x7f) return true;
								 } break;
	 }
 }
 return false;
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COEditZeile::ZeichenMaske_Einfugen(WPARAM wParam)
{
 VMBLOCK vbZeichen_Maske; ULONG ulZeichen; BYTE ucFesteZeichen = 1;

 for(ulZeichen = 1; ulZeichen <= ulZeichenPos + ucFesteZeichen; ulZeichen++){
	 vasZeichenMaske->SubString(vbZeichen_Maske, ulZeichen, ulZeichen);
	 if(*(PBYTE)vbZeichen_Maske == 0x27){ VMFrei(vbZeichen_Maske);
		 if(ulZeichen == ulZeichenPos + ucFesteZeichen){
       ucFesteZeichen = (BYTE)vasZeichenMaske->SearchCharacters("'", ulZeichen, vasZeichenMaske->Length());
		   vasZeichenMaske->SubString(vbZeichen_Maske, ++ucFesteZeichen, ucFesteZeichen);
			 if(!ZeichenMaske_Einfugen_Prufen(wParam, vbZeichen_Maske)) return false;
       ulZeichenPos += ucFesteZeichen - ulZeichen - 2;
			 return true;
		 }
     ucFesteZeichen += 2;
		 ulZeichen = vasZeichenMaske->SearchCharacters("'", ulZeichen, vasZeichenMaske->Length());
	 }
	 else VMFrei(vbZeichen_Maske);
 }
 vasZeichenMaske->SubString(vbZeichen_Maske, --ulZeichen, ulZeichen);

 return ZeichenMaske_Einfugen_Prufen(wParam, vbZeichen_Maske);
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COEditZeile::ZeichenMaske_Einfugen_Prufen(WPARAM wParam, VMBLOCK vbZeichen_Maske)
{
 switch(*(PBYTE)vbZeichen_Maske){
		case 0x41 : if(wParam >= 0x41 && wParam <= 0x5a){ VMFrei(vbZeichen_Maske); return true; }
								break;
		case 0x61 : if(wParam >= 0x61 && wParam <= 0x7a){ VMFrei(vbZeichen_Maske); return true; }
								break;
		case 0x42 : if(wParam >= 0x20 && wParam <= 0x2f || wParam >= 0x3a && wParam <= 0x7d){	VMFrei(vbZeichen_Maske); return true; }
								break;
		case 0x62 : if(wParam >= 0x20 && wParam <= 0x2f || wParam >= 0x3a && wParam <= 0x40 ||
									 wParam >= 0x5b && wParam <= 0x60 || wParam >= 0x7b && wParam <= 0x7e){ VMFrei(vbZeichen_Maske); return true; }
								break;
		case 0x5a : VMFrei(vbZeichen_Maske); return true;
		case 0x39 : if(wParam >= 0x30 && wParam <= 0x39){ VMFrei(vbZeichen_Maske); return true; }
								break;
		case 0x38 : if(wParam >= 0x30 && wParam <= 0x39  || wParam >= 0x2b && wParam <= 0x2e){ VMFrei(vbZeichen_Maske); return true; }
								break;
		case 0x58	: if(wParam >= 0x30 && wParam <= 0x39 || wParam >= 0x41 && wParam <= 0x46 ||
								wParam >= 0x61 && wParam <= 0x66){ VMFrei(vbZeichen_Maske); return true; }
								break;
 }
 VMFrei(vbZeichen_Maske);
 return false;
}

//--------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COEditZeile::ZeichenMaske_FestRechts(void)
{
 VMBLOCK vbZeichen_Maske; ULONG ulZeichen; BYTE ucFesteZeichen = 1;
 for(ulZeichen = 1; ulZeichen < ulZeichenPos + ucFesteZeichen; ulZeichen++){
	 vasZeichenMaske->SubString(vbZeichen_Maske, ulZeichen, ulZeichen);
	 if(*(PBYTE)vbZeichen_Maske == 0x27){
		 VMFrei(vbZeichen_Maske);
		 ucFesteZeichen += 2;
		 ulZeichen = vasZeichenMaske->SearchCharacters("'", ulZeichen, vasZeichenMaske->Length());
	 }
	 else VMFrei(vbZeichen_Maske);
 }
 if(ulZeichenPos < ulZeichen - ucFesteZeichen){ ulZeichenPos = ulZeichen - ucFesteZeichen; return true; }
 return false;
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COEditZeile::ZeichenMaske_FestLinks(void)
{
 VMBLOCK vbZeichen_Maske; BYTE ucFesteZeichen = 1;

 for(ULONG ulZeichen = 1; ulZeichen <= ulZeichenPos + ucFesteZeichen; ulZeichen++){
	 vasZeichenMaske->SubString(vbZeichen_Maske, ulZeichen, ulZeichen);
	 if(*(PBYTE)vbZeichen_Maske == 0x27){ VMFrei(vbZeichen_Maske);
     ucFesteZeichen += 2;
		 ULONG ulSprungZeichen = vasZeichenMaske->SearchCharacters("'", ulZeichen, vasZeichenMaske->Length());
		 if(ulSprungZeichen == ulZeichenPos + ucFesteZeichen){ ulZeichenPos -= ulSprungZeichen - ulZeichen - 2; return true; }
		 ulZeichen = ulSprungZeichen;
	 }
	 else VMFrei(vbZeichen_Maske);
 }
 return false;
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COEditZeile::ZeichenMaske_Loschen(void)
{
 VMBLOCK vbZeichen_Maske; BYTE ucFesteZeichen = 1;

 for(ULONG ulZeichen = 1; ulZeichen <= ulZeichenPos + ucFesteZeichen; ulZeichen++){
	 vasZeichenMaske->SubString(vbZeichen_Maske, ulZeichen, ulZeichen);
	 if(*(PBYTE)vbZeichen_Maske == 0x27){ VMFrei(vbZeichen_Maske);
     ucFesteZeichen += 2;
		 ULONG ulSprungZeichen = vasZeichenMaske->SearchCharacters("'", ulZeichen, vasZeichenMaske->Length());
		 if(ulSprungZeichen == ulZeichenPos + ucFesteZeichen - 1){ ulZeichenPos -= --ulSprungZeichen - ulZeichen; return false; }
		 ulZeichen = ulSprungZeichen;
	 }
	 else VMFrei(vbZeichen_Maske);
 }
 return true;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::Text(char* pcText)
{
 ThreadSicher_Anfang();
 //SetFocus(GetParent(hWndElement));
 *vasInhalt = NULL; ulZeichenPos = 0;
 if(pcText){
	 if(vasZeichenMaske->Length() && StrLength(pcText)){ VMBLOCK vbZeichen_Maske, vbZeichen_Text; BYTE ucFesteZeichen = 0, ucZeichen; COStringA asText = pcText;
	   for(ULONG ulZeichen = 1; ulZeichen <= vasZeichenMaske->Length(); ulZeichen++){
			 vasZeichenMaske->SubString(vbZeichen_Maske, ulZeichen, ulZeichen);
			 asText.SubString(vbZeichen_Text, ulZeichen - ucFesteZeichen, ulZeichen - ucFesteZeichen);
			 switch(*(PBYTE)vbZeichen_Maske){
					case 0x41 : if(*(PBYTE)vbZeichen_Text >= 0x41 && *(PBYTE)vbZeichen_Text <= 0x5a) *vasInhalt += vbZeichen_Text;
											else if(ucZeichenVorgabe & 1 << 5) *vasInhalt += "_";
											else *vasInhalt += " ";
											break;
					case 0x61 : if(*(PBYTE)vbZeichen_Text >= 0x61 && *(PBYTE)vbZeichen_Text <= 0x7a) *vasInhalt += vbZeichen_Text;
											else if(ucZeichenVorgabe & 1 << 5) *vasInhalt += "_";
											else *vasInhalt += " ";
											break;
					case 0x42 : if(*(PBYTE)vbZeichen_Text >= 0x20 && *(PBYTE)vbZeichen_Text <= 0x2f || *(PBYTE)vbZeichen_Text >= 0x3a && *(PBYTE)vbZeichen_Text <= 0x7d)
						            *vasInhalt += vbZeichen_Text;
											else if(ucZeichenVorgabe & 1 << 5) *vasInhalt += "_";
											else *vasInhalt += " ";
											break;
					case 0x62 : if(*(PBYTE)vbZeichen_Text >= 0x20 && *(PBYTE)vbZeichen_Text <= 0x2f || *(PBYTE)vbZeichen_Text >= 0x3a && *(PBYTE)vbZeichen_Text <= 0x40 ||
												 *(PBYTE)vbZeichen_Text >= 0x5b && *(PBYTE)vbZeichen_Text <= 0x60 || *(PBYTE)vbZeichen_Text >= 0x7b && *(PBYTE)vbZeichen_Text <= 0x7e)
												 *vasInhalt += vbZeichen_Text;
											else if(ucZeichenVorgabe & 1 << 5) *vasInhalt += "_";
											else *vasInhalt += " ";
											break;
					case 0x5a : *vasInhalt += vbZeichen_Text;
		                  break;
					case 0x39 : if(*(PBYTE)vbZeichen_Text >= 0x30 && *(PBYTE)vbZeichen_Text <= 0x39) *vasInhalt += vbZeichen_Text;
											else if(ucZeichenVorgabe & 1 << 5) *vasInhalt += "_";
											else *vasInhalt += " ";
											break;
					case 0x38 : if(*(PBYTE)vbZeichen_Text >= 0x30 && *(PBYTE)vbZeichen_Text <= 0x39 || *(PBYTE)vbZeichen_Text >= 0x2b && *(PBYTE)vbZeichen_Text <= 0x2e) *vasInhalt += vbZeichen_Text;
											else if(ucZeichenVorgabe & 1 << 5) *vasInhalt += "_";
											else *vasInhalt += " ";
											break;
					case 0x27 : ucZeichen = (BYTE)(vasZeichenMaske->SearchCharacters("'", ulZeichen, vasZeichenMaske->Length()) - ulZeichen++);
						          vasZeichenMaske->SubString(vbZeichen_Maske, ulZeichen, ulZeichen + ucZeichen - 2);
                      *vasInhalt += vbZeichen_Maske;
											ucFesteZeichen += 2;
                      ulZeichen += ucZeichen - 1;
											break;  
					case 0x58 : if(*(PBYTE)vbZeichen_Text >= 0x30 && *(PBYTE)vbZeichen_Text <= 0x39 || *(PBYTE)vbZeichen_Text >= 0x41 && *(PBYTE)vbZeichen_Text <= 0x46 || 
												 *(PBYTE)vbZeichen_Text >= 0x61 && *(PBYTE)vbZeichen_Text <= 0x66)	*vasInhalt += vbZeichen_Text;
											else if(ucZeichenVorgabe & 1 << 5) *vasInhalt += "_";
											else *vasInhalt += " ";
											break;
			 }
			 VMFrei(vbZeichen_Maske); VMFrei(vbZeichen_Text);
		 }
	 }		
	 else *vasInhalt = pcText;
 }
 UpdateFenster(nullptr, true, false);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::DeSelect(void)
{
 if(cSelect){ cSelect = 0; UpdateFenster(&rcSelect, true, false); ShowCaret(hWndElement); }
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::Select_Loschen(HDC hdc)
{
 RECT rcZeichnen; SIZE stZeichengrosse;

 DeSelect();
 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
 if(ulSelectPos <= ulZeichenPos){ vasInhalt->Delete(ulSelectPos, ulZeichenPos - ulSelectPos); ulZeichenPos = ulSelectPos; }
 else vasInhalt->Delete(ulZeichenPos, ulSelectPos - ulZeichenPos);

 rcZeichnen.top = 0; rcZeichnen.bottom = lHohe;
 if(ucSchriftausrichtung & TXA_LINKS){
	 rcZeichnen.left = rcSelect.right;
 	 (stZeichengrosse.cx - lTextPos >= lBreite ? rcZeichnen.right = lBreite : rcZeichnen.right = stZeichengrosse.cx - lTextPos + 1);
   stZeichengrosse.cx = rcSelect.right - rcSelect.left;
	 if(ulSelectPos == ulZeichenPos) ptCaret.x -= stZeichengrosse.cx;
	 if(rcZeichnen.left == rcZeichnen.right){ UpdateFenster(&rcSelect, true, false); }
	 else{
		 ScrollWindow(hWndElement, stZeichengrosse.cx *-1, 0, &rcZeichnen, nullptr);
		 if(stZeichengrosse.cx > rcZeichnen.right - rcZeichnen.left) rcZeichnen.left = rcSelect.left + rcZeichnen.right - rcZeichnen.left;
		 else rcZeichnen.left = lBreite - stZeichengrosse.cx;
		 UpdateFenster(&rcZeichnen, true, false);
	 }
 }
 else if(ucSchriftausrichtung & TXA_RECHTS){
	 if(stZeichengrosse.cx - lTextPos > lBreite){
     rcZeichnen.left = stZeichengrosse.cx - lTextPos - lBreite; 
		 stZeichengrosse.cx = rcSelect.right - rcSelect.left;
		 if(rcZeichnen.left > stZeichengrosse.cx){
       rcZeichnen.left = rcSelect.right; rcZeichnen.right = lBreite;
			 if(ulSelectPos == ulZeichenPos) ptCaret.x -= stZeichengrosse.cx;
			 ScrollWindow(hWndElement, stZeichengrosse.cx *-1, 0, &rcZeichnen, nullptr);
       rcZeichnen.left = lBreite - stZeichengrosse.cx;
			 UpdateFenster(&rcZeichnen, true, false);
		 }
		 else{
			 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
       lTextPos = stZeichengrosse.cx - lBreite;
       GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
			 ptCaret.x = stZeichengrosse.cx - lTextPos;
			 UpdateFenster(nullptr, true, false);
		 }
	 }
	 else{
		 rcZeichnen.right = rcSelect.left;
		 (!lTextPos ? rcZeichnen.left = lBreite - stZeichengrosse.cx : rcZeichnen.left = 0);
		 stZeichengrosse.cx = rcSelect.right - rcSelect.left;
		 if(ulSelectPos != ulZeichenPos) ptCaret.x += stZeichengrosse.cx;
		 if(rcZeichnen.left == rcZeichnen.right){ UpdateFenster(&rcSelect, true, false); }
		 else{ ScrollWindow(hWndElement, stZeichengrosse.cx, 0, &rcZeichnen, nullptr);
			 if(stZeichengrosse.cx > rcZeichnen.right - rcZeichnen.left){ 
				 rcZeichnen.right = rcSelect.right - rcZeichnen.right + rcZeichnen.left;
				 rcZeichnen.left = rcSelect.left;
				 UpdateFenster(&rcZeichnen, true, false);
			 }
		 }

		 if(lTextPos){
			 rcZeichnen.right = stZeichengrosse.cx;
			 (lTextPos <= stZeichengrosse.cx ? lTextPos = 0 : lTextPos -= stZeichengrosse.cx);
			 UpdateFenster(&rcZeichnen, true, false);
		 }
	 }
 }
 else{
	 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
	 ptCaret.x = (lBreite - stZeichengrosse.cx) / 2;
	 (stZeichengrosse.cx > lBreite ? lTextPos = ptCaret.x *-1 : lTextPos = 0);
	 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
	 ptCaret.x += stZeichengrosse.cx;
	 UpdateFenster(nullptr, true, false);
 }
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::SelectSchriftfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
 ThreadSicher_Anfang();
 stSelectSchriftfarbe.ucRot = ucRot; stSelectSchriftfarbe.ucGrun = ucGrun; stSelectSchriftfarbe.ucBlau = ucBlau;
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::SelectHintergrundfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
 ThreadSicher_Anfang();
 stSelectHintergrundfarbe.ucRot = ucRot; stSelectHintergrundfarbe.ucGrun = ucGrun; stSelectHintergrundfarbe.ucBlau = ucBlau;
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::SetzZeichen_Max(unsigned long ulZeichen)
{
 ThreadSicher_Anfang();
 ulZeichen_max = ulZeichen;
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
unsigned long __vectorcall RePag::GUI::COEditZeile::Zeichen_Max(void)
{
 ThreadSicher_Anfang();
 ULONG ulMaxZeichen = ulZeichen_max;
 ThreadSicher_Ende();
 return ulMaxZeichen;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::Zeichenvorgabe(unsigned char ucZeichenVorgabeA)
{
 ThreadSicher_Anfang();
 ucZeichenVorgabe = ucZeichenVorgabeA;
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::Zeichenmaske(const char* pcZeichenmaske)
{
 ThreadSicher_Anfang();
 if(!pcZeichenmaske || !StrCompare(pcZeichenmaske, 1, "", 1)){ *vasZeichenMaske = NULL; ulZeichen_max = 0x7fffffff; }
 else if(*vasZeichenMaske != pcZeichenmaske){ VMBLOCK vbZeichen; ULONG ulZeichenLange;
	 *vasZeichenMaske = pcZeichenmaske; *vasInhalt = NULL;

	 for(ULONG ulZeichen = 1; ulZeichen <= vasZeichenMaske->Length(); ulZeichen++){
		 vasZeichenMaske->SubString(vbZeichen, ulZeichen, ulZeichen);
		 if(*(PBYTE)vbZeichen == 0x27){ VMFrei(vbZeichen); ulZeichen++;
			 ulZeichenLange = vasZeichenMaske->SearchCharacters("'", ulZeichen, vasZeichenMaske->Length());
			 vasZeichenMaske->SubString(vbZeichen, ulZeichen, ulZeichenLange - 1);
			 *vasInhalt += vbZeichen;
			 ulZeichen += ulZeichenLange - ulZeichen;
		 }
		 else if(ucZeichenVorgabe & 1 << 5) *vasInhalt += "_";
		 else *vasInhalt += " ";
		 VMFrei(vbZeichen);
	 }
	 ulZeichen_max = vasInhalt->Length();
 }
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
COStringA* __vectorcall RePag::GUI::COEditZeile::Zeichenmaske(COStringA* pasZeichenmaske)
{
 ThreadSicher_Anfang();
 *pasZeichenmaske = *vasZeichenMaske;
 ThreadSicher_Ende();
 return pasZeichenmaske;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::SelectAlles(void)
{
 if(hWndElement){ SIZE stZeichengrosse; 
   ThreadSicher_Anfang();
	 cSelect = 1; ulSelectPos = 0; ulZeichenPos = vasInhalt->Length();
	 HDC hdc = GetDC(hWndElement); SelectObject(hdc, hFont); 
	 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), vasInhalt->Length(), &stZeichengrosse);
	 ReleaseDC(hWndElement, hdc); HideCaret(hWndElement);

	 if(ucSchriftausrichtung & TXA_LINKS){ rcSelect.right = stZeichengrosse.cx + 1; rcSelect.left = 0; ptCaret.x = stZeichengrosse.cx; }
	 else if(ucSchriftausrichtung & TXA_RECHTS){ rcSelect.right = lBreite; ptCaret.x = lBreite;
		 (stZeichengrosse.cx > lBreite ? rcSelect.left = 0 : rcSelect.left = lBreite - stZeichengrosse.cx);
	 }
	 else{
		 if(stZeichengrosse.cx > lBreite){ rcSelect.left = 0; rcSelect.right = lBreite;}
		 else{ rcSelect.left = (lBreite - stZeichengrosse.cx) / 2; rcSelect.right = rcSelect.left + stZeichengrosse.cx; }
	 }
	 UpdateFenster(nullptr, true, false);
   ThreadSicher_Ende();
 }
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditZeile::SelectEntfernen(void)
{
 ThreadSicher_Anfang();
 if(cSelect) DeSelect();
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------