/****************************************************************************
  Media.cpp
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
#include "OVideo.h"

using namespace RePag::GUI;
//---------------------------------------------------------------------------
char pcRePag_Video[] = "RePag_Video";
char pcRePag_Audio[] = "RePag_Audio";
//---------------------------------------------------------------------------
void __vectorcall RegisterKlassenName(void)
{
  WNDCLASSEX wndKlasse;
  wndKlasse.cbSize = sizeof(WNDCLASSEX);
  wndKlasse.style = CS_OWNDC;
  wndKlasse.cbClsExtra = 0;
  wndKlasse.cbWndExtra = 16;
  wndKlasse.hInstance = nullptr;
  wndKlasse.hIcon = nullptr;
  wndKlasse.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wndKlasse.hbrBackground = nullptr;
  wndKlasse.lpszMenuName = nullptr;
  wndKlasse.hIconSm = nullptr;

  wndKlasse.lpszClassName = pcRePag_Video;
  wndKlasse.lpfnWndProc = WndProc_Video;
  RegisterClassEx(&wndKlasse);

  wndKlasse.lpszClassName = pcRePag_Audio;
  wndKlasse.lpfnWndProc = WndProc_Audio;
  RegisterClassEx(&wndKlasse);
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

  UnregisterClass(pcRePag_Video, wndKlasse.hInstance);
  UnregisterClass(pcRePag_Audio, wndKlasse.hInstance);
  UnregisterClass(pcUnregister, wndKlasse.hInstance);
}
//---------------------------------------------------------------------------