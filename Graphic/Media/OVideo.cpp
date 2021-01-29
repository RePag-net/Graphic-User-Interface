/****************************************************************************
	OVideo.h
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

#include "HMedia.h"
#include "OVideo.h"
//---------------------------------------------------------------------------
RePag::GUI::COVideo* __vectorcall RePag::GUI::COVideoV(const char* pcFensterName, unsigned int uiIDElement)
{
 COVideo* vVideo = (COVideo*)VMBlock(VMDialog(), sizeof(COVideo));
 vVideo->COVideoV(VMDialog(), pcFensterName, uiIDElement);
 return vVideo;
}
//---------------------------------------------------------------------------
RePag::GUI::COVideo* __vectorcall RePag::GUI::COVideoV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement)
{
 COVideo* vVideo = (COVideo*)VMBlock(vmSpeicher, sizeof(COVideo));
 vVideo->COVideoV(vmSpeicher, pcFensterName, uiIDElement);
 return vVideo;
}
//---------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_Video(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
 COVideo* pVideo;
 switch(uiMessage){
    case WM_CREATE          : ((COVideo*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create(hWnd);
												      return NULL;
		case WM_SIZE            : pVideo = (COVideo*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
															if(pVideo) pVideo->WM_Size(lParam);
															else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													    return NULL;
		case WM_CHAR            : if(IsWindowEnabled(hWnd)) ((COVideo*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_Char(wParam);
													    return NULL;
		case WM_COMMAND         : pVideo = (COVideo*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			                        if(!pVideo->WM_Command(wParam)) return NULL;
			                        else if(pVideo->pfnWM_Command){ pVideo->ThreadSicher_Anfang();
																if(!pVideo->pfnWM_Command(pVideo, wParam)){ pVideo->ThreadSicher_Ende(); return NULL;}
																pVideo->ThreadSicher_Ende();
													    }
															else PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
													    break;
		case WM_CONTEXTMENU     : ((COVideo*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_ContexMenu(lParam);
			                        return NULL;
		case WM_LBUTTONDOWN     : SetFocus(hWnd);
			                        return NULL;
		case WM_LBUTTONUP       : PostMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWnd, GWLP_ID), wParam), lParam);
			                        break;
		case MCIWNDM_NOTIFYMODE : ((COVideo*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->MCIWNDM_NotifyMode(lParam);
			                        return NULL;
		case WM_PAINT           : ((COVideo*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_Paint();
										          return NULL;
		case WM_NCDESTROY       : pVideo = (COVideo*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
															if(pVideo->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pVideo->htEffekt_Timer, INVALID_HANDLE_VALUE);
                              if(pVideo->htPosition) DeleteTimerQueueTimer(TimerQueue(), pVideo->htPosition, INVALID_HANDLE_VALUE);
			                        VMFreiV(pVideo);
			                        return NULL;
 }
 return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//---------------------------------------------------------------------------
void __vectorcall RePag::GUI::COVideo::COVideoV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA)
{
 COMediaV(vmSpeicher, pcRePag_Video, pcFensterName, uiIDElementA);

 bBildformatErhalten = true;
 vasInfotext = COStringAV(vmSpeicher, "Kein Video");

 hMenu = CreatePopupMenu();
 AppendMenu(hMenu, MF_STRING, IDM_DATEI_OFFNEN, "Video Öffnen		Strg+O");
 AppendMenu(hMenu, MF_STRING, IDM_DATEI_SPEICHERNUNTER, "Video Speichern unter...");
 AppendMenu(hMenu, MF_STRING, IDM_DATEI_SPEICHERN, "Video Speichern		Strg+S");
 AppendMenu(hMenu, MF_STRING, IDM_DATEI_ENTFERNEN, "Video Entfernen		Strg+Umsch+Entf");

 AppendMenu(hMenu, MF_SEPARATOR, NULL, NULL);
 AppendMenu(hMenu, MF_STRING | MF_CHECKED, IDM_BILDFORMAT, "Videoformat erhalten");
}
//---------------------------------------------------------------------------
void __vectorcall RePag::GUI::COVideo::WM_Paint(void)
{
 ThreadSicher_Anfang();
 if(MCIWndGetMode(hWndMCI, NULL, 0) != MCI_MODE_NOT_READY){ RECT rcClient, rcMedia_Quelle, rcMedia_Ziel; 
	 MCIWndGetSource(hWndMCI, &rcMedia_Quelle);
	 GetClientRect(hWndElement, &rcClient);
	 rcClient.bottom -= 25;
	 if(bBildformatErhalten){ rcMedia_Ziel.top = rcMedia_Ziel.left = 0;
		 if(rcMedia_Quelle.bottom > rcClient.bottom){
			 rcMedia_Ziel.right = rcMedia_Quelle.right * rcClient.bottom / rcMedia_Quelle.bottom;
			 if(rcMedia_Ziel.right > rcClient.right){
				 rcMedia_Ziel.bottom = rcMedia_Quelle.bottom * rcClient.right / rcMedia_Quelle.right;
				 rcMedia_Ziel.right = rcMedia_Quelle.right * rcClient.right / rcMedia_Quelle.right;
			 }
			 else rcMedia_Ziel.bottom = rcMedia_Quelle.bottom * rcClient.bottom / rcMedia_Quelle.bottom;
		 }
		 else if(rcMedia_Quelle.right > rcClient.right){
			 rcMedia_Ziel.bottom = rcMedia_Quelle.bottom * rcClient.right / rcMedia_Quelle.right;
			 if(rcMedia_Ziel.bottom > rcClient.bottom){
				 rcMedia_Ziel.right = rcMedia_Quelle.right * rcClient.bottom / rcMedia_Quelle.bottom;
				 rcMedia_Ziel.bottom = rcMedia_Quelle.bottom * rcClient.bottom / rcMedia_Quelle.bottom;
			 }
			 else rcMedia_Ziel.right = rcMedia_Quelle.right * rcClient.right / rcMedia_Quelle.right;
		 }
		 else if(rcMedia_Quelle.bottom != rcClient.bottom || rcMedia_Quelle.right != rcClient.right){
			 rcMedia_Ziel.right = rcMedia_Quelle.right * rcClient.bottom / rcMedia_Quelle.bottom;
			 if(rcMedia_Ziel.right > rcClient.right){
				 rcMedia_Ziel.bottom = rcMedia_Quelle.bottom * rcClient.right / rcMedia_Quelle.right;
				 rcMedia_Ziel.right = rcMedia_Quelle.right * rcClient.right / rcMedia_Quelle.right;
			 }
			 else rcMedia_Ziel.bottom = rcMedia_Quelle.bottom * rcClient.bottom / rcMedia_Quelle.bottom;
		 }
		 else{ rcMedia_Ziel = rcMedia_Quelle;}
	 }
	 MCIWndPutDest(hWndMCI, &rcMedia_Ziel);
	 HDC hdc = GetDC(hWndMCI);
	 HBRUSH hBrush = CreateSolidBrush(RGB(stHintergrundfarbe.ucRot, stHintergrundfarbe.ucGrun, stHintergrundfarbe.ucBlau));
	 SelectObject(hdc, hBrush);
	 if(rcClient.bottom == rcMedia_Ziel.bottom) rcClient.left = rcMedia_Ziel.right;
	 else rcClient.top = rcMedia_Ziel.bottom;
	 FillRect(hdc, &rcClient, hBrush);
	 DeleteObject(hBrush);
	 ReleaseDC(hWndMCI, hdc);
 }
 else{ 
	 PAINTSTRUCT stPaint; 
	 BeginPaint(hWndElement, &stPaint);
	 WM_Paint_Hintergrund(stPaint); 
	 
	 SIZE stZeichengrosse; POINT ptText;
	 HFONT hFont = CreateFontIndirect(&lfSchrift);
	 SelectObject(stPaint.hdc, hFont);
   GetTextExtentPoint32(stPaint.hdc, vasInfotext->c_Str(), vasInfotext->Length(), &stZeichengrosse);
	 ptText.x = lBreite / 2 - stZeichengrosse.cx / 2;
	 ptText.y = (lHohe - ucRahmenbreite - 25) / 2 - stZeichengrosse.cy / 2;
	 SetBkMode(stPaint.hdc, TRANSPARENT);
	 SetTextColor(stPaint.hdc, RGB(stSchriftfarbe.ucRot, stSchriftfarbe.ucGrun, stSchriftfarbe.ucBlau));
	 ExtTextOut(stPaint.hdc, ptText.x, ptText.y, ETO_CLIPPED, &stPaint.rcPaint, vasInfotext->c_Str(), vasInfotext->Length(), nullptr);
	 DeleteObject(hFont);

	 EndPaint(hWndElement, &stPaint);
 }
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COVideo::WM_Command(WPARAM wParam)
{
 short sIDElement = LOWORD(wParam);

 if(sIDElement == GetWindowLongPtr(hWndElement, GWLP_ID) + 1){
	 ThreadSicher_Anfang();
	 switch(MCIWndGetMode(hWndMCI, NULL, 0)){
			 case MCI_MODE_STOP  : MCIWndPlay(hWndMCI); break;
			 case MCI_MODE_PLAY  : MCIWndStop(hWndMCI); break;
	 }
	 ThreadSicher_Ende(); 
	 return false;
 }
 else{
	 switch(LOWORD(wParam)){
			case IDM_DATEI_OFFNEN         : ThreadSicher_Anfang();
				                              if(IsWindowEnabled(hWndElement) && ucRechte & ZR_OFFNEN) DateiOffnen();
																			ThreadSicher_Ende(); return false;
			case IDM_DATEI_SPEICHERN      : ThreadSicher_Anfang();
				                              if(IsWindowEnabled(hWndElement) && ucRechte & ZR_SPEICHERN) DateiSpeichernUnter();
																			ThreadSicher_Ende(); return false;
			case IDM_DATEI_SPEICHERNUNTER : ThreadSicher_Anfang();
				                              if(IsWindowEnabled(hWndElement) && ucRechte & ZR_SPEICHERN) DateiSpeichernUnter();
																			ThreadSicher_Ende(); return false;
			case IDM_DATEI_ENTFERNEN      : ThreadSicher_Anfang();
				                              if(IsWindowEnabled(hWndElement) && ucRechte & ZR_LOSCHEN) DateiEntfernen();
																			ThreadSicher_Ende(); return false;
			case IDM_BILDFORMAT           : ThreadSicher_Anfang();
				                              if(bBildformatErhalten){ bBildformatErhalten = false; CheckMenuItem(hMenu, IDM_BILDFORMAT, MF_UNCHECKED); }
																		  else{ bBildformatErhalten = true; CheckMenuItem(hMenu, IDM_BILDFORMAT, MF_CHECKED); }
																			UpdateFenster(nullptr, true, false);
				                              ThreadSicher_Ende(); return false;
			default                       : return true;
	 }
 }
}
//---------------------------------------------------------------------------
void __vectorcall RePag::GUI::COVideo::DateiOffnen(void)
{
 OPENFILENAME stOffnenDatei; char c256Dateiname[256]; c256Dateiname[0] = 0; static char c256Filter[256];

 stOffnenDatei.lStructSize = sizeof(OPENFILENAME);
 stOffnenDatei.hwndOwner = hWndElement;
 stOffnenDatei.lpstrDefExt = "wmv"; stOffnenDatei.nFilterIndex = 0;
 stOffnenDatei.lpstrFilter = "Video WMV\0*.wmv\0Video MEPG\0*.mpg;*.mpeg\0Alle Dateien\0*.*\0\0";
 stOffnenDatei.lpstrCustomFilter = c256Filter; stOffnenDatei.nMaxCustFilter = 256;
 stOffnenDatei.lpstrFile = c256Dateiname; stOffnenDatei.nMaxFile = 256;
 stOffnenDatei.lpstrFileTitle = nullptr; stOffnenDatei.lpstrInitialDir = nullptr; stOffnenDatei.lpstrTitle = nullptr;
 stOffnenDatei.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING | OFN_EXTENSIONDIFFERENT | OFN_FORCESHOWHIDDEN | OFN_HIDEREADONLY;

 if(GetOpenFileName(&stOffnenDatei)){
	 *vasDateiname = c256Dateiname; *vasTypEndung = c256Dateiname; vasTypEndung->ShortLeft(stOffnenDatei.nFileExtension);
	 MCIOffnen();
	 bGeandert = true;
 }
 else if(CommDlgExtendedError()) MessageBox(hWndElement, pcDateifehlerOffnenText, pcDateifehlerOffnen, MB_ICONHAND);
}
//---------------------------------------------------------------------------
void __vectorcall RePag::GUI::COVideo::DateiSpeichernUnter(void)
{
 if(MCIWndGetMode(hWndMCI, nullptr, 0) == MCI_MODE_NOT_READY) return;

 OPENFILENAME stSpeichernDatei; char c256Dateiname[256]; c256Dateiname[0] = 0; static char c256Filter[256];

 stSpeichernDatei.lStructSize = sizeof(OPENFILENAME);
 stSpeichernDatei.hwndOwner = hWndElement;
 stSpeichernDatei.lpstrDefExt = "wmv"; stSpeichernDatei.nFilterIndex = 0;
 stSpeichernDatei.lpstrFilter = "Video WMV\0*.wmv\0Video MPEG\0*.mpg;*.mpeg\0Alle Dateien\0*.*\0\0";
 stSpeichernDatei.lpstrCustomFilter = c256Filter; stSpeichernDatei.nMaxCustFilter = 256;
 stSpeichernDatei.lpstrFile = c256Dateiname; stSpeichernDatei.nMaxFile = 256;
 stSpeichernDatei.lpstrFileTitle = nullptr; stSpeichernDatei.lpstrInitialDir = nullptr; stSpeichernDatei.lpstrTitle = nullptr;
 stSpeichernDatei.Flags = OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING | OFN_EXTENSIONDIFFERENT | OFN_FORCESHOWHIDDEN | OFN_HIDEREADONLY;

 if(GetSaveFileName(&stSpeichernDatei)){ 
	 if(*vasDateiname != c256Dateiname){
	   DateiSchreiben(c256Dateiname);
 	   *vasDateiname = c256Dateiname; *vasTypEndung = c256Dateiname; vasTypEndung->ShortLeft(stSpeichernDatei.nFileExtension);
	 }
 }
 else if(CommDlgExtendedError()) MessageBox(hWndElement, pcDateifehlerSpeichernText, pcDateifehlerSpeichern, MB_ICONHAND);
}
//---------------------------------------------------------------------------