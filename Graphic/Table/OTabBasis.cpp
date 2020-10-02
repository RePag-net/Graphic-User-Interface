/****************************************************************************
	OTabBasis.cpp
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

#include "HTable.h"
#include "OTabBasis.h"
//-----------------------------------------------------------------------------------------------------------------------------------------
char pcFalscheSpalten[] = "Datei öffnen nicht möglich.\nDie Spalten in der Datei sind inkompatibel zur vorgegebenen Tabelle";
char pcKeineAktion[] = "Es muss erst eine Datenabfrage (auch Leerabfrage) erfolgen\nbevor eine Aktion ausgeführt werden kann.";
char pcNix[] = "Da is noch gar nix";
char pcFalscheDatei[] = "Die Datei ist falsch";
char pcDateiOffnen[] = "Fehler Datei Öffnen";
char pcDateiSpeichern[] = "Fehler Datei Speichern";
char pcKeinSpeichern[] = "Die Datei konnte nicht gespeichert werden";
char pcKeinOffnen[] = "Die Datei konnte nicht geöffnet werden";
#define _Kalender GetWindowLongPtr(hWndElement, GWLP_ID) + ucSpaltenanzahl + 16
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frWM_Paint_Kopf(COSchalterGruppe* psgKopf, PAINTSTRUCT& stPaint)
{
 COTabBasis* pTabelle = (COTabBasis*)GetWindowLongPtr(GetParent(psgKopf->HWND_Element()), GWLP_USERDATA);
 SCROLLINFO stScrollHorz; stScrollHorz.cbSize = sizeof(SCROLLINFO); stScrollHorz.fMask = SIF_POS | SIF_PAGE;

 HPEN hPen = CreatePen(PS_SOLID, 1, RGB(pTabelle->stHintergrundfarbe.ucRot, pTabelle->stHintergrundfarbe.ucGrun, pTabelle->stHintergrundfarbe.ucBlau));

 SelectObject(stPaint.hdc, hPen);

 GetScrollInfo(pTabelle->HWND_Element(), SB_HORZ, &stScrollHorz);

 long lPos_x;
 for(BYTE ucSpalte = stScrollHorz.nPos + 1; ucSpalte <= stScrollHorz.nPos + stScrollHorz.nPage + 1; ucSpalte++){
   psgKopf->Schalter_Pos_X(ucSpalte, lPos_x)--;
	 MoveToEx(stPaint.hdc, lPos_x, 0, NULL);
   LineTo(stPaint.hdc, lPos_x, pTabelle->ucKopfhohe);
 }
 if(psgKopf->Breite(lPos_x) < pTabelle->lRand_rechts){
	 MoveToEx(stPaint.hdc, --lPos_x, 0, NULL);
   LineTo(stPaint.hdc, lPos_x, pTabelle->ucKopfhohe);
 }
 DeleteObject(hPen);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frAndernSpaltenBreite(HWND hWndTabelle, unsigned char ucSpalte, long lBreite)
{
 COTabBasis* pTabelle = (COTabBasis*)GetWindowLongPtr(hWndTabelle, GWLP_USERDATA); POINT stCursor; RECT rcZeichnen;

 GetCursorPos(&stCursor);
 ScreenToClient(hWndTabelle, &stCursor);
 if(pTabelle->Breite(rcZeichnen.right) - 3 <= stCursor.x) ReleaseCapture();

 SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO);
 pTabelle->vstSpalten[ucSpalte].lBreite += lBreite;

 pTabelle->SetzScrollHorz(stScrollInfo);

 rcZeichnen.bottom = pTabelle->lRand_unten; rcZeichnen.right = pTabelle->lRand_rechts; rcZeichnen.top = pTabelle->ucKopfhohe;
 pTabelle->sgKopf->Schalter_Pos_X(ucSpalte, rcZeichnen.left);
 pTabelle->UpdateFenster(&rcZeichnen, true, false);

 stScrollInfo.fMask = SIF_POS;
 GetScrollInfo(pTabelle->HWND_Element(), SB_VERT, &stScrollInfo); 

 if(pTabelle->sgKopf->Schalter_Pos_X(pTabelle->ucSpalteSelect, rcZeichnen.left) >= 0 && rcZeichnen.left + pTabelle->vstSpalten[pTabelle->ucSpalteSelect].lBreite < pTabelle->lRand_rechts){
	 if(pTabelle->lbEingabe->Sichtbar()) pTabelle->Eingabe_Position(pTabelle->lbEingabe, stScrollInfo);
	 else if(pTabelle->ebEingabe->Sichtbar()) pTabelle->Eingabe_Position(pTabelle->ebEingabe, stScrollInfo);
	 else if(pTabelle->biEingabe->Sichtbar()) pTabelle->Eingabe_Position(pTabelle->biEingabe, stScrollInfo);
	 else if(pTabelle->doEingabe->Sichtbar()) pTabelle->Eingabe_Position(pTabelle->doEingabe, stScrollInfo);
	 else if(pTabelle->maEingabe->Sichtbar()) pTabelle->Eingabe_Position(pTabelle->maEingabe, stScrollInfo);
	 else if(pTabelle->mvEingabe->Sichtbar()) pTabelle->Eingabe_Position(pTabelle->mvEingabe, stScrollInfo);
	 else if(pTabelle->daEingabe->Sichtbar()) pTabelle->Eingabe_Position(pTabelle->daEingabe, stScrollInfo);
	 else if(pTabelle->ezEingabe->Sichtbar()){
		 pTabelle->ezEingabe->NeuesFenster(pTabelle->ucZeilenhohe, pTabelle->vstSpalten[pTabelle->ucSpalteSelect].lBreite,
				pTabelle->sgKopf->Schalter_Pos_X(pTabelle->ucSpalteSelect, rcZeichnen.left), pTabelle->ucZeilenhohe * (pTabelle->ulZeileSelect - stScrollInfo.nPos) + pTabelle->ucKopfhohe);
     if(!(pTabelle->vstSpalten[pTabelle->ucSpalteSelect].usFormat & TX_UNANDERBAR)) pTabelle->bAktiv = true;
	 }
	 else{ pTabelle->ezEingabe->NeuesFenster(pTabelle->ucZeilenhohe, pTabelle->vstSpalten[pTabelle->ucSpalteSelect].lBreite,
	 	     pTabelle->sgKopf->Schalter_Pos_X(pTabelle->ucSpalteSelect, rcZeichnen.left), pTabelle->ucZeilenhohe * (pTabelle->ulZeileSelect - stScrollInfo.nPos) + pTabelle->ucKopfhohe);
	   pTabelle->bAktiv = true;
		 if(pTabelle->bSelectiert){
			 if(pTabelle->bEinfugen || pTabelle->bFeldanderung) pTabelle->SelectZelle(pTabelle->ulZeileSelect, pTabelle->ucSpalteSelect);
			 else pTabelle->ezEingabe->SetzSichtbar(true);
		 }
	 }
 }
 else{ pTabelle->bAktiv = false;
	 if(pTabelle->ezEingabe->Sichtbar()){ pTabelle->ezEingabe->SetzSichtbar(false); pTabelle->ezEingabe->SetzVerfugbar(false); }
	 else if(pTabelle->ebEingabe->Sichtbar()){ pTabelle->ebEingabe->SetzSichtbar(false); if(pTabelle->bAnsehen){ pTabelle->bAnsehen = false; pTabelle->ebEingabe->SetzVerfugbar(false); pTabelle->ebEingabe->Zeichenvorgabe(ZV_ALLE); } }
	 else if(pTabelle->lbEingabe->Sichtbar()){ pTabelle->lbEingabe->SetzSichtbar(false); if(pTabelle->bAnsehen){ pTabelle->bAnsehen = false; pTabelle->lbEingabe->SetzVerfugbar(false); } }
	 else if(pTabelle->biEingabe->Sichtbar()){ pTabelle->biEingabe->SetzSichtbar(false); if(pTabelle->bAnsehen){ pTabelle->bAnsehen = false; pTabelle->biEingabe->SetzVerfugbar(false); } }
	 else if(pTabelle->doEingabe->Sichtbar()){ pTabelle->doEingabe->SetzSichtbar(false); if(pTabelle->bAnsehen){ pTabelle->bAnsehen = false; pTabelle->doEingabe->SetzVerfugbar(false); } }
 	 else if(pTabelle->maEingabe->Sichtbar()){ pTabelle->maEingabe->SetzSichtbar(false); if(pTabelle->bAnsehen){ pTabelle->bAnsehen = false; pTabelle->maEingabe->SetzVerfugbar(false); } }
 	 else if(pTabelle->mvEingabe->Sichtbar()){ pTabelle->mvEingabe->SetzSichtbar(false); if(pTabelle->bAnsehen){ pTabelle->bAnsehen = false; pTabelle->mvEingabe->SetzVerfugbar(false); } }
 	 else if(pTabelle->daEingabe->Sichtbar()){ pTabelle->daEingabe->SetzSichtbar(false); if(pTabelle->bAnsehen){ pTabelle->bAnsehen = false; pTabelle->daEingabe->SetzVerfugbar(false); } }
 }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frEingabeZeile_WM_Char_Return(COEditZeile* pezEingabe)
{
 COTabBasis* pTabelle = (COTabBasis*)GetWindowLongPtr(GetParent(pezEingabe->HWND_Element()), GWLP_USERDATA);
 pTabelle->Eingabe_Return();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frEingabeZeile_WM_Char_Escape(COEditZeile* pezEingabe)
{
 COTabBasis* pTabelle = (COTabBasis*)GetWindowLongPtr(GetParent(pezEingabe->HWND_Element()), GWLP_USERDATA);
 pTabelle->Eingabe_Escape();
 pezEingabe->SetzVerfugbar(false);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall fnEingabeZeile_WM_Command(COEditZeile* pezEingabe, WPARAM wParam)
{
 if(LOWORD(wParam) == IDM_DS_SENDEN) SendMessage(GetParent(pezEingabe->HWND_Element()), WM_COMMAND, IDM_DS_SENDEN, NULL);
 else return true;
 return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frEingabeBox_WM_Char_ShiftReturn(COEditBox* pebEingabe)
{
 COTabBasis* pTabelle = (COTabBasis*)GetWindowLongPtr(GetParent(pebEingabe->HWND_Element()), GWLP_USERDATA);
 pTabelle->Eingabe_Return();

 RECT rcZeichnen; pTabelle->UpdateFenster(&pebEingabe->Fenster(rcZeichnen), true, false);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frEingabeBox_WM_Char_Escape(COEditBox* pebEingabe)
{
 COTabBasis* pTabelle = (COTabBasis*)GetWindowLongPtr(GetParent(pebEingabe->HWND_Element()), GWLP_USERDATA);
 pTabelle->Eingabe_Escape();
 pebEingabe->SetzSichtbar(false); pebEingabe->SetzVerfugbar(false); pebEingabe->Zeichenvorgabe(ZV_ALLE);

 RECT rcZeichnen; pTabelle->UpdateFenster(&pebEingabe->Fenster(rcZeichnen), true, false);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall fnEingabeBox_WM_Command(COEditBox* pebEingabe, WPARAM wParam)
{
 if(LOWORD(wParam) == IDM_DS_SENDEN) SendMessage(GetParent(pebEingabe->HWND_Element()), WM_COMMAND, IDM_DS_SENDEN, NULL);
 else return true;
 return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frEingabeBild_WM_Char_Return(COBild* pbiEingabe)
{
 COTabBasis* pTabelle = (COTabBasis*)GetWindowLongPtr(GetParent(pbiEingabe->HWND_Element()), GWLP_USERDATA);
 pTabelle->Eingabe_Return();

 RECT rcZeichnen; pTabelle->UpdateFenster(&pbiEingabe->Fenster(rcZeichnen), true, false);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frEingabeBild_WM_Char_Escape(COBild* pbiEingabe)
{
 COTabBasis* pTabelle = (COTabBasis*)GetWindowLongPtr(GetParent(pbiEingabe->HWND_Element()), GWLP_USERDATA);
 pTabelle->Eingabe_Escape();
 pbiEingabe->SetzSichtbar(false); pbiEingabe->SetzVerfugbar(false); 

 RECT rcZeichnen; pTabelle->UpdateFenster(&pbiEingabe->Fenster(rcZeichnen), true, false);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall fnEingabeBild_WM_Command(COBild* pbiEingabe, WPARAM wParam)
{
 if(LOWORD(wParam) == IDM_DS_SENDEN) SendMessage(GetParent(pbiEingabe->HWND_Element()), WM_COMMAND, IDM_DS_SENDEN, NULL);
 else return true;
 return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frEingabeListe_WM_Char_Return(COListBox* plbEingabe)
{
 COTabBasis* pTabelle = (COTabBasis*)GetWindowLongPtr(GetParent(plbEingabe->HWND_Element()), GWLP_USERDATA);

 (pTabelle->bAnsehen ? pTabelle->bAnsehen = false : pTabelle->AndernWert(true));

 RECT rcZeichnen;
 plbEingabe->SetzSichtbar(false); plbEingabe->SetzVerfugbar(false);
 pTabelle->UpdateFenster(&plbEingabe->Fenster(rcZeichnen), true, false);

 COStringA* vasFeldWert = COStringAV(pTabelle->vpTabelle[pTabelle->ulZeileSelect][pTabelle->ucSpalteSelect].ulLange);
 MemCopy(vasFeldWert->c_Str(), pTabelle->vpTabelle[pTabelle->ulZeileSelect][pTabelle->ucSpalteSelect].vbWert, pTabelle->vpTabelle[pTabelle->ulZeileSelect][pTabelle->ucSpalteSelect].ulLange);
 pTabelle->ezEingabe->Text(vasFeldWert->c_Str()); VMFreiV(vasFeldWert);
 pTabelle->ezEingabe->SetzSichtbar(true);
 pTabelle->Eingabe_Return();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frEingabeListe_WM_Char_Escape(COListBox* plbEingabe)
{
 COTabBasis* pTabelle = (COTabBasis*)GetWindowLongPtr(GetParent(plbEingabe->HWND_Element()), GWLP_USERDATA);
 pTabelle->Eingabe_Escape();
 plbEingabe->SetzSichtbar(false); plbEingabe->SetzVerfugbar(false); 

 RECT rcZeichnen; pTabelle->UpdateFenster(&plbEingabe->Fenster(rcZeichnen), true, false);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall fnEingabeListe_WM_Command(COListBox* plbEingabe, WPARAM wParam)
{
 if(LOWORD(wParam) == IDM_DS_SENDEN) SendMessage(GetParent(plbEingabe->HWND_Element()), WM_COMMAND, IDM_DS_SENDEN, NULL);
 else return true;
 return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frEingabeListe_WM_LButtonUp(COListBox* plbEingabe)
{
 COTabBasis* pTabelle = (COTabBasis*)GetWindowLongPtr(GetParent(plbEingabe->HWND_Element()), GWLP_USERDATA);

 RECT rcZeichnen;
 if(pTabelle->bAnsehen) pTabelle->bAnsehen = false;
 else{ pTabelle->AndernWert(true); pTabelle->ezEingabe->Hintergrundfarbe(255, 255, 255); pTabelle->ezEingabe->Schriftfarbe(0, 0, 128);}

 plbEingabe->SetzSichtbar(false); plbEingabe->SetzVerfugbar(false);
 pTabelle->UpdateFenster(&plbEingabe->Fenster(rcZeichnen), true, false);

 COStringA* vasFeldWert = COStringAV(pTabelle->vpTabelle[pTabelle->ulZeileSelect][pTabelle->ucSpalteSelect].ulLange);
 MemCopy(vasFeldWert->c_Str(), pTabelle->vpTabelle[pTabelle->ulZeileSelect][pTabelle->ucSpalteSelect].vbWert, pTabelle->vpTabelle[pTabelle->ulZeileSelect][pTabelle->ucSpalteSelect].ulLange);
 pTabelle->ezEingabe->Text(vasFeldWert->c_Str()); VMFreiV(vasFeldWert);
 pTabelle->ezEingabe->SetzSichtbar(true);
 SetFocus(pTabelle->ezEingabe->HWND_Element());
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frEingabeDokument_WM_Char_ShiftReturn(CODokument* pdoEingabe)
{
 COTabBasis* pTabelle = (COTabBasis*)GetWindowLongPtr(GetParent(pdoEingabe->HWND_Element()), GWLP_USERDATA);
 pTabelle->Eingabe_Return();

 RECT rcZeichnen; pTabelle->UpdateFenster(&pdoEingabe->Fenster(rcZeichnen), true, false);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frEingabeDokument_WM_Char_Escape(CODokument* pdoEingabe)
{
 COTabBasis* pTabelle = (COTabBasis*)GetWindowLongPtr(GetParent(pdoEingabe->HWND_Element()), GWLP_USERDATA);
 pTabelle->Eingabe_Escape();
 pdoEingabe->SetzSichtbar(false); pdoEingabe->SetzVerfugbar(false); 

 RECT rcZeichnen; pTabelle->UpdateFenster(&pdoEingabe->Fenster(rcZeichnen), true, false);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall fnEingabeDokument_WM_Command(CODokument* pdoEingabe, WPARAM wParam)
{
 if(LOWORD(wParam) == IDM_DS_SENDEN) SendMessage(GetParent(pdoEingabe->HWND_Element()), WM_COMMAND, IDM_DS_SENDEN, NULL);
 else return true;
 return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frEingabeAudio_WM_Char_Return(COMedia* pmmEingabe)
{
 COTabBasis* pTabelle = (COTabBasis*)GetWindowLongPtr(GetParent(pmmEingabe->HWND_Element()), GWLP_USERDATA);
 pTabelle->Eingabe_Return();

 RECT rcZeichnen; pTabelle->UpdateFenster(&pmmEingabe->Fenster(rcZeichnen), true, false);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frEingabeAudio_WM_Char_Escape(COMedia* pmmEingabe)
{
 COTabBasis* pTabelle = (COTabBasis*)GetWindowLongPtr(GetParent(pmmEingabe->HWND_Element()), GWLP_USERDATA);
 pTabelle->Eingabe_Escape();
 pmmEingabe->SetzSichtbar(false); pmmEingabe->SetzVerfugbar(false); 

 RECT rcZeichnen; pTabelle->UpdateFenster(&pmmEingabe->Fenster(rcZeichnen), true, false);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall fnEingabeAudio_WM_Command(COMedia* pmmEingabe, WPARAM wParam)
{
 if(LOWORD(wParam) == IDM_DS_SENDEN) SendMessage(GetParent(pmmEingabe->HWND_Element()), WM_COMMAND, IDM_DS_SENDEN, NULL);
 else return true;
 return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frEingabeVideo_WM_Char_Return(COMedia* pmmEingabe)
{
 COTabBasis* pTabelle = (COTabBasis*)GetWindowLongPtr(GetParent(pmmEingabe->HWND_Element()), GWLP_USERDATA);
 pTabelle->Eingabe_Return();

 RECT rcZeichnen; pTabelle->UpdateFenster(&pmmEingabe->Fenster(rcZeichnen), true, false);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frEingabeVideo_WM_Char_Escape(COMedia* pmmEingabe)
{
 COTabBasis* pTabelle = (COTabBasis*)GetWindowLongPtr(GetParent(pmmEingabe->HWND_Element()), GWLP_USERDATA);
 pTabelle->Eingabe_Escape();
 pmmEingabe->SetzSichtbar(false); pmmEingabe->SetzVerfugbar(false); 

 RECT rcZeichnen; pTabelle->UpdateFenster(&pmmEingabe->Fenster(rcZeichnen), true, false);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall fnEingabeVideo_WM_Command(COMedia* pmmEingabe, WPARAM wParam)
{
 if(LOWORD(wParam) == IDM_DS_SENDEN) SendMessage(GetParent(pmmEingabe->HWND_Element()), WM_COMMAND, IDM_DS_SENDEN, NULL);
 else return true;
 return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frEingabeDatum_WM_Char_Return(CODatum* pedEingabe)
{
 COTabBasis* pTabelle = (COTabBasis*)GetWindowLongPtr(GetParent(pedEingabe->HWND_Element()), GWLP_USERDATA);

 (pTabelle->bAnsehen ? pTabelle->bAnsehen = false : pTabelle->AndernWert(true));

 pTabelle->daEingabe->SetzSichtbar(false); pTabelle->daEingabe->SetzVerfugbar(false);

 COTime zFeldWert; zFeldWert.Write(pTabelle->vpTabelle[pTabelle->ulZeileSelect][pTabelle->ucSpalteSelect].vbWert);
 if(!zFeldWert.FileTime().dwHighDateTime && !zFeldWert.FileTime().dwLowDateTime) pTabelle->ezEingabe->Text(NULL);
 else{ COStringA asFormat_Datum, asFormat_Zeit; char c20DatumZeit[20]; USHORT usFormat; VMBLOCK vbDatum;
   usFormat = pTabelle->vstSpalten[pTabelle->ucSpalteSelect].usFormat; usFormat >>= 3; usFormat <<= 3;
	 switch(usFormat){
		 case TXF_ZEIT            : pTabelle->ezEingabe->Text(zFeldWert.CHARTime(c20DatumZeit));
			                          break;
		 case TXF_DATUM           : pTabelle->ezEingabe->Text(zFeldWert.CHARDate(c20DatumZeit));
			                          break;
		 case TXF_DATUMZEIT       : pTabelle->ezEingabe->Text(zFeldWert.CHARDateTime(c20DatumZeit));
			                          break;
		 case TXF_DATUMEXTERN     : if(pTabelle->pfnDatumZeitFormat){ pTabelle->pfnDatumZeitFormat(pTabelle->ucSpalteSelect, &asFormat_Datum, nullptr);
			                             pTabelle->ezEingabe->Text(zFeldWert.VMBLOCKDateFormat(vbDatum, asFormat_Datum.c_Str())); VMFrei(vbDatum); break; }
		 case TXF_DATUMINTERN     : pTabelle->ezEingabe->Text(zFeldWert.VMBLOCKDateFormat(vbDatum, pTabelle->daEingabe->Datumformat(&asFormat_Datum)->c_Str())); VMFrei(vbDatum);
			                          break;
		 case TXF_DATUMZEITEXTERN : if(pTabelle->pfnDatumZeitFormat){ pTabelle->pfnDatumZeitFormat(pTabelle->ucSpalteSelect, &asFormat_Datum, &asFormat_Zeit);
																   pTabelle->ezEingabe->Text(zFeldWert.VMBLOCKDateTimeFormat(vbDatum, asFormat_Datum.c_Str(), asFormat_Zeit.c_Str(), true)); VMFrei(vbDatum); break; }
		 case TXF_DATUMZEITINTERN : pTabelle->ezEingabe->Text(zFeldWert.VMBLOCKDateTimeFormat(vbDatum, pTabelle->daEingabe->Datumformat(&asFormat_Datum)->c_Str(),
																	 pTabelle->daEingabe->Zeitformat(&asFormat_Zeit)->c_Str(), true)); VMFrei(vbDatum);
			                          break;
		 case TXF_ZEITDATUMEXTERN : if(pTabelle->pfnDatumZeitFormat){ pTabelle->pfnDatumZeitFormat(pTabelle->ucSpalteSelect, &asFormat_Datum, &asFormat_Zeit);
																   pTabelle->ezEingabe->Text(zFeldWert.VMBLOCKDateTimeFormat(vbDatum, asFormat_Datum.c_Str(), asFormat_Zeit.c_Str(), false)); VMFrei(vbDatum); break; }
		 case TXF_ZEITDATUMINTERN : pTabelle->ezEingabe->Text(zFeldWert.VMBLOCKDateTimeFormat(vbDatum, asFormat_Datum.c_Str(), asFormat_Zeit.c_Str(), false)); VMFrei(vbDatum);
																break;
   }
 }

 pTabelle->ezEingabe->SetzSichtbar(true);
 pTabelle->Eingabe_Return();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frEingabeDatum_WM_Char_Escape(CODatum* pedEingabe)
{
 COTabBasis* pTabelle = (COTabBasis*)GetWindowLongPtr(GetParent(pedEingabe->HWND_Element()), GWLP_USERDATA);
 pTabelle->Eingabe_Escape();
 pedEingabe->SetzSichtbar(false); pedEingabe->SetzVerfugbar(false); 

 RECT rcZeichnen; pTabelle->UpdateFenster(&pedEingabe->Fenster(rcZeichnen), true, false);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall fnEingabeDatum_WM_Command(CODatum* pdaEingabe, WPARAM wParam)
{
 if(LOWORD(wParam) == IDM_DS_SENDEN) SendMessage(GetParent(pdaEingabe->HWND_Element()), WM_COMMAND, IDM_DS_SENDEN, NULL);
 else return true;
 return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::frEingabeDatum_WM_LButtonUp(CODatum* pdaEingabe)
{
 COTabBasis* pTabelle = (COTabBasis*)GetWindowLongPtr(GetParent(pdaEingabe->HWND_Element()), GWLP_USERDATA);

 if(pTabelle->bAnsehen) pTabelle->bAnsehen = false;
 else{ pTabelle->AndernWert(true); pTabelle->ezEingabe->Hintergrundfarbe(255, 255, 255); pTabelle->ezEingabe->Schriftfarbe(0, 0, 128); }

 pdaEingabe->SetzSichtbar(false); pdaEingabe->SetzVerfugbar(false);

 COTime zFeldWert; zFeldWert.Write(pTabelle->vpTabelle[pTabelle->ulZeileSelect][pTabelle->ucSpalteSelect].vbWert);
 if(!zFeldWert.FileTime().dwHighDateTime && !zFeldWert.FileTime().dwLowDateTime) pTabelle->ezEingabe->Text(NULL);
 else{ COStringA asFormat_Datum, asFormat_Zeit; char c20DatumZeit[20]; USHORT usFormat; VMBLOCK vbDatum;
	 usFormat = pTabelle->vstSpalten[pTabelle->ucSpalteSelect].usFormat; usFormat >>= 3; usFormat <<= 3;
	 switch(usFormat){
		 case TXF_ZEIT            : pTabelle->ezEingabe->Text(zFeldWert.CHARTime(c20DatumZeit));
																break;
		 case TXF_DATUM           : pTabelle->ezEingabe->Text(zFeldWert.CHARDate(c20DatumZeit));
																break;
		 case TXF_DATUMZEIT       : pTabelle->ezEingabe->Text(zFeldWert.CHARDateTime(c20DatumZeit));
																break;
		 case TXF_DATUMEXTERN     : if(pTabelle->pfnDatumZeitFormat){ pTabelle->pfnDatumZeitFormat(pTabelle->ucSpalteSelect, &asFormat_Datum, nullptr);
																	pTabelle->ezEingabe->Text(zFeldWert.VMBLOCKDateFormat(vbDatum, asFormat_Datum.c_Str())); VMFrei(vbDatum); break;}
		 case TXF_DATUMINTERN     : pTabelle->ezEingabe->Text(zFeldWert.VMBLOCKDateFormat(vbDatum, pTabelle->daEingabe->Datumformat(&asFormat_Datum)->c_Str())); VMFrei(vbDatum);
																break;
		 case TXF_DATUMZEITEXTERN : if(pTabelle->pfnDatumZeitFormat){ pTabelle->pfnDatumZeitFormat(pTabelle->ucSpalteSelect, &asFormat_Datum, &asFormat_Zeit);
																	pTabelle->ezEingabe->Text(zFeldWert.VMBLOCKDateTimeFormat(vbDatum, asFormat_Datum.c_Str(), asFormat_Zeit.c_Str(), true)); VMFrei(vbDatum); break; }
		 case TXF_DATUMZEITINTERN : pTabelle->ezEingabe->Text(zFeldWert.VMBLOCKDateTimeFormat(vbDatum, pTabelle->daEingabe->Datumformat(&asFormat_Datum)->c_Str(),
																	pTabelle->daEingabe->Zeitformat(&asFormat_Zeit)->c_Str(), true)); VMFrei(vbDatum);
																break;
		 case TXF_ZEITDATUMEXTERN : if(pTabelle->pfnDatumZeitFormat){ pTabelle->pfnDatumZeitFormat(pTabelle->ucSpalteSelect, &asFormat_Datum, &asFormat_Zeit);
																	pTabelle->ezEingabe->Text(zFeldWert.VMBLOCKDateTimeFormat(vbDatum, asFormat_Datum.c_Str(), asFormat_Zeit.c_Str(), false)); VMFrei(vbDatum); break; }
		 case TXF_ZEITDATUMINTERN : pTabelle->ezEingabe->Text(zFeldWert.VMBLOCKDateTimeFormat(vbDatum, asFormat_Datum.c_Str(), asFormat_Zeit.c_Str(), false)); VMFrei(vbDatum);
																break;
	 }
 }
 pTabelle->ezEingabe->SetzSichtbar(true);
 SetFocus(pTabelle->ezEingabe->HWND_Element());
}
//-----------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RePag::GUI::WndProc_Tabelle(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
 COTabBasis* pTabelle;
 switch(uiMessage){
    case WM_CREATE        : ((COTabBasis*)((LPCREATESTRUCT)lParam)->lpCreateParams)->WM_Create(hWnd);
												    return NULL;
		case WM_SIZE          : pTabelle = (COTabBasis*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
														if(pTabelle) pTabelle->WM_Size(lParam);
														else return DefWindowProc(hWnd, uiMessage, wParam, lParam);
			                      return NULL;
		case WM_KEYDOWN       : ((COTabBasis*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_KeyDown(wParam);
												    return NULL;
		case WM_COMMAND       : pTabelle = (COTabBasis*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			                      if(!pTabelle->WM_Command(wParam)) return NULL;
														else PostMessage(GetParent(hWnd), WM_COMMAND, wParam, lParam);
			                      break;
		case WM_CONTEXTMENU   : ((COTabBasis*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_ContexMenu(lParam);
			                      return NULL;
		case WM_VSCROLL       : ((COTabBasis*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_VScroll(wParam);
													  return NULL;
		case WM_HSCROLL       : ((COTabBasis*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_HScroll(wParam);
													  return NULL;
		case WM_LBUTTONDBLCLK : SendMessage(hWnd, WM_COMMAND, IDM_DS_ANDERN, NULL);
                            return NULL;
		case WM_LBUTTONUP     : ((COTabBasis*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_LButtonUp(lParam);
			                      PostMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWnd, GWLP_ID), wParam), lParam);
			                      return NULL; 
		case WM_MOUSEWHEEL    : ((COTabBasis*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_MouseWheel(wParam, lParam);
			                      return NULL;
		case WM_PAINT         : ((COTabBasis*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WM_Paint();
										        return NULL;
		case WM_NCDESTROY     : pTabelle = (COTabBasis*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
														if(pTabelle->htEffekt_Timer) DeleteTimerQueueTimer(TimerQueue(), pTabelle->htEffekt_Timer, INVALID_HANDLE_VALUE);
			                      VMFreiV(pTabelle);
			                      return NULL;
 }
 return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::COTabBasisV(VMEMORY vmSpeicher, const char* pcKlassenName, const char* pcFensterName, unsigned int uiIDElementA, unsigned long ulZeilen_Blatt, unsigned char ucSpalten)
{
 COGrafikV(vmSpeicher, pcKlassenName, pcFensterName, uiIDElementA);

 lfSchrift.lfFaceName[0] = '\0';
 lfSchrift.lfEscapement = GM_COMPATIBLE;
 lfSchrift.lfOrientation = GM_COMPATIBLE;
 lfSchrift.lfItalic = FALSE;
 lfSchrift.lfUnderline = FALSE;
 lfSchrift.lfStrikeOut = FALSE;
 lfSchrift.lfCharSet = DEFAULT_CHARSET;
 lfSchrift.lfOutPrecision = OUT_DEVICE_PRECIS;
 lfSchrift.lfClipPrecision = CLIP_DEFAULT_PRECIS;
 lfSchrift.lfQuality = DEFAULT_QUALITY;
 lfSchrift.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

 hMenu = CreatePopupMenu();
 AppendMenu(hMenu, MF_STRING, IDM_AUSSCHNEIDEN, "Ausschneiden		Strg+X");
 AppendMenu(hMenu, MF_STRING, IDM_KOPIEREN, "Kopieren		Strg+C");
 AppendMenu(hMenu, MF_STRING, IDM_EINFUGEN, "Einfügen		Strg+V");

 AppendMenu(hMenu, MF_SEPARATOR, NULL, NULL);
 AppendMenu(hMenu, MF_STRING, IDM_DS_LOSCHEN, "Datensatz Löschen		Strg+Entf");
 AppendMenu(hMenu, MF_STRING, IDM_DS_EINFUGEN, "Datensatz Einfügen		Einfg");
 AppendMenu(hMenu, MF_STRING, IDM_DS_ANDERN, "Datensatz Ändern		F2");
 AppendMenu(hMenu, MF_STRING, IDM_DS_SENDEN, "Datensatz Senden		F3");
 AppendMenu(hMenu, MF_STRING, IDM_DF_ANSEHEN, "Datenfeld Ansehen		F4");

 AppendMenu(hMenu, MF_SEPARATOR, NULL, NULL);
 AppendMenu(hMenu, MF_STRING, IDM_DATEI_OFFNEN, "Tabelle Öffnen		Strg+O");
 AppendMenu(hMenu, MF_STRING, IDM_DATEI_SPEICHERNUNTER, "Tabelle Speichern unter...");
 AppendMenu(hMenu, MF_STRING, IDM_DATEI_SPEICHERN, "Tabelle Speichern		Strg+S");

 heSelect = CreateEvent(nullptr, false, false, nullptr);
 vmTabelle = vmTabellen;

 ulTabZeilen_blatt = ulZeilen_Blatt; ulDSAnzahl = 0;
 ucSpaltenanzahl = ucSpalten;
 ucZusatzspalten = 0;
 ucKopfhohe = 25; ucZeilenhohe = 20;
 ucSpalteSelect = 0; ulZeileSelect = 0;

 bEinfugen = false;
 bFeldanderung = false;
 bSelectiert = false;
 bAnsehen = false;
 bAktiv = false;
 ucMaxSpalte = 0xFF;
 vstFelderAlt = nullptr;
 ucRechte = 255;
 vbBool_true = VMBlock(2); vbBool_true[0] = 0x31; vbBool_true[1] = 0;
 vbBool_false = VMBlock(2); vbBool_false[0] = 0x30; vbBool_false[1] = 0;

 pfnEingabeMaske = nullptr;
 pfnPrufenWert = nullptr;
 pfnDSSenden = nullptr;
 pfnServerVerbunden = nullptr;
 pfnEingabeListe = nullptr;
 pfnAndernWert_Zusatz = nullptr;
 pfnLeerFeld = nullptr;
 pfnDatumZeitFormat = nullptr;

 vucFeldStatus = nullptr;
 vbFeldGeandert = nullptr;
 vucDBTabellenSpalte = nullptr;

 lfSchrift.lfHeight = -14;
 lfSchrift.lfWidth = 5;
 lfSchrift.lfWeight = FW_SEMIBOLD;

 vasNeuerWert = COStringAV(vmSpeicher, pcFensterName);
 *vasNeuerWert += "_sgTabKopf"; 
 sgKopf = (COSchalterGruppe*)VMBlock(vmSpeicher, sizeof(COSchalterGruppe));
 sgKopf->COTabellenKopfV(vmSpeicher, vasNeuerWert->c_Str(), ++uiIDElementA, ucSpaltenanzahl);
 sgKopf->ErweitertStil(WS_EX_LEFT | WS_EX_NOPARENTNOTIFY);
 sgKopf->pfnWM_Paint = frWM_Paint_Kopf;
 sgKopf->pfnAndernSpaltenBreite = frAndernSpaltenBreite;
 vstSpalten = (STSpalte*)VMBlock(vmSpeicher, sizeof(STSpalte) * ucSpaltenanzahl);
 for(BYTE ucSpalte = 0; ucSpalte < ucSpaltenanzahl; ucSpalte++){
	 vstSpalten[ucSpalte].lBreite = 10;
	 vstSpalten[ucSpalte].usFormat = TXA_LINKS | TXA_UNTEN;
	 sgKopf->Schalter_Schriftart(ucSpalte, lfSchrift);
 }
 uiIDElementA += ucSpaltenanzahl;
 lfSchrift.lfHeight = -13;
 lfSchrift.lfWidth = 0;
 lfSchrift.lfWeight = FW_DONTCARE;

 *vasNeuerWert = pcFensterName; *vasNeuerWert += "_ezEingabe";
 ezEingabe = COEditZeileV(vmSpeicher, vasNeuerWert->c_Str(), ++uiIDElementA);
 ezEingabe->SetzZeichen_Max(255);
 ezEingabe->Zeichenvorgabe(ZV_ALLE | ZV_UBERLANGE);
 ezEingabe->Schriftart(lfSchrift);
 ezEingabe->pfnWM_Char_Return = frEingabeZeile_WM_Char_Return;
 ezEingabe->pfnWM_Char_Escape = frEingabeZeile_WM_Char_Escape;
 ezEingabe->pfnWM_Command = fnEingabeZeile_WM_Command;

 *vasNeuerWert = pcFensterName; *vasNeuerWert += "_ebEingabe";
 ebEingabe = COEditBoxV(vmSpeicher, vasNeuerWert->c_Str(), ++uiIDElementA);
 ebEingabe->Schriftart(lfSchrift);
 ebEingabe->FensterStil(WS_CHILDWINDOW | WS_THICKFRAME | WS_VISIBLE);
 ebEingabe->ErweitertStil(WS_EX_STATICEDGE | WS_EX_NOPARENTNOTIFY);
 ebEingabe->pfnWM_Char_ShiftReturn = frEingabeBox_WM_Char_ShiftReturn;
 ebEingabe->pfnWM_Char_Escape = frEingabeBox_WM_Char_Escape;
 ebEingabe->pfnWM_Command = fnEingabeBox_WM_Command;
 stGrosse_Textbox.lHohe = 300; stGrosse_Textbox.lBreite = 300;

 *vasNeuerWert = pcFensterName; *vasNeuerWert += "_lbEingabe";
 lbEingabe = COListBoxV(vmSpeicher, vasNeuerWert->c_Str(), ++uiIDElementA);
 lbEingabe->Hintergrundfarbe(120, 120, 120);
 lbEingabe->Schriftfarbe(240, 240, 240);
 lbEingabe->Schriftart(lfSchrift);
 lbEingabe->pfnWM_Char_Return = frEingabeListe_WM_Char_Return;
 lbEingabe->pfnWM_Char_Escape = frEingabeListe_WM_Char_Escape;
 lbEingabe->pfnWM_Command = fnEingabeListe_WM_Command;
 lbEingabe->pfnWM_LButtonUp = frEingabeListe_WM_LButtonUp;
 stGrosse_Listbox.lHohe = 120; stGrosse_Listbox.lBreite = 120;

 *vasNeuerWert = pcFensterName; *vasNeuerWert += "_biEingabe";
 biEingabe = COBildV(vmSpeicher, vasNeuerWert->c_Str(), ++uiIDElementA);
 biEingabe->Hintergrundfarbe(120, 120, 120);
 biEingabe->Schriftfarbe(240, 240, 240);
 biEingabe->Schriftart(lfSchrift);
 biEingabe->FensterStil(WS_CHILDWINDOW | WS_THICKFRAME | WS_VISIBLE);
 biEingabe->ErweitertStil(WS_EX_STATICEDGE | WS_EX_NOPARENTNOTIFY);
 biEingabe->pfnWM_Char_Return = frEingabeBild_WM_Char_Return;
 biEingabe->pfnWM_Char_Escape = frEingabeBild_WM_Char_Escape;
 biEingabe->pfnWM_Command = fnEingabeBild_WM_Command;
 stGrosse_Bild.lHohe = 300; stGrosse_Bild.lBreite = 300;

 *vasNeuerWert = pcFensterName; *vasNeuerWert += "_doEingabe";
 doEingabe = CODokumentV(vmSpeicher, vasNeuerWert->c_Str(), ++uiIDElementA);
 doEingabe->pfnWM_Char_ShiftReturn = frEingabeDokument_WM_Char_ShiftReturn;
 doEingabe->pfnWM_Char_Escape = frEingabeDokument_WM_Char_Escape;
 doEingabe->pfnWM_Command = fnEingabeDokument_WM_Command;

 *vasNeuerWert = pcFensterName; *vasNeuerWert += "_maEingabe";
 maEingabe = COAudioV(vmSpeicher, vasNeuerWert->c_Str(), ++uiIDElementA);
 maEingabe->Hintergrundfarbe(120, 120, 120);
 maEingabe->Schriftfarbe(240, 240, 240);
 maEingabe->Schriftart(lfSchrift);
 maEingabe->FensterStil(WS_CHILDWINDOW | WS_THICKFRAME | WS_VISIBLE);
 maEingabe->ErweitertStil(WS_EX_STATICEDGE | WS_EX_NOPARENTNOTIFY);
 maEingabe->pfnWM_Char_Return = frEingabeAudio_WM_Char_Return;
 maEingabe->pfnWM_Char_Escape = frEingabeAudio_WM_Char_Escape;
 maEingabe->pfnWM_Command = fnEingabeAudio_WM_Command;
 stGrosse_Audio.lHohe = 65; stGrosse_Audio.lBreite = 300;

 uiIDElementA += 4;
 *vasNeuerWert = pcFensterName; *vasNeuerWert += "_mvEingabe";
 mvEingabe = COVideoV(vmSpeicher, vasNeuerWert->c_Str(), uiIDElementA);
 mvEingabe->Hintergrundfarbe(120, 120, 120);
 mvEingabe->Schriftfarbe(240, 240, 240);
 mvEingabe->Schriftart(lfSchrift);
 mvEingabe->FensterStil(WS_CHILDWINDOW | WS_THICKFRAME | WS_VISIBLE);
 mvEingabe->ErweitertStil(WS_EX_STATICEDGE | WS_EX_NOPARENTNOTIFY);
 mvEingabe->pfnWM_Char_Return = frEingabeVideo_WM_Char_Return;
 mvEingabe->pfnWM_Char_Escape = frEingabeVideo_WM_Char_Escape;
 mvEingabe->pfnWM_Command = fnEingabeVideo_WM_Command;
 stGrosse_Video.lHohe = 300; stGrosse_Video.lBreite = 300;

 uiIDElementA += 4;
 *vasNeuerWert = pcFensterName; *vasNeuerWert += "_daEingabe";
 daEingabe = CODatumV(vmSpeicher, vasNeuerWert->c_Str(), uiIDElementA);
 daEingabe->Hintergrundfarbe(120, 120, 120);
 daEingabe->Schriftfarbe(240, 240, 240);
 daEingabe->Schriftart(lfSchrift);
 daEingabe->Schriftausrichtung_EditDatum(TXA_LINKS | TXA_UNTEN);
 daEingabe->pfnWM_Char_Return = frEingabeDatum_WM_Char_Return;
 daEingabe->pfnWM_Char_Escape = frEingabeDatum_WM_Char_Escape;
 daEingabe->pfnWM_Command = fnEingabeDatum_WM_Command;
 daEingabe->pfnWM_LButtonUp = frEingabeDatum_WM_LButtonUp;
 stGrosse_Datum.lHohe = 140; stGrosse_Datum.lBreite = 140;

 *vasNeuerWert = NULL;
 vasDateiname = COStringAV();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::GUI::COTabBasis::COFreiV(void)
{
 VMFreiV(vasNeuerWert); VMFreiV(vasDateiname); VMFrei(vbBool_true); VMFrei(vbBool_false);
 CloseHandle(heSelect); DestroyMenu(hMenu);

 while(ulDSAnzahl){ ulDSAnzahl--;
   for(BYTE ucSpalte = 0; ucSpalte < ucSpaltenanzahl + ucZusatzspalten; ucSpalte++){
     if(vpTabelle[ulDSAnzahl][ucSpalte].vbWert) VMFreiS(vmTabelle, vpTabelle[ulDSAnzahl][ucSpalte].vbWert);
   }
   VMFreiS(vmTabelle, vpTabelle[ulDSAnzahl]);
 }
 if(vpTabelle)VMFreiS(vmTabelle, vpTabelle);

 VMFrei(vmSpeicher, vstSpalten);

 return ((COElement*)this)->COFreiV();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::WM_Paint(void)
{
 ThreadSicher_Anfang();
 PAINTSTRUCT stPaint;
 BeginPaint(hWndElement, &stPaint);

 SCROLLINFO siZeile; siZeile.fMask = SIF_ALL; siZeile.cbSize = sizeof(SCROLLINFO); RECT rcZelle;
 GetScrollInfo(hWndElement, SB_VERT, &siZeile);

 if(!siZeile.nMax || stPaint.rcPaint.top >= lZeichnen_unten) WM_Paint_Hintergrund(stPaint);
 else{ ULONG ulZeile; BYTE ucSpalte, ucSpalteTemp; long lLeftTemp;
	 SIZE stZeichengrosse; POINT ptText; char c37Zahl[37]; COStringA asFeldWert;
	 SCROLLINFO siSpalte; siSpalte.cbSize = sizeof(SCROLLINFO); siSpalte.fMask = SIF_ALL;
	 GetScrollInfo(hWndElement, SB_HORZ, &siSpalte);

	 rcZelle.left = 0; rcZelle.top = ucKopfhohe;
   HFONT hFont = CreateFontIndirect(&lfSchrift); SelectObject(stPaint.hdc, hFont);
	 TEXTMETRIC stTextmetric; GetTextMetrics(stPaint.hdc, &stTextmetric); long lZeichen_hohe = stTextmetric.tmHeight + stTextmetric.tmExternalLeading;
	 HPEN hPen = CreatePen(PS_SOLID, 1, RGB(stHintergrundfarbe.ucRot, stHintergrundfarbe.ucGrun, stHintergrundfarbe.ucBlau));
   SelectObject(stPaint.hdc, hPen); 

	 ucSpalte = siSpalte.nPos;
	 while(rcZelle.left < stPaint.rcPaint.left) rcZelle.left += vstSpalten[ucSpalte++].lBreite + 1;
	 if(ucSpalte > siSpalte.nMax || rcZelle.left > stPaint.rcPaint.left) rcZelle.left -= vstSpalten[--ucSpalte].lBreite + 1; 
   ucSpalteTemp = ucSpalte; lLeftTemp = rcZelle.left;
	 
	 ulZeile = siZeile.nPos;
	 while(rcZelle.top < stPaint.rcPaint.top){ rcZelle.top += ucZeilenhohe; ulZeile++;} 
	 if(ulZeile && rcZelle.top > stPaint.rcPaint.top){ rcZelle.top -= ucZeilenhohe; ulZeile--; }

	 do{
		 do{
			 rcZelle.bottom = rcZelle.top + ucZeilenhohe; rcZelle.right = rcZelle.left + vstSpalten[ucSpalte].lBreite;

			 switch(FeldTyp(vucFeldStatus[ucSpalte])){
			    case FT_EMPTY      : 
					case FT_LISTSTR    :
					case FT_SHORTSTR   : asFeldWert.SetLength(vpTabelle[ulZeile][ucSpalte].ulLange);
															 MemCopy(asFeldWert.c_Str(), vpTabelle[ulZeile][ucSpalte].vbWert, vpTabelle[ulZeile][ucSpalte].ulLange);
															 SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 break;
					case FT_BOOL       : (*(bool*)vpTabelle[ulZeile][ucSpalte].vbWert ? asFeldWert = vbBool_true : asFeldWert = vbBool_false);
															 SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 break;
					case FT_CHAR       : asFeldWert = LONGtoCHAR(c37Zahl, *(PCHAR)vpTabelle[ulZeile][ucSpalte].vbWert);
															 SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 break;
					case FT_BYTE       : asFeldWert = ULONGtoCHAR(c37Zahl, *(PBYTE)vpTabelle[ulZeile][ucSpalte].vbWert);
															 SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 break;
					case FT_SHORT      : asFeldWert = LONGtoCHAR(c37Zahl, *(PSHORT)vpTabelle[ulZeile][ucSpalte].vbWert);
															 SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 break;
					case FT_USHORT     : asFeldWert = ULONGtoCHAR(c37Zahl, *(PUSHORT)vpTabelle[ulZeile][ucSpalte].vbWert);
															 SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 break;
					case FT_LONG       : asFeldWert = LONGtoCHAR(c37Zahl, *(PLONG)vpTabelle[ulZeile][ucSpalte].vbWert);
															 SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 break;
					case FT_ULONG      : asFeldWert = ULONGtoCHAR(c37Zahl, *(PULONG)vpTabelle[ulZeile][ucSpalte].vbWert);
															 SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 break;
 					case FT_LONGLONG   : asFeldWert = LONGLONGtoCHAR(c37Zahl, *(PLONGLONG)vpTabelle[ulZeile][ucSpalte].vbWert);
															 SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 break;
 					case FT_FLOAT      : asFeldWert = FLOAT_B10toCHAR(c37Zahl, *(PFLOAT)vpTabelle[ulZeile][ucSpalte].vbWert, KommaStellen(ucSpalte));
															 SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 break;
 					case FT_DOUBLE     : asFeldWert = DOUBLE_B10toCHAR(c37Zahl, *(double*)vpTabelle[ulZeile][ucSpalte].vbWert, KommaStellen(ucSpalte));
															 SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 break;
					case FT_COMMA4     : WM_Paint_Komma4(&asFeldWert, ulZeile, ucSpalte); SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 break;
					case FT_COMMA4_80  : WM_Paint_Komma4_80(&asFeldWert, ulZeile, ucSpalte); SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 break;
					case FT_TIME       : WM_Paint_Zeit(&asFeldWert, ulZeile, ucSpalte); SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 break;
					case FT_DATE       : WM_Paint_Datum(&asFeldWert, ulZeile, ucSpalte); SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 break;
					case FT_BIT128     : asFeldWert = BIT128toGUID(c37Zahl, *(BIT128*)vpTabelle[ulZeile][ucSpalte].vbWert); SetTextColor(stPaint.hdc, RGB(0, 0, 0));
						                   break;
					case FT_MEMOSTR    : asFeldWert = "Memo";
															 if(!vpTabelle[ulZeile][ucSpalte].ulLange) SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 else SetTextColor(stPaint.hdc, RGB(10, 150, 10));
															 break;
					case FT_LONGSTR    : asFeldWert = "Text";
															 if(!vpTabelle[ulZeile][ucSpalte].ulLange) SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 else SetTextColor(stPaint.hdc, RGB(10, 150, 10));
															 break;
					case FT_SMALLIMAGE : asFeldWert = "Kleinbild";
															 if(!vpTabelle[ulZeile][ucSpalte].ulLange) SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 else SetTextColor(stPaint.hdc, RGB(10, 150, 10));
															 break;
					case FT_BIGIMAGE   : asFeldWert = "Grossbild";
															 if(!vpTabelle[ulZeile][ucSpalte].ulLange) SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 else SetTextColor(stPaint.hdc, RGB(10, 150, 10));
															 break;
					case FT_DOCUMENT   : asFeldWert = "Dokument";
															 if(!vpTabelle[ulZeile][ucSpalte].ulLange) SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 else SetTextColor(stPaint.hdc, RGB(10, 150, 10));
															 break;
					case FT_AUDIO      : asFeldWert = "Audio";
															 if(!vpTabelle[ulZeile][ucSpalte].ulLange) SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 else SetTextColor(stPaint.hdc, RGB(10, 150, 10));
															 break;
					case FT_VIDEO      : asFeldWert = "Video";
															 if(!vpTabelle[ulZeile][ucSpalte].ulLange) SetTextColor(stPaint.hdc, RGB(0, 0, 0));
															 else SetTextColor(stPaint.hdc, RGB(10, 150, 10));
															 break;
			 }

			 GetTextExtentPoint32(stPaint.hdc, asFeldWert.c_Str(), asFeldWert.Length(), &stZeichengrosse);
			 ptText.x = rcZelle.left; ptText.y = rcZelle.top;
			 if(vstSpalten[ucSpalte].usFormat & TXA_RECHTS) ptText.x = rcZelle.right - stZeichengrosse.cx;
			 if(vstSpalten[ucSpalte].usFormat & TXA_MITTEHORIZONTAL) ptText.x += (rcZelle.right - rcZelle.left - stZeichengrosse.cx) / 2;
			 if(vstSpalten[ucSpalte].usFormat & TXA_UNTEN) ptText.y = rcZelle.bottom - lZeichen_hohe;
			 if(vstSpalten[ucSpalte].usFormat & TXA_MITTEVERTICAL) ptText.y += (rcZelle.bottom - rcZelle.top - lZeichen_hohe) / 2;

 			 if(ulZeile & 1) SetBkColor(stPaint.hdc, RGB(190, 190, 190));
			 else SetBkColor(stPaint.hdc, RGB(240, 240, 240));

			 ExtTextOut(stPaint.hdc, ptText.x, ptText.y, ETO_OPAQUE | ETO_CLIPPED, &rcZelle, asFeldWert.c_Str(), asFeldWert.Length(), nullptr);
			 MoveToEx(stPaint.hdc, rcZelle.right, rcZelle.top, nullptr);
       LineTo(stPaint.hdc, rcZelle.right, rcZelle.bottom);

       rcZelle.left = rcZelle.right + 1;
		 }
		 while(ucSpalte++ < siSpalte.nMax && rcZelle.right < stPaint.rcPaint.right);

		 ucSpalte = ucSpalteTemp; rcZelle.left = lLeftTemp; rcZelle.top += ucZeilenhohe;
		 if(++ulZeile >= (UINT)siZeile.nMax && stPaint.rcPaint.bottom > rcZelle.bottom){
			 rcZelle.top = stPaint.rcPaint.top;
       stPaint.rcPaint.top = rcZelle.bottom;
       WM_Paint_Hintergrund(stPaint);
       stPaint.rcPaint.top = rcZelle.top;
			 break;
		 }
	 }
	 while(rcZelle.top < stPaint.rcPaint.bottom);

	 if(rcZelle.right < stPaint.rcPaint.right){ stPaint.rcPaint.left = rcZelle.right; WM_Paint_Hintergrund(stPaint); }

   DeleteObject(hFont); DeleteObject(hPen);
 }
 EndPaint(hWndElement, &stPaint);
 ThreadSicher_Ende(); 
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::WM_Paint_Komma4(COStringA* vasFeldWert, unsigned long& ulZeile, unsigned char& ucSpalte)
{
 COComma4 k4FeldWert; char c20Zahl[20];
 k4FeldWert.Write(vpTabelle[ulZeile][ucSpalte].vbWert);
 *vasFeldWert = Comma4toCHAR(c20Zahl, &k4FeldWert, KommaStellen(ucSpalte));
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::WM_Paint_Komma4_80(COStringA* vasFeldWert, unsigned long& ulZeile, unsigned char& ucSpalte)
{
 COComma4_80 k4gFeldWert; char c32Zahl[32];
 k4gFeldWert.Write(vpTabelle[ulZeile][ucSpalte].vbWert);
 *vasFeldWert = Comma4_80toCHAR(c32Zahl, &k4gFeldWert, KommaStellen(ucSpalte));
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::WM_Paint_Zeit(COStringA* vasFeldWert, unsigned long& ulZeile, unsigned char& ucSpalte)
{
 COTime zFeldWert; char c20DatumZeit[20];
 zFeldWert.Write(vpTabelle[ulZeile][ucSpalte].vbWert);
 if(!zFeldWert.FileTime().dwHighDateTime && !zFeldWert.FileTime().dwLowDateTime) *vasFeldWert = NULL;
 else if(vstSpalten[ucSpalte].usFormat & TXF_ZEIT) *vasFeldWert = zFeldWert.CHARTime(c20DatumZeit);
 else if(vstSpalten[ucSpalte].usFormat & TXF_DATUM) *vasFeldWert = zFeldWert.CHARDate(c20DatumZeit);
 else *vasFeldWert = zFeldWert.CHARDateTime(c20DatumZeit);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::WM_Paint_Datum(COStringA* vasFeldWert, unsigned long& ulZeile, unsigned char& ucSpalte)
{
 COTime zFeldWert;
 zFeldWert.Write(vpTabelle[ulZeile][ucSpalte].vbWert);
 if(!zFeldWert.FileTime().dwHighDateTime && !zFeldWert.FileTime().dwLowDateTime) *vasFeldWert = NULL;
 else{ COStringA asFormat_Datum, asFormat_Zeit; char c20DatumZeit[20]; USHORT usFormat;
   usFormat = vstSpalten[ucSpalte].usFormat; usFormat >>= 3; usFormat <<= 3;
	 switch(usFormat){
		 case TXF_ZEIT            : *vasFeldWert = zFeldWert.CHARTime(c20DatumZeit);
			                          break;
		 case TXF_DATUM           : *vasFeldWert = zFeldWert.CHARDate(c20DatumZeit);
			                          break;
		 case TXF_DATUMZEIT       : *vasFeldWert = zFeldWert.CHARDateTime(c20DatumZeit);
			                          break;
		 case TXF_DATUMEXTERN     : if(pfnDatumZeitFormat){ pfnDatumZeitFormat(ucSpalte, &asFormat_Datum, nullptr);
			                            zFeldWert.StrDateFormat(vasFeldWert, asFormat_Datum.c_Str()); break; }
		 case TXF_DATUMINTERN     : zFeldWert.StrDateFormat(vasFeldWert, daEingabe->Datumformat(&asFormat_Datum)->c_Str());
			                          break;
		 case TXF_DATUMZEITEXTERN : if(pfnDatumZeitFormat){ pfnDatumZeitFormat(ucSpalte, &asFormat_Datum, &asFormat_Zeit);
																  zFeldWert.StrDateTimeFormat(vasFeldWert, asFormat_Datum.c_Str(), asFormat_Zeit.c_Str(), true); break; }
		 case TXF_DATUMZEITINTERN : zFeldWert.StrDateTimeFormat(vasFeldWert, daEingabe->Datumformat(&asFormat_Datum)->c_Str(),
																	daEingabe->Zeitformat(&asFormat_Zeit)->c_Str(), true);
			                          break;
		 case TXF_ZEITDATUMEXTERN : if(pfnDatumZeitFormat){ pfnDatumZeitFormat(ucSpalte, &asFormat_Datum, &asFormat_Zeit);
																  zFeldWert.StrDateTimeFormat(vasFeldWert, asFormat_Datum.c_Str(), asFormat_Zeit.c_Str(), false); break; }
		 case TXF_ZEITDATUMINTERN : zFeldWert.StrDateTimeFormat(vasFeldWert, asFormat_Datum.c_Str(), asFormat_Zeit.c_Str(), false);
																break;
   }
 }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::WM_Create(HWND hWnd)
{
 BYTE ucSpalte; long lPos_x = 0;
 SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO); stScrollInfo.fMask = SIF_ALL;
 lRand_rechts = lBreite; lRand_unten = lHohe;
 lZeichnen_unten = 0;

 for(ucSpalte = 0; ucSpalte < ucSpaltenanzahl; ucSpalte++){
	 lPos_x += vstSpalten[ucSpalte].lBreite + 1;
	 if(lPos_x <= lBreite) stScrollInfo.nPage = ucSpalte;
	 else break;
 }
 if(lPos_x < lBreite) stScrollInfo.nPage++;
 stScrollInfo.nMax = ucSpaltenanzahl - 1;
 stScrollInfo.nTrackPos = stScrollInfo.nMin = stScrollInfo.nPos = 0;
 SetScrollInfo(hWnd, SB_HORZ, &stScrollInfo, false);

 stScrollInfo.nPage = stScrollInfo.nMax = 0;
 SetScrollInfo(hWnd, SB_VERT, &stScrollInfo, false);

 sgKopf->ErstellFenster(hWnd, ucKopfhohe, lBreite, 0, 0); lPos_x = 0;
 for(ucSpalte = 0; ucSpalte < ucSpaltenanzahl; ucSpalte++){
	 sgKopf->ErstellSchalterInGruppe(ucSpalte, ucKopfhohe, vstSpalten[ucSpalte].lBreite, lPos_x, 0);
	 sgKopf->Schalter_SetzAktiv(ucSpalte, false);
	 lPos_x += vstSpalten[ucSpalte].lBreite + 1;
 }
 sgKopf->NeueFensterGrosse(ucKopfhohe, lPos_x);

 SCROLLBARINFO stScrollBarInfo; stScrollBarInfo.cbSize = sizeof(SCROLLBARINFO);
 GetScrollBarInfo(hWnd, OBJID_HSCROLL, &stScrollBarInfo);
 if(stScrollBarInfo.rgstate[0] != STATE_SYSTEM_INVISIBLE) lRand_unten -= stScrollBarInfo.dxyLineButton;

 ezEingabe->ErstellFenster(hWnd, ucKopfhohe, vstSpalten[0].lBreite, 0, ucKopfhohe);

 if(stGrosse_Textbox.lHohe > lRand_unten - ucKopfhohe){
   if(stGrosse_Textbox.lBreite > lRand_rechts) ebEingabe->ErstellFenster(hWnd, lRand_unten - ucKopfhohe, lRand_rechts, 0, 0);
	 else ebEingabe->ErstellFenster(hWnd, lRand_unten - ucKopfhohe, stGrosse_Textbox.lBreite, 0, 0);
 }
 else if(stGrosse_Textbox.lBreite > lRand_rechts) ebEingabe->ErstellFenster(hWnd, stGrosse_Textbox.lHohe, lRand_rechts, 0, 0);
 else ebEingabe->ErstellFenster(hWnd, stGrosse_Textbox.lHohe, stGrosse_Textbox.lBreite, 0, 0);

 if(stGrosse_Listbox.lHohe > lRand_unten - ucKopfhohe){
   if(stGrosse_Listbox.lBreite > lRand_rechts) lbEingabe->ErstellFenster(hWnd, lRand_unten - ucKopfhohe, lRand_rechts, 0, 0);
	 else lbEingabe->ErstellFenster(hWnd, lRand_unten - ucKopfhohe, stGrosse_Listbox.lBreite, 0, 0);
 }
 else if(stGrosse_Listbox.lBreite > lRand_rechts) lbEingabe->ErstellFenster(hWnd, stGrosse_Listbox.lHohe, lRand_rechts, 0, 0);
 else lbEingabe->ErstellFenster(hWnd, stGrosse_Listbox.lHohe, stGrosse_Listbox.lBreite, 0, 0);

 if(stGrosse_Bild.lHohe > lRand_unten - ucKopfhohe){
   if(stGrosse_Bild.lBreite > lRand_rechts) biEingabe->ErstellFenster(hWnd, lRand_unten - ucKopfhohe, lRand_rechts, 0, 0);
	 else biEingabe->ErstellFenster(hWnd, lRand_unten - ucKopfhohe, stGrosse_Bild.lBreite, 0, 0);
 }
 else if(stGrosse_Bild.lBreite > lRand_rechts) biEingabe->ErstellFenster(hWnd, stGrosse_Bild.lHohe, lRand_rechts, 0, 0);
 else biEingabe->ErstellFenster(hWnd, stGrosse_Bild.lHohe, stGrosse_Bild.lBreite, 0, 0);

 doEingabe->ErstellFenster(hWnd, 25, 120, 0, 0);

 if(stGrosse_Audio.lHohe > lRand_unten - ucKopfhohe){
   if(stGrosse_Audio.lBreite > lRand_rechts) maEingabe->ErstellFenster(hWnd, lRand_unten - ucKopfhohe, lRand_rechts, 0, 0);
	 else maEingabe->ErstellFenster(hWnd, lRand_unten - ucKopfhohe, stGrosse_Audio.lBreite, 0, 0);
 }
 else if(stGrosse_Audio.lBreite > lRand_rechts) maEingabe->ErstellFenster(hWnd, stGrosse_Audio.lHohe, lRand_rechts, 0, 0);
 else maEingabe->ErstellFenster(hWnd, stGrosse_Audio.lHohe, stGrosse_Audio.lBreite, 0, 0);

 if(stGrosse_Video.lHohe > lRand_unten - ucKopfhohe){
   if(stGrosse_Video.lBreite > lRand_rechts) mvEingabe->ErstellFenster(hWnd, lRand_unten - ucKopfhohe, lRand_rechts, 0, 0);
	 else mvEingabe->ErstellFenster(hWnd, lRand_unten - ucKopfhohe, stGrosse_Video.lBreite, 0, 0);
 }
 else if(stGrosse_Video.lBreite > lRand_rechts) mvEingabe->ErstellFenster(hWnd, stGrosse_Video.lHohe, lRand_rechts, 0, 0);
 else mvEingabe->ErstellFenster(hWnd, stGrosse_Video.lHohe, stGrosse_Video.lBreite, 0, 0);

 if(stGrosse_Datum.lHohe > lRand_unten - ucKopfhohe){
   if(stGrosse_Datum.lBreite > lRand_rechts) daEingabe->ErstellFenster(hWnd, lRand_unten - ucKopfhohe, lRand_rechts, 0, 0);
	 else daEingabe->ErstellFenster(hWnd, lRand_unten - ucKopfhohe, stGrosse_Datum.lBreite, 0, 0);
 }
 else if(stGrosse_Datum.lBreite > lRand_rechts) daEingabe->ErstellFenster(hWnd, stGrosse_Datum.lHohe, lRand_rechts, 0, 0);
 else daEingabe->ErstellFenster(hWnd, stGrosse_Datum.lHohe, stGrosse_Datum.lBreite, 0, 0);

 ezEingabe->SetzSichtbar(false); ebEingabe->SetzSichtbar(false); EnableWindow(ebEingabe->HWND_Element(), false);
 lbEingabe->SetzSichtbar(false); EnableWindow(lbEingabe->HWND_Element(), false);
 biEingabe->SetzSichtbar(false); EnableWindow(biEingabe->HWND_Element(), false); doEingabe->SetzSichtbar(false); EnableWindow(doEingabe->HWND_Element(), false);
 maEingabe->SetzSichtbar(false); EnableWindow(maEingabe->HWND_Element(), false); mvEingabe->SetzSichtbar(false); EnableWindow(mvEingabe->HWND_Element(), false);
 daEingabe->SetzSichtbar(false); daEingabe->SetzVerfugbar(false);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::WM_Size(LPARAM lParam)
{
 ThreadSicher_Anfang();
 WM_Size_Element(hWndElement, lParam);
 SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO);
 if(lRand_unten != HIWORD(lParam)){ lRand_unten = HIWORD(lParam); SetzScrollVert(stScrollInfo);
	 (stGrosse_Textbox.lHohe > lRand_unten - ucKopfhohe ? ebEingabe->NeueFensterHohe(lRand_unten - ucKopfhohe) : ebEingabe->NeueFensterHohe(stGrosse_Textbox.lHohe));
 	 (stGrosse_Listbox.lHohe > lRand_unten - ucKopfhohe ? lbEingabe->NeueFensterHohe(lRand_unten - ucKopfhohe) : lbEingabe->NeueFensterHohe(stGrosse_Listbox.lHohe));
 	 (stGrosse_Bild.lHohe > lRand_unten - ucKopfhohe ? biEingabe->NeueFensterHohe(lRand_unten - ucKopfhohe) : biEingabe->NeueFensterHohe(stGrosse_Bild.lHohe));
	 (stGrosse_Audio.lHohe > lRand_unten - ucKopfhohe ? maEingabe->NeueFensterHohe(lRand_unten - ucKopfhohe) : maEingabe->NeueFensterHohe(stGrosse_Audio.lHohe));
	 (stGrosse_Video.lHohe > lRand_unten - ucKopfhohe ? mvEingabe->NeueFensterHohe(lRand_unten - ucKopfhohe) : mvEingabe->NeueFensterHohe(stGrosse_Video.lHohe));
	 (stGrosse_Datum.lHohe > lRand_unten - ucKopfhohe ? daEingabe->NeueFensterHohe(lRand_unten - ucKopfhohe) : daEingabe->NeueFensterHohe(stGrosse_Datum.lHohe));
 }
 if(lRand_rechts != LOWORD(lParam)){ lRand_rechts = LOWORD(lParam); SetzScrollHorz(stScrollInfo);
	 (stGrosse_Textbox.lBreite > lRand_rechts ? ebEingabe->NeueFensterBreite(lRand_rechts) : ebEingabe->NeueFensterBreite(stGrosse_Textbox.lBreite));
 	 (stGrosse_Listbox.lBreite > lRand_rechts ? lbEingabe->NeueFensterBreite(lRand_rechts) : lbEingabe->NeueFensterBreite(stGrosse_Listbox.lBreite));
 	 (stGrosse_Bild.lBreite > lRand_rechts ? biEingabe->NeueFensterBreite(lRand_rechts) : biEingabe->NeueFensterBreite(stGrosse_Bild.lBreite));
	 (stGrosse_Audio.lBreite > lRand_rechts ? maEingabe->NeueFensterBreite(lRand_rechts) : maEingabe->NeueFensterBreite(stGrosse_Audio.lBreite));
 	 (stGrosse_Video.lBreite > lRand_rechts ? mvEingabe->NeueFensterBreite(lRand_rechts) : mvEingabe->NeueFensterBreite(stGrosse_Video.lBreite));
	 (stGrosse_Datum.lBreite > lRand_rechts ? daEingabe->NeueFensterBreite(lRand_rechts) : daEingabe->NeueFensterBreite(stGrosse_Datum.lBreite));
 }
 ThreadSicher_Ende(); 
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::WM_KeyDown(WPARAM wParam)
{
 SCROLLINFO stScrollInfo; long lEingabe_links; RECT rcEingabe;
 switch(wParam){
     case VK_LEFT   : ThreadSicher_Anfang();
			                stScrollInfo.cbSize = sizeof(SCROLLINFO); stScrollInfo.fMask = SIF_POS; GetScrollInfo(hWndElement, SB_HORZ, &stScrollInfo);
			                if(ucSpalteSelect > stScrollInfo.nPos)
												      SelectZelle(ulZeileSelect, ucSpalteSelect - 1);
                      else if(stScrollInfo.nPos){
												      SendMessage(hWndElement, WM_HSCROLL, SB_LINELEFT, NULL);
											        SelectZelle(ulZeileSelect, ucSpalteSelect - 1); }
											ThreadSicher_Ende(); 
                      break;
     case VK_RETURN :
		 case VK_RIGHT  : ThreadSicher_Anfang();
			                stScrollInfo.cbSize = sizeof(SCROLLINFO); stScrollInfo.fMask = SIF_ALL;	GetScrollInfo(hWndElement, SB_HORZ, &stScrollInfo);
											if(ucSpalteSelect + 1 <= stScrollInfo.nMax){
												if(sgKopf->Schalter_Pos_X(ucSpalteSelect + 1, lEingabe_links) + vstSpalten[ucSpalteSelect + 1].lBreite < lRand_rechts) SelectZelle(ulZeileSelect, ucSpalteSelect + 1);
												else{
													do{ SendMessage(hWndElement, WM_HSCROLL, SB_LINERIGHT, NULL); GetScrollInfo(hWndElement, SB_HORZ, &stScrollInfo); }
													while(ucSpalteSelect + 1 >= stScrollInfo.nPos + (int)stScrollInfo.nPage);
													SelectZelle(ulZeileSelect, ucSpalteSelect + 1);
												}
											}
											ThreadSicher_Ende(); 
                      break;
     case VK_UP     : ThreadSicher_Anfang();
			                stScrollInfo.cbSize = sizeof(SCROLLINFO); stScrollInfo.fMask = SIF_POS; GetScrollInfo(hWndElement, SB_VERT, &stScrollInfo);
			                if(ulZeileSelect > (UINT)stScrollInfo.nPos)
												      SelectZelle(ulZeileSelect - 1, ucSpalteSelect);
                      else if(stScrollInfo.nPos){
												      SendMessage(hWndElement, WM_VSCROLL, SB_LINEUP, NULL);
												      SelectZelle(ulZeileSelect - 1, ucSpalteSelect); }
											ThreadSicher_Ende(); 
                      break;
     case VK_DOWN   : ThreadSicher_Anfang();
			                stScrollInfo.cbSize = sizeof(SCROLLINFO); stScrollInfo.fMask = SIF_ALL; GetScrollInfo(hWndElement, SB_VERT, &stScrollInfo);
			                if(ulZeileSelect + 1 < (UINT)stScrollInfo.nMax){
												if(ulZeileSelect + 1 < (UINT)stScrollInfo.nPos + stScrollInfo.nPage - 1)
															SelectZelle(ulZeileSelect + 1, ucSpalteSelect);
												else{	SendMessage(hWndElement, WM_VSCROLL, SB_LINEDOWN, NULL);
															SelectZelle(ulZeileSelect + 1, ucSpalteSelect); }
											}
											ThreadSicher_Ende(); 
                      break;
     case VK_HOME   : 
     case VK_END    : break; 
     case VK_PRIOR  : SendMessage(hWndElement, WM_VSCROLL, SB_PAGEUP, NULL); break; 
     case VK_NEXT   : SendMessage(hWndElement, WM_VSCROLL, SB_PAGEDOWN, NULL); break;
		 case VK_ESCAPE : ThreadSicher_Anfang();
			                if(bEinfugen){ EinfugenRollback(); LoschEinfugen();}
											else if(bFeldanderung){ AndernRollback(); LoschAndern(); }
											else if(bAnsehen) bAnsehen = false; 
			                bSelectiert = false; bAktiv = false;
											if(ezEingabe->Sichtbar()){ ezEingabe->Fenster(rcEingabe); ezEingabe->SetzSichtbar(false); ezEingabe->SetzVerfugbar(false); } 
											else if(ebEingabe->Sichtbar()){ ebEingabe->Fenster(rcEingabe); ebEingabe->SetzSichtbar(false); ebEingabe->SetzVerfugbar(false); ebEingabe->Zeichenvorgabe(ZV_ALLE); }
											else if(lbEingabe->Sichtbar()){ lbEingabe->Fenster(rcEingabe); lbEingabe->SetzSichtbar(false); lbEingabe->SetzVerfugbar(false); }
											else if(biEingabe->Sichtbar()){ biEingabe->Fenster(rcEingabe); biEingabe->SetzSichtbar(false); biEingabe->SetzVerfugbar(false); }
											else if(doEingabe->Sichtbar()){ doEingabe->Fenster(rcEingabe); doEingabe->SetzSichtbar(false); doEingabe->SetzVerfugbar(false); }
											else if(maEingabe->Sichtbar()){ maEingabe->Fenster(rcEingabe); maEingabe->SetzSichtbar(false); maEingabe->SetzVerfugbar(false); }
											else if(mvEingabe->Sichtbar()){ mvEingabe->Fenster(rcEingabe); mvEingabe->SetzSichtbar(false); mvEingabe->SetzVerfugbar(false); }
											else if(daEingabe->Sichtbar()){ daEingabe->Fenster(rcEingabe); daEingabe->SetzSichtbar(false); daEingabe->SetzVerfugbar(false); }
											UpdateFenster(&rcEingabe, true, false);
											ThreadSicher_Ende(); 
                      break;
		 case VK_SPACE  : ThreadSicher_Anfang();
			                if(bEinfugen || bFeldanderung && FeldTyp(vucFeldStatus[ucSpalteSelect]) == FT_BOOL){ 
		                    (*(bool*)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert ? ezEingabe->Text(vbBool_false) : ezEingabe->Text(vbBool_true));
		                    AndernWert(true);
											}
											ThreadSicher_Ende();
											break;
 }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COTabBasis::WM_Command(WPARAM wParam)
{
 ThreadSicher_Anfang();
 SCROLLINFO stScrollVert; stScrollVert.cbSize = sizeof(SCROLLINFO); stScrollVert.fMask = SIF_ALL; RECT rcZeichnen;
 GetScrollInfo(hWndElement, SB_VERT, &stScrollVert);

 short sIDElement = LOWORD(wParam); rcZeichnen.top = GetWindowLongPtr(sgKopf->HWND_Element(), GWLP_ID);
 if(sIDElement > rcZeichnen.top && sIDElement < rcZeichnen.top + ucSpaltenanzahl + 1){
	 sIDElement -= rcZeichnen.top + 1;

	 if(!vucFeldStatus){ MessageBox(hWndElement, pcKeineAktion, pcNix, MB_ICONINFORMATION);
		 sgKopf->Schalter_SetzSchalter(sIDElement, false); sgKopf->Schalter_SetzAktiv(sIDElement, false);
		 ThreadSicher_Ende(); return false;
	 }

	 if(bEinfugen){ EinfugenRollback(); LoschEinfugen(); }
	 else if(bFeldanderung){ AndernRollback(); LoschAndern(); }
   bSelectiert = false; bAktiv = false;

	 if(ezEingabe->Sichtbar()){ ezEingabe->SetzSichtbar(false); ezEingabe->SetzVerfugbar(false); }
	 else if(ebEingabe->Sichtbar()){ ebEingabe->SetzSichtbar(false); ebEingabe->SetzVerfugbar(false); bAnsehen = false; ebEingabe->Zeichenvorgabe(ZV_ALLE); }
	 else if(lbEingabe->Sichtbar()){ lbEingabe->SetzSichtbar(false); lbEingabe->SetzVerfugbar(false); bAnsehen = false; }
	 else if(biEingabe->Sichtbar()){ biEingabe->SetzSichtbar(false); biEingabe->SetzVerfugbar(false); bAnsehen = false; }
	 else if(doEingabe->Sichtbar()){ doEingabe->SetzSichtbar(false); doEingabe->SetzVerfugbar(false); bAnsehen = false; }
	 else if(maEingabe->Sichtbar()){ maEingabe->SetzSichtbar(false); maEingabe->SetzVerfugbar(false); bAnsehen = false; }
	 else if(mvEingabe->Sichtbar()){ mvEingabe->SetzSichtbar(false); mvEingabe->SetzVerfugbar(false); bAnsehen = false; }
	 else if(daEingabe->Sichtbar()){ daEingabe->SetzSichtbar(false); daEingabe->SetzVerfugbar(false); bAnsehen = false; }
 
	 if(vucFeldStatus[sIDElement] & FT_SORTDOWN){
     vucFeldStatus[sIDElement] ^= FT_SORTDOWN | FT_SORTUP; 
     QuickSort(sIDElement, vucFeldStatus[sIDElement], 0, ulDSAnzahl - 1);
	 }
	 else if(vucFeldStatus[sIDElement] & FT_SORTUP){
		 vucFeldStatus[sIDElement] ^= FT_SORTDOWN | FT_SORTUP;
     QuickSort(sIDElement, vucFeldStatus[sIDElement], 0, ulDSAnzahl - 1);
	 }
	 else{
		 vucFeldStatus[sIDElement] |= FT_SORTUP;
     QuickSort(sIDElement, vucFeldStatus[sIDElement], 0, ulDSAnzahl - 1);
	 }

	 rcZeichnen.left = 0; rcZeichnen.top = ucKopfhohe;
	 if((int)stScrollVert.nPage + stScrollVert.nPos > stScrollVert.nMax) rcZeichnen.bottom = (stScrollVert.nPage - 1) * ucZeilenhohe + ucKopfhohe;
	 else rcZeichnen.bottom = lRand_unten;
   if(sgKopf->Breite(rcZeichnen.right) > lRand_rechts) rcZeichnen.right = lRand_rechts; 
	 UpdateFenster(&rcZeichnen, true, false);
	 ThreadSicher_Ende(); return false;
 }

 if(!vucFeldStatus){ MessageBox(hWndElement, pcKeineAktion, pcNix, MB_ICONINFORMATION); ThreadSicher_Ende(); return false; }

 if(sIDElement == _Kalender){
	 sgKopf->Schalter_Pos_X(ucSpalteSelect, rcZeichnen.left);
	 if(lRand_rechts - rcZeichnen.left < daEingabe->Breite(rcZeichnen.right)) rcZeichnen.left = rcZeichnen.left - rcZeichnen.right + vstSpalten[ucSpalteSelect].lBreite;
	 rcZeichnen.top = ucZeilenhohe * (ulZeileSelect - stScrollVert.nPos) + ucKopfhohe;
	 if(lRand_unten < rcZeichnen.top + daEingabe->Hohe(rcZeichnen.bottom)) rcZeichnen.top = lRand_unten - rcZeichnen.bottom;
   daEingabe->NeueFensterPosition(rcZeichnen.left, rcZeichnen.top);
	 ThreadSicher_Ende(); return false;
 }
 
 switch(sIDElement){
		case IDM_DS_LOSCHEN        : if(bAktiv && bSelectiert && ucRechte & ZR_LOSCHEN){
																	 if(pfnDSSenden && !pfnDSSenden(this)){ ThreadSicher_Ende(); return false; }
															     DSLoschen();
																	 SetzScrollVert(stScrollVert);
																	 if(stScrollVert.nPage == 1 && stScrollVert.nPos) SendMessage(hWndElement, WM_VSCROLL, SB_LINEUP, NULL);
															     rcZeichnen.left = 0;
																	 if(sgKopf->Breite(rcZeichnen.right) > lRand_rechts) rcZeichnen.right = lRand_rechts; 
															     rcZeichnen.top = (ulZeileSelect - stScrollVert.nPos) * ucZeilenhohe + ucKopfhohe;										
																	 rcZeichnen.bottom = lZeichnen_unten + ucZeilenhohe;
																	 if(ulDSAnzahl && ulZeileSelect == ulDSAnzahl){ rcZeichnen.top -= ucZeilenhohe; ulZeileSelect--; }
																	 UpdateFenster(&rcZeichnen, true, false);

																	 bSelectiert = false; bAktiv = false; ezEingabe->SetzSichtbar(false); ezEingabe->SetzVerfugbar(false);
														     }
														     ThreadSicher_Ende(); return false;
		case IDM_DS_ANDERN         : if(bAktiv && bSelectiert && ucRechte & ZR_ANDERN){
																	 if(pfnServerVerbunden && !pfnServerVerbunden(this)){ ThreadSicher_Ende(); return false; }
																	 if(bEinfugen || bFeldanderung){ WM_Command(IDM_DS_SENDEN); bAktiv = true; bSelectiert = true; }
																	 bFeldanderung = true; bAnsehen = false;
																	 switch(FeldTyp(vucFeldStatus[ucSpalteSelect])){
																		 case FT_LONGSTR    :
																		 case FT_MEMOSTR    : if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
																													else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
																													else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
																													else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
																													else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
																													else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
																													else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																													Eingabe_Position(ebEingabe, stScrollVert);
																													FeldWert();
																													ebEingabe->Hintergrundfarbe(255, 255, 255); ebEingabe->Schriftfarbe(0, 0, 128);
																													ezEingabe->SetzVerfugbar(false); ebEingabe->SetzVerfugbar(true); SetFocus(ebEingabe->HWND_Element());
																													break;
																		 case FT_LISTSTR    : if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
																													else if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
																													else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
																													else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
																													else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
																													else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
																													else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																													Eingabe_Position(lbEingabe, stScrollVert);
																													FeldWert();
																													lbEingabe->Hintergrundfarbe(255, 255, 255); lbEingabe->Schriftfarbe(0, 0, 128);
																													ezEingabe->SetzVerfugbar(false); lbEingabe->SetzVerfugbar(true); SetFocus(lbEingabe->HWND_Element());
																													break;
																		 case FT_BIGIMAGE   :
																		 case FT_SMALLIMAGE : if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
																													else if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
																													else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
																													else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
																													else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
																													else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
																													else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																													Eingabe_Position(biEingabe, stScrollVert);
																													biEingabe->Berechtigungen(ZR_OFFNEN | ZR_SPEICHERN | ZR_LOSCHEN);
																													FeldWert();
																													biEingabe->Infotext("Ein Bild einfügen");
																													biEingabe->Hintergrundfarbe(255, 255, 255); biEingabe->Schriftfarbe(0, 0, 128);
																													ezEingabe->SetzVerfugbar(false); biEingabe->SetzVerfugbar(true); SetFocus(biEingabe->HWND_Element());
																													break;
																		 case FT_DOCUMENT   : if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
																													else if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
																													else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
																													else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
																													else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
																													else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
																													else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																													Eingabe_Position(doEingabe, stScrollVert);
																													FeldWert();
																													ezEingabe->SetzVerfugbar(false); doEingabe->SetzVerfugbar(true); SetFocus(doEingabe->HWND_Element());
																													break;
																		 case FT_AUDIO      : if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
																													else if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
																													else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
																													else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
																													else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
																													else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
																													else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																													Eingabe_Position(maEingabe, stScrollVert);
																													maEingabe->Berechtigungen(ZR_OFFNEN | ZR_SPEICHERN | ZR_LOSCHEN);
																													FeldWert();
																													maEingabe->Infotext("Ein Audio einfügen");
																													maEingabe->Hintergrundfarbe(255, 255, 255); maEingabe->Schriftfarbe(0, 0, 128);
																													ezEingabe->SetzVerfugbar(false); maEingabe->SetzVerfugbar(true); SetFocus(maEingabe->HWND_Element());
																													break;
																		 case FT_VIDEO      : if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
																													else if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
																													else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
																													else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
																													else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
																													else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
																													else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																													Eingabe_Position(mvEingabe, stScrollVert);
																													mvEingabe->Berechtigungen(ZR_OFFNEN | ZR_SPEICHERN | ZR_LOSCHEN);
																													FeldWert();
																													mvEingabe->Infotext("Ein Video einfügen");
																													mvEingabe->Hintergrundfarbe(255, 255, 255); mvEingabe->Schriftfarbe(0, 0, 128);
																													ezEingabe->SetzVerfugbar(false); mvEingabe->SetzVerfugbar(true); SetFocus(mvEingabe->HWND_Element());
																													break;
																		 case FT_DATE       : if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
																													else if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
																													else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
																													else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
																													else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
																													else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
																													else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
																													Eingabe_Position(daEingabe, stScrollVert);
																													FeldWert();
																													daEingabe->Hintergrundfarbe(255, 255, 255); daEingabe->Schriftfarbe(0, 0, 128);
																													ezEingabe->SetzVerfugbar(false); daEingabe->SetzVerfugbar(true);
																													SetFocus(daEingabe->HWND_Element());
																													break;
																		 case FT_BOOL       : if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
																													else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
																													else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
																													else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
																													else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
																													else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
																													else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																													ezEingabe->Hintergrundfarbe(255, 255, 255); ezEingabe->Schriftfarbe(0, 0, 128);
																													FeldWert();
																													break;
																							default   : if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
																													else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
																													else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
																													else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
																													else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
																													else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
																													else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																													ezEingabe->Hintergrundfarbe(255, 255, 255); ezEingabe->Schriftfarbe(0, 0, 128);
																													FeldWert();
																													ezEingabe->SetzVerfugbar(true); SetFocus(ezEingabe->HWND_Element());
																	 }
														     }
														     ThreadSicher_Ende(); return false;
		case IDM_DS_SENDEN         : if(bAktiv && bSelectiert && ucRechte & ZR_SENDEN && (bEinfugen || bFeldanderung)){
															     if(AndernWert(true)){
																		 if(pfnDSSenden && !pfnDSSenden(this)){
																			 if(bEinfugen) EinfugenRollback();
																			 else if(bFeldanderung) AndernRollback();
																		 }
																     if(bEinfugen) LoschEinfugen();
																     else if(bFeldanderung) LoschAndern();
							                       bSelectiert = false; bAktiv = false;
																		 switch(FeldTyp(vucFeldStatus[ucSpalteSelect])){
																			 case FT_LONGSTR    :
																			 case FT_MEMOSTR    : ebEingabe->SetzSichtbar(false); ebEingabe->SetzVerfugbar(false);
																														UpdateFenster(&ebEingabe->Fenster(rcZeichnen), true, false);
																														break;
																			 case FT_LISTSTR    : if(lbEingabe->Sichtbar()){ lbEingabe->SetzSichtbar(false); lbEingabe->SetzVerfugbar(false);
																															InvalidateRect(hWndElement, &ezEingabe->Fenster(rcZeichnen), false); 
																														  UpdateFenster(&lbEingabe->Fenster(rcZeichnen), true, false);
																														}
																														else ezEingabe->SetzSichtbar(false);
																														break;
																			 case FT_BIGIMAGE   :
																			 case FT_SMALLIMAGE : biEingabe->SetzSichtbar(false); biEingabe->SetzVerfugbar(false);
																														UpdateFenster(&biEingabe->Fenster(rcZeichnen), true, false);
																														break;
 																			 case FT_DOCUMENT   : doEingabe->SetzSichtbar(false); doEingabe->SetzVerfugbar(false);
																														UpdateFenster(&doEingabe->Fenster(rcZeichnen), true, false);
																														break;
 																			 case FT_AUDIO      : maEingabe->SetzSichtbar(false); maEingabe->SetzVerfugbar(false);
																														UpdateFenster(&maEingabe->Fenster(rcZeichnen), true, false);
																														break;
 																			 case FT_VIDEO      : mvEingabe->SetzSichtbar(false); mvEingabe->SetzVerfugbar(false);
																														UpdateFenster(&mvEingabe->Fenster(rcZeichnen), true, false);
																														break;
 																			 case FT_DATE       : if(daEingabe->Sichtbar()){ daEingabe->SetzSichtbar(false); daEingabe->SetzVerfugbar(false);
																															InvalidateRect(hWndElement, &ezEingabe->Fenster(rcZeichnen), false); 
																															UpdateFenster(&daEingabe->Fenster(rcZeichnen), true, false);
																														}
																														else ezEingabe->SetzSichtbar(false);
																														break;
																								default   : ezEingabe->SetzSichtbar(false); ezEingabe->SetzVerfugbar(false);																							
																		 }
															     }
																	 else SetFocus(ezEingabe->HWND_Element());
                                 }
																 ThreadSicher_Ende(); return false;
		case IDM_DS_EINFUGEN       : if(ucRechte & ZR_EINFUGEN){ 
			                             if(pfnServerVerbunden && !pfnServerVerbunden(this)){ ThreadSicher_Ende(); return false; }
																	 if(bFeldanderung || bEinfugen){ WM_Command(IDM_DS_SENDEN); bAktiv = true; bSelectiert = true; }
																	 bEinfugen = true; bAnsehen = false;														 
			                             DSEinfugen();
																 }
														     ThreadSicher_Ende(); return false;
		case IDM_DF_ANSEHEN        : if(bAktiv && bSelectiert){ bAnsehen = true;
																	 switch(FeldTyp(vucFeldStatus[ucSpalteSelect])){
																		 case FT_LONGSTR    :
																		 case FT_MEMOSTR    : if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
																													else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
																													else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
																													else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
																													else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
																													else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
																													else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																													Eingabe_Position(ebEingabe, stScrollVert);
																													FeldWert();
																													ebEingabe->Hintergrundfarbe(120, 120, 120); ebEingabe->Schriftfarbe(240, 240, 240);
																													ebEingabe->SetzVerfugbar(true); ebEingabe->Zeichenvorgabe(ZV_KEINE); SetFocus(ebEingabe->HWND_Element()); 
																													break;
																		 case FT_LISTSTR    : if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
																													else if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
																													else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
																													else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
																													else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
																													else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
																													else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																													Eingabe_Position(lbEingabe, stScrollVert);
																													FeldWert();
																													lbEingabe->Hintergrundfarbe(120, 120, 120); lbEingabe->Schriftfarbe(240, 240, 240);
																													lbEingabe->SetzVerfugbar(true); SetFocus(lbEingabe->HWND_Element()); 
																													break;
																		 case FT_BIGIMAGE   :
																		 case FT_SMALLIMAGE : if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
																													else if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
																													else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
																													else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
																													else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
																													else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
																													else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																													Eingabe_Position(biEingabe, stScrollVert);
																													FeldWert();
																													biEingabe->Infotext("Kein Bild verfügbar");
																													biEingabe->Hintergrundfarbe(120, 120, 120); biEingabe->Schriftfarbe(240, 240, 240);
																													biEingabe->SetzVerfugbar(true); biEingabe->Berechtigungen(ZR_SPEICHERN); SetFocus(biEingabe->HWND_Element()); 
																													break;
																		 case FT_DOCUMENT   : if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
																													else if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
																													else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
																													else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
																													else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
																													else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
																													else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																													Eingabe_Position(doEingabe, stScrollVert);
																													FeldWert();
																													if(vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange){
																														doEingabe->SetzVerfugbar(true); 
																														SendMessage(doEingabe->HWND_Element(), WM_CHAR, VK_RETURN, NULL);
																														doEingabe->SetzSichtbar(false); doEingabe->SetzVerfugbar(false); 
																													}
																													else SendMessage(hWndElement, WM_KEYDOWN, VK_ESCAPE, NULL);
																													break;
																		 case FT_AUDIO      : if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
																													else if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
																													else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
																													else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
																													else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
																													else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
																													else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																													Eingabe_Position(maEingabe, stScrollVert);
																													maEingabe->Infotext("Kein Audio verfügbar");
																													maEingabe->Hintergrundfarbe(120, 120, 120); maEingabe->Schriftfarbe(240, 240, 240);
																													FeldWert();
																													maEingabe->SetzVerfugbar(true); maEingabe->Berechtigungen(ZR_SPEICHERN);
																													SetFocus(maEingabe->HWND_Element()); 
																													break;
																		 case FT_VIDEO      : if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
																													else if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
																													else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
																													else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
																													else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
																													else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
																													else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																													Eingabe_Position(mvEingabe, stScrollVert);
																													mvEingabe->Infotext("Kein Video verfügbar");
																													mvEingabe->Hintergrundfarbe(120, 120, 120); mvEingabe->Schriftfarbe(240, 240, 240);
																													FeldWert();																												 
																													mvEingabe->SetzVerfugbar(true); mvEingabe->Berechtigungen(ZR_SPEICHERN);
																													SetFocus(mvEingabe->HWND_Element()); 
																													break;
																	 }
														     }
														        ThreadSicher_Ende(); return false;
		case IDM_DATEI_OFFNEN         : if(ucRechte & ZR_OFFNEN && !bAnsehen && !bEinfugen && !bFeldanderung) DateiOffnen();
			                              ThreadSicher_Ende(); return false;
		case IDM_DATEI_SPEICHERN      : if(ucRechte & ZR_SPEICHERN && !bAnsehen && !bEinfugen && !bFeldanderung) DateiSpeichern();
			                              ThreadSicher_Ende(); return false;
		case IDM_DATEI_SPEICHERNUNTER : if(ucRechte & ZR_SPEICHERN && !bAnsehen && !bEinfugen && !bFeldanderung) DateiSpeichernUnter();
			                              ThreadSicher_Ende(); return false;
		case IDM_KOPIEREN             : if(!bAnsehen && ezEingabe->Sichtbar()){ 
			                              ezEingabe->SelectAlles(); SendMessage(ezEingabe->HWND_Element(), WM_COMMAND, IDM_KOPIEREN, NULL); ezEingabe->SelectEntfernen(); }
			                              ThreadSicher_Ende(); return false;
		default                       : ThreadSicher_Ende(); return true;
 }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::WM_ContexMenu(LPARAM lParam)
{
 ThreadSicher_Anfang();
 if(!vucFeldStatus){ MessageBox(hWndElement, pcKeineAktion, pcNix, MB_ICONINFORMATION); ThreadSicher_Ende(); return; }

 EnableMenuItem(hMenu, IDM_AUSSCHNEIDEN, MF_BYCOMMAND | MF_GRAYED);
 EnableMenuItem(hMenu, IDM_EINFUGEN, MF_BYCOMMAND | MF_GRAYED);
 if(!bAnsehen && ulDSAnzahl) EnableMenuItem(hMenu, IDM_KOPIEREN, MF_BYCOMMAND | MF_ENABLED);
 else EnableMenuItem(hMenu, IDM_KOPIEREN, MF_BYCOMMAND | MF_GRAYED);

 if(ucRechte & ZR_OFFNEN && !bAnsehen && !bEinfugen && !bFeldanderung) EnableMenuItem(hMenu, IDM_DATEI_OFFNEN, MF_BYCOMMAND | MF_ENABLED);
 else EnableMenuItem(hMenu, IDM_DATEI_OFFNEN, MF_BYCOMMAND | MF_GRAYED);
 if(ucRechte & ZR_SPEICHERN && !bAnsehen && !bEinfugen && !bFeldanderung){ EnableMenuItem(hMenu, IDM_DATEI_SPEICHERN, MF_BYCOMMAND | MF_ENABLED); EnableMenuItem(hMenu, IDM_DATEI_SPEICHERNUNTER, MF_BYCOMMAND | MF_ENABLED); }
 else{ EnableMenuItem(hMenu, IDM_DATEI_SPEICHERN, MF_BYCOMMAND | MF_GRAYED); EnableMenuItem(hMenu, IDM_DATEI_SPEICHERNUNTER, MF_BYCOMMAND | MF_GRAYED); }

 if(bAktiv && !bAnsehen){
	 if(ucRechte & ZR_LOSCHEN && !bEinfugen && !bFeldanderung) EnableMenuItem(hMenu, IDM_DS_LOSCHEN, MF_BYCOMMAND | MF_ENABLED);
	 else EnableMenuItem(hMenu, IDM_DS_LOSCHEN, MF_BYCOMMAND | MF_GRAYED);
	 if(ucRechte & ZR_ANDERN && !bEinfugen && !bFeldanderung) EnableMenuItem(hMenu, IDM_DS_ANDERN, MF_BYCOMMAND | MF_ENABLED);
	 else EnableMenuItem(hMenu, IDM_DS_ANDERN, MF_BYCOMMAND | MF_GRAYED);
	 if(ucRechte & ZR_SENDEN) EnableMenuItem(hMenu, IDM_DS_SENDEN, MF_BYCOMMAND | MF_ENABLED);
	 else EnableMenuItem(hMenu, IDM_DS_SENDEN, MF_BYCOMMAND | MF_GRAYED);

	 switch(FeldTyp(vucFeldStatus[ucSpalteSelect])){
		 case FT_AUDIO      :
		 case FT_VIDEO      :
		 case FT_LONGSTR    :
		 case FT_MEMOSTR    :
		 case FT_SMALLIMAGE :
		 case FT_BIGIMAGE   : 
		 case FT_LISTSTR    : if(!bEinfugen && !bFeldanderung) EnableMenuItem(hMenu, IDM_DF_ANSEHEN, MF_BYCOMMAND | MF_ENABLED);
			                    else EnableMenuItem(hMenu, IDM_DF_ANSEHEN, MF_BYCOMMAND | MF_GRAYED);
												  break;
		 case FT_DOCUMENT   : if(vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange && !bEinfugen && !bFeldanderung) EnableMenuItem(hMenu, IDM_DF_ANSEHEN, MF_BYCOMMAND | MF_ENABLED);
												  else EnableMenuItem(hMenu, IDM_DF_ANSEHEN, MF_BYCOMMAND | MF_GRAYED);
												  break;
		 default            : EnableMenuItem(hMenu, IDM_DF_ANSEHEN, MF_BYCOMMAND | MF_GRAYED);
	 }
 }
 else{
	 EnableMenuItem(hMenu, IDM_DS_LOSCHEN, MF_BYCOMMAND | MF_GRAYED);
	 EnableMenuItem(hMenu, IDM_DS_EINFUGEN, MF_BYCOMMAND | MF_GRAYED);
	 EnableMenuItem(hMenu, IDM_DS_ANDERN, MF_BYCOMMAND | MF_GRAYED);
	 EnableMenuItem(hMenu, IDM_DS_SENDEN, MF_BYCOMMAND | MF_GRAYED);
	 EnableMenuItem(hMenu, IDM_DF_ANSEHEN, MF_BYCOMMAND | MF_GRAYED);
 }

 if(ucRechte & ZR_EINFUGEN) EnableMenuItem(hMenu, IDM_DS_EINFUGEN, MF_BYCOMMAND | MF_ENABLED);

 POINT ptPosition;
 ptPosition.x = LOWORD(lParam); ptPosition.y = HIWORD(lParam);
 if(ptPosition.x == USHRT_MAX && ptPosition.y == USHRT_MAX) ClientToScreen(GetParent(hWndElement), &Position(ptPosition));
 TrackPopupMenuEx(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, ptPosition.x, ptPosition.y, hWndElement, nullptr);  
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::WM_VScroll(WPARAM wParam)
{
 ThreadSicher_Anfang();
 SCROLLINFO stScrollVert; stScrollVert.cbSize = sizeof(SCROLLINFO); stScrollVert.fMask = SIF_ALL;
 GetScrollInfo(hWndElement, SB_VERT, &stScrollVert);
 int iPos_Alt = stScrollVert.nPos;

 switch(LOWORD(wParam)){
		 case SB_LINEUP     : if(stScrollVert.nPos) stScrollVert.nPos--;
			                    break;
		 case SB_LINEDOWN   : if(stScrollVert.nMax >= (int)stScrollVert.nPage + stScrollVert.nPos) stScrollVert.nPos++;
			                    break;
		 case SB_PAGEUP     : if(int(stScrollVert.nPos - stScrollVert.nPage) >= 0) stScrollVert.nPos -= stScrollVert.nPage - 1; 
													else stScrollVert.nPos = 0;
			                    break;
		 case SB_PAGEDOWN   : if(stScrollVert.nPos + stScrollVert.nPage < int(stScrollVert.nMax - stScrollVert.nPage)) stScrollVert.nPos += stScrollVert.nPage - 1;
													else stScrollVert.nPos = stScrollVert.nMax - stScrollVert.nPage + 1;
			                    break;
     case SB_THUMBTRACK : stScrollVert.nPos = stScrollVert.nTrackPos; break;
		 default            : break;
 }

 if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
 else if(ebEingabe->Sichtbar()){ ebEingabe->SetzSichtbar(false); if(bAnsehen){ bAnsehen = false; ebEingabe->SetzVerfugbar(false); ebEingabe->Zeichenvorgabe(ZV_ALLE); } }
 else if(lbEingabe->Sichtbar()){ lbEingabe->SetzSichtbar(false); if(bAnsehen){ bAnsehen = false; lbEingabe->SetzVerfugbar(false); } }
 else if(biEingabe->Sichtbar()){ biEingabe->SetzSichtbar(false); if(bAnsehen){ bAnsehen = false; biEingabe->SetzVerfugbar(false); } }
 else if(doEingabe->Sichtbar()){ doEingabe->SetzSichtbar(false); if(bAnsehen){ bAnsehen = false; doEingabe->SetzVerfugbar(false); } }
 else if(maEingabe->Sichtbar()){ maEingabe->SetzSichtbar(false); if(bAnsehen){ bAnsehen = false; maEingabe->SetzVerfugbar(false); } }
 else if(mvEingabe->Sichtbar()){ mvEingabe->SetzSichtbar(false); if(bAnsehen){ bAnsehen = false; mvEingabe->SetzVerfugbar(false); } }
 else if(daEingabe->Sichtbar()){ daEingabe->SetzSichtbar(false); if(bAnsehen){ bAnsehen = false; daEingabe->SetzVerfugbar(false); } }

 RECT rcZeichnen; rcZeichnen.left = 0;
 if(sgKopf->Breite(rcZeichnen.right) > lRand_rechts) rcZeichnen.right = lRand_rechts; 
 rcZeichnen.bottom = lZeichnen_unten;

 if(stScrollVert.nPos + 1 == iPos_Alt || stScrollVert.nPos - 1 == iPos_Alt){ 
   SetScrollInfo(hWndElement, SB_VERT, &stScrollVert, true);
   
	 if(stScrollVert.nPos > iPos_Alt){
		 rcZeichnen.top = ucKopfhohe + ucZeilenhohe;
		 ScrollWindow(hWndElement, 0, ucZeilenhohe *-1, &rcZeichnen, nullptr);
 
		 if(lZeichnen_unten > lRand_unten) rcZeichnen.top = rcZeichnen.bottom - ucZeilenhohe * 2;
		 else{ rcZeichnen.top = rcZeichnen.bottom - ucZeilenhohe; rcZeichnen.bottom = lRand_unten; }
	 }
	 else{
		 rcZeichnen.top = ucKopfhohe;
		 ScrollWindow(hWndElement, 0, ucZeilenhohe, &rcZeichnen, nullptr);

		 rcZeichnen.bottom = rcZeichnen.top + ucZeilenhohe;

		 if((int)stScrollVert.nPage <= (lRand_unten - ucKopfhohe) / ucZeilenhohe){
		   if(stScrollVert.nPos){ stScrollVert.nPage++; SetScrollInfo(hWndElement, SB_VERT, &stScrollVert, true);
				 lZeichnen_unten = (stScrollVert.nPage - 1) * ucZeilenhohe + ucKopfhohe;				
			 }
			 else lZeichnen_unten = (stScrollVert.nPage) * ucZeilenhohe + ucKopfhohe;
		 }
	 }
	 UpdateFenster(&rcZeichnen, true, false);
 }
 else if(stScrollVert.nPos != iPos_Alt){
   SetScrollInfo(hWndElement, SB_VERT, &stScrollVert, true);

	 rcZeichnen.top = ucKopfhohe; rcZeichnen.bottom += ucZeilenhohe;
	 if((int)stScrollVert.nPage <= (lRand_unten - ucKopfhohe) / ucZeilenhohe){
		 stScrollVert.nPage = (lRand_unten - ucKopfhohe) / ucZeilenhohe;
		 if((lRand_unten - ucKopfhohe) > (lRand_unten - ucKopfhohe) / ucZeilenhohe * ucZeilenhohe) stScrollVert.nPage++;
		 if(stScrollVert.nPos + (int)stScrollVert.nPage > stScrollVert.nMax) stScrollVert.nPage = stScrollVert.nMax - stScrollVert.nPos + 1;
		 stScrollVert.nPos = stScrollVert.nMax - stScrollVert.nPage + 1;
     SetScrollInfo(hWndElement, SB_VERT, &stScrollVert, true);
		 rcZeichnen.bottom = lZeichnen_unten = (stScrollVert.nPage - 1) * ucZeilenhohe + ucKopfhohe;
	 }
	 UpdateFenster(&rcZeichnen, true, false);
 }

 if(bSelectiert){
	 if(bEinfugen || bFeldanderung) SelectZelle(ulZeileSelect, ucSpalteSelect);
	 else{ long lEingabe = ucZeilenhohe * (ulZeileSelect - stScrollVert.nPos) + ucKopfhohe + ucZeilenhohe;
		 if(lEingabe < lRand_unten && lEingabe >= ucKopfhohe){ bAktiv = true;
		   ezEingabe->NeuesFenster(ucZeilenhohe, vstSpalten[ucSpalteSelect].lBreite, sgKopf->Schalter_Pos_X(ucSpalteSelect, lEingabe),
			                         ucZeilenhohe * (ulZeileSelect - stScrollVert.nPos) + ucKopfhohe); 
		   ezEingabe->SetzSichtbar(true);
		 }
		 else bAktiv = false;
	 }
 }
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::WM_HScroll(WPARAM wParam)
{
 ThreadSicher_Anfang();
 SCROLLINFO stScrollHorz; stScrollHorz.cbSize = sizeof(SCROLLINFO); stScrollHorz.fMask = SIF_ALL;
 GetScrollInfo(hWndElement, SB_HORZ, &stScrollHorz);
 int iPos_Alt = stScrollHorz.nPos;

 switch(LOWORD(wParam)){
	 case SB_LINELEFT      : if(stScrollHorz.nPos) stScrollHorz.nPos--;
													 break;
	 case SB_LINERIGHT     : if(stScrollHorz.nMax >= (int)stScrollHorz.nPage + stScrollHorz.nPos) stScrollHorz.nPos++;
													 break;
	 case SB_PAGELEFT      : stScrollHorz.nPos -= stScrollHorz.nPage;
													 if(stScrollHorz.nPos < 0) stScrollHorz.nPos = 0;
													 break;
	 case SB_PAGERIGHT     : if(stScrollHorz.nPos + stScrollHorz.nPage <= int(stScrollHorz.nMax)) stScrollHorz.nPos = stScrollHorz.nPage + 1;
													 break;
	 case SB_THUMBTRACK    : stScrollHorz.nPos = stScrollHorz.nTrackPos;
													 break;
	 default               : break;
 }

 if(stScrollHorz.nPos != iPos_Alt){ RECT rcZeichnen; rcZeichnen.top = ucKopfhohe;
   if(lZeichnen_unten > lRand_unten) rcZeichnen.bottom = lZeichnen_unten;
	 else rcZeichnen.bottom = lRand_unten;

   if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
	 else if(ebEingabe->Sichtbar()){ ebEingabe->SetzSichtbar(false); if(bAnsehen){ bAnsehen = false; ebEingabe->SetzVerfugbar(false); ebEingabe->Zeichenvorgabe(ZV_ALLE); } }
	 else if(lbEingabe->Sichtbar()){ lbEingabe->SetzSichtbar(false); if(bAnsehen){ bAnsehen = false; lbEingabe->SetzVerfugbar(false); } }
	 else if(biEingabe->Sichtbar()){ biEingabe->SetzSichtbar(false); if(bAnsehen){ bAnsehen = false; biEingabe->SetzVerfugbar(false); } }
	 else if(doEingabe->Sichtbar()){ doEingabe->SetzSichtbar(false); if(bAnsehen){ bAnsehen = false; doEingabe->SetzVerfugbar(false); } }
	 else if(maEingabe->Sichtbar()){ maEingabe->SetzSichtbar(false); if(bAnsehen){ bAnsehen = false; maEingabe->SetzVerfugbar(false); } }
	 else if(mvEingabe->Sichtbar()){ mvEingabe->SetzSichtbar(false); if(bAnsehen){ bAnsehen = false; mvEingabe->SetzVerfugbar(false); } }
 	 else if(daEingabe->Sichtbar()){ daEingabe->SetzSichtbar(false); if(bAnsehen){ bAnsehen = false; daEingabe->SetzVerfugbar(false); } }

	 BYTE ucSpalte; long lPos_x = 0; stScrollHorz.nPage = 0; 
	 for(ucSpalte = stScrollHorz.nPos; ucSpalte < ucSpaltenanzahl; ucSpalte++){
		 lPos_x += vstSpalten[ucSpalte].lBreite + 1;
		 if(lPos_x <= lRand_rechts) stScrollHorz.nPage++;
		 else break;
	 }
	 if(stScrollHorz.nPos + stScrollHorz.nPage > stScrollHorz.nMax){
		 while(stScrollHorz.nPos--){
			 lPos_x += vstSpalten[stScrollHorz.nPos].lBreite + 1;
			 if(lPos_x <= lRand_rechts) stScrollHorz.nPage++;
			 else{ stScrollHorz.nPos++; break;}
		 }
	 }
	 SetScrollInfo(hWndElement, SB_HORZ, &stScrollHorz, true);

	 if(stScrollHorz.nPos + 1 == iPos_Alt || stScrollHorz.nPos - 1 == iPos_Alt){
		 if(stScrollHorz.nPos > iPos_Alt){
       sgKopf->Schalter_Pos_X(stScrollHorz.nPos, rcZeichnen.left);
			 rcZeichnen.right = lRand_rechts;
			 ScrollWindow(hWndElement, rcZeichnen.left *-1, 0, &rcZeichnen, nullptr);

			 rcZeichnen.left = rcZeichnen.right - rcZeichnen.left;

			 lPos_x = (vstSpalten[iPos_Alt].lBreite + 1) *-1;
			 sgKopf->AndernFensterGrosse(0, lPos_x);
       for(ucSpalte = 0; ucSpalte < ucSpaltenanzahl; ucSpalte++) sgKopf->Schalter_AndernPosition(ucSpalte, lPos_x, 0);
		 }
		 else{ rcZeichnen.left = 0;
       rcZeichnen.right = lRand_rechts - vstSpalten[stScrollHorz.nPos].lBreite + 1;
			 ScrollWindow(hWndElement, vstSpalten[stScrollHorz.nPos].lBreite + 1, 0, &rcZeichnen, nullptr);

			 rcZeichnen.right = vstSpalten[stScrollHorz.nPos].lBreite + 1;
			 
			 lPos_x = vstSpalten[stScrollHorz.nPos].lBreite + 1;
			 sgKopf->AndernFensterGrosse(0, lPos_x);
       for(ucSpalte = 0; ucSpalte < ucSpaltenanzahl; ucSpalte++) sgKopf->Schalter_AndernPosition(ucSpalte, lPos_x, 0);
		 }
		 UpdateFenster(&rcZeichnen, true, false);
	 }
	 else{ lPos_x = 0;
	   if(stScrollHorz.nPos > iPos_Alt){
			 for(ucSpalte = iPos_Alt; ucSpalte < stScrollHorz.nPos; ucSpalte++) lPos_x -= vstSpalten[ucSpalte].lBreite + 1; }
		 else{ 
			 for(ucSpalte = stScrollHorz.nPos; ucSpalte < iPos_Alt; ucSpalte++) lPos_x += vstSpalten[ucSpalte].lBreite + 1; }

     sgKopf->AndernFensterGrosse(0, lPos_x);
		 for(ucSpalte = 0; ucSpalte < ucSpaltenanzahl; ucSpalte++) sgKopf->Schalter_AndernPosition(ucSpalte, lPos_x, 0);
		 
		 rcZeichnen.left = 0; rcZeichnen.right = lRand_rechts; 
		 UpdateFenster(&rcZeichnen, true, false);
	 }
 }

 if(bSelectiert){
	 if(bEinfugen || bFeldanderung) SelectZelle(ulZeileSelect, ucSpalteSelect);
	 else{ long lEingabe;	
		 if(sgKopf->Schalter_Pos_X(ucSpalteSelect, lEingabe) >= 0 && lEingabe + vstSpalten[ucSpalteSelect].lBreite < lBreite){ bAktiv = true;
		   SCROLLINFO stScrollVert; stScrollVert.cbSize = sizeof(SCROLLINFO); stScrollVert.fMask = SIF_POS;
       GetScrollInfo(hWndElement, SB_VERT, &stScrollVert);
		   ezEingabe->NeuesFenster(ucZeilenhohe, vstSpalten[ucSpalteSelect].lBreite, lEingabe, ucZeilenhohe * (ulZeileSelect - stScrollVert.nPos) + ucKopfhohe);
			 ezEingabe->SetzSichtbar(true);
		 }
		 else{ bAktiv = false; ezEingabe->SetzSichtbar(false); }
	 }
 }
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::WM_LButtonUp(LPARAM lParam)
{
 ThreadSicher_Anfang();
 SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO); stScrollInfo.fMask = SIF_ALL;
 GetScrollInfo(hWndElement, SB_HORZ, &stScrollInfo);

 long lPixel = 0; BYTE ucSpalteNeu = 0xFF;
 for(BYTE ucSpalte = stScrollInfo.nPos; ucSpalte < ucSpaltenanzahl; ucSpalte++){
   lPixel += vstSpalten[ucSpalte].lBreite + 1;
   if(lPixel < LOWORD(lParam)) ucSpalteNeu = ucSpalte;
   else{ ucSpalteNeu++;
     if(!ucSpalteNeu && stScrollInfo.nPos) ucSpalteNeu += stScrollInfo.nPos;
     break;
   }
 }
 if(ucSpalteNeu == ucSpaltenanzahl - 1 && lPixel < LOWORD(lParam)){ SetFocus(hWndElement); ThreadSicher_Ende(); return; }

 ULONG ulZeileNeu;
 GetScrollInfo(hWndElement, SB_VERT, &stScrollInfo);
 ulZeileNeu = (HIWORD(lParam) - ucKopfhohe) / ucZeilenhohe + stScrollInfo.nPos;
 if(ulZeileNeu >= (UINT)stScrollInfo.nMax){ SetFocus(hWndElement); ThreadSicher_Ende(); return;}
 if(ulZeileNeu - stScrollInfo.nPos == stScrollInfo.nPage - 1 && lRand_unten < lZeichnen_unten + ucZeilenhohe) SendMessage(hWndElement, WM_VSCROLL, SB_LINEDOWN, NULL);

 long lEingabe_links;
 GetScrollInfo(hWndElement, SB_HORZ, &stScrollInfo);
 if(sgKopf->Schalter_Pos_X(ucSpalteNeu, lEingabe_links) + vstSpalten[ucSpalteNeu].lBreite > lRand_rechts){
	 do{ SendMessage(hWndElement, WM_HSCROLL, SB_LINERIGHT, NULL); GetScrollInfo(hWndElement, SB_HORZ, &stScrollInfo); }
	 while(ucSpalteNeu >= stScrollInfo.nPos + (int)stScrollInfo.nPage);
 }

 bAktiv = true; SelectZelle(ulZeileNeu, ucSpalteNeu);
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::WM_MouseWheel(WPARAM wParam, LPARAM lParam)
{
 ThreadSicher_Anfang();
 RECT stClient; GetClientRect(hWndElement, &stClient);
 POINTS stPunkte = MAKEPOINTS(lParam);
 POINT stPunkt; stPunkt.x = stPunkte.x; stPunkt.y = stPunkte.y;
 ScreenToClient(hWndElement, &stPunkt);

 if(stPunkt.x > 0 && stPunkt.x < stClient.right && stPunkt.y > 0 && stPunkt.y < stClient.bottom){
   if(GET_WHEEL_DELTA_WPARAM(wParam) < 0) SendMessage(hWndElement, WM_VSCROLL, SB_LINEDOWN, NULL);
   else SendMessage(hWndElement, WM_VSCROLL, SB_LINEUP, NULL);
 }
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::SetzScrollHorz(SCROLLINFO& stScrollHorz)
{
 stScrollHorz.fMask = SIF_ALL;
 GetScrollInfo(hWndElement, SB_HORZ, &stScrollHorz);

 BYTE ucSpalte; long lPixel = 0; 
 for(ucSpalte = stScrollHorz.nPos; ucSpalte < ucSpaltenanzahl; ucSpalte++){
	 lPixel += vstSpalten[ucSpalte].lBreite + 1;
	 if(lPixel <= lRand_rechts) stScrollHorz.nPage = ucSpalte;
	 else break;
 }
 stScrollHorz.nPage -= stScrollHorz.nPos - 1;
 SetScrollInfo(hWndElement, SB_HORZ, &stScrollHorz, true);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::SetzScrollVert(SCROLLINFO& stScrollVert)
{
 stScrollVert.fMask = SIF_ALL;
 GetScrollInfo(hWndElement, SB_VERT, &stScrollVert);
 stScrollVert.nPage = (lRand_unten - ucKopfhohe) / ucZeilenhohe;
 stScrollVert.nMax = ulDSAnzahl;
 if((int)stScrollVert.nPage <= (lRand_unten - ucKopfhohe) / ucZeilenhohe) stScrollVert.nPage++;
 if(stScrollVert.nPos + (int)stScrollVert.nPage > stScrollVert.nMax) stScrollVert.nPage = stScrollVert.nMax - stScrollVert.nPos + 1;
 SetScrollInfo(hWndElement, SB_VERT, &stScrollVert, true);
 lZeichnen_unten = (stScrollVert.nPage - 1) * ucZeilenhohe + ucKopfhohe;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::TabellenKopf_Schriftart(LOGFONT& lfSchriftA)
{
 ThreadSicher_Anfang();
 for(BYTE ucSpalte = 0; ucSpalte < ucSpaltenanzahl; ucSpalte++) sgKopf->Schalter_Schriftart(ucSpalte, lfSchriftA);
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::TabellenZeilen_Schriftart(LOGFONT& lfSchriftA)
{
 ThreadSicher_Anfang();
 lfSchrift = lfSchriftA;
 ezEingabe->Schriftart(lfSchriftA); ebEingabe->Schriftart(lfSchriftA); lbEingabe->Schriftart(lfSchriftA);
 biEingabe->Schriftart(lfSchriftA); maEingabe->Schriftart(lfSchriftA); mvEingabe->Schriftart(lfSchriftA);
 daEingabe->Schriftart(lfSchriftA);
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::Spaltenhohe(unsigned char ucKopf, unsigned char ucZeile)
{
 ThreadSicher_Anfang();
 ucKopfhohe = ucKopf; ucZeilenhohe = ucZeile;
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
unsigned char __vectorcall RePag::GUI::COTabBasis::Spaltenanzahl(void)
{
 ThreadSicher_Anfang();
 BYTE ucSpaltenanzahlA = ucSpaltenanzahl;
 ThreadSicher_Ende();
 return ucSpaltenanzahlA;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
unsigned long __vectorcall RePag::GUI::COTabBasis::Zeilenanzahl(void)
{
 ThreadSicher_Anfang();
 ULONG ulDSAnzahlA = ulDSAnzahl;
 ThreadSicher_Ende();
 return ulDSAnzahlA;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
unsigned long __vectorcall RePag::GUI::COTabBasis::SelectedZeile(void)
{
 ThreadSicher_Anfang();
 ULONG ulZeileSelectA = ulZeileSelect;
 ThreadSicher_Ende();
 return ulZeileSelectA;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
unsigned char __vectorcall RePag::GUI::COTabBasis::SelectedSpalte(void)
{
 ThreadSicher_Anfang();
 BYTE ucSpalteSelectA = ucSpalteSelect;
 ThreadSicher_Ende();
 return ucSpalteSelectA;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::NeueTabelle(void)
{
 if(bEinfugen) LoschEinfugen();
 else if(bFeldanderung) LoschAndern();

 BYTE ucSpalte;
 while(ulDSAnzahl){ ulDSAnzahl--;
   for(ucSpalte = 0; ucSpalte < ucSpaltenanzahl + ucZusatzspalten; ucSpalte++){
     if(vpTabelle[ulDSAnzahl][ucSpalte].vbWert) VMFreiS(vmTabelle, vpTabelle[ulDSAnzahl][ucSpalte].vbWert);
   }
   VMFreiS(vmTabelle, vpTabelle[ulDSAnzahl]);
 }
 if(vpTabelle) VMFreiS(vmTabelle, vpTabelle);

 for(ucSpalte = 0; ucSpalte < ucSpaltenanzahl; ucSpalte++){	 
	 if(vucFeldStatus){ vucFeldStatus[ucSpalte] <<= 3; vucFeldStatus[ucSpalte] >>= 3; }
	 if(sgKopf->Schalter_Eingeschaltet(ucSpalte)) sgKopf->Schalter_SetzSchalter(ucSpalte, false);
	 if(sgKopf->Schalter_Aktiv(ucSpalte)) sgKopf->Schalter_SetzAktiv(ucSpalte, false);
 }

 ulTabZeilen_max = ulTabZeilen_blatt;
 vpTabelle = (STFeld**)VMBlockS(vmTabelle, ulTabZeilen_max * 4);

 ucSpalteSelect = 0; ulZeileSelect = 0; bSelectiert = false; bAktiv = false;
 if(ezEingabe->Sichtbar()){ ezEingabe->SetzSichtbar(false); ezEingabe->SetzVerfugbar(false); }
 else if(ebEingabe->Sichtbar()){ ebEingabe->SetzSichtbar(false); ebEingabe->SetzVerfugbar(false); bAnsehen = false; ebEingabe->Zeichenvorgabe(ZV_ALLE); }
 else if(lbEingabe->Sichtbar()){ lbEingabe->SetzSichtbar(false); lbEingabe->SetzVerfugbar(false); bAnsehen = false; }
 else if(biEingabe->Sichtbar()){ biEingabe->SetzSichtbar(false); biEingabe->SetzVerfugbar(false); bAnsehen = false; }
 else if(doEingabe->Sichtbar()){ doEingabe->SetzSichtbar(false); doEingabe->SetzVerfugbar(false); bAnsehen = false; }
 else if(maEingabe->Sichtbar()){ maEingabe->SetzSichtbar(false); maEingabe->SetzVerfugbar(false); bAnsehen = false; }
 else if(mvEingabe->Sichtbar()){ mvEingabe->SetzSichtbar(false); mvEingabe->SetzVerfugbar(false); bAnsehen = false; }
 else if(daEingabe->Sichtbar()){ daEingabe->SetzSichtbar(false); daEingabe->SetzVerfugbar(false); bAnsehen = false; }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::NeueTabelle_Zeichnen(void)
{
 if(pfnLeerFeld){
	 for(ucSpalteSelect = 0; ucSpalteSelect < ucSpaltenanzahl; ucSpalteSelect++){
		if(FeldTyp(vucFeldStatus[ucSpalteSelect]) == FT_EMPTY){
			for(ulZeileSelect = 0; ulZeileSelect < ulDSAnzahl; ulZeileSelect++)	pfnLeerFeld(this);
		}
	 }
 }
 ulZeileSelect = ucSpalteSelect = 0;

 SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO); stScrollInfo.fMask = SIF_ALL; RECT rcZeichnen;
 GetScrollInfo(hWndElement, SB_VERT, &stScrollInfo);
 if(stScrollInfo.nPage >= ulDSAnzahl){ 
	 stScrollInfo.nMax = 0;
	 SetScrollInfo(hWndElement, SB_VERT, &stScrollInfo, false);
	 UpdateFenster(NULL, true, false);
 }
 stScrollInfo.nPos = 0;
 SetScrollInfo(hWndElement, SB_VERT, &stScrollInfo, false);
 SetzScrollVert(stScrollInfo);
 GetScrollInfo(hWndElement, SB_HORZ, &stScrollInfo);
 while(stScrollInfo.nPos){ SendMessage(hWndElement, WM_HSCROLL, SB_PAGELEFT, NULL); GetScrollInfo(hWndElement, SB_HORZ, &stScrollInfo); }
 SetzScrollHorz(stScrollInfo);

 rcZeichnen.top = ucKopfhohe; rcZeichnen.left = 0;
 if(sgKopf->Breite(rcZeichnen.right) > lRand_rechts) rcZeichnen.right = lRand_rechts; 
 rcZeichnen.bottom = lZeichnen_unten;
 UpdateFenster(&rcZeichnen, true, false);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
STFeld* __vectorcall RePag::GUI::COTabBasis::DSErstellen(void)
{
 STFeld* vstFelder = (STFeld*)VMBlockS(vmTabelle, BY_STFeld * (ucSpaltenanzahl + ucZusatzspalten));

 for(BYTE ucSpalte = 0; ucSpalte < ucSpaltenanzahl + ucZusatzspalten; ucSpalte++){
	 switch(FeldTyp(vucFeldStatus[ucSpalte])){
		 case FT_BOOL       : vstFelder[ucSpalte].ulLange = BY_BOOL; vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange); break;
		 case FT_CHAR       : vstFelder[ucSpalte].ulLange = BY_CHAR; vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange); break;
		 case FT_BYTE       : vstFelder[ucSpalte].ulLange = BY_BYTE; vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange); break;
		 case FT_SHORT      : vstFelder[ucSpalte].ulLange = BY_SHORT; vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange); break;
		 case FT_USHORT     : vstFelder[ucSpalte].ulLange = BY_USHORT; vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange); break;
		 case FT_LONG       : vstFelder[ucSpalte].ulLange = BY_LONG; vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange); break;
		 case FT_ULONG      : vstFelder[ucSpalte].ulLange = BY_ULONG; vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange); break;
		 case FT_LONGLONG   : vstFelder[ucSpalte].ulLange = BY_LONGLONG; vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange); break;
		 case FT_FLOAT      : vstFelder[ucSpalte].ulLange = BY_FLOAT; vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange); break;
		 case FT_DOUBLE     : vstFelder[ucSpalte].ulLange = BY_DOUBLE; vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange); break;
		 case FT_COMMA4     : vstFelder[ucSpalte].ulLange = BY_COMMA4; vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange); break;
		 case FT_COMMA4_80  : vstFelder[ucSpalte].ulLange = BY_COMMA4_80; vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange); break;
		 case FT_DATE       : vstFelder[ucSpalte].ulLange = BY_DATE; vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange); break;
		 case FT_TIME       : vstFelder[ucSpalte].ulLange = BY_TIME; vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange); break;
		 case FT_BIT128     : vstFelder[ucSpalte].ulLange = BY_BIT128; vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange); break;
		 case FT_VIDEO      :	
		 case FT_AUDIO      :
		 case FT_DOCUMENT   :
		 case FT_BIGIMAGE   :
		 case FT_LONGSTR    : 
		 case FT_EMPTY      : 
		 case FT_SMALLIMAGE :
		 case FT_LISTSTR    :
		 case FT_MEMOSTR    :
		 case FT_SHORTSTR   : vstFelder[ucSpalte].ulLange = BY_NULL; vstFelder[ucSpalte].vbWert = nullptr; break;
   }
 }
 return vstFelder;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::DSAnfugen(STFeld* pstFelder)
{
 if(ulDSAnzahl < ulTabZeilen_max) vpTabelle[ulDSAnzahl++] = pstFelder;
 else{ ULONG ulDS = 0; ulTabZeilen_max += ulTabZeilen_blatt;
       STFeld** vpTabelleNeu = (STFeld**)VMBlockS(vmTabelle, ulTabZeilen_max * 4);
			 while(ulDS < ulDSAnzahl){ vpTabelleNeu[ulDS] = vpTabelle[ulDS]; ulDS++; }
       vpTabelleNeu[ulDS++] = pstFelder; ulDSAnzahl++;
       VMFreiS(vmTabelle, vpTabelle);
       vpTabelle = vpTabelleNeu;
 }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
inline unsigned char __vectorcall RePag::GUI::COTabBasis::FeldTyp(unsigned char ucStatusSpalte)
{
 ucStatusSpalte <<= 3; ucStatusSpalte >>= 3; return ucStatusSpalte;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall COTabBasis::QuickSort(long lSpalte, unsigned char ucStatusSpalte, long lIndexLinks, long lIndexRechts)
{
 if(lIndexRechts <= lIndexLinks) return;
 STFeld* pstFelder = vpTabelle[lIndexRechts];
 long lZeigerLinks_1 = lIndexLinks - 1, lZeigerRechts_1 = lIndexRechts,
      lZeigerLinks_2 = lIndexLinks - 1, lZeigerRechts_2 = lIndexRechts;

 char cRichtung = 1;
 if(ucStatusSpalte & FT_SORTUP) cRichtung = -1;
 while(TRUE){
   while(FeldVergleich(ucStatusSpalte, vpTabelle[++lZeigerLinks_1][lSpalte], pstFelder[lSpalte]) == cRichtung);
   while(FeldVergleich(ucStatusSpalte, pstFelder[lSpalte], vpTabelle[--lZeigerRechts_1][lSpalte]) == cRichtung)
      if( lZeigerRechts_1 == lIndexLinks) break;
   if(lZeigerLinks_1 >= lZeigerRechts_1) break;
   QSIndexTausch(lZeigerLinks_1, lZeigerRechts_1);
   if(!FeldVergleich(ucStatusSpalte, vpTabelle[lZeigerLinks_1][lSpalte], pstFelder[lSpalte])){
       lZeigerLinks_2++; QSIndexTausch(lZeigerLinks_2, lZeigerLinks_1); }
   if(!FeldVergleich(ucStatusSpalte, pstFelder[lSpalte], vpTabelle[lZeigerRechts_1][lSpalte])){
       lZeigerRechts_2--; QSIndexTausch(lZeigerRechts_2, lZeigerRechts_1); } 
 }
 QSIndexTausch(lZeigerLinks_1, lIndexRechts); lZeigerRechts_1 = lZeigerLinks_1 - 1; lZeigerLinks_1++;
 long lx;
 for(lx = lIndexLinks; lx <= lZeigerLinks_2; lx++, lZeigerRechts_1--) QSIndexTausch(lx, lZeigerRechts_1);
 for(lx = lIndexRechts - 1; lx >= lZeigerRechts_2; lx--, lZeigerLinks_1++) QSIndexTausch(lx, lZeigerLinks_1);
 QuickSort(lSpalte, ucStatusSpalte, lIndexLinks, lZeigerRechts_1);
 QuickSort(lSpalte, ucStatusSpalte, lZeigerLinks_1, lIndexRechts);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::QSIndexTausch(long lIndex_1, long lIndex_2)
{
 STFeld* pstFelderTemp = vpTabelle[lIndex_1];
 vpTabelle[lIndex_1] = vpTabelle[lIndex_2];
 vpTabelle[lIndex_2] = pstFelderTemp;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
char __vectorcall RePag::GUI::COTabBasis::FeldVergleich(unsigned char ucStatusSpalte, STFeld& stFeld_1, STFeld& stFeld_2)
{
 switch(FeldTyp(ucStatusSpalte)){
	 case FT_EMPTY      :
	 case FT_LISTSTR    :
	 case FT_SHORTSTR   : return StrCompare(stFeld_1.vbWert, stFeld_1.ulLange, stFeld_2.vbWert, stFeld_2.ulLange);

	 case FT_BOOL       : if(*(PCHAR)stFeld_1.vbWert == *(PCHAR)stFeld_2.vbWert) return 0;
											  else if(*(PCHAR)stFeld_1.vbWert < *(PCHAR)stFeld_2.vbWert) return - 1;
											  break;
	 case FT_CHAR       : if(*(PCHAR)stFeld_1.vbWert == *(PCHAR)stFeld_2.vbWert) return 0;
											  else if(*(PCHAR)stFeld_1.vbWert < *(PCHAR)stFeld_2.vbWert) return - 1;
											  break;
	 case FT_BYTE       : if(*(PBYTE)stFeld_1.vbWert == *(PBYTE)stFeld_2.vbWert) return 0;
											  else if(*(PBYTE)stFeld_1.vbWert < *(PBYTE)stFeld_2.vbWert) return - 1;
											  break;
	 case FT_SHORT      : if(*(PSHORT)stFeld_1.vbWert == *(PSHORT)stFeld_2.vbWert) return 0;
											  else if(*(PSHORT)stFeld_1.vbWert < *(PSHORT)stFeld_2.vbWert) return - 1;
											  break;
	 case FT_USHORT     : if(*(PUSHORT)stFeld_1.vbWert == *(PUSHORT)stFeld_2.vbWert) return 0;
											  else if(*(PUSHORT)stFeld_1.vbWert == *(PUSHORT)stFeld_2.vbWert) return - 1;
											  break;
	 case FT_LONG       : if(*(PLONG)stFeld_1.vbWert == *(PLONG)stFeld_2.vbWert) return 0;
											  else if(*(PLONG)stFeld_1.vbWert < *(PLONG)stFeld_2.vbWert) return - 1;
											  break;
	 case FT_ULONG      : if(*(PULONG)stFeld_1.vbWert == *(PULONG)stFeld_2.vbWert) return 0;
											  else if(*(PULONG)stFeld_1.vbWert < *(PULONG)stFeld_2.vbWert) return - 1;
											  break;
	 case FT_LONGLONG   : if(*(long long*)stFeld_1.vbWert == *(long long*)stFeld_2.vbWert) return 0;
											  else if(*(long long*)stFeld_1.vbWert < *(long long*)stFeld_2.vbWert) return - 1;
											  break;
	 case FT_FLOAT      : if(*(float*)stFeld_1.vbWert == *(float*)stFeld_2.vbWert) return 0;
											  else if(*(float*)stFeld_1.vbWert < *(float*)stFeld_2.vbWert) return - 1;
											  break;
	 case FT_DOUBLE     : if(*(double*)stFeld_1.vbWert == *(double*)stFeld_2.vbWert) return 0;
												else if(*(double*)stFeld_1.vbWert < *(double*)stFeld_2.vbWert) return - 1;
											  break;
	 case FT_COMMA4     : return Komma4Vergleich(stFeld_1.vbWert, stFeld_2.vbWert);
	 case FT_COMMA4_80  : return Komma4_80Vergleich(stFeld_1.vbWert, stFeld_2.vbWert);
	 case FT_DATE       :
	 case FT_TIME       : return ZeitVergleich(stFeld_1.vbWert, stFeld_2.vbWert);
	 case FT_BIT128     : return BIT128Compare(*(BIT128*)stFeld_1.vbWert, *(BIT128*)stFeld_2.vbWert);
	 case FT_VIDEO      :
	 case FT_AUDIO      :
	 case FT_DOCUMENT   :
	 case FT_BIGIMAGE   :
	 case FT_LONGSTR    : 
	 case FT_SMALLIMAGE : 
	 case FT_MEMOSTR    : return 0;
 }
 return + 1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
char __vectorcall RePag::GUI::COTabBasis::Komma4Vergleich(char* pcWert_1, char* pcWert_2)
{
 COComma4 k4Zahl_1, k4Zahl_2; char cReturn;
 k4Zahl_1.Write(pcWert_1);
 k4Zahl_2.Write(pcWert_2);
 cReturn = k4Zahl_1.Compare(&k4Zahl_2);
 return cReturn;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
char __vectorcall RePag::GUI::COTabBasis::Komma4_80Vergleich(char* pcWert_1, char* pcWert_2)
{
 COComma4_80 k4gZahl_1, k4gZahl_2; char cReturn;
 k4gZahl_1.Write(pcWert_1);
 k4gZahl_2.Write(pcWert_2);
 cReturn = k4gZahl_1.Compare(&k4gZahl_2);
 return cReturn;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
char __vectorcall RePag::GUI::COTabBasis::ZeitVergleich(char* pcWert_1, char* pcWert_2)
{
	COTime zZahl_1, zZahl_2; char cReturn;
 zZahl_1.Write(pcWert_1);
 zZahl_2.Write(pcWert_2);
 cReturn = (char)CompareFileTime(&zZahl_1.FileTime(), &zZahl_2.FileTime());
 return cReturn;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
COFeld* __vectorcall RePag::GUI::COTabBasis::FeldWert(COFeld* pfdFeld, unsigned long ulZeile, unsigned char ucSpalte)
{
 ThreadSicher_Anfang();
 if(hWndElement && ulZeile < ulDSAnzahl && ucSpalte < ucSpaltenanzahl + ucZusatzspalten) *pfdFeld = vpTabelle[ulZeile][ucSpalte];
 ThreadSicher_Ende();
 return pfdFeld;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::SelectZelle(unsigned long ulZeileNeu, unsigned char ucSpalteNeu)
{
 SCROLLINFO siZeile, siSpalte; siSpalte.cbSize = siZeile.cbSize = sizeof(SCROLLINFO); siSpalte.fMask = siZeile.fMask = SIF_POS | SIF_PAGE;
 GetScrollInfo(hWndElement, SB_VERT, &siZeile); GetScrollInfo(hWndElement, SB_HORZ, &siSpalte);

 if(ulZeileNeu <= (UINT)siZeile.nPos + siZeile.nPage - 1 && ucSpalteNeu <= (UINT)siSpalte.nPos + siSpalte.nPage){	 
	 long lEingabe_Links, lEingabe_Breite;
	 if(bSelectiert){ 
		 if(bEinfugen || bFeldanderung){ bAktiv = true;
		   if(!AndernWert(true)) return;
			 if(ulZeileNeu == ulZeileSelect){ ucSpalteSelect = ucSpalteNeu;
				 if(!(vstSpalten[ucSpalteSelect].usFormat & TX_UNANDERBAR)){ 
					 sgKopf->Schalter_Pos_X(ucSpalteSelect, lEingabe_Links);
					 (lEingabe_Links + vstSpalten[ucSpalteSelect].lBreite > lRand_rechts ? lEingabe_Breite = lRand_rechts - lEingabe_Links : lEingabe_Breite = vstSpalten[ucSpalteSelect].lBreite);
					 ezEingabe->NeuesFenster(ucZeilenhohe, lEingabe_Breite, lEingabe_Links, ucZeilenhohe * (ulZeileSelect - siZeile.nPos) + ucKopfhohe);
					 switch(FeldTyp(vucFeldStatus[ucSpalteSelect])){
							 case FT_LONGSTR   :
							 case FT_MEMOSTR   : if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
												           else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
																   else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
																	 else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
																	 else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
																	 else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
																	 else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																	 Eingabe_Position(ebEingabe, siZeile);
																	 FeldWert();
																   ebEingabe->Hintergrundfarbe(255, 255, 255); ebEingabe->Schriftfarbe(0, 0, 128);
																   ezEingabe->SetzVerfugbar(false); ebEingabe->SetzVerfugbar(true); SetFocus(ebEingabe->HWND_Element()); 
								                   return;
							 case FT_LISTSTR   : if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
												           else if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
																   else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
																	 else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
																	 else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
																	 else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
																	 else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																	 Eingabe_Position(lbEingabe, siZeile);
																	 FeldWert();
																	 lbEingabe->Hintergrundfarbe(255, 255, 255); lbEingabe->Schriftfarbe(0, 0, 128);
																   ezEingabe->SetzVerfugbar(false); lbEingabe->SetzVerfugbar(true); SetFocus(lbEingabe->HWND_Element()); 
								                   return;
							 case FT_BIGIMAGE :
							 case FT_SMALLIMAGE : if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
																   else if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
                                   else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
																	 else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
																	 else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
																	 else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
																	 else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																	 Eingabe_Position(biEingabe, siZeile);
																	 FeldWert();
																	 biEingabe->Infotext("Ein Bild einfügen");
																	 biEingabe->Hintergrundfarbe(255, 255, 255); biEingabe->Schriftfarbe(0, 0, 128);
																   ezEingabe->SetzVerfugbar(false); biEingabe->SetzVerfugbar(true);
																	 biEingabe->Berechtigungen(ZR_OFFNEN | ZR_SPEICHERN | ZR_LOSCHEN); SetFocus(biEingabe->HWND_Element()); 
								                   return;
							 case FT_DOCUMENT  : if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
												           else if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
																   else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
																	 else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
																	 else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
																	 else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
																	 else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																	 Eingabe_Position(doEingabe, siZeile);
																	 FeldWert();
																   ezEingabe->SetzVerfugbar(false); doEingabe->SetzVerfugbar(true); SetFocus(doEingabe->HWND_Element()); 
								                   return;
							 case FT_AUDIO     : if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
																   else if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
                                   else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
																	 else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
																	 else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
																	 else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
																	 else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																	 Eingabe_Position(maEingabe, siZeile);
																	 FeldWert();
																	 maEingabe->Infotext("Ein Audio einfügen");
																	 maEingabe->Hintergrundfarbe(255, 255, 255); maEingabe->Schriftfarbe(0, 0, 128);
																   ezEingabe->SetzVerfugbar(false); maEingabe->SetzVerfugbar(true); maEingabe->Berechtigungen(ZR_OFFNEN | ZR_SPEICHERN | ZR_LOSCHEN);
																	 SetFocus(maEingabe->HWND_Element()); 
								                   return;
							 case FT_VIDEO     : if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
																   else if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
                                   else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
																	 else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
																	 else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
																	 else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
																	 else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																	 Eingabe_Position(mvEingabe, siZeile);
																	 FeldWert();
																	 mvEingabe->Infotext("Ein Video einfügen");
																	 mvEingabe->Hintergrundfarbe(255, 255, 255); mvEingabe->Schriftfarbe(0, 0, 128);
																   ezEingabe->SetzVerfugbar(false); mvEingabe->SetzVerfugbar(true); mvEingabe->Berechtigungen(ZR_OFFNEN | ZR_SPEICHERN | ZR_LOSCHEN);
																	 SetFocus(mvEingabe->HWND_Element()); 
								                   return;
							 case FT_DATE     : if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
																   else if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
                                   else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
																	 else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
																	 else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
																	 else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
																	 else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
																	 Eingabe_Position(daEingabe, siZeile);
																	 FeldWert();
																	 daEingabe->Hintergrundfarbe(255, 255, 255); daEingabe->Schriftfarbe(0, 0, 128);
																   ezEingabe->SetzVerfugbar(false); daEingabe->SetzVerfugbar(true); SetFocus(daEingabe->HWND_Element()); 
								                   return;
							 case FT_BOOL      : if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
												           else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
																   else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
																	 else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
																	 else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
																	 else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
																	 else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																   ezEingabe->Hintergrundfarbe(255, 255, 255); ezEingabe->Schriftfarbe(0, 0, 128);	
																   FeldWert();
																	 ezEingabe->SetzSichtbar(true); SetFocus(ezEingabe->HWND_Element()); 
																   return;
							 //case FT_BIT128    : if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
								//									 else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
								//									 else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
								//									 else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
								//									 else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
								//									 else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
								//									 else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
								//				 					 ezEingabe->Hintergrundfarbe(255, 255, 255); ezEingabe->Schriftfarbe(0, 0, 128);
								//									 FeldWert();
								//									 ezEingabe->SetzSichtbar(true); SetFocus(ezEingabe->HWND_Element());
								//									 return;
											 default   : if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
												           else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
																   else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
																	 else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
																	 else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
																	 else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
																	 else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
																   ezEingabe->Hintergrundfarbe(255, 255, 255); ezEingabe->Schriftfarbe(0, 0, 128);	
																   FeldWert();
																   ezEingabe->SetzVerfugbar(true); SetFocus(ezEingabe->HWND_Element()); 						 
																   return;
					 }
				 }
				 else{ bAktiv = false; ezEingabe->SetzVerfugbar(false); ezEingabe->Hintergrundfarbe(130, 200, 130); ezEingabe->Schriftfarbe(100, 100, 100); FeldWert(); }
			 }
			 else{ WM_Command(IDM_DS_SENDEN);
						 SetEvent(heSelect); bSelectiert = true; ucSpalteSelect = ucSpalteNeu; ulZeileSelect = ulZeileNeu;
             ezEingabe->SetzSichtbar(true); ezEingabe->SetzVerfugbar(false);
						 if(!(vstSpalten[ucSpalteSelect].usFormat & TX_UNANDERBAR)){ bAktiv = true; ezEingabe->Hintergrundfarbe(0, 128, 0); ezEingabe->Schriftfarbe(255, 255, 255); }
						 else{ bAktiv = false; ezEingabe->Hintergrundfarbe(130, 200, 130); ezEingabe->Schriftfarbe(100, 100, 100); }
					
						 sgKopf->Schalter_Pos_X(ucSpalteSelect, lEingabe_Links);
						 (lEingabe_Links + vstSpalten[ucSpalteSelect].lBreite > lRand_rechts ? lEingabe_Breite = lRand_rechts - lEingabe_Links : lEingabe_Breite = vstSpalten[ucSpalteSelect].lBreite);
						 ezEingabe->NeuesFenster(ucZeilenhohe, lEingabe_Breite, lEingabe_Links, ucZeilenhohe * (ulZeileSelect - siZeile.nPos) + ucKopfhohe); 
						 FeldWert();
			 }
		 }
		 else if(ucSpalteSelect == ucSpalteNeu && ulZeileSelect == ulZeileNeu){ SetEvent(heSelect);
			 switch(FeldTyp(vucFeldStatus[ucSpalteSelect])){
					 case FT_AUDIO      :
					 case FT_VIDEO      :
					 case FT_LONGSTR    :
					 case FT_MEMOSTR    :
					 case FT_LISTSTR    : 
					 case FT_BIGIMAGE   :
					 case FT_SMALLIMAGE : SendMessage(hWndElement, WM_COMMAND, IDM_DF_ANSEHEN, NULL); break;
					 case FT_DOCUMENT   : SetFocus(hWndElement);
					         default    : ezEingabe->SetzSichtbar(true); 
															  if(!(vstSpalten[ucSpalteSelect].usFormat & TX_UNANDERBAR)){ bAktiv = true; ezEingabe->Hintergrundfarbe(0, 128, 0); ezEingabe->Schriftfarbe(255, 255, 255); }
															  else{ bAktiv = false; ezEingabe->Hintergrundfarbe(130, 200, 130); ezEingabe->Schriftfarbe(100, 100, 100); }
														
															  sgKopf->Schalter_Pos_X(ucSpalteSelect, lEingabe_Links);
															  (lEingabe_Links + vstSpalten[ucSpalteSelect].lBreite > lRand_rechts ? lEingabe_Breite = lRand_rechts - lEingabe_Links : lEingabe_Breite = vstSpalten[ucSpalteSelect].lBreite);
															  ezEingabe->NeuesFenster(ucZeilenhohe, lEingabe_Breite, lEingabe_Links, ucZeilenhohe * (ulZeileSelect - siZeile.nPos) + ucKopfhohe); 
															  SetFocus(hWndElement);
			 }		   
			 return;
		 }
		 else{ ucSpalteSelect = ucSpalteNeu; ulZeileSelect = ulZeileNeu;
		   if(bAnsehen){ bAnsehen = false;
				 if(ebEingabe->Sichtbar()){ ebEingabe->SetzSichtbar(false); ebEingabe->SetzVerfugbar(false); ebEingabe->Zeichenvorgabe(ZV_ALLE); }
				 else if(lbEingabe->Sichtbar()){ lbEingabe->SetzSichtbar(false); lbEingabe->SetzVerfugbar(false); }
				 else if(biEingabe->Sichtbar()){ biEingabe->SetzSichtbar(false); biEingabe->SetzVerfugbar(false); }
				 else if(doEingabe->Sichtbar()){ doEingabe->SetzSichtbar(false); doEingabe->SetzVerfugbar(false); }
				 else if(maEingabe->Sichtbar()){ maEingabe->SetzSichtbar(false); maEingabe->SetzVerfugbar(false); }
				 else if(mvEingabe->Sichtbar()){ mvEingabe->SetzSichtbar(false); mvEingabe->SetzVerfugbar(false); }
				 else if(daEingabe->Sichtbar()){ daEingabe->SetzSichtbar(false); daEingabe->SetzVerfugbar(false); }
			 }
		   ezEingabe->SetzSichtbar(true); SetEvent(heSelect);  
		   if(!(vstSpalten[ucSpalteSelect].usFormat & TX_UNANDERBAR)){ bAktiv = true; ezEingabe->Hintergrundfarbe(0, 128, 0); ezEingabe->Schriftfarbe(255, 255, 255); }
		   else{ bAktiv = false; ezEingabe->Hintergrundfarbe(130, 200, 130); ezEingabe->Schriftfarbe(100, 100, 100); }
			 FeldWert();
		 }
	 }
	 else{ ucSpalteSelect = ucSpalteNeu; ulZeileSelect = ulZeileNeu; bSelectiert = true; SetEvent(heSelect); 
	   ezEingabe->SetzSichtbar(true); ezEingabe->SetzVerfugbar(false); 
	   if(!(vstSpalten[ucSpalteSelect].usFormat & TX_UNANDERBAR)){ bAktiv = true; ezEingabe->Hintergrundfarbe(0, 128, 0); ezEingabe->Schriftfarbe(255, 255, 255); }
		 else{ bAktiv = false; ezEingabe->Hintergrundfarbe(130, 200, 130); ezEingabe->Schriftfarbe(100, 100, 100); }
		 FeldWert();
	 }

	 sgKopf->Schalter_Pos_X(ucSpalteSelect, lEingabe_Links);
	 (lEingabe_Links + vstSpalten[ucSpalteSelect].lBreite > lRand_rechts ? lEingabe_Breite = lRand_rechts - lEingabe_Links : lEingabe_Breite = vstSpalten[ucSpalteSelect].lBreite);
	 ezEingabe->NeuesFenster(ucZeilenhohe, lEingabe_Breite, lEingabe_Links, ucZeilenhohe * (ulZeileSelect - siZeile.nPos) + ucKopfhohe);
	 SetFocus(hWndElement);
 }
 else{ bAktiv = false;
	 if(ezEingabe->Sichtbar()) ezEingabe->SetzSichtbar(false);
	 else if(ebEingabe->Sichtbar()) ebEingabe->SetzSichtbar(false);
	 else if(biEingabe->Sichtbar()) biEingabe->SetzSichtbar(false);
	 else if(lbEingabe->Sichtbar()) lbEingabe->SetzSichtbar(false);
	 else if(doEingabe->Sichtbar()) doEingabe->SetzSichtbar(false);
	 else if(maEingabe->Sichtbar()) maEingabe->SetzSichtbar(false);
	 else if(mvEingabe->Sichtbar()) mvEingabe->SetzSichtbar(false);
	 else if(daEingabe->Sichtbar()) daEingabe->SetzSichtbar(false);
 }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::EinfugenRollback(void)
{
 ulDSAnzahl--;
 for(BYTE ucSpalte = 0; ucSpalte < ucSpaltenanzahl + ucZusatzspalten; ucSpalte++) VMFreiS(vmTabelle, vpTabelle[ulDSAnzahl][ucSpalte].vbWert);
 VMFreiS(vmTabelle, vpTabelle[ulDSAnzahl]);
 vpTabelle[ulDSAnzahl] = NULL;

 bSelectiert = false; 
 (ulDSAnzahl ? ulZeileSelect-- : ulZeileSelect = 0);
 switch(FeldTyp(vucFeldStatus[ucSpalteSelect])){
		 case FT_LONGSTR    :
		 case FT_MEMOSTR    : ebEingabe->SetzSichtbar(false); ebEingabe->SetzVerfugbar(false); break;
		 case FT_LISTSTR    : lbEingabe->SetzSichtbar(false); lbEingabe->SetzVerfugbar(false); break;
		 case FT_BIGIMAGE   :
		 case FT_SMALLIMAGE : biEingabe->SetzSichtbar(false); biEingabe->SetzVerfugbar(false); break;
		 case FT_DOCUMENT   : doEingabe->SetzSichtbar(false); doEingabe->SetzVerfugbar(false); break;
		 case FT_AUDIO      : maEingabe->SetzSichtbar(false); maEingabe->SetzVerfugbar(false); break;
		 case FT_VIDEO      : mvEingabe->SetzSichtbar(false); mvEingabe->SetzVerfugbar(false); break;
		 case FT_DATE       : daEingabe->SetzSichtbar(false); daEingabe->SetzVerfugbar(false); break;
		         default    : ezEingabe->SetzSichtbar(false); ezEingabe->SetzVerfugbar(false); 
 }
 SCROLLINFO stScrollVert; stScrollVert.cbSize = sizeof(SCROLLINFO); RECT rcZeichnen;
 SetzScrollVert(stScrollVert);
 rcZeichnen.left = 0;
 if(sgKopf->Breite(rcZeichnen.right) > lRand_rechts) rcZeichnen.right = lRand_rechts;
 rcZeichnen.top = (stScrollVert.nPage - 2) * ucZeilenhohe + ucKopfhohe; rcZeichnen.bottom = rcZeichnen.top + ucZeilenhohe * 2;
 UpdateFenster(&rcZeichnen, true, false);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::LoschEinfugen(void)
{
 bEinfugen = false; ucMaxSpalte = 0xFF;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::AndernRollback(void)
{
 if(vstFelderAlt){
	 for(BYTE ucSpalte = 0; ucSpalte < ucSpaltenanzahl + ucZusatzspalten; ucSpalte++){
		 switch(FeldTyp(vucFeldStatus[ucSpalte])){
				 case FT_EMPTY      : 
				 case FT_VIDEO      :
				 case FT_AUDIO      :
				 case FT_DOCUMENT   :
				 case FT_SHORTSTR   :
				 case FT_LONGSTR    :
				 case FT_MEMOSTR    : 
				 case FT_LISTSTR    :					 
				 case FT_BIGIMAGE   :
				 case FT_SMALLIMAGE : if(vpTabelle[ulZeileSelect][ucSpalte].vbWert) VMFreiS(vmTabelle, vpTabelle[ulZeileSelect][ucSpalte].vbWert);
														  vpTabelle[ulZeileSelect][ucSpalte].ulLange = vstFelderAlt[ucSpalte].ulLange;
														  vpTabelle[ulZeileSelect][ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelderAlt[ucSpalte].ulLange);		 
									 default  : MemCopy(vpTabelle[ulZeileSelect][ucSpalte].vbWert, vstFelderAlt[ucSpalte].vbWert, vstFelderAlt[ucSpalte].ulLange);
		 }  
	 }
 }
 SCROLLINFO stScrollVert; stScrollVert.cbSize = sizeof(SCROLLINFO); stScrollVert.fMask = SIF_POS; RECT rcZeichnen;
 GetScrollInfo(hWndElement, SB_VERT, &stScrollVert);
 rcZeichnen.left = 0;
 if(sgKopf->Breite(rcZeichnen.right) > lRand_rechts) rcZeichnen.right = lRand_rechts;
 rcZeichnen.top = (ulZeileSelect - stScrollVert.nPos) * ucZeilenhohe + ucKopfhohe; rcZeichnen.bottom = rcZeichnen.top + ucZeilenhohe;
 UpdateFenster(&rcZeichnen, true, false);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::LoschAndern(void)
{
 if(vstFelderAlt){
   for(BYTE ucSpalte = 0; ucSpalte < ucSpaltenanzahl + ucZusatzspalten; ucSpalte++)
     if(vstFelderAlt[ucSpalte].vbWert){ VMFreiS(vmTabelle, vstFelderAlt[ucSpalte].vbWert); vstFelderAlt[ucSpalte].vbWert = nullptr; }
   VMFreiS(vmTabelle, vstFelderAlt); vstFelderAlt = nullptr;
 }
 bFeldanderung = false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::FeldWert(void)
{
 char c37Zahl[37];
 ezEingabe->Schriftausrichtung((BYTE)vstSpalten[ucSpalteSelect].usFormat); ezEingabe->SelectEntfernen();
 if(pfnEingabeMaske){
	 if(bEinfugen || bFeldanderung) pfnEingabeMaske(this);
	 else ezEingabe->Zeichenmaske("");
 }

 switch(FeldTyp(vucFeldStatus[ucSpalteSelect])){
     case FT_LISTSTR    : if(bEinfugen || bFeldanderung || bAnsehen){
                            if(pfnEingabeListe) pfnEingabeListe(this);
												  }
												  ezEingabe->Text("Liste");
												  break;
     case FT_EMPTY      : FeldWert_String(FT_EMPTY); break;
     case FT_SHORTSTR   : FeldWert_String(FT_SHORTSTR); break;
		 case FT_LONGSTR    : FeldWert_String(FT_LONGSTR); break;
		 case FT_MEMOSTR    : FeldWert_String(FT_MEMOSTR); break;
     case FT_BOOL       : (*(bool*)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert ? ezEingabe->Text(vbBool_true) : ezEingabe->Text(vbBool_false));
			                    ezEingabe->SetzVerfugbar(false); 
			                    break;
     case FT_CHAR       : ezEingabe->Text(LONGtoCHAR(c37Zahl, *(PCHAR)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert)); break;
     case FT_BYTE       : ezEingabe->Text(ULONGtoCHAR(c37Zahl, *(PBYTE)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert)); break;
     case FT_SHORT      : ezEingabe->Text(LONGtoCHAR(c37Zahl, *(PSHORT)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert)); break;
     case FT_USHORT     : ezEingabe->Text(ULONGtoCHAR(c37Zahl, *(PUSHORT)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert)); break;
     case FT_LONG       : ezEingabe->Text(LONGtoCHAR(c37Zahl, *(PLONG)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert)); break;
     case FT_ULONG      : ezEingabe->Text(ULONGtoCHAR(c37Zahl, *(PULONG)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert)); break;
		 case FT_LONGLONG   : ezEingabe->Text(LONGLONGtoCHAR(c37Zahl, *(PLONGLONG)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert)); break;
		 case FT_FLOAT      : ezEingabe->Text(FLOAT_B10toCHAR(c37Zahl, *(PFLOAT)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert, KommaStellen(ucSpalteSelect))); break;
		 case FT_DOUBLE     : ezEingabe->Text(DOUBLE_B10toCHAR(c37Zahl, *(double*)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert, KommaStellen(ucSpalteSelect))); break;
     case FT_COMMA4     : FeldWert_Komma4(); break;
     case FT_COMMA4_80  : FeldWert_Komma4_80(); break;
		 case FT_DATE       : FeldWert_Datum(); break;
     case FT_TIME       : FeldWert_Zeit(); break;
		 case FT_BIT128     : ezEingabe->Zeichenvorgabe(ZV_BUCHSTABEN | ZV_ZIFFERN); ezEingabe->Zeichenmaske("XXXXXXXX'-'XXXX'-'XXXX'-'XXXX'-'XXXXXXXXXXXX");
			                    ezEingabe->Text(BIT128toGUID(c37Zahl, *(BIT128*)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert)); 
													break;
		 case FT_BIGIMAGE   :
		 case FT_SMALLIMAGE : if(bEinfugen || bFeldanderung || bAnsehen){
													  if(!vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange) biEingabe->Schreibe(nullptr, 0);
													  else{ biEingabe->Schreibe(&vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert[*(PBYTE)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert + 1],
														  vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange - *(PBYTE)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert - 1);
													  }
												  }
												  ezEingabe->Text("Bild");
												  break;
		 case FT_DOCUMENT   : if(bEinfugen || bFeldanderung || bAnsehen){
													  if(!vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange) doEingabe->Schreibe(nullptr, 0, nullptr, 0);
													  else{ doEingabe->Schreibe(&vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert[*(PBYTE)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert + 1],
														  vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange - *(PBYTE)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert - 1,
														  &vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert[1], *(PBYTE)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert);
													  }
												  }
												  ezEingabe->Text("Dokument");
												  break;
		 case FT_AUDIO      : if(bEinfugen || bFeldanderung || bAnsehen){
													  if(!vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange) maEingabe->Schreibe(nullptr, 0, nullptr, 0);
													  else{ maEingabe->Schreibe(&vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert[*(PBYTE)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert + 1],
														  vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange - *(PBYTE)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert - 1,
														  &vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert[1], *(PBYTE)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert);
													    maEingabe->Infotext("Audio verfügbar");
													  }
												  }
												  ezEingabe->Text("Audio");
												  break;
		 case FT_VIDEO      : if(bEinfugen || bFeldanderung || bAnsehen){
													  if(!vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange) mvEingabe->Schreibe(nullptr, 0, nullptr, 0);
													  else{ mvEingabe->Schreibe(&vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert[*(PBYTE)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert + 1],
														  vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange - *(PBYTE)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert - 1,
														  &vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert[1], *(PBYTE)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert);
													  }
												  }
												  ezEingabe->Text("Video");
												  break;
 }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::FeldWert_String(unsigned char ucFeldtyp)
{
 COStringA asFeldWert;
 switch(ucFeldtyp){
	 case FT_EMPTY      :
	 case FT_SHORTSTR   : if(!vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange) ezEingabe->Text(NULL);
	 										  else{ asFeldWert.SetLength(vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
													MemCopy(asFeldWert.c_Str(), vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert, vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
													ezEingabe->Text(asFeldWert.c_Str()); 
												}
												break;
	 case FT_LONGSTR    : if(bEinfugen || bFeldanderung || bAnsehen){
													if(!vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange) ebEingabe->Text(NULL);
													else{ 
														asFeldWert.SetLength(vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange - *(PBYTE)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert - 1);
														MemCopy(asFeldWert.c_Str(),
																		&vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert[*(PBYTE)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert + 1],
																		vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange - *(PBYTE)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert - 1);
														ebEingabe->Text(asFeldWert.c_Str());
													}
												}
												ezEingabe->Text("Text");
												break;
	 case FT_MEMOSTR    : if(bEinfugen || bFeldanderung || bAnsehen){
													if(!vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange) ebEingabe->Text(NULL);
													else{ asFeldWert.SetLength(vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
														MemCopy(asFeldWert.c_Str(), vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert, vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
														ebEingabe->Text(asFeldWert.c_Str());
													}
												}
												ezEingabe->Text("Memo");
												break;
 }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::FeldWert_Komma4(void)
{
 COComma4 k4FeldWert; char c20Zahl[20];
 k4FeldWert.Write(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert); ezEingabe->Text(Comma4toCHAR(c20Zahl, &k4FeldWert, KommaStellen(ucSpalteSelect)));
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::FeldWert_Komma4_80(void)
{
 COComma4_80 k4gFeldWert; char c32Zahl[32];
 k4gFeldWert.Write(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert); ezEingabe->Text(Comma4_80toCHAR(c32Zahl, &k4gFeldWert, KommaStellen(ucSpalteSelect)));
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::FeldWert_Zeit(void)
{
 COTime zFeldWert; COStringA asFeldWert; char c20Zahl[20];
 zFeldWert.Write(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert);
 if(!zFeldWert.FileTime().dwHighDateTime && !zFeldWert.FileTime().dwLowDateTime){
	 if(ezEingabe->Zeichenmaske(&asFeldWert)->Length()){ zFeldWert.Now(); ezEingabe->Text(zFeldWert.CHARDateTime(c20Zahl)); }
	 else ezEingabe->Text(NULL);
 }
 else if(vstSpalten[ucSpalteSelect].usFormat & TXF_ZEIT) ezEingabe->Text(zFeldWert.CHARTime(c20Zahl));
 else if(vstSpalten[ucSpalteSelect].usFormat & TXF_DATUM) ezEingabe->Text(zFeldWert.CHARDate(c20Zahl));
 else ezEingabe->Text(zFeldWert.CHARDateTime(c20Zahl));
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::FeldWert_Datum(void)
{
 if(bEinfugen || bFeldanderung) daEingabe->Schreibe(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert);
 else{ COTime zFeldWert; COStringA asFormat_Datum, asFormat_Zeit; char c20DatumZeit[20]; USHORT usFormat; VMBLOCK vbDatum;
	 zFeldWert.Write(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert);
	 usFormat = vstSpalten[ucSpalteSelect].usFormat; usFormat >>= 3; usFormat <<= 3;
	 switch(usFormat){
		 case TXF_ZEIT            : ezEingabe->Text(zFeldWert.CHARTime(c20DatumZeit));
																break;
		 case TXF_DATUM           : ezEingabe->Text(zFeldWert.CHARDate(c20DatumZeit));
																break;
		 case TXF_DATUMZEIT       : ezEingabe->Text(zFeldWert.CHARDateTime(c20DatumZeit));
																break;
		 case TXF_DATUMEXTERN     : if(pfnDatumZeitFormat){ pfnDatumZeitFormat(ucSpalteSelect, &asFormat_Datum, nullptr);
			                            ezEingabe->Text(zFeldWert.VMBLOCKDateFormat(vbDatum, asFormat_Datum.c_Str())); VMFrei(vbDatum); break; }
		 case TXF_DATUMINTERN     : ezEingabe->Text(zFeldWert.VMBLOCKDateFormat(vbDatum, daEingabe->Datumformat(&asFormat_Datum)->c_Str())); VMFrei(vbDatum);
																break;
		 case TXF_DATUMZEITEXTERN : if(pfnDatumZeitFormat){ pfnDatumZeitFormat(ucSpalteSelect, &asFormat_Datum, &asFormat_Zeit);
																	ezEingabe->Text(zFeldWert.VMBLOCKDateTimeFormat(vbDatum, asFormat_Datum.c_Str(), asFormat_Zeit.c_Str(), true)); VMFrei(vbDatum); break; }
		 case TXF_DATUMZEITINTERN : ezEingabe->Text(zFeldWert.VMBLOCKDateTimeFormat(vbDatum, daEingabe->Datumformat(&asFormat_Datum)->c_Str(),
																	daEingabe->Zeitformat(&asFormat_Zeit)->c_Str(), true)); VMFrei(vbDatum);
																break;
		 case TXF_ZEITDATUMEXTERN : if(pfnDatumZeitFormat){ pfnDatumZeitFormat(ucSpalteSelect, &asFormat_Datum, &asFormat_Zeit);
																	ezEingabe->Text(zFeldWert.VMBLOCKDateTimeFormat(vbDatum, asFormat_Datum.c_Str(), asFormat_Zeit.c_Str(), false)); VMFrei(vbDatum); break; }
		 case TXF_ZEITDATUMINTERN : ezEingabe->Text(zFeldWert.VMBLOCKDateTimeFormat(vbDatum, asFormat_Datum.c_Str(), asFormat_Zeit.c_Str(), false)); VMFrei(vbDatum);
																break;
	 }
 }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COTabBasis::AndernWert(bool bEingabe)
{
 BYTE ucFeldtyp_SpalteSelect = FeldTyp(vucFeldStatus[ucSpalteSelect]);
 if(bEingabe){
	 switch(ucFeldtyp_SpalteSelect){
	 		 case FT_LONGSTR    : ebEingabe->Inhalt(vasNeuerWert);
													  stDateifeld.ulBytes = vasNeuerWert->Length();
													  stDateifeld.vbDaten = VMBlock(stDateifeld.ulBytes);
													  MemCopy(stDateifeld.vbDaten, vasNeuerWert->c_Str(), stDateifeld.ulBytes);
													  *vasNeuerWert = "txt"; break;
			 case FT_MEMOSTR    : ebEingabe->Inhalt(vasNeuerWert); break;
			 case FT_LISTSTR    : lbEingabe->SelectEintrag(vasNeuerWert); break;
			 case FT_BIGIMAGE   : 
			 case FT_SMALLIMAGE : biEingabe->Lese(stDateifeld.vbDaten, stDateifeld.ulBytes, vasNeuerWert); break;
			 case FT_DOCUMENT   : doEingabe->Lese(stDateifeld.vbDaten, stDateifeld.ulBytes, vasNeuerWert); break;
			 case FT_AUDIO      : maEingabe->Lese(stDateifeld.vbDaten, stDateifeld.ulBytes, vasNeuerWert); break;
			 case FT_VIDEO      : mvEingabe->Lese(stDateifeld.vbDaten, stDateifeld.ulBytes, vasNeuerWert); break;
			 case FT_DATE       : daEingabe->Datum(vasNeuerWert); break;
			 case FT_BOOL       : (*ezEingabe->Inhalt(vasNeuerWert) == vbBool_true ? *vasNeuerWert = "1" : *vasNeuerWert = "0"); break;
							 default    : ezEingabe->Inhalt(vasNeuerWert);
	 }
 }

 bool bPrufen = true;
 switch(ucFeldtyp_SpalteSelect){
		 case FT_BOOL       : bPrufen = vasNeuerWert->IsIntegralNumber(); break;
		 case FT_CHAR       : bPrufen = vasNeuerWert->IsIntegralNumber(); break;
		 case FT_BYTE       : bPrufen = vasNeuerWert->IsIntegralNumber(); break;
		 case FT_SHORT      : bPrufen = vasNeuerWert->IsIntegralNumber(); break;
		 case FT_USHORT     : bPrufen = vasNeuerWert->IsIntegralNumber(); break;
		 case FT_LONG       : bPrufen = vasNeuerWert->IsIntegralNumber(); break;
		 case FT_ULONG      : bPrufen = vasNeuerWert->IsIntegralNumber(); break;
		 case FT_LONGLONG   : bPrufen = vasNeuerWert->IsIntegralNumber(); break;
		 case FT_FLOAT      : bPrufen = vasNeuerWert->IsFloatingPointNumber(); break;
		 case FT_DOUBLE     : bPrufen = vasNeuerWert->IsFloatingPointNumber(); break;
		 case FT_COMMA4     : bPrufen = vasNeuerWert->IsFloatingPointNumber(); break;
		 case FT_COMMA4_80  : bPrufen = vasNeuerWert->IsFloatingPointNumber(); break;
		 case FT_MEMOSTR    : (vasNeuerWert->Length() > ebEingabe->Zeichen_Max() ? bPrufen = false : bPrufen = true); break;
		 case FT_SMALLIMAGE : (stDateifeld.ulBytes > 65511 - vasNeuerWert->Length() ? bPrufen = false : bPrufen = true); break;
 }
 if(!bPrufen){ COStringA asMeldung; char c11Zahl[11];
	 switch(ucFeldtyp_SpalteSelect){
				 case FT_MEMOSTR    : asMeldung = "Das Textfeld enthält mehr als die möglichen ";
														  asMeldung += ULONGtoCHAR(c11Zahl, ebEingabe->Zeichen_Max()); asMeldung += " Zeichen."; break;
				 case FT_SMALLIMAGE : asMeldung = "Das Kleinbild ist grösser als die möglichen 64 kByte."; break;
				 default            : asMeldung = "'"; asMeldung += *vasNeuerWert + "'" + " ist keine gültige Zahl.";
	 }
	 FehlerMeldung(&asMeldung);	return false;
 }
 else{
	 if(pfnPrufenWert){ COStringA asMeldung;
		 if(!pfnPrufenWert(this, &asMeldung)){ FehlerMeldung(&asMeldung); return false; }
	 }

	 if(bFeldanderung && !vstFelderAlt){ vstFelderAlt = DSErstellen();
	   for(BYTE ucSpalte = 0; ucSpalte < ucSpaltenanzahl + ucZusatzspalten; ucSpalte++){
        switch(FeldTyp(vucFeldStatus[ucSpalte])){
					 case FT_VIDEO      : 	
					 case FT_AUDIO      :
					 case FT_DOCUMENT   :
					 case FT_BIGIMAGE   :
					 case FT_LONGSTR    : 
					 case FT_EMPTY      : 
					 case FT_SMALLIMAGE :
					 case FT_LISTSTR    :
					 case FT_MEMOSTR    : 
					 case FT_SHORTSTR   : vstFelderAlt[ucSpalte].ulLange = vpTabelle[ulZeileSelect][ucSpalte].ulLange;
													      vstFelderAlt[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelderAlt[ucSpalte].ulLange);               
					 default            : MemCopy(vstFelderAlt[ucSpalte].vbWert, vpTabelle[ulZeileSelect][ucSpalte].vbWert, vstFelderAlt[ucSpalte].ulLange);
			 }
		 }
	 }

	 char cTemp; BYTE ucTemp; short sTemp; USHORT usTemp; long lTemp; ULONG ulTemp; float fTemp; double dTemp; long long llTemp; BIT128 bit128Temp;
	 switch(ucFeldtyp_SpalteSelect){
			 case FT_EMPTY      : break;
			 case FT_LISTSTR    : if(StrCompare(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert, vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange,
																					 vasNeuerWert->c_Str(), vasNeuerWert->Length())){
														  if(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert) VMFreiS(vmTabelle, vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert);
														  vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange = vasNeuerWert->Length();
														  vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert = VMBlockS(vmTabelle, vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
														  MemCopy(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert, vasNeuerWert->c_Str(), vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
														  if(vucDBTabellenSpalte){ 
															  if(pfnAndernWert_Zusatz && vucDBTabellenSpalte[ucSpalteSelect] != ucSpalteSelect){
																  pfnAndernWert_Zusatz(lbEingabe->SelectIndex(), vasNeuerWert);
																  ucTemp = ucSpalteSelect;
																  ucSpalteSelect = vucDBTabellenSpalte[vucDBTabellenSpalte[ucSpalteSelect]];
																  AndernWert(false);
																  ucSpalteSelect = ucTemp;
															  }
															  if(bFeldanderung) vbFeldGeandert[vucDBTabellenSpalte[vucDBTabellenSpalte[ucSpalteSelect]]] = true;
														  }
													  }
													  if(vucDBTabellenSpalte && bEinfugen && pfnAndernWert_Zusatz && vucDBTabellenSpalte[ucSpalteSelect] != ucSpalteSelect){ 
														  pfnAndernWert_Zusatz(lbEingabe->SelectIndex(), vasNeuerWert);
														  ucTemp = ucSpalteSelect;
														  ucSpalteSelect = vucDBTabellenSpalte[vucDBTabellenSpalte[ucSpalteSelect]];
														  AndernWert(false);
														  ucSpalteSelect = ucTemp;
													  }
													  break;
			 case FT_MEMOSTR    :
			 case FT_SHORTSTR   : if(StrCompare(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert, vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange,
																					 vasNeuerWert->c_Str(), vasNeuerWert->Length())){
														  if(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert) VMFreiS(vmTabelle, vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert);
														  vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange = vasNeuerWert->Length();
														  vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert = VMBlockS(vmTabelle, vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
														  MemCopy(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert, vasNeuerWert->c_Str(), vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
														  if(vucDBTabellenSpalte && bFeldanderung) vbFeldGeandert[vucDBTabellenSpalte[vucDBTabellenSpalte[ucSpalteSelect]]] = true;
													  }
													  break;
			 case FT_BOOL       : vasNeuerWert->CHAR(cTemp);
													  if(cTemp != *(PCHAR)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert){
														  MemCopy(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert, &cTemp, vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
														  if(vucDBTabellenSpalte && bFeldanderung) vbFeldGeandert[vucDBTabellenSpalte[vucDBTabellenSpalte[ucSpalteSelect]]] = true;
													  } break;
			 case FT_CHAR       : vasNeuerWert->CHAR(cTemp);
													  if(cTemp != *(PCHAR)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert){
														  MemCopy(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert, &cTemp, vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
														  if(vucDBTabellenSpalte && bFeldanderung) vbFeldGeandert[vucDBTabellenSpalte[vucDBTabellenSpalte[ucSpalteSelect]]] = true;
													  } break;
			 case FT_BYTE       : vasNeuerWert->BYTE(ucTemp);
													  if(ucTemp != *(PBYTE)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert){
														  MemCopy(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert, &ucTemp, vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
														  if(vucDBTabellenSpalte && bFeldanderung) vbFeldGeandert[vucDBTabellenSpalte[vucDBTabellenSpalte[ucSpalteSelect]]] = true;
													  } break;
			 case FT_SHORT      : vasNeuerWert->SHORT(sTemp);
													  if(sTemp != *(PSHORT)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert){
														  MemCopy(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert, &sTemp, vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
														  if(vucDBTabellenSpalte && bFeldanderung) vbFeldGeandert[vucDBTabellenSpalte[vucDBTabellenSpalte[ucSpalteSelect]]] = true;
													  } break;
			 case FT_USHORT     : vasNeuerWert->USHORT(usTemp);
													  if(usTemp != *(PUSHORT)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert){
														  MemCopy(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert, &usTemp, vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
														  if(vucDBTabellenSpalte && bFeldanderung) vbFeldGeandert[vucDBTabellenSpalte[vucDBTabellenSpalte[ucSpalteSelect]]] = true;
													  } break;
			 case FT_LONG       : vasNeuerWert->LONG(lTemp);
													  if(lTemp != *(PLONG)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert){
														  MemCopy(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert, &lTemp, vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
														  if(vucDBTabellenSpalte && bFeldanderung) vbFeldGeandert[vucDBTabellenSpalte[vucDBTabellenSpalte[ucSpalteSelect]]] = true;
													  } break;
			 case FT_ULONG      : vasNeuerWert->ULONG(ulTemp);
													  if(ulTemp != *(PULONG)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert){
														  MemCopy(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert, &ulTemp, vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
														  if(vucDBTabellenSpalte && bFeldanderung) vbFeldGeandert[vucDBTabellenSpalte[vucDBTabellenSpalte[ucSpalteSelect]]] = true;
													  } break;
			 case FT_LONGLONG   : vasNeuerWert->LONGLONG(llTemp);
													  if(llTemp != *(PLONGLONG)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert){
														  MemCopy(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert, &llTemp, vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
														  if(vucDBTabellenSpalte && bFeldanderung) vbFeldGeandert[vucDBTabellenSpalte[vucDBTabellenSpalte[ucSpalteSelect]]] = true;
													  } break;
			 case FT_FLOAT      : vasNeuerWert->FLOAT(fTemp);
													  if(fTemp != *(PFLOAT)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert){
														  MemCopy(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert, &fTemp, vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
														  if(vucDBTabellenSpalte && bFeldanderung) vbFeldGeandert[vucDBTabellenSpalte[vucDBTabellenSpalte[ucSpalteSelect]]] = true;
													  } break;
			 case FT_DOUBLE     : vasNeuerWert->DOUBLE(dTemp);
													  if(dTemp != *(double*)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert){
														  MemCopy(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert, &dTemp, vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
														  if(vucDBTabellenSpalte && bFeldanderung) vbFeldGeandert[vucDBTabellenSpalte[vucDBTabellenSpalte[ucSpalteSelect]]] = true;
													  } break;
			 case FT_BIT128     : vasNeuerWert->BIT128fromGUID(bit128Temp);
														if(BIT128Compare(bit128Temp, *(BIT128*)vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert)){
															MemCopy(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert, bit128Temp, vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
															if(vucDBTabellenSpalte && bFeldanderung) vbFeldGeandert[vucDBTabellenSpalte[vucDBTabellenSpalte[ucSpalteSelect]]] = true;
														} break;
			 case FT_COMMA4     : AndernWert_Komma4(); break;
			 case FT_COMMA4_80  : AndernWert_Komma4_80(); break;
			 case FT_DATE       : AndernWert_Datum(); break;
			 case FT_TIME       : AndernWert_Zeit(); break;
			 case FT_LONGSTR    : AndernWert_Datei(); break;
			 case FT_DOCUMENT   : if(doEingabe->Geandert() || !bEingabe) AndernWert_Datei(); break;
			 case FT_BIGIMAGE   : 
			 case FT_SMALLIMAGE : if(biEingabe->Geandert() || !bEingabe) AndernWert_Datei(); break;
			 case FT_AUDIO      : if(maEingabe->Geandert() || !bEingabe) AndernWert_Datei(); break;
			 case FT_VIDEO      : if(mvEingabe->Geandert() || !bEingabe) AndernWert_Datei(); break;
	 }
	 if(pfnLeerFeld) pfnLeerFeld(this);
 }
 return true;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::AndernWert_Komma4(void)
{
 COComma4 k4FeldWert_1, k4FeldWert_2;
 vasNeuerWert->COMMA4(&k4FeldWert_1); k4FeldWert_2.Write(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert);
 if(k4FeldWert_1.Compare(&k4FeldWert_2)){
   k4FeldWert_1.Read(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert);
	 if(vucDBTabellenSpalte && bFeldanderung) vbFeldGeandert[vucDBTabellenSpalte[vucDBTabellenSpalte[ucSpalteSelect]]] = true;
 }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::AndernWert_Komma4_80(void)
{
 COComma4_80 k4gFeldWert_1, k4gFeldWert_2;
 vasNeuerWert->COMMA4_80(&k4gFeldWert_1); k4gFeldWert_2.Write(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert);
 if(k4gFeldWert_1.Compare(&k4gFeldWert_2)){
   k4gFeldWert_1.Read(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert);
	 if(vucDBTabellenSpalte && bFeldanderung) vbFeldGeandert[vucDBTabellenSpalte[vucDBTabellenSpalte[ucSpalteSelect]]] = true;
 }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::AndernWert_Zeit(void)
{
 COTime zFeldWert_1, zFeldWert_2;
 zFeldWert_1 = vasNeuerWert->c_Str(); zFeldWert_2.Write(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert);
 if(CompareFileTime(&zFeldWert_1.FileTime(), &zFeldWert_2.FileTime())){
   zFeldWert_1.Read(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert);
	 if(vucDBTabellenSpalte && bFeldanderung) vbFeldGeandert[vucDBTabellenSpalte[vucDBTabellenSpalte[ucSpalteSelect]]] = true;
 }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::AndernWert_Datum(void)
{
 COTime zFeldWert_1, zFeldWert_2;
 zFeldWert_1 = daEingabe->SystemTime(); zFeldWert_2.Write(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert);
 if(CompareFileTime(&zFeldWert_1.FileTime(), &zFeldWert_2.FileTime())){
   zFeldWert_1.Read(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert);
	 if(vucDBTabellenSpalte && bFeldanderung) vbFeldGeandert[vucDBTabellenSpalte[vucDBTabellenSpalte[ucSpalteSelect]]] = true;
 }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::AndernWert_Datei(void)
{
 if(stDateifeld.ulBytes){ BYTE ucTemp;
	 if(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert) VMFreiS(vmTabelle, vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert);
	 vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange = stDateifeld.ulBytes + vasNeuerWert->Length() + 1;
	 vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert = VMBlockS(vmTabelle, vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
	 ucTemp = (BYTE)vasNeuerWert->Length();
	 MemCopy(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert, &ucTemp, BY_BYTE);
	 MemCopy(&vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert[1], vasNeuerWert->c_Str(), ucTemp);
	 MemCopy(&vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert[ucTemp + 1], stDateifeld.vbDaten, stDateifeld.ulBytes);
   VMFrei(stDateifeld.vbDaten); stDateifeld.vbDaten = nullptr; stDateifeld.ulBytes = 0;
	 if(vucDBTabellenSpalte && bFeldanderung) vbFeldGeandert[vucDBTabellenSpalte[vucDBTabellenSpalte[ucSpalteSelect]]] = true;
 }
 else if(vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange){
   vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange = stDateifeld.ulBytes;
	 VMFreiS(vmTabelle, vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert); vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert = nullptr;
	 if(vucDBTabellenSpalte && bFeldanderung) vbFeldGeandert[vucDBTabellenSpalte[vucDBTabellenSpalte[ucSpalteSelect]]] = true;
 }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::FehlerMeldung(COStringA* vasMeldung)
{
 *vasMeldung += "\nDas Feld wird wieder auf seinen vorherigen Wert zurück gesetzt.";
 MessageBox(hWndElement, vasMeldung->c_Str(), "Falscher Wert", MB_ICONERROR); 
 VMFreiV(vasMeldung);	
 if(stDateifeld.vbDaten){ VMFrei(stDateifeld.vbDaten); stDateifeld.vbDaten = NULL; stDateifeld.ulBytes = 0; }
 FeldWert(); 
 if(ezEingabe->Sichtbar()){ SetFocus(ezEingabe->HWND_Element()); ezEingabe->SetzVerfugbar(true); }
 else if(ebEingabe->Sichtbar()){ SetFocus(ebEingabe->HWND_Element()); ebEingabe->SetzVerfugbar(true); }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::DSLoschen(void)
{
 for(BYTE ucSpalte = 0; ucSpalte < ucSpaltenanzahl + ucZusatzspalten; ucSpalte++) VMFreiS(vmTabelle, vpTabelle[ulZeileSelect][ucSpalte].vbWert);
 VMFreiS(vmTabelle, vpTabelle[ulZeileSelect]);

 for(ULONG ulDS = ulZeileSelect; ulDS < ulDSAnzahl - 1; ulDS++) vpTabelle[ulDS] = vpTabelle[ulDS + 1];
 ulDSAnzahl--;
 vpTabelle[ulDSAnzahl] = nullptr;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::DSEinfugen(void)
{
 COStringA asFeldWert; COTime zFeldWert; zFeldWert.Now(); COComma4 k4FeldWert; COComma4_80 k4gFeldWert; char c10Temp[10];

 STFeld* vstFelder = DSErstellen();
 DSAnfugen(vstFelder);

 bSelectiert = true;
 ulZeileSelect = ulDSAnzahl - 1;

 for(BYTE ucSpalte = 0; ucSpalte < ucSpaltenanzahl; ucSpalte++){
		switch(FeldTyp(vucFeldStatus[ucSpalte])){
			 case FT_EMPTY      : if(pfnLeerFeld){ ucSpalteSelect = ucSpalte; pfnLeerFeld(this); }
													  else{ asFeldWert = "Leer";
													   vstFelder[ucSpalte].ulLange = asFeldWert.Length();
														 vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange);
                             MemCopy(vstFelder[ucSpalte].vbWert, asFeldWert.c_Str(), vstFelder[ucSpalte].ulLange);
													 } break;
			 case FT_LISTSTR   : if(pfnEingabeListe){ ucSpalteSelect = ucSpalte; pfnEingabeListe(this); }
 													 else{ asFeldWert = "Keine Liste";
													   vstFelder[ucSpalte].ulLange = asFeldWert.Length();
														 vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange);
                             MemCopy(vstFelder[ucSpalte].vbWert, asFeldWert.c_Str(), vstFelder[ucSpalte].ulLange);
													 } break;
			 case FT_COMMA4    : k4FeldWert.Read(c10Temp); MemCopy(vstFelder[ucSpalte].vbWert, c10Temp, vstFelder[ucSpalte].ulLange); break;
			 case FT_COMMA4_80 : k4gFeldWert.Read(c10Temp); MemCopy(vstFelder[ucSpalte].vbWert, c10Temp, vstFelder[ucSpalte].ulLange); break;
			 case FT_DATE      : 
			 case FT_TIME      : zFeldWert.Read(c10Temp); MemCopy(vstFelder[ucSpalte].vbWert, c10Temp, vstFelder[ucSpalte].ulLange); break;
		}
 }

 ucSpalteSelect = 0;
 FeldWert();

 SCROLLINFO siZeile, siSpalte; siZeile.cbSize = siSpalte.cbSize = sizeof(SCROLLINFO); siZeile.fMask = siSpalte.fMask = SIF_ALL;
 SetzScrollVert(siZeile);

 GetScrollInfo(hWndElement, SB_HORZ, &siSpalte);
 while(siSpalte.nPos){ SendMessage(hWndElement, WM_HSCROLL, SB_PAGELEFT, NULL); GetScrollInfo(hWndElement, SB_HORZ, &siSpalte); }

 GetScrollInfo(hWndElement, SB_VERT, &siZeile);
 while(siZeile.nMax > siZeile.nPos + (int)siZeile.nPage){ SendMessage(hWndElement, WM_VSCROLL, SB_PAGEDOWN, NULL); GetScrollInfo(hWndElement, SB_VERT, &siZeile); }
 SendMessage(hWndElement, WM_VSCROLL, SB_LINEDOWN, NULL);

 RECT rcZeichnen;
 rcZeichnen.left = 0; rcZeichnen.right = lRand_rechts;
 rcZeichnen.top = (siZeile.nPage - 2) * ucZeilenhohe + ucKopfhohe; rcZeichnen.bottom = rcZeichnen.top + ucZeilenhohe; 
 UpdateFenster(&rcZeichnen, true, false);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::Einfugen(void)
{
 ThreadSicher_Anfang();
 if(hWndElement && ucRechte & ZR_EINFUGEN){ 
	 if(bEinfugen || bFeldanderung) Senden();
	 bEinfugen = true;													 
	 DSEinfugen();
	 Eingabe_SichtbarFalse();
 }
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::Loschen(unsigned long ulZeile)
{
 ThreadSicher_Anfang();
 if(hWndElement && ulZeile < ulDSAnzahl){
	 if(bEinfugen || bFeldanderung) Senden();
	 Eingabe_SichtbarFalse();	
	 bAktiv = true; bSelectiert = true; ulZeileSelect = ulZeile;
   WM_Command(IDM_DS_LOSCHEN);
 }
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::Andern(COStringA* vasFeldwert, unsigned long ulZeile, unsigned char ucSpalte)
{
 ThreadSicher_Anfang();
 Andern(vasFeldwert->c_Str(), ulZeile, ucSpalte);	
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::Andern(char* pcFeldwert, unsigned long ulZeile, unsigned char ucSpalte)
{
 ThreadSicher_Anfang();
 if(hWndElement && ulZeile < ulDSAnzahl && ucSpalte < ucSpaltenanzahl && ucRechte & ZR_ANDERN){
   if(ulZeile != ulZeileSelect && (bEinfugen || bFeldanderung)) Senden();
	 Eingabe_SichtbarFalse();
	 if(!bEinfugen && !bFeldanderung){ bSelectiert = true; bFeldanderung = true; }
	 ulZeileSelect = ulZeile; ucSpalteSelect = ucSpalte;
	 *vasNeuerWert = pcFeldwert;

	 if(FeldTyp(vucFeldStatus[ucSpalte]) == FT_LONGSTR){
	 	 stDateifeld.ulBytes = vasNeuerWert->Length();
		 stDateifeld.vbDaten = VMBlock(stDateifeld.ulBytes);
		 MemCopy(stDateifeld.vbDaten, vasNeuerWert->c_Str(), stDateifeld.ulBytes);
		 *vasNeuerWert = "txt";
	 }
   AndernWert_Zeichnen(ulZeile, ucSpalte);
 }	
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::Andern(COStream* psmDatei, char* pcDateiendung, unsigned long ulZeile, unsigned char ucSpalte)
{
 ThreadSicher_Anfang();
 if(hWndElement && ulZeile < ulDSAnzahl && ucSpalte < ucSpaltenanzahl && ucRechte & ZR_ANDERN){
   if(ulZeile != ulZeileSelect && (bEinfugen || bFeldanderung)) Senden();
	 Eingabe_SichtbarFalse();
	 if(!bEinfugen){ bSelectiert = true; bFeldanderung = true; }
	 ulZeileSelect = ulZeile; ucSpalteSelect = ucSpalte;

   switch(FeldTyp(vucFeldStatus[ucSpalte])){	
				case FT_AUDIO      :
				case FT_VIDEO      :
			  case FT_BIGIMAGE   :
			  case FT_SMALLIMAGE : 
				case FT_DOCUMENT   : psmDatei->Data(stDateifeld.vbDaten); stDateifeld.ulBytes = psmDatei->Bytes(); *vasNeuerWert = pcDateiendung;
					                   if(vasNeuerWert->SearchCharacters(".")) vasNeuerWert->ShortLeft(1);
	 }
   AndernWert_Zeichnen(ulZeile, ucSpalte);
 }
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::Andern(VMBLOCK vbDaten, unsigned long ulBytes, char* pcDateiendung, unsigned long ulZeile, unsigned char ucSpalte)
{
 ThreadSicher_Anfang();
 if(hWndElement && ulZeile < ulDSAnzahl && ucSpalte < ucSpaltenanzahl && ucRechte & ZR_ANDERN){
   if(ulZeile != ulZeileSelect && (bEinfugen || bFeldanderung)) Senden();
   Eingabe_SichtbarFalse();
	 if(!bEinfugen){ bSelectiert = true; bFeldanderung = true; }
	 ulZeileSelect = ulZeile; ucSpalteSelect = ucSpalte;

   switch(FeldTyp(vucFeldStatus[ucSpalte])){	
				case FT_AUDIO      :
				case FT_VIDEO      :
			  case FT_BIGIMAGE   :
			  case FT_SMALLIMAGE : 
				case FT_DOCUMENT   : stDateifeld.vbDaten = VMBlock(ulBytes); stDateifeld.ulBytes = ulBytes;
					                   MemCopy(stDateifeld.vbDaten, vbDaten, ulBytes); *vasNeuerWert = pcDateiendung;
														 if(vasNeuerWert->SearchCharacters(".")) vasNeuerWert->ShortLeft(1);
	 }
   AndernWert_Zeichnen(ulZeile, ucSpalte);
 }
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::Senden(void)
{
 ThreadSicher_Anfang();
 if(hWndElement && ucRechte & ZR_SENDEN && (bEinfugen || bFeldanderung)){
	 if(ezEingabe->Sichtbar() || ebEingabe->Sichtbar() || lbEingabe->Sichtbar() || biEingabe->Sichtbar() || doEingabe->Sichtbar()) WM_Command(IDM_DS_SENDEN);
	 else{
		 if(pfnDSSenden && !pfnDSSenden(this)){
			 if(bEinfugen) EinfugenRollback();
			 else if(bFeldanderung) AndernRollback();
		 }
		 if(bEinfugen) LoschEinfugen();
		 else if(bFeldanderung) LoschAndern();
		 bSelectiert = false; bAktiv = false;
	 }
 }
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::Eingabe_SichtbarFalse(void)
{
 bAnsehen = false;
 if(ezEingabe->Sichtbar()){ ezEingabe->SetzSichtbar(false); ezEingabe->SetzVerfugbar(false); } 
 else if(ebEingabe->Sichtbar()){ ebEingabe->SetzSichtbar(false); ebEingabe->SetzVerfugbar(false); ebEingabe->Zeichenvorgabe(ZV_ALLE); }
 else if(lbEingabe->Sichtbar()){ lbEingabe->SetzSichtbar(false); lbEingabe->SetzVerfugbar(false); }
 else if(biEingabe->Sichtbar()){ biEingabe->SetzSichtbar(false); biEingabe->SetzVerfugbar(false); }
 else if(doEingabe->Sichtbar()){ doEingabe->SetzSichtbar(false); doEingabe->SetzVerfugbar(false); }
 else if(maEingabe->Sichtbar()){ maEingabe->SetzSichtbar(false); maEingabe->SetzVerfugbar(false); }
 else if(mvEingabe->Sichtbar()){ mvEingabe->SetzSichtbar(false); mvEingabe->SetzVerfugbar(false); }
 else if(daEingabe->Sichtbar()){ daEingabe->SetzSichtbar(false); daEingabe->SetzVerfugbar(false); }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::AndernWert_Zeichnen(unsigned long& ulZeile, unsigned char& ucSpalte)
{
 if(AndernWert(false)){ FeldWert();
	 RECT rcZeichnen; rcZeichnen.top = ulZeile * ucZeilenhohe + ucKopfhohe; rcZeichnen.bottom = rcZeichnen.top + ucZeilenhohe;
	 sgKopf->Schalter_Pos_X(ucSpalte, rcZeichnen.left); rcZeichnen.right = rcZeichnen.left + vstSpalten[ucSpalte].lBreite;
	 UpdateFenster(&rcZeichnen, true, false);
 }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::DateiOffnen(void)
{
 OPENFILENAME stOffnenDatei; char c256Dateiname[256]; c256Dateiname[0] = 0; static char c256Filter[256];

 stOffnenDatei.lStructSize = sizeof(OPENFILENAME);
 stOffnenDatei.hwndOwner = hWndElement;
 stOffnenDatei.lpstrDefExt = "tdd"; stOffnenDatei.nFilterIndex = 0;
 stOffnenDatei.lpstrFilter = "Tabelle\0*.tdd\0";
 stOffnenDatei.lpstrCustomFilter = c256Filter; stOffnenDatei.nMaxCustFilter = 256;
 stOffnenDatei.lpstrFile = c256Dateiname; stOffnenDatei.nMaxFile = 256;
 stOffnenDatei.lpstrFileTitle = nullptr; stOffnenDatei.lpstrInitialDir = nullptr; stOffnenDatei.lpstrTitle = nullptr;
 stOffnenDatei.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING | OFN_EXTENSIONDIFFERENT | OFN_FORCESHOWHIDDEN | OFN_HIDEREADONLY;

 if(GetOpenFileName(&stOffnenDatei)) DateiLesen(c256Dateiname);
 else if(CommDlgExtendedError()) MessageBox(hWndElement, pcKeinOffnen, pcDateiOffnen, MB_ICONHAND);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COTabBasis::DateiLesen(char* pcDateiname)
{ 
 ThreadSicher_Anfang();
 HANDLE hDatei = CreateFile(pcDateiname, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
 if(hDatei == INVALID_HANDLE_VALUE) MessageBox(hWndElement, pcKeinOffnen, pcDateiOffnen, MB_ICONHAND);
 else{ COStream smDatei(false); BYTE ucSpalte;
   smDatei.ReadFile(hDatei, true); CloseHandle(hDatei);

	 NeueTabelle();

	 smDatei.SetPosition(0, STM_POS_ANFANG);
   smDatei.Read(&ucSpalte, BY_BYTE);
	 if(ucSpalte != ucSpaltenanzahl){ MessageBox(hWndElement, pcFalscheSpalten, pcFalscheDatei, MB_ICONINFORMATION); goto Ende; }
	 smDatei.Read(&ucSpalte, BY_BYTE);
   if(ucSpalte != ucZusatzspalten){ MessageBox(hWndElement, pcFalscheSpalten, pcFalscheDatei, MB_ICONINFORMATION); goto Ende; }
	 unsigned char* vucFeldStatus_Kontrolle = (PBYTE)VMBlock(ucSpaltenanzahl + ucZusatzspalten);
   smDatei.Read(vucFeldStatus_Kontrolle, ucSpaltenanzahl + ucZusatzspalten);
	 for(ucSpalte = 0; ucSpalte < ucSpaltenanzahl + ucZusatzspalten; ucSpalte++){
		 if(vucFeldStatus[ucSpalte] != vucFeldStatus_Kontrolle[ucSpalte]){ 
			 VMFrei(vucFeldStatus_Kontrolle); MessageBox(hWndElement, pcFalscheSpalten, pcFalscheDatei, MB_ICONINFORMATION); goto Ende; }
	 }
	 VMFrei(vucFeldStatus_Kontrolle);

	 BYTE ucLange;
	 while(!smDatei.End()){
		 STFeld* vstFelder = DSErstellen(); 
		 for(ucSpalte = 0; ucSpalte < ucSpaltenanzahl + ucZusatzspalten; ucSpalte++){
			 switch(vucFeldStatus[ucSpalte]){
					 case FT_EMPTY      : break;
					 case FT_LISTSTR    :
					 case FT_SHORTSTR   : smDatei.Read(&ucLange, BY_BYTE); vstFelder[ucSpalte].ulLange = ucLange;
															  vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange);
															  smDatei.Read(vstFelder[ucSpalte].vbWert, vstFelder[ucSpalte].ulLange);
															  break;
					 case FT_SMALLIMAGE :
					 case FT_MEMOSTR    : smDatei.Read(&vstFelder[ucSpalte].ulLange, BY_USHORT);
															  vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange);
															  smDatei.Read(vstFelder[ucSpalte].vbWert, vstFelder[ucSpalte].ulLange);
															  break;
					 case FT_AUDIO      :
					 case FT_VIDEO      :
					 case FT_DOCUMENT   :
					 case FT_BIGIMAGE   :
					 case FT_LONGSTR    : smDatei.Read(&vstFelder[ucSpalte].ulLange, BY_ULONG);
															  vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange);
					 case FT_BOOL       : 
					 case FT_CHAR       : 
					 case FT_BYTE       : 
					 case FT_SHORT      : 
					 case FT_USHORT     : 
					 case FT_LONG       : 
					 case FT_ULONG      : 
					 case FT_LONGLONG   :  
					 case FT_FLOAT      : 
					 case FT_DOUBLE     : 
					 case FT_COMMA4     : 
					 case FT_COMMA4_80  : 
					 case FT_DATE       :
					 case FT_TIME       : 
					 case FT_BIT128     : smDatei.Read(vstFelder[ucSpalte].vbWert, vstFelder[ucSpalte].ulLange);
			 }
		 }
	 	 DSAnfugen(vstFelder);
	 }  
	 NeueTabelle_Zeichnen();
	 *vasDateiname = pcDateiname;
	 ThreadSicher_Ende(); return true;
 }

Ende:
 ThreadSicher_Ende(); return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::DateiSpeichernUnter(void)
{
 OPENFILENAME stSpeichernDatei; char c256Dateiname[256]; c256Dateiname[0] = 0; static char c256Filter[256];

 stSpeichernDatei.lStructSize = sizeof(OPENFILENAME);
 stSpeichernDatei.hwndOwner = hWndElement;
 stSpeichernDatei.lpstrDefExt = "tdd"; stSpeichernDatei.nFilterIndex = 0;
 stSpeichernDatei.lpstrFilter = "Tabelle\0*.tdd\0Text\0*.txt\0";
 stSpeichernDatei.lpstrCustomFilter = c256Filter; stSpeichernDatei.nMaxCustFilter = 256;
 stSpeichernDatei.lpstrFile = c256Dateiname; stSpeichernDatei.nMaxFile = 256;
 stSpeichernDatei.lpstrFileTitle = nullptr; stSpeichernDatei.lpstrInitialDir = nullptr; stSpeichernDatei.lpstrTitle = nullptr;
 stSpeichernDatei.Flags = OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING | OFN_EXTENSIONDIFFERENT | OFN_FORCESHOWHIDDEN | OFN_HIDEREADONLY;

 if(GetSaveFileName(&stSpeichernDatei)){ *vasDateiname = c256Dateiname; DateiSpeichern(); }
 else if(CommDlgExtendedError()) MessageBox(hWndElement, pcKeinSpeichern, pcDateiSpeichern, MB_ICONHAND);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::DateiSpeichern(void)
{
 if(!vasDateiname->Length()){ DateiSpeichernUnter(); return;}

 COStringA asEndung;
 vasDateiname->SubString(&asEndung, vasDateiname->SearchCharacters(".") + 1, vasDateiname->Length());
 if(asEndung == "tdd"){
	 if(!DateiSchreiben(vasDateiname->c_Str())) MessageBox(hWndElement, pcKeinSpeichern, pcDateiSpeichern, MB_ICONHAND);
 }
 else{ HANDLE hDatei; 
	 if(hDatei = CreateFile(vasDateiname->c_Str(), GENERIC_WRITE, NULL, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr)){ 
		 COStream smDatei(false); BYTE ucSpalte; char c37Zahl[37]; char cSemikolon = 0x3b, cZeilenumbruch = 0x0a, cText = 0x22;
		 COStringA asWert; COComma4 k4Zahl; COComma4_80 k4gZahl; COTime zZeit;
		 for(ULONG ulZeile = 0; ulZeile < ulDSAnzahl; ulZeile++){
			 for(ucSpalte = 0; ucSpalte < ucSpaltenanzahl; ucSpalte++){
					switch(FeldTyp(vucFeldStatus[ucSpalte])){
							case FT_EMPTY      :
							case FT_LISTSTR    :
							case FT_MEMOSTR    : 
							case FT_LONGSTR    :
							case FT_SHORTSTR   : smDatei.Write(&cText, BY_BYTE);
																	 smDatei.Write(vpTabelle[ulZeile][ucSpalte].vbWert, vpTabelle[ulZeile][ucSpalte].ulLange);
																	 smDatei.Write(&cText, BY_BYTE);
																	 break;
							case FT_AUDIO      :
							case FT_VIDEO      :
							case FT_DOCUMENT   : 
							case FT_SMALLIMAGE :
							case FT_BIGIMAGE   : smDatei.Write(&vpTabelle[ulZeile][ucSpalte].vbWert[*(PBYTE)vpTabelle[ulZeile][ucSpalte].vbWert + 1],
																	 vpTabelle[ulZeile][ucSpalte].ulLange - *(PBYTE)vpTabelle[ulZeile][ucSpalte].vbWert - 1);
																	 break; 
							case FT_BOOL      : (*(bool*)vpTabelle[ulZeile][ucSpalte].vbWert ? asWert = "1" : asWert = "0");
																	smDatei.Write(asWert.c_Str(), asWert.Length()); break;
							case FT_CHAR      : asWert = LONGtoCHAR(c37Zahl, *(PCHAR)vpTabelle[ulZeile][ucSpalte].vbWert);
																	smDatei.Write(asWert.c_Str(), asWert.Length()); break;
							case FT_BYTE      : asWert = ULONGtoCHAR(c37Zahl, *(PBYTE)vpTabelle[ulZeile][ucSpalte].vbWert);
																	smDatei.Write(asWert.c_Str(), asWert.Length()); break;
							case FT_SHORT     : asWert = LONGtoCHAR(c37Zahl, *(PSHORT)vpTabelle[ulZeile][ucSpalte].vbWert);
																	smDatei.Write(asWert.c_Str(), asWert.Length()); break;
							case FT_USHORT    : asWert = ULONGtoCHAR(c37Zahl, *(PUSHORT)vpTabelle[ulZeile][ucSpalte].vbWert);
																	smDatei.Write(asWert.c_Str(), asWert.Length()); break;
							case FT_LONG      : asWert = LONGtoCHAR(c37Zahl, *(PLONG)vpTabelle[ulZeile][ucSpalte].vbWert);
																	smDatei.Write(asWert.c_Str(), asWert.Length()); break;
							case FT_ULONG     : asWert = ULONGtoCHAR(c37Zahl, *(PULONG)vpTabelle[ulZeile][ucSpalte].vbWert);
																	smDatei.Write(asWert.c_Str(), asWert.Length()); break;
							case FT_LONGLONG  : asWert = LONGLONGtoCHAR(c37Zahl, *(PLONGLONG)vpTabelle[ulZeile][ucSpalte].vbWert);
																	smDatei.Write(asWert.c_Str(), asWert.Length()); break;
							case FT_FLOAT     : asWert = FLOAT_B10toCHAR(c37Zahl, *(PFLOAT)vpTabelle[ulZeile][ucSpalte].vbWert, KommaStellen(ucSpalte));
																	smDatei.Write(asWert.c_Str(), asWert.Length()); break;
							case FT_DOUBLE    : asWert = DOUBLE_B10toCHAR(c37Zahl, *(double*)vpTabelle[ulZeile][ucSpalte].vbWert, KommaStellen(ucSpalte));
																	smDatei.Write(asWert.c_Str(), asWert.Length()); break;
							case FT_COMMA4    : k4Zahl.Write(vpTabelle[ulZeile][ucSpalte].vbWert); Comma4toStringA(&asWert, &k4Zahl, KommaStellen(ucSpalte));
																	smDatei.Write(asWert.c_Str(), asWert.Length()); break;
							case FT_COMMA4_80 : k4gZahl.Write(vpTabelle[ulZeile][ucSpalte].vbWert); Comma4_80toStringA(&asWert, &k4gZahl, KommaStellen(ucSpalte));
																	smDatei.Write(asWert.c_Str(), asWert.Length()); break;
							case FT_BIT128    : asWert = BIT128toGUID(c37Zahl, *(BIT128*)vpTabelle[ulZeile][ucSpalte].vbWert);
								                  smDatei.Write(asWert.c_Str(), asWert.Length()); break;
							case FT_DATE      :
							case FT_TIME      : zZeit.Write(vpTabelle[ulZeile][ucSpalte].vbWert);
																	(zZeit.IsZero() ? asWert = NULL : zZeit.StrDateTime(&asWert));
																	smDatei.Write(asWert.c_Str(), asWert.Length()); break;
					} 
					smDatei.Write(&cSemikolon, BY_BYTE);
			 }
			 smDatei.Write(&cZeilenumbruch, BY_BYTE);
		 }
		 smDatei.WriteFile(hDatei, true);
		 CloseHandle(hDatei);
	 }
	 else MessageBox(hWndElement, pcKeinSpeichern, pcDateiSpeichern, MB_ICONHAND);
 }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COTabBasis::DateiSchreiben(char* pcDateinamen)
{
 ThreadSicher_Anfang();
 HANDLE hDatei = CreateFile(pcDateinamen, GENERIC_WRITE, NULL, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
 if(hDatei == INVALID_HANDLE_VALUE) MessageBox(hWndElement, pcKeinSpeichern, pcDateiSpeichern, MB_ICONHAND);
 else{ COStream smDatei(false); BYTE ucLange, ucSpalte;

   smDatei.Write(&ucSpaltenanzahl, BY_BYTE); smDatei.Write(&ucZusatzspalten, BY_BYTE);
   for(ucSpalte = 0; ucSpalte < ucSpaltenanzahl; ucSpalte++){ vucFeldStatus[ucSpalte] <<= 3; vucFeldStatus[ucSpalte] >>= 3; }
   smDatei.Write(vucFeldStatus, ucSpaltenanzahl + ucZusatzspalten);

	 for(ULONG ulZeile = 0; ulZeile < ulDSAnzahl; ulZeile++){
		 for(ucSpalte = 0; ucSpalte < ucSpaltenanzahl + ucZusatzspalten; ucSpalte++){
			 switch(vucFeldStatus[ucSpalte]){
					 case FT_EMPTY      : break;
					 case FT_LISTSTR    :
					 case FT_SHORTSTR   : ucLange = (BYTE)vpTabelle[ulZeile][ucSpalte].ulLange; smDatei.Write(&ucLange, BY_BYTE);
					 case FT_BOOL       : 
					 case FT_CHAR       : 
					 case FT_BYTE       : 
					 case FT_SHORT      : 
					 case FT_USHORT     : 
					 case FT_LONG       : 
					 case FT_ULONG      : 
					 case FT_LONGLONG   : 
					 case FT_FLOAT      : 
					 case FT_DOUBLE     : 
					 case FT_COMMA4     : 
					 case FT_COMMA4_80  : 
					 case FT_DATE       :
					 case FT_TIME       : 
					 case FT_BIT128     : smDatei.Write(vpTabelle[ulZeile][ucSpalte].vbWert, vpTabelle[ulZeile][ucSpalte].ulLange); break;
					 case FT_SMALLIMAGE :
					 case FT_MEMOSTR    : smDatei.Write(&vpTabelle[ulZeile][ucSpalte].ulLange, BY_USHORT);
				 											  smDatei.Write(vpTabelle[ulZeile][ucSpalte].vbWert, vpTabelle[ulZeile][ucSpalte].ulLange); break;
					 case FT_AUDIO      :
					 case FT_VIDEO      :
					 case FT_DOCUMENT   :
					 case FT_BIGIMAGE   :
					 case FT_LONGSTR    : smDatei.Write(&vpTabelle[ulZeile][ucSpalte].ulLange, BY_ULONG);
				 											  smDatei.Write(vpTabelle[ulZeile][ucSpalte].vbWert, vpTabelle[ulZeile][ucSpalte].ulLange);

			 }      
		 }
	 }
	 smDatei.WriteFile(hDatei, true);
   CloseHandle(hDatei);
	 ThreadSicher_Ende();
	 return true;
 }
 ThreadSicher_Ende();
 return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::DruckDaten(COStream* psmDrucken)
{
 BYTE ucLange; USHORT usLange;
 ThreadSicher_Anfang();
 for(ULONG ulZeile = 0; ulZeile < ulDSAnzahl; ulZeile++){
	 for(BYTE ucSpalte = 0; ucSpalte < ucSpaltenanzahl; ucSpalte++){
		 switch(FeldTyp(vucFeldStatus[ucSpalte])){
				 case FT_EMPTY     :
				 case FT_LISTSTR   :
				 case FT_SHORTSTR  : ucLange = (BYTE)vpTabelle[ulZeile][ucSpalte].ulLange; psmDrucken->Write(&ucLange, BY_BYTE);
				 case FT_BOOL      : 
				 case FT_CHAR      : 
				 case FT_BYTE      : 
				 case FT_SHORT     : 
				 case FT_USHORT    : 
				 case FT_LONG      : 
				 case FT_ULONG     : 
				 case FT_LONGLONG  : 
				 case FT_FLOAT     : 
				 case FT_DOUBLE    : 
				 case FT_COMMA4    : 
 				 case FT_COMMA4_80 : 
				 case FT_DATE      :
				 case FT_TIME      : 
				 case FT_BIT128    : psmDrucken->Write(vpTabelle[ulZeile][ucSpalte].vbWert, vpTabelle[ulZeile][ucSpalte].ulLange); break;
				 case FT_MEMOSTR   : usLange = (USHORT)vpTabelle[ulZeile][ucSpalte].ulLange; psmDrucken->Write(&usLange, BY_USHORT);
			 											 psmDrucken->Write(vpTabelle[ulZeile][ucSpalte].vbWert, vpTabelle[ulZeile][ucSpalte].ulLange); break;
				 case FT_LONGSTR   : psmDrucken->Write(&vpTabelle[ulZeile][ucSpalte].ulLange, BY_ULONG);
			 											 psmDrucken->Write(vpTabelle[ulZeile][ucSpalte].vbWert, vpTabelle[ulZeile][ucSpalte].ulLange);

		 }      
	 }
 }
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::Lese(COStream* psmDaten)
{
 BYTE ucLange; USHORT usLange;
 ThreadSicher_Anfang();
 for(ULONG ulZeile = 0; ulZeile < ulDSAnzahl; ulZeile++){
	 for(BYTE ucSpalte = 0; ucSpalte < ucSpaltenanzahl + ucZusatzspalten; ucSpalte++){
		 switch(FeldTyp(vucFeldStatus[ucSpalte])){
				 case FT_EMPTY      : break;
				 case FT_LISTSTR    :
				 case FT_SHORTSTR   : ucLange = (BYTE)vpTabelle[ulZeile][ucSpalte].ulLange; psmDaten->Write(&ucLange, BY_BYTE);
				 case FT_BOOL       : 
				 case FT_CHAR       : 
				 case FT_BYTE       : 
				 case FT_SHORT      : 
				 case FT_USHORT     : 
				 case FT_LONG       : 
				 case FT_ULONG      : 
				 case FT_LONGLONG   : 
				 case FT_FLOAT      : 
				 case FT_DOUBLE     : 
				 case FT_COMMA4     : 
 				 case FT_COMMA4_80  : 
				 case FT_DATE       :
				 case FT_TIME       : 
				 case FT_BIT128     : psmDaten->Write(vpTabelle[ulZeile][ucSpalte].vbWert, vpTabelle[ulZeile][ucSpalte].ulLange); break;
				 case FT_SMALLIMAGE :
				 case FT_MEMOSTR    : usLange = (USHORT)vpTabelle[ulZeile][ucSpalte].ulLange; psmDaten->Write(&usLange, BY_USHORT);
			 											  psmDaten->Write(vpTabelle[ulZeile][ucSpalte].vbWert, vpTabelle[ulZeile][ucSpalte].ulLange); break;
				 case FT_AUDIO      :
				 case FT_VIDEO      :
				 case FT_DOCUMENT   :
				 case FT_BIGIMAGE   :
				 case FT_LONGSTR    : psmDaten->Write(&vpTabelle[ulZeile][ucSpalte].ulLange, BY_ULONG);
			 											  psmDaten->Write(vpTabelle[ulZeile][ucSpalte].vbWert, vpTabelle[ulZeile][ucSpalte].ulLange);

		 }      
	 }
 }
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::Schreibe(VMBLOCK vbDaten, unsigned long ulBytes)
{
 ThreadSicher_Anfang();
 ULONG ulPosition = 0;
 NeueTabelle();

 while(ulPosition < ulBytes){
	 STFeld* vstFelder = DSErstellen();
	 for(BYTE ucSpalte = 0; ucSpalte < ucSpaltenanzahl + ucZusatzspalten; ucSpalte++){
		 switch(vucFeldStatus[ucSpalte]){
				 case FT_EMPTY      : break;
				 case FT_LISTSTR    :
				 case FT_SHORTSTR   : vstFelder[ucSpalte].ulLange = (BYTE)vbDaten[ulPosition]; ulPosition++;
														  vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange);
														  MemCopy(vstFelder[ucSpalte].vbWert, &vbDaten[ulPosition], vstFelder[ucSpalte].ulLange);
														  ulPosition += vstFelder[ucSpalte].ulLange; break;
				 case FT_SMALLIMAGE :
				 case FT_MEMOSTR    : vstFelder[ucSpalte].ulLange = (USHORT)vbDaten[ulPosition]; ulPosition += BY_USHORT;
														  vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange);
														  MemCopy(vstFelder[ucSpalte].vbWert, &vbDaten[ulPosition], vstFelder[ucSpalte].ulLange);
														  ulPosition += vstFelder[ucSpalte].ulLange; break;  
				 case FT_AUDIO      :
				 case FT_VIDEO      :
				 case FT_DOCUMENT   :
				 case FT_BIGIMAGE   :
				 case FT_LONGSTR    : vstFelder[ucSpalte].ulLange = (ULONG)vbDaten[ulPosition]; ulPosition += BY_ULONG;
														  vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange);
														  MemCopy(vstFelder[ucSpalte].vbWert, &vbDaten[ulPosition], vstFelder[ucSpalte].ulLange);
														  ulPosition += vstFelder[ucSpalte].ulLange; break;   
				 case FT_BOOL       : MemCopy(vstFelder[ucSpalte].vbWert, &vbDaten[ulPosition], vstFelder[ucSpalte].ulLange); ulPosition++; break;
				 case FT_CHAR       : MemCopy(vstFelder[ucSpalte].vbWert, &vbDaten[ulPosition], vstFelder[ucSpalte].ulLange); ulPosition++; break;
				 case FT_BYTE       : MemCopy(vstFelder[ucSpalte].vbWert, &vbDaten[ulPosition], vstFelder[ucSpalte].ulLange); ulPosition++; break;
				 case FT_SHORT      : MemCopy(vstFelder[ucSpalte].vbWert, &vbDaten[ulPosition], vstFelder[ucSpalte].ulLange); ulPosition += BY_SHORT; break;
				 case FT_USHORT     : MemCopy(vstFelder[ucSpalte].vbWert, &vbDaten[ulPosition], vstFelder[ucSpalte].ulLange); ulPosition += BY_USHORT; break;
				 case FT_LONG       : MemCopy(vstFelder[ucSpalte].vbWert, &vbDaten[ulPosition], vstFelder[ucSpalte].ulLange); ulPosition += BY_LONG; break;
				 case FT_ULONG      : MemCopy(vstFelder[ucSpalte].vbWert, &vbDaten[ulPosition], vstFelder[ucSpalte].ulLange); ulPosition += BY_ULONG; break;
				 case FT_LONGLONG   : MemCopy(vstFelder[ucSpalte].vbWert, &vbDaten[ulPosition], vstFelder[ucSpalte].ulLange); ulPosition += BY_LONGLONG; break;
				 case FT_FLOAT      : MemCopy(vstFelder[ucSpalte].vbWert, &vbDaten[ulPosition], vstFelder[ucSpalte].ulLange); ulPosition += BY_FLOAT; break;
				 case FT_DOUBLE     : MemCopy(vstFelder[ucSpalte].vbWert, &vbDaten[ulPosition], vstFelder[ucSpalte].ulLange); ulPosition += BY_DOUBLE; break;
				 case FT_COMMA4     : MemCopy(vstFelder[ucSpalte].vbWert, &vbDaten[ulPosition], vstFelder[ucSpalte].ulLange); ulPosition += BY_COMMA4; break;
				 case FT_COMMA4_80  : MemCopy(vstFelder[ucSpalte].vbWert, &vbDaten[ulPosition], vstFelder[ucSpalte].ulLange); ulPosition += BY_COMMA4_80; break;
				 case FT_DATE       :
				 case FT_TIME       : MemCopy(vstFelder[ucSpalte].vbWert, &vbDaten[ulPosition], vstFelder[ucSpalte].ulLange); ulPosition += BY_TIME; break;
				 case FT_BIT128     : MemCopy(vstFelder[ucSpalte].vbWert, &vbDaten[ulPosition], vstFelder[ucSpalte].ulLange); ulPosition += BY_BIT128; break;
		 }
	 }
 	 DSAnfugen(vstFelder);
 }  
 NeueTabelle_Zeichnen();
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::Schreibe(COStream* psmDaten)
{
 ThreadSicher_Anfang();
 NeueTabelle();
 if(psmDaten){ psmDaten->SetPosition(0, STM_POS_ANFANG); BYTE ucLange;
	 while(!psmDaten->End()){
		 STFeld* vstFelder = DSErstellen();
		 for(BYTE ucSpalte = 0; ucSpalte < ucSpaltenanzahl + ucZusatzspalten; ucSpalte++){
			 switch(vucFeldStatus[ucSpalte]){
			 		 case FT_EMPTY      : break;
					 case FT_LISTSTR    :
					 case FT_SHORTSTR   : psmDaten->Read(&ucLange, BY_BYTE); vstFelder[ucSpalte].ulLange = ucLange;
															  vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange);
					 case FT_BOOL       : 
					 case FT_CHAR       : 
					 case FT_BYTE       : 
					 case FT_SHORT      : 
					 case FT_USHORT     : 
					 case FT_LONG       : 
					 case FT_ULONG      : 
					 case FT_LONGLONG   : 
					 case FT_FLOAT      : 
					 case FT_DOUBLE     : 
					 case FT_COMMA4     : 
					 case FT_COMMA4_80  : 
					 case FT_DATE       :
					 case FT_TIME       : 
					 case FT_BIT128     : psmDaten->Read(vstFelder[ucSpalte].vbWert, vstFelder[ucSpalte].ulLange); break;
					 case FT_SMALLIMAGE :
					 case FT_MEMOSTR    : psmDaten->Read(&vstFelder[ucSpalte].ulLange, BY_USHORT);
															  vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange);
															  psmDaten->Read(vstFelder[ucSpalte].vbWert, vstFelder[ucSpalte].ulLange); break;
					 case FT_AUDIO      :
					 case FT_VIDEO      :
 					 case FT_DOCUMENT   :
					 case FT_BIGIMAGE   :
					 case FT_LONGSTR    : psmDaten->Read(&vstFelder[ucSpalte].ulLange, BY_ULONG);
														 	  vstFelder[ucSpalte].vbWert = VMBlockS(vmTabelle, vstFelder[ucSpalte].ulLange);
															  psmDaten->Read(vstFelder[ucSpalte].vbWert, vstFelder[ucSpalte].ulLange); break;

			 }
		 }
 		 DSAnfugen(vstFelder);
	 }  
 }
 NeueTabelle_Zeichnen();
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
COStringA* __vectorcall RePag::GUI::COTabBasis::Prufwert(COStringA* pasWert)
{
 ThreadSicher_Anfang();
 *pasWert = *vasNeuerWert;
 ThreadSicher_Ende();
 return pasWert;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::Berechtigungen(unsigned char ucRechteA)
{
 ThreadSicher_Anfang();
 ucRechte = ucRechteA;
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::BoolWertText(char* pcTrue, char* pcFalse)
{
 ThreadSicher_Anfang();
 BYTE ucStrLange;
 VMFrei(vbBool_true); VMFrei(vbBool_false);
 ucStrLange = StrLength(pcTrue); vbBool_true = VMBlock(ucStrLange + 1); MemCopy(vbBool_true, pcTrue, ucStrLange); vbBool_true[ucStrLange] = 0;
 ucStrLange = StrLength(pcFalse); vbBool_false = VMBlock(ucStrLange + 1); MemCopy(vbBool_false, pcFalse, ucStrLange); vbBool_false[ucStrLange] = 0;
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::LeerFeld(COStringA* vasText, unsigned long ulZeile, unsigned char ucSpalte)
{
 ThreadSicher_Anfang();
 LeerFeld(vasText->c_Str(), ulZeile, ucSpalte);
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::LeerFeld(char* pcText, unsigned long ulZeile, unsigned char ucSpalte)
{
 ThreadSicher_Anfang();
 if(hWndElement && FeldTyp(vucFeldStatus[ucSpalte]) == FT_EMPTY && ulZeile < ulDSAnzahl && ucSpalte < ucSpaltenanzahl){
	 if(vpTabelle[ulZeile][ucSpalte].vbWert) VMFrei(vmTabelle, vpTabelle[ulZeile][ucSpalte].vbWert);
   vpTabelle[ulZeile][ucSpalte].ulLange = StrLength(pcText);
   vpTabelle[ulZeile][ucSpalte].vbWert = VMBlock(vmTabelle, vpTabelle[ulZeile][ucSpalte].ulLange);
	 MemCopy(vpTabelle[ulZeile][ucSpalte].vbWert, pcText, vpTabelle[ulZeile][ucSpalte].ulLange);

	 if(bEinfugen || bFeldanderung){ SCROLLINFO stScrollVert; stScrollVert.cbSize = sizeof(SCROLLINFO); stScrollVert.fMask = SIF_POS; RECT rcZeichnen;
     GetScrollInfo(hWndElement, SB_VERT, &stScrollVert);
     rcZeichnen.top = (ulZeileSelect - stScrollVert.nPos) * ucZeilenhohe + ucKopfhohe; rcZeichnen.bottom = rcZeichnen.top + ucZeilenhohe;

		 sgKopf->Schalter_Pos_X(ucSpalte, rcZeichnen.left); rcZeichnen.right = rcZeichnen.left + vstSpalten[ucSpalte].lBreite;
		 UpdateFenster(&rcZeichnen, true, false);
	 }
 }
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
unsigned char __vectorcall RePag::GUI::COTabBasis::KommaStellen(unsigned char ucSpalte)
{
 unsigned char ucStellen = 1;	
 if(vstSpalten[ucSpalte].usFormat & (1 << 6)){ ucStellen = 1;
   if(vstSpalten[ucSpalte].usFormat & (1 << 7)) ucStellen++;
 }
 else if(vstSpalten[ucSpalte].usFormat & (1 << 7)){ ucStellen = 3;
   if(vstSpalten[ucSpalte].usFormat & (1 << 8)) ucStellen++;
 }
 else if(vstSpalten[ucSpalte].usFormat & (1 << 8)){ ucStellen = 5;
   if(vstSpalten[ucSpalte].usFormat & (1 << 9)) ucStellen++;
 }
 else if(vstSpalten[ucSpalte].usFormat & (1 << 9)){ ucStellen = 7;
   if(vstSpalten[ucSpalte].usFormat & (1 << 10)) ucStellen++;
 }
 else if(vstSpalten[ucSpalte].usFormat & (1 << 10)){ ucStellen = 9;
   if(vstSpalten[ucSpalte].usFormat & (1 << 11)) ucStellen++;
 }
 else if(vstSpalten[ucSpalte].usFormat & (1 << 11)){ ucStellen = 11;
   if(vstSpalten[ucSpalte].usFormat & (1 << 12)) ucStellen++;
 }
 else if(vstSpalten[ucSpalte].usFormat & (1 << 12)){ ucStellen = 13;
   if(vstSpalten[ucSpalte].usFormat & (1 << 13)) ucStellen++;
 }
 else if(vstSpalten[ucSpalte].usFormat & (1 << 13)){ ucStellen = 15;
   if(vstSpalten[ucSpalte].usFormat & (1 << 14)) ucStellen++;
 }
 return ucStellen;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::Eingabe_Position(COElement* pElement, SCROLLINFO& stScrollInfo)
{
 RECT rcZeichnen; BYTE ucFeldTyp = FeldTyp(vucFeldStatus[ucSpalteSelect]);
 sgKopf->Schalter_Pos_X(ucSpalteSelect, rcZeichnen.left);

 (ucFeldTyp == FT_DATE ? daEingabe->Breite_EditDatum(rcZeichnen.right) : pElement->Breite(rcZeichnen.right));
 if(lRand_rechts - rcZeichnen.left < rcZeichnen.right) rcZeichnen.left = rcZeichnen.left - rcZeichnen.right + vstSpalten[ucSpalteSelect].lBreite;

 (ucFeldTyp == FT_LISTSTR ? rcZeichnen.top = ucZeilenhohe * (ulZeileSelect - stScrollInfo.nPos + 1) + ucKopfhohe
                          : rcZeichnen.top = ucZeilenhohe * (ulZeileSelect - stScrollInfo.nPos) + ucKopfhohe);

 (ucFeldTyp == FT_DATE ? daEingabe->Hohe_EditDatum(rcZeichnen.bottom) : pElement->Hohe(rcZeichnen.bottom));

 if(lRand_unten < rcZeichnen.top + rcZeichnen.bottom) rcZeichnen.top = lRand_unten - rcZeichnen.bottom;

 (ucFeldTyp == FT_DATE ? daEingabe->NeueFensterPosition(rcZeichnen.left, rcZeichnen.top) : pElement->NeueFensterPosition(rcZeichnen.left, rcZeichnen.top));
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::Eingabe_Return(void)
{
 SCROLLINFO stScrollHorz; stScrollHorz.cbSize = sizeof(SCROLLINFO); stScrollHorz.fMask = SIF_ALL; long lEingabe_links;
 GetScrollInfo(hWndElement, SB_HORZ, &stScrollHorz);

 if(ucSpalteSelect + 1 <= stScrollHorz.nMax){
	 if(sgKopf->Schalter_Pos_X(ucSpalteSelect + 1, lEingabe_links) + vstSpalten[ucSpalteSelect + 1].lBreite < lRand_rechts) SelectZelle(ulZeileSelect, ucSpalteSelect + 1);
   else{
		 do{ SendMessage(hWndElement, WM_HSCROLL, SB_LINERIGHT, NULL); GetScrollInfo(hWndElement, SB_HORZ, &stScrollHorz); }
		 while(ucSpalteSelect + 1 >= stScrollHorz.nPos + (int)stScrollHorz.nPage);
		 SelectZelle(ulZeileSelect, ucSpalteSelect + 1);
	 }
 }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::Eingabe_Escape(void)
{
 if(bEinfugen){ EinfugenRollback(); LoschEinfugen(); }
 else{ 
	 if(bFeldanderung){ AndernRollback(); LoschAndern(); }	 
 	 else if(bAnsehen) bAnsehen = false;
	 SCROLLINFO stScrollInfo; stScrollInfo.cbSize = sizeof(SCROLLINFO); stScrollInfo.fMask = SIF_POS; long lEingabe_Links;
	 GetScrollInfo(hWndElement, SB_VERT, &stScrollInfo); 
	 ezEingabe->NeuesFenster(ucZeilenhohe, vstSpalten[ucSpalteSelect].lBreite,
		 sgKopf->Schalter_Pos_X(ucSpalteSelect, lEingabe_Links), ucZeilenhohe * (ulZeileSelect - stScrollInfo.nPos) + ucKopfhohe);
	 ezEingabe->Hintergrundfarbe(0, 128, 0); ezEingabe->Schriftfarbe(255, 255, 255);
	 ezEingabe->SetzSichtbar(true); FeldWert();
 }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::EingabeZeile_Zeichenvorgabe(unsigned char ucZeichenvorgabe)
{
 ezEingabe->Zeichenvorgabe(ucZeichenvorgabe);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::EingabeZeile_Zeichenmaske(const char* pcZeichenmaske)
{
 ezEingabe->Zeichenmaske(pcZeichenmaske);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::EingabeZeile_SetzZeichen_Max(unsigned long ulZeichen)
{
 ThreadSicher_Anfang();
 if(ulZeichen <= UCHAR_MAX) ezEingabe->SetzZeichen_Max(ulZeichen);
 else ezEingabe->SetzZeichen_Max(UCHAR_MAX);
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::EingabeListe_Grosse(long lHoheA, long lBreiteA)
{
 ThreadSicher_Anfang();
 if(!lbEingabe->HWND_Element()){ stGrosse_Listbox.lHohe = lHoheA; stGrosse_Listbox.lBreite = lBreiteA; }
 else{ (lHoheA > lRand_unten - ucKopfhohe ? stGrosse_Listbox.lHohe = lRand_unten - ucKopfhohe : stGrosse_Listbox.lHohe = lHoheA);
	     (lBreiteA > lRand_rechts ? stGrosse_Listbox.lBreite = lRand_rechts : stGrosse_Listbox.lBreite = lBreiteA);	 
	     lbEingabe->NeueFensterGrosse(stGrosse_Listbox.lHohe, stGrosse_Listbox.lBreite); }
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::EingabeListe_Text(char* pcText)
{
 lbEingabe->Text(pcText);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
unsigned char __vectorcall RePag::GUI::COTabBasis::EingabeListe_SelectIndex(void)
{
 return lbEingabe->SelectIndex();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
bool __vectorcall RePag::GUI::COTabBasis::EingabeListe_SuchUndSetzEintrag(COStringA* vasEintrag, unsigned char& ucIndex)
{
 ThreadSicher_Anfang();
 if(vasEintrag->Length()){ ThreadSicher_Ende(); return lbEingabe->SuchUndSetzEintrag(vasEintrag, ucIndex); }
 else{ lbEingabe->SetzUndSuchEintrag(0, vasEintrag);
   vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange = vasEintrag->Length();
   vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert = VMBlock(vmTabelle, vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
	 MemCopy(vpTabelle[ulZeileSelect][ucSpalteSelect].vbWert, vasEintrag->c_Str(), vpTabelle[ulZeileSelect][ucSpalteSelect].ulLange);
	 ThreadSicher_Ende(); return true;
 }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::EingabeText_Grosse(long lHoheA, long lBreiteA)
{
 ThreadSicher_Anfang();
 if(!ebEingabe->HWND_Element()){ stGrosse_Textbox.lHohe = lHoheA; stGrosse_Textbox.lBreite = lBreiteA; }
 else{ (lHoheA > lRand_unten - ucKopfhohe ? stGrosse_Textbox.lHohe = lRand_unten - ucKopfhohe : stGrosse_Textbox.lHohe = lHoheA);
	     (lBreiteA > lRand_rechts ? stGrosse_Textbox.lBreite = lRand_rechts : stGrosse_Textbox.lBreite = lBreiteA);	 
	     ebEingabe->NeueFensterGrosse(stGrosse_Textbox.lHohe, stGrosse_Textbox.lBreite); }
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::EingabeText_SetzZeichen_Max(unsigned long ulZeichen)
{
 ThreadSicher_Anfang();
 if(FeldTyp(vucFeldStatus[ucSpalteSelect]) == FT_MEMOSTR){
   if(ulZeichen <= USHRT_MAX) ebEingabe->SetzZeichen_Max(ulZeichen);
	 else ebEingabe->SetzZeichen_Max(USHRT_MAX);
 }
 else if(FeldTyp(vucFeldStatus[ucSpalteSelect]) == FT_LONGSTR){
   if(ulZeichen <= ULONG_MAX) ebEingabe->SetzZeichen_Max(ulZeichen);
	 else ebEingabe->SetzZeichen_Max(ULONG_MAX);
 }
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::EingabeBild_Grosse(long lHoheA, long lBreiteA)
{
 ThreadSicher_Anfang();
 if(!biEingabe->HWND_Element()){ stGrosse_Bild.lHohe = lHoheA; stGrosse_Bild.lBreite = lBreiteA; }
 else{ (lHoheA > lRand_unten - ucKopfhohe ? stGrosse_Bild.lHohe = lRand_unten - ucKopfhohe : stGrosse_Bild.lHohe = lHoheA);
	     (lBreiteA > lRand_rechts ? stGrosse_Bild.lBreite = lRand_rechts : stGrosse_Bild.lBreite = lBreiteA);	 
	     biEingabe->NeueFensterGrosse(stGrosse_Bild.lHohe, stGrosse_Bild.lBreite); }
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::EingabeAudio_Grosse(long lHoheA, long lBreiteA)
{
 ThreadSicher_Anfang();
 if(!maEingabe->HWND_Element()){ stGrosse_Audio.lHohe = lHoheA; stGrosse_Audio.lBreite = lBreiteA; } 
 else{ (lHoheA > lRand_unten - ucKopfhohe ? stGrosse_Audio.lHohe = lRand_unten - ucKopfhohe : stGrosse_Audio.lHohe = lHoheA);
	     (lBreiteA > lRand_rechts ? stGrosse_Audio.lBreite = lRand_rechts : stGrosse_Audio.lBreite = lBreiteA);	 
	     maEingabe->NeueFensterGrosse(stGrosse_Audio.lHohe, stGrosse_Audio.lBreite); }
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::EingabeVideo_Grosse(long lHoheA, long lBreiteA)
{
 ThreadSicher_Anfang();
 if(!mvEingabe->HWND_Element()){ stGrosse_Video.lHohe = lHoheA; stGrosse_Video.lBreite = lBreiteA; }
 else{ (lHoheA > lRand_unten - ucKopfhohe ? stGrosse_Video.lHohe = lRand_unten - ucKopfhohe : stGrosse_Video.lHohe = lHoheA);
	     (lBreiteA > lRand_rechts ? stGrosse_Video.lBreite = lRand_rechts : stGrosse_Video.lBreite = lBreiteA);	 
	     mvEingabe->NeueFensterGrosse(stGrosse_Video.lHohe, stGrosse_Video.lBreite); }
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::EingabeDatum_Grosse(long lHoheA, long lBreiteA)
{
 ThreadSicher_Anfang();
 if(!daEingabe->HWND_Element()){ stGrosse_Datum.lHohe = lHoheA; stGrosse_Datum.lBreite = lBreiteA; }
 else{ (lHoheA > lRand_unten - ucKopfhohe ? stGrosse_Datum.lHohe = lRand_unten - ucKopfhohe : stGrosse_Datum.lHohe = lHoheA);
	     (lBreiteA > lRand_rechts ? stGrosse_Datum.lBreite = lRand_rechts : stGrosse_Datum.lBreite = lBreiteA);	 
	     daEingabe->NeueFensterGrosse(stGrosse_Datum.lHohe, stGrosse_Datum.lBreite); }
 ThreadSicher_Ende();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::EingabeBild_Berechtigungen(unsigned char ucRechte)
{
 biEingabe->Berechtigungen(ucRechte);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::EingabeDokument_Berechtigungen(unsigned char ucRechte)
{
 doEingabe->Berechtigungen(ucRechte);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::EingabeAudio_Berechtigungen(unsigned char ucRechte)
{
 maEingabe->Berechtigungen(ucRechte);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::EingabeVideo_Berechtigungen(unsigned char ucRechte)
{
 mvEingabe->Berechtigungen(ucRechte);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabBasis::EingabeDatum_DatumZeitFormat(const char* pcFormat_Datum, const char* pcFormat_Zeit, bool bAnordnung_DatumZeit)
{
 daEingabe->DatumZeitFormat(pcFormat_Datum, pcFormat_Zeit, bAnordnung_DatumZeit);
}
//-----------------------------------------------------------------------------------------------------------------------------------------