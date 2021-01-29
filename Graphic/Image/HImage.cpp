/****************************************************************************
  HImage.cpph
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

#include "HImage.h"

#pragma comment(lib, "gdiplus")
#pragma comment(lib, "CompSys")
#pragma comment(lib, "ADT")

GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;
void __vectorcall RegisterKlassenName(void);
void __vectorcall UnRegisterKlassenName(void);
//---------------------------------------------------------------------------
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ulReason, LPVOID lpReserved)
{
  switch(ulReason){
    case DLL_PROCESS_ATTACH : RegisterKlassenName(); GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr); break;
    case DLL_THREAD_ATTACH  : 
    case DLL_THREAD_DETACH  : break;
    case DLL_PROCESS_DETACH : UnRegisterKlassenName(); GdiplusShutdown(gdiplusToken); break;
  }
  return true;
}