/****************************************************************************
	OKnopf.cpp
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
#include "OKnopf.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COKnopf* __vectorcall RePag::GUI::COKnopfV(const char* pcFensterName, unsigned int uiIDElement)
{
 COKnopf* vKnopf = (COKnopf*)VMBlock(VMDialog(), sizeof(COKnopf));
 vKnopf->COKnopfV(VMDialog(), pcFensterName, uiIDElement);
 return vKnopf;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COKnopf* __vectorcall RePag::GUI::COKnopfV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement)
{
 COKnopf* vKnopf = (COKnopf*)VMBlock(vmSpeicher, sizeof(COKnopf));
 vKnopf->COKnopfV(vmSpeicher, pcFensterName, uiIDElement);
 return vKnopf;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_Knopf(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
	COKnopf* pKnopf;
	switch(uiMessage){
		case WM_CREATE      : ((COKnopf*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create(hWnd);
													return NULL;
		case WM_SIZE        : pKnopf = (COKnopf*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pKnopf) pKnopf->WM_Size_Element(hWnd, lParam);
													else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													return NULL;
		case WM_SETFOCUS    : ((COKnopf*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_SetFocus();
													return NULL;
		case WM_KILLFOCUS   : ((COKnopf*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_KillFocus();
													return NULL;
		case WM_CHAR        : pKnopf = (COKnopf*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(IsWindowEnabled(hWnd)) pKnopf->WM_Char(wParam);
													return NULL;
		case WM_COMMAND     : PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
													break;
		case WM_LBUTTONDOWN : ((COKnopf*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonDown();
													return NULL;
		case WM_LBUTTONUP   : ((COKnopf*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonUp(wParam, lParam);
													return NULL;
		case WM_PAINT       : ((COKnopf*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_Paint();
													return NULL;
		case WM_NCDESTROY   : pKnopf = (COKnopf*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pKnopf->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pKnopf->htEffekt_Timer, INVALID_HANDLE_VALUE);
													VMFreiV(pKnopf);
													return NULL;

	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKnopf::COKnopfV(VMEMORY vmSpeicher, const char* pcKlassenName, const char* pcFensterName, unsigned int uiIDElement)
{
 COTextZeileV(vmSpeicher, pcKlassenName, pcFensterName, uiIDElement);

 lfSchrift.lfHeight = -14;
 lfSchrift.lfWidth = 6;
 lfSchrift.lfWeight = FW_SEMIBOLD;

 stHintergrundfarbe.ucRot = 20; stHintergrundfarbe.ucGrun = 100; stHintergrundfarbe.ucBlau = 210;
 stHintergrundfarbe_1 = stHintergrundfarbe;
 stHintergrundfarbe_2.ucRot = 20; stHintergrundfarbe_2.ucGrun = 210; stHintergrundfarbe_2.ucBlau = 20;
 stHintergrundfarbe_3.ucRot = 200; stHintergrundfarbe_3.ucGrun = 200; stHintergrundfarbe_3.ucBlau = 200;
 stSchriftfarbe.ucRot = stSchriftfarbe.ucGrun = stSchriftfarbe.ucBlau = 230;
 stSchriftfarbe_1 = stSchriftfarbe;
 ucSchriftausrichtung = TXA_MITTEVERTICAL | TXA_MITTEHORIZONTAL;
 stSchriftfarbe_Focus.ucRot = 250; stSchriftfarbe_Focus.ucGrun = 250; stSchriftfarbe_Focus.ucBlau = 10;
 bAktiv = true;

 pfnWM_LButtonDown = nullptr;
 pfnWM_LButtonUp = nullptr;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKnopf::COKnopfV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement)
{
 COKnopfV(vmSpeicher, pcRePag_Knopf, pcFensterName, uiIDElement);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKnopf::WM_SetFocus(void)
{
 ThreadSicher_Anfang();
 stSchriftfarbe = stSchriftfarbe_Focus;
 UpdateFenster(nullptr, false, false);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKnopf::WM_KillFocus(void)
{
 ThreadSicher_Anfang();
 stSchriftfarbe = stSchriftfarbe_1;
 UpdateFenster(nullptr, false, false);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKnopf::WM_Char(WPARAM wParam)
{
 switch(wParam){
	  case VK_RETURN : SendMessage(hWndElement, WM_LBUTTONDOWN, NULL, NULL); Sleep(100); SendMessage(hWndElement, WM_LBUTTONUP, NULL, NULL);
		default        : PostMessage(GetParent(hWndElement), WM_CHAR, wParam, NULL);
		                 break;
 }
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKnopf::WM_LButtonDown(void)
{
 ThreadSicher_Anfang();
 stHintergrundfarbe = stHintergrundfarbe_2;
 SetCapture(hWndElement); 
 UpdateFenster(nullptr, true, true);
 if(hWndElement != GetFocus()) SetFocus(hWndElement);
 if(pfnWM_LButtonDown) pfnWM_LButtonDown(this);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKnopf::WM_LButtonUp(WPARAM wParam, LPARAM lParam)
{
 ThreadSicher_Anfang();
 stHintergrundfarbe = stHintergrundfarbe_1;
 ReleaseCapture();
 UpdateFenster(NULL, true, true);
 if(pfnWM_LButtonUp) pfnWM_LButtonUp(this);
 else PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), lParam);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKnopf::Schriftfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
 ThreadSicher_Anfang();
 stSchriftfarbe_1.ucRot = ucRot; stSchriftfarbe_1.ucGrun = ucGrun; stSchriftfarbe_1.ucBlau = ucBlau;
 stSchriftfarbe = stSchriftfarbe_1;
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKnopf::Schriftfarbe_Focus(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
 ThreadSicher_Anfang();
 stSchriftfarbe_Focus.ucRot = ucRot; stSchriftfarbe_Focus.ucGrun = ucGrun; stSchriftfarbe_Focus.ucBlau = ucBlau;
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKnopf::Hintergrundfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
 ThreadSicher_Anfang();
 stHintergrundfarbe_1.ucRot = ucRot; stHintergrundfarbe_1.ucGrun = ucGrun; stHintergrundfarbe_1.ucBlau = ucBlau;
 stHintergrundfarbe = stHintergrundfarbe_1;
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKnopf::Hintergrundfarbe_1(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
 ThreadSicher_Anfang();
 stHintergrundfarbe_1.ucRot = ucRot; stHintergrundfarbe_1.ucGrun = ucGrun; stHintergrundfarbe_1.ucBlau = ucBlau;
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKnopf::Hintergrundfarbe_2(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
 ThreadSicher_Anfang();
 stHintergrundfarbe_2.ucRot = ucRot; stHintergrundfarbe_2.ucGrun = ucGrun; stHintergrundfarbe_2.ucBlau = ucBlau;
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKnopf::Hintergrundfarbe_3(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
 ThreadSicher_Anfang();
 stHintergrundfarbe_3.ucRot = ucRot; stHintergrundfarbe_3.ucGrun = ucGrun; stHintergrundfarbe_3.ucBlau = ucBlau;
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKnopf::Beschriftung(const char* pcBeschriftung)
{
 ThreadSicher_Anfang();
 *vasInhalt = pcBeschriftung;
 UpdateFenster(nullptr, true, false);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKnopf::SetzAktiv(bool bAktivA)
{
 ThreadSicher_Anfang();
 bAktiv = bAktivA;
 if(bAktiv) stHintergrundfarbe = stHintergrundfarbe_1;
 else stHintergrundfarbe = stHintergrundfarbe_3;
 UpdateFenster(nullptr, true, true);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COKnopf::Aktiv(void)
{
 ThreadSicher_Anfang();
 bool bAktivA = bAktiv;
 ThreadSicher_Ende();
 return bAktivA;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKnopf::Sperren(void)
{
 ThreadSicher_Anfang();
 bAktiv = false;
 stHintergrundfarbe = stHintergrundfarbe_3;
 EnableWindow(hWndElement, false);
 UpdateFenster(nullptr, true, true);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COKnopf::Freigeben(void)
{
 ThreadSicher_Anfang();
 bAktiv = true;
 stHintergrundfarbe = stHintergrundfarbe_1;
 EnableWindow(hWndElement, true);
 UpdateFenster(nullptr, true, true);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------