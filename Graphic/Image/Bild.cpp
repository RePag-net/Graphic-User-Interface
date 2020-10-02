/****************************************************************************
  Bild.cpp
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
#include "OBildBox.h"
#include "ODLGMCodec.h"

//---------------------------------------------------------------------------
char pcRePag_Bild[] = "RePag_Bild";
char pcRePag_BildBox[] = "RePag_BildBox";
//---------------------------------------------------------------------------
void __vectorcall RegisterKlassenName(void)
{
  WNDCLASSEX wndKlasse;
  wndKlasse.cbSize = sizeof(WNDCLASSEX);
  wndKlasse.style = CS_OWNDC;
  wndKlasse.cbClsExtra = 0;
  wndKlasse.cbWndExtra = 8;
  wndKlasse.hInstance = nullptr;
  wndKlasse.hIcon = nullptr;
  wndKlasse.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wndKlasse.hbrBackground = nullptr;
  wndKlasse.lpszMenuName = nullptr;
  wndKlasse.hIconSm = nullptr;

  wndKlasse.lpszClassName = pcRePag_Bild;
  wndKlasse.lpfnWndProc = WndProc_Bild;
  wndKlasse.style |= CS_HREDRAW | CS_VREDRAW;
  RegisterClassEx(&wndKlasse);
  wndKlasse.style = CS_OWNDC;

  wndKlasse.lpszClassName = pcRePag_BildBox;
  wndKlasse.lpfnWndProc = WndProc_BildBox;
  wndKlasse.style |= CS_HREDRAW | CS_VREDRAW;
  RegisterClassEx(&wndKlasse);
  wndKlasse.style = CS_OWNDC;
}
//---------------------------------------------------------------------------
void __vectorcall UnRegisterKlassenName(void)
{
  WNDCLASSEX wndKlasse; char pcUnregister[] = "Unregister";
  wndKlasse.cbSize = sizeof(WNDCLASSEX);
  wndKlasse.style = CS_OWNDC;
  wndKlasse.cbClsExtra = 0;
  wndKlasse.cbWndExtra = 0;
  wndKlasse.hInstance = nullptr;
  wndKlasse.hIcon = nullptr;
  wndKlasse.hCursor = nullptr;
  wndKlasse.hbrBackground = nullptr;
  wndKlasse.lpszMenuName = nullptr;
  wndKlasse.hIconSm = nullptr;
  wndKlasse.lpszClassName = pcUnregister;
  wndKlasse.lpfnWndProc = nullptr;
  RegisterClassEx(&wndKlasse);

  UnregisterClass(pcRePag_Bild, wndKlasse.hInstance);
  UnregisterClass(pcRePag_BildBox, wndKlasse.hInstance);
  UnregisterClass(pcUnregister, wndKlasse.hInstance);
}
//---------------------------------------------------------------------------
void __vectorcall CODEC(_In_ HWND hWnd)
{
  dlgmCodec = VMBlock(VMDialog(), dlgmCodec); dlgmCodec->CODLGMCodecV();
  dlgmCodec->ErstellFenster(hWnd, 355, 290, CW_USEDEFAULT, CW_USEDEFAULT);
  dlgmCodec->SetzSichtbar(true, DLG_LINKS | DLG_OBEN, NULL);
  DestroyWindow(dlgmCodec->HWND_Element());
}
//------------------------------------------------------------------------------------------------------------------------------------------