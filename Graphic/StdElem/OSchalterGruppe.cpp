/****************************************************************************
	OSchalterGruppe.cpp
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
#include "OSchalterGruppe.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COSchalterGruppe* __vectorcall RePag::GUI::COSchalterGruppeV(const char* pcFensterName, unsigned int uiIDElement, unsigned char ucSchalteranzahl, unsigned char  ucWechselmodus)
{
 COSchalterGruppe* vSchalterGruppe = (COSchalterGruppe*)VMBlock(VMDialog(), sizeof(COSchalterGruppe));
 vSchalterGruppe->COSchalterGruppeV(VMDialog(), pcFensterName, uiIDElement, ucSchalteranzahl, ucWechselmodus);
 return vSchalterGruppe;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
RePag::GUI::COSchalterGruppe* __vectorcall RePag::GUI::COSchalterGruppeV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement, unsigned char ucSchalteranzahl, unsigned char  ucWechselmodus)
{
 COSchalterGruppe* vSchalterGruppe = (COSchalterGruppe*)VMBlock(vmSpeicher, sizeof(COSchalterGruppe));
 vSchalterGruppe->COSchalterGruppeV(vmSpeicher, pcFensterName, uiIDElement, ucSchalteranzahl, ucWechselmodus);
 return vSchalterGruppe;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_SchalterGruppe(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
 COSchalterGruppe* pSchalterGruppe;
 switch(uiMessage){
	  case WM_SIZE        : pSchalterGruppe = (COSchalterGruppe*)GetWindowLongPtr(hWnd, GWL_USERDATA);
			                    if(pSchalterGruppe) pSchalterGruppe->WM_Size_Element(hWnd, lParam);
													else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
			                    return NULL;
		case WM_COMMAND     : PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
			                    return NULL;	
		case WM_LBUTTONDOWN : ((COSchalterGruppe*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_LButtonDown(lParam);
			                    return NULL;
		case WM_LBUTTONUP   : ReleaseCapture();
													PostMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWnd, GWLP_ID), wParam), lParam);
			                    return NULL;
		case WM_MOUSEMOVE   : ((COSchalterGruppe*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_MouseMove(wParam, lParam);
			                    return NULL;												
		case WM_PAINT       : ((COSchalterGruppe*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_Paint();
										      return NULL;
		case WM_NCDESTROY   : pSchalterGruppe = (COSchalterGruppe*)GetWindowLongPtr(hWnd, GWL_USERDATA);
													if(pSchalterGruppe->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pSchalterGruppe->htEffekt_Timer, INVALID_HANDLE_VALUE);
			                    VMFreiV(pSchalterGruppe);
			                    return NULL;
 }
 return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_WechselSchalter(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
 COSchalterGruppe::COWechselSchalter* pWechselSchalter;
 switch(uiMessage){
		case WM_CREATE        : ((COSchalterGruppe::COWechselSchalter*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create(hWnd);
														return NULL;
	  case WM_SIZE          : pWechselSchalter = (COSchalterGruppe::COWechselSchalter*)GetWindowLongPtr(hWnd, GWL_USERDATA);
			                      if(pWechselSchalter) pWechselSchalter->WM_Size_Element(hWnd, lParam);
			                      return NULL;
		case WM_COMMAND       : PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
													  break;
		case WM_LBUTTONDOWN   : pWechselSchalter = (COSchalterGruppe::COWechselSchalter*)GetWindowLongPtr(hWnd, GWL_USERDATA);
			                      SetCapture(hWnd);
														if(pWechselSchalter->pfnWM_LButtonDown){ pWechselSchalter->ThreadSicher_Anfang();
														  pWechselSchalter->pfnWM_LButtonDown(pWechselSchalter); pWechselSchalter->ThreadSicher_Ende(); }
                            return NULL;
		case WM_LBUTTONUP     : ((COSchalterGruppe::COWechselSchalter*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_LButtonUp(wParam, lParam);	
                            return NULL;
		case WM_PAINT         : ((COSchalterGruppe::COWechselSchalter*)GetWindowLongPtr(hWnd, GWL_USERDATA))->WM_Paint();
										        return NULL;
		case WM_NCDESTROY     : pWechselSchalter = (COSchalterGruppe::COWechselSchalter*)GetWindowLongPtr(hWnd, GWL_USERDATA);
														pWechselSchalter->pSchalterGruppe->vpWechselSchalter[pWechselSchalter->IDElement() - pWechselSchalter->pSchalterGruppe->IDElement() - 1] = NULL;
			                      VMFreiV(pWechselSchalter);
			                      return NULL;
 }
 return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::COSchalterGruppeV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA, unsigned char ucSchalteranzahl, unsigned char ucWechselmodus)
{
 COGrafikV(vmSpeicher, pcRePag_SchalterGruppe, pcFensterName, uiIDElementA);

 ErweitertStil(WS_EX_STATICEDGE | WS_EX_NOPARENTNOTIFY);
 stHintergrundfarbe.ucRot = stHintergrundfarbe.ucGrun = stHintergrundfarbe.ucBlau = 100;
 SetzHintergrund(false);

 ucModus = ucWechselmodus;
 pfnWM_Paint = NULL;
 pfnAndernSpaltenBreite = NULL;
 sMausPos_x = 0;
 ucIndex = 0xFF;

 vpWechselSchalter = (COWechselSchalter**)VMBlock(vmSpeicher, ucSchalteranzahl * 4);
 char pc11Zahl[11]; COStringA asName = pcFensterName; asName += "_WechselSchalter_";
 ucAnzahl = 0; uiIDElementA++;
 while(ucAnzahl < ucSchalteranzahl){
	 asName += ULONGtoCHAR(pc11Zahl, ucAnzahl);
   vpWechselSchalter[ucAnzahl] = (COWechselSchalter*)VMBlock(vmSpeicher, sizeof(COWechselSchalter));
	 vpWechselSchalter[ucAnzahl]->COWechselSchalterV(vmSpeicher, asName.c_Str(), uiIDElementA + ucAnzahl, this);
   if(ucWechselmodus) vpWechselSchalter[ucAnzahl]->bGruppe = true;
	 vpWechselSchalter[ucAnzahl]->Schriftfarbe(50, 50, 50);
	 vpWechselSchalter[ucAnzahl]->Hintergrundfarbe(150, 150, 150);
	 vpWechselSchalter[ucAnzahl]->Hintergrundfarbe_2(250, 250, 250);
	 vpWechselSchalter[ucAnzahl]->Hintergrundfarbe_3(200, 200, 200);
	 vpWechselSchalter[ucAnzahl]->Schriftausrichtung(TXA_MITTEVERTICAL | TXA_MITTEHORIZONTAL);
	 (ucAnzahl < 10 ? asName.Delete(asName.Length() - 1, 1) : asName.Delete(asName.Length(), 2));
	 ucAnzahl++;
 }
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::COTabellenKopfV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA, unsigned char ucSchalteranzahl)
{
 COGrafikV(vmSpeicher, pcRePag_TabellenKopf, pcFensterName, uiIDElementA);

 ErweitertStil(WS_EX_STATICEDGE | WS_EX_NOPARENTNOTIFY);
 stHintergrundfarbe.ucRot = stHintergrundfarbe.ucGrun = stHintergrundfarbe.ucBlau = 100;
 SetzHintergrund(false);

 ucModus = SGM_EINAUSINAKTIV;
 pfnWM_Paint = NULL;
 pfnAndernSpaltenBreite = NULL;
 sMausPos_x = 0;
 ucIndex = 0xFF;

 vpWechselSchalter = (COWechselSchalter**)VMBlock(vmSpeicher, ucSchalteranzahl * 4);
 char pc11Zahl[11]; COStringA asName = pcFensterName; asName += "_Schalter_";
 ucAnzahl = 0; uiIDElementA++;
 while(ucAnzahl < ucSchalteranzahl){
	 asName += ULONGtoCHAR(pc11Zahl, ucAnzahl);
   vpWechselSchalter[ucAnzahl] = (COWechselSchalter*)VMBlock(vmSpeicher, sizeof(COWechselSchalter));
	 vpWechselSchalter[ucAnzahl]->COWechselSchalterV(vmSpeicher, asName.c_Str(), uiIDElementA + ucAnzahl, this);
   vpWechselSchalter[ucAnzahl]->bGruppe = true;
	 vpWechselSchalter[ucAnzahl]->Schriftfarbe(50, 50, 50);
	 vpWechselSchalter[ucAnzahl]->Hintergrundfarbe(200, 180, 10);
	 vpWechselSchalter[ucAnzahl]->Hintergrundfarbe_2(245, 225, 5);
	 vpWechselSchalter[ucAnzahl]->Hintergrundfarbe_3(150, 150, 150);
	 vpWechselSchalter[ucAnzahl]->Schriftausrichtung(TXA_MITTEVERTICAL | TXA_MITTEHORIZONTAL);
	 (ucAnzahl < 10 ? asName.Delete(asName.Length() - 1, 1) : asName.Delete(asName.Length(), 2));
	 ucAnzahl++;
 }
}
//---------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::GUI::COSchalterGruppe::COFreiV(void)
{
 while(--ucAnzahl){ if(vpWechselSchalter[ucAnzahl]) DestroyWindow(vpWechselSchalter[ucAnzahl]->HWND_Element()); }
 if(vpWechselSchalter[ucAnzahl]) DestroyWindow(vpWechselSchalter[ucAnzahl]->HWND_Element());
 VMFrei(vmSpeicher, vpWechselSchalter);
 return ((COElement*)this)->COFreiV();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::COWechselSchalter::COWechselSchalterV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA, COSchalterGruppe* pSchalterGruppeA)
{
 COSchalterV(vmSpeicher, pcRePag_WechselSchalter, pcFensterName, uiIDElementA);
 pSchalterGruppe = pSchalterGruppeA;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::WM_Paint(void)
{
 ThreadSicher_Anfang();
 PAINTSTRUCT stPaint; 
 BeginPaint(hWndElement, &stPaint);
 WM_Paint_Hintergrund(stPaint);
 if(pfnWM_Paint) pfnWM_Paint(this, stPaint);
 EndPaint(hWndElement, &stPaint);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::COWechselSchalter::WM_LButtonUp(WPARAM wParam, LPARAM lParam)
{
 ThreadSicher_Anfang();
 if(pSchalterGruppe->ucModus){ BYTE ucIndexA = GetWindowLongPtr(hWndElement, GWLP_ID) - GetWindowLongPtr(pSchalterGruppe->HWND_Element(), GWLP_ID) - 1;
	 if(pSchalterGruppe->ucIndex < pSchalterGruppe->ucAnzahl){
		 if(pSchalterGruppe->ucModus == SGM_EINAUS){
			 if(pSchalterGruppe->ucIndex != ucIndexA){ Schalten(true);
				 if(pSchalterGruppe->vpWechselSchalter[pSchalterGruppe->ucIndex]->bGruppe) pSchalterGruppe->vpWechselSchalter[pSchalterGruppe->ucIndex]->SetzSchalter(false);
			 }
		 }
		 else{ SetzSchalter(!Eingeschaltet());
			 if(pSchalterGruppe->ucIndex != ucIndexA && pSchalterGruppe->vpWechselSchalter[pSchalterGruppe->ucIndex]->bGruppe)
				  pSchalterGruppe->vpWechselSchalter[pSchalterGruppe->ucIndex]->SetzAktiv(false);
		 }
	 }
	 else Schalten(true);
	 pSchalterGruppe->ucIndex = ucIndexA;
 }

 if(pfnWM_LButtonUp) pfnWM_LButtonUp(this);	
 else PostMessage(GetParent(hWndElement), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWndElement, GWLP_ID), wParam), lParam);
 ThreadSicher_Ende();
 ReleaseCapture();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::WM_LButtonDown(LPARAM lParam)
{
 SetCapture(hWndElement);
 ThreadSicher_Anfang();
 sMausPos_x = LOWORD(lParam);

 long lPos_x;
 ucSchalter = 0;
 while(ucSchalter < ucAnzahl && vpWechselSchalter[ucSchalter]->Pos_X(lPos_x) != sMausPos_x + 1) ucSchalter++;
 ucSchalter--;
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::WM_MouseMove(WPARAM wParam, LPARAM lParam)
{
 if(pfnAndernSpaltenBreite && wParam == MK_LBUTTON){ short sBreite = 0; long lSchalterBreite;  
   ThreadSicher_Anfang();
	 if(LOWORD(lParam) >= sMausPos_x) sBreite = LOWORD(lParam) - sMausPos_x;
	 else sBreite = LOWORD(lParam) - sMausPos_x;

	 if(vpWechselSchalter[ucSchalter]->Breite(lSchalterBreite) + sBreite >= 3){
		 //AndernFensterGrosse(0, sBreite);
		 lBreite += sBreite;
		 InvalidateRect(hWndElement, NULL, false);
		 MoveWindow(hWndElement, ptPosition.x, ptPosition.y, lBreite, lHohe, true);

		 vpWechselSchalter[ucSchalter]->AndernFensterGrosse(0, sBreite);
     vpWechselSchalter[ucSchalter]->UpdateFenster(NULL, true, false);

		 for(BYTE ucNummer = ucSchalter + 1; ucNummer < ucAnzahl; ucNummer++) vpWechselSchalter[ucNummer]->AndernFensterPosition(sBreite, 0);

		 sMausPos_x = LOWORD(lParam);
		 pfnAndernSpaltenBreite(GetParent(hWndElement), ucSchalter, sBreite);
	 }
	 ThreadSicher_Ende();
 }
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::ErstellSchalterInGruppe(unsigned char ucSchalterA, long lHoheA, long lBreiteA, long lPos_x, long lPos_y)
{
 if(ucSchalterA < ucAnzahl) vpWechselSchalter[ucSchalterA]->ErstellFenster(hWndElement, lHoheA, lBreiteA, lPos_x, lPos_y);
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::SetzAlleSchalter_Aus(void)
{
 ThreadSicher_Anfang();
 for(BYTE ucNummer = 0; ucNummer < ucAnzahl; ucNummer++) vpWechselSchalter[ucNummer]->SetzSchalter(false);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::SetzAlleSchalter_Aktiv(bool bAktiv)
{
 ThreadSicher_Anfang();
 for(BYTE ucNummer = 0; ucNummer < ucAnzahl; ucNummer++) vpWechselSchalter[ucNummer]->SetzAktiv(bAktiv);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
unsigned char __vectorcall RePag::GUI::COSchalterGruppe::SelectSchalter(void)
{
 ThreadSicher_Anfang();
 BYTE ucIndexA = ucIndex;
 ThreadSicher_Ende();
 return ucIndexA;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::Sperren(void)
{
 ThreadSicher_Anfang();
 for(BYTE ucNummer = 0; ucNummer < ucAnzahl; ucNummer++) vpWechselSchalter[ucNummer]->Sperren();
 EnableWindow(hWndElement, false);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::Freigeben(void)
{
 ThreadSicher_Anfang();
 for(BYTE ucNummer = 0; ucNummer < ucAnzahl; ucNummer++) vpWechselSchalter[ucNummer]->Freigeben();
 EnableWindow(hWndElement, true);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::Schalter_Beschriftung(unsigned char ucSchalterA, const char* pcBeschriftung)
{
 ThreadSicher_Anfang();
 if(ucSchalterA < ucAnzahl) vpWechselSchalter[ucSchalterA]->Beschriftung(pcBeschriftung);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::Schalter_Beschriftung_Ein(unsigned char ucSchalterA, const char* pcBeschriftung)
{
 ThreadSicher_Anfang();
 if(ucSchalterA < ucAnzahl) vpWechselSchalter[ucSchalterA]->Beschriftung_Ein(pcBeschriftung);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::Schalter_Beschriftung_Aus(unsigned char ucSchalterA, const char* pcBeschriftung)
{
 ThreadSicher_Anfang();
 if(ucSchalterA < ucAnzahl) vpWechselSchalter[ucSchalterA]->Beschriftung_Aus(pcBeschriftung);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::Schalter_SetzSchalter(unsigned char ucSchalterA, bool bEinschalten)
{
 ThreadSicher_Anfang();
 if(ucSchalterA < ucAnzahl){ vpWechselSchalter[ucSchalterA]->SetzSchalter(bEinschalten); if(bEinschalten) ucIndex = ucSchalterA; }
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COSchalterGruppe::Schalter_Aktiv(unsigned char ucSchalterA)
{
 ThreadSicher_Anfang();
 bool bReturn = false;
 if(ucSchalterA < ucAnzahl) bReturn = vpWechselSchalter[ucSchalterA]->Aktiv();
 ThreadSicher_Ende();
 return bReturn;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::Schalter_SetzAktiv(unsigned char ucSchalterA, bool bAktiv)
{
 ThreadSicher_Anfang();
 if(ucSchalterA < ucAnzahl) vpWechselSchalter[ucSchalterA]->SetzAktiv(bAktiv);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::Schalter_SetzGruppe(unsigned char ucSchalterA, bool bGruppe)
{
 ThreadSicher_Anfang();
 if(ucSchalterA < ucAnzahl) vpWechselSchalter[ucSchalterA]->bGruppe = bGruppe;
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::Schalter_SetzBreite(unsigned char ucSchalterA, long lBreiteA)
{
 ThreadSicher_Anfang();
 if(ucSchalterA < ucAnzahl) vpWechselSchalter[ucSchalterA]->AndernFensterGrosse(0, lBreiteA);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::Schalter_Schriftart(unsigned char ucSchalterA, LOGFONT& lfSchrift)
{
 ThreadSicher_Anfang();
 if(ucSchalterA < ucAnzahl) vpWechselSchalter[ucSchalterA]->Schriftart(lfSchrift);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::Schalter_Hintergrundfarbe(unsigned char ucSchalterA, unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
 ThreadSicher_Anfang();
 if(ucSchalterA < ucAnzahl) vpWechselSchalter[ucSchalterA]->Hintergrundfarbe(ucRot, ucGrun, ucBlau);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::Schalter_Hintergrundfarbe_1(unsigned char ucSchalterA, unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
 ThreadSicher_Anfang();
 if(ucSchalterA < ucAnzahl) vpWechselSchalter[ucSchalterA]->Hintergrundfarbe_1(ucRot, ucGrun, ucBlau);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::Schalter_Hintergrundfarbe_2(unsigned char ucSchalterA, unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
 ThreadSicher_Anfang();
 if(ucSchalterA < ucAnzahl) vpWechselSchalter[ucSchalterA]->Hintergrundfarbe_2(ucRot, ucGrun, ucBlau);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::Schalter_Hintergrundfarbe_3(unsigned char ucSchalterA, unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau)
{
 ThreadSicher_Anfang();
 if(ucSchalterA < ucAnzahl) vpWechselSchalter[ucSchalterA]->Hintergrundfarbe_3(ucRot, ucGrun, ucBlau);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
long& __vectorcall RePag::GUI::COSchalterGruppe::Schalter_Pos_X(unsigned char ucSchalterA, long& lPos_x)
{
 ThreadSicher_Anfang();
 if(ucSchalterA < ucAnzahl) vpWechselSchalter[ucSchalterA]->Pos_X(lPos_x);
 ThreadSicher_Ende();
 return lPos_x;
}
//---------------------------------------------------------------------------------------------------------------------------------------
long& __vectorcall RePag::GUI::COSchalterGruppe::Schalter_Pos_Y(unsigned char ucSchalterA, long& lPos_y)
{
 ThreadSicher_Anfang();
 if(ucSchalterA < ucAnzahl) vpWechselSchalter[ucSchalterA]->Pos_Y(lPos_y);
 ThreadSicher_Ende();
 return lPos_y;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::Schalter_SetzSichtbar(unsigned char ucSchalterA, bool bSichtbar)
{
 ThreadSicher_Anfang();
 if(ucSchalterA < ucAnzahl) vpWechselSchalter[ucSchalterA]->SetzSichtbar(bSichtbar);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::Schalter_NeuePosition(unsigned char ucSchalterA, long lPos_x, long lPos_y)
{
 ThreadSicher_Anfang();
 if(ucSchalterA < ucAnzahl) vpWechselSchalter[ucSchalterA]->NeueFensterPosition(lPos_x, lPos_y);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::Schalter_AndernPosition(unsigned char ucSchalterA, long lPos_x, long lPos_y)
{
 ThreadSicher_Anfang();
 if(ucSchalterA < ucAnzahl) vpWechselSchalter[ucSchalterA]->AndernFensterPosition(lPos_x, lPos_y);
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COSchalterGruppe::Schalter_Eingeschaltet(unsigned char ucSchalterA)
{
 ThreadSicher_Anfang();
 bool bReturn = false;
 if(ucSchalterA < ucAnzahl) bReturn = vpWechselSchalter[ucSchalterA]->Eingeschaltet();
 ThreadSicher_Ende();
 return bReturn;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::Schalter_pfnWM_LButtonUp(unsigned char ucSchalterA, void (__vectorcall *pfnSchalter_WM_LButtonUp)(COSchalter*))
{
 ThreadSicher_Anfang();
 if(ucSchalterA < ucAnzahl) vpWechselSchalter[ucSchalterA]->pfnWM_LButtonUp = pfnSchalter_WM_LButtonUp;
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COSchalterGruppe::Schalter_pfnWM_LButtonDown(unsigned char ucSchalterA, void (__vectorcall *pfnSchalter_WM_LButtonDown)(COSchalter*))
{
 ThreadSicher_Anfang();
 if(ucSchalterA < ucAnzahl) vpWechselSchalter[ucSchalterA]->pfnWM_LButtonDown = pfnSchalter_WM_LButtonDown;
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------------------------------------------------------------------