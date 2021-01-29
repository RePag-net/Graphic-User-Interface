/****************************************************************************
  OGrafic.cpp
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

#include "HProgDlg.h"
#include "OGrafik.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COGrafik::COGrafikV(VMEMORY vmSpeicher, const char* pcKlassenName, const char* pcFensterName, unsigned int uiIDElementA)
{
	COElementV(vmSpeicher);

	vstFensterBau = (STFensterBau*)VMBlock(sizeof(STFensterBau));
	vstFensterBau->dwFensterStil = WS_CHILDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS;
	vstFensterBau->dwErweitertStil = WS_EX_LEFT | WS_EX_NOPARENTNOTIFY;
	vstFensterBau->uiIDElement = uiIDElementA;
	vstFensterBau->asFensterName = "pcFensterName";
	BYTE ucBytes_Name = (BYTE)StrLength(pcKlassenName);
	vstFensterBau->vbKlassenName = VMBlock(ucBytes_Name + 1); vstFensterBau->vbKlassenName[ucBytes_Name] = 0;
	MemCopy(vstFensterBau->vbKlassenName, pcKlassenName, ucBytes_Name);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COGrafik::FensterStil(DWORD dwFensterStilA)
{
 if(vstFensterBau) vstFensterBau->dwFensterStil = dwFensterStilA;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COGrafik::ErweitertStil(DWORD dwErweitertStilA)
{
 if(vstFensterBau) vstFensterBau->dwErweitertStil = dwErweitertStilA | WS_EX_NOPARENTNOTIFY;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COGrafik::ErstellFenster(HWND hWndHaupt, long lHoheA, long lBreiteA, long lPos_x, long lPos_y)
{
 if(vstFensterBau){
	 while(FindWindowEx(hWndHaupt, nullptr, nullptr, vstFensterBau->asFensterName.c_Str())) vstFensterBau->asFensterName += "A";

	 lHohe = lHoheA; lBreite = lBreiteA; ptPosition.x = lPos_x; ptPosition.y = lPos_y;

	 hWndElement = CreateWindowEx(vstFensterBau->dwErweitertStil, vstFensterBau->vbKlassenName, vstFensterBau->asFensterName.c_Str(), vstFensterBau->dwFensterStil,
	 	                            ptPosition.x, ptPosition.y, lBreite, lHohe, hWndHaupt, (HMENU)vstFensterBau->uiIDElement, hInstance, this);

	 if(hWndElement){
	 	 SetWindowLongPtr(hWndElement, GWLP_USERDATA, (LONG_PTR)this);
		 RECT rcClient; GetClientRect(hWndElement, &rcClient); WM_Size_Element(hWndElement, MAKELPARAM(rcClient.right, rcClient.bottom));
		 VMFrei(vstFensterBau->vbKlassenName); vstFensterBau->asFensterName.~COStringA(); VMFrei(vstFensterBau); vstFensterBau = nullptr;
	 }
 }
}
//-------------------------------------------------------------------------------------------------------------------------------------------
unsigned int __vectorcall RePag::GUI::COGrafik::IDElement(void)
{
 return GetWindowLongPtr(hWndElement, GWLP_ID);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COGrafik::SetzSichtbar(bool bSichtbar)
{
 if(bSichtbar) ShowWindow(hWndElement, SW_SHOWNORMAL);
 else{ if(hWndElement == GetFocus()) SetFocus(GetParent(hWndElement)); ShowWindow(hWndElement, SW_HIDE); }
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COGrafik::Sichtbar(void)
{
 return IsWindowVisible(hWndElement);
}
//-------------------------------------------------------------------------------------------------------------------------------------------