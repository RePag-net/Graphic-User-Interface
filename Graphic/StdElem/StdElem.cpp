/****************************************************************************
  StdElem.cpp
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
#include "OTextZeile.h"
#include "OEditZeile.h"
#include "OTextBox.h"
#include "OEditBox.h"
#include "OListBox.h"
#include "OPasswort.h"
#include "OKnopf.h"
#include "OKlappBox.h"
#include "OSchalter.h"
#include "OSchalterGruppe.h"
#include "OBalken.h"
#include "ODatum.h"
#include "OLeuchte.h"

using namespace RePag::GUI;
//---------------------------------------------------------------------------
HKEY hSchlussel; DWORD dwBytes = 4;
//---------------------------------------------------------------------------
char pcRePag_TextZeile[] = "RePag_TextZeile";
char pcRePag_EditZeile[] = "RePag_EditZeile";
char pcRePag_TextBox[] = "RePag_TextBox";
char pcRePag_EditBox[] = "RePag_EditBox";
char pcRePag_ListBox[] = "RePag_ListBox";
char pcRePag_Passwort[] = "RePag_Passwort";
char pcRePag_Knopf[] = "RePag_Knopf";
char pcRePag_KlappBox[] = "RePag_KlappBox";
char pcRePag_KlappBoxEintrage[] = "RePag_KlappBoxEintrage";
char pcRePag_Schalter[] = "RePag_Schalter";
char pcRePag_SchalterGruppe[] = "RePag_SchalterGruppe";
char pcRePag_WechselSchalter[] = "RePag_WechselSchalter";
char pcRePag_TabellenKopf[] = "RePag_TabellenKopf";
char pcRePag_Balken[] = "RePag_Balken";
char pcRePag_Datum[] = "RePag_Datum";
char pcRePag_EditDatum[] = "RePag_EditDatum";
char pcRePag_Leuchte[] = "RePag_Leuchte";
//---------------------------------------------------------------------------
void __vectorcall RegisterKlassenName(void)
{
  WNDCLASSEX wndKlasse;
  wndKlasse.cbSize = sizeof(WNDCLASSEX);
  wndKlasse.style = CS_OWNDC;
  wndKlasse.cbClsExtra = 0;
  wndKlasse.cbWndExtra = 8;
  wndKlasse.hInstance = NULL;
  wndKlasse.hIcon = NULL;
  wndKlasse.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndKlasse.hbrBackground = NULL;
  wndKlasse.lpszMenuName = NULL;
  wndKlasse.hIconSm = NULL;

  wndKlasse.lpszClassName = pcRePag_TextZeile;
  wndKlasse.lpfnWndProc = WndProc_TextZeile;
  RegisterClassEx(&wndKlasse);

  wndKlasse.lpszClassName = pcRePag_Balken;
  wndKlasse.lpfnWndProc = WndProc_Balken;
  RegisterClassEx(&wndKlasse);

  wndKlasse.lpszClassName = pcRePag_Leuchte;
  wndKlasse.lpfnWndProc = WndProc_Leuchte;
  RegisterClassEx(&wndKlasse);

  wndKlasse.lpszClassName = pcRePag_SchalterGruppe;
  wndKlasse.lpfnWndProc = WndProc_SchalterGruppe;
  RegisterClassEx(&wndKlasse);

  wndKlasse.lpszClassName = pcRePag_TabellenKopf;
  wndKlasse.lpfnWndProc = WndProc_SchalterGruppe;
  wndKlasse.hCursor = LoadCursor(NULL, IDC_SIZEWE);
  RegisterClassEx(&wndKlasse);

  wndKlasse.lpszClassName = pcRePag_EditZeile;
  wndKlasse.lpfnWndProc = WndProc_EditZeile;
  wndKlasse.style |= CS_DBLCLKS;
  wndKlasse.hCursor = LoadCursor(NULL, IDC_IBEAM);
  wndKlasse.hbrBackground = NULL;
  RegisterClassEx(&wndKlasse);
  wndKlasse.style = CS_OWNDC;

  wndKlasse.lpszClassName = pcRePag_TextBox;
  wndKlasse.lpfnWndProc = WndProc_TextBox;
  wndKlasse.style |= CS_HREDRAW | CS_VREDRAW;
  wndKlasse.hCursor = LoadCursor(NULL, IDC_HAND);
  RegisterClassEx(&wndKlasse);
  wndKlasse.style = CS_OWNDC;

  wndKlasse.lpszClassName = pcRePag_EditBox;
  wndKlasse.lpfnWndProc = WndProc_EditBox;
  wndKlasse.style |= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
  wndKlasse.hCursor = LoadCursor(NULL, IDC_IBEAM);
  RegisterClassEx(&wndKlasse);
  wndKlasse.style = CS_OWNDC;

  wndKlasse.lpszClassName = pcRePag_ListBox;
  wndKlasse.lpfnWndProc = WndProc_ListBox;
  wndKlasse.style |= CS_HREDRAW | CS_VREDRAW;
  wndKlasse.hCursor = LoadCursor(NULL, IDC_HAND);
  RegisterClassEx(&wndKlasse);
  wndKlasse.style = CS_OWNDC;

  wndKlasse.lpszClassName = pcRePag_Passwort;
  wndKlasse.lpfnWndProc = WndProc_Passwort;
  wndKlasse.hCursor = LoadCursor(NULL, IDC_IBEAM);
  RegisterClassEx(&wndKlasse);

  wndKlasse.lpszClassName = pcRePag_Knopf;
  wndKlasse.lpfnWndProc = WndProc_Knopf;
  wndKlasse.hCursor = LoadCursor(NULL, IDC_HAND);
  RegisterClassEx(&wndKlasse);

  wndKlasse.lpszClassName = pcRePag_KlappBox;
  wndKlasse.lpfnWndProc = WndProc_KlappBox;
  wndKlasse.style |= CS_HREDRAW | CS_VREDRAW;
  wndKlasse.hCursor = LoadCursor(NULL, IDC_HAND);
  RegisterClassEx(&wndKlasse);
  wndKlasse.style = CS_OWNDC;

  wndKlasse.lpszClassName = pcRePag_KlappBoxEintrage;
  wndKlasse.lpfnWndProc = WndProc_EintragZeile;
  RegisterClassEx(&wndKlasse);

  wndKlasse.lpszClassName = pcRePag_Schalter;
  wndKlasse.lpfnWndProc = WndProc_Schalter;
  wndKlasse.hCursor = LoadCursor(NULL, IDC_HAND);
  RegisterClassEx(&wndKlasse);

  wndKlasse.lpszClassName = pcRePag_WechselSchalter;
  wndKlasse.lpfnWndProc = WndProc_WechselSchalter;
  RegisterClassEx(&wndKlasse);

  wndKlasse.lpszClassName = pcRePag_Datum;
  wndKlasse.lpfnWndProc = WndProc_Datum;
  RegisterClassEx(&wndKlasse);

  wndKlasse.lpszClassName = pcRePag_EditDatum;
  wndKlasse.lpfnWndProc = WndProc_EditDatum;
  wndKlasse.style |= CS_DBLCLKS;
  wndKlasse.hCursor = LoadCursor(NULL, IDC_IBEAM);
  RegisterClassEx(&wndKlasse);
  wndKlasse.style = CS_OWNDC;
  wndKlasse.hCursor = LoadCursor(NULL, IDC_ARROW);
}
//---------------------------------------------------------------------------
void __vectorcall UnRegisterKlassenName(void)
{
  WNDCLASSEX wndKlasse; char pcUnregister[] = "Unregister";
  wndKlasse.cbSize = sizeof(WNDCLASSEX);
  wndKlasse.style = CS_OWNDC;
  wndKlasse.cbClsExtra = 0;
  wndKlasse.cbWndExtra = 0;
  wndKlasse.hInstance = NULL;
  wndKlasse.hIcon = NULL;
  wndKlasse.hCursor = NULL;
  wndKlasse.hbrBackground = NULL;
  wndKlasse.lpszMenuName = NULL;
  wndKlasse.hIconSm = NULL;
  wndKlasse.lpszClassName = pcUnregister;
  wndKlasse.lpfnWndProc = NULL;
  RegisterClassEx(&wndKlasse);

  UnregisterClass(pcRePag_TextZeile, wndKlasse.hInstance);
  UnregisterClass(pcRePag_EditZeile, wndKlasse.hInstance);
  UnregisterClass(pcRePag_TextBox, wndKlasse.hInstance);
  UnregisterClass(pcRePag_EditBox, wndKlasse.hInstance);
  UnregisterClass(pcRePag_ListBox, wndKlasse.hInstance);
  UnregisterClass(pcRePag_Passwort, wndKlasse.hInstance);
  UnregisterClass(pcRePag_Knopf, wndKlasse.hInstance);
  UnregisterClass(pcRePag_KlappBox, wndKlasse.hInstance);
  UnregisterClass(pcRePag_KlappBoxEintrage, wndKlasse.hInstance);
  UnregisterClass(pcRePag_Schalter, wndKlasse.hInstance);
  UnregisterClass(pcRePag_SchalterGruppe, wndKlasse.hInstance);
  UnregisterClass(pcRePag_WechselSchalter, wndKlasse.hInstance);
  UnregisterClass(pcRePag_TabellenKopf, wndKlasse.hInstance);
  UnregisterClass(pcRePag_Balken, wndKlasse.hInstance);
  UnregisterClass(pcRePag_Datum, wndKlasse.hInstance);
  UnregisterClass(pcRePag_EditDatum, wndKlasse.hInstance);
  UnregisterClass(pcRePag_Leuchte, wndKlasse.hInstance);
  UnregisterClass(pcUnregister, wndKlasse.hInstance);
}
//---------------------------------------------------------------------------
