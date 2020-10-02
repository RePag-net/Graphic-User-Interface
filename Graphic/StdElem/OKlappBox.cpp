/****************************************************************************
	OKlappBox.cpp
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
#include "OKlappBox.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COKlappBox* __vectorcall RePag::GUI::COKlappBoxV(const char* pcFensterName, unsigned int uiIDElement)
{
 COKlappBox* vKlappBox = (COKlappBox*)VMBlock(VMDialog(), sizeof(COKlappBox));
 vKlappBox->COKlappBoxV(VMDialog(), pcFensterName, uiIDElement);
 return vKlappBox;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COKlappBox* __vectorcall RePag::GUI::COKlappBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement)
{
 COKlappBox* vKlappBox = (COKlappBox*)VMBlock(vmSpeicher, sizeof(COKlappBox));
 vKlappBox->COKlappBoxV(vmSpeicher, pcFensterName, uiIDElement);
 return vKlappBox;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_KlappBox(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
	COKlappBox* pKlappBox;
	switch(uiMessage){
		case WM_CREATE      : ((COKlappBox*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create(hWnd);
												  ((COKlappBox*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create_KlappBox(hWnd);
												  return NULL;
		case WM_SIZE        : pKlappBox = (COKlappBox*)GetWindowLongPtr(hWnd, GWL_USERDATA);
												  if(pKlappBox) pKlappBox->WM_Size(lParam);
												  else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
												  return NULL;
		case WM_KILLFOCUS   : pKlappBox = (COKlappBox*)GetWindowLongPtr(hWnd, GWL_USERDATA);
												  pKlappBox->ThreadSicher_Anfang();
												  if(IsWindowVisible(hWnd)) pKlappBox->WM_LButtonUp_KlappBox();
												  if(pKlappBox->pfnWM_KillFocus) pKlappBox->pfnWM_KillFocus(pKlappBox);
												  pKlappBox->ThreadSicher_Ende();
												  return NULL;
		case WM_VSCROLL     : ((COKlappBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_VScroll(wParam);
													return NULL;
		case WM_HSCROLL     : ((COKlappBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_HScroll(wParam);
													return NULL;
		case WM_KEYDOWN     : ((COKlappBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_KeyDown(wParam);
													return NULL;
		case WM_CHAR        : ((COKlappBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_Char(wParam);
													return NULL;
		case WM_COMMAND     : pKlappBox = (COKlappBox*)GetWindowLongPtr(hWnd, GWL_USERDATA);
													if(pKlappBox->pfnWM_Command){
														pKlappBox->ThreadSicher_Anfang();
														if(!pKlappBox->pfnWM_Command(pKlappBox, wParam)){ pKlappBox->ThreadSicher_Ende(); return NULL; }
														pKlappBox->ThreadSicher_Ende();
													}
													else PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
													break;
		case WM_LBUTTONDOWN : pKlappBox = (COKlappBox*)GetWindowLongPtr(hWnd, GWL_USERDATA);
													if(pKlappBox->pfnWM_LButtonDown){ pKlappBox->ThreadSicher_Anfang(); pKlappBox->pfnWM_LButtonDown(pKlappBox); pKlappBox->ThreadSicher_Ende(); }
													return NULL;
		case WM_LBUTTONUP   : pKlappBox = (COKlappBox*)GetWindowLongPtr(hWnd, GWL_USERDATA);
													pKlappBox->ThreadSicher_Anfang();
													pKlappBox->WM_LButtonUp(lParam);
													pKlappBox->WM_LButtonUp_KlappBox();
													if(pKlappBox->pfnWM_LButtonUp) pKlappBox->pfnWM_LButtonUp(pKlappBox);
													else PostMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWnd, GWLP_ID), wParam), lParam);
													pKlappBox->ThreadSicher_Ende();
													return NULL;
		case WM_MOUSEWHEEL  : ((COKlappBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_MouseWheel(wParam, lParam);
													return NULL;
		case WM_PAINT       : ((COKlappBox*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_Paint();
													return NULL;
		case WM_NCDESTROY   : pKlappBox = (COKlappBox*)GetWindowLongPtr(hWnd, GWL_USERDATA);
													if(pKlappBox->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pKlappBox->htEffekt_Timer, INVALID_HANDLE_VALUE);
													VMFreiV(pKlappBox);
													return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//---------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_EintragZeile(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
	COKlappBox::COEintragZeile* pEintragZeile;
	switch(uiMessage){
		case WM_CREATE			: ((COKlappBox::COEintragZeile*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create(hWnd);
													return NULL;
		case WM_SIZE				: pEintragZeile = (COKlappBox::COEintragZeile*)GetWindowLongPtr(hWnd, GWL_USERDATA);
													if(pEintragZeile) pEintragZeile->WM_Size_Element(hWnd, lParam);
													else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													return NULL;
		case WM_SETFOCUS		: ((COKlappBox::COEintragZeile*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_SetFocus();
													return NULL;
		case WM_KILLFOCUS		: ((COKlappBox::COEintragZeile*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_KillFocus();
													return NULL;
		case WM_CHAR				: pEintragZeile = (COKlappBox::COEintragZeile*)GetWindowLongPtr(hWnd, GWL_USERDATA);
													if(IsWindowEnabled(hWnd)) pEintragZeile->WM_Char(wParam);
													return NULL;
		case WM_COMMAND			: PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
													return NULL;
		case WM_LBUTTONDOWN : pEintragZeile = (COKlappBox::COEintragZeile*)GetWindowLongPtr(hWnd, GWL_USERDATA);
													if(pEintragZeile->pfnWM_LButtonDown){ pEintragZeile->ThreadSicher_Anfang(); pEintragZeile->pfnWM_LButtonDown(pEintragZeile); pEintragZeile->ThreadSicher_Ende(); }
													return NULL;
		case WM_LBUTTONUP		: ((COKlappBox::COEintragZeile*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_LButtonUp(wParam, lParam);
													return NULL;
		case WM_PAINT				: ((COKlappBox::COEintragZeile*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_Paint();
													return NULL;
		case WM_NCDESTROY		: pEintragZeile = (COKlappBox::COEintragZeile*)GetWindowLongPtr(hWnd, GWL_USERDATA);
													if(pEintragZeile->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pEintragZeile->htEffekt_Timer, INVALID_HANDLE_VALUE);
													pEintragZeile->pKlappBox->ezEintrag = NULL;	VMFreiV(pEintragZeile);
													return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKlappBox::COKlappBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA)
{
 COListBoxV(vmSpeicher, pcRePag_KlappBox, pcFensterName, uiIDElementA);

 FensterStil(WS_CHILDWINDOW | WS_CLIPSIBLINGS);

 COStringA asTemp = pcFensterName; asTemp += "_lbKlappBoxEintrage";
 ezEintrag = (COEintragZeile*)VMBlock(vmSpeicher, sizeof(COEintragZeile));
 ezEintrag->COEintragZeileV(vmSpeicher, asTemp.c_Str(), ++uiIDElementA, this);

 ezEintrag->Hintergrundfarbe(stHintergrundfarbe.ucRot, stHintergrundfarbe.ucGrun, stHintergrundfarbe.ucBlau);
 ezEintrag->Schriftfarbe(stSchriftfarbe.ucRot, stSchriftfarbe.ucGrun, stSchriftfarbe.ucBlau);
 ucZeilenhohe_EintragZeile = 20;
}
//---------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::GUI::COKlappBox::COFreiV(void)
{
 if(ezEintrag) DestroyWindow(ezEintrag->HWND_Element());
 return ((COListBox*)this)->COFreiV();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKlappBox::COEintragZeile::COEintragZeileV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA, COKlappBox* pKlappBoxA)
{
 COKnopfV(vmSpeicher, pcRePag_KlappBoxEintrage, pcFensterName, uiIDElementA);

 pKlappBox = pKlappBoxA;

 ucSchriftausrichtung = TXA_MITTEVERTICAL | TXA_LINKS;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKlappBox::COEintragZeile::WM_Paint(void)
{
 ThreadSicher_Anfang();
 PAINTSTRUCT stPaint; 
 BeginPaint(hWndElement, &stPaint);
 WM_Paint_Hintergrund(stPaint);

 RECT rcKnopf; rcKnopf.right = lBreite; rcKnopf.left = lBreite - lHohe; rcKnopf.top = 0; rcKnopf.bottom = lHohe;
 HBRUSH hBrush = CreateSolidBrush(RGB(20, 100, 210));
 SelectObject(stPaint.hdc, hBrush);
 FillRect(stPaint.hdc, &rcKnopf, hBrush);
 DeleteObject(hBrush);

 hBrush = CreateSolidBrush(RGB(230, 230, 230));
 HPEN hPen = CreatePen(PS_SOLID, 1, RGB(230, 230, 230));
 SelectObject(stPaint.hdc, hBrush);
 SelectObject(stPaint.hdc, hPen);
 POINT points3[3];
 points3[0].x = lBreite - rcKnopf.bottom / 4; points3[0].y = rcKnopf.bottom / 4;
 points3[1].x = (long)(lBreite - rcKnopf.bottom * 0.75); points3[1].y = rcKnopf.bottom / 4;
 points3[2].x = lBreite - rcKnopf.bottom / 2; points3[2].y = (long)(rcKnopf.bottom * 0.75);
 Polygon(stPaint.hdc, points3, 3);

 DeleteObject(hBrush); DeleteObject(hPen);

 SIZE stZeichengrosse; POINT ptText = {0,0};
 SelectObject(stPaint.hdc, hFont);
 Schriftausrichtung(stPaint.hdc, stZeichengrosse, ptText);
 stPaint.rcPaint.right = rcKnopf.left;

 SetBkMode(stPaint.hdc, TRANSPARENT); 
 SetTextColor(stPaint.hdc, RGB(stSchriftfarbe.ucRot, stSchriftfarbe.ucGrun, stSchriftfarbe.ucBlau));
 ExtTextOut(stPaint.hdc, ptText.x, ptText.y, ETO_CLIPPED, &stPaint.rcPaint, vasInhalt->c_Str(), vasInhalt->Length(), nullptr);

 EndPaint(hWndElement, &stPaint);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKlappBox::WM_Create_KlappBox(HWND hWnd)
{
 hWndElement = hWnd; LOGFONT lfTemp;

 ezEintrag->Schriftart(Schrift(lfTemp));
 ezEintrag->ErstellFenster(GetParent(hWndElement), ucZeilenhohe_EintragZeile, lBreite, ptPosition.x, ptPosition.y - ucZeilenhohe_EintragZeile);
 NeuesFenster(lHohe - ucZeilenhohe_EintragZeile, lBreite, ptPosition.x, ptPosition.y);

 RECT rcClient; SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO);
 GetClientRect(hWndElement, &rcClient);
 if(lRand_unten != rcClient.bottom){ lRand_unten = rcClient.bottom; SetzScrollVert(stScrollInfo); }
 GetClientRect(hWndElement, &rcClient);
 if(lRand_rechts != rcClient.right){ lRand_rechts = rcClient.right; SetzScrollHorz(stScrollInfo); }
 GetClientRect(hWndElement, &rcClient);
 if(lRand_unten != rcClient.bottom){ lRand_unten = rcClient.bottom; SetzScrollVert(stScrollInfo); }

 if(vliText->Number()) ezEintrag->Text(SetzUndSuchEintrag(0, vasInhalt)->c_Str());
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKlappBox::WM_LButtonUp_KlappBox(void)
{
 ThreadSicher_Anfang();
 ezEintrag->Text(SelectEintrag(vasInhalt)->c_Str());
 ShowWindow(hWndElement, SW_HIDE);
 RECT stRect; HWND hWndZeichnen_1, hWndZeichnen_2 = GetParent(hWndElement); 
 while(IsChild(hWndZeichnen_1 = GetParent(hWndZeichnen_2), hWndZeichnen_2)) hWndZeichnen_2 = hWndZeichnen_1;
 ((COElement*)GetWindowLongPtr(hWndZeichnen_2, GWL_USERDATA))->UpdateFenster(&Fenster(stRect), true, false);
 //InvalidateRect(hWndZeichnen_2, &Fenster(stRect), true); UpdateWindow(hWndZeichnen_2);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKlappBox::COEintragZeile::WM_LButtonUp(WPARAM wParam, LPARAM lParam)
{
 ThreadSicher_Anfang();
 SetWindowPos(pKlappBox->HWND_Element(), HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
 SetFocus(pKlappBox->HWND_Element());
 if(pfnWM_LButtonUp) pfnWM_LButtonUp(this);
 else PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), lParam);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
long& __vectorcall RePag::GUI::COKlappBox::Hohe(long& lHoheA)
{
 ThreadSicher_Anfang();
 lHoheA = lHohe + ucZeilenhohe_EintragZeile;
 ThreadSicher_Ende();
 return lHoheA;
}
//---------------------------------------------------------------------------------------------------------------------------------------
RECT& __vectorcall RePag::GUI::COKlappBox::Fenster(RECT& rcFenster)
{
 ThreadSicher_Anfang();
 rcFenster.left = ptPosition.x;
 rcFenster.top = ptPosition.y - ucZeilenhohe_EintragZeile;
 rcFenster.right = ptPosition.x + lBreite;
 rcFenster.bottom = lHohe + ptPosition.y;
 ThreadSicher_Ende();
 return rcFenster;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKlappBox::NeueFensterPosition(long lPos_x, long lPos_y)
{
 ThreadSicher_Anfang();
 ptPosition.x = lPos_x; ptPosition.y = lPos_y + ucZeilenhohe_EintragZeile;
 MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lBreite, lHohe, IsWindowVisible(hWndElement));

 ezEintrag->NeueFensterPosition(ptPosition.x, ptPosition.y - ucZeilenhohe_EintragZeile);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKlappBox::NeueFensterPosition(POINT& ptPositionA)
{
 ThreadSicher_Anfang();
 ptPositionA.y += ucZeilenhohe_EintragZeile;
 ptPosition = ptPositionA;
 MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lBreite, lHohe, IsWindowVisible(hWndElement));

 ezEintrag->NeueFensterPosition(ptPosition.x, ptPosition.y - ucZeilenhohe_EintragZeile);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKlappBox::NeuesFenster(long lHoheA, long lBreiteA, long lPos_x, long lPos_y)
{
 ThreadSicher_Anfang();
 lHohe = lHoheA; lBreite = lBreiteA; ptPosition.x = lPos_x; ptPosition.y = lPos_y + ucZeilenhohe_EintragZeile; 
 InvalidateRect(hWndElement, nullptr, false);
 MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lBreite, lHohe, IsWindowVisible(hWndElement));

 ezEintrag->NeuesFenster(ucZeilenhohe_EintragZeile, lBreiteA, lPos_x, lPos_y);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKlappBox::NeueFensterGrosse(long lHoheA, long lBreiteA)
{
 ThreadSicher_Anfang();
 lHohe = lHoheA - ucZeilenhohe_EintragZeile; lBreite = lBreiteA;
 InvalidateRect(hWndElement, nullptr, false);
 MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lBreite, lHohe, IsWindowVisible(hWndElement));

 ezEintrag->NeueFensterGrosse(ucZeilenhohe_EintragZeile, lBreiteA);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKlappBox::NeueFensterHohe(long lHoheA)
{
 ThreadSicher_Anfang();
 lHohe = lHoheA - ucZeilenhohe_EintragZeile;

 InvalidateRect(hWndElement, nullptr, false);
 MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lBreite, lHohe, IsWindowVisible(hWndElement));
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKlappBox::NeueFensterBreite(long lBreiteA)
{
 ThreadSicher_Anfang();
 lBreite = lBreiteA;
 InvalidateRect(hWndElement, nullptr, false);
 MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lBreite, lHohe, IsWindowVisible(hWndElement));

 ezEintrag->NeueFensterBreite(lBreiteA);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKlappBox::AndernFensterGrosse(long lHoheA, long lBreiteA)
{
 ThreadSicher_Anfang();
 lHohe += lHoheA; lBreite += lBreiteA;
 InvalidateRect(hWndElement, nullptr, false);
 MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lBreite, lHohe, IsWindowVisible(hWndElement));

 ezEintrag->AndernFensterGrosse(0, lBreiteA);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKlappBox::AndernFensterPosition(long lPos_x, long lPos_y)
{
 ThreadSicher_Anfang();
 ptPosition.x += lPos_x; ptPosition.y += lPos_y;
 MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lBreite, lHohe, IsWindowVisible(hWndElement));

 ezEintrag->AndernFensterPosition(lPos_x, lPos_y);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKlappBox::Schriftart(LOGFONT& lfSchriftA)
{
 if(!hWndElement){ lfSchrift = lfSchriftA; ezEintrag->Schriftart(lfSchriftA);}
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKlappBox::Zeilenhohe_EintragZeile(unsigned char ucHohe)
{
 ThreadSicher_Anfang();
 ucZeilenhohe_EintragZeile = ucHohe;
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKlappBox::Schriftfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
 ThreadSicher_Anfang();
 stSchriftfarbe.ucRot = ucRot; stSchriftfarbe.ucGrun = ucGrun; stSchriftfarbe.ucBlau = ucBlau;
 ezEintrag->Schriftfarbe(ucRot, ucGrun, ucBlau);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKlappBox::Hintergrundfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
 ThreadSicher_Anfang();
 stHintergrundfarbe.ucRot = ucRot; stHintergrundfarbe.ucGrun = ucGrun; stHintergrundfarbe.ucBlau = ucBlau;
 if(hWndElement) UpdateFenster(nullptr, true, true);
 ezEintrag->Hintergrundfarbe(ucRot, ucGrun, ucBlau);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKlappBox::Sperren(void)
{
 ezEintrag->Sperren();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKlappBox::Freigeben(void)
{
 ezEintrag->Freigeben();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKlappBox::Text(char* pcText)
{
 ThreadSicher_Anfang();
 ((COTextBox*)this)->Text(pcText);
 if(ezEintrag->Aktiv() && vliText->Number()) ezEintrag->Text(SetzUndSuchEintrag(0, vasInhalt)->c_Str());
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COKlappBox::SetzSelectIndex(unsigned char ucIndex)
{
 ThreadSicher_Anfang();
 if(((COListBox*)this)->SetzSelectIndex(ucIndex)){ 
	 ezEintrag->Text(SetzUndSuchEintrag(ucIndex, vasInhalt)->c_Str()); 
   ThreadSicher_Ende();
	 return true;
 }
 ThreadSicher_Ende();
 return false;
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COKlappBox::SuchUndSetzEintrag(COStringA* vasEintrag, unsigned char& ucIndexA)
{
 ThreadSicher_Anfang();
 bool bReturn = false;
 if(SuchEintrag(vasEintrag, ucIndexA)) bReturn = SetzSelectIndex(ucIndexA);
 ThreadSicher_Ende();
 return bReturn;
}
//---------------------------------------------------------------------------------------------------------------------------------------