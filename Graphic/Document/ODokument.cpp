/****************************************************************************
	ODocument.cpp
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


#include "HDocument.h"
#include "ODokument.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
#define SHIFTED 0x8000
char pcDateifehlerSpeichern[] = "Fehler Datei Speichern";
char pcDateifehlerSpeichernText[] = "Die Datei konnte nicht gespeichert werden";
char pcDateifehlerOffnen[] = "Fehler Datei Offnen";
char pcDateifehlerOffnenText[] = "Die Datei konnte nicht geöffnet werden";
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::CODokument* __vectorcall RePag::GUI::CODokumentV(const char* pcFensterName, unsigned int uiIDElement)
{
 CODokument* vDokument = (CODokument*)VMBlock(VMDialog(), sizeof(CODokument));
 vDokument->CODokumentV(VMDialog(), pcFensterName, uiIDElement);
 return vDokument;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::CODokument* __vectorcall RePag::GUI::CODokumentV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement)
{
 CODokument* vDokument = (CODokument*)VMBlock(vmSpeicher, sizeof(CODokument));
 vDokument->CODokumentV(vmSpeicher, pcFensterName, uiIDElement);
 return vDokument;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_Dokument(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
 CODokument* pDokument;
 switch(uiMessage){
	  case WM_SIZE         : pDokument = (CODokument*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			                     if(pDokument) pDokument->WM_Size_Element(hWnd, lParam);
													 else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
			                     return NULL;
    case WM_SETFOCUS     : ((CODokument*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_SetFocus();
													 return NULL;
		case WM_KILLFOCUS    : pDokument = (CODokument*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													 pDokument->ThreadSicher_Anfang();
			                     pDokument->WM_KillFocus();
													 if(pDokument->pfnWM_KillFocus) pDokument->pfnWM_KillFocus(pDokument);
													 pDokument->ThreadSicher_Ende();
													 return NULL;
    case WM_CHAR         : pDokument = (CODokument*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			                     if(IsWindowEnabled(hWnd)) pDokument->WM_Char(wParam);
													 return NULL;
		case WM_COMMAND      : pDokument = (CODokument*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			                     if(!pDokument->WM_Command(wParam)) return NULL;
			                     else if(pDokument->pfnWM_Command){ pDokument->ThreadSicher_Anfang();
														 if(!pDokument->pfnWM_Command(pDokument, wParam)){ pDokument->ThreadSicher_Ende(); return NULL; }
														 pDokument->ThreadSicher_Ende();
													 }
													 else PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
													 break;
		case WM_CONTEXTMENU  : ((CODokument*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_ContexMenu(lParam);
			                     return NULL;
		case WM_LBUTTONDOWN  : ((CODokument*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonDown();
                           return NULL;
		case WM_LBUTTONUP    : ((CODokument*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonUp(wParam, lParam);
                           return NULL;
		case WM_PAINT        : ((CODokument*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_Paint();
										       return NULL;
	  case WM_NCDESTROY    : pDokument = (CODokument*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
													 if(pDokument->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pDokument->htEffekt_Timer, INVALID_HANDLE_VALUE);
			                     VMFreiV(pDokument);
			                     return NULL;
										      
 }
 return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODokument::CODokumentV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA)
{
 COKnopfV(vmSpeicher, pcRePag_Dokument, pcFensterName, uiIDElementA);

 stHintergrundfarbe_3.ucRot = 150; stHintergrundfarbe_3.ucGrun = 150; stHintergrundfarbe_3.ucBlau = 150;
 stHintergrundfarbe = stHintergrundfarbe_3;
 *vasInhalt = "Dokument Laden";
 bAktiv = false;
 bGeandert = false;
 ucRechte = 255;

 hMenu = CreatePopupMenu();
 AppendMenu(hMenu, MF_STRING, IDM_DATEI_LADEN, "Dokument Laden	Strg+L");
 AppendMenu(hMenu, MF_STRING, IDM_DATEI_OFFNEN, "Dokument Öffnen	Strg+O");
 AppendMenu(hMenu, MF_STRING, IDM_DATEI_SPEICHERNUNTER, "Dokument Speichern unter...");
 AppendMenu(hMenu, MF_STRING, IDM_DATEI_SPEICHERN, "Dokument Speichern	Strg+S");
 AppendMenu(hMenu, MF_STRING, IDM_DATEI_ENTFERNEN, "Dokument Entfernen	Strg+Umsch+Entf");

 vasDateiname = COStringAV(vmSpeicher);
 vsmDatei = COStreamV(vmSpeicher, false);
 vasTypEndung = COStringAV(vmSpeicher);

 pfnWM_Char_ShiftReturn = nullptr;
 pfnWM_Char_Escape = nullptr;
 pfnWM_KillFocus = nullptr;
 pfnWM_Command = nullptr;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::GUI::CODokument::COFreiV(void)
{
 DestroyMenu(hMenu); 
 VMFreiV(vasDateiname); VMFreiV(vsmDatei); VMFreiV(vasTypEndung);
 return ((COTextZeile*)this)->COFreiV();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODokument::WM_Char(WPARAM wParam)
{
 switch(wParam){
	  case VK_RETURN : ThreadSicher_Anfang();
			               if(pfnWM_Char_ShiftReturn && GetKeyState(VK_SHIFT) & SHIFTED) pfnWM_Char_ShiftReturn(this);
										 else{ SendMessage(hWndElement, WM_LBUTTONDOWN, NULL, NULL); Sleep(100); SendMessage(hWndElement, WM_LBUTTONUP, NULL, NULL); }
										 ThreadSicher_Ende(); 
		                 break;
	  case VK_ESCAPE : ThreadSicher_Anfang();
			               if(pfnWM_Char_Escape) pfnWM_Char_Escape(this);
										 ThreadSicher_Ende(); 
								     break;
 }
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::CODokument::WM_Command(WPARAM wParam)
{
 switch(LOWORD(wParam)){
		case IDM_DATEI_LADEN          : ThreadSicher_Anfang();
			                              if(ucRechte & ZR_ANDERN){ DateiLaden();
																			if(bAktiv) stHintergrundfarbe = stHintergrundfarbe_1;
																			else stHintergrundfarbe = stHintergrundfarbe_3;
																			UpdateFenster(nullptr, true, true);
																		}
			                              ThreadSicher_Ende(); return false;
		case IDM_DATEI_OFFNEN         : ThreadSicher_Anfang();
			                              if(ucRechte & ZR_OFFNEN){ DateiOffnen();
																			stHintergrundfarbe = stHintergrundfarbe_1;
																			UpdateFenster(nullptr, true, true);
																		}
			                              ThreadSicher_Ende(); return false;
		case IDM_DATEI_SPEICHERN      : ThreadSicher_Anfang();
			                              if(ucRechte & ZR_SPEICHERN) DateiSpeichern();
			                              ThreadSicher_Ende(); return false;
		case IDM_DATEI_SPEICHERNUNTER : ThreadSicher_Anfang();
			                              if(ucRechte & ZR_SPEICHERN) DateiSpeichernUnter();
			                              ThreadSicher_Ende(); return false;
		case IDM_DATEI_ENTFERNEN      : ThreadSicher_Anfang();
			                              if(ucRechte & ZR_LOSCHEN) DateiEntfernen();
			                              ThreadSicher_Ende(); return false;
		default                       : return true;
 }
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODokument::WM_ContexMenu(LPARAM lParam)
{
 ThreadSicher_Anfang();
 if(bAktiv){
	 if(ucRechte & ZR_ANDERN) EnableMenuItem(hMenu, IDM_DATEI_LADEN, MF_BYCOMMAND | MF_ENABLED);
	 else EnableMenuItem(hMenu, IDM_DATEI_LADEN, MF_BYCOMMAND | MF_GRAYED);
	 if(ucRechte & ZR_OFFNEN) EnableMenuItem(hMenu, IDM_DATEI_OFFNEN, MF_BYCOMMAND | MF_ENABLED);
	 else EnableMenuItem(hMenu, IDM_DATEI_OFFNEN, MF_BYCOMMAND | MF_GRAYED);
	 if(ucRechte & ZR_SPEICHERN) EnableMenuItem(hMenu, IDM_DATEI_SPEICHERNUNTER, MF_BYCOMMAND | MF_ENABLED);
	 else EnableMenuItem(hMenu, IDM_DATEI_SPEICHERNUNTER, MF_BYCOMMAND | MF_GRAYED);
	 if(ucRechte & ZR_SPEICHERN) EnableMenuItem(hMenu, IDM_DATEI_SPEICHERN, MF_BYCOMMAND | MF_ENABLED);
	 else EnableMenuItem(hMenu, IDM_DATEI_SPEICHERN, MF_BYCOMMAND | MF_GRAYED);
	 if(ucRechte & ZR_LOSCHEN) EnableMenuItem(hMenu, IDM_DATEI_ENTFERNEN, MF_BYCOMMAND | MF_ENABLED);
	 else EnableMenuItem(hMenu, IDM_DATEI_ENTFERNEN, MF_BYCOMMAND | MF_GRAYED);
 }
 else{
	 if(ucRechte & ZR_ANDERN) EnableMenuItem(hMenu, IDM_DATEI_LADEN, MF_BYCOMMAND | MF_ENABLED);
	 else EnableMenuItem(hMenu, IDM_DATEI_LADEN, MF_BYCOMMAND | MF_GRAYED);
	 EnableMenuItem(hMenu, IDM_DATEI_OFFNEN, MF_BYCOMMAND | MF_GRAYED);
	 EnableMenuItem(hMenu, IDM_DATEI_SPEICHERNUNTER, MF_BYCOMMAND | MF_GRAYED);
	 EnableMenuItem(hMenu, IDM_DATEI_SPEICHERN, MF_BYCOMMAND | MF_GRAYED);
	 EnableMenuItem(hMenu, IDM_DATEI_ENTFERNEN, MF_BYCOMMAND | MF_GRAYED);
 }

 POINT ptPosition;
 ptPosition.x = LOWORD(lParam); ptPosition.y = HIWORD(lParam);
 if(ptPosition.x == USHRT_MAX && ptPosition.y == USHRT_MAX) ClientToScreen(GetParent(hWndElement), &Position(ptPosition));
 TrackPopupMenuEx(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, ptPosition.x, ptPosition.y, hWndElement, nullptr);
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODokument::WM_LButtonUp(WPARAM wParam, LPARAM lParam)
{
 ThreadSicher_Anfang();
 if(bAktiv){
	 if(ucRechte & ZR_OFFNEN) DateiOffnen();
   stHintergrundfarbe = stHintergrundfarbe_1;
 }
 else{
	 if(ucRechte & ZR_ANDERN) DateiLaden();
	 if(bAktiv) stHintergrundfarbe = stHintergrundfarbe_1;
	 else stHintergrundfarbe = stHintergrundfarbe_3;
 }
 UpdateFenster(nullptr, true, true);

 if(pfnWM_LButtonUp) pfnWM_LButtonUp(this);
 else PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), lParam);
 ThreadSicher_Ende();
 ReleaseCapture();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODokument::SetzVerfugbar(bool bVerfugbar)
{
 ThreadSicher_Anfang();
 if(bVerfugbar){ EnableWindow(hWndElement, bVerfugbar); if(!IsWindowVisible(hWndElement)) ShowWindow(hWndElement, SW_SHOW); }
 else{ if(hWndElement == GetFocus()) SetFocus(GetParent(hWndElement)); EnableWindow(hWndElement, bVerfugbar); }
 bGeandert = false;
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::CODokument::Geandert(void)
{
 ThreadSicher_Anfang();
 bool bGeandertA = bGeandert;
 ThreadSicher_Ende();
 return bGeandertA;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODokument::Berechtigungen(unsigned char ucRechteA)
{
 ThreadSicher_Anfang();
 ucRechte = ucRechteA;
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODokument::DateiLaden(void)
{
 OPENFILENAME stOffnenDatei; char c256Dateiname[256]; c256Dateiname[0] = 0; static char c256Filter[256];

 stOffnenDatei.lStructSize = sizeof(OPENFILENAME);
 stOffnenDatei.hwndOwner = hWndElement;
 stOffnenDatei.lpstrDefExt = "pdf"; stOffnenDatei.nFilterIndex = 0;
 stOffnenDatei.lpstrFilter = "PDF-Dokument PDF\0*.pdf\0Word-Dokument *.DOC;*.DOCX\0*.doc;*.docx\0Excel-Tabelle *.XLS;*XLSX\0*.xls;*.xlsx\0Alle Dateien\0*.*\0\0";
 stOffnenDatei.lpstrCustomFilter = c256Filter; stOffnenDatei.nMaxCustFilter = 256;
 stOffnenDatei.lpstrFile = c256Dateiname; stOffnenDatei.nMaxFile = 256;
 stOffnenDatei.lpstrFileTitle = nullptr; stOffnenDatei.lpstrInitialDir = nullptr; stOffnenDatei.lpstrTitle = nullptr;
 stOffnenDatei.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING | OFN_EXTENSIONDIFFERENT | OFN_FORCESHOWHIDDEN | OFN_HIDEREADONLY;

 if(GetOpenFileName(&stOffnenDatei)){
	 HANDLE hDatei = CreateFile(c256Dateiname, GENERIC_READ, NULL, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	 if(hDatei == INVALID_HANDLE_VALUE) MessageBox(hWndElement, "Die Datei konnte nicht geladen werden", "Fehler Datei Laden", MB_ICONHAND);
	 else{
		 if(vsmDatei->Bytes()) vsmDatei->Delete();
	   vsmDatei->ReadFile(hDatei, true); 
		 *vasDateiname = c256Dateiname;
		 vasDateiname->SubString(vasTypEndung, vasDateiname->SearchCharacters(".") + 1, vasDateiname->Length());
		 *vasInhalt = "Dokument Öffnen"; bAktiv = true; bGeandert = true;
   }
	 CloseHandle(hDatei);
 }
 else if(CommDlgExtendedError()) MessageBox(hWndElement, pcDateifehlerOffnenText, pcDateifehlerOffnen, MB_ICONHAND);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODokument::DateiOffnen(void)
{
 vasTypEndung->Uppercase();
 COStringA* vasProcess;
 if(*vasTypEndung == "PDF"){
	 if(vasAdobePfad) vasProcess = COStringAV(vasAdobePfad);
	 else{ MessageBox(hWndElement, "Die Datei konnte nicht geöffnet werden\nEs ist kein Acrobat Reader installiert", pcDateifehlerOffnen, MB_ICONHAND); return; }
 }
 else if(*vasTypEndung == "XLS" || *vasTypEndung == "XLSX"){
	 if(vasExcelPfad) vasProcess = COStringAV(vasExcelPfad);
	 else{ MessageBox(hWndElement, "Die Datei konnte nicht geöffnet werden\nEs ist kein Microsoft Excel installiert", pcDateifehlerOffnen, MB_ICONHAND); return; }
 }
 else if(*vasTypEndung == "DOCX" || *vasTypEndung == "DOCX"){
	 if(vasWordPfad) vasProcess = COStringAV(vasWordPfad);
	 else{ MessageBox(hWndElement, "Die Datei konnte nicht geöffnet werden\nEs ist kein Microsoft Word installiert", pcDateifehlerOffnen, MB_ICONHAND); return; }
 }
 else return;

 if(!vasDateiname->Length()){ HANDLE hDatei; DWORD dwBytes = 0;
   dwBytes = GetTempPath(NULL, nullptr);
	 vasDateiname->SetLength(dwBytes);
   GetTempPath(dwBytes, vasDateiname->c_Str());
	 vasDateiname->ShortRight(1); *vasDateiname += "Temp."; *vasDateiname += *vasTypEndung;
	 hDatei = CreateFile(vasDateiname->c_Str(), GENERIC_WRITE, NULL, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	 if(hDatei == INVALID_HANDLE_VALUE){ CloseHandle(hDatei); return; }
	 vsmDatei->WriteFile(hDatei, true); CloseHandle(hDatei);
 }
 *vasProcess += *vasDateiname;

 STARTUPINFO stStartupInfo = {0};
 stStartupInfo.cb = sizeof(STARTUPINFO);
 stStartupInfo.dwFlags = STARTF_USESHOWWINDOW;
 stStartupInfo.wShowWindow = SW_SHOWNORMAL | STARTF_UNTRUSTEDSOURCE;

 PROCESS_INFORMATION stProcessInfo = {0};
 CreateProcess(nullptr, vasProcess->c_Str(), nullptr, nullptr, false, NULL, nullptr, nullptr, &stStartupInfo, &stProcessInfo);
 vthlProcess->ThToEnd(stProcessInfo.hProcess);
 VMFreiV(vasProcess);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODokument::DateiSpeichernUnter(void)
{
 OPENFILENAME stSpeichernDatei; char c256Dateiname[256]; c256Dateiname[0] = 0; static char c256Filter[256];

 stSpeichernDatei.lStructSize = sizeof(OPENFILENAME);
 stSpeichernDatei.hwndOwner = hWndElement;
 stSpeichernDatei.lpstrDefExt = "pdf"; stSpeichernDatei.nFilterIndex = 0;
 stSpeichernDatei.lpstrFilter = "PDF-Dokument PDF\0*.pdf\0Word-Dokument *.DOC;*DOCX\0*.doc;*.docx\0Excel-Tabelle *.XLS;*.XLSX\0*.xls;*.xlsx\0Alle Dateien\0*.*\0\0";
 stSpeichernDatei.lpstrCustomFilter = c256Filter; stSpeichernDatei.nMaxCustFilter = 256;
 stSpeichernDatei.lpstrFile = c256Dateiname; stSpeichernDatei.nMaxFile = 256;
 stSpeichernDatei.lpstrFileTitle = nullptr; stSpeichernDatei.lpstrInitialDir = nullptr; stSpeichernDatei.lpstrTitle = nullptr;
 stSpeichernDatei.Flags = OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING | OFN_EXTENSIONDIFFERENT | OFN_FORCESHOWHIDDEN | OFN_HIDEREADONLY;

 if(GetSaveFileName(&stSpeichernDatei)){ *vasDateiname = c256Dateiname;
	 vasDateiname->SubString(vasTypEndung, vasDateiname->SearchCharacters(".") + 1, vasDateiname->Length());
	 DateiSpeichern();
 }
 else if(CommDlgExtendedError()) MessageBox(hWndElement, pcDateifehlerSpeichernText, pcDateifehlerSpeichern, MB_ICONHAND);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODokument::DateiSpeichern(void)
{
 if(!vasDateiname->Length()){ DateiSpeichernUnter(); return; }

 HANDLE hDatei = CreateFile(vasDateiname->c_Str(), GENERIC_WRITE, NULL, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
 if(hDatei == INVALID_HANDLE_VALUE) MessageBox(hWndElement, pcDateifehlerSpeichernText, pcDateifehlerSpeichern, MB_ICONHAND);
 else vsmDatei->WriteFile(hDatei, true);
 CloseHandle(hDatei);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODokument::DateiEntfernen(void)
{
 *vasDateiname = NULL; *vasTypEndung = NULL;
 vsmDatei->Delete();
 *vasInhalt = "Dokument Laden"; SetzAktiv(false); bGeandert = true;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODokument::Lese(VMBLOCK& vbDokument, unsigned long& ulBytes, COStringA* pasDateiendung)
{
 ThreadSicher_Anfang();
 if(vsmDatei->Bytes()){ 
	 vsmDatei->Data(vbDokument); ulBytes = vsmDatei->Bytes();
   if(pasDateiendung) *pasDateiendung = *vasTypEndung;
 }
 else{ vbDokument = nullptr; ulBytes = 0; if(pasDateiendung) *pasDateiendung = NULL; }
 ThreadSicher_Ende(); 
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODokument::Schreibe(VMBLOCK vbDokument, unsigned long ulBytes, VMBLOCK vbDateiendung, unsigned char ucDateiendung_Lange)
{
 ThreadSicher_Anfang();
 if(vbDokument && ulBytes){
   Schreibe(vbDokument, ulBytes);
   vasTypEndung->SetLength(ucDateiendung_Lange);
	 MemCopy(vasTypEndung->c_Str(), vbDateiendung, ucDateiendung_Lange);
 }
 else DateiEntfernen();
 ThreadSicher_Ende(); 
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::CODokument::Schreibe(VMBLOCK vbDokument, unsigned long& ulBytes)
{
 if(vsmDatei->Bytes()){ vsmDatei->Delete(); *vasDateiname = NULL; *vasTypEndung = NULL;}
 vsmDatei->Write(vbDokument, ulBytes);
 *vasInhalt = "Dokument Öffnen"; bAktiv = true; 
 stHintergrundfarbe = stHintergrundfarbe_1;
 UpdateFenster(nullptr, true, true);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::CODokument::DateiLesen(char* pcDateinamen)
{
 ThreadSicher_Anfang();
 if(pcDateinamen){
	 HANDLE hDatei = CreateFile(pcDateinamen, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	 if(hDatei == INVALID_HANDLE_VALUE) MessageBox(hWndElement, pcDateifehlerOffnenText, pcDateifehlerOffnen, MB_ICONHAND);
	 else{ 
		 if(vsmDatei->Bytes()) vsmDatei->Delete();
     vsmDatei->ReadFile(hDatei, true); CloseHandle(hDatei);
		 *vasDateiname = pcDateinamen;
     vasDateiname->SubString(vasTypEndung, vasDateiname->SearchCharacters(".") + 1, vasDateiname->Length());
		 bGeandert = true; ThreadSicher_Ende(); return true;    
	 }
	 CloseHandle(hDatei);
 }
 ThreadSicher_Ende();
 return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::CODokument::DateiSchreiben(char* pcDateinamen)
{
 ThreadSicher_Anfang();
 if(pcDateinamen){
	 HANDLE hDatei = CreateFile(pcDateinamen, GENERIC_WRITE, NULL, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	 if(hDatei == INVALID_HANDLE_VALUE) MessageBox(hWndElement, pcDateifehlerSpeichernText, pcDateifehlerSpeichern, MB_ICONHAND);
	 else{ vsmDatei->WriteFile(hDatei, true); CloseHandle(hDatei); ThreadSicher_Ende(); return true; }
	 CloseHandle(hDatei);
 }
 ThreadSicher_Ende();
 return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------