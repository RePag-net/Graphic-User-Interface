/****************************************************************************
	OBildBox.cpp
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

#include "HImage.h"
#include "OBildBox.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COBildBox* __vectorcall RePag::GUI::COBildBoxV(const char* pcFensterName, unsigned int uiIDElement)
{
 COBildBox* vBildBox = (COBildBox*)VMBlock(VMDialog(), sizeof(COBildBox));
 vBildBox->COBildBoxV(VMDialog(), pcFensterName, uiIDElement);
 return vBildBox;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COBildBox* __vectorcall RePag::GUI::COBildBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement)
{
 COBildBox* vBildBox = (COBildBox*)VMBlock(vmSpeicher, sizeof(COBildBox));
 vBildBox->COBildBoxV(vmSpeicher, pcFensterName, uiIDElement);
 return vBildBox;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_BildBox(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
 COBildBox* pBildBox;
 switch(uiMessage){
		case WM_CREATE          : ((COBildBox*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create(hWnd);
															return NULL;
		case WM_SIZE            : pBildBox = (COBildBox*)GetWindowLongPtr(hWnd, GWL_USERDATA);
														  if(pBildBox) pBildBox->WM_Size(lParam);
															else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
															return NULL;
		case WM_VSCROLL         : ((COBildBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_VScroll(wParam);
															return NULL;
		case WM_HSCROLL         : ((COBildBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_HScroll(wParam);
															return NULL;
		case WM_MOUSEWHEEL      : ((COBildBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_MouseWheel(wParam, lParam);
			                        return NULL;
		case WM_PAINT           : ((COBildBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_Paint();
										          return NULL;
		case WM_NCDESTROY       : pBildBox = (COBildBox*)GetWindowLongPtr(hWnd, GWL_USERDATA);
															if(pBildBox->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pBildBox->htEffekt_Timer, INVALID_HANDLE_VALUE);
			                        VMFreiV(pBildBox);
			                        return NULL;
 }
 return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBildBox::COBildBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA)
{
 COGrafikV(vmSpeicher, pcRePag_BildBox, pcFensterName, uiIDElementA);

 COStringA asTemp = pcFensterName; asTemp += "_biBild";
 biBild = COBildV(vmSpeicher, asTemp.c_Str(), ++uiIDElementA); 
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBildBox::WM_Paint(void)
{
 ThreadSicher_Anfang();
 PAINTSTRUCT stPaint; 
 BeginPaint(hWndElement, &stPaint);
 WM_Paint_Hintergrund(stPaint);
 EndPaint(hWndElement, &stPaint);
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBildBox::WM_Create(HWND hWnd)
{
 lRand_rechts = lBreite; lRand_unten = lHohe;

 SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO); stScrollInfo.fMask = SIF_ALL;
 stScrollInfo.nPos = stScrollInfo.nTrackPos = stScrollInfo.nMin = 0;
 stScrollInfo.nMax = 1;
 stScrollInfo.nPage = lRand_unten / 10;
 SetScrollInfo(hWnd, SB_VERT, &stScrollInfo, false);

 stScrollInfo.nPage = lBreite / 10;
 SetScrollInfo(hWnd, SB_HORZ, &stScrollInfo, false);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBildBox::WM_Size(LPARAM lParam)
{
 ThreadSicher_Anfang();
 WM_Size_Element(hWndElement, lParam);
 long lBildbreite;
 if(LOWORD(lParam) > biBild->Breite(lBildbreite))	biBild->NeueFensterPosition((LOWORD(lParam) - lBildbreite) / 2, 0);
 else biBild->NeueFensterPosition(0, 0);

 SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO); 
 if(lRand_unten != HIWORD(lParam)){ lRand_unten = HIWORD(lParam); SetzScrollVert(stScrollInfo);}
 if(lRand_rechts != LOWORD(lParam)){ lRand_rechts = LOWORD(lParam); SetzScrollHorz(stScrollInfo);}
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBildBox::WM_VScroll(WPARAM wParam)
{
 SCROLLINFO stScrollVert; stScrollVert.cbSize = sizeof(SCROLLINFO); stScrollVert.fMask = SIF_ALL;
 ThreadSicher_Anfang();
 GetScrollInfo(hWndElement, SB_VERT, &stScrollVert);
 int iVertPos = stScrollVert.nPos;

 switch(LOWORD(wParam)){
		 case SB_TOP        : stScrollVert.nPos = stScrollVert.nMin; break;
		 case SB_BOTTOM     : stScrollVert.nPos = stScrollVert.nMax; break;
		 case SB_LINEUP     : stScrollVert.nPos--; break;
		 case SB_LINEDOWN   : stScrollVert.nPos++; break;
		 case SB_PAGEUP     : stScrollVert.nPos -= stScrollVert.nPage; break;
		 case SB_PAGEDOWN   : stScrollVert.nPos += stScrollVert.nPage; break;
		 case SB_THUMBTRACK : stScrollVert.nPos = stScrollVert.nTrackPos; break;
		 default            : break;
 }

 stScrollVert.fMask = SIF_POS;
 SetScrollInfo(hWndElement, SB_VERT, &stScrollVert, true);
 GetScrollInfo(hWndElement, SB_VERT, &stScrollVert);

 if(stScrollVert.nPos != iVertPos) ScrollWindow(hWndElement, 0, 10 * (iVertPos - stScrollVert.nPos), nullptr, nullptr);
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBildBox::WM_HScroll(WPARAM wParam)
{
 SCROLLINFO stScrollHorz; stScrollHorz.cbSize = sizeof(SCROLLINFO); stScrollHorz.fMask = SIF_ALL;
 ThreadSicher_Anfang();
 GetScrollInfo(hWndElement, SB_HORZ, &stScrollHorz);
 int iHorzPos = stScrollHorz.nPos;

 switch(LOWORD(wParam)){
		 case SB_LINELEFT      : stScrollHorz.nPos--; break;
		 case SB_LINERIGHT     : stScrollHorz.nPos++; break;
		 case SB_PAGELEFT      : stScrollHorz.nPos -= stScrollHorz.nPage; break;
		 case SB_PAGERIGHT     : stScrollHorz.nPos += stScrollHorz.nPage; break;
		 case SB_THUMBPOSITION : stScrollHorz.nPos = stScrollHorz.nTrackPos; break;
		 default               : break;
 }

 stScrollHorz.fMask = SIF_POS;
 SetScrollInfo(hWndElement, SB_HORZ, &stScrollHorz, true);
 GetScrollInfo(hWndElement, SB_HORZ, &stScrollHorz);

 if(stScrollHorz.nPos != iHorzPos) ScrollWindow(hWndElement, 10 * (iHorzPos - stScrollHorz.nPos), 0, nullptr, nullptr);
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBildBox::WM_MouseWheel(WPARAM wParam, LPARAM lParam)
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
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBildBox::SetzScrollHorz(SCROLLINFO& stScrollHorz)
{
 long lBildbreite;
 stScrollHorz.fMask = SIF_ALL;
 GetScrollInfo(hWndElement, SB_HORZ, &stScrollHorz);
 stScrollHorz.nPos = 0;
 stScrollHorz.nMax = biBild->Breite(lBildbreite) / 10;
 stScrollHorz.nPage = lRand_rechts / 10;
 SetScrollInfo(hWndElement, SB_HORZ, &stScrollHorz, true);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBildBox::SetzScrollVert(SCROLLINFO& stScrollVert)
{
 long lBildhohe;
 stScrollVert.fMask = SIF_ALL;
 GetScrollInfo(hWndElement, SB_VERT, &stScrollVert);
 stScrollVert.nPos = 0;
 stScrollVert.nMax = biBild->Hohe(lBildhohe) / 10;
 stScrollVert.nPage = lRand_unten / 10;
 SetScrollInfo(hWndElement, SB_VERT, &stScrollVert, true);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBildBox::ErstellBildInBox(long lHoheA, long lBreiteA, bool bMillimeter)
{
 if(bMillimeter){ lBildhohe = MMzuPixel(LOGPIXELSY, lHoheA); lBildbreite = MMzuPixel(LOGPIXELSX, lBreiteA);}
 else{ lBildhohe = lHoheA; lBildbreite = lBreiteA;}
 biBild->ErstellFenster(hWndElement, lBildhohe, lBildbreite, 0, 0);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBildBox::Bild_Hintergrundfarbe(BYTE ucRot, BYTE ucGrun, BYTE ucBlau)
{
 ThreadSicher_Anfang();
 biBild->Hintergrundfarbe(ucRot, ucGrun, ucBlau);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBildBox::Bild_pfnWM_Paint(void (__vectorcall *pfnWM_Paint_Bild)(COBild*, PAINTSTRUCT&))
{
 ThreadSicher_Anfang();
 biBild->pfnWM_Paint = pfnWM_Paint_Bild;
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBildBox::Bild_AndernGrosse(int lHoheA, int lBreiteA, bool bMillimeter)
{
 ThreadSicher_Anfang();
 if(bMillimeter) biBild->NeueFensterGrosse(MMzuPixel(LOGPIXELSY, lHoheA), MMzuPixel(LOGPIXELSX, lBreiteA));
 else biBild->NeueFensterGrosse(lHoheA, lBreiteA);

 long lTemp;
 if(lRand_rechts > biBild->Breite(lTemp))	biBild->NeueFensterPosition((lRand_rechts - biBild->Breite(lTemp)) / 2, 0);
 else biBild->NeueFensterPosition(0, 0);

 SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO);
 SetzScrollHorz(stScrollInfo);
 SetzScrollVert(stScrollInfo);
 biBild->UpdateFenster(NULL, false, false);
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
SIZE& __vectorcall RePag::GUI::COBildBox::Bild_Grosse_Original(SIZE& stGrosse)
{
 ThreadSicher_Anfang();
 stGrosse.cx = lBildbreite;
 stGrosse.cy = lBildhohe;
 ThreadSicher_Ende();
 return stGrosse;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
SIZE& __vectorcall RePag::GUI::COBildBox::Bild_Grosse_Aktuell(SIZE& stGrosse)
{
 ThreadSicher_Anfang();
 long lTemp;
 stGrosse.cx = biBild->Breite(lTemp);
 stGrosse.cy = biBild->Hohe(lTemp);
 ThreadSicher_Ende();
 return stGrosse;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
int __vectorcall RePag::GUI::COBildBox::MMzuPixel(int iIndex, int iMillimeter)
{
 ThreadSicher_Anfang();
 HDC hdc = GetDC(biBild->HWND_Element());
 int iPixel = GetDeviceCaps(hdc, iIndex) * iMillimeter / 25.4;
 ReleaseDC(biBild->HWND_Element(), hdc);
 ThreadSicher_Ende();
 return iPixel;
}