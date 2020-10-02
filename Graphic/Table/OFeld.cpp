/****************************************************************************
  OFeld.cpp
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
#include "OFeld.h"
#define BY_COFELD 12
#define _Feld ((STFeld*)c8Feld)
//---------------------------------------------------------------------------
RePag::GUI::COFeld* __vectorcall RePag::GUI::COFeldV(const unsigned char ucFeldtyp)
{
 COFeld* vFeld = (COFeld*)VMBlock(sizeof(COFeld));
 vFeld->COFeldV(ucFeldtyp);
 return vFeld;
}
//---------------------------------------------------------------------------
void __vectorcall RePag::GUI::COFeld::COFeldV(const unsigned char ucFeldtypA)
{
 ucFeldtyp = ucFeldtypA;
}
//---------------------------------------------------------------------------
VMEMORY __vectorcall RePag::GUI::COFeld::COFreiV(void)
{
 if(_Feld->vbWert) VMFrei(_Feld->vbWert);
 return NULL;
}
//---------------------------------------------------------------------------
void __vectorcall RePag::GUI::COFeld::operator =(const STFeld& stFeld)
{
 if(_Feld->vbWert) VMFrei(_Feld->vbWert);
 _Feld->ulLange = stFeld.ulLange;

 switch(ucFeldtyp){ 
	  case FT_SHORTSTR : _Feld->vbWert = VMBlock(_Feld->ulLange + 1); _Feld->vbWert[_Feld->ulLange] = 0; break;
		default          : _Feld->vbWert = VMBlock(_Feld->ulLange); break;
 }

 MemCopy(_Feld->vbWert, stFeld.vbWert, _Feld->ulLange);
}
//---------------------------------------------------------------------------
void __vectorcall RePag::GUI::COFeld::operator =(COStringA& asWert)
{
 if(_Feld->vbWert) VMFrei(_Feld->vbWert);
 _Feld->ulLange = asWert.Length();
 _Feld->vbWert = VMBlock(_Feld->ulLange + 1);
 MemCopy(_Feld->vbWert, asWert.c_Str(), _Feld->ulLange);
 _Feld->vbWert[_Feld->ulLange] = 0;
}
//---------------------------------------------------------------------------
void __vectorcall RePag::GUI::COFeld::operator =(const char* pcString)
{
 if(_Feld->vbWert) VMFrei(_Feld->vbWert);
 _Feld->ulLange = StrLength(pcString);
 _Feld->vbWert = VMBlock(_Feld->ulLange + 1);
 MemCopy(_Feld->vbWert, pcString, _Feld->ulLange);
 _Feld->vbWert[_Feld->ulLange] = 0;
}
//---------------------------------------------------------------------------
ULONG __vectorcall RePag::GUI::COFeld::Lange(void)
{
 return _Feld->ulLange;
}
//---------------------------------------------------------------------------
VMBLOCK __vectorcall RePag::GUI::COFeld::Wert(void)
{
 return _Feld->vbWert;
}
//---------------------------------------------------------------------------
COComma4* __vectorcall RePag::GUI::COFeld::KOMMA4(COComma4* pk4Zahl)
{
 pk4Zahl->Write(_Feld->vbWert);
 return pk4Zahl;
}
//---------------------------------------------------------------------------
COComma4_80* __vectorcall RePag::GUI::COFeld::KOMMA4_80(COComma4_80* pk4gZahl)
{
 pk4gZahl->Write(_Feld->vbWert);
 return pk4gZahl;
}
//---------------------------------------------------------------------------
COTime* __vectorcall RePag::GUI::COFeld::ZEIT(COTime* pzZeit)
{
 pzZeit->Write(_Feld->vbWert);
 return pzZeit;
}
//---------------------------------------------------------------------------