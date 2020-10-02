/****************************************************************************
	OBild.cpp
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

#include "HImage.h"
#include "OBild.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
char pcDateifehlerSpeichern[] = "Fehler Datei Speichern";
char pcDateifehlerSpeichernText[] = "Die Datei konnte nicht gespeichert werden";
char pcDateifehlerOffnen[] = "Fehler Datei Offnen";
char pcDateifehlerOffnenText[] = "Die Datei konnte nicht geöffnet werden";
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COBild* __vectorcall RePag::GUI::COBildV(const char* pcFensterName, unsigned int uiIDElement)
{
 COBild* vBild = (COBild*)VMBlock(VMDialog(), sizeof(COBild));
 vBild->COBildV(VMDialog(), pcFensterName, uiIDElement);
 return vBild;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COBild* __vectorcall RePag::GUI::COBildV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement)
{
 COBild* vBild = (COBild*)VMBlock(vmSpeicher, sizeof(COBild));
 vBild->COBildV(vmSpeicher, pcFensterName, uiIDElement);
 return vBild;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_Bild(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
 COBild* pBild;
 switch(uiMessage){
		case WM_CREATE       : ((COBild*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create(hWnd);
												 	 return NULL;
		case WM_SIZE         : pBild = (COBild*)GetWindowLongPtr(hWnd, GWL_USERDATA);
													 if(pBild) pBild->WM_Size(lParam);	
													 else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
													 return NULL;
		case WM_KILLFOCUS    : pBild = (COBild*)GetWindowLongPtr(hWnd, GWL_USERDATA);
													 if(pBild->pfnWM_KillFocus){ pBild->ThreadSicher_Anfang(); pBild->pfnWM_KillFocus(pBild); pBild->ThreadSicher_Ende(); }
													 return NULL;
		case WM_KEYDOWN      : pBild = (COBild*)GetWindowLongPtr(hWnd, GWL_USERDATA);
			                     if(IsWindowEnabled(hWnd)) pBild->WM_KeyDown(wParam);
													 return NULL;
		case WM_COMMAND      : pBild = (COBild*)GetWindowLongPtr(hWnd, GWL_USERDATA);
			                     if(!pBild->WM_Command(wParam)) return NULL;
			                     else if(pBild->pfnWM_Command){ pBild->ThreadSicher_Anfang();
														 if(!pBild->pfnWM_Command(pBild, wParam)){ pBild->ThreadSicher_Ende(); return NULL; }
														 pBild->ThreadSicher_Ende();
													 }
													 else PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
													 break;
		case WM_CONTEXTMENU  : ((COBild*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_ContexMenu(lParam);
			                     return NULL;
		case WM_LBUTTONDOWN  : SetFocus(hWnd);
			                     return NULL;
		case WM_LBUTTONUP    : PostMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWnd, GWLP_ID), wParam), lParam);
			                     break;
		case WM_PAINT        : ((COBild*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_Paint();
										       return NULL;
		case WM_NCDESTROY    : pBild = (COBild*)GetWindowLongPtr(hWnd, GWL_USERDATA);
													 if(pBild->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pBild->htEffekt_Timer, INVALID_HANDLE_VALUE);
			                     VMFreiV(pBild);
			                     return NULL;
 }
 return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBild::COBildV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA)
{
 COGrafikV(vmSpeicher, pcRePag_Bild, pcFensterName, uiIDElementA);

 imBild = nullptr;
 bBildformatErhalten = true;
 bGeandert = false;
 ucRechte = 255;

 vasInfotext = COStringAV(vmSpeicher, "Kein Bild");
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

 hMenu = CreatePopupMenu();
 AppendMenu(hMenu, MF_STRING, IDM_AUSSCHNEIDEN, "Ausschneiden		Strg+X");
 AppendMenu(hMenu, MF_STRING, IDM_KOPIEREN, "Kopieren		Strg+C");
 AppendMenu(hMenu, MF_STRING, IDM_EINFUGEN, "Einfügen		Strg+V");

 AppendMenu(hMenu, MF_SEPARATOR, NULL, NULL);
 AppendMenu(hMenu, MF_STRING, IDM_DATEI_OFFNEN, "Bild Öffnen		Strg+O");
 AppendMenu(hMenu, MF_STRING, IDM_DATEI_SPEICHERNUNTER, "Bild Speichern unter...");
 AppendMenu(hMenu, MF_STRING, IDM_DATEI_SPEICHERN, "Bild Speichern		Strg+S");
 AppendMenu(hMenu, MF_STRING, IDM_DATEI_ENTFERNEN, "Bild Entfernen		Strg+Umsch+Entf");

 AppendMenu(hMenu, MF_SEPARATOR, NULL, NULL);

 AppendMenu(hMenu, MF_STRING | MF_CHECKED, IDM_BILDFORMAT, "Bildformat erhalten");
 AppendMenu(hMenu, MF_STRING, IDM_INFO_CODEC, "Installierte (De)Encoder");

 pfnWM_Paint = nullptr;
 pfnWM_Char_Return = nullptr;
 pfnWM_Char_Escape = nullptr;
 pfnWM_KillFocus = nullptr;
 pfnWM_Command = nullptr;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::GUI::COBild::COFreiV(void)
{
 DestroyMenu(hMenu); VMFreiV(vasInfotext);
 if(imBild) delete imBild;
 return ((COElement*)this)->COFreiV();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBild::WM_Paint(void)
{
 ThreadSicher_Anfang();
 PAINTSTRUCT stPaint; 
 BeginPaint(hWndElement, &stPaint);
 WM_Paint_Hintergrund(stPaint);

 if(pfnWM_Paint) pfnWM_Paint(this, stPaint);
 else if(imBild){ 
   Graphics grBild(stPaint.hdc);
	 if(bBildformatErhalten){ UINT uiHohe, uiBreite;
		 if(imBild->GetHeight() > lHohe){
       uiBreite = imBild->GetWidth() * lHohe / imBild->GetHeight();
			 if(uiBreite > lBreite){
         uiHohe = imBild->GetHeight() * lBreite / imBild->GetWidth();
				 uiBreite = imBild->GetWidth() * lBreite / imBild->GetWidth();
			 }
			 else uiHohe = imBild->GetHeight() * lHohe / imBild->GetHeight();
		 }
		 else if(imBild->GetWidth() > lBreite){
       uiHohe = imBild->GetHeight() * lBreite / imBild->GetWidth();
			 if(uiHohe > lHohe){
         uiBreite = imBild->GetWidth() * lHohe / imBild->GetHeight();
				 uiHohe = imBild->GetHeight() * lHohe / imBild->GetHeight();
			 }
			 else uiBreite = imBild->GetWidth() * lBreite / imBild->GetWidth();
		 }
		 else if(imBild->GetHeight() != lHohe || imBild->GetWidth() != lBreite){
       uiBreite = imBild->GetWidth() * lHohe / imBild->GetHeight();
			 if(uiBreite > lBreite){
				 uiHohe = imBild->GetHeight() * lBreite / imBild->GetWidth();
				 uiBreite = imBild->GetWidth() * lBreite / imBild->GetWidth();
			 }
			 else uiHohe = imBild->GetHeight() * lHohe / imBild->GetHeight();
		 }
		 else{ uiBreite = lBreite; uiHohe = lHohe;}
     grBild.DrawImage(imBild, 0, 0, uiBreite, uiHohe);
	 }
	 else grBild.DrawImage(imBild, 0, 0, lBreite, lHohe);
 }
 else{ SIZE stZeichengrosse; POINT ptText;
	 HFONT hFont = CreateFontIndirect(&lfSchrift);
	 SelectObject(stPaint.hdc, hFont);
   GetTextExtentPoint32(stPaint.hdc, vasInfotext->c_Str(), vasInfotext->Length(), &stZeichengrosse);
	 ptText.x = lBreite / 2 - stZeichengrosse.cx / 2;
	 ptText.y = lHohe / 2 - stZeichengrosse.cy / 2;
	 SetBkMode(stPaint.hdc, TRANSPARENT);
	 SetTextColor(stPaint.hdc, RGB(stSchriftfarbe.ucRot, stSchriftfarbe.ucGrun, stSchriftfarbe.ucBlau));
	 ExtTextOut(stPaint.hdc, ptText.x, ptText.y, ETO_CLIPPED, &stPaint.rcPaint, vasInfotext->c_Str(), vasInfotext->Length(), nullptr);
	 DeleteObject(hFont);
 }
 EndPaint(hWndElement, &stPaint);
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBild::WM_Create(HWND hWnd)
{
 RECT rcClient; GetClientRect(hWnd, &rcClient);
 ucRahmenbreite = lHohe - rcClient.bottom;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBild::WM_Size(LPARAM lParam)
{
 ThreadSicher_Anfang();
 WM_Size_Element(hWndElement, lParam);
 if(lHohe - ucRahmenbreite != HIWORD(lParam)) AndernFensterGrosse(ucRahmenbreite + HIWORD(lParam) - lHohe, 0);
 if(lBreite - ucRahmenbreite != LOWORD(lParam)) AndernFensterGrosse(0, ucRahmenbreite + LOWORD(lParam) - lBreite);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBild::WM_KeyDown(WPARAM wParam)
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
bool __vectorcall RePag::GUI::COBild::WM_Command(WPARAM wParam)
{
 switch(LOWORD(wParam)){
		case IDM_DATEI_OFFNEN         : ThreadSicher_Anfang();
			                              if(IsWindowEnabled(hWndElement) && ucRechte & ZR_OFFNEN) DateiOffnen();
			                              ThreadSicher_Ende(); return false;
		case IDM_DATEI_SPEICHERN      : ThreadSicher_Anfang();
			                              if(IsWindowEnabled(hWndElement) && ucRechte & ZR_SPEICHERN) DateiSpeichern(nullptr);
			                              ThreadSicher_Ende(); return false;
		case IDM_DATEI_SPEICHERNUNTER : ThreadSicher_Anfang();
			                              if(IsWindowEnabled(hWndElement) && ucRechte & ZR_SPEICHERN) DateiSpeichernUnter();
			                              ThreadSicher_Ende(); return false;
		case IDM_DATEI_ENTFERNEN      : ThreadSicher_Anfang();
			                              if(IsWindowEnabled(hWndElement) && ucRechte & ZR_LOSCHEN) DateiEntfernen();
			                              ThreadSicher_Ende(); return false;
    case IDM_INFO_CODEC           : CODEC(hWndElement);
			                              return false;
		case IDM_BILDFORMAT           : ThreadSicher_Anfang();
			                              if(bBildformatErhalten){ bBildformatErhalten = false; CheckMenuItem(hMenu, IDM_BILDFORMAT, MF_UNCHECKED); }
																	  else{ bBildformatErhalten = true; CheckMenuItem(hMenu, IDM_BILDFORMAT, MF_CHECKED); }
																		UpdateFenster(nullptr, true, false);
			                              ThreadSicher_Ende(); return false;
		default                       : return true;
 }
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBild::WM_ContexMenu(LPARAM lParam)
{
 ThreadSicher_Anfang();
 SetFocus(hWndElement);

 EnableMenuItem(hMenu, IDM_AUSSCHNEIDEN, MF_BYCOMMAND | MF_GRAYED);
 //if(imBild) EnableMenuItem(hMenu, IDM_KOPIEREN, MF_BYCOMMAND | MF_ENABLED);
 //else EnableMenuItem(hMenu, IDM_KOPIEREN, MF_BYCOMMAND | MF_GRAYED);
 EnableMenuItem(hMenu, IDM_KOPIEREN, MF_BYCOMMAND | MF_GRAYED);
 EnableMenuItem(hMenu, IDM_EINFUGEN, MF_BYCOMMAND | MF_GRAYED);

 if(ucRechte & ZR_OFFNEN) EnableMenuItem(hMenu, IDM_DATEI_OFFNEN, MF_BYCOMMAND | MF_ENABLED);
 else EnableMenuItem(hMenu, IDM_DATEI_OFFNEN, MF_BYCOMMAND | MF_GRAYED);
 if(ucRechte & ZR_SPEICHERN && imBild) EnableMenuItem(hMenu, IDM_DATEI_SPEICHERNUNTER, MF_BYCOMMAND | MF_ENABLED);
 else EnableMenuItem(hMenu, IDM_DATEI_SPEICHERNUNTER, MF_BYCOMMAND | MF_GRAYED);
 if(ucRechte & ZR_SPEICHERN && imBild) EnableMenuItem(hMenu, IDM_DATEI_SPEICHERN, MF_BYCOMMAND | MF_ENABLED);
 else EnableMenuItem(hMenu, IDM_DATEI_SPEICHERN, MF_BYCOMMAND | MF_GRAYED);
 if(ucRechte & ZR_LOSCHEN && imBild) EnableMenuItem(hMenu, IDM_DATEI_ENTFERNEN, MF_BYCOMMAND | MF_ENABLED);
 else EnableMenuItem(hMenu, IDM_DATEI_ENTFERNEN, MF_BYCOMMAND | MF_GRAYED);

 POINT ptPosition;
 ptPosition.x = LOWORD(lParam); ptPosition.y = HIWORD(lParam);
 if(ptPosition.x == USHRT_MAX && ptPosition.y == USHRT_MAX) ClientToScreen(GetParent(hWndElement), &Position(ptPosition));
 TrackPopupMenuEx(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, ptPosition.x, ptPosition.y, hWndElement, nullptr);
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBild::Infotext(char* pcText)
{ 
 ThreadSicher_Anfang();
 *vasInfotext = pcText; 
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBild::Schriftart(LOGFONT& lfSchriftA)
{
 ThreadSicher_Anfang();
 lfSchrift = lfSchriftA;
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBild::Schriftfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
 ThreadSicher_Anfang();
 stSchriftfarbe.ucRot = ucRot; stSchriftfarbe.ucGrun = ucGrun; stSchriftfarbe.ucBlau = ucBlau;
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBild::SetzVerfugbar(bool bVerfugbar)
{
 ThreadSicher_Anfang();
 if(bVerfugbar){ EnableWindow(hWndElement, bVerfugbar); if(!IsWindowVisible(hWndElement)) ShowWindow(hWndElement, SW_SHOW); }
 else{ if(hWndElement == GetFocus()) SetFocus(GetParent(hWndElement)); EnableWindow(hWndElement, bVerfugbar); }
 bGeandert = false;
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBild::DateiOffnen(void)
{
 OPENFILENAMEW stOffnenDatei; wchar_t wc256Dateiname[256]; wc256Dateiname[0] = 0; static wchar_t wc256Filter[256];

 stOffnenDatei.lStructSize = sizeof(OPENFILENAMEW);
 stOffnenDatei.hwndOwner = hWndElement;
 stOffnenDatei.lpstrDefExt = L"jpg"; stOffnenDatei.nFilterIndex = 0;
 stOffnenDatei.lpstrFilter = L"Bild JPEG\0*.jpg;*.jpeg;*.jpe;*.jfif\0Bild BMP\0*.bmr;*.dib;*.rle\0Bild GIF\0*.gif\0Bild TIF\0*.tif;*.tiff\0Bild PNG\0*.png\0Alle Dateien\0*.*\0\0";
 stOffnenDatei.lpstrCustomFilter = wc256Filter; stOffnenDatei.nMaxCustFilter = 256;
 stOffnenDatei.lpstrFile = wc256Dateiname; stOffnenDatei.nMaxFile = 256;
 stOffnenDatei.lpstrFileTitle = nullptr; stOffnenDatei.lpstrInitialDir = NULL; stOffnenDatei.lpstrTitle = nullptr;
 stOffnenDatei.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING | OFN_EXTENSIONDIFFERENT | OFN_FORCESHOWHIDDEN | OFN_HIDEREADONLY;

 if(GetOpenFileNameW(&stOffnenDatei)){
   if(imBild) delete imBild;
   imBild = new Image(wc256Dateiname, false);
	 UpdateFenster(nullptr, true, false);
	 bGeandert = true;
 }
 else if(CommDlgExtendedError()) MessageBox(hWndElement, pcDateifehlerOffnenText, pcDateifehlerOffnen, MB_ICONHAND);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBild::DateiSpeichernUnter(void)
{
 if(!imBild) return;

 OPENFILENAME stSpeichernDatei; char c256Dateiname[256]; c256Dateiname[0] = 0; static char c256Filter[256];

 stSpeichernDatei.lStructSize = sizeof(OPENFILENAME);
 stSpeichernDatei.hwndOwner = hWndElement;
 stSpeichernDatei.lpstrDefExt = "jpg"; stSpeichernDatei.nFilterIndex = 0;
 stSpeichernDatei.lpstrFilter = "Bild JPEG\0*.jpg;*.jpeg;*.jpe;*.jfif\0Bild BMP\0*.bmp;*.dib;*.rle\0Bild GIF\0*.gif\0Bild TIF\0*.tif;*.tiff\0"
	 "Bild PNG\0*.png\0Bild EMF\0*.emf\0Bild WMF\0*.wmf\0Bild ICO\0*.ico\0Alle Dateien\0*.*\0\0";
 stSpeichernDatei.lpstrCustomFilter = c256Filter; stSpeichernDatei.nMaxCustFilter = 256;
 stSpeichernDatei.lpstrFile = c256Dateiname; stSpeichernDatei.nMaxFile = 256;
 stSpeichernDatei.lpstrFileTitle = nullptr; stSpeichernDatei.lpstrInitialDir = nullptr; stSpeichernDatei.lpstrTitle = nullptr;
 stSpeichernDatei.Flags = OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING | OFN_EXTENSIONDIFFERENT | OFN_FORCESHOWHIDDEN | OFN_HIDEREADONLY;

 if(GetSaveFileName(&stSpeichernDatei)) DateiSpeichern(c256Dateiname);
 else if(CommDlgExtendedError()) MessageBox(hWndElement, pcDateifehlerSpeichernText, pcDateifehlerSpeichern, MB_ICONHAND);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBild::DateiSpeichern(char* pcDateiname)
{
 if(!imBild) return;

 if(!pcDateiname){ DateiSpeichernUnter(); return; }

 COStringA* vasEndung = COStringAV(&pcDateiname[CharactersPosition(pcDateiname, 0x2e, true) + 1]); vasEndung->Uppercase(); VMBLOCK vbBild; ULONG ulBytes;
 if(*vasEndung == "JPG" || *vasEndung == "JPEG" || *vasEndung == "JPE" || *vasEndung == "JFIF"){
	 if(!BildKonvertierenZu(ImageFormatJPEG, vbBild, ulBytes)){
		 MessageBox(hWndElement, pcDateifehlerSpeichernText, pcDateifehlerSpeichern, MB_ICONHAND); VMFreiV(vasEndung); return; }
 }
 else if(*vasEndung == "BMP" || *vasEndung == "DIB" || *vasEndung == "RLE"){
	 if(!BildKonvertierenZu(ImageFormatBMP, vbBild, ulBytes)){
		 MessageBox(hWndElement, pcDateifehlerSpeichernText, pcDateifehlerSpeichern, MB_ICONHAND); VMFreiV(vasEndung); return; }
 }
 else if(*vasEndung == "TIF" || *vasEndung == "TIFF"){
	 if(!BildKonvertierenZu(ImageFormatGIF, vbBild, ulBytes)){
		 MessageBox(hWndElement, pcDateifehlerSpeichernText, pcDateifehlerSpeichern, MB_ICONHAND); VMFreiV(vasEndung); return; }
 }
 else if(*vasEndung == "PNG"){
	 if(!BildKonvertierenZu(ImageFormatPNG, vbBild, ulBytes)){
		 MessageBox(hWndElement, pcDateifehlerSpeichernText, pcDateifehlerSpeichern, MB_ICONHAND); VMFreiV(vasEndung); return; }
 }
 else if(*vasEndung == "EMF"){
	 if(!BildKonvertierenZu(ImageFormatEMF, vbBild, ulBytes)){
		 MessageBox(hWndElement, pcDateifehlerSpeichernText, pcDateifehlerSpeichern, MB_ICONHAND); VMFreiV(vasEndung); return; }
 }
 else if(*vasEndung == "WMF"){
	 if(!BildKonvertierenZu(ImageFormatWMF, vbBild, ulBytes)){
		 MessageBox(hWndElement, pcDateifehlerSpeichernText, pcDateifehlerSpeichern, MB_ICONHAND); VMFreiV(vasEndung); return; }
 }
 else if(*vasEndung == "ICO"){
	 if(!BildKonvertierenZu(ImageFormatIcon, vbBild, ulBytes)){
		 MessageBox(hWndElement, pcDateifehlerSpeichernText, pcDateifehlerSpeichern, MB_ICONHAND); VMFreiV(vasEndung); return; }
 }
 else{
	 if(!BildKonvertierenZu(ImageFormatJPEG, vbBild, ulBytes)){
		 MessageBox(hWndElement, pcDateifehlerSpeichernText, pcDateifehlerSpeichern, MB_ICONHAND); VMFreiV(vasEndung); return; }
 }
 VMFreiV(vasEndung); 

 ULONG ulKontrollbytes;
 HANDLE hDatei = CreateFile(pcDateiname, GENERIC_WRITE, NULL, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
 if(hDatei == INVALID_HANDLE_VALUE) MessageBox(hWndElement, pcDateifehlerSpeichernText, pcDateifehlerSpeichern, MB_ICONHAND);
 else WriteFile(hDatei, vbBild, ulBytes, &ulKontrollbytes, nullptr);
 CloseHandle(hDatei); VMFrei(vbBild); bGeandert = false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBild::DateiEntfernen(void)
{
 if(imBild){ delete imBild; imBild = NULL; bGeandert = true; UpdateFenster(nullptr, true, false); }
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COBild::BildKonvertierenZu(GUID guidImage, VMBLOCK& vbBild, unsigned long& ulGrosse)
{
 UINT uiAnzahl, uiGrosse; IStorage* pStorage; IStream* pStream; STATSTG statstg; ULONG ulKontrollbytes; LARGE_INTEGER ullAnfang; ullAnfang.QuadPart = 0;

 vbBild = nullptr;
 GetImageEncodersSize(&uiAnzahl, &uiGrosse);
 if(!uiGrosse) return false;
 ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)VMBlock(uiGrosse);
 if(!pImageCodecInfo) return false;

 GetImageEncoders(uiAnzahl, uiGrosse, pImageCodecInfo);
 for(UINT uiEncoder = 0; uiEncoder < uiAnzahl; ++uiEncoder){
   if(pImageCodecInfo[uiEncoder].FormatID == guidImage){
		 StgCreateDocfile(NULL, STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 0, &pStorage);
		 pStorage->CreateStream(L"Bildstream", STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 0, 0, &pStream);
		 imBild->Save(pStream, &pImageCodecInfo[uiEncoder].Clsid);
		 pStream->Stat(&statstg, STATFLAG_NONAME);
		 ulGrosse = statstg.cbSize.LowPart;
     vbBild = VMBlock(statstg.cbSize.LowPart);
		 pStream->Seek(ullAnfang, STREAM_SEEK_SET, NULL);
     pStream->Read(vbBild, statstg.cbSize.LowPart, &ulKontrollbytes);
		 VMFrei(pImageCodecInfo);
		 return true;
   }    
 }
 VMFrei(pImageCodecInfo);
 if(vbBild) VMFrei(vbBild);
 return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COBild::Lese(VMBLOCK& vbBild, unsigned long& ulBytes, COStringA* pasDateiendung)
{
 GUID guidImage;
 ThreadSicher_Anfang();
 if(imBild){
	 imBild->GetRawFormat(&guidImage);
	 if(pasDateiendung){
		 if(guidImage == ImageFormatJPEG) *pasDateiendung = "JPG";
		 else if(guidImage == ImageFormatGIF) *pasDateiendung = "GIF"; 
		 else if(guidImage == ImageFormatBMP) *pasDateiendung = "BMP";
		 else if(guidImage == ImageFormatPNG) *pasDateiendung = "PNG"; 
		 else if(guidImage == ImageFormatWMF) *pasDateiendung = "WMF"; 
		 else if(guidImage == ImageFormatEMF) *pasDateiendung = "EMF";
		 else if(guidImage == ImageFormatIcon) *pasDateiendung = "ICO"; 
		 else *pasDateiendung = "BILD";
	 }
	 bool bReturn = BildKonvertierenZu(guidImage, vbBild, ulBytes);
   ThreadSicher_Ende();
   return bReturn;
 }
 vbBild = nullptr; ulBytes = 0;
 ThreadSicher_Ende(); return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBild::Schreibe(VMBLOCK vbBild, unsigned long ulBytes)
{
 ThreadSicher_Anfang();
 if(vbBild && ulBytes){
	 IStorage* pStorage; IStream* pStream; ULONG ulKontrollbytes; LARGE_INTEGER ullAnfang; ullAnfang.QuadPart = 0;
	 StgCreateDocfile(nullptr, STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 0, &pStorage);
	 pStorage->CreateStream(L"Bildstream", STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 0, 0, &pStream);
	 pStream->Write(vbBild, ulBytes, &ulKontrollbytes);
	 if(ulBytes == ulKontrollbytes){
		 pStream->Seek(ullAnfang, STREAM_SEEK_SET, nullptr);
		 if(imBild) delete imBild;
		 imBild = new Image(pStream, false);
	 }
	 else{
		 if(imBild){ delete imBild; imBild = nullptr;}
	 }
 }
 else if(imBild){ delete imBild; imBild = nullptr;}
 UpdateFenster(nullptr, true, false);
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COBild::DateiLesen(char* pcDateinamen)
{
 ThreadSicher_Anfang();
 if(pcDateinamen){
	 HANDLE hDatei = CreateFile(pcDateinamen, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	 if(hDatei == INVALID_HANDLE_VALUE) MessageBox(hWndElement, pcDateifehlerOffnenText, pcDateifehlerOffnen, MB_ICONHAND);
	 else{ ULONG ulBytes = GetFileSize(hDatei, nullptr);
     if(ulBytes != 0xFFFFFFFF){ ULONG ulGeleseneBytes;
		   VMBLOCK vbBild = VMBlock(ulBytes);
			 if(ReadFile(hDatei, vbBild, ulBytes, &ulGeleseneBytes, nullptr)){
				 Schreibe(vbBild, ulBytes); 
				 VMFrei(vbBild); CloseHandle(hDatei); bGeandert = true;
				 ThreadSicher_Ende(); return true;
			 }
		 }
     CloseHandle(hDatei); 
	 }
 }
 ThreadSicher_Ende();
 return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COBild::DateiSchreiben(char* pcDateinamen)
{
 ThreadSicher_Anfang();
 if(pcDateinamen){
	 HANDLE hDatei = CreateFile(pcDateinamen, GENERIC_WRITE, NULL, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	 if(hDatei == INVALID_HANDLE_VALUE) MessageBox(hWndElement, pcDateifehlerSpeichernText, pcDateifehlerSpeichern, MB_ICONHAND);
	 else{ VMBLOCK vbBild; ULONG ulBytes; 
		 if(Lese(vbBild, ulBytes, nullptr)){ ULONG ulGeleseneBytes;
			 if(WriteFile(hDatei, vbBild, ulBytes, &ulGeleseneBytes, nullptr)){
			   VMFrei(vbBild); CloseHandle(hDatei); 
				 ThreadSicher_Ende(); return true;
			 }
		 }
		 CloseHandle(hDatei); 
	 }
 }
 ThreadSicher_Ende();
 return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COBild::Geandert(void)
{
 ThreadSicher_Anfang();
 bool bGeandertA = bGeandert;
 ThreadSicher_Ende();
 return bGeandertA;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COBild::Berechtigungen(unsigned char ucRechteA)
{
 ThreadSicher_Anfang();
 ucRechte = ucRechteA;
 ThreadSicher_Ende();
}
//-------------------------------------------------------------------------------------------------------------------------------------------