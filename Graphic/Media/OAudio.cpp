/****************************************************************************
	OAudio.cpp
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

#include "HMedia.h"
#include "OAudio.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COAudio* __vectorcall RePag::GUI::COAudioV(const char* pcFensterName, unsigned int uiIDElement)
{
 COAudio* vAudio = (COAudio*)VMBlock(VMDialog(), sizeof(COAudio));
 vAudio->COAudioV(VMDialog(), pcFensterName, uiIDElement);
 return vAudio;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COAudio* __vectorcall RePag::GUI::COAudioV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement)
{
 COAudio* vAudio = (COAudio*)VMBlock(vmSpeicher, sizeof(COAudio));
 vAudio->COAudioV(vmSpeicher, pcFensterName, uiIDElement);
 return vAudio;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_Audio(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
 COAudio* pAudio;
 switch(uiMessage){
    case WM_CREATE          : ((COAudio*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create(hWnd);
												      return NULL;
		case WM_SIZE            : pAudio = (COAudio*)GetWindowLongPtr(hWnd, GWL_USERDATA);
															if(pAudio) pAudio->WM_Size(lParam);
															else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													    return NULL;
		case WM_CHAR            : if(IsWindowEnabled(hWnd)) ((COAudio*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_Char(wParam);
													    return NULL;
		case WM_COMMAND         : pAudio = (COAudio*)GetWindowLongPtr(hWnd, GWL_USERDATA);
			                        if(!pAudio->WM_Command(wParam)) return NULL;
			                        else if(pAudio->pfnWM_Command){ pAudio->ThreadSicher_Anfang();
																if(!pAudio->pfnWM_Command(pAudio, wParam)){ pAudio->ThreadSicher_Ende(); return NULL; }
																pAudio->ThreadSicher_Ende();
													    }
															else PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
													    break;
		case WM_CONTEXTMENU     : ((COAudio*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_ContexMenu(lParam);
			                        return NULL;
		case WM_LBUTTONDOWN     : SetFocus(hWnd);
			                        return NULL;
		case WM_LBUTTONUP       : PostMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWnd, GWLP_ID), wParam), lParam);
			                        break;
		case MCIWNDM_NOTIFYMODE : ((COAudio*)GetWindowLongPtr(hWnd, GWL_USERDATA))->MCIWNDM_NotifyMode(lParam);
			                        return NULL;
		case WM_PAINT           : ((COAudio*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_Paint();
										          return NULL;
		case WM_NCDESTROY       : pAudio = (COAudio*)GetWindowLongPtr(hWnd, GWL_USERDATA);
															if(pAudio->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pAudio->htEffekt_Timer, INVALID_HANDLE_VALUE);
															if(pAudio->htPosition) DeleteTimerQueueTimer(TimerQueue(), pAudio->htPosition, INVALID_HANDLE_VALUE);
			                        VMFreiV(pAudio);
			                        return NULL;
 }
 return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COAudio::COAudioV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA)
{
 COMediaV(vmSpeicher, pcRePag_Audio, pcFensterName, uiIDElementA);

 vasInfotext = COStringAV(vmSpeicher, "Kein Audio");

 hMenu = CreatePopupMenu();
 AppendMenu(hMenu, MF_STRING, IDM_DATEI_OFFNEN, "Audio Öffnen		Strg+O");
 AppendMenu(hMenu, MF_STRING, IDM_DATEI_SPEICHERNUNTER, "Audio Speichern unter...");
 AppendMenu(hMenu, MF_STRING, IDM_DATEI_SPEICHERN, "Audio Speichern		Strg+S");
 AppendMenu(hMenu, MF_STRING, IDM_DATEI_ENTFERNEN, "Audio Entfernen		Strg+Umsch+Entf");
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COAudio::WM_Paint(void)
{
 ThreadSicher_Anfang();
 PAINTSTRUCT stPaint; 
 BeginPaint(hWndElement, &stPaint);
 WM_Paint_Hintergrund(stPaint); 
 
 SIZE stZeichengrosse; POINT ptText; 
 HFONT hFont = CreateFontIndirect(&lfSchrift);
 SelectObject(stPaint.hdc, hFont);
 GetTextExtentPoint32(stPaint.hdc, vasInfotext->c_Str(), vasInfotext->Length(), &stZeichengrosse);
 ptText.x = lBreite / 2 - stZeichengrosse.cx / 2;
 ptText.y = (lHohe - ucRahmenbreite - 25)/ 2 - stZeichengrosse.cy / 2;
 SetBkMode(stPaint.hdc, TRANSPARENT);
 SetTextColor(stPaint.hdc, RGB(stSchriftfarbe.ucRot, stSchriftfarbe.ucGrun, stSchriftfarbe.ucBlau));
 ExtTextOut(stPaint.hdc, ptText.x, ptText.y, ETO_CLIPPED, &stPaint.rcPaint, vasInfotext->c_Str(), vasInfotext->Length(), nullptr);
 DeleteObject(hFont);

 EndPaint(hWndElement, &stPaint);
 ThreadSicher_Ende(); 
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COAudio::WM_Command(WPARAM wParam)
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
				                              if(IsWindowEnabled(hWndElement) && ucRechte & ZR_LOSCHEN){ *vasInfotext = "Kein Audio"; DateiEntfernen();}
																			ThreadSicher_Ende(); return false;
			default                       : return true;
	 }
 }
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COAudio::DateiOffnen(void)
{
 OPENFILENAME stOffnenDatei; char c256Dateiname[256]; c256Dateiname[0] = 0; static char c256Filter[256];

 stOffnenDatei.lStructSize = sizeof(OPENFILENAME);
 stOffnenDatei.hwndOwner = hWndElement;
 stOffnenDatei.lpstrDefExt = "mp3"; stOffnenDatei.nFilterIndex = 0;
 stOffnenDatei.lpstrFilter = "Audio MP3\0*.mp3\0Audio WMA\0*.wma\0Alle Dateien\0*.*\0\0";
 stOffnenDatei.lpstrCustomFilter = c256Filter; stOffnenDatei.nMaxCustFilter = 256;
 stOffnenDatei.lpstrFile = c256Dateiname; stOffnenDatei.nMaxFile = 256;
 stOffnenDatei.lpstrFileTitle = nullptr; stOffnenDatei.lpstrInitialDir = nullptr; stOffnenDatei.lpstrTitle = nullptr;
 stOffnenDatei.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING | OFN_EXTENSIONDIFFERENT | OFN_FORCESHOWHIDDEN | OFN_HIDEREADONLY;

 if(GetOpenFileName(&stOffnenDatei)){
	 *vasDateiname = c256Dateiname; *vasTypEndung = c256Dateiname; vasTypEndung->ShortLeft(stOffnenDatei.nFileExtension);
	 MCIOffnen();
	 bGeandert = true;
	 *vasInfotext = *vasDateiname; vasInfotext->ShortLeft(stOffnenDatei.nFileOffset);
	 UpdateFenster(nullptr, true, false);
 }
 else if(CommDlgExtendedError()) MessageBox(hWndElement, pcDateifehlerOffnenText, pcDateifehlerOffnen, MB_ICONHAND);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COAudio::DateiSpeichernUnter(void)
{
 if(MCIWndGetMode(hWndMCI, nullptr, 0) == MCI_MODE_NOT_READY) return;

 OPENFILENAME stSpeichernDatei; char c256Dateiname[256]; c256Dateiname[0] = 0; static char c256Filter[256];

 stSpeichernDatei.lStructSize = sizeof(OPENFILENAME);
 stSpeichernDatei.hwndOwner = hWndElement;
 stSpeichernDatei.lpstrDefExt = "mp3"; stSpeichernDatei.nFilterIndex = 0;
 stSpeichernDatei.lpstrFilter = "Audio MP3\0*.mp3\0Audio WMA\0*.wma\0Alle Dateien\0*.*\0\0";
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
//-------------------------------------------------------------------------------------------------------------------------------------------