/****************************************************************************
	ODatum.cpp
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
#include "ODatum.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
char pcFalschesDatumZeitText[] = "Das Datum ist ungültig.\nEs wird wieder auf seinen vorherigen Wert zurück gesetzt.";
char pcFalschesDatum[] = "Falsches Datum";
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::CODatum* __vectorcall RePag::GUI::CODatumV(const char* pcFensterName, unsigned int uiIDElement)
{
 CODatum* vTextZeile = (CODatum*)VMBlock(VMDialog(), sizeof(CODatum));
 vTextZeile->CODatumV(VMDialog(), pcFensterName, uiIDElement);
 return vTextZeile;
}
//---------------------------------------------------------------------------
RePag::GUI::CODatum* __vectorcall RePag::GUI::CODatumV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement)
{
 CODatum* vTextZeile = (CODatum*)VMBlock(vmSpeicher, sizeof(CODatum));
 vTextZeile->CODatumV(vmSpeicher, pcFensterName, uiIDElement);
 return vTextZeile;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_Datum(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
	CODatum* pDatum;
	switch(uiMessage){
		case WM_CREATE    : ((CODatum*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create(hWnd);
												return NULL;
		case WM_SIZE      : pDatum = (CODatum*)GetWindowLongPtr(hWnd, GWL_USERDATA);
												if(pDatum) pDatum->WM_Size_Element(hWnd, lParam);
												else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
												return NULL;
		//case WM_SETFOCUS    : pDatum = (CODatum*)GetWindowLongPtr(hWnd, GWL_USERDATA);
				//                   SetFocus(pDatum->edDatum->HWND_Element());
				//								  return NULL;
		case WM_KILLFOCUS : ((CODatum*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_KillFocus();
												return NULL;
		case WM_COMMAND   : pDatum = (CODatum*)GetWindowLongPtr(hWnd, GWL_USERDATA);
												if(!pDatum->WM_Command(wParam)) return NULL;
												else PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
												break;
		case WM_LBUTTONUP : pDatum = (CODatum*)GetWindowLongPtr(hWnd, GWL_USERDATA);
												pDatum->ThreadSicher_Anfang();
												if(pDatum->WM_LButtonUp(lParam)){
													if(pDatum->pfnWM_LButtonUp)	pDatum->pfnWM_LButtonUp(pDatum);
													else PostMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWnd, GWLP_ID), wParam), lParam);
												}
												pDatum->ThreadSicher_Ende();
												return NULL;
		case WM_PAINT     : ((CODatum*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_Paint();
												return NULL;
		case WM_NCDESTROY : pDatum = (CODatum*)GetWindowLongPtr(hWnd, GWL_USERDATA);
												if(pDatum->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pDatum->htEffekt_Timer, INVALID_HANDLE_VALUE);
												VMFreiV(pDatum);
												return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//---------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_EditDatum(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
	CODatum::COEditDatum* pEditDatum;
	switch(uiMessage){
		case WM_CREATE       : ((CODatum::COEditDatum*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create(hWnd);
													 return NULL;
		case WM_SIZE         : pEditDatum = (CODatum::COEditDatum*)GetWindowLongPtr(hWnd, GWL_USERDATA);
													 if(pEditDatum) pEditDatum->WM_Size_Element(hWnd, lParam);
													 else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													 return NULL;
		case WM_SETFOCUS     : ((CODatum::COEditDatum*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_SetFocus();
														return NULL;
		case WM_KILLFOCUS    : ((CODatum::COEditDatum*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_KillFocus();
														return NULL;
		case WM_KEYDOWN      :	((CODatum::COEditDatum*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_KeyDown(wParam, lParam);
														return NULL;
		case WM_CHAR         :	pEditDatum = (CODatum::COEditDatum*)GetWindowLongPtr(hWnd, GWL_USERDATA);
														pEditDatum->ThreadSicher_Anfang();
														pEditDatum->WM_Char(wParam);
														pEditDatum->WM_Char_EditDatum(wParam);
														pEditDatum->ThreadSicher_Ende();
														return NULL;
		case WM_COMMAND       : pEditDatum = (CODatum::COEditDatum*)GetWindowLongPtr(hWnd, GWL_USERDATA);
														if(!pEditDatum->WM_Command(wParam)) return NULL;
														else if(!pEditDatum->WM_Command_EditDatum(wParam)) return NULL;
														else if(pEditDatum->pfnWM_Command){
															pEditDatum->ThreadSicher_Anfang();
															if(!pEditDatum->pfnWM_Command(pEditDatum, wParam)){ pEditDatum->ThreadSicher_Ende(); return NULL; }
															pEditDatum->ThreadSicher_Ende();
														}
														else PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
														break;
		case WM_CONTEXTMENU   : ((CODatum::COEditDatum*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_ContexMenu(lParam);
														return NULL;
		case WM_MOUSEMOVE     : ((CODatum::COEditDatum*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_MouseMove(wParam, lParam);
														return NULL;
		case WM_LBUTTONDOWN   : ((CODatum::COEditDatum*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_LButtonDown(wParam, lParam);
														return NULL;
		case WM_LBUTTONUP     : ((CODatum::COEditDatum*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_LButtonUp(wParam, lParam);
														return NULL;
		case WM_LBUTTONDBLCLK : ((CODatum::COEditDatum*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_LButtonDBClick(lParam);
														return NULL;
		case WM_PAINT         : pEditDatum = (CODatum::COEditDatum*)GetWindowLongPtr(hWnd, GWL_USERDATA);
														pEditDatum->ThreadSicher_Anfang();
														pEditDatum->WM_Paint();
														pEditDatum->WM_Paint_EditDatum();
														pEditDatum->ThreadSicher_Ende();
														return NULL;
		case WM_NCDESTROY     : pEditDatum = (CODatum::COEditDatum*)GetWindowLongPtr(hWnd, GWL_USERDATA);
														if(pEditDatum->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pEditDatum->htEffekt_Timer, INVALID_HANDLE_VALUE);
														pEditDatum->pDatum->edDatum = NULL; VMFreiV(pEditDatum);
														return NULL;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::CODatumV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA)
{
 COGrafikV(vmSpeicher, pcRePag_Datum, pcFensterName, uiIDElementA);

 FensterStil(WS_CHILDWINDOW | WS_CLIPSIBLINGS);

 hFont = NULL;
 lfSchrift.lfHeight = -13;
 lfSchrift.lfWidth = 0;
 lfSchrift.lfFaceName[0] = '\0';
 lfSchrift.lfEscapement = GM_COMPATIBLE;
 lfSchrift.lfOrientation = GM_COMPATIBLE;
 lfSchrift.lfItalic = FALSE;
 lfSchrift.lfUnderline = FALSE;
 lfSchrift.lfStrikeOut = FALSE;
 lfSchrift.lfWeight = FW_DONTCARE;
 lfSchrift.lfCharSet = DEFAULT_CHARSET;
 lfSchrift.lfOutPrecision = OUT_DEVICE_PRECIS;
 lfSchrift.lfClipPrecision = CLIP_DEFAULT_PRECIS;
 lfSchrift.lfQuality = DEFAULT_QUALITY;
 lfSchrift.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

 stSchriftfarbe.ucRot = stSchriftfarbe.ucGrun = stSchriftfarbe.ucBlau = 0;
 stKopf_Hintergrundfarbe.ucRot = 180; stKopf_Hintergrundfarbe.ucGrun = 125; stKopf_Hintergrundfarbe.ucBlau = 90;

 lBreite_Kalender = ucSpaltenbreite = ucZeilenhohe = ucErsterWochentag = ucLetzterTag = ucJahr_Pos_x = 0;

 COStringA asTemp = COStringA(pcFensterName); asTemp += "_edEditDatum"; 
 edDatum = (COEditDatum*)VMBlock(vmSpeicher, sizeof(COEditDatum));
 edDatum->COEditDatumV(vmSpeicher, asTemp.c_Str(), ++uiIDElementA, this);
 lHohe_EditDatum = 20;

 asTemp = pcFensterName; asTemp += "_lbMonat";
 lbMonat = COListBoxV(vmSpeicher, asTemp.c_Str(), ++uiIDElementA);
 lbMonat->Hintergrundfarbe(255, 255, 255);
 lbMonat->Schriftfarbe(0, 0, 128);

 DWORD dwKalenderInfo = CAL_SMONTHNAME1; int iBytes; VMBLOCK vbText;
 for(BYTE ucMonat = 0; ucMonat < 12; ucMonat++){
   iBytes = GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, dwKalenderInfo, nullptr, NULL, nullptr);
	 vbText = VMBlock(iBytes + 1); vbText[iBytes] = 0;
   GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, dwKalenderInfo, vbText, iBytes, nullptr);
	 lbMonat->Text_NeueZeile(vbText); VMFrei(vbText);
   dwKalenderInfo++;
 }

 GetLocalTime(&stDatum);
 bAnordnung_DatumZeit = true;

 pfnWM_LButtonUp = nullptr;
 pfnWM_Char_Return = nullptr;
 pfnWM_Char_Escape = nullptr;
 pfnWM_Command = nullptr;
}
//---------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::GUI::CODatum::COFreiV(void)
{
 if(edDatum) DestroyWindow(edDatum->HWND_Element()); 
 DestroyWindow(lbMonat->HWND_Element());
 return ((COElement*)this)->COFreiV();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::COEditDatum::COEditDatumV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA, CODatum* pDatumA)
{
 COEditZeileV(vmSpeicher, pcRePag_EditDatum, pcFensterName, uiIDElementA);

 pDatum = pDatumA;

 bNeueMaske = true;
 ucZeichenVorgabe = ZV_ZIFFERN;
 Zeichenmaske("99'.'99'.'9999");
 vasFormat_Datum = COStringAV(vmSpeicher, "dd'.'MM'.'yyyy");
 vasFormat_Zeit = COStringAV(vmSpeicher);

 AppendMenu(hMenu, MF_SEPARATOR, NULL, NULL);
 AppendMenu(hMenu, MF_STRING, IDM_DATUM, "Kalender	Strg+K");
}
//---------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::GUI::CODatum::COEditDatum::COFreiV(void)
{
	VMFreiV(vasFormat_Datum); VMFreiV(vasFormat_Zeit);
	return ((COEditZeile*)this)->COFreiV();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::WM_Paint(void)
{
 ThreadSicher_Anfang();
 PAINTSTRUCT stPaint;
 BeginPaint(hWndElement, &stPaint);
 WM_Paint_Hintergrund(stPaint);

 SelectObject(stPaint.hdc, hFont);
 SetBkMode(stPaint.hdc, TRANSPARENT);
 SetTextColor(stPaint.hdc, RGB(stSchriftfarbe.ucRot, stSchriftfarbe.ucGrun, stSchriftfarbe.ucBlau));

 SYSTEMTIME stMonat = stDatum; stMonat.wDay = 1; DWORD dwKalenderInfo = CAL_SABBREVDAYNAME1; char pcWochentag_1[3]; pcWochentag_1[2] = 0; char pcWochentag_2[3]; pcWochentag_2[2] = 0;
 GetDateFormat(LOCALE_USER_DEFAULT, NULL, &stMonat, "ddd", pcWochentag_1, 3);
 for(BYTE ucTag = 0; ucTag < 7; ucTag++){
   GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, dwKalenderInfo++, pcWochentag_2, 3, nullptr);
	 if(!StrCompare(pcWochentag_1, 2, pcWochentag_2, 2)){ (ucTag == 6 ? stMonat.wDayOfWeek = 0 : stMonat.wDayOfWeek = ucTag + 1); break; }
 }
 ucErsterWochentag = stMonat.wDayOfWeek;

 BYTE ucWochenMonat = 5;
 switch(stMonat.wMonth){
		case 1  : 
		case 3  : 
		case 5  : 
		case 7  : 
		case 8  : 
		case 10 :
		case 12 : if(!stMonat.wDayOfWeek || stMonat.wDayOfWeek == 6) ucWochenMonat++;
							ucLetzterTag = 31;
							break;
		case 4  :
		case 6  :
		case 9  : 
		case 11 : if(!stMonat.wDayOfWeek) ucWochenMonat++;
							ucLetzterTag = 30;
							break;
		case 2  : stMonat.wDay = 29;
							if(!GetDateFormat(LOCALE_USER_DEFAULT, NULL, &stMonat, nullptr, nullptr, NULL)){
								if(stMonat.wDayOfWeek == 1) ucWochenMonat--;
								ucLetzterTag = 28;
							}
							else ucLetzterTag = 29;	         
							break;
 }

 ucSpaltenbreite = lBreite / 7; ucZeilenhohe = lHohe / (ucWochenMonat + 1);

 RECT rcKopf; rcKopf.right = lBreite; rcKopf.left = 0; rcKopf.top = 0; rcKopf.bottom = ucZeilenhohe;
 HBRUSH hBrush = CreateSolidBrush(RGB(stKopf_Hintergrundfarbe.ucRot, stKopf_Hintergrundfarbe.ucGrun, stKopf_Hintergrundfarbe.ucBlau));
 SelectObject(stPaint.hdc, hBrush); FillRect(stPaint.hdc, &rcKopf, hBrush); DeleteObject(hBrush);

 SIZE stZeichengrosse; POINT ptText; char c11Zahl[11];
 ULONGtoCHAR(c11Zahl, stMonat.wYear);
 GetTextExtentPoint32(stPaint.hdc, c11Zahl, 4, &stZeichengrosse);
 ptText.y = (ucZeilenhohe - stZeichengrosse.cy) / 2; ucJahr_Pos_x = lBreite - stZeichengrosse.cx - ucZeilenhohe;
 ExtTextOut(stPaint.hdc, ucJahr_Pos_x, ptText.y, NULL, nullptr, c11Zahl, 4, nullptr);

 dwKalenderInfo = CAL_SMONTHNAME1 + stMonat.wMonth; int iBytes; VMBLOCK vbMonat;
 iBytes = GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, --dwKalenderInfo, nullptr, NULL, nullptr);
 vbMonat = VMBlock(iBytes + 1); vbMonat[iBytes] = 0;
 GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, dwKalenderInfo, vbMonat, iBytes, nullptr);
 ExtTextOut(stPaint.hdc, 5, ptText.y, NULL, nullptr, vbMonat, StrLength(vbMonat), nullptr); VMFrei(vbMonat);

 hBrush = CreateSolidBrush(RGB(230, 230, 230));
 HPEN hPen = CreatePen(PS_SOLID, 1, RGB(230, 230, 230));
 SelectObject(stPaint.hdc, hBrush); SelectObject(stPaint.hdc, hPen);
 POINT ptKnopf3[3];
 ptKnopf3[0].x = lBreite - ucZeilenhohe / 4; ptKnopf3[0].y = (long)(ucZeilenhohe / 2);
 ptKnopf3[1].x = (long)(lBreite - ucZeilenhohe * 0.75); ptKnopf3[1].y = ucZeilenhohe / 4;
 ptKnopf3[2].x = ptKnopf3[1].x; ptKnopf3[2].y = (long)(ucZeilenhohe * 0.75);
 Polygon(stPaint.hdc, ptKnopf3, 3);

 ptKnopf3[0].x = lBreite - stZeichengrosse.cx - (long)(ucZeilenhohe * 1.75); ptKnopf3[0].y = (long)(ucZeilenhohe / 2);
 ptKnopf3[1].x = lBreite - stZeichengrosse.cx - (long)(ucZeilenhohe * 1.25); ptKnopf3[1].y = ucZeilenhohe / 4;
 ptKnopf3[2].x = ptKnopf3[1].x; ptKnopf3[2].y = (long)(ucZeilenhohe * 0.75);
 Polygon(stPaint.hdc, ptKnopf3, 3);
 DeleteObject(hBrush); DeleteObject(hPen);

 BYTE ucWochentag, ucTag = 0; ptText.y += ucZeilenhohe;
 if(!stMonat.wDayOfWeek){ ucWochentag = 6; ptText.x = ucSpaltenbreite * 6;}  
 else{ ucWochentag = stMonat.wDayOfWeek - 1; ptText.x = ucWochentag * ucSpaltenbreite;}

 for(BYTE ucWoche = 0; ucWoche < ucWochenMonat; ucWoche++){
	 do{
		 if(++ucTag < 10){ ULONGtoCHAR(c11Zahl, ucTag);
			 GetTextExtentPoint32(stPaint.hdc, c11Zahl, 1, &stZeichengrosse);
			 ExtTextOut(stPaint.hdc, ptText.x + (ucSpaltenbreite - stZeichengrosse.cx) / 2, ptText.y, NULL, nullptr, c11Zahl, 1, nullptr);
		 }
		 else{ ULONGtoCHAR(c11Zahl, ucTag);
			 GetTextExtentPoint32(stPaint.hdc, c11Zahl, 2, &stZeichengrosse);
			 ExtTextOut(stPaint.hdc, ptText.x + (ucSpaltenbreite - stZeichengrosse.cx) / 2, ptText.y, NULL, nullptr, c11Zahl, 2, nullptr);
		 }
     ptText.x += ucSpaltenbreite;
	 }
	 while(++ucWochentag < 7 && ucTag < ucLetzterTag);
	 ucWochentag = 0; ptText.x = 0; ptText.y += ucZeilenhohe;
 }
 EndPaint(hWndElement, &stPaint);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::COEditDatum::WM_Paint_EditDatum(void)
{
 ThreadSicher_Anfang();
 HDC hdc = GetDC(hWndElement);
 RECT rcKnopf; rcKnopf.right = lBreite; rcKnopf.left = lBreite - lHohe; rcKnopf.top = 0; rcKnopf.bottom = lHohe;
 HBRUSH hBrush = CreateSolidBrush(RGB(20, 100, 210));
 SelectObject(hdc, hBrush);
 FillRect(hdc, &rcKnopf, hBrush);
 DeleteObject(hBrush);

 hBrush = CreateSolidBrush(RGB(230, 230, 230));
 HPEN hPen = CreatePen(PS_SOLID, 1, RGB(230, 230, 230));
 SelectObject(hdc, hBrush); SelectObject(hdc, hPen);
 POINT points3[3];
 points3[0].x = lBreite - lHohe / 4; points3[0].y = lHohe / 4;
 points3[1].x = (long)(lBreite - lHohe * 0.75); points3[1].y = lHohe / 4;
 points3[2].x = lBreite - lHohe / 2; points3[2].y = (long)(lHohe * 0.75);
 Polygon(hdc, points3, 3);
 DeleteObject(hBrush); DeleteObject(hPen); ReleaseDC(hWndElement, hdc);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::WM_Create(HWND hWnd)
{
 hWndElement = hWnd; LOGFONT lfTemp;

 edDatum->Schriftart(Schrift(lfTemp));
 edDatum->ErstellFenster(GetParent(hWndElement), lHohe_EditDatum, lBreite, ptPosition.x, ptPosition.y - lHohe_EditDatum); 
 NeuesFenster(lHohe - lHohe_EditDatum, lBreite, ptPosition.x, ptPosition.y);

 SetzWochentag(); edDatum->DatumZeitText(); 
 hFont = CreateFontIndirect(&lfSchrift);

 lbMonat->Schriftart(Schrift(lfTemp));
 lbMonat->ErstellFenster(hWnd, lHohe - 20, lBreite, 0, 20);
 SCROLLBARINFO stScrollBarInfo; stScrollBarInfo.cbSize = sizeof(SCROLLBARINFO);
 GetScrollBarInfo(lbMonat->HWND_Element(), OBJID_VSCROLL, &stScrollBarInfo);
 stScrollBarInfo.dxyLineButton += lbMonat->BreitesteZeile();
 if(lBreite > stScrollBarInfo.dxyLineButton) lbMonat->NeueFensterBreite(stScrollBarInfo.dxyLineButton);
 lbMonat->SetzSichtbar(false); EnableWindow(lbMonat->HWND_Element(), true);
 lbMonat->SetzSelectIndex(stDatum.wMonth - 1);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::WM_KillFocus(void)
{
 ThreadSicher_Anfang();
 if(lbMonat->Sichtbar()) SetFocus(edDatum->HWND_Element());
 else if(IsWindowVisible(hWndElement)){ ShowWindow(hWndElement, SW_HIDE);
	 RECT stRect; HWND hWndZeichnen_1, hWndZeichnen_2 = GetParent(hWndElement); 
	 while(IsChild(hWndZeichnen_1 = GetParent(hWndZeichnen_2), hWndZeichnen_2)) hWndZeichnen_2 = hWndZeichnen_1;
	 ((COElement*)GetWindowLongPtr(hWndZeichnen_2, GWL_USERDATA))->UpdateFenster(&Fenster(stRect), true, false);
 }
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::CODatum::WM_Command(WPARAM wParam)
{
 if(LOWORD(wParam) == GetWindowLongPtr(hWndElement, GWLP_ID) + 2){
	 ThreadSicher_Anfang();
   stDatum.wMonth = lbMonat->SelectIndex() + 1;
   lbMonat->SetzSichtbar(false);
	 UpdateFenster(nullptr, true, false);
	 ThreadSicher_Ende();
	 return false;
 }
 return true;
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::CODatum::COEditDatum::WM_Command_EditDatum(WPARAM wParam)
{
 ThreadSicher_Anfang();
 if(LOWORD(wParam) == IDM_DATUM){ 
	 if(TextzuSYSTEMTIME()){ SetWindowPos(pDatum->HWND_Element(), HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE); SetFocus(pDatum->HWND_Element()); }
	 else{ MessageBox(hWndElement, pcFalschesDatumZeitText, pcFalschesDatum, MB_ICONERROR); DatumZeitText(); }
	 ThreadSicher_Ende();
	 return false;
 }
 else{ 
	 if(pDatum->pfnWM_Command){
	   bool bReturn = pDatum->pfnWM_Command(pDatum, wParam);
     ThreadSicher_Ende();
	   return bReturn;
   }
 }
 ThreadSicher_Ende();
 return true;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::COEditDatum::WM_Char_EditDatum(WPARAM wParam)
{
 HDC hdc; SIZE stZeichengrosse; BYTE ucStelle;
 switch(wParam){
 		case VK_RETURN : ThreadSicher_Anfang();
										 if(pDatum->pfnWM_Char_Return) pDatum->pfnWM_Char_Return(pDatum);
										 ThreadSicher_Ende();
										 break;
		case VK_ESCAPE : ThreadSicher_Anfang();
										 if(pDatum->pfnWM_Char_Escape) pDatum->pfnWM_Char_Escape(pDatum);
										 ThreadSicher_Ende();
										 break;
		default        : ThreadSicher_Anfang();
										 if(IsWindowVisible(pDatum->HWND_Element())) SendMessage(pDatum->HWND_Element(), WM_KILLFOCUS, NULL, NULL);
										 if(ZeichenVorgabe(wParam)){ ucStelle = ulZeichenPos;
											 if(!TextzuSYSTEMTIME()) ucStelle--;											
											 DatumZeitText(); 
											 //SetFocus(hWndElement);	
											 ulZeichenPos = ucStelle;                        
											 hdc = GetDC(hWndElement); SelectObject(hdc, hFont);	
											 GetTextExtentPoint32(hdc, vasInhalt->c_Str(), ulZeichenPos, &stZeichengrosse);
											 ptCaret.x = stZeichengrosse.cx;	
											 SetCaretPos(ptCaret.x, ptCaret.y);
											 ReleaseDC(hWndElement, hdc);
										 }
											ThreadSicher_Ende();
											break;
 }
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::CODatum::WM_LButtonUp(LPARAM lParam)
{
 BYTE ucWoche = HIWORD(lParam) / ucZeilenhohe; 
 BYTE ucWochentag = LOWORD(lParam) / ucSpaltenbreite; (ucWochentag == 6 ? ucWochentag = 0 : ucWochentag++);
 if(!ucWoche){
	 if(LOWORD(lParam) > lBreite - ucZeilenhohe){ 
		 if(lbMonat->Sichtbar()) lbMonat->SetzSichtbar(false);
		 stDatum.wYear++; UpdateFenster(nullptr, true, false);
	 }
	 else if(LOWORD(lParam) < ucJahr_Pos_x && LOWORD(lParam) > ucJahr_Pos_x - ucZeilenhohe){
		 if(lbMonat->Sichtbar()) lbMonat->SetzSichtbar(false);
		 stDatum.wYear--; UpdateFenster(nullptr, true, false);
	 }
	 else if(LOWORD(lParam) < ucJahr_Pos_x - ucZeilenhohe){ long lTemp;
	   lbMonat->NeuesFenster(lHohe - ucZeilenhohe, lbMonat->Breite(lTemp), lbMonat->Pos_X(lTemp), ucZeilenhohe); lbMonat->SetzSichtbar(true);}
	   return false;
 }
 else{ (ucErsterWochentag ? stDatum.wDay = 8 - ucErsterWochentag : stDatum.wDay = 1);
	 if(!ucWochentag){ stDatum.wDay += --ucWoche * 7;
     if(stDatum.wDay > ucLetzterTag) stDatum.wDay = ucLetzterTag;
	 }
	 else if(!--ucWoche){
		 if(ucErsterWochentag && ucWochentag > ucErsterWochentag) stDatum.wDay -= 7 - ucWochentag;
		 else stDatum.wDay = 1;
	 }
	 else{ stDatum.wDay += --ucWoche * 7 + ucWochentag;
	   if(stDatum.wDay > ucLetzterTag) stDatum.wDay = ucLetzterTag;
	 }
 }

 SetzWochentag(); edDatum->DatumZeitText();
 if(lbMonat->Sichtbar()) lbMonat->SetzSichtbar(false);
 ShowWindow(hWndElement, SW_HIDE);

 RECT stRect; HWND hWndZeichnen_1, hWndZeichnen_2 = GetParent(hWndElement); 
 while(IsChild(hWndZeichnen_1 = GetParent(hWndZeichnen_2), hWndZeichnen_2)) hWndZeichnen_2 = hWndZeichnen_1;
 ((COElement*)GetWindowLongPtr(hWndZeichnen_2, GWL_USERDATA))->UpdateFenster(&Fenster(stRect), true, false);
 return true;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::COEditDatum::WM_LButtonUp(WPARAM wParam, LPARAM lParam)
{
 ThreadSicher_Anfang();
 if(LOWORD(lParam) > lBreite - lHohe){
	 if(TextzuSYSTEMTIME()){ SetWindowPos(pDatum->HWND_Element(), HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE); SetFocus(pDatum->HWND_Element()); }
	 else{ MessageBox(hWndElement, pcFalschesDatumZeitText, pcFalschesDatum, MB_ICONERROR); DatumZeitText(); }
 }

 if(pfnWM_LButtonUp) pfnWM_LButtonUp(this, wParam, lParam);
 else PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), lParam);
 ThreadSicher_Ende();
 ReleaseCapture();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::COEditDatum::WM_LButtonDBClick(LPARAM lParam)
{
 ThreadSicher_Anfang();
 if(TextzuSYSTEMTIME()){ SetWindowPos(pDatum->HWND_Element(), HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE); SetFocus(pDatum->HWND_Element()); }
 else{ MessageBox(hWndElement, pcFalschesDatumZeitText, pcFalschesDatum, MB_ICONERROR); DatumZeitText(); }
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
long& __vectorcall RePag::GUI::CODatum::Hohe(long& lHoheA)
{
 ThreadSicher_Anfang();
 lHoheA = lHohe + lHohe_EditDatum;
 ThreadSicher_Ende();
 return lHoheA;
}
//---------------------------------------------------------------------------------------------------------------------------------------
long& __vectorcall RePag::GUI::CODatum::Hohe_EditDatum(long& lHoheA)
{
 ThreadSicher_Anfang();
 lHoheA = lHohe_EditDatum;
 ThreadSicher_Ende();
 return lHoheA;
}
//---------------------------------------------------------------------------------------------------------------------------------------
long& __vectorcall RePag::GUI::CODatum::Breite_EditDatum(long& lBreiteA)
{
 ThreadSicher_Anfang();
 edDatum->Breite(lBreiteA);
 ThreadSicher_Ende();
 return lBreiteA;
}
//---------------------------------------------------------------------------------------------------------------------------------------
RECT& __vectorcall RePag::GUI::CODatum::Fenster(RECT& rcFenster)
{
 ThreadSicher_Anfang();
 rcFenster.left = ptPosition.x;
 rcFenster.top = ptPosition.y - lHohe_EditDatum;
 (lBreite_Kalender ? rcFenster.right = ptPosition.x + lBreite_Kalender : rcFenster.right = ptPosition.x + lBreite);
 rcFenster.bottom = ptPosition.y + lHohe;
 ThreadSicher_Ende();
 return rcFenster;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::NeueFensterPosition(long lPos_x, long lPos_y)
{
 ThreadSicher_Anfang();
 ptPosition.x = lPos_x; ptPosition.y = lPos_y + lHohe_EditDatum;

 MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lBreite, lHohe, IsWindowVisible(hWndElement));
 edDatum->NeueFensterPosition(ptPosition.x, ptPosition.y - lHohe_EditDatum);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::NeueFensterPosition(POINT& ptPositionA)
{
 ThreadSicher_Anfang();
 ptPosition = ptPositionA;
 ptPosition.y += lHohe_EditDatum;

 MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lBreite, lHohe, IsWindowVisible(hWndElement));
 edDatum->NeueFensterPosition(ptPosition.x, ptPosition.y - lHohe_EditDatum);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::NeuesFenster(long lHoheA, long lBreiteA, long lPos_x, long lPos_y)
{
 ThreadSicher_Anfang();
 lHohe = lHoheA; ptPosition.x = lPos_x; ptPosition.y = lPos_y + lHohe_EditDatum;
 (lBreite_Kalender ? lBreite = lBreite_Kalender : lBreite = lBreiteA);

 InvalidateRect(hWndElement, nullptr, false);
 MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lBreite, lHohe, IsWindowVisible(hWndElement));
 edDatum->NeuesFenster(lHohe_EditDatum, lBreiteA, lPos_x, lPos_y);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::NeueFensterGrosse(long lHoheA, long lBreiteA)
{
 ThreadSicher_Anfang();
 lHohe = lHoheA - lHohe_EditDatum; 
 (lBreite_Kalender > lBreiteA ? lBreite = lBreite_Kalender : lBreite = lBreiteA);

 InvalidateRect(hWndElement, nullptr, false);
 MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lBreite, lHohe, IsWindowVisible(hWndElement));
 edDatum->NeueFensterGrosse(lHohe_EditDatum, lBreiteA);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::NeueFensterHohe(long lHoheA)
{
 ThreadSicher_Anfang();
 lHohe = lHoheA - lHohe_EditDatum;

 InvalidateRect(hWndElement, nullptr, false);
 MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lBreite, lHohe, IsWindowVisible(hWndElement));
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::NeueFensterBreite(long lBreiteA)
{
 ThreadSicher_Anfang();
 (lBreite_Kalender > lBreiteA ? lBreite = lBreite_Kalender : lBreite = lBreiteA);

 InvalidateRect(hWndElement, nullptr, false);
 MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lBreite, lHohe, IsWindowVisible(hWndElement));
 edDatum->NeueFensterBreite(lBreiteA);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::AndernFensterGrosse(long lHoheA, long lBreiteA)
{
 ThreadSicher_Anfang();
 lHohe += lHoheA; 
 (lBreite_Kalender > lBreiteA ? lBreite += lBreite_Kalender : lBreite += lBreiteA);

 InvalidateRect(hWndElement, nullptr, false);
 MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lBreite, lHohe, IsWindowVisible(hWndElement));
 edDatum->AndernFensterGrosse(0, lBreiteA);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::AndernFensterPosition(long lPos_x, long lPos_y)
{
 ThreadSicher_Anfang();
 ptPosition.x += lPos_x; ptPosition.y += lPos_y;

 MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lBreite, lHohe, IsWindowVisible(hWndElement));
 edDatum->AndernFensterPosition(ptPosition.x, ptPosition.y);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::Schriftfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
 ThreadSicher_Anfang();
 stSchriftfarbe.ucRot = ucRot; stSchriftfarbe.ucGrun = ucGrun; stSchriftfarbe.ucBlau = ucBlau;
 edDatum->Schriftfarbe(ucRot, ucGrun, ucBlau);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::Hintergrundfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
 ThreadSicher_Anfang();
 stHintergrundfarbe.ucRot = ucRot; stHintergrundfarbe.ucGrun = ucGrun; stHintergrundfarbe.ucBlau = ucBlau;
 if(hWndElement) UpdateFenster(nullptr, true, true);
 edDatum->Hintergrundfarbe(ucRot, ucGrun, ucBlau);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::Kopf_Hintergrundfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
 ThreadSicher_Anfang();
 stKopf_Hintergrundfarbe.ucRot = ucRot; stKopf_Hintergrundfarbe.ucGrun = ucGrun; stKopf_Hintergrundfarbe.ucBlau = ucBlau;
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::Schriftart(LOGFONT& lfSchriftA)
{
 if(!hWndElement){ lfSchrift = lfSchriftA; edDatum->Schriftart(lfSchriftA); lbMonat->Schriftart(lfSchriftA);}
}
//---------------------------------------------------------------------------------------------------------------------------------------
LOGFONT& __vectorcall RePag::GUI::CODatum::Schrift(LOGFONT& lfSchriftA)
{
 ThreadSicher_Anfang();
 lfSchriftA = lfSchrift;
 ThreadSicher_Ende();
 return lfSchriftA;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::SetzVerfugbar(bool bVerfugbar)
{
 ThreadSicher_Anfang();
 if(bVerfugbar){ 
	 if(!IsWindowVisible(edDatum->HWND_Element())) ShowWindow(edDatum->HWND_Element(), SW_SHOW);
	 if(!IsWindowEnabled(edDatum->HWND_Element())) EnableWindow(edDatum->HWND_Element(), bVerfugbar);
	 else if(edDatum->HWND_Element() == GetFocus()) SendMessage(edDatum->HWND_Element(), WM_SETFOCUS, NULL, NULL);
 }
 else{ if(edDatum->HWND_Element() == GetFocus()) SetFocus(GetParent(hWndElement)); EnableWindow(edDatum->HWND_Element(), bVerfugbar);}
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::SetzSichtbar(bool bSichtbar)
{
 ThreadSicher_Anfang();
 if(bSichtbar) ShowWindow(edDatum->HWND_Element(), SW_SHOWNA);
 else{ if(edDatum->HWND_Element() == GetFocus()) SetFocus(GetParent(hWndElement)); ShowWindow(edDatum->HWND_Element(), SW_HIDE); ShowWindow(hWndElement, SW_HIDE);}
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::CODatum::Sichtbar(void)
{
 return IsWindowVisible(edDatum->HWND_Element());
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::ErweiterteBreite_Kalender(long lBreiteA)
{
 ThreadSicher_Anfang();
 lBreite_Kalender = lBreiteA;
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::SetzHohe_EditDatum(long lHoheA)
{
 ThreadSicher_Anfang();
 lHohe_EditDatum = lHoheA;
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::Zeichenvorgabe_EditDatum_ZVSICHTBAR(bool bSichtbar)
{
 ThreadSicher_Anfang();
 if(bSichtbar) edDatum->Zeichenvorgabe(ZV_ZIFFERN | ZV_SICHTBAR);
 else edDatum->Zeichenvorgabe(ZV_ZIFFERN);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::Schriftausrichtung_EditDatum(unsigned char ucSchriftausrichtung)
{
 ThreadSicher_Anfang();
 edDatum->Schriftausrichtung(ucSchriftausrichtung);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
COStringA* __vectorcall RePag::GUI::CODatum::Datumformat(COStringA* pasFormat_Datum)
{
 ThreadSicher_Anfang();
 if(pasFormat_Datum) *pasFormat_Datum = *edDatum->vasFormat_Datum;
 ThreadSicher_Ende();
 return pasFormat_Datum;
}
//---------------------------------------------------------------------------------------------------------------------------------------
COStringA* __vectorcall RePag::GUI::CODatum::Zeitformat(COStringA* pasFormat_Zeit)
{
 ThreadSicher_Anfang();
 if(pasFormat_Zeit) *pasFormat_Zeit = *edDatum->vasFormat_Zeit;
 ThreadSicher_Ende();
 return pasFormat_Zeit;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::DatumZeitFormat(const char* pcFormat_Datum, const char* pcFormat_Zeit, bool bAnordnung_DatumZeitA)
{
 ThreadSicher_Anfang();
 if(!pcFormat_Datum) pcFormat_Datum = "dd'.'MM'.'yyyy";
 BYTE ucMaske, ucStelle = 0, ucLange = (BYTE)StrLength(pcFormat_Datum); bool bUngultig = false; char c2Buchstabe[2]; c2Buchstabe[1] = 0; COStringA asMaske_Datum;

 do{  
	 switch(pcFormat_Datum[ucStelle]){
		 case 0x64 : ucMaske = 1; while(pcFormat_Datum[++ucStelle] == 0x64 && ucStelle < ucLange) ucMaske++;
								 switch(ucMaske){
										case 1  : asMaske_Datum += "d"; break;
										case 2  : asMaske_Datum += "dd"; break;
										case 3  : asMaske_Datum += "ddd"; break;
										case 4  : asMaske_Datum += "dddd"; break;
										default : bUngultig = true; ucLange = 0;
								 }
								 break; 
	   case 0x4d : ucMaske = 1; while(pcFormat_Datum[++ucStelle] == 0x4d && ucStelle < ucLange) ucMaske++;
								 switch(ucMaske){
										case 1  : asMaske_Datum += "M"; break;
										case 2  : asMaske_Datum += "MM"; break;
										case 3  : asMaske_Datum += "MMM"; break;
										case 4  : asMaske_Datum += "MMMM"; break;
										default : bUngultig = true; ucLange = 0;
								 }
                 break; 
	   case 0x79 : ucMaske = 1; while(pcFormat_Datum[++ucStelle] == 0x79 && ucStelle < ucLange) ucMaske++;
								 switch(ucMaske){
										case 1  : asMaske_Datum += "y"; break;
										case 2  : asMaske_Datum += "yy"; break;
										case 4  : asMaske_Datum += "yyyy"; break;
										default : bUngultig = true; ucLange = 0;
								 }
                 break; 
		 case 0x27 : asMaske_Datum += "'";
			           while(pcFormat_Datum[++ucStelle] != 0x27 && ucStelle < ucLange){
			             *c2Buchstabe = pcFormat_Datum[ucStelle];   
			             asMaske_Datum += c2Buchstabe;
								 } 
			           asMaske_Datum += "'"; ucStelle++;
			           break; 
		 default   : bUngultig = true; ucLange = 0; break;
	 }
 }
 while(ucStelle < ucLange);

 if(pcFormat_Zeit && !bUngultig){ COStringA asMaske_Zeit; ucStelle = 0, ucLange = (BYTE)StrLength(pcFormat_Zeit);
	 do{  
		 switch(pcFormat_Zeit[ucStelle]){
			 case 0x68 : ucMaske = 1; while(pcFormat_Zeit[++ucStelle] == 0x68 && ucStelle < ucLange) ucMaske++;
									 switch(ucMaske){
											case 1  : asMaske_Zeit += "h"; break;
											case 2  : asMaske_Zeit += "hh"; break;
											default : bUngultig = true; ucLange = 0;
									 }
									 break; 
			 case 0x48 : ucMaske = 1; while(pcFormat_Zeit[++ucStelle] == 0x48 && ucStelle < ucLange) ucMaske++;
									 switch(ucMaske){
											case 1  : asMaske_Zeit += "H"; break;
											case 2  : asMaske_Zeit += "HH"; break;
											default : bUngultig = true; ucLange = 0;
									 }
									 break; 
			 case 0x6d : ucMaske = 1; while(pcFormat_Zeit[++ucStelle] == 0x6d && ucStelle < ucLange) ucMaske++;
									 switch(ucMaske){
											case 1  : asMaske_Zeit += "m"; break;
											case 2  : asMaske_Zeit += "mm"; break;
											default : bUngultig = true; ucLange = 0;
									 }
									 break; 
			 case 0x73 : ucMaske = 1; while(pcFormat_Zeit[++ucStelle] == 0x73 && ucStelle < ucLange) ucMaske++;
									 switch(ucMaske){
											case 1  : asMaske_Zeit += "s"; break;
											case 2  : asMaske_Zeit += "ss"; break;
											default : bUngultig = true; ucLange = 0;
									 }
									 break; 
			 case 0x74 : ucMaske = 1; while(pcFormat_Zeit[++ucStelle] == 0x74 && ucStelle < ucLange) ucMaske++;
									 switch(ucMaske){
											case 1  : asMaske_Zeit += "t"; break;
											case 2  : asMaske_Zeit += "tt"; break;
											default : bUngultig = true; ucLange = 0;
									 }
									 break;
			 case 0x27 : asMaske_Zeit += "'";
									 while(pcFormat_Zeit[++ucStelle] != 0x27 && ucStelle < ucLange){
										 *c2Buchstabe = pcFormat_Zeit[ucStelle];   
										 asMaske_Zeit += c2Buchstabe;
									 } 
									 asMaske_Zeit += "'"; ucStelle++;
									 break; 
			 default   : bUngultig = true; ucLange = 0; break;
		 }
	 }
	 while(ucStelle < ucLange);

	 if(!bUngultig) *edDatum->vasFormat_Zeit = asMaske_Zeit;
 }

 if(!bUngultig){ bAnordnung_DatumZeit = bAnordnung_DatumZeitA; edDatum->bNeueMaske = true; *edDatum->vasFormat_Datum = asMaske_Datum; edDatum->DatumZeitText(); }
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::COEditDatum::DatumZeitText(void)
{
 int iBytes_Datum = GetDateFormat(LOCALE_USER_DEFAULT, NULL, &pDatum->stDatum, vasFormat_Datum->c_Str(), nullptr, NULL);
 VMBLOCK vbDatum = VMBlock(iBytes_Datum); vbDatum[iBytes_Datum - 1] = 0; VMBLOCK vbDatum_A = nullptr;
 GetDateFormat(LOCALE_USER_DEFAULT, NULL, &pDatum->stDatum, vasFormat_Datum->c_Str(), vbDatum, iBytes_Datum);

 int iBytes_Zeit = 0; VMBLOCK vbZeit = nullptr; VMBLOCK vbZeit_A = nullptr;
 if(vasFormat_Zeit->Length()){
	 iBytes_Zeit = GetTimeFormat(LOCALE_USER_DEFAULT, NULL, &pDatum->stDatum, vasFormat_Zeit->c_Str(), nullptr, NULL);
	 vbZeit = VMBlock(iBytes_Zeit); vbZeit[iBytes_Zeit - 1] = 0;
	 GetTimeFormat(LOCALE_USER_DEFAULT, NULL, &pDatum->stDatum, vasFormat_Zeit->c_Str(), vbZeit, iBytes_Zeit);
 }

 if(bNeueMaske){ BYTE ucMaske, ucStelle_Maske = 0, ucStelle_DatumZeit = 0, ucLange = vasFormat_Datum->Length(); COStringA asMaske_Datum;
   char c2Buchstabe[2]; c2Buchstabe[1] = 0; WORD wJahr;	bNeueMaske = false; char pc5KurzText[5]; int iBytes; DWORD dwKalenderInfo; VMBLOCK vbText;
	 do{  
		 switch((*vasFormat_Datum)[ucStelle_Maske]){
			 case 0x64 : ucMaske = 1; while((*vasFormat_Datum)[++ucStelle_Maske] == 0x64 && ucStelle_Maske < ucLange) ucMaske++;
									 switch(ucMaske){
											case 1  : if(pDatum->stDatum.wDay < 10){ 
																	if(!vbDatum_A){ vbDatum_A = VMBlock(iBytes_Datum + 1); vbDatum_A[iBytes_Datum] = 0;
																		vbDatum_A[ucStelle_DatumZeit] = 0x20;
																		MemCopy(vbDatum_A, vbDatum, ucStelle_DatumZeit);
																		MemCopy(&vbDatum_A[ucStelle_DatumZeit + 1], &vbDatum[ucStelle_DatumZeit], iBytes_Datum - ucStelle_DatumZeit - 1);
																		VMFrei(vbDatum); vbDatum = nullptr;
																	}
																	else{ vbDatum = VMBlock(iBytes_Datum + 1); vbDatum[iBytes_Datum] = 0;
																		vbDatum[ucStelle_DatumZeit] = 0x20;
																		MemCopy(vbDatum, vbDatum_A, ucStelle_DatumZeit);
																		MemCopy(&vbDatum[ucStelle_DatumZeit + 1], &vbDatum_A[ucStelle_DatumZeit], iBytes_Datum - ucStelle_DatumZeit - 1);
 																		VMFrei(vbDatum_A); vbDatum_A = nullptr;
																	}
																	iBytes_Datum++;
																} 
											case 2  : asMaske_Datum += "99"; ucStelle_DatumZeit += 2; break;
											case 3  : (asMaske_Datum.Length() && asMaske_Datum[asMaske_Datum.Length() - 1] == 0x27 ? asMaske_Datum.ShortRightOne() : asMaske_Datum += "'");
																(!pDatum->stDatum.wDayOfWeek ? GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, CAL_SABBREVDAYNAME7, pc5KurzText, 3, nullptr)
																	: GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, CAL_SABBREVDAYNAME1 + pDatum->stDatum.wDayOfWeek - 1, pc5KurzText, 3, nullptr));
																pc5KurzText[2] = 0x27; pc5KurzText[3] = 0x00; asMaske_Datum += pc5KurzText;
																ucStelle_DatumZeit += 2; bNeueMaske = true; break;
											case 4  : (asMaske_Datum.Length() && asMaske_Datum[asMaske_Datum.Length() - 1] == 0x27 ? asMaske_Datum.ShortRightOne() : asMaske_Datum += "'");
																(!pDatum->stDatum.wDayOfWeek ? dwKalenderInfo = CAL_SDAYNAME7: dwKalenderInfo = CAL_SDAYNAME1 + pDatum->stDatum.wDayOfWeek - 1);
																iBytes = GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, dwKalenderInfo, nullptr, NULL, nullptr);
																vbText = VMBlock(iBytes + 1); vbText[iBytes - 1] = 0x27; vbText[iBytes] = 0x00;
																GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, dwKalenderInfo, vbText, --iBytes, nullptr);
																asMaske_Datum += vbText; ucStelle_DatumZeit += iBytes; VMFrei(vbText);
																bNeueMaske = true; break;
									 } break;
			 case 0x4d : ucMaske = 1; while((*vasFormat_Datum)[++ucStelle_Maske] == 0x4d && ucStelle_Maske < ucLange) ucMaske++;
									 switch(ucMaske){
											case 1  : if(pDatum->stDatum.wMonth < 10){ 
																	if(!vbDatum_A){ vbDatum_A = VMBlock(iBytes_Datum + 1); vbDatum_A[iBytes_Datum] = 0;
																		vbDatum_A[ucStelle_DatumZeit] = 0x20;
																		MemCopy(vbDatum_A, vbDatum, ucStelle_DatumZeit);
																		MemCopy(&vbDatum_A[ucStelle_DatumZeit + 1], &vbDatum[ucStelle_DatumZeit], iBytes_Datum - ucStelle_DatumZeit - 1);
																		VMFrei(vbDatum); vbDatum = nullptr;
																	}
																	else{ vbDatum = VMBlock(iBytes_Datum + 1); vbDatum[iBytes_Datum] = 0;
																		vbDatum[ucStelle_DatumZeit] = 0x20;
																		MemCopy(vbDatum, vbDatum_A, ucStelle_DatumZeit);
																		MemCopy(&vbDatum[ucStelle_DatumZeit + 1], &vbDatum_A[ucStelle_DatumZeit], iBytes_Datum - ucStelle_DatumZeit - 1);
 																		VMFrei(vbDatum_A); vbDatum_A = nullptr;
																	}
																	iBytes_Datum++;
																}
											case 2  : asMaske_Datum += "99"; ucStelle_DatumZeit += 2; break;
											case 3  : (asMaske_Datum.Length() && asMaske_Datum[asMaske_Datum.Length() - 1] == 0x27 ? asMaske_Datum.ShortRightOne() : asMaske_Datum += "'");
																GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, CAL_SABBREVMONTHNAME1 + pDatum->stDatum.wMonth - 1, pc5KurzText, 4, nullptr);
																pc5KurzText[3] = 0x27; pc5KurzText[4] = 0x00; asMaske_Datum += pc5KurzText;
																ucStelle_DatumZeit += 3; bNeueMaske = true; break;
											case 4  : (asMaske_Datum.Length() && asMaske_Datum[asMaske_Datum.Length() - 1] == 0x27 ? asMaske_Datum.ShortRightOne() : asMaske_Datum += "'");
																dwKalenderInfo = CAL_SMONTHNAME1 + pDatum->stDatum.wMonth;
																iBytes = GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, --dwKalenderInfo, nullptr, NULL, nullptr);
																vbText = VMBlock(iBytes + 1); vbText[iBytes - 1] = 0x27; vbText[iBytes] = 0x00;
																GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, dwKalenderInfo, vbText, --iBytes, nullptr);
																asMaske_Datum += vbText; ucStelle_DatumZeit += iBytes; VMFrei(vbText);
																bNeueMaske = true; break;
									 } break;
			 case 0x79 : ucMaske = 1; while((*vasFormat_Datum)[++ucStelle_Maske] == 0x79 && ucStelle_Maske < ucLange) ucMaske++;
									 switch(ucMaske){
											case 1  : wJahr = pDatum->stDatum.wYear; wJahr <<= 12; wJahr >>= 12;
												        if(wJahr < 10){
												          if(!vbDatum_A){ vbDatum_A = VMBlock(iBytes_Datum + 1); vbDatum_A[iBytes_Datum] = 0;
																		vbDatum_A[ucStelle_DatumZeit] = 0x20;
																		MemCopy(vbDatum_A, vbDatum, ucStelle_DatumZeit);
																		MemCopy(&vbDatum_A[ucStelle_DatumZeit + 1], &vbDatum[ucStelle_DatumZeit], iBytes_Datum - ucStelle_DatumZeit - 1);
																		VMFrei(vbDatum); vbDatum = NULL;
																  }
																  else{ vbDatum = VMBlock(iBytes_Datum + 1); vbDatum[iBytes_Datum] = 0;
																		vbDatum[ucStelle_DatumZeit] = 0x20;
																		MemCopy(vbDatum, vbDatum_A, ucStelle_DatumZeit);
																		MemCopy(&vbDatum[ucStelle_DatumZeit + 1], &vbDatum_A[ucStelle_DatumZeit], iBytes_Datum - ucStelle_DatumZeit - 1);
 																		VMFrei(vbDatum_A); vbDatum_A = NULL;
																  }
																	iBytes_Datum++;
																}
											case 2  : asMaske_Datum += "99"; ucStelle_DatumZeit += 2; break;
											case 4  : asMaske_Datum += "9999"; ucStelle_DatumZeit += 4; break;
									 }
									 break; 
			 case 0x27 : (asMaske_Datum.Length() && asMaske_Datum[asMaske_Datum.Length() - 1] == 0x27 ? asMaske_Datum.ShortRightOne() : asMaske_Datum += "'");
									 while((*vasFormat_Datum)[++ucStelle_Maske] != 0x27 && ucStelle_Maske < ucLange){
										 *c2Buchstabe = (*vasFormat_Datum)[ucStelle_Maske];   
									   asMaske_Datum += c2Buchstabe;
										 ucStelle_DatumZeit++;
									 } 
									 asMaske_Datum += "'"; ucStelle_Maske++; 
									 break; 
		 }
	 }
	 while(ucStelle_Maske < ucLange);

	 if(iBytes_Zeit){ ucStelle_Maske = 0, ucStelle_DatumZeit = 0, ucLange = vasFormat_Zeit->Length(); COStringA asMaske_Zeit;
		 do{  
			 switch((*vasFormat_Zeit)[ucStelle_Maske]){
				 case 0x68 : ucMaske = 1; while((*vasFormat_Zeit)[++ucStelle_Maske] == 0x68 && ucStelle_Maske < ucLange) ucMaske++;
										 switch(ucMaske){
												case 1  : if(pDatum->stDatum.wHour < 10){ 
																		if(!vbZeit_A){ vbZeit_A = VMBlock(iBytes_Zeit + 1); vbZeit_A[iBytes_Zeit] = 0;
																			vbZeit_A[ucStelle_DatumZeit] = 0x20;
																			MemCopy(vbZeit_A, vbZeit, ucStelle_DatumZeit);
																			MemCopy(&vbZeit_A[ucStelle_DatumZeit + 1], &vbZeit[ucStelle_DatumZeit], iBytes_Zeit - ucStelle_DatumZeit - 1);
																			VMFrei(vbZeit); vbZeit = nullptr;
																		}
																		else{ vbZeit = VMBlock(iBytes_Zeit + 1); vbZeit[iBytes_Zeit] = 0;
																			vbZeit[ucStelle_DatumZeit] = 0x20;
																			MemCopy(vbZeit, vbZeit_A, ucStelle_DatumZeit);
																			MemCopy(&vbZeit[ucStelle_DatumZeit + 1], &vbZeit_A[ucStelle_DatumZeit], iBytes_Zeit - ucStelle_DatumZeit - 1);
 																			VMFrei(vbZeit_A); vbZeit_A = nullptr;
																		}
																		iBytes_Zeit++;
																	} 
												case 2  : asMaske_Zeit += "99"; ucStelle_DatumZeit += 2; break;
										 } break;
				 case 0x48 : ucMaske = 1; while((*vasFormat_Zeit)[++ucStelle_Maske] == 0x48 && ucStelle_Maske < ucLange) ucMaske++;
										 switch(ucMaske){
												case 1  : if(pDatum->stDatum.wHour < 10){ 
																		if(!vbZeit_A){ vbZeit_A = VMBlock(iBytes_Zeit + 1); vbZeit_A[iBytes_Zeit] = 0;
																			vbZeit_A[ucStelle_DatumZeit] = 0x20;
																			MemCopy(vbZeit_A, vbZeit, ucStelle_DatumZeit);
																			MemCopy(&vbZeit_A[ucStelle_DatumZeit + 1], &vbZeit[ucStelle_DatumZeit], iBytes_Zeit - ucStelle_DatumZeit - 1);
																			VMFrei(vbZeit); vbZeit = nullptr;
																		}
																		else{ vbZeit = VMBlock(iBytes_Zeit + 1); vbZeit[iBytes_Zeit] = 0;
																			vbZeit[ucStelle_DatumZeit] = 0x20;
																			MemCopy(vbZeit, vbZeit_A, ucStelle_DatumZeit);
																			MemCopy(&vbZeit[ucStelle_DatumZeit + 1], &vbZeit_A[ucStelle_DatumZeit], iBytes_Zeit - ucStelle_DatumZeit - 1);
 																			VMFrei(vbZeit_A); vbZeit_A = nullptr;
																		}
																		iBytes_Zeit++;
																	}
												case 2  : asMaske_Zeit += "99"; ucStelle_DatumZeit += 2; break;
										 } break;
				 case 0x6d : ucMaske = 1; while((*vasFormat_Zeit)[++ucStelle_Maske] == 0x6d && ucStelle_Maske < ucLange) ucMaske++;
										 switch(ucMaske){
												case 1  : if(pDatum->stDatum.wMinute < 10){
																		if(!vbZeit_A){ vbZeit_A = VMBlock(iBytes_Zeit + 1); vbZeit_A[iBytes_Zeit] = 0;
																			vbZeit_A[ucStelle_DatumZeit] = 0x20;
																			MemCopy(vbZeit_A, vbZeit, ucStelle_DatumZeit);
																			MemCopy(&vbZeit_A[ucStelle_DatumZeit + 1], &vbZeit[ucStelle_DatumZeit], iBytes_Zeit - ucStelle_DatumZeit - 1);
																			VMFrei(vbZeit); vbZeit = nullptr;
																		}
																		else{ vbZeit = VMBlock(iBytes_Zeit + 1); vbZeit[iBytes_Zeit] = 0;
																			vbZeit[ucStelle_DatumZeit] = 0x20;
																			MemCopy(vbZeit, vbZeit_A, ucStelle_DatumZeit);
																			MemCopy(&vbZeit[ucStelle_DatumZeit + 1], &vbZeit_A[ucStelle_DatumZeit], iBytes_Zeit - ucStelle_DatumZeit - 1);
 																			VMFrei(vbZeit_A); vbZeit_A = nullptr;
																		}
																		iBytes_Zeit++;
																	}
												case 2  : asMaske_Zeit += "99"; ucStelle_DatumZeit += 2; break;
										 }
										 break; 
				 case 0x73 : ucMaske = 1; while((*vasFormat_Zeit)[++ucStelle_Maske] == 0x73 && ucStelle_Maske < ucLange) ucMaske++;
										 switch(ucMaske){
												case 1  : if(pDatum->stDatum.wSecond < 10){
																		if(!vbZeit_A){ vbZeit_A = VMBlock(iBytes_Zeit + 1); vbZeit_A[iBytes_Zeit] = 0;
																			vbZeit_A[ucStelle_DatumZeit] = 0x20;
																			MemCopy(vbZeit_A, vbZeit, ucStelle_DatumZeit);
																			MemCopy(&vbZeit_A[ucStelle_DatumZeit + 1], &vbZeit[ucStelle_DatumZeit], iBytes_Zeit - ucStelle_DatumZeit - 1);
																			VMFrei(vbZeit); vbZeit = nullptr;
																		}
																		else{ vbZeit = VMBlock(iBytes_Zeit + 1); vbZeit[iBytes_Zeit] = 0;
																			vbZeit[ucStelle_DatumZeit] = 0x20;
																			MemCopy(vbZeit, vbZeit_A, ucStelle_DatumZeit);
																			MemCopy(&vbZeit[ucStelle_DatumZeit + 1], &vbZeit_A[ucStelle_DatumZeit], iBytes_Zeit - ucStelle_DatumZeit - 1);
 																			VMFrei(vbZeit_A); vbZeit_A = nullptr;
																		}
																		iBytes_Zeit++;
																	}
												case 2  : asMaske_Zeit += "99"; ucStelle_DatumZeit += 2; break;
										 }
										 break; 
				 case 0x74 : ucMaske = 1; while((*vasFormat_Zeit)[++ucStelle_Maske] == 0x74 && ucStelle_Maske < ucLange) ucMaske++;
										 switch(ucMaske){
												case 1  : (asMaske_Zeit.Length() && asMaske_Zeit[asMaske_Zeit.Length() - 1] == 0x74 ? asMaske_Zeit.ShortRightOne() : asMaske_Zeit += "'");
																	if(pDatum->stDatum.wHour < 12) asMaske_Zeit += "A'";
																	else asMaske_Zeit += "P'";
																	ucStelle_DatumZeit++; bNeueMaske = true; break;
												case 2  : (asMaske_Zeit.Length() && asMaske_Zeit[asMaske_Zeit.Length() - 1] == 0x74 ? asMaske_Zeit.ShortRightOne() : asMaske_Zeit += "'");
																	if(pDatum->stDatum.wHour < 12) asMaske_Zeit += "AM'";
																	else asMaske_Zeit += "PM'";
																	ucStelle_DatumZeit += 2; bNeueMaske = true; break;
										 }
										 break; 
				 case 0x27 : (asMaske_Zeit.Length() && asMaske_Zeit[asMaske_Zeit.Length() - 1] == 0x27 ? asMaske_Zeit.ShortRightOne() : asMaske_Zeit += "'");
										 while((*vasFormat_Zeit)[++ucStelle_Maske] != 0x27 && ucStelle_Maske < ucLange){
											 *c2Buchstabe = (*vasFormat_Zeit)[ucStelle_Maske];   
											 asMaske_Zeit += c2Buchstabe;
											 ucStelle_DatumZeit++;
										 } 
										 asMaske_Zeit += "'"; ucStelle_Maske++; 
										 break; 
			 }
		 }
		 while(ucStelle_Maske < ucLange);

		 if(pDatum->bAnordnung_DatumZeit){ asMaske_Datum += asMaske_Zeit;	Zeichenmaske(asMaske_Datum.c_Str());}
		 else{ asMaske_Zeit += asMaske_Datum; Zeichenmaske(asMaske_Zeit.c_Str());}
	 }
	 else Zeichenmaske(asMaske_Datum.c_Str());
 }

 if(!iBytes_Zeit) (vbDatum_A ? Text(vbDatum_A), VMFrei(vbDatum_A) : Text(vbDatum), VMFrei(vbDatum));
 else{ COStringA asDatumZeit;
	 if(pDatum->bAnordnung_DatumZeit){
     (vbDatum_A ? asDatumZeit = vbDatum_A, VMFrei(vbDatum_A) : asDatumZeit = vbDatum, VMFrei(vbDatum));
     (vbZeit_A ? asDatumZeit += vbZeit_A, VMFrei(vbZeit_A) : asDatumZeit += vbZeit, VMFrei(vbZeit));
	 }
	 else{
     (vbZeit_A ? asDatumZeit = vbZeit_A, VMFrei(vbZeit_A) : asDatumZeit = vbZeit, VMFrei(vbZeit));
     (vbDatum_A ? asDatumZeit += vbDatum_A, VMFrei(vbDatum_A) : asDatumZeit += vbDatum, VMFrei(vbDatum));
	 }
	 Text(asDatumZeit.c_Str());
 }
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::SetzWochentag(void)
{
 DWORD dwKalenderInfo = CAL_SABBREVDAYNAME1; char pcWochentag_1[3]; pcWochentag_1[2] = 0; char pcWochentag_2[3]; pcWochentag_2[2] = 0;
 GetDateFormat(LOCALE_USER_DEFAULT, NULL, &stDatum, "ddd", pcWochentag_1, 3);
 for(BYTE ucTag = 0; ucTag < 7; ucTag++){
   GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, dwKalenderInfo++, pcWochentag_2, 3, nullptr);
	 if(!StrCompare(pcWochentag_1, 2, pcWochentag_2, 2)){ (ucTag == 6 ? stDatum.wDayOfWeek = 0 : stDatum.wDayOfWeek = ucTag + 1); break;}
 }
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::CODatum::COEditDatum::TextzuSYSTEMTIME(void)
{
 BYTE ucStelle_DatumZeit = 0; 
 SYSTEMTIME stSystemTime = pDatum->stDatum; 

 if(pDatum->bAnordnung_DatumZeit){ 
	 if(DatumTextzuSYSTEMTIME(stSystemTime, ucStelle_DatumZeit)){
		 if(vasFormat_Zeit->Length()){
			 if(!ZeitTextzuSYSTEMTIME(stSystemTime, ucStelle_DatumZeit)) return false;
		 }
		 pDatum->stDatum = stSystemTime; pDatum->SetzWochentag(); return true;
	 }
 }
 else{
   if(vasFormat_Zeit->Length()){
		 if(!ZeitTextzuSYSTEMTIME(stSystemTime, ucStelle_DatumZeit)) return false;
	 }
	 if(DatumTextzuSYSTEMTIME(stSystemTime, ucStelle_DatumZeit)){ pDatum->stDatum = stSystemTime; pDatum->SetzWochentag(); return true;}
 }
 return false;
}
//---------------------------------------------------------------------------------------------------------------------------------------
inline bool __vectorcall RePag::GUI::CODatum::COEditDatum::DatumTextzuSYSTEMTIME(SYSTEMTIME& stSystemTime, unsigned char& ucStelle_DatumZeit)
{
	BYTE ucMaske, ucStelle_Maske = 0, ucLange = vasFormat_Datum->Length(); COStringA asZahl;
 do{  
	 switch((*vasFormat_Datum)[ucStelle_Maske]){
		 case 0x64 : ucMaske = 1; while((*vasFormat_Datum)[++ucStelle_Maske] == 0x64 && ucStelle_Maske < ucLange) ucMaske++;
								 switch(ucMaske){
										case 1  : 
										case 2  : vasInhalt->SubString(&asZahl, ucStelle_DatumZeit + 1, ucStelle_DatumZeit + 2);
											        if(asZahl[1] == 0x20 || asZahl[1] == 0x5f) asZahl[1] = 0x30;
											        asZahl.USHORT(stSystemTime.wDay); ucStelle_DatumZeit += 2; break;
										case 3  : ucStelle_DatumZeit += 2; break;
							      case 4  : (!pDatum->stDatum.wDayOfWeek ? ucStelle_DatumZeit += GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, CAL_SDAYNAME7, nullptr, NULL, nullptr) -  1
																: ucStelle_DatumZeit += GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, CAL_SDAYNAME1 + pDatum->stDatum.wDayOfWeek - 1, nullptr, NULL, nullptr) -  1);
															break;
								 } break;
		 case 0x4d : ucMaske = 1; while((*vasFormat_Datum)[++ucStelle_Maske] == 0x4d && ucStelle_Maske < ucLange) ucMaske++;
								 switch(ucMaske){
		                case 1  : 
										case 2  : vasInhalt->SubString(&asZahl, ucStelle_DatumZeit + 1, ucStelle_DatumZeit + 2);
											        if(asZahl[1] == 0x20 || asZahl[1] == 0x5f) asZahl[1] = 0x30;
											        asZahl.USHORT(stSystemTime.wMonth); ucStelle_DatumZeit += 2; break;
										case 3  : ucStelle_DatumZeit += 3; break;
										case 4  : ucStelle_DatumZeit += GetCalendarInfo(LOCALE_USER_DEFAULT, CAL_GREGORIAN, CAL_SMONTHNAME1 + pDatum->stDatum.wMonth - 1, nullptr, NULL, nullptr) -  1;
															break;
								 } break;
		 case 0x79 : ucMaske = 1; while((*vasFormat_Datum)[++ucStelle_Maske] == 0x79 && ucStelle_Maske < ucLange) ucMaske++;
								 switch(ucMaske){
										case 1  : 
										case 2  : vasInhalt->SubString(&asZahl, ucStelle_DatumZeit + 1, ucStelle_DatumZeit + 2);
											        if(asZahl[1] == 0x20 || asZahl[1] == 0x5f) asZahl[1] = 0x30;
											        asZahl.USHORT(stSystemTime.wYear);
											        stSystemTime.wYear += 2000; ucStelle_DatumZeit += 2; break;
										case 4  : vasInhalt->SubString(&asZahl, ucStelle_DatumZeit + 1, ucStelle_DatumZeit + 4);
											        ucMaske = 1;
															do{
																if(asZahl[ucMaske] == 0x20 || asZahl[ucMaske] == 0x5f) asZahl[ucMaske] = 0x30;
															}
											        while(++ucMaske < 4); 
											        asZahl.USHORT(stSystemTime.wYear); ucStelle_DatumZeit += 4; break;
								 }
								 break; 
		 case 0x27 : while((*vasFormat_Datum)[++ucStelle_Maske] != 0x27 && ucStelle_Maske < ucLange) ucStelle_DatumZeit++;
								 ucStelle_Maske++; break; 
	 }
 }
 while(ucStelle_Maske < ucLange);

 if(!GetDateFormat(LOCALE_USER_DEFAULT, NULL, &stSystemTime, vasFormat_Datum->c_Str(), nullptr, NULL)) return false;
 return true;
}
//---------------------------------------------------------------------------------------------------------------------------------------
inline bool __vectorcall RePag::GUI::CODatum::COEditDatum::ZeitTextzuSYSTEMTIME(SYSTEMTIME& stSystemTime, unsigned char& ucStelle_DatumZeit)
{
 BYTE ucMaske, ucStelle_Maske = 0, ucLange = vasFormat_Zeit->Length(); COStringA asZahl;
 do{  
	 switch((*vasFormat_Zeit)[ucStelle_Maske]){
		 case 0x68 : ucMaske = 1; while((*vasFormat_Zeit)[++ucStelle_Maske] == 0x68 && ucStelle_Maske < ucLange) ucMaske++;
								 switch(ucMaske){
										case 1  : 
										case 2  : vasInhalt->SubString(&asZahl, ucStelle_DatumZeit + 1, ucStelle_DatumZeit + 2);
											        if(asZahl[1] == 0x20 || asZahl[1] == 0x5f) asZahl[1] = 0x30;
											        asZahl.USHORT(stSystemTime.wHour); ucStelle_DatumZeit += 2; break;
								 } break;
		 case 0x48 : ucMaske = 1; while((*vasFormat_Zeit)[++ucStelle_Maske] == 0x48 && ucStelle_Maske < ucLange) ucMaske++;
								 switch(ucMaske){
										case 1  : 
										case 2  : vasInhalt->SubString(&asZahl, ucStelle_DatumZeit + 1, ucStelle_DatumZeit + 2);
											        if(asZahl[1] == 0x20 || asZahl[1] == 0x5f) asZahl[1] = 0x30;
											        asZahl.USHORT(stSystemTime.wHour); ucStelle_DatumZeit += 2; break;
								 } break;
		 case 0x6d : ucMaske = 1; while((*vasFormat_Zeit)[++ucStelle_Maske] == 0x6d && ucStelle_Maske < ucLange) ucMaske++;
								 switch(ucMaske){
										case 1  : 
										case 2  : vasInhalt->SubString(&asZahl, ucStelle_DatumZeit + 1, ucStelle_DatumZeit + 2);
											        if(asZahl[1] == 0x20 || asZahl[1] == 0x5f) asZahl[1] = 0x30;
											        asZahl.USHORT(stSystemTime.wMinute); ucStelle_DatumZeit += 2; break;
								 }
								 break; 
		 case 0x73 : ucMaske = 1; while((*vasFormat_Zeit)[++ucStelle_Maske] == 0x73 && ucStelle_Maske < ucLange) ucMaske++;
								 switch(ucMaske){
										case 1  : 
										case 2  : vasInhalt->SubString(&asZahl, ucStelle_DatumZeit + 1, ucStelle_DatumZeit + 2);
											        if(asZahl[1] == 0x20 || asZahl[1] == 0x5f) asZahl[1] = 0x30;
											        asZahl.USHORT(stSystemTime.wSecond); ucStelle_DatumZeit += 2; break;
								 }
								 break; 
		 case 0x74 : ucMaske = 1; while((*vasFormat_Zeit)[++ucStelle_Maske] == 0x74 && ucStelle_Maske < ucLange) ucMaske++;
								 switch(ucMaske){
										case 1  : 
										case 2  : ucStelle_DatumZeit += 2; break;
								 }
								 break; 
		 case 0x27 : while((*vasFormat_Zeit)[++ucStelle_Maske] != 0x27 && ucStelle_Maske < ucLange) ucStelle_DatumZeit++;
								 ucStelle_Maske++; break; 
	 }
 }
 while(ucStelle_Maske < ucLange);
 if(!GetTimeFormat(LOCALE_USER_DEFAULT, NULL, &stSystemTime, vasFormat_Zeit->c_Str(), NULL, NULL)) return false;
 return true;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::SetzDatum(COTime* pzDatum)
{
 ThreadSicher_Anfang();
 if(pzDatum){ stDatum = pzDatum->SystemTime(); SetzWochentag(); edDatum->DatumZeitText();}
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::CODatum::SetzDatum(STTime& stZeit)
{
 ThreadSicher_Anfang();
 SYSTEMTIME stSystemTime;
 stSystemTime.wYear = stZeit.usJahr; stSystemTime.wMonth = stZeit.ucMonat; stSystemTime.wDay = stZeit.ulTag; stSystemTime.wHour = stZeit.ucStunde;
 stSystemTime.wMinute = stZeit.ucMinute; stSystemTime.wSecond = stZeit.ucSekunde; stSystemTime.wMilliseconds = stZeit.usMillisekunde;
 if(GetDateFormat(LOCALE_USER_DEFAULT, NULL, &stSystemTime, nullptr, nullptr, NULL)){
	 stDatum = stSystemTime; SetzWochentag();
	 edDatum->DatumZeitText();
	 ThreadSicher_Ende();
	 return true;
 }
 ThreadSicher_Ende();
 return false;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::Today(void)
{
 ThreadSicher_Anfang();
 GetLocalTime(&stDatum); stDatum.wHour = 0; stDatum.wMinute = 0; stDatum.wSecond = 0; stDatum.wMilliseconds = 0;
 SetzWochentag(); 
 edDatum->DatumZeitText();
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::Morgen(void)
{
 ThreadSicher_Anfang();
 FILETIME stFileTime; ULARGE_INTEGER unZeit;
 GetLocalTime(&stDatum); stDatum.wHour = 0; stDatum.wMinute = 0; stDatum.wSecond = 0; stDatum.wMilliseconds = 0;

 SystemTimeToFileTime(&stDatum, &stFileTime);
 unZeit.HighPart = stFileTime.dwHighDateTime;
 unZeit.LowPart = stFileTime.dwLowDateTime;
 unZeit.QuadPart += 864000000000;
 stFileTime.dwHighDateTime = unZeit.HighPart;
 stFileTime.dwLowDateTime = unZeit.LowPart;
 FileTimeToSystemTime(&stFileTime, &stDatum);

 SetzWochentag(); 
 edDatum->DatumZeitText(); 
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::Lese(char* pcInhalt)
{
 if(pcInhalt){ FILETIME stFZeit; 
   ThreadSicher_Anfang();
   SystemTimeToFileTime(&stDatum, &stFZeit);
	 MemCopy(pcInhalt, &stFZeit.dwHighDateTime, 4); MemCopy(&pcInhalt[4], &stFZeit.dwLowDateTime, 4);
   ThreadSicher_Ende();
 }
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODatum::Schreibe(const char* pcInhalt)
{
	if(pcInhalt){	FILETIME stFZeit = {0};
   ThreadSicher_Anfang();
   MemCopy(&stFZeit.dwHighDateTime, pcInhalt, 4); MemCopy(&stFZeit.dwLowDateTime, &pcInhalt[4], 4);
	 FileTimeToSystemTime(&stFZeit, &stDatum);
	 SetzWochentag(); 
	 edDatum->DatumZeitText(); 
   ThreadSicher_Ende();
 }
}
//---------------------------------------------------------------------------------------------------------------------------------------
COStringA* __vectorcall RePag::GUI::CODatum::Datum(COStringA* pasDatum)
{
 ThreadSicher_Anfang();
 if(pasDatum) edDatum->Inhalt(pasDatum);
 ThreadSicher_Ende();
 return pasDatum;
}
//---------------------------------------------------------------------------------------------------------------------------------------
COTime* __vectorcall RePag::GUI::CODatum::ZEIT(COTime* pzDatum)
{
 ThreadSicher_Anfang();
 if(pzDatum) *pzDatum = stDatum;
 ThreadSicher_Ende();
 return pzDatum;
}
//---------------------------------------------------------------------------------------------------------------------------------------
SYSTEMTIME __vectorcall RePag::GUI::CODatum::SystemTime(void)
{
 ThreadSicher_Anfang();
 SYSTEMTIME stDatumA = stDatum;
 ThreadSicher_Ende();
 return stDatumA;
}
//---------------------------------------------------------------------------------------------------------------------------------------