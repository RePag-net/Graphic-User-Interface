/****************************************************************************
	OGrafikGruppe.cpp
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

#include "HProgDlg.h"
#include "OGrafikGruppe.h"

#define _WM_Command ((STTHWM_CommandGG*)pvParam)
//-------------------------------------------------------------------------------------------------------------------------------------------
COGrafikGruppe* __vectorcall RePag::GUI::COGrafikGruppeV(const char* pcFensterName, unsigned int uiIDElement)
{
	COGrafikGruppe* vGrafikGruppe = (COGrafikGruppe*)VMBlock(vmDialog, sizeof(COGrafikGruppe));
	vGrafikGruppe->COGrafikGruppeV(vmDialog, pcFensterName, uiIDElement);
	return vGrafikGruppe;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
COGrafikGruppe* __vectorcall RePag::GUI::COGrafikGruppeV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement)
{
	COGrafikGruppe* vGrafikGruppe = (COGrafikGruppe*)VMBlock(vmSpeicher, sizeof(COGrafikGruppe));
	vGrafikGruppe->COGrafikGruppeV(vmSpeicher, pcFensterName, uiIDElement);
	return vGrafikGruppe;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
	namespace GUI
	{
		//---------------------------------------------------------------------------------------------------------------------------------------
		typedef struct STTHWM_CommandGG
		{
			HANDLE hThread;
			DWORD dwThreadId;
			HWND hWndGG;
			unsigned int uiMessage;
			WPARAM wParam;
			LPARAM lParam;
			COGrafikGruppe* pGrafikGruppe;
		} STTHWM_CommandGG;
		//---------------------------------------------------------------------------------------------------------------------------------------
		DWORD WINAPI thWM_Command_GrafikGruppe(void* pvParam)
		{
			_WM_Command->pGrafikGruppe->ThreadSicher_Anfang();
			_WM_Command->pGrafikGruppe->pfnWM_Command(_WM_Command->hWndGG, _WM_Command->uiMessage, _WM_Command->wParam, _WM_Command->lParam);
			_WM_Command->pGrafikGruppe->ThreadSicher_Ende();

			void* pvThreadId = vthlThreadId->ThIteratorToBegin_Lock(); void* pvLoschen = nullptr;
			while(pvThreadId){
				if(((STThreadId*)vthlThreadId->Element(pvThreadId))->dwThreadId == _WM_Command->dwThreadId){ vthlThreadId->DeleteElement(pvThreadId, pvLoschen, true); break; }
				vthlThreadId->NextElement(pvThreadId, pvLoschen);
			}
			vthlThreadId->ThIteratorEnd();
			DeleteEvent(_WM_Command->hThread);
			VMFrei(pvParam);
			return NULL;
		}
		//---------------------------------------------------------------------------------------------------------------------------------------
		LRESULT CALLBACK WndProc_GrafikGruppe(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
		{
			COGrafikGruppe* pGrafikGruppe;
			switch(uiMessage){
				case WM_SIZE			: pGrafikGruppe = (COGrafikGruppe*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
														if(pGrafikGruppe) pGrafikGruppe->WM_Size_Element(hWnd, lParam);
														return NULL;
				case WM_COMMAND		:	pGrafikGruppe = (COGrafikGruppe*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
														if(pGrafikGruppe->pfnWM_Command){ pGrafikGruppe->WM_Command_GrafikGruppe(uiMessage, wParam, lParam); return NULL; }
														else PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
														break;
				case WM_PAINT			:	pGrafikGruppe = (COGrafikGruppe*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
														pGrafikGruppe->WM_Paint();
														return NULL;
				case WM_NCDESTROY	:	pGrafikGruppe = (COGrafikGruppe*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
														if(pGrafikGruppe->htEffekt_Timer)	DeleteTimerQueueTimer(htqTimerQueue, pGrafikGruppe->htEffekt_Timer, INVALID_HANDLE_VALUE);
														VMFreiV(pGrafikGruppe);
														return NULL;
			}
			return DefWindowProc(hWnd, uiMessage, wParam, lParam);
		}
		//---------------------------------------------------------------------------------------------------------------------------------------
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COGrafikGruppe::COGrafikGruppeV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA)
{
	COGrafikV(vmSpeicher, pcRePag_GrafikGruppe, pcFensterName, uiIDElementA);
	SetzHintergrund(false);

	pfnWM_Command = nullptr;
	pfnWM_Paint = nullptr;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void __vectorcall RePag::GUI::COGrafikGruppe::WM_Paint(void)
{
	ThreadSicher_Anfang();
	PAINTSTRUCT stPaint; 
	BeginPaint(hWndElement, &stPaint);
	WM_Paint_Hintergrund(stPaint);
	if(pfnWM_Paint) pfnWM_Paint(this, stPaint);
	EndPaint(hWndElement, &stPaint);
	ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COGrafikGruppe::WM_Command_GrafikGruppe(unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
	STTHWM_CommandGG* stthWM_Command = (STTHWM_CommandGG*)VMBlock(sizeof(STTHWM_CommandGG));
	stthWM_Command->hWndGG = hWndElement;	stthWM_Command->pGrafikGruppe = this; stthWM_Command->uiMessage = uiMessage; stthWM_Command->wParam = wParam;	stthWM_Command->lParam = lParam;
	stthWM_Command->hThread = CreateThread(nullptr, 0, thWM_Command_GrafikGruppe, stthWM_Command, CREATE_SUSPENDED, &stthWM_Command->dwThreadId);

	STThreadId* stThreadId = (STThreadId*)VMBlock(vmDialog, sizeof(STThreadId));
	stThreadId->pElement = this; stThreadId->dwThreadId = stthWM_Command->dwThreadId; vthlThreadId->ThToEnd(stThreadId);

	if(stthWM_Command->hThread){ SetThreadPriority(stthWM_Command->hThread, THREAD_PRIORITY_NORMAL); ResumeThread(stthWM_Command->hThread); }
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COGrafikGruppe::ErstellFensterInRahmen(COGrafik* pGrafik)
{
	pGrafik->ErstellFenster(hWndElement, lHohe - rcEffektrand.top - rcEffektrand.bottom, lBreite - rcEffektrand.left - rcEffektrand.right, rcEffektrand.left, rcEffektrand.top);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COGrafikGruppe::ErstellFensterInGruppe(COGrafik* pGrafik, long lHoheA, long lBreiteA, long lPos_x, long lPos_y)
{
	pGrafik->ErstellFenster(hWndElement, lHoheA, lBreiteA, lPos_x, lPos_y);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COGrafikGruppe::SetzSichtbar(bool bSichtbar)
{
	if(bSichtbar) ShowWindow(hWndElement, SW_SHOW);
	else ShowWindow(hWndElement, SW_HIDE);
}