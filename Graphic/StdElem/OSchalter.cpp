/****************************************************************************
	OSchalter.cpp
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
#include "OSchalter.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COSchalter* __vectorcall RePag::GUI::COSchalterV(const char* pcFensterName, unsigned int uiIDElement)
{
 COSchalter* vSchalter = (COSchalter*)VMBlock(VMDialog(), sizeof(COSchalter));
 vSchalter->COSchalterV(VMDialog(), pcFensterName, uiIDElement);
 return vSchalter;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COSchalter* __vectorcall RePag::GUI::COSchalterV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement)
{
 COSchalter* vSchalter = (COSchalter*)VMBlock(vmSpeicher, sizeof(COSchalter));
 vSchalter->COSchalterV(vmSpeicher, pcFensterName, uiIDElement);
 return vSchalter;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_Schalter(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
	COSchalter* pSchalter;
	switch(uiMessage){
		case WM_CREATE      : ((COSchalter*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create(hWnd);
													return NULL;
		case WM_SIZE        : pSchalter = (COSchalter*)GetWindowLongPtr(hWnd, GWL_USERDATA);
													if(pSchalter) pSchalter->WM_Size_Element(hWnd, lParam);
													else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													return NULL;
		case WM_COMMAND     : PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
													break;
		case WM_LBUTTONDOWN : pSchalter = (COSchalter*)GetWindowLongPtr(hWnd, GWL_USERDATA);
													SetCapture(hWnd);
													if(pSchalter->pfnWM_LButtonDown){ pSchalter->ThreadSicher_Anfang(); pSchalter->pfnWM_LButtonDown(pSchalter); pSchalter->ThreadSicher_Ende(); }
													return NULL;
		case WM_LBUTTONUP   : ((COSchalter*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_LButtonUp(wParam, lParam);
													return NULL;
		case WM_PAINT       : ((COSchalter*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_Paint();
													return NULL;
		case WM_NCDESTROY   : pSchalter = (COSchalter*)GetWindowLongPtr(hWnd, GWL_USERDATA);
													if(pSchalter->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pSchalter->htEffekt_Timer, INVALID_HANDLE_VALUE);
													VMFreiV(pSchalter);
													return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalter::COSchalterV(VMEMORY vmSpeicher, const char* pcKlassenName, const char* pcFensterName, unsigned int uiIDElementA)
{
 COKnopfV(vmSpeicher, pcKlassenName, pcFensterName, uiIDElementA);

 bEingeschaltet = false;
 vasBeschriftung_ein = COStringAV(vmSpeicher);
 vasBeschriftung_aus = COStringAV(vmSpeicher);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalter::COSchalterV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA)
{
 COKnopfV(vmSpeicher, pcRePag_Schalter, pcFensterName, uiIDElementA);

 bEingeschaltet = false;
 vasBeschriftung_ein = COStringAV(vmSpeicher);
 vasBeschriftung_aus = COStringAV(vmSpeicher);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::GUI::COSchalter::COFreiV(void)
{
 VMFreiV(vasBeschriftung_ein); VMFreiV(vasBeschriftung_aus);
 return ((COTextZeile*)this)->COFreiV();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalter::WM_LButtonUp(WPARAM wParam, LPARAM lParam)
{
 ThreadSicher_Anfang();
 Schalten(!bEingeschaltet);
 if(pfnWM_LButtonUp) pfnWM_LButtonUp(this);
 else PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), lParam);
 ReleaseCapture();
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalter::Schalten(bool bEinschalten)
{
 if(bEinschalten){ bEingeschaltet = true; stHintergrundfarbe = stHintergrundfarbe_2; *vasInhalt = *vasBeschriftung_ein; } 
 else{ bEingeschaltet = false; stHintergrundfarbe = stHintergrundfarbe_1; *vasInhalt = *vasBeschriftung_aus; }
 bAktiv = true;
 UpdateFenster(nullptr, true, true);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COSchalter::Eingeschaltet(void)
{
 ThreadSicher_Anfang();
 bool bEingeschaltetA = bEingeschaltet;
 ThreadSicher_Ende();
 return bEingeschaltetA;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalter::SetzSchalter(bool bEinschalten)
{
 ThreadSicher_Anfang();
 Schalten(bEinschalten);
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalter::Beschriftung(const char* pcBeschriftung)
{
 ThreadSicher_Anfang();
 *vasBeschriftung_ein = pcBeschriftung;
 *vasBeschriftung_aus = pcBeschriftung;
 *vasInhalt = pcBeschriftung; 
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalter::Beschriftung_Ein(const char* pcBeschriftung)
{
 ThreadSicher_Anfang();
 *vasBeschriftung_ein = pcBeschriftung;
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalter::Beschriftung_Aus(const char* pcBeschriftung)
{
 ThreadSicher_Anfang();
 *vasBeschriftung_aus = pcBeschriftung;
 *vasInhalt = pcBeschriftung;
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------