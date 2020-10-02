/****************************************************************************
  OTabelle.cpp
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
#include "OTabelle.h"
//---------------------------------------------------------------------------
RePag::GUI::COTabelle* __vectorcall RePag::GUI::COTabelleV(const char* pcFensterName, unsigned int uiIDElement, unsigned long ulZeilen_Blatt, unsigned char ucSpalten)
{
 COTabelle* vTabelle = (COTabelle*)VMBlock(VMDialog(), sizeof(COTabelle));
 vTabelle->COTabelleV(VMDialog(), pcFensterName, uiIDElement, ulZeilen_Blatt, ucSpalten);
 return vTabelle;
}
//---------------------------------------------------------------------------
RePag::GUI::COTabelle* __vectorcall RePag::GUI::COTabelleV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement, unsigned long ulZeilen_Blatt, unsigned char ucSpalten)
{
 COTabelle* vTabelle = (COTabelle*)VMBlock(vmSpeicher, sizeof(COTabelle));
 vTabelle->COTabelleV(vmSpeicher, pcFensterName, uiIDElement, ulZeilen_Blatt, ucSpalten);
 return vTabelle;
}
//---------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabelle::COTabelleV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA, unsigned long ulZeilen_Blatt, unsigned char ucSpalten)
{
 COTabBasisV(vmSpeicher, pcRePag_Tabelle, pcFensterName, uiIDElementA, ulZeilen_Blatt, ucSpalten);

 vucFeldStatus = (unsigned char*)VMBlock(vmTabelle, ucSpaltenanzahl);
}
//---------------------------------------------------------------------------
VMEMORY __vectorcall RePag::GUI::COTabelle::COFreiV(void)
{
 VMFrei(vmTabelle, vucFeldStatus);
 return ((COTabBasis*)this)->COFreiV();
}
//---------------------------------------------------------------------------
void __vectorcall RePag::GUI::COTabelle::Spalte(unsigned char ucSpalte, const char* pcBeschriftung, long lBreite, unsigned short usFormat, unsigned char ucFeldstatus)
{
 ThreadSicher_Anfang();
 if(ucSpalte < ucSpaltenanzahl){
   sgKopf->Schalter_Beschriftung(ucSpalte, pcBeschriftung);
	 vstSpalten[ucSpalte].lBreite = lBreite;
	 vstSpalten[ucSpalte].usFormat = usFormat;
   vucFeldStatus[ucSpalte] = ucFeldstatus;
 }
 ThreadSicher_Ende();
}
//---------------------------------------------------------------------------