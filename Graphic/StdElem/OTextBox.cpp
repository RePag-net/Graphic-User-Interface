/****************************************************************************
	OTextBox.cpp
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
#include "OTextBox.h"

#define _Zeile ((COStringA*)vliText->Element(pvIterator))

//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COTextBox* __vectorcall RePag::GUI::COTextBoxV(const char* pcFensterName, unsigned int uiIDElement)
{
 COTextBox* vTextBox = (COTextBox*)VMBlock(VMDialog(), sizeof(COTextBox));
 vTextBox->COTextBoxV(VMDialog(), pcFensterName, uiIDElement);
 return vTextBox;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COTextBox* __vectorcall RePag::GUI::COTextBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement)
{
 COTextBox* vTextBox = (COTextBox*)VMBlock(vmSpeicher, sizeof(COTextBox));
 vTextBox->COTextBoxV(vmSpeicher, pcFensterName, uiIDElement);
 return vTextBox;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_TextBox(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
 COTextBox* pTextBox;
 switch(uiMessage){
		case WM_CREATE          : ((COTextBox*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create(hWnd);
															return NULL;
		case WM_SIZE            : pTextBox = (COTextBox*)GetWindowLongPtr(hWnd, GWL_USERDATA);
															if(pTextBox) pTextBox->WM_Size(lParam);
															else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
															return NULL;
		case WM_VSCROLL         : ((COTextBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_VScroll(wParam);
															return NULL;
		case WM_HSCROLL         : ((COTextBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_HScroll(wParam);
															return NULL;
		case WM_LBUTTONDOWN     : ((COTextBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_LButtonDown();
			                        return NULL;
		case WM_LBUTTONUP       : ((COTextBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_LButtonUp(wParam, lParam);
                              return NULL;
		case WM_MOUSEWHEEL      : ((COTextBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_MouseWheel(wParam, lParam);
			                        return NULL;
		case WM_PAINT           : ((COTextBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_Paint();
															return NULL;
		case WM_NCDESTROY       : pTextBox = (COTextBox*)GetWindowLongPtr(hWnd, GWL_USERDATA);
															if(pTextBox->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pTextBox->htEffekt_Timer, INVALID_HANDLE_VALUE);
			                        VMFreiV(pTextBox);
			                        return NULL;
 }
 return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTextBox::COTextBoxV(VMEMORY vmSpeicher, const char* pcKlassenName, const char* pcFensterName, unsigned int uiIDElementA)
{
 COEditZeileV(vmSpeicher, pcKlassenName, pcFensterName, uiIDElementA);

 vliText = COListV(vmSpeicher, false);
 lBreitesteZeile = 0;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTextBox::COTextBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA)
{
 COTextBoxV(vmSpeicher, pcRePag_TextBox, pcFensterName, uiIDElementA);
}
//---------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::GUI::COTextBox::COFreiV(void)
{
 vliText->DeleteList(true); VMFreiV(vliText);
 return ((COTextZeile*)this)->COFreiV();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTextBox::WM_Paint(void)
{
 ThreadSicher_Anfang();
 PAINTSTRUCT stPaint;
 BeginPaint(hWndElement, &stPaint);
 WM_Paint_Hintergrund(stPaint);
 
 if(vliText->Number()){ SCROLLINFO siZeile, siSpalte; siZeile.cbSize = siSpalte.cbSize = sizeof(SCROLLINFO); siZeile.fMask = siSpalte.fMask = SIF_ALL;
	 GetScrollInfo(hWndElement, SB_HORZ, &siSpalte);
	 GetScrollInfo(hWndElement, SB_VERT, &siZeile);

	 long lZeile = 0; 
	 RECT rcZeichnen; rcZeichnen.top = 0; rcZeichnen.left = stPaint.rcPaint.left; rcZeichnen.right = stPaint.rcPaint.right;
	 void* pvIterator = vliText->IteratorToBegin(); void* pvIterator_1 = nullptr;
	 while(lZeile++ < (long)siZeile.nPos) vliText->NextElement(pvIterator);
	 while(pvIterator && rcZeichnen.top < stPaint.rcPaint.top){ rcZeichnen.top += lZeichen_hohe; vliText->NextElement(pvIterator, pvIterator_1); }
	 if(!pvIterator) pvIterator = pvIterator_1;
	 if(rcZeichnen.top > stPaint.rcPaint.top){ pvIterator = pvIterator_1; rcZeichnen.top -= lZeichen_hohe;}
	 rcZeichnen.bottom = rcZeichnen.top;
	 
	 SelectObject(stPaint.hdc, hFont);
	 SetBkMode(stPaint.hdc, TRANSPARENT); SetTextColor(stPaint.hdc, RGB(stSchriftfarbe.ucRot, stSchriftfarbe.ucGrun, stSchriftfarbe.ucBlau));
	 if(!cSelect){ 
		 do{
			 rcZeichnen.bottom += lZeichen_hohe;
			 ExtTextOut(stPaint.hdc, siSpalte.nPos * lZeichen_mittel *-1, rcZeichnen.top, ETO_CLIPPED, &rcZeichnen, _Zeile->c_Str(), _Zeile->Length(), nullptr);
			 vliText->NextElement(pvIterator);
			 rcZeichnen.top += lZeichen_hohe;
		 }
		 while(pvIterator && rcZeichnen.top < stPaint.rcPaint.bottom);
	 }
	 else{
		 do{
			 rcZeichnen.bottom += lZeichen_hohe;
			 if(rcSelect.top <= rcZeichnen.top  && rcSelect.bottom >= rcZeichnen.bottom){ 
				 SetBkMode(stPaint.hdc, OPAQUE);
				 SetBkColor(stPaint.hdc, RGB(stSelectHintergrundfarbe.ucRot, stSelectHintergrundfarbe.ucGrun, stSelectHintergrundfarbe.ucBlau));
				 SetTextColor(stPaint.hdc, RGB(stSelectSchriftfarbe.ucRot, stSelectSchriftfarbe.ucGrun, stSelectSchriftfarbe.ucBlau));

				 if(rcSelect.left <= stPaint.rcPaint.left && rcSelect.right >= stPaint.rcPaint.right){
					 ExtTextOut(stPaint.hdc, siSpalte.nPos * lZeichen_mittel *-1, rcZeichnen.top, ETO_CLIPPED, &rcZeichnen, _Zeile->c_Str(), _Zeile->Length(), nullptr);
					 SetBkMode(stPaint.hdc, TRANSPARENT); SetTextColor(stPaint.hdc, RGB(stSchriftfarbe.ucRot, stSchriftfarbe.ucGrun, stSchriftfarbe.ucBlau));
				 }
				 else{ 
					 if(cSelect == -1 || cSelect == 1){
						 rcZeichnen.left = rcSelect.left; rcZeichnen.right = rcSelect.right; 
					 }
					 else if(cSelect == -2 || cSelect == 2){
						 if(rcSelect.top == rcZeichnen.top) rcZeichnen.left = rcSelect.left;
						 else if(rcSelect.bottom == rcZeichnen.bottom) rcZeichnen.right = rcSelect.right;
					 }
					 ExtTextOut(stPaint.hdc, siSpalte.nPos * lZeichen_mittel *-1, rcZeichnen.top, ETO_CLIPPED, &rcZeichnen, _Zeile->c_Str(), _Zeile->Length(), nullptr);
					 SetBkMode(stPaint.hdc, TRANSPARENT); SetTextColor(stPaint.hdc, RGB(stSchriftfarbe.ucRot, stSchriftfarbe.ucGrun, stSchriftfarbe.ucBlau));				 

					 if(stPaint.rcPaint.left < rcSelect.left && rcSelect.top == rcZeichnen.top){
						 rcZeichnen.right = rcSelect.left; rcZeichnen.left = stPaint.rcPaint.left;
						 ExtTextOut(stPaint.hdc, siSpalte.nPos * lZeichen_mittel *-1, rcZeichnen.top, ETO_CLIPPED, &rcZeichnen, _Zeile->c_Str(), _Zeile->Length(), nullptr);
						 rcZeichnen.left = 0; rcZeichnen.right = stPaint.rcPaint.right;		 
					 }
					 if(stPaint.rcPaint.right > rcSelect.right && rcSelect.bottom == rcZeichnen.bottom){
						 rcZeichnen.right = stPaint.rcPaint.right; rcZeichnen.left = rcSelect.right;
						 ExtTextOut(stPaint.hdc, siSpalte.nPos * lZeichen_mittel *-1, rcZeichnen.top, ETO_CLIPPED, &rcZeichnen, _Zeile->c_Str(), _Zeile->Length(), nullptr);
						 rcZeichnen.left = stPaint.rcPaint.left;
					 }
				 }
			 }
			 else ExtTextOut(stPaint.hdc, siSpalte.nPos * lZeichen_mittel *-1, rcZeichnen.top, ETO_CLIPPED, &rcZeichnen, _Zeile->c_Str(), _Zeile->Length(), nullptr);
			 
			 vliText->NextElement(pvIterator);
			 rcZeichnen.top += lZeichen_hohe;
		 }
		 while(pvIterator && rcZeichnen.top < stPaint.rcPaint.bottom);
	 }
 }
 EndPaint(hWndElement, &stPaint);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTextBox::WM_Create(HWND hWnd)
{
 ZeichenMetric(hWnd); 

 RECT rcClient; GetClientRect(hWnd, &rcClient);
 ucRahmenbreite = lHohe - rcClient.bottom;

 SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO); stScrollInfo.fMask = SIF_ALL;

 stScrollInfo.nPos = stScrollInfo.nTrackPos = stScrollInfo.nMin = 0;
 stScrollInfo.nMax = 1;
 stScrollInfo.nPage = rcClient.bottom / lZeichen_hohe;
 SetScrollInfo(hWnd, SB_VERT, &stScrollInfo, false);

 stScrollInfo.nPage = rcClient.right / lZeichen_mittel;
 SetScrollInfo(hWnd, SB_HORZ, &stScrollInfo, false);

 GetClientRect(hWnd, &rcClient);
 lRand_rechts = rcClient.right; lRand_unten = rcClient.bottom;

 if(vasInhalt->Length()) Text_Create(hWnd);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTextBox::WM_Size(LPARAM lParam)
{
 ThreadSicher_Anfang();
 WM_Size_Element(hWndElement, lParam);

 WORD wRand_unten = HIWORD(lParam), wRand_rechts = LOWORD(lParam);
 SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO); 
 if(wRand_unten && lRand_unten != wRand_unten){ lRand_unten = wRand_unten; SetzScrollVert(stScrollInfo);}

 if(wRand_rechts && lRand_rechts != wRand_rechts){ lRand_rechts = wRand_rechts; SetzScrollHorz(stScrollInfo);}
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTextBox::WM_VScroll(WPARAM wParam)
{
 SCROLLINFO stScrollVert; stScrollVert.cbSize = sizeof(SCROLLINFO); stScrollVert.fMask = SIF_ALL;  
 ThreadSicher_Anfang();
 GetScrollInfo(hWndElement, SB_VERT, &stScrollVert);
 int iPos_alt = stScrollVert.nPos;

 switch(LOWORD(wParam)){
		 case SB_LINEUP        : if(stScrollVert.nPos) stScrollVert.nPos--;
			                       break;
		 case SB_LINEDOWN      : if(stScrollVert.nPos + (int)stScrollVert.nPage <= stScrollVert.nMax) stScrollVert.nPos++;
			                       break;
		 case SB_PAGEUP        : if(int(stScrollVert.nPos - stScrollVert.nPage) >= 0) stScrollVert.nPos -= stScrollVert.nPage;
													   else stScrollVert.nPos = 0;
			                       break;
		 case SB_PAGEDOWN      : if(stScrollVert.nPos + stScrollVert.nPage < int(stScrollVert.nMax - stScrollVert.nPage)) stScrollVert.nPos += stScrollVert.nPage;
													   else stScrollVert.nPos = stScrollVert.nMax - stScrollVert.nPage + 1;
			                       break;
		 case SB_THUMBTRACK    : stScrollVert.nPos = stScrollVert.nTrackPos; break;
		 default               : break;
 }

 if(stScrollVert.nPos + 1 == iPos_alt || stScrollVert.nPos - 1 == iPos_alt){ RECT rcZeichnen;
   SetScrollInfo(hWndElement, SB_VERT, &stScrollVert, true);

	 if(lRand_rechts < lBreitesteZeile)  rcZeichnen.right = lRand_rechts;
	 else rcZeichnen.right = lBreitesteZeile;

	 rcZeichnen.left = 0; 
	 rcZeichnen.bottom = lRand_unten / lZeichen_hohe * lZeichen_hohe;
	 if(stScrollVert.nPos > iPos_alt){
		 rcZeichnen.top = lZeichen_hohe;
		 ScrollWindow(hWndElement, 0, lZeichen_hohe *-1, &rcZeichnen, nullptr);

		 rcZeichnen.top = rcZeichnen.bottom - lZeichen_hohe;
		 rcZeichnen.bottom = lRand_unten;
	   ptCaret.y -= lZeichen_hohe;
		 if(cSelect){ rcSelect.top -= lZeichen_hohe; rcSelect.bottom -= lZeichen_hohe;}
	 }
	 else{
		 rcZeichnen.top = 0;
		 ScrollWindow(hWndElement, 0, lZeichen_hohe, &rcZeichnen, nullptr);

		 rcZeichnen.bottom = rcZeichnen.top + lZeichen_hohe;
		 ptCaret.y += lZeichen_hohe;
		 if(cSelect){ rcSelect.top += lZeichen_hohe; rcSelect.bottom += lZeichen_hohe;}

		 if(stScrollVert.nPos && lRand_unten >= (long)stScrollVert.nPage * lZeichen_hohe){
			 stScrollVert.nPage++; SetScrollInfo(hWndElement, SB_VERT, &stScrollVert, true);
		 }
	 }
	 UpdateFenster(&rcZeichnen, true, false);
   if(GetFocus() == hWndElement) SetCaretPos(ptCaret.x, ptCaret.y);
 }
 else if(stScrollVert.nPos != iPos_alt){ RECT rcZeichnen;
   SetScrollInfo(hWndElement, SB_VERT, &stScrollVert, true);

 	 rcZeichnen.top = 0; rcZeichnen.left = 0; rcZeichnen.right = lRand_rechts; rcZeichnen.bottom = lRand_unten;
	 if(lRand_unten >= (long)stScrollVert.nPage * lZeichen_hohe) SetzScrollVert(stScrollVert);

   iPos_alt -= stScrollVert.nPos; iPos_alt *= lZeichen_hohe;
   ptCaret.y += iPos_alt;
	 if(cSelect){ rcSelect.top += iPos_alt; rcSelect.bottom += iPos_alt;}

	 UpdateFenster(&rcZeichnen, true, false);
	 if(GetFocus() == hWndElement) SetCaretPos(ptCaret.x, ptCaret.y);
 }
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTextBox::WM_HScroll(WPARAM wParam)
{
 SCROLLINFO stScrollHorz; stScrollHorz.cbSize = sizeof(SCROLLINFO); stScrollHorz.fMask = SIF_ALL;
 ThreadSicher_Anfang();
 GetScrollInfo(hWndElement, SB_HORZ, &stScrollHorz);
 int iPos_alt = stScrollHorz.nPos;

 switch(LOWORD(wParam)){
		 case SB_LINELEFT        : if(stScrollHorz.nPos) stScrollHorz.nPos--;
														   break;
		 case SB_LINERIGHT       : if(stScrollHorz.nMax >= (int)stScrollHorz.nPage + stScrollHorz.nPos) stScrollHorz.nPos++;
														   break;
		 case SB_PAGELEFT        : stScrollHorz.nPos -= stScrollHorz.nPage;
			                         if(stScrollHorz.nPos < 0) stScrollHorz.nPos = 0;
			                         break;
		 case SB_PAGERIGHT       : if(stScrollHorz.nPos + stScrollHorz.nPage < int(stScrollHorz.nMax - stScrollHorz.nPage)) stScrollHorz.nPos += stScrollHorz.nPage;
													     else stScrollHorz.nPos = stScrollHorz.nMax - stScrollHorz.nPage + 1;
			                         break;
		 case SB_THUMBTRACK      : stScrollHorz.nPos = stScrollHorz.nTrackPos; break;
		 default                 : break;
 }

 if(stScrollHorz.nPos + 1 == iPos_alt || stScrollHorz.nPos - 1 == iPos_alt){ RECT rcZeichnen;
	 SetScrollInfo(hWndElement, SB_HORZ, &stScrollHorz, true);
	 
	 rcZeichnen.top = 0; rcZeichnen.bottom = lRand_unten;
	 if(stScrollHorz.nPos > iPos_alt){
		 rcZeichnen.left = lZeichen_mittel;
		 (lRand_rechts < lBreitesteZeile ? rcZeichnen.right = lRand_rechts : rcZeichnen.right = lBreitesteZeile);
		 ScrollWindow(hWndElement, rcZeichnen.left *-1, 0, &rcZeichnen, nullptr);

		 rcZeichnen.left = rcZeichnen.right - rcZeichnen.left; rcZeichnen.right = lRand_rechts;
		 ptCaret.x -= lZeichen_mittel; lTextPos += lZeichen_mittel;
		 if(cSelect){ rcSelect.left -= lZeichen_mittel; rcSelect.right -= lZeichen_mittel;}
	 }
	 else{
		 rcZeichnen.left = 0;
		 (lRand_rechts < lBreitesteZeile ? rcZeichnen.right = lRand_rechts - lZeichen_mittel : rcZeichnen.right = lBreitesteZeile);
		 ScrollWindow(hWndElement, lZeichen_mittel, 0, &rcZeichnen, nullptr);

		 rcZeichnen.left = 0; rcZeichnen.right = lZeichen_mittel;
		 ptCaret.x += lZeichen_mittel; lTextPos -= lZeichen_mittel;
		 if(cSelect){ rcSelect.left += lZeichen_mittel; rcSelect.right += lZeichen_mittel;}

		 if(stScrollHorz.nPos && lRand_rechts >= (long)stScrollHorz.nPage * lZeichen_mittel){ 
			 stScrollHorz.nPage++; SetScrollInfo(hWndElement, SB_HORZ, &stScrollHorz, true);
		 }
	 }
	 UpdateFenster(&rcZeichnen, true, false);
	 if(GetFocus() == hWndElement) SetCaretPos(ptCaret.x, ptCaret.y);
 }
 else if(stScrollHorz.nPos != iPos_alt){ RECT rcZeichnen;
	 SetScrollInfo(hWndElement, SB_HORZ, &stScrollHorz, true);

 	 rcZeichnen.top = 0; rcZeichnen.left = 0; rcZeichnen.right = lRand_rechts; rcZeichnen.bottom = lRand_unten;
	 if(lRand_rechts >= (long)stScrollHorz.nPage * lZeichen_mittel) SetzScrollHorz(stScrollHorz);

   iPos_alt -= stScrollHorz.nPos; iPos_alt *= lZeichen_mittel;
	 ptCaret.x += iPos_alt;
	 lTextPos -= iPos_alt;
	 if(cSelect){ rcSelect.left += iPos_alt; rcSelect.right += iPos_alt;}

	 UpdateFenster(&rcZeichnen, true, false);
	 if(GetFocus() == hWndElement) SetCaretPos(ptCaret.x, ptCaret.y);
 }
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTextBox::WM_LButtonDown(void)
{
 SetCapture(hWndElement);
 SetFocus(hWndElement);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTextBox::WM_MouseWheel(WPARAM wParam, LPARAM lParam)
{
 ThreadSicher_Anfang();
 RECT stClient; GetClientRect(hWndElement, &stClient);
 POINTS stPunkte = MAKEPOINTS(lParam);
 POINT stPunkt; stPunkt.x = stPunkte.x; stPunkt.y = stPunkte.y;
 ScreenToClient(hWndElement, &stPunkt);

 if(stPunkt.x > 0 && stPunkt.x < stClient.right && stPunkt.y > 0 && stPunkt.y < stClient.bottom){
   if(GET_WHEEL_DELTA_WPARAM(wParam) < 0) SendMessage(hWndElement, WM_VSCROLL, SB_LINEDOWN, NULL);
   else SendMessage(hWndElement, WM_VSCROLL, SB_LINEUP, NULL);
 }
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTextBox::SetzScrollHorz(SCROLLINFO& stScrollHorz)
{
 stScrollHorz.fMask = SIF_ALL;
 GetScrollInfo(hWndElement, SB_HORZ, &stScrollHorz);
 stScrollHorz.nPage = (UINT)(lRand_rechts / lZeichen_mittel);
 stScrollHorz.nMax = (int)(lBreitesteZeile / lZeichen_mittel);
 if(stScrollHorz.nPos + (int)stScrollHorz.nPage >= stScrollHorz.nMax) stScrollHorz.nPage = stScrollHorz.nMax - stScrollHorz.nPos + 1;
 //if(stScrollHorz.nPos + (int)stScrollHorz.nPage > stScrollHorz.nMax) stScrollHorz.nPage = stScrollHorz.nMax - stScrollHorz.nPos + 1;
 SetScrollInfo(hWndElement, SB_HORZ, &stScrollHorz, true);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTextBox::SetzScrollVert(SCROLLINFO& stScrollVert)
{
 stScrollVert.fMask = SIF_ALL;
 GetScrollInfo(hWndElement, SB_VERT, &stScrollVert);
 stScrollVert.nMax = vliText->Number();
 stScrollVert.nPage = lRand_unten / lZeichen_hohe;
 if(stScrollVert.nPos + (int)stScrollVert.nPage >= stScrollVert.nMax) stScrollVert.nPage = stScrollVert.nMax - stScrollVert.nPos + 1;
 else if(lRand_unten >= (long)stScrollVert.nPage * lZeichen_hohe) stScrollVert.nPage++;
 SetScrollInfo(hWndElement, SB_VERT, &stScrollVert, true);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTextBox::Text(char* pcText)
{
 ThreadSicher_Anfang(); 
 void* pvIterator = vliText->IteratorToBegin();
 //if(pvIterator){
 while(pvIterator){ VMFreiV((COStringA*)vliText->Element(pvIterator)); vliText->DeleteFirstElement(pvIterator, false); }
	// if(hWndElement){ SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO); stScrollInfo.fMask = SIF_ALL;
	//	 stScrollInfo.nPos = stScrollInfo.nTrackPos = stScrollInfo.nMin = 0;
	//	 stScrollInfo.nMax = 1;
	//	 stScrollInfo.nPage = lRand_unten / lZeichen_hohe;
	//	 SetScrollInfo(hWndElement, SB_VERT, &stScrollInfo, false);

	//	 stScrollInfo.nPage = lRand_rechts / lZeichen_mittel;
	//	 SetScrollInfo(hWndElement, SB_HORZ, &stScrollInfo, false);
	// }
 //}

 if(cSelect) DeSelect();
     
 if(pcText && StrLength(pcText)){
	 *vasInhalt = pcText;
	 if(vasInhalt->Length() && (*vasInhalt)[vasInhalt->Length() - 1] != 0x0A) *vasInhalt += "\n";

	 if(hWndElement){
		 SIZE stZeichengrosse; COStringA* vasZeile; 
		 HDC hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
		 cSelect = 0; lBreitesteZeile = 0;
		 ULONG ulLange = vasInhalt->Length(), ulZeichen = 0, ulZeichen_Rechts = 1;
		 do{
			 ulZeichen_Rechts++;
			 if((*vasInhalt)[++ulZeichen] == 0x0A){
				 vasZeile = COStringAV(vmSpeicher);
				 vasInhalt->SubString(vasZeile, ulZeichen - ulZeichen_Rechts + 2, ulZeichen);
				 vliText->ToEnd(vasZeile);
				 ulZeichen_Rechts = 0;
				 GetTextExtentPoint32(hdc, vasZeile->c_Str(), vasZeile->Length(), &stZeichengrosse);
				 if(lBreitesteZeile < stZeichengrosse.cx) lBreitesteZeile = stZeichengrosse.cx;
			 }
		 }
		 while(ulZeichen < ulLange);
		 ReleaseDC(hWndElement, hdc); 
		 *vasInhalt = NULL;

		 SCROLLINFO stScrollHorz, stScrollVert; stScrollHorz.cbSize = stScrollVert.cbSize = sizeof(SCROLLINFO); stScrollHorz.fMask = stScrollVert.fMask = SIF_ALL;
		 GetScrollInfo(hWndElement, SB_HORZ, &stScrollHorz);
		 stScrollHorz.nPos = 0;
		 stScrollHorz.nPage = (UINT)(lRand_rechts / lZeichen_mittel);
		 stScrollHorz.nMax = (int)(lBreitesteZeile / lZeichen_mittel);
		 if(stScrollHorz.nPos + (int)stScrollHorz.nPage >= stScrollHorz.nMax) stScrollHorz.nPage = stScrollHorz.nMax - stScrollHorz.nPos + 1;
		 SetScrollInfo(hWndElement, SB_HORZ, &stScrollHorz, true);
     //SetzScrollHorz(stScrollInfo);
		 
		 GetScrollInfo(hWndElement, SB_VERT, &stScrollVert);
 		 stScrollVert.nPos = 0;
		 stScrollVert.nMax = vliText->Number();
		 stScrollVert.nPage = lRand_unten / lZeichen_hohe;
		 if(stScrollVert.nPos + (int)stScrollVert.nPage >= stScrollVert.nMax) stScrollVert.nPage = stScrollVert.nMax - stScrollVert.nPos + 1;
		 else if(lRand_unten >= (long)stScrollVert.nPage * lZeichen_hohe) stScrollVert.nPage++;
		 SetScrollInfo(hWndElement, SB_VERT, &stScrollVert, true);
		 //SetzScrollVert(stScrollInfo);

	 }
 }
 else if(lBreitesteZeile){ SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO);
	 lBreitesteZeile = 0; SetzScrollHorz(stScrollInfo);
	 stScrollInfo.nPos = 0; stScrollInfo.nMax = 1;
	 stScrollInfo.nPage = lRand_unten / lZeichen_hohe;
	 SetScrollInfo(hWndElement, SB_VERT, &stScrollInfo, true);
 }
 ptCaret.x = ptCaret.y = 0;
 UpdateFenster(nullptr, true, false);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTextBox::Text_Create(HWND hWnd)
{
 hWndElement = hWnd;
 void* pvIterator = vliText->IteratorToBegin();
 while(pvIterator){ VMFreiV((COStringA*)vliText->Element(pvIterator)); vliText->DeleteFirstElement(pvIterator, false); }

 SIZE stZeichengrosse; COStringA* vasZeile; SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO);
 HDC hdc = GetDC(hWnd); SelectObject(hdc, hFont);
 cSelect = 0; lBreitesteZeile = 0;
 ULONG ulLange = vasInhalt->Length(), ulZeichen = 0, ulZeichen_Rechts = 1;
 do{
	 ulZeichen_Rechts++;
	 if((*vasInhalt)[++ulZeichen] == 0x0A){
		 vasZeile = COStringAV(vmSpeicher);
		 vasInhalt->SubString(vasZeile, ulZeichen - ulZeichen_Rechts + 2, ulZeichen);
		 vliText->ToEnd(vasZeile);
		 ulZeichen_Rechts = 0;
		 GetTextExtentPoint32(hdc, vasZeile->c_Str(), vasZeile->Length(), &stZeichengrosse);
		 if(lBreitesteZeile < stZeichengrosse.cx){ lBreitesteZeile = stZeichengrosse.cx; SetzScrollHorz(stScrollInfo); }
	 }
 }
 while(ulZeichen < ulLange);
 ReleaseDC(hWnd, hdc); 
 *vasInhalt = NULL;

 RECT rcClient; GetClientRect(hWnd, &rcClient);
 lRand_unten = rcClient.bottom;

 SetzScrollVert(stScrollInfo);
 stScrollInfo.nPos = 0;
 SetScrollInfo(hWnd, SB_VERT, &stScrollInfo, true);

 GetClientRect(hWnd, &rcClient);
 if(lRand_rechts != rcClient.right){ lRand_rechts = rcClient.right; SetzScrollHorz(stScrollInfo); }
 GetClientRect(hWnd, &rcClient);
 if(lRand_unten != rcClient.bottom){ lRand_unten = rcClient.bottom; SetzScrollVert(stScrollInfo); }
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTextBox::Text_NeueZeile(char* pcText)
{
 if(!pcText) return;
 
 if(!hWndElement){ *vasInhalt += pcText; *vasInhalt += "\n";}
 else{ SIZE stZeichengrosse; SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO);
   ThreadSicher_Anfang();
	 COStringA* vasZeile = COStringAV(vmSpeicher, pcText); vliText->ToEnd(vasZeile);
	 HDC hdc = GetDC(hWndElement); SelectObject(hdc, hFont);
	 GetTextExtentPoint32(hdc, vasZeile->c_Str(), vasZeile->Length(), &stZeichengrosse);
	 if(lBreitesteZeile < stZeichengrosse.cx){ lBreitesteZeile = stZeichengrosse.cx; SetzScrollHorz(stScrollInfo); }
	 ReleaseDC(hWndElement, hdc); 
	 SetzScrollVert(stScrollInfo);

	 RECT rcZeichnen; rcZeichnen.left = 0; rcZeichnen.right = lRand_rechts;
   GetScrollInfo(hWndElement, SB_VERT, &stScrollInfo);
	 if(!stScrollInfo.nPos && stScrollInfo.nMax < (int)stScrollInfo.nPage){ 
		 rcZeichnen.top = (stScrollInfo.nMax - 1) * lZeichen_hohe; rcZeichnen.bottom = rcZeichnen.top + lZeichen_hohe;
		 UpdateFenster(&rcZeichnen, true, false);
	 }
	 else{
		 rcZeichnen.top = lRand_unten - lZeichen_hohe; rcZeichnen.bottom = lRand_unten;
		 UpdateFenster(&rcZeichnen, true, false);
		 while(stScrollInfo.nMax > stScrollInfo.nPos + (int)stScrollInfo.nPage){ SendMessage(hWndElement, WM_VSCROLL, SB_PAGEDOWN, NULL); GetScrollInfo(hWndElement, SB_VERT, &stScrollInfo); }
	 }
	 SendMessage(hWndElement, WM_VSCROLL, SB_LINEDOWN, NULL);
	 
	 GetScrollInfo(hWndElement, SB_HORZ, &stScrollInfo);
	 while(stScrollInfo.nPos){ SendMessage(hWndElement, WM_HSCROLL, SB_PAGELEFT, NULL); GetScrollInfo(hWndElement, SB_HORZ, &stScrollInfo); }
	 ThreadSicher_Ende();
 }
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTextBox::DeSelect(void)
{
 if(cSelect == 2 || cSelect == -2){ rcSelect.left = 0; rcSelect.right = lRand_rechts; }
 cSelect = 0; UpdateFenster(&rcSelect, true, false); ShowCaret(hWndElement); 
}
//---------------------------------------------------------------------------------------------------------------------------------------
unsigned long __vectorcall RePag::GUI::COTextBox::Zeilenanzahl(void)
{
 ThreadSicher_Anfang();
 ULONG ulAnzahl = vliText->Number();
 ThreadSicher_Ende();
 return ulAnzahl;
}
//---------------------------------------------------------------------------------------------------------------------------------------
long __vectorcall RePag::GUI::COTextBox::BreitesteZeile(void)
{
 ThreadSicher_Anfang();
 long lZeile = lBreitesteZeile;
 ThreadSicher_Ende();
 return lZeile;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTextBox::Scroll_Anfang(void)
{
 ThreadSicher_Anfang();
 SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO); stScrollInfo.fMask = SIF_POS;
 stScrollInfo.nPos = 0;
 SetScrollInfo(hWndElement, SB_HORZ, &stScrollInfo, true);
 SetScrollInfo(hWndElement, SB_VERT, &stScrollInfo, true);

 ptCaret.x = ptCaret.y = ulZeichenPos = lTextPos = 0;
 UpdateFenster(nullptr, true, false);
 SetCaretPos(ptCaret.x, ptCaret.y);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTextBox::Scroll_Ende(void)
{
 ThreadSicher_Anfang();
 SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO); stScrollInfo.fMask = SIF_POS;
 stScrollInfo.nPos = 0;
 SetScrollInfo(hWndElement, SB_HORZ, &stScrollInfo, true);

 stScrollInfo.fMask = SIF_POS | SIF_RANGE;
 GetScrollInfo(hWndElement, SB_VERT, &stScrollInfo);
 stScrollInfo.nPos = stScrollInfo.nMax - lRand_unten / lZeichen_hohe;
 SetScrollInfo(hWndElement, SB_VERT, &stScrollInfo, true);

 ptCaret.x = ptCaret.y = ulZeichenPos = lTextPos = 0;
 UpdateFenster(nullptr, true, false);
 SetCaretPos(ptCaret.x, ptCaret.y);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTextBox::Scroll_Zeile(bool bAbwarts)
{
 if(bAbwarts) SendMessage(hWndElement, WM_VSCROLL, SB_LINEDOWN, NULL);
 else SendMessage(hWndElement, WM_VSCROLL, SB_LINEUP, NULL);
}
//---------------------------------------------------------------------------------------------------------------------------------------