/****************************************************************************
	OEditBox.cpp
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
#include "OEditBox.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
#define SHIFTED 0x8000
#define _Zeile ((COStringA*)pvZeile)
#define _ZeileAktuell ((COStringA*)vliText->Element(pvZeileAktuell))
#define _ZeileErste ((COStringA*)vliText->Element(pvZeileErste))
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COEditBox* __vectorcall RePag::GUI::COEditBoxV(const char* pcFensterName, unsigned int uiIDElement)
{
 COEditBox* vEditBox = (COEditBox*)VMBlock(VMDialog(), sizeof(COEditBox));
 vEditBox->COEditBoxV(VMDialog(), pcFensterName, uiIDElement);
 return vEditBox;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COEditBox* __vectorcall RePag::GUI::COEditBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement)
{
 COEditBox* vEditBox = (COEditBox*)VMBlock(vmSpeicher, sizeof(COEditBox));
 vEditBox->COEditBoxV(vmSpeicher, pcFensterName, uiIDElement);
 return vEditBox;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_EditBox(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
	COEditBox* pEditBox;
	switch(uiMessage){
		case WM_CREATE        : ((COEditBox*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create(hWnd);
														return NULL;
		case WM_SIZE          : pEditBox = (COEditBox*)GetWindowLongPtr(hWnd, GWL_USERDATA);
														if(pEditBox) pEditBox->WM_Size(lParam);
														else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
														return NULL;
		case WM_VSCROLL       : ((COEditBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_VScroll(wParam);
														return NULL;
		case WM_HSCROLL       : ((COEditBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_HScroll(wParam);
														return NULL;
		case WM_SETFOCUS      : ((COEditBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_SetFocus();
														return NULL;
		case WM_KILLFOCUS     : ((COEditBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_KillFocus();
														return NULL;
		case WM_KEYDOWN       : ((COEditBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_KeyDown(wParam, lParam);
														return NULL;
		case WM_CHAR          :	((COEditBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_Char(wParam);
														return NULL;
		case WM_COMMAND       : pEditBox = (COEditBox*)GetWindowLongPtr(hWnd, GWL_USERDATA);
														if(!pEditBox->WM_Command(wParam)) return NULL;
														else if(pEditBox->pfnWM_Command){
															pEditBox->ThreadSicher_Anfang();
															if(!pEditBox->pfnWM_Command(pEditBox, wParam)){ pEditBox->ThreadSicher_Ende(); return NULL; }
															pEditBox->ThreadSicher_Ende();
														}
														else PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
														break;
		case WM_CONTEXTMENU   : ((COEditBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_ContexMenu(lParam);
														return NULL;
		case WM_MOUSEMOVE     : ((COEditBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_MouseMove(wParam, lParam);
														return NULL;
		case WM_LBUTTONDOWN   : ((COEditBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_LButtonDown(lParam);
														return NULL;
		case WM_LBUTTONUP     : ((COEditBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_LButtonUp(wParam, lParam);
														return NULL;
		case WM_MOUSEWHEEL    : ((COEditBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_MouseWheel(wParam, lParam);
														return NULL;
		case WM_PAINT         : ((COEditBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_Paint();
														return NULL;
		case WM_NCDESTROY     : pEditBox = (COEditBox*)GetWindowLongPtr(hWnd, GWL_USERDATA);
														if(pEditBox->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pEditBox->htEffekt_Timer, INVALID_HANDLE_VALUE);
														VMFreiV(pEditBox);
														return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditBox::COEditBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA)
{
 COTextBoxV(vmSpeicher, pcRePag_EditBox, pcFensterName, uiIDElementA);

 pvZeile = nullptr;
 lZeile = 0;

 pfnWM_Char_ShiftReturn = nullptr;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditBox::WM_SetFocus(void)
{
 ThreadSicher_Anfang();
 CreateCaret(hWndElement, nullptr, 0, lZeichen_hohe);
 if(!cSelect){
	 if(!vliText->Number()){ COStringA* vasZeile = COStringAV(vmSpeicher); vliText->ToEnd(vasZeile); pvZeile = vasZeile;
	   ptCaret.x = ptCaret.y = lTextPos = ulZeichenPos = lZeile = 0; }
	 else{ SCROLLINFO stScrollVert, stScrollHorz; stScrollVert.cbSize = stScrollHorz.cbSize = sizeof(SCROLLINFO); stScrollVert.fMask = stScrollHorz.fMask = SIF_POS;
     GetScrollInfo(hWndElement, SB_VERT, &stScrollVert); GetScrollInfo(hWndElement, SB_HORZ, &stScrollHorz);
		 if(!ptCaret.x && !ptCaret.y && !stScrollVert.nPos && !stScrollHorz.nPos){ lTextPos = ulZeichenPos = lZeile = 0; pvZeile = vliText->Element(lZeile); }
	 }
	 SetCaretPos(ptCaret.x, ptCaret.y);
	 ShowCaret(hWndElement);
 }
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditBox::WM_KeyDown(WPARAM wParam, LPARAM lParam)
{
 HDC hdc; VMBLOCK vbZeichen; SIZE stZeichengrosse; SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO); 
 long lZeilen; void* pvZeileTemp = nullptr; RECT rcZeichnen;
 switch(wParam){
		case VK_HOME   : ThreadSicher_Anfang();
			               stScrollInfo.fMask = SIF_ALL;
										 ptCaret.x = ulZeichenPos = lTextPos = 0;
										 GetScrollInfo(hWndElement, SB_HORZ, &stScrollInfo);
										 stScrollInfo.nPos = 0;
										 stScrollInfo.nPage = lRand_rechts / lZeichen_mittel;
										 stScrollInfo.nMax = lBreitesteZeile / lZeichen_mittel + 1;
										 if(stScrollInfo.nPos + (int)stScrollInfo.nPage >= stScrollInfo.nMax) ShowScrollBar(hWndElement, SB_HORZ, false);
										 SetScrollInfo(hWndElement, SB_HORZ, &stScrollInfo, true);	 
										 UpdateFenster(nullptr, true, false);
										 SetCaretPos(ptCaret.x, ptCaret.y);
										 ThreadSicher_Ende();
										 break;
		case VK_END    : ThreadSicher_Anfang();
			               hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
			               GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
										 ulZeichenPos = _Zeile->Length();
										 if(lRand_rechts > stZeichengrosse.cx - lTextPos) ptCaret.x = stZeichengrosse.cx - lTextPos;
										 else{ stScrollInfo.fMask = SIF_POS;		
											 stScrollInfo.nPos = (stZeichengrosse.cx - lRand_rechts) / lZeichen_mittel + 1;
                       lTextPos = stScrollInfo.nPos * lZeichen_mittel;
											 ptCaret.x = stZeichengrosse.cx - lTextPos;
											 SetScrollInfo(hWndElement, SB_HORZ, &stScrollInfo, true);
											 UpdateFenster(nullptr, true, false);
										 }
                     ReleaseDC(hWndElement, hdc); 
										 SetCaretPos(ptCaret.x, ptCaret.y);
										 ThreadSicher_Ende();
										 break;
		case VK_LEFT   : ThreadSicher_Anfang();
			               if(ulZeichenPos){
			                 hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
                       GetTextExtentPoint32(hdc, _Zeile->c_Str(), --ulZeichenPos, &stZeichengrosse);
											 rcZeichnen.right = ptCaret.x;
										   ptCaret.x = stZeichengrosse.cx - lTextPos;  

											 if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
												 if(cSelect < 0) rcSelect.left = ptCaret.x;
												 else if(cSelect > 0){
													 if(cSelect == 2 || ulSelectPos < ulZeichenPos) rcSelect.right = ptCaret.x;
													 else{ cSelect = 0; ShowCaret(hWndElement);}
												 }
												 else{ cSelect = -1;
													 ulSelectPos = ulZeichenPos + 1;
													 rcSelect.top = ptCaret.y; rcSelect.bottom = rcSelect.top + lZeichen_hohe;
 													 rcSelect.left = ptCaret.x; rcSelect.right = rcZeichnen.right;
													 HideCaret(hWndElement);
												 }
												 rcZeichnen.left = ptCaret.x;
												 rcZeichnen.top = ptCaret.y; rcZeichnen.bottom = rcZeichnen.top + lZeichen_hohe;
												 UpdateFenster(&rcZeichnen, true, false);
											 }
											 else if(cSelect) DeSelect();
                       ReleaseDC(hWndElement, hdc);

											 while(ptCaret.x < 0) SendMessage(hWndElement, WM_HSCROLL, SB_LINELEFT, NULL);
										 }
										 SetCaretPos(ptCaret.x, ptCaret.y);
										 ThreadSicher_Ende();
										 break;
		case VK_RIGHT  : ThreadSicher_Anfang();
			               if(ulZeichenPos < _Zeile->Length()){
											 hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
                       GetTextExtentPoint32(hdc, _Zeile->c_Str(), ++ulZeichenPos, &stZeichengrosse);
											 rcZeichnen.left = ptCaret.x;
										   ptCaret.x = stZeichengrosse.cx - lTextPos; 
										
											 if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){ 
												 if(cSelect > 0) rcSelect.right = ptCaret.x;
												 else if(cSelect < 0){
													 if(cSelect == -2 || ulSelectPos > ulZeichenPos) rcSelect.left = ptCaret.x;
													 else{ cSelect = 0; ShowCaret(hWndElement);}
												 }
												 else{ cSelect = 1;
													 ulSelectPos = ulZeichenPos - 1; 
													 rcSelect.top = ptCaret.y; rcSelect.bottom = rcSelect.top + lZeichen_hohe;
													 rcSelect.left = rcZeichnen.left; rcSelect.right = ptCaret.x;
													 HideCaret(hWndElement);
												 }											 
												 rcZeichnen.right = ptCaret.x;
												 rcZeichnen.top = ptCaret.y; rcZeichnen.bottom = rcZeichnen.top + lZeichen_hohe;
												 UpdateFenster(&rcZeichnen, true, false);
											 }
											 else if(cSelect) DeSelect();
                       ReleaseDC(hWndElement, hdc);

											 while(ptCaret.x > lRand_rechts  && ulZeichenPos <= _Zeile->Length()) SendMessage(hWndElement, WM_HSCROLL, SB_LINERIGHT, NULL);
										 }
										 SetCaretPos(ptCaret.x, ptCaret.y);
										 ThreadSicher_Ende();
										 break;
		case VK_UP     : ThreadSicher_Anfang();
			               if(lZeile){
			                 hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
			                 pvZeile = vliText->Element(--lZeile);
                       rcZeichnen.right = ulZeichenPos;
											 ulZeichenPos = 0;
											 do{ GetTextExtentPoint32(hdc, _Zeile->c_Str(), ulZeichenPos++, &stZeichengrosse); }
											 while(stZeichengrosse.cx < ptCaret.x + lTextPos  && ulZeichenPos <= _Zeile->Length());
											 ulZeichenPos--;
                       rcZeichnen.left = ptCaret.x;
											 ptCaret.x = stZeichengrosse.cx - lTextPos;

											 if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
												 if(cSelect < 0){ rcSelect.top = ptCaret.y - lZeichen_hohe;
													 if(cSelect == -1) cSelect--;  
												 }
												 else if(cSelect > 0){
													 if(cSelect == 1){ cSelect = -2; rcSelect.top -= lZeichen_hohe; rcSelect.left = ptCaret.x; }
													 else if(rcSelect.top < rcSelect.bottom - lZeichen_hohe * 2) rcSelect.bottom -= lZeichen_hohe;
													 else{ cSelect = 0; ShowCaret(hWndElement); }
												 }
												 else{ cSelect = -2;
													 ulSelectPos = rcZeichnen.right;
                           rcSelect.top = ptCaret.y - lZeichen_hohe; rcSelect.bottom = ptCaret.y + lZeichen_hohe;
													 rcSelect.left = ptCaret.x; rcSelect.right = rcZeichnen.left;
													 HideCaret(hWndElement);
												 }
												 rcZeichnen.left = 0; rcZeichnen.right = lRand_rechts;
												 rcZeichnen.top = ptCaret.y - lZeichen_hohe; rcZeichnen.bottom = ptCaret.y + lZeichen_hohe;
												 UpdateFenster(&rcZeichnen, true, false);
											 }
											 else if(cSelect) DeSelect();
                       ReleaseDC(hWndElement, hdc);				

											 if(ptCaret.y) ptCaret.y -= lZeichen_hohe;
											 else{ SendMessage(hWndElement, WM_VSCROLL, SB_LINEUP, NULL); ptCaret.y -= lZeichen_hohe; }
										 }
										 SetCaretPos(ptCaret.x, ptCaret.y);
										 ThreadSicher_Ende();
										 break;
		case VK_DOWN   : ThreadSicher_Anfang();
			               stScrollInfo.fMask = SIF_RANGE;
										 GetScrollInfo(hWndElement, SB_VERT, &stScrollInfo);
			               if(lZeile < stScrollInfo.nMax - 1){
                       hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
			                 pvZeile = vliText->Element(++lZeile);
											 rcZeichnen.right = ulZeichenPos;
											 ulZeichenPos = 0;
											 do{ GetTextExtentPoint32(hdc, _Zeile->c_Str(), ulZeichenPos++, &stZeichengrosse); }
											 while(stZeichengrosse.cx < ptCaret.x + lTextPos  && ulZeichenPos <= _Zeile->Length());
											 ulZeichenPos--;
											 rcZeichnen.left = ptCaret.x;
											 ptCaret.x = stZeichengrosse.cx - lTextPos;

											 if(GetKeyState(VK_SHIFT) & SHIFTED || !lParam){
												 if(cSelect > 0){ rcSelect.bottom = ptCaret.y + lZeichen_hohe * 2;
                           if(cSelect == 1) cSelect++;
												 }
												 else if(cSelect < 0){
													 if(cSelect == -1){ cSelect = 2; rcSelect.bottom += lZeichen_hohe; rcSelect.right = ptCaret.x; }
                           else if(rcSelect.top < rcSelect.bottom - lZeichen_hohe * 2) rcSelect.top += lZeichen_hohe;
													 else{ cSelect = 0; ShowCaret(hWndElement); }
												 }
												 else{ cSelect = 2;
													 ulSelectPos = rcZeichnen.right;
                           rcSelect.top = ptCaret.y; rcSelect.bottom = ptCaret.y + lZeichen_hohe * 2;
													 rcSelect.left = rcZeichnen.left; rcSelect.right = ptCaret.x;
													 HideCaret(hWndElement);
												 }
												 rcZeichnen.left = 0; rcZeichnen.right = lRand_rechts;
												 rcZeichnen.top = ptCaret.y; rcZeichnen.bottom = ptCaret.y + lZeichen_hohe * 2;
												 UpdateFenster(&rcZeichnen, true, false);
											 }
											 else if(cSelect) DeSelect();
											 ptCaret.y += lZeichen_hohe;

											 if(ptCaret.y >= lRand_unten) SendMessage(hWndElement, WM_VSCROLL, SB_LINEDOWN, NULL);
                       ReleaseDC(hWndElement, hdc);
										 }
										 SetCaretPos(ptCaret.x, ptCaret.y);
										 ThreadSicher_Ende();
										 break;
		case VK_DELETE : ThreadSicher_Anfang();
										 if(!ucZeichenVorgabe){ ThreadSicher_Ende(); break; }
										 if(ulZeichenPos == _Zeile->Length() && lZeile == vliText->Number() - 1){ ThreadSicher_Ende(); break; }
										 hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
										 if(cSelect) Select_Loschen(hdc);
										 else{
											 GetTextExtentPoint32(hdc, _Zeile->c_Str(), ulZeichenPos + 1, &stZeichengrosse);
											 if(stZeichengrosse.cx - lTextPos > lRand_rechts){ ReleaseDC(hWndElement, hdc); ThreadSicher_Ende(); break; }		
											 
											 pvZeile = vliText->IteratorToBegin();
											 for(lZeilen = 0; lZeilen <= lZeile; lZeilen++) vliText->NextElement(pvZeile, pvZeileTemp);
											 if(ulZeichenPos == ((COStringA*)vliText->Element(pvZeileTemp))->Length()){
												 *((COStringA*)vliText->Element(pvZeileTemp)) += *((COStringA*)vliText->Element(pvZeile));
												 VMFreiV((COStringA*)vliText->Element(pvZeile));
												 vliText->DeleteElement(pvZeile, pvZeileTemp, false);
												 pvZeile = vliText->Element(pvZeileTemp);
												 
												 GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
												 if(lBreitesteZeile < stZeichengrosse.cx){ lBreitesteZeile = stZeichengrosse.cx; SetzScrollHorz(stScrollInfo); }
												 SetzScrollVert(stScrollInfo);

												 rcZeichnen.left = 0; rcZeichnen.right = lRand_rechts;
												 rcZeichnen.top = ptCaret.y + lZeichen_hohe * 2;
												 rcZeichnen.bottom = stScrollInfo.nPage * lZeichen_hohe;
												 if(rcZeichnen.bottom > lRand_unten) rcZeichnen.bottom = lRand_unten;
												 if(rcZeichnen.top == rcZeichnen.bottom) rcZeichnen.bottom += lZeichen_hohe;
												 ScrollWindow(hWndElement, 0, lZeichen_hohe *-1, &rcZeichnen, nullptr);
												 rcZeichnen.left = ptCaret.x; 
												 rcZeichnen.top = ptCaret.y; rcZeichnen.bottom = rcZeichnen.top + lZeichen_hohe; 
											 }
											 else{
												 GetTextExtentPoint32(hdc, ((COStringA*)vliText->Element(pvZeileTemp))->c_Str(), ((COStringA*)vliText->Element(pvZeileTemp))->Length(), &stZeichengrosse);
												 rcZeichnen.right = stZeichengrosse.cx;
												 if(lBreitesteZeile == stZeichengrosse.cx){
													 ((COStringA*)vliText->Element(pvZeileTemp))->SubString(vbZeichen, ulZeichenPos + 1, ulZeichenPos + 1);
													 GetTextExtentPoint32(hdc, vbZeichen, 1, &stZeichengrosse); VMFrei(vbZeichen);
													 rcZeichnen.left = lBreitesteZeile - stZeichengrosse.cx;

													 pvZeile = vliText->IteratorToBegin();
													 while(pvZeile){
														 GetTextExtentPoint32(hdc, ((COStringA*)vliText->Element(pvZeile))->c_Str(), ((COStringA*)vliText->Element(pvZeile))->Length(), &stZeichengrosse);
														 if(rcZeichnen.left < stZeichengrosse.cx && pvZeile != pvZeileTemp) break;
														 vliText->NextElement(pvZeile);
													 }
													 if(!pvZeile) lBreitesteZeile = rcZeichnen.left;
													 else lBreitesteZeile = stZeichengrosse.cx;
													 SetzScrollHorz(stScrollInfo);
												 }

												 pvZeile = vliText->Element(pvZeileTemp);
												 GetTextExtentPoint32(hdc, _Zeile->c_Str(), ulZeichenPos + 1, &stZeichengrosse);
												 _Zeile->Delete(ulZeichenPos, 1);
 												 if(rcZeichnen.right - lTextPos < lRand_rechts) rcZeichnen.right -= lTextPos;
												 else rcZeichnen.right = lRand_rechts;
												 rcZeichnen.left = stZeichengrosse.cx - lTextPos;
												 rcZeichnen.top = ptCaret.y; rcZeichnen.bottom = ptCaret.y + lZeichen_hohe;
												 ScrollWindow(hWndElement, ptCaret.x - stZeichengrosse.cx + lTextPos, 0, &rcZeichnen, nullptr);
												 rcZeichnen.left = rcZeichnen.right - stZeichengrosse.cx + ptCaret.x + lTextPos;
											 }	
											 UpdateFenster(&rcZeichnen, true, false);
										 }
										 ReleaseDC(hWndElement, hdc);
										 SetCaretPos(ptCaret.x, ptCaret.y);
										 ThreadSicher_Ende();
										 break;
		case VK_PRIOR  : SendMessage(hWndElement, WM_VSCROLL, SB_PAGEUP, NULL); break;
		case VK_NEXT   : SendMessage(hWndElement, WM_VSCROLL, SB_PAGEDOWN, NULL); break;
 }
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditBox::WM_Char(WPARAM wParam)
{
 HDC hdc; VMBLOCK vbZeichen; SIZE stZeichengrosse; long lZeilen; void* pvZeileTemp = nullptr;
 RECT rcZeichnen; SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO); COStringA* vasZeile;
 switch(wParam){
 	 case VK_ESCAPE : ThreadSicher_Anfang();
		                if(pfnWM_Char_Escape) pfnWM_Char_Escape(this);
										ThreadSicher_Ende();
								    break;
	 case VK_TAB    : ThreadSicher_Anfang();
									  if(!ucZeichenVorgabe){ ThreadSicher_Ende(); break; }
		                rcZeichnen.right = 0;
			              do{ SendMessage(hWndElement, WM_CHAR, ' ', NULL); }
								    while(++rcZeichnen.right < 4);
										ThreadSicher_Ende();
								    break;
	 case VK_BACK   : ThreadSicher_Anfang();
									  if(!ucZeichenVorgabe){ ThreadSicher_Ende(); break;}
		                if(cSelect){ hdc = GetDC(hWndElement); SelectObject(hdc, hFont); Select_Loschen(hdc);
										  ReleaseDC(hWndElement, hdc); ThreadSicher_Ende(); break;}
										else if(ulZeichenPos){ 
											hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
											_Zeile->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
											GetTextExtentPoint32(hdc, vbZeichen, 1, &stZeichengrosse); VMFrei(vbZeichen);
											if(ptCaret.x < stZeichengrosse.cx){ ReleaseDC(hWndElement, hdc); ThreadSicher_Ende(); break; }	

											pvZeile = vliText->IteratorToBegin();
											for(lZeilen = 0; lZeilen <= lZeile; lZeilen++) vliText->NextElement(pvZeile, pvZeileTemp);
											GetTextExtentPoint32(hdc, ((COStringA*)vliText->Element(pvZeileTemp))->c_Str(), ((COStringA*)vliText->Element(pvZeileTemp))->Length(), &stZeichengrosse);
											rcZeichnen.right = stZeichengrosse.cx;
											if(lBreitesteZeile == stZeichengrosse.cx){
												((COStringA*)vliText->Element(pvZeileTemp))->SubString(vbZeichen, ulZeichenPos, ulZeichenPos);
												GetTextExtentPoint32(hdc, vbZeichen, 1, &stZeichengrosse); VMFrei(vbZeichen);
												rcZeichnen.left = lBreitesteZeile - stZeichengrosse.cx;

												pvZeile = vliText->IteratorToBegin();
												while(pvZeile){
													GetTextExtentPoint32(hdc, ((COStringA*)vliText->Element(pvZeile))->c_Str(), ((COStringA*)vliText->Element(pvZeile))->Length(), &stZeichengrosse);
													if(rcZeichnen.left < stZeichengrosse.cx && pvZeile != pvZeileTemp){ ThreadSicher_Ende(); break; }
													vliText->NextElement(pvZeile);
												}

												if(!pvZeile) lBreitesteZeile = rcZeichnen.left;
												else lBreitesteZeile = stZeichengrosse.cx;
												SetzScrollHorz(stScrollInfo);
											}

											pvZeile = vliText->Element(pvZeileTemp);
											_Zeile->Delete(--ulZeichenPos, 1);
											if(rcZeichnen.right - lTextPos < lRand_rechts) rcZeichnen.right -= lTextPos;
											else rcZeichnen.right = lRand_rechts;
											rcZeichnen.left = ptCaret.x; rcZeichnen.top = ptCaret.y; rcZeichnen.bottom = rcZeichnen.top + lZeichen_hohe;
											GetTextExtentPoint32(hdc, _Zeile->c_Str(), ulZeichenPos, &stZeichengrosse);
											ScrollWindow(hWndElement, stZeichengrosse.cx - ptCaret.x - lTextPos, 0, &rcZeichnen, nullptr);
											rcZeichnen.left = stZeichengrosse.cx - lTextPos;
											UpdateFenster(&rcZeichnen, true, false);
											ptCaret.x = stZeichengrosse.cx - lTextPos;
											ReleaseDC(hWndElement, hdc);
										}
										else{ stScrollInfo.fMask = SIF_POS;
											GetScrollInfo(hWndElement, SB_VERT, &stScrollInfo);
											if(lZeile && lZeile > stScrollInfo.nPos){ lZeile--;  
												pvZeile = vliText->IteratorToBegin();
												for(lZeilen = 0; lZeilen <= lZeile; lZeilen++) vliText->NextElement(pvZeile, pvZeileTemp);

												hdc = GetDC(hWndElement);	SelectObject(hdc, hFont);
												GetTextExtentPoint32(hdc, ((COStringA*)vliText->Element(pvZeileTemp))->c_Str(), ((COStringA*)vliText->Element(pvZeileTemp))->Length(), &stZeichengrosse);
												ulZeichenPos = ((COStringA*)vliText->Element(pvZeileTemp))->Length();
												if(lRand_rechts > stZeichengrosse.cx - lTextPos) ptCaret.x = stZeichengrosse.cx - lTextPos;
												else{
													stScrollInfo.nPos = (stZeichengrosse.cx - lRand_rechts) / lZeichen_mittel + 1;
													SetScrollInfo(hWndElement, SB_HORZ, &stScrollInfo, true);
													lTextPos = stScrollInfo.nPos * lZeichen_mittel;
													ptCaret.x = stZeichengrosse.cx - lTextPos;
													UpdateFenster(nullptr, true, false);
												}

												*((COStringA*)vliText->Element(pvZeileTemp)) += *((COStringA*)vliText->Element(pvZeile));
												VMFreiV((COStringA*)vliText->Element(pvZeile));
												vliText->DeleteElement(pvZeile, pvZeileTemp, false);
												pvZeile = vliText->Element(pvZeileTemp);

												GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
												if(lBreitesteZeile < stZeichengrosse.cx){	lBreitesteZeile = stZeichengrosse.cx;	SetzScrollHorz(stScrollInfo); }
												ReleaseDC(hWndElement, hdc);

												SetzScrollVert(stScrollInfo);

												rcZeichnen.left = 0; rcZeichnen.right = lRand_rechts;
												rcZeichnen.top = ptCaret.y + lZeichen_hohe;
												rcZeichnen.bottom = stScrollInfo.nPage * lZeichen_hohe;
												if(rcZeichnen.bottom > lRand_unten) rcZeichnen.bottom = lRand_unten;
												if(rcZeichnen.top == rcZeichnen.bottom) rcZeichnen.bottom += lZeichen_hohe;
												ScrollWindow(hWndElement, 0, lZeichen_hohe *-1, &rcZeichnen, nullptr);
												rcZeichnen.left = ptCaret.x; 
												rcZeichnen.bottom = ptCaret.y; rcZeichnen.top = rcZeichnen.bottom - lZeichen_hohe; 
												UpdateFenster(&rcZeichnen, true, false);
												ptCaret.y -= lZeichen_hohe;
											}
										}
										SetCaretPos(ptCaret.x, ptCaret.y);
										ThreadSicher_Ende();
								    break;
	 case VK_RETURN : ThreadSicher_Anfang();
		                if(pfnWM_Char_ShiftReturn && GetKeyState(VK_SHIFT) & SHIFTED) pfnWM_Char_ShiftReturn(this);
										else if(!ucZeichenVorgabe){ ThreadSicher_Ende(); break; }
										else{ hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
											if(cSelect) Select_Loschen(hdc);
											vasZeile = COStringAV(vmSpeicher);
											pvZeile = vliText->IteratorToBegin();
											if(pvZeile){
												for(lZeilen = 0; lZeilen <= lZeile; lZeilen++) vliText->NextElement(pvZeile, pvZeileTemp);
												if(ulZeichenPos != ((COStringA*)vliText->Element(pvZeileTemp))->Length()){
													((COStringA*)vliText->Element(pvZeileTemp))->SubString(vbZeichen, ulZeichenPos + 1, ((COStringA*)vliText->Element(pvZeileTemp))->Length());
													*vasZeile = vbZeichen; VMFrei(vbZeichen);
													((COStringA*)vliText->Element(pvZeileTemp))->Delete(ulZeichenPos, ((COStringA*)vliText->Element(pvZeileTemp))->Length() - ulZeichenPos);
												}
											}
											if(!pvZeile) pvZeile = vliText->ToEnd(vasZeile);
											else vliText->Insert(pvZeile, pvZeileTemp, vasZeile);
											pvZeile = vasZeile;
											
											lZeile++; ulZeichenPos = 0; 
											SetzScrollVert(stScrollInfo);

											GetTextExtentPoint32(hdc, vasZeile->c_Str(), vasZeile->Length(), &stZeichengrosse);
											if(pvZeileTemp){
												rcZeichnen.right = stZeichengrosse.cx;
												GetTextExtentPoint32(hdc, ((COStringA*)vliText->Element(pvZeileTemp))->c_Str(), ((COStringA*)vliText->Element(pvZeileTemp))->Length(), &stZeichengrosse);
												rcZeichnen.right += stZeichengrosse.cx;
												if(lBreitesteZeile == rcZeichnen.right)	BreitesteZeile(hdc);
											}
											else{	lBreitesteZeile = stZeichengrosse.cx;	SetzScrollHorz(stScrollInfo); }
											ReleaseDC(hWndElement, hdc); 

											lZeilen = lRand_unten - lZeichen_hohe * 2;
											if(!lTextPos && ptCaret.y <= lZeilen){
												rcZeichnen.top = ptCaret.y;	rcZeichnen.bottom = lZeilen + lZeichen_hohe;
												rcZeichnen.left = 0; rcZeichnen.right = lRand_rechts;
												ScrollWindow(hWndElement, 0, lZeichen_hohe, &rcZeichnen, NULL);
												rcZeichnen.bottom = ptCaret.y + lZeichen_hohe; rcZeichnen.left = ptCaret.x; 	
												UpdateFenster(&rcZeichnen, true, false);
												ptCaret.x = 0; ptCaret.y += lZeichen_hohe;
												rcZeichnen.left = 0; rcZeichnen.top = ptCaret.y; rcZeichnen.bottom = ptCaret.y + lZeichen_hohe;
												UpdateFenster(&rcZeichnen, true, false);
											}
											else{
												if(lTextPos) SendMessage(hWndElement, WM_KEYDOWN, VK_HOME, NULL);
												else{	rcZeichnen.left = ptCaret.x; rcZeichnen.right = lRand_rechts;
													rcZeichnen.top = ptCaret.y; rcZeichnen.bottom = ptCaret.y + lZeichen_hohe; 
													UpdateFenster(&rcZeichnen, true, false);
												}
												//if(ptCaret.y > rcClient.bottom){ SendMessage(hWndElement, WM_VSCROLL, SB_LINEDOWN, NULL); ptCaret.x = 0; }
												if(ptCaret.y > lZeilen){ SendMessage(hWndElement, WM_VSCROLL, SB_LINEDOWN, NULL);	ptCaret.x = 0; }
												ptCaret.y += lZeichen_hohe;
											}
											SetCaretPos(ptCaret.x, ptCaret.y);
										}		
										ThreadSicher_Ende();
		                break;
	 default        : ThreadSicher_Anfang();
		                if(ZeichenVorgabe(wParam)){
											hdc = GetDC(hWndElement);	SelectObject(hdc, hFont);
		                  if(cSelect) Select_Loschen(hdc);
										  (ulZeichenPos == _Zeile->Length() ? *_Zeile += (char*)&wParam : _Zeile->Insert((char*)&wParam, ulZeichenPos));

											GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
										  if(lBreitesteZeile < stZeichengrosse.cx){ lBreitesteZeile = stZeichengrosse.cx;	SetzScrollHorz(stScrollInfo); }

											ulZeichenPos++; 
											GetTextExtentPoint32(hdc, (LPSTR)&wParam, 1, &stZeichengrosse);
											while(ptCaret.x + stZeichengrosse.cx >= lRand_rechts){
                        GetScrollInfo(hWndElement, SB_HORZ, &stScrollInfo);
												stScrollInfo.nMax++; 
												SetScrollInfo(hWndElement, SB_HORZ, &stScrollInfo, false);
												SendMessage(hWndElement, WM_HSCROLL, SB_LINERIGHT, NULL);
											}
											//while(ptCaret.x + stZeichengrosse.cx > lRand_rechts) SendMessage(hWndElement, WM_HSCROLL, SB_LINERIGHT, NULL);
											rcZeichnen.top = ptCaret.y; rcZeichnen.bottom = ptCaret.y + lZeichen_hohe;
										  rcZeichnen.left = ptCaret.x + stZeichengrosse.cx; rcZeichnen.right = lRand_rechts - stZeichengrosse.cx;
											ScrollWindow(hWndElement, stZeichengrosse.cx, 0, &rcZeichnen, NULL);

											rcZeichnen.left = ptCaret.x; rcZeichnen.right = rcZeichnen.left + stZeichengrosse.cx;
										  ptCaret.x += stZeichengrosse.cx;
											UpdateFenster(&rcZeichnen, true, false);
											ReleaseDC(hWndElement, hdc);
										}
										SetCaretPos(ptCaret.x, ptCaret.y);
										ThreadSicher_Ende();
								    break;
 }
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COEditBox::WM_Command(WPARAM wParam)
{
 HDC hdc; VMBLOCK vbZeichen; HGLOBAL hGlobal; char* pcAblage; SIZE stZeichengrosse; RECT rcZeichnen; void* pvZeileAktuell = nullptr; void* pvZeileErste = nullptr;
 SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO); ULONG ulZeichen; long lZeilen; COStringA* vasZeile = nullptr;
 switch(LOWORD(wParam)){
		case IDM_KOPIEREN     : ThreadSicher_Anfang();
			                      *vasInhalt = ""; OpenClipboard(hWndElement); EmptyClipboard(); 
														switch(cSelect){
															case  1 : _Zeile->SubString(vasInhalt, ulSelectPos + 1, ulZeichenPos);
																				ulZeichen = vasInhalt->Length();
																	      break;
															case -1 : _Zeile->SubString(vasInhalt, ulZeichenPos + 1, ulSelectPos);
																				ulZeichen = vasInhalt->Length();
																	      break;
															case  2 : rcZeichnen.top = lZeile - (rcSelect.bottom - rcSelect.top) / lZeichen_hohe + 1;
																	      rcZeichnen.bottom = rcSelect.top + lZeichen_hohe;
																	      pvZeileAktuell = vliText->IteratorToBegin();
			                                  for(lZeilen = 0; lZeilen < rcZeichnen.top; lZeilen++) vliText->NextElement(pvZeileAktuell);
																				ulZeichen = _ZeileAktuell->SubString(vbZeichen, ulSelectPos + 1, _ZeileAktuell->Length());
                                        *vasInhalt += vbZeichen; VMFrei(vbZeichen);
                                        vliText->NextElement(pvZeileAktuell);
																				rcZeichnen.bottom += lZeichen_hohe;
																				*vasInhalt += "\n"; ulZeichen++;
																				while(pvZeileAktuell && rcZeichnen.bottom < rcSelect.bottom){
                                          ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, _ZeileAktuell->Length());
																					*vasInhalt += vbZeichen; VMFrei(vbZeichen);
                                          vliText->NextElement(pvZeileAktuell);
																					rcZeichnen.bottom += lZeichen_hohe;
																					*vasInhalt += "\n"; ulZeichen++;
																				}
                                        ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, ulZeichenPos);
																				*vasInhalt += vbZeichen; VMFrei(vbZeichen);
																	      break;
															case -2 : rcZeichnen.bottom = rcSelect.top + lZeichen_hohe;
																	      pvZeileAktuell = vliText->IteratorToBegin();
			                                  for(lZeilen = 0; lZeilen < lZeile; lZeilen++) vliText->NextElement(pvZeileAktuell);
																				ulZeichen = _ZeileAktuell->SubString(vbZeichen, ulZeichenPos + 1, _ZeileAktuell->Length());
                                        *vasInhalt += vbZeichen; VMFrei(vbZeichen);
                                        vliText->NextElement(pvZeileAktuell);
																				rcZeichnen.bottom += lZeichen_hohe;
																				*vasInhalt += "\n"; ulZeichen++;
																				while(pvZeileAktuell && rcZeichnen.bottom < rcSelect.bottom){
                                          ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, _ZeileAktuell->Length());
																					*vasInhalt += vbZeichen; VMFrei(vbZeichen);
                                          vliText->NextElement(pvZeileAktuell);
																					rcZeichnen.bottom += lZeichen_hohe;
																					*vasInhalt += "\n"; ulZeichen++;
																				}
                                        ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, ulSelectPos);
																				*vasInhalt += vbZeichen; VMFrei(vbZeichen);
																	      break;
															default  : CloseClipboard(); ThreadSicher_Ende(); return true;
														}

														hGlobal = GlobalAlloc(GMEM_MOVEABLE, ulZeichen + 1); 
														pcAblage = (char*)GlobalLock(hGlobal); 
														MemCopy(pcAblage, vasInhalt->c_Str(), ulZeichen); 
														pcAblage[ulZeichen] = 0;
														GlobalUnlock(hGlobal); 

										 				SetClipboardData(CF_TEXT, hGlobal); CloseClipboard(); 
			                      ThreadSicher_Ende(); return false;
		case IDM_AUSSCHNEIDEN : ThreadSicher_Anfang();
			                      if(!ucZeichenVorgabe){ ThreadSicher_Ende(); return false;}
			                      OpenClipboard(hWndElement);	EmptyClipboard(); 
														hdc = GetDC(hWndElement);	SelectObject(hdc, hFont);	
														switch(cSelect){
															case  1 : GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
																	      _Zeile->SubString(vasInhalt, ulSelectPos + 1, ulZeichenPos);
																				ulZeichen = vasInhalt->Length();
                                        _Zeile->Delete(ulSelectPos, ulZeichen);
                                        ulZeichenPos -= ulZeichen;

																				if(stZeichengrosse.cx == lBreitesteZeile) BreitesteZeile(hdc);

																				rcZeichnen.top = rcSelect.top; rcZeichnen.bottom = rcSelect.bottom;
																				rcZeichnen.left = rcSelect.right; rcZeichnen.right = lRand_rechts;

																				if(stZeichengrosse.cx - lTextPos < rcZeichnen.right) rcZeichnen.right = stZeichengrosse.cx - lTextPos;
																				GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichen, &stZeichengrosse);
																				ScrollWindow(hWndElement, stZeichengrosse.cx *-1, 0, &rcZeichnen, nullptr);
																				rcZeichnen.left = rcSelect.left + rcZeichnen.right - rcZeichnen.left;
																				cSelect = 0; ShowCaret(hWndElement);
																				UpdateFenster(&rcZeichnen, true, false);
																				ptCaret.x -= stZeichengrosse.cx;
																				SetCaretPos(ptCaret.x, ptCaret.y);
																	      break;
															case -1 : GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
																	      _Zeile->SubString(vasInhalt, ulZeichenPos + 1, ulSelectPos);
																	      ulZeichen = vasInhalt->Length();
															          _Zeile->Delete(ulZeichenPos, ulZeichen);

																				if(stZeichengrosse.cx == lBreitesteZeile) BreitesteZeile(hdc);

																				rcZeichnen.top = rcSelect.top; rcZeichnen.bottom = rcSelect.bottom;
																				rcZeichnen.left = rcSelect.right;	rcZeichnen.right = lRand_rechts;

																				if(stZeichengrosse.cx - lTextPos < rcZeichnen.right) rcZeichnen.right = stZeichengrosse.cx - lTextPos;
																				GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichen, &stZeichengrosse);
																				ScrollWindow(hWndElement, stZeichengrosse.cx *-1, 0, &rcZeichnen, nullptr);
																				rcZeichnen.left = rcSelect.left + rcZeichnen.right - rcZeichnen.left;
																				cSelect = 0; ShowCaret(hWndElement);
																				UpdateFenster(&rcZeichnen, true, false);
																	      break;
															case  2 : rcZeichnen.top = lZeile - (rcSelect.bottom - rcSelect.top) / lZeichen_hohe + 1;
																	      rcZeichnen.bottom = rcSelect.top + lZeichen_hohe;
																	      pvZeileAktuell = vliText->IteratorToBegin();
			                                  for(lZeilen = 0; lZeilen < rcZeichnen.top; lZeilen++) vliText->NextElement(pvZeileAktuell, pvZeileErste);
                                        ulZeichen = _ZeileAktuell->SubString(vbZeichen, ulSelectPos + 1, _ZeileAktuell->Length());
                                        *vasInhalt += vbZeichen; VMFrei(vbZeichen);
                                        _ZeileAktuell->Delete(ulSelectPos, ulZeichen);
																				rcZeichnen.bottom += lZeichen_hohe;
																				*vasInhalt += "\n"; ulZeichen++;
                                        vliText->NextElement(pvZeileAktuell, pvZeileErste);
																				while(pvZeileAktuell && rcZeichnen.bottom < rcSelect.bottom){
                                          ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, _ZeileAktuell->Length());
																					*vasInhalt += vbZeichen; VMFrei(vbZeichen);
																					VMFreiV((COStringA*)vliText->Element(pvZeileAktuell));
                                          vliText->DeleteElement(pvZeileAktuell, pvZeileErste, false);
																					vliText->NextElement(pvZeileAktuell);
																					rcZeichnen.bottom += lZeichen_hohe;
																					*vasInhalt += "\n"; ulZeichen++;
																				}
                                        ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, ulZeichenPos);
																				*vasInhalt += vbZeichen; VMFrei(vbZeichen);
																				_ZeileAktuell->SubString(vbZeichen, ulZeichenPos + 1, _ZeileAktuell->Length());
																				*((COStringA*)vliText->Element(pvZeileErste)) += vbZeichen; VMFrei(vbZeichen);
																				vliText->DeleteElement(pvZeileAktuell, pvZeileErste, false);

																				pvZeile = vliText->Element(pvZeileErste);
																				lZeilen = (rcSelect.bottom - rcSelect.top) / lZeichen_hohe - 1;
																				lZeile -= lZeilen;
																				ptCaret.y -= lZeichen_hohe * lZeilen;
																				SetzScrollVert(stScrollInfo);

																				GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
																				if(stZeichengrosse.cx > lBreitesteZeile){ lBreitesteZeile = stZeichengrosse.cx; SetzScrollHorz(stScrollInfo); }
																				else BreitesteZeile(hdc);

																				ulZeichenPos = ulSelectPos;
																				GetTextExtentPoint32(hdc, _Zeile->c_Str(), ulZeichenPos, &stZeichengrosse);
                                        ptCaret.x = stZeichengrosse.cx;
                                        SetCaretPos(ptCaret.x, ptCaret.y);
                             
																				GetClientRect(hWndElement, &rcZeichnen);
																				rcZeichnen.top = rcSelect.top;
																				cSelect = 0; ShowCaret(hWndElement);
																				UpdateFenster(&rcZeichnen, true, false);
																	      break;
															case -2 : rcZeichnen.bottom = rcSelect.top + lZeichen_hohe;
																	      pvZeileAktuell = vliText->IteratorToBegin();
			                                  for(lZeilen = 0; lZeilen < lZeile; lZeilen++) vliText->NextElement(pvZeileAktuell, pvZeileErste);
                                        ulZeichen = _ZeileAktuell->SubString(vbZeichen, ulZeichenPos + 1, _ZeileAktuell->Length());
                                        *vasInhalt += vbZeichen; VMFrei(vbZeichen);
                                        _ZeileAktuell->Delete(ulZeichenPos, ulZeichen);
																				rcZeichnen.bottom += lZeichen_hohe;
																				*vasInhalt += "\n"; ulZeichen++;
                                        vliText->NextElement(pvZeileAktuell, pvZeileErste);
																				while(pvZeileAktuell && rcZeichnen.bottom < rcSelect.bottom){
                                          ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, _ZeileAktuell->Length());
																					*vasInhalt += vbZeichen; VMFrei(vbZeichen);
																					VMFreiV((COStringA*)vliText->Element(pvZeileAktuell));
                                          vliText->DeleteElement(pvZeileAktuell, pvZeileErste, false);
																					vliText->NextElement(pvZeileAktuell);
																					rcZeichnen.bottom += lZeichen_hohe;
																					*vasInhalt += "\n"; ulZeichen++;
																				}
                                        ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, ulSelectPos);
																				*vasInhalt += vbZeichen; VMFrei(vbZeichen);
																				_ZeileAktuell->SubString(vbZeichen, ulSelectPos + 1, _ZeileAktuell->Length());
																				*((COStringA*)vliText->Element(pvZeileErste)) += vbZeichen; VMFrei(vbZeichen);
																				vliText->DeleteElement(pvZeileAktuell, pvZeileErste, false);

																				lZeilen = (rcSelect.bottom - rcSelect.top) / lZeichen_hohe - 1;
																				SetzScrollVert(stScrollInfo);

																				GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
																				if(stZeichengrosse.cx > lBreitesteZeile){ lBreitesteZeile = stZeichengrosse.cx; SetzScrollHorz(stScrollInfo);}
																				else BreitesteZeile(hdc);
                             
																				GetClientRect(hWndElement, &rcZeichnen);
																				rcZeichnen.top = rcSelect.top;
																				cSelect = 0; ShowCaret(hWndElement);
																				UpdateFenster(&rcZeichnen, true, false);
																	      break;
															default  : CloseClipboard(); ReleaseDC(hWndElement, hdc); ThreadSicher_Ende(); return true;
														}
											      ReleaseDC(hWndElement, hdc);

														hGlobal = GlobalAlloc(GMEM_MOVEABLE, ulZeichen + 1); 
														pcAblage = (char*)GlobalLock(hGlobal); 
														MemCopy(pcAblage, vasInhalt->c_Str(), ulZeichen);
														pcAblage[ulZeichen] = 0;
														GlobalUnlock(hGlobal); 
										 				SetClipboardData(CF_TEXT, hGlobal); CloseClipboard(); 	
														ThreadSicher_Ende(); return false;
		case IDM_EINFUGEN     :	ThreadSicher_Anfang();
			                      if(!IsClipboardFormatAvailable(CF_TEXT) || !ucZeichenVorgabe){ ThreadSicher_Ende(); return false;}
														if(!vliText->Number()){ COStringA* vasZeile = COStringAV(vmSpeicher); vliText->ToEnd(vasZeile); pvZeile = vasZeile; }
														hdc = GetDC(hWndElement);	SelectObject(hdc, hFont);
														if(cSelect) Select_Loschen(hdc);

														OpenClipboard(hWndElement); 
														hGlobal = GetClipboardData(CF_TEXT);
														*vasInhalt = (char*)GlobalLock(hGlobal);
														rcZeichnen.bottom = vasInhalt->Length();
														GlobalUnlock(hGlobal); 
														CloseClipboard(); 

														rcZeichnen.top = 0; ulZeichen = 1; 
														vliText->IteratorToBegin();
														do{ ulZeichen++;
															if((*vasInhalt)[++rcZeichnen.top] == 0x0A){
																if(!pvZeileAktuell){
                                  pvZeileAktuell = vliText->IteratorToBegin();
																  for(lZeilen = 0; lZeilen <= lZeile; lZeilen++) vliText->NextElement(pvZeileAktuell, pvZeileErste);

																	vasZeile = COStringAV(vmSpeicher);
																  if(ulZeichenPos != ((COStringA*)vliText->Element(pvZeileErste))->Length()){
																		_ZeileErste->SubString(vbZeichen, ulZeichenPos + 1, _ZeileErste->Length());
																		*vasZeile = vbZeichen; VMFrei(vbZeichen);
																		_ZeileErste->Delete(ulZeichenPos, _ZeileErste->Length() - ulZeichenPos);
																  }
																	if(!pvZeileAktuell) pvZeileAktuell = vliText->ToEnd(vasZeile);
																	else pvZeileAktuell = vliText->Insert(pvZeileAktuell, pvZeileErste, vasZeile);

																	vasInhalt->SubString(vbZeichen, rcZeichnen.top - ulZeichen + 2, rcZeichnen.top);
																	_Zeile->Insert(vbZeichen, ulZeichenPos); VMFrei(vbZeichen);
																	GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
																}
																else{
																	vasZeile = COStringAV(vmSpeicher);
																	vasInhalt->SubString(vasZeile, rcZeichnen.top - ulZeichen + 2, rcZeichnen.top);
																	vliText->Insert(pvZeileAktuell, pvZeileErste, vasZeile);
																	vliText->NextElement(pvZeileErste);
																	GetTextExtentPoint32(hdc, vasZeile->c_Str(), vasZeile->Length(), &stZeichengrosse);
																}		
																if(lBreitesteZeile < stZeichengrosse.cx){ lBreitesteZeile = stZeichengrosse.cx; SetzScrollHorz(stScrollInfo); }
																lZeile++;	ptCaret.y += lZeichen_hohe;
																ulZeichen = 0;
															}
														}
														while(rcZeichnen.top < rcZeichnen.bottom);

														if(pvZeileAktuell){
															vasInhalt->SubString(vbZeichen, rcZeichnen.top - ulZeichen + 2, rcZeichnen.top);
															_ZeileAktuell->Insert(vbZeichen, 0); VMFrei(vbZeichen);
															pvZeile = vliText->Element(pvZeileAktuell);

															GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
															if(lBreitesteZeile < stZeichengrosse.cx){ lBreitesteZeile = stZeichengrosse.cx; SetzScrollHorz(stScrollInfo); }
															SetzScrollVert(stScrollInfo);

															ulZeichenPos = --ulZeichen;
															GetTextExtentPoint32(hdc, _Zeile->c_Str(), ulZeichenPos, &stZeichengrosse);
															ptCaret.x = stZeichengrosse.cx;
															SetCaretPos(ptCaret.x, ptCaret.y);

															GetClientRect(hWndElement, &rcZeichnen);
															rcZeichnen.top = ptCaret.y - rcSelect.bottom + rcSelect.top + lZeichen_hohe * 2;
															ScrollWindow(hWndElement, 0, rcSelect.bottom - rcSelect.top - lZeichen_hohe, &rcZeichnen, nullptr);
															rcZeichnen.top -= lZeichen_hohe; rcZeichnen.bottom = rcZeichnen.top + rcSelect.bottom - rcSelect.top;
															UpdateFenster(&rcZeichnen, true, false);
														}
														else{ 
                              _Zeile->Insert(vasInhalt, ulZeichenPos);
															ulZeichenPos += --ulZeichen;	

															GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
															if(lBreitesteZeile < stZeichengrosse.cx){ lBreitesteZeile = stZeichengrosse.cx; SetzScrollHorz(stScrollInfo); }

															GetClientRect(hWndElement, &rcZeichnen);
															rcZeichnen.top = ptCaret.y; rcZeichnen.bottom = rcZeichnen.top + lZeichen_hohe;
															GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichen, &stZeichengrosse);			
															if(ptCaret.x + stZeichengrosse.cx <= rcZeichnen.right){
																rcZeichnen.left = ptCaret.x; rcZeichnen.right -= stZeichengrosse.cx;
																ScrollWindow(hWndElement, stZeichengrosse.cx, 0, &rcZeichnen, nullptr);
																rcZeichnen.right = ptCaret.x + stZeichengrosse.cx;
																UpdateFenster(&rcZeichnen, true, false);
																ptCaret.x = rcZeichnen.right;
																SetCaretPos(ptCaret.x, ptCaret.y);
															}
															else{ ptCaret.x += stZeichengrosse.cx;
															  while(ptCaret.x > rcZeichnen.right) SendMessage(hWndElement, WM_HSCROLL, SB_LINERIGHT, NULL);
															}
														}
														ReleaseDC(hWndElement, hdc);
														ThreadSicher_Ende(); return false;
		default               : return true;
 }
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditBox::WM_ContexMenu(LPARAM lParam)
{
 ThreadSicher_Anfang();
 if(!IsWindowEnabled(hWndElement)){
   EnableMenuItem(hMenu, IDM_AUSSCHNEIDEN, MF_BYCOMMAND | MF_GRAYED);
	 EnableMenuItem(hMenu, IDM_KOPIEREN, MF_BYCOMMAND | MF_GRAYED);
	 EnableMenuItem(hMenu, IDM_EINFUGEN, MF_BYCOMMAND | MF_GRAYED);
 }
 else{
   EnableMenuItem(hMenu, IDM_AUSSCHNEIDEN, MF_BYCOMMAND | MF_ENABLED);
	 EnableMenuItem(hMenu, IDM_KOPIEREN, MF_BYCOMMAND | MF_ENABLED);
	 EnableMenuItem(hMenu, IDM_EINFUGEN, MF_BYCOMMAND | MF_ENABLED);
 }

 POINT ptPosition;
 ptPosition.x = LOWORD(lParam); ptPosition.y = HIWORD(lParam);
 if(ptPosition.x == USHRT_MAX && ptPosition.y == USHRT_MAX) ClientToScreen(GetParent(hWndElement), &Position(ptPosition));
 TrackPopupMenuEx(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, ptPosition.x, ptPosition.y, hWndElement, nullptr); 
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditBox::WM_MouseMove(WPARAM wParam, LPARAM lParam)
{
 if(hWndElement == GetFocus() && wParam == MK_LBUTTON){
	 ThreadSicher_Anfang();
	 if((short)LOWORD(lParam) < ptCaret.x - lZeichen_mittel) SendMessage(hWndElement, WM_KEYDOWN, VK_LEFT, NULL);
	 else if((short)LOWORD(lParam) > ptCaret.x + lZeichen_mittel) SendMessage(hWndElement, WM_KEYDOWN, VK_RIGHT, NULL);

	 if((short)HIWORD(lParam) < ptCaret.y - lZeichen_hohe) SendMessage(hWndElement, WM_KEYDOWN, VK_UP, NULL);
	 else if((short)HIWORD(lParam) > ptCaret.y + lZeichen_hohe) SendMessage(hWndElement, WM_KEYDOWN, VK_DOWN, NULL);
	 ThreadSicher_Ende();
 }
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditBox::WM_LButtonDown(LPARAM lParam)
{
 SetCapture(hWndElement);
 ThreadSicher_Anfang();
 if(hWndElement != GetFocus()) SetFocus(hWndElement);
 if(cSelect) DeSelect();

 SIZE stZeichengrosse;
 HDC hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
 SCROLLINFO stScrollVert; stScrollVert.cbSize = sizeof(SCROLLINFO); stScrollVert.fMask = SIF_POS | SIF_RANGE;
 GetScrollInfo(hWndElement, SB_VERT, &stScrollVert);

 ptCaret.x = 0; ulZeichenPos = 0;
 ptCaret.y = HIWORD(lParam) / lZeichen_hohe * lZeichen_hohe;
 lZeile = ptCaret.y / lZeichen_hohe + stScrollVert.nPos;
 if(lZeile >= stScrollVert.nMax){ ptCaret.y -= (lZeile - stScrollVert.nMax + 1) * lZeichen_hohe; lZeile = stScrollVert.nMax - 1; }

 pvZeile = vliText->Element(lZeile);

 if(_Zeile->Length()){
	 do{ GetTextExtentPoint32(hdc, _Zeile->c_Str(), ++ulZeichenPos, &stZeichengrosse);}
	 while(stZeichengrosse.cx - lTextPos < LOWORD(lParam) && ulZeichenPos < _Zeile->Length());
	 ptCaret.x = stZeichengrosse.cx - lTextPos;
 }
 else ptCaret.x = 0;
 ReleaseDC(hWndElement, hdc);
 SetCaretPos(ptCaret.x, ptCaret.y);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
COStringA* __vectorcall RePag::GUI::COEditBox::Inhalt(COStringA* vasInhaltA)
{
 ThreadSicher_Anfang();
 *vasInhaltA = NULL;
 void* pvZeilen = vliText->IteratorToBegin();
 while(pvZeilen){
   *vasInhaltA += *((COStringA*)vliText->Element(pvZeilen));
	 *vasInhaltA += "\n";
   vliText->NextElement(pvZeilen);
 }
 if(vasInhaltA->Length() == 1 && (*vasInhaltA)[0] == 0x0A) *vasInhaltA = NULL;
 ThreadSicher_Ende();
 return vasInhaltA;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditBox::Select_Loschen(HDC hdc)
{
 VMBLOCK vbZeichen; SIZE stZeichengrosse; RECT rcZeichnen; void* pvZeileAktuell = nullptr; void* pvZeileErste = nullptr;
 SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO); ULONG ulZeichen; long lZeilen; COStringA* vasZeile = nullptr;
 *vasInhalt = NULL;

 switch(cSelect){
		case  1 : GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
			        _Zeile->SubString(vasInhalt, ulSelectPos + 1, ulZeichenPos);
							ulZeichen = vasInhalt->Length();
              _Zeile->Delete(ulSelectPos, ulZeichen);
              ulZeichenPos -= ulZeichen;

							if(stZeichengrosse.cx == lBreitesteZeile) BreitesteZeile(hdc);

							rcZeichnen.top = rcSelect.top; rcZeichnen.bottom = rcSelect.bottom;
							rcZeichnen.left = rcSelect.right; rcZeichnen.right = lRand_rechts;

							if(stZeichengrosse.cx - lTextPos < rcZeichnen.right) rcZeichnen.right = stZeichengrosse.cx - lTextPos;
							GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichen, &stZeichengrosse);
							ScrollWindow(hWndElement, stZeichengrosse.cx *-1, 0, &rcZeichnen, nullptr);
							rcZeichnen.left = rcSelect.left + rcZeichnen.right - rcZeichnen.left;
							cSelect = 0; ShowCaret(hWndElement);
							UpdateFenster(&rcZeichnen, true, false);
							ptCaret.x -= stZeichengrosse.cx;
							SetCaretPos(ptCaret.x, ptCaret.y);
			        break;
		case -1 : GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
			        _Zeile->SubString(vasInhalt, ulZeichenPos + 1, ulSelectPos);
			        ulZeichen = vasInhalt->Length();
	            _Zeile->Delete(ulZeichenPos, ulZeichen);

							if(stZeichengrosse.cx == lBreitesteZeile) BreitesteZeile(hdc);

							rcZeichnen.top = rcSelect.top; rcZeichnen.bottom = rcSelect.bottom;
							rcZeichnen.left = rcSelect.right;	rcZeichnen.right = lRand_rechts;

							if(stZeichengrosse.cx - lTextPos < rcZeichnen.right) rcZeichnen.right = stZeichengrosse.cx - lTextPos;
							GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichen, &stZeichengrosse);
							ScrollWindow(hWndElement, stZeichengrosse.cx *-1, 0, &rcZeichnen, nullptr);
							rcZeichnen.left = rcSelect.left + rcZeichnen.right - rcZeichnen.left;
							cSelect = 0; ShowCaret(hWndElement);
							UpdateFenster(&rcZeichnen, true, false);
			        break;
		case  2 : rcZeichnen.top = lZeile - (rcSelect.bottom - rcSelect.top) / lZeichen_hohe + 1;
			        rcZeichnen.bottom = rcSelect.top + lZeichen_hohe;
			        pvZeileAktuell = vliText->IteratorToBegin();
              for(lZeilen = 0; lZeilen < rcZeichnen.top; lZeilen++) vliText->NextElement(pvZeileAktuell, pvZeileErste);
              ulZeichen = _ZeileAktuell->SubString(vbZeichen, ulSelectPos + 1, _ZeileAktuell->Length());
              *vasInhalt += vbZeichen; VMFrei(vbZeichen);
              _ZeileAktuell->Delete(ulSelectPos, ulZeichen);
							rcZeichnen.bottom += lZeichen_hohe;
							*vasInhalt += "\n"; ulZeichen++;
              vliText->NextElement(pvZeileAktuell, pvZeileErste);
							while(pvZeileAktuell && rcZeichnen.bottom < rcSelect.bottom){
                ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, _ZeileAktuell->Length());
								*vasInhalt += vbZeichen; VMFrei(vbZeichen);
								VMFreiV((COStringA*)vliText->Element(pvZeileAktuell));
                vliText->DeleteElement(pvZeileAktuell, pvZeileErste, false);
								vliText->NextElement(pvZeileAktuell);
							  rcZeichnen.bottom += lZeichen_hohe;
								*vasInhalt += "\n"; ulZeichen++;
							}
              ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, ulZeichenPos);
							*vasInhalt += vbZeichen; VMFrei(vbZeichen);
							_ZeileAktuell->SubString(vbZeichen, ulZeichenPos + 1, _ZeileAktuell->Length());
							*((COStringA*)vliText->Element(pvZeileErste)) += vbZeichen; VMFrei(vbZeichen);
							vliText->DeleteElement(pvZeileAktuell, pvZeileErste, false);

							pvZeile = vliText->Element(pvZeileErste);
							lZeilen = (rcSelect.bottom - rcSelect.top) / lZeichen_hohe - 1;
							lZeile -= lZeilen;
							ptCaret.y -= lZeichen_hohe * lZeilen;
							SetzScrollVert(stScrollInfo);

							GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
							if(stZeichengrosse.cx > lBreitesteZeile){ lBreitesteZeile = stZeichengrosse.cx; SetzScrollHorz(stScrollInfo); }
							else BreitesteZeile(hdc);

							ulZeichenPos = ulSelectPos;
							GetTextExtentPoint32(hdc, _Zeile->c_Str(), ulZeichenPos, &stZeichengrosse);
              ptCaret.x = stZeichengrosse.cx;
              SetCaretPos(ptCaret.x, ptCaret.y);
 
							GetClientRect(hWndElement, &rcZeichnen);
							rcZeichnen.top = rcSelect.top;
							cSelect = 0; ShowCaret(hWndElement);
							UpdateFenster(&rcZeichnen, true, false);
			        break;
		case -2 : rcZeichnen.bottom = rcSelect.top + lZeichen_hohe;
			        pvZeileAktuell = vliText->IteratorToBegin();
              for(lZeilen = 0; lZeilen < lZeile; lZeilen++) vliText->NextElement(pvZeileAktuell, pvZeileErste);
              ulZeichen = _ZeileAktuell->SubString(vbZeichen, ulZeichenPos + 1, _ZeileAktuell->Length());
              *vasInhalt += vbZeichen; VMFrei(vbZeichen);
              _ZeileAktuell->Delete(ulZeichenPos, ulZeichen);
							rcZeichnen.bottom += lZeichen_hohe;
							*vasInhalt += "\n"; ulZeichen++;
              vliText->NextElement(pvZeileAktuell, pvZeileErste);
							while(pvZeileAktuell && rcZeichnen.bottom < rcSelect.bottom){
                ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, _ZeileAktuell->Length());
								*vasInhalt += vbZeichen; VMFrei(vbZeichen);
								VMFreiV((COStringA*)vliText->Element(pvZeileAktuell));
                vliText->DeleteElement(pvZeileAktuell, pvZeileErste, false);
								vliText->NextElement(pvZeileAktuell);
							  rcZeichnen.bottom += lZeichen_hohe;
								*vasInhalt += "\n"; ulZeichen++;
							}
              ulZeichen += _ZeileAktuell->SubString(vbZeichen, 1, ulSelectPos);
							*vasInhalt += vbZeichen; VMFrei(vbZeichen);
							_ZeileAktuell->SubString(vbZeichen, ulSelectPos + 1, _ZeileAktuell->Length());
							*((COStringA*)vliText->Element(pvZeileErste)) += vbZeichen; VMFrei(vbZeichen);
							vliText->DeleteElement(pvZeileAktuell, pvZeileErste, false);

							lZeilen = (rcSelect.bottom - rcSelect.top) / lZeichen_hohe - 1;
							SetzScrollVert(stScrollInfo);

							GetTextExtentPoint32(hdc, _Zeile->c_Str(), _Zeile->Length(), &stZeichengrosse);
							if(stZeichengrosse.cx > lBreitesteZeile){ lBreitesteZeile = stZeichengrosse.cx; SetzScrollHorz(stScrollInfo); }
							else BreitesteZeile(hdc);
 
							GetClientRect(hWndElement, &rcZeichnen);
							rcZeichnen.top = rcSelect.top;
							cSelect = 0; ShowCaret(hWndElement);
							UpdateFenster(&rcZeichnen, true, false);
			        break;
 }
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COEditBox::BreitesteZeile(HDC hdc)
{
 void* pvZeilen = vliText->IteratorToBegin(); long lZeilenbreite = 0; SIZE stZeichengrosse;
 while(pvZeilen){
	 GetTextExtentPoint32(hdc, ((COStringA*)vliText->Element(pvZeilen))->c_Str(), ((COStringA*)vliText->Element(pvZeilen))->Length(), &stZeichengrosse);
	 if(stZeichengrosse.cx == lBreitesteZeile) break;
	 else if(stZeichengrosse.cx > lZeilenbreite) lZeilenbreite = stZeichengrosse.cx;
	 vliText->NextElement(pvZeilen);
 }
 if(!pvZeilen){ SCROLLINFO stScrollHorz; stScrollHorz.cbSize = sizeof(SCROLLINFO); lBreitesteZeile = lZeilenbreite; SetzScrollHorz(stScrollHorz); }
}
//---------------------------------------------------------------------------------------------------------------------------------------