/****************************************************************************
	OMedia.cpp
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
#include "OMedia.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
char pcDateifehlerSpeichern[] = "Fehler Datei Speichern";
char pcDateifehlerSpeichernText[] = "Die Datei konnte nicht gespeichert werden";
char pcDateifehlerOffnen[] = "Fehler Datei Offnen";
char pcDateifehlerOffnenText[] = "Die Datei konnte nicht geöffnet werden";
//-------------------------------------------------------------------------------------------------------------------------------------------
void CALLBACK fnTimer_Position(void* pvParam, bool bTimerOrWaitFired)
{
 ((RePag::GUI::COMedia*)pvParam)->Timer_Position();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frBalken_WM_LButtonDown(COBalken* pBalken, WPARAM wParam, LPARAM lParam)
{
 COMedia* pMedia = (COMedia*)GetWindowLongPtr(GetParent(pBalken->HWND_Element()), GWLP_USERDATA); long lBreite;
 MCIWndStop(pMedia->hWndMCI);
 MCIWndSeek(pMedia->hWndMCI, pMedia->lLange / pMedia->bkPosition->Breite(lBreite) * LOWORD(lParam));
 pMedia->Timer_Position();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frBalken_WM_MouseMove(COBalken* pBalken, WPARAM wParam, LPARAM lParam)
{
 if(wParam == MK_LBUTTON){ long lBreite;
	 COMedia* pMedia = (COMedia*)GetWindowLongPtr(GetParent(pBalken->HWND_Element()), GWLP_USERDATA); 
	 MCIWndSeek(pMedia->hWndMCI, pMedia->lLange / pMedia->bkPosition->Breite(lBreite) * LOWORD(lParam));
	 pMedia->Timer_Position();
 }
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COMedia::COMediaV(VMEMORY vmSpeicher, const char* pcKlassenName, const char* pcFensterName, unsigned int uiIDElementA)
{
 COGrafikV(vmSpeicher, pcKlassenName, pcFensterName, uiIDElementA);

 hWndMCI = nullptr;
 bGeandert = false;
 ucRechte = 255;
 lLange = 0;

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

 COStringA asName = pcFensterName; asName += "_kStartStop";
 kStartStop = COKnopfV(vmSpeicher, asName.c_Str(), ++uiIDElementA);
 kStartStop->Beschriftung("Stop"); 

 asName = pcFensterName; asName += "_tzZeit";
 tzZeit = COTextZeileV(vmSpeicher, asName.c_Str(), ++uiIDElementA);
 tzZeit->Schriftfarbe(0, 255, 0);
 tzZeit->Hintergrundfarbe(0, 0, 0);
 tzZeit->Schriftausrichtung(TXA_MITTEHORIZONTAL | TXA_MITTEVERTICAL);
 tzZeit->Text("00:00:00");
 lfSchrift.lfHeight = -14;
 lfSchrift.lfWidth = 7;
 tzZeit->Schriftart(lfSchrift);
 lfSchrift.lfHeight = -13;
 lfSchrift.lfWidth = 0;

 asName = pcFensterName; asName += "_bPosition";
 bkPosition = COBalkenV(vmSpeicher, asName.c_Str(), ++uiIDElementA);
 bkPosition->Hintergrundfarbe(100, 100, 100);
 bkPosition->ErweitertStil(WS_EX_STATICEDGE | WS_EX_NOPARENTNOTIFY);
 bkPosition->pfnWM_LButtonDown = frBalken_WM_LButtonDown;
 bkPosition->pfnWM_MouseMove = frBalken_WM_MouseMove;

 vasDateiname = COStringAV(vmSpeicher);
 vasTypEndung = COStringAV(vmSpeicher);

 pfnWM_Command = nullptr;
 pfnWM_Char_Return = nullptr;
 pfnWM_Char_Escape = nullptr;

 htPosition = nullptr;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::GUI::COMedia::COFreiV(void)
{
 DestroyMenu(hMenu); 
 VMFreiV(vasDateiname); VMFreiV(vasTypEndung); VMFreiV(vasInfotext); 
 MCIWndDestroy(hWndMCI); 

 return ((COElement*)this)->COFreiV();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COMedia::WM_Create(HWND hWnd)
{
 RECT rcClient; GetClientRect(hWnd, &rcClient);
 ucRahmenbreite = lHohe - rcClient.bottom;

 kStartStop->ErstellFenster(hWnd, 25, 60, 0, rcClient.bottom - 25);
 tzZeit->ErstellFenster(hWnd, 25, 75, lBreite - ucRahmenbreite - 75, rcClient.bottom - 25);
 bkPosition->ErstellFenster(hWnd, 25, lBreite - ucRahmenbreite - 137, 61, rcClient.bottom - 25);

 hWndMCI = MCIWndCreate(hWnd, Hinstance(), WS_CHILDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | MCIWNDF_NOPLAYBAR | MCIWNDF_NOMENU | MCIWNDF_NOTIFYALL | MCIWNDF_NOERRORDLG, nullptr);
 MCIWndUseTime(hWndMCI);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COMedia::WM_Size(LPARAM lParam)
{
 ThreadSicher_Anfang();
 WM_Size_Element(hWndElement, lParam); 
 if(lHohe - ucRahmenbreite != HIWORD(lParam)){ long lPos;
	 AndernFensterGrosse(ucRahmenbreite + HIWORD(lParam) - lHohe, 0);
   kStartStop->NeueFensterPosition(0, HIWORD(lParam) - 25);
	 tzZeit->NeueFensterPosition(tzZeit->Pos_X(lPos), HIWORD(lParam) - 25);
 	 bkPosition->NeueFensterPosition(bkPosition->Pos_X(lPos), HIWORD(lParam) - 25);
 }
 if(lBreite - ucRahmenbreite != LOWORD(lParam)){ long lPos;
	 AndernFensterGrosse(0, ucRahmenbreite + LOWORD(lParam) - lBreite);
	 tzZeit->NeueFensterPosition(LOWORD(lParam) - 75, tzZeit->Pos_Y(lPos));
 	 bkPosition->NeueFensterBreite(LOWORD(lParam) - 137);
 }
 ThreadSicher_Ende(); 
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COMedia::WM_Char(WPARAM wParam)
{
 switch(wParam){
	  case VK_RETURN   : ThreadSicher_Anfang();
			                 if(pfnWM_Char_Return) pfnWM_Char_Return(this);
			                 ThreadSicher_Ende(); 
		                   break;
	  case VK_ESCAPE   : ThreadSicher_Anfang(); 
			                 if(pfnWM_Char_Escape) pfnWM_Char_Escape(this);
			                 ThreadSicher_Ende(); 
								       break;
		case VK_DELETE   : ThreadSicher_Anfang(); 
			                 if(GetKeyState(VK_SHIFT) & 0x8000 && GetKeyState(VK_CONTROL) & 0x8000) DateiEntfernen();
			                 ThreadSicher_Ende(); 
											 break;
 }

}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COMedia::WM_ContexMenu(LPARAM lParam)
{
 ThreadSicher_Anfang();
 SetFocus(hWndElement);

 long lMediaMode = MCIWndGetMode(hWndMCI, nullptr, 0);
 if(ucRechte & ZR_OFFNEN) EnableMenuItem(hMenu, IDM_DATEI_OFFNEN, MF_BYCOMMAND | MF_ENABLED);
 else EnableMenuItem(hMenu, IDM_DATEI_OFFNEN, MF_BYCOMMAND | MF_GRAYED);
 if(ucRechte & ZR_SPEICHERN && lMediaMode != MCI_MODE_NOT_READY) EnableMenuItem(hMenu, IDM_DATEI_SPEICHERNUNTER, MF_BYCOMMAND | MF_ENABLED);
 else EnableMenuItem(hMenu, IDM_DATEI_SPEICHERNUNTER, MF_BYCOMMAND | MF_GRAYED);
 if(ucRechte & ZR_SPEICHERN && lMediaMode != MCI_MODE_NOT_READY) EnableMenuItem(hMenu, IDM_DATEI_SPEICHERN, MF_BYCOMMAND | MF_ENABLED);
 else EnableMenuItem(hMenu, IDM_DATEI_SPEICHERN, MF_BYCOMMAND | MF_GRAYED);
 if(ucRechte & ZR_LOSCHEN && lMediaMode != MCI_MODE_NOT_READY) EnableMenuItem(hMenu, IDM_DATEI_ENTFERNEN, MF_BYCOMMAND | MF_ENABLED);
 else EnableMenuItem(hMenu, IDM_DATEI_ENTFERNEN, MF_BYCOMMAND | MF_GRAYED);

 POINT ptPosition;
 ptPosition.x = LOWORD(lParam); ptPosition.y = HIWORD(lParam);
 if(ptPosition.x == USHRT_MAX && ptPosition.y == USHRT_MAX) ClientToScreen(GetParent(hWndElement), &Position(ptPosition));
 TrackPopupMenuEx(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, ptPosition.x, ptPosition.y, hWndElement, nullptr);
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COMedia::MCIWNDM_NotifyMode(LPARAM lParam)
{
 ThreadSicher_Anfang();
 switch(lParam){
		 case MCI_MODE_STOP : kStartStop->Beschriftung("Start"); ChangeTimerQueueTimer(TimerQueue(), htPosition, 0, 0); break;
		 case MCI_MODE_PLAY : kStartStop->Beschriftung("Stop"); CreateTimerQueueTimer(&htPosition, TimerQueue(), (WAITORTIMERCALLBACK)fnTimer_Position, this, 0, 500, WT_EXECUTEINPERSISTENTTHREAD); break;
 }
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COMedia::Infotext(char* pcText)
{ 
 ThreadSicher_Anfang();
 *vasInfotext = pcText; 
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COMedia::Schriftart(LOGFONT& lfSchriftA)
{
 ThreadSicher_Anfang();
 lfSchrift = lfSchriftA;
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COMedia::Schriftfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
 ThreadSicher_Anfang();
 stSchriftfarbe.ucRot = ucRot; stSchriftfarbe.ucGrun = ucGrun; stSchriftfarbe.ucBlau = ucBlau;
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COMedia::SetzVerfugbar(bool bVerfugbar)
{
 ThreadSicher_Anfang();
 if(bVerfugbar){ EnableWindow(hWndElement, bVerfugbar); if(!IsWindowVisible(hWndElement)) ShowWindow(hWndElement, SW_SHOW);}
 else{ if(hWndElement == GetFocus()) SetFocus(GetParent(hWndElement)); MCIWndClose(hWndMCI); EnableWindow(hWndElement, bVerfugbar);}
 bGeandert = false;
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COMedia::DateiEntfernen(void)
{
 if(MCIWndGetMode(hWndMCI, NULL, 0) == MCI_MODE_NOT_READY) return;

 kStartStop->Beschriftung("Stop");
 bGeandert = true; *vasDateiname = NULL; *vasTypEndung = NULL;
 MCIWndClose(hWndMCI);
 UpdateFenster(nullptr, true, false);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COMedia::MCIOffnen(void)
{
 if(MCIWndGetMode(hWndMCI, NULL, 0) != MCI_MODE_NOT_READY) MCIWndClose(hWndMCI);

 if(!MCIWndOpen(hWndMCI, vasDateiname->c_Str(), nullptr)){
   if(!IsWindowVisible(hWndElement)) ShowWindow(hWndElement, SW_SHOW); 
	 MCIWndHome(hWndMCI);
	 lLange = MCIWndGetLength(hWndMCI); bkPosition->SetzPosition_Max(lLange);
	 kStartStop->Beschriftung("Start");
 }
 else{ char c256Fehler[256];
	 MCIWndGetError(hWndMCI, c256Fehler, 256);
	 MessageBox(hWndElement, c256Fehler, pcDateifehlerOffnen, MB_ICONHAND);
	 *vasDateiname = NULL; *vasTypEndung = NULL;
 }
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COMedia::Lese(VMBLOCK& vbMedia, unsigned long& ulBytes, COStringA* pasDateiendung)
{
 ThreadSicher_Anfang();
 if(vasDateiname->Length()){
   HANDLE hDatei = CreateFile(vasDateiname->c_Str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	 if(hDatei == INVALID_HANDLE_VALUE) MessageBox(hWndElement, pcDateifehlerOffnenText, pcDateifehlerOffnen, MB_ICONHAND);
	 else{ Cursorform(IDC_WAIT);
		 ulBytes = GetFileSize(hDatei, nullptr);
		 if(ulBytes != 0xFFFFFFFF){ ULONG ulGeleseneBytes;
			 try{ 
				 vbMedia = VMBlock(ulBytes);
				 ReadFile(hDatei, vbMedia, ulBytes, &ulGeleseneBytes, nullptr);
		     if(pasDateiendung) *pasDateiendung = *vasTypEndung;	
         Cursorform(nullptr);
			 }
			 catch(EXCEPTION exFehler){ Cursorform(nullptr);
				 if(exFehler == EX_VM_KEINSPEICHER) MessageBox(hWndElement, "Es steht nicht genügend Arbeitspeicher für diese Datei zur Verfügung.", "Dat is zu eng", MB_ICONHAND);
			   vbMedia = nullptr; ulBytes = 0; if(pasDateiendung) *pasDateiendung = NULL;
			 }
		 }
		 else{ vbMedia = nullptr; ulBytes = 0; if(pasDateiendung) *pasDateiendung = NULL;}
	 }
	 CloseHandle(hDatei);
 }
 else{ vbMedia = nullptr; ulBytes = 0; if(pasDateiendung) *pasDateiendung = NULL;}
 ThreadSicher_Ende(); 
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COMedia::Schreibe(VMBLOCK vbMedia, unsigned long ulBytes, VMBLOCK vbDateiendung, unsigned char ucDateiendung_Lange)
{
 ThreadSicher_Anfang();
 if(vbMedia && ulBytes){ HANDLE hDatei; DWORD dwBytes = 0;
   vasTypEndung->SetLength(ucDateiendung_Lange);
	 MemCopy(vasTypEndung->c_Str(), vbDateiendung, ucDateiendung_Lange);

	 dwBytes = GetTempPath(NULL, nullptr);
	 vasDateiname->SetLength(dwBytes);
	 GetTempPath(dwBytes, vasDateiname->c_Str());
	 vasDateiname->ShortRight(1); *vasDateiname += "\\Temp."; *vasDateiname += *vasTypEndung;
	 hDatei = CreateFile(vasDateiname->c_Str(), GENERIC_WRITE, NULL, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	 if(hDatei == INVALID_HANDLE_VALUE){ CloseHandle(hDatei); ThreadSicher_Ende(); return;}
	 Cursorform(IDC_WAIT);
	 if(WriteFile(hDatei, vbMedia, ulBytes, &dwBytes, nullptr)){ CloseHandle(hDatei); MCIOffnen();}
	 else CloseHandle(hDatei);
	 Cursorform(nullptr);
 }
 else DateiEntfernen();
 ThreadSicher_Ende(); 
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COMedia::DateiLesen(char* pcDateinamen)
{
 ThreadSicher_Anfang();
 if(pcDateinamen){ 
	 HANDLE hDatei = CreateFile(pcDateinamen, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	 if(hDatei == INVALID_HANDLE_VALUE) MessageBox(hWndElement, pcDateifehlerOffnenText, pcDateifehlerOffnen, MB_ICONHAND);
	 else{ CloseHandle(hDatei);
 		 *vasDateiname = pcDateinamen; vasDateiname->SubString(vasTypEndung, vasDateiname->SearchCharacters(".") + 1, vasDateiname->Length());
		 Cursorform(IDC_WAIT);
     MCIOffnen();	
		 Cursorform(nullptr);
		 bGeandert = true; 
		 ThreadSicher_Ende(); return true;
	 }
	 CloseHandle(hDatei); 
 }
 ThreadSicher_Ende(); 
 return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COMedia::DateiSchreiben(char* pcDateinamen)
{
 ThreadSicher_Anfang();
 if(pcDateinamen){ VMBLOCK vbMedia; ULONG ulBytes;
   Lese(vbMedia, ulBytes, NULL);
	 if(!ulBytes){ ThreadSicher_Ende(); return false;}

	 HANDLE hDatei = CreateFile(pcDateinamen, GENERIC_WRITE, NULL, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	 if(hDatei == INVALID_HANDLE_VALUE) MessageBox(hWndElement, pcDateifehlerSpeichernText, pcDateifehlerSpeichern, MB_ICONHAND);
	 else{ ULONG ulGeleseneBytes; Cursorform(IDC_WAIT);
		 WriteFile(hDatei, vbMedia, ulBytes, &ulGeleseneBytes, nullptr);
		 VMFrei(vbMedia); CloseHandle(hDatei); Cursorform(nullptr);
		 ThreadSicher_Ende(); return true;
	 }
	 CloseHandle(hDatei);
 }
 ThreadSicher_Ende();
 return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COMedia::Geandert(void)
{
 ThreadSicher_Anfang();
 bool bGeandertA = bGeandert;
 ThreadSicher_Ende();
 return bGeandertA;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COMedia::Berechtigungen(unsigned char ucRechteA)
{
 ThreadSicher_Anfang();
 ucRechte = ucRechteA;
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COMedia::Timer_Position(void)
{
 ThreadSicher_Anfang();
 char c11Zahl[11]; long lZeit; BYTE ucStunden, ucMinuten, ucSekunden; COStringA asZeit;

 lZeit = MCIWndGetPosition(hWndMCI);
 bkPosition->SetzPosition(lZeit);
 if(lLange && lLange == lZeit) MCIWndStop(hWndMCI);
 ucSekunden = lZeit / 1000; ucMinuten = ucSekunden / 60; ucStunden = ucMinuten / 60;
 if(ucStunden) ucMinuten -= ucStunden * 60;
 if(ucMinuten) ucSekunden -= ucMinuten * 60;
 if(ucStunden < 10) asZeit = "0";
 asZeit += ULONGtoCHAR(c11Zahl, ucStunden); asZeit += ":";
 if(ucMinuten < 10) asZeit += "0";
 asZeit += ULONGtoCHAR(c11Zahl, ucMinuten); asZeit += ":";
 if(ucSekunden < 10) asZeit += "0";
 asZeit += ULONGtoCHAR(c11Zahl, ucSekunden);
 tzZeit->Text(asZeit.c_Str());
 ThreadSicher_Ende();
}