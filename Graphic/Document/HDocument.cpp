/****************************************************************************
  HDocument.cpp
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

#include "HDocument.h"

#pragma comment(lib, "CompSys")
#pragma comment(lib, "ADT")

void __vectorcall RegisterKlassenName(void);
void __vectorcall UnRegisterKlassenName(void);
//---------------------------------------------------------------------------
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ulReason, LPVOID lpReserved)
{
  HKEY hSchlussel; DWORD dwBytes, dwRegistryTyp; void* pvProcess; CLSID clsid; LPOLESTR polestrCLSID;
  wchar_t pwc60UnterSchlussel[60];
  MemCopy(pwc60UnterSchlussel, L"CLSID\\", 14);
  MemCopy(&pwc60UnterSchlussel[44], L"\\LocalServer32", 30);
  pwc60UnterSchlussel[59] = 0;

  switch(ulReason) {
    case DLL_PROCESS_ATTACH : RegisterKlassenName();
                              vthlProcess = COListV(VMDialog(), true);
                              if(!RegOpenKeyEx(HKEY_CLASSES_ROOT, "Software\\Adobe\\Acrobat\\Exe", 0, KEY_READ, &hSchlussel)){
                                if(!RegQueryValueEx(hSchlussel, nullptr, nullptr, &dwRegistryTyp, nullptr, &dwBytes)){
                                  vasAdobePfad = COStringAV(dwBytes);
                                  RegQueryValueEx(hSchlussel, nullptr, nullptr, &dwRegistryTyp, (PBYTE)vasAdobePfad->c_Str(), &dwBytes);
                                  vasAdobePfad->Delete(0, 1); vasAdobePfad->Delete(vasAdobePfad->Length() - 2, 2); *vasAdobePfad += " /s /o ";
                                }
                                RegCloseKey(hSchlussel);
                              }

                              CoInitialize(nullptr);
                              if(CLSIDFromProgID(L"Excel.Application", &clsid) == S_OK){
                                StringFromCLSID(clsid, &polestrCLSID);
                                MemCopy(&pwc60UnterSchlussel[6], polestrCLSID, 76);
                                if(!RegOpenKeyExW(HKEY_CLASSES_ROOT, pwc60UnterSchlussel, 0, KEY_READ, &hSchlussel)){
                                  if(!RegQueryValueEx(hSchlussel, nullptr, nullptr, &dwRegistryTyp, nullptr, &dwBytes)){
                                    vasExcelPfad = COStringAV(dwBytes);
                                    RegQueryValueEx(hSchlussel, nullptr, nullptr, &dwRegistryTyp, (PBYTE)vasExcelPfad->c_Str(), &dwBytes);
                                    vasExcelPfad->ShortRightOne(); *vasExcelPfad += " /e /r ";
                                  }
                                  RegCloseKey(hSchlussel);
                                }
                              }

                              if(CLSIDFromProgID(L"Word.Application", &clsid) == S_OK){
                                StringFromCLSID(clsid, &polestrCLSID);
                                MemCopy(&pwc60UnterSchlussel[6], polestrCLSID, 76);
                                if(!RegOpenKeyExW(HKEY_CLASSES_ROOT, pwc60UnterSchlussel, 0, KEY_READ, &hSchlussel)){
                                  if(!RegQueryValueEx(hSchlussel, nullptr, nullptr, &dwRegistryTyp, nullptr, &dwBytes)){
                                    vasWordPfad = COStringAV(dwBytes);
                                    RegQueryValueEx(hSchlussel, nullptr, nullptr, &dwRegistryTyp, (PBYTE)vasWordPfad->c_Str(), &dwBytes);
                                    vasWordPfad->ShortRightOne(); *vasWordPfad += " /q ";
                                  }
                                  RegCloseKey(hSchlussel);
                                }
                              }
                              CoUninitialize();
                              break;
    case DLL_THREAD_ATTACH  : 
    case DLL_THREAD_DETACH  : break;
    case DLL_PROCESS_DETACH : pvProcess = vthlProcess->ThIteratorToBegin();
                              while(pvProcess){
                                TerminateProcess(vthlProcess->Element(pvProcess), 0);
                                vthlProcess->DeleteFirstElement(pvProcess, false);
                              }
                              vthlProcess->ThIteratorEnd();
                              VMFreiV(vthlProcess);
                              UnRegisterKlassenName();
                              break;
  }
  return true;
}