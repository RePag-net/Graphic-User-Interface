/****************************************************************************
	OListBox.cpp
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
#include "OListBox.h"
#include "OKlappBox.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COListBox* __vectorcall RePag::GUI::COListBoxV(const char* pcFensterName, unsigned int uiIDElement)
{
 COListBox* vListBox = (COListBox*)VMBlock(VMDialog(), sizeof(COListBox));
 vListBox->COListBoxV(VMDialog(), pcFensterName, uiIDElement);
 return vListBox;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COListBox* __vectorcall RePag::GUI::COListBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement)
{
 COListBox* vListBox = (COListBox*)VMBlock(vmSpeicher, sizeof(COListBox));
 vListBox->COListBoxV(vmSpeicher, pcFensterName, uiIDElement);
 return vListBox;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_ListBox(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
	COListBox* pListBox;
	switch(uiMessage){
		case WM_CREATE      : ((COListBox*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create(hWnd);
													return NULL;
		case WM_SIZE        : pListBox = (COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pListBox) pListBox->WM_Size(lParam);
													else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													return NULL;
		case WM_KILLFOCUS   : pListBox = (COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pListBox->pfnWM_KillFocus){ pListBox->ThreadSicher_Anfang(); pListBox->pfnWM_KillFocus(pListBox); pListBox->ThreadSicher_Ende(); }
													return NULL;
		case WM_VSCROLL     : ((COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_VScroll(wParam);
													return NULL;
		case WM_HSCROLL     : ((COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_HScroll(wParam);
													return NULL;
		case WM_KEYDOWN     : ((COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_KeyDown(wParam);
													return NULL;
		case WM_CHAR        : ((COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_Char(wParam);
													return NULL;
		case WM_COMMAND     : pListBox = (COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pListBox->pfnWM_Command){
														pListBox->ThreadSicher_Anfang();
														if(!pListBox->pfnWM_Command(pListBox, wParam)){ pListBox->ThreadSicher_Ende(); return NULL; }
														pListBox->ThreadSicher_Ende();
													}
													else PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
													break;
		case WM_LBUTTONDOWN : pListBox = (COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pListBox->pfnWM_LButtonDown){ pListBox->ThreadSicher_Anfang(); pListBox->pfnWM_LButtonDown(pListBox); pListBox->ThreadSicher_Ende(); }
													return NULL;
		case WM_LBUTTONUP   : pListBox = (COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													pListBox->ThreadSicher_Anfang();
													pListBox->WM_LButtonUp(lParam);
													if(pListBox->pfnWM_LButtonUp) pListBox->pfnWM_LButtonUp(pListBox);
													else PostMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWnd, GWLP_ID), wParam), lParam);
													pListBox->ThreadSicher_Ende();
													return NULL;
		case WM_MOUSEWHEEL  : ((COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseWheel(wParam, lParam);
													return NULL;
		case WM_PAINT       : ((COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_Paint();
													return NULL;
		case WM_NCDESTROY   : pListBox = (COListBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													if(pListBox->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pListBox->htEffekt_Timer, INVALID_HANDLE_VALUE);
													VMFreiV(pListBox);
													return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COListBox::COListBoxV(VMEMORY vmSpeicher, const char* pcKlassenName, const char* pcFensterName, unsigned int uiIDElementA)
{
 COTextBoxV(vmSpeicher, pcKlassenName, pcFensterName, uiIDElementA);

 stSelectSchriftfarbe.ucRot = stSelectSchriftfarbe.ucGrun = stSelectSchriftfarbe.ucBlau = 230;
 stSelectHintergrundfarbe.ucRot = 50; stSelectHintergrundfarbe.ucGrun = 190; stSelectHintergrundfarbe.ucBlau = 50;
 ucIndex = 0;
 
 pfnWM_Command = nullptr;
 pfnWM_LButtonDown = nullptr;
 pfnWM_LButtonUp = nullptr;
 pfnWM_Char_Return = nullptr;
 pfnWM_Char_Escape = nullptr;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COListBox::COListBoxV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA)
{
 COListBoxV(vmSpeicher, pcRePag_ListBox, pcFensterName, uiIDElementA);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::GUI::COListBox::COFreiV(void)
{
 return ((COTextBox*)this)->COFreiV();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COListBox::WM_KeyDown(WPARAM wParam)
{
 SCROLLINFO stScrollVert; stScrollVert.cbSize = sizeof(SCROLLINFO); stScrollVert.fMask = SIF_POS | SIF_PAGE;
 switch(wParam){
		case VK_UP     : ThreadSicher_Anfang();
			               if(cSelect){ 
											 if(!rcSelect.top){ GetScrollInfo(hWndElement, SB_VERT, &stScrollVert);
												 if(stScrollVert.nPos){ SendMessage(hWndElement, WM_VSCROLL, SB_LINEUP, NULL); SetzSelectIndex(ucIndex - 1); }
											 }
											 else SetzSelectIndex(ucIndex - 1);
										 } 
										 ThreadSicher_Ende();
										 break;
		case VK_DOWN   : ThreadSicher_Anfang();
			               if(cSelect){ GetScrollInfo(hWndElement, SB_VERT, &stScrollVert);
			                 if(ucIndex - stScrollVert.nPos >= stScrollVert.nPage - 2){ SendMessage(hWndElement, WM_VSCROLL, SB_LINEDOWN, NULL); SetzSelectIndex(ucIndex + 1); }
											 else SetzSelectIndex(ucIndex + 1);
										 } 
										 ThreadSicher_Ende();
										 break;
 }
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COListBox::WM_Char(WPARAM wParam)
{
 switch(wParam){
	  case VK_RETURN : ThreadSicher_Anfang();
										 if(pfnWM_Char_Return) pfnWM_Char_Return(this);
										 ThreadSicher_Ende();
		                 break;
	  case VK_ESCAPE : ThreadSicher_Anfang();
			               DeSelect();
			               if(pfnWM_Char_Escape) pfnWM_Char_Escape(this);
										 ThreadSicher_Ende();
								     break;
 }
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COListBox::WM_LButtonUp(LPARAM lParam)
{
 if(vliText->Number()){
	 SCROLLINFO stScrollVert; stScrollVert.cbSize = sizeof(SCROLLINFO); stScrollVert.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
	 GetScrollInfo(hWndElement, SB_VERT, &stScrollVert); 

   BYTE ucIndexAlt = ucIndex;
   ucIndex = (BYTE)(HIWORD(lParam) / lZeichen_hohe + stScrollVert.nPos);
	 if(ucIndex >= stScrollVert.nMax) ucIndex = stScrollVert.nMax - 1;

	 if((ucIndex - stScrollVert.nPos) * lZeichen_hohe + lZeichen_hohe <= lRand_unten){

		 if(cSelect){ cSelect = 0; UpdateFenster(&rcSelect, true, false); }
	   
     SelectZeile(stScrollVert);
		 SetFocus(hWndElement);
   }
   else ucIndex = ucIndexAlt; 
 }
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COListBox::SetzSelectIndex(unsigned char ucIndexA)
{
 ThreadSicher_Anfang();
 if(ucIndexA < vliText->Number()){
   SCROLLINFO stScrollVert; stScrollVert.cbSize = sizeof(SCROLLINFO); stScrollVert.fMask = SIF_POS | SIF_PAGE;
	 GetScrollInfo(hWndElement, SB_VERT, &stScrollVert); 

	 if(cSelect){ cSelect = 0; UpdateFenster(&rcSelect, true, false); }

	 ucIndex = ucIndexA;
	 SelectZeile(stScrollVert);
	 ThreadSicher_Ende();
	 return true;
 }
 ThreadSicher_Ende();
 return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COListBox::SelectZeile(SCROLLINFO& stScrollVert)
{
 SIZE stZeichengrosse;
 COStringA* pasZeile = (COStringA*)vliText->Element(ucIndex);
 ulZeichenPos = pasZeile->Length();

 while(ucIndex < stScrollVert.nPos){
   SendMessage(hWndElement, WM_VSCROLL, SB_LINEUP, NULL);
	 GetScrollInfo(hWndElement, SB_VERT, &stScrollVert); 
 }
 while(ucIndex - stScrollVert.nPos >= stScrollVert.nPage - 1){
   SendMessage(hWndElement, WM_VSCROLL, SB_LINEDOWN, NULL);
	 GetScrollInfo(hWndElement, SB_VERT, &stScrollVert); 
 }

 HDC hdc = GetDC(hWndElement); SelectObject(hdc, hFont);

 cSelect = 1;
 rcSelect.top = (ucIndex - stScrollVert.nPos) * lZeichen_hohe; rcSelect.bottom = rcSelect.top + lZeichen_hohe;
 GetTextExtentPoint32(hdc, pasZeile->c_Str(), pasZeile->Length(), &stZeichengrosse);
 if(stZeichengrosse.cx < lRand_rechts) rcSelect.right = stZeichengrosse.cx;
 else rcSelect.right = lRand_rechts; 
 ReleaseDC(hWndElement, hdc);
 
 UpdateFenster(&rcSelect, true, false);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
unsigned char __vectorcall RePag::GUI::COListBox::SelectIndex(void)
{
 ThreadSicher_Anfang();
 BYTE ucIndexA = ucIndex;
 ThreadSicher_Ende();
 return ucIndexA;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
COStringA* __vectorcall RePag::GUI::COListBox::SelectEintrag(COStringA* vasEintrag)
{
 ThreadSicher_Anfang();
 if(!cSelect) *vasEintrag = NULL;
 else{ void* pvIterator = vliText->Element(ucIndex);
	 if(pvIterator) *vasEintrag = *((COStringA*)pvIterator);
	 else *vasEintrag = NULL;
 }
 ThreadSicher_Ende();
 return vasEintrag;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COListBox::SuchEintrag(COStringA* vasEintrag, unsigned char& ucIndexA)
{
 ThreadSicher_Anfang();
 void* pvIterator = vliText->IteratorToBegin(); ucIndexA = 0;
 while(pvIterator){
	 if(*(COStringA*)vliText->Element(pvIterator) == *vasEintrag){ ThreadSicher_Ende(); return true; }
   vliText->NextElement(pvIterator); ucIndexA++;
 }
 ThreadSicher_Ende();
 return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COListBox::SuchUndSetzEintrag(COStringA* vasEintrag, unsigned char& ucIndexA)
{
 ThreadSicher_Anfang();
 bool bReturn = false;
 if(SuchEintrag(vasEintrag, ucIndexA)) bReturn = SetzSelectIndex(ucIndexA);
 ThreadSicher_Ende();
 return bReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
COStringA* __vectorcall RePag::GUI::COListBox::SetzUndSuchEintrag(unsigned char ucIndexA, COStringA* vasEintrag)
{
 ThreadSicher_Anfang();
 if(SetzSelectIndex(ucIndexA)) SelectEintrag(vasEintrag);
 else *vasEintrag = NULL;
 ThreadSicher_Ende();
 return vasEintrag;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
unsigned long __vectorcall RePag::GUI::COListBox::AnzahlEintrage(void)
{
 ThreadSicher_Anfang();
 ULONG ulAnzahl = vliText->Number();
 ThreadSicher_Ende();
 return ulAnzahl;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COListBox::DeSelectEintrag(void)
{
 ThreadSicher_Anfang();
 DeSelect();
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------