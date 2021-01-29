/****************************************************************************
  ProgDlg.cpp
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

#include "HProgDlg.h"
#include "OGrafikGruppe.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
HINSTANCE hInstance;
VMEMORY vmDialog;
HACCEL hAccelerator;
HANDLE htqTimerQueue;
COList* vthlWndKlassen;
HANDLE heWndKlassen;
HANDLE hthWndKlassen;
STTHWndKlassen thstWndKlassen;
COList* vthlDialoge;
COList* vthlThreadId;
char pcRePag_GrafikGruppe[] = "RePag_GrafikGruppe";
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePagPrg_Start(void)
{
	SYSTEM_INFO stSystem_Info; GetSystemInfo(&stSystem_Info);
	CPUID(stSystem_Info);

  ULONG ulMinimumWorkingSetSize, ulMaximumWorkingSetSize;
  HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
  ulMinimumWorkingSetSize = 12288000; ulMaximumWorkingSetSize = 20480000;
  SetProcessWorkingSetSize(hProcess, ulMinimumWorkingSetSize, ulMaximumWorkingSetSize);

  vmDialog = InitVirtualMem(true, "Dialoge");

  WNDCLASSEX wndKlasse;
  wndKlasse.cbSize = sizeof(WNDCLASSEX);
  wndKlasse.style = CS_OWNDC;
  wndKlasse.cbClsExtra = 0;
  wndKlasse.cbWndExtra = 16;
  wndKlasse.hInstance = NULL;
  wndKlasse.hIcon = NULL;
  wndKlasse.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndKlasse.hbrBackground = NULL;
  wndKlasse.lpszMenuName = NULL;
  wndKlasse.hIconSm = NULL;
  wndKlasse.lpszClassName = pcRePag_GrafikGruppe;
  wndKlasse.lpfnWndProc = WndProc_GrafikGruppe;
  RegisterClassEx(&wndKlasse);

  hInstance = wndKlasse.hInstance;

  ACCEL accel7[7];
  accel7[0].fVirt = FVIRTKEY | FCONTROL;
  accel7[0].key = 0x58;
  accel7[0].cmd = IDM_AUSSCHNEIDEN;

  accel7[1].fVirt = FVIRTKEY | FCONTROL;
  accel7[1].key = 0x43;
  accel7[1].cmd = IDM_KOPIEREN;

  accel7[2].fVirt = FVIRTKEY | FCONTROL;
  accel7[2].key = 0x56;
  accel7[2].cmd = IDM_EINFUGEN;

  accel7[3].fVirt = FVIRTKEY | FCONTROL;
  accel7[3].key = 0x4f;
  accel7[3].cmd = IDM_DATEI_OFFNEN;

  accel7[4].fVirt = FVIRTKEY | FCONTROL;
  accel7[4].key = 0x53;
  accel7[4].cmd = IDM_DATEI_SPEICHERN;

  accel7[5].fVirt = FVIRTKEY | FCONTROL | FSHIFT;
  accel7[5].key = VK_DELETE;
  accel7[5].cmd = IDM_DATEI_ENTFERNEN;

  accel7[6].fVirt = FVIRTKEY | FCONTROL;
  accel7[6].key = 0x4c;
  accel7[6].cmd = IDM_DATEI_LADEN;

  hAccelerator = CreateAcceleratorTable(accel7, 7);
  htqTimerQueue = CreateTimerQueue();

  vthlDialoge = COListV(vmDialog, true);
  vthlThreadId = COListV(vmDialog, true);
  vthlWndKlassen = COListV(true);
  InitializeCriticalSectionAndSpinCount(&thstWndKlassen.csWndKlasse, 1000);
  thstWndKlassen.bAbbruch = false;
  heWndKlassen = CreateEvent(NULL, false, false, NULL);
  hthWndKlassen = CreateThread(NULL, 0, thLoschWndKlassen, &thstWndKlassen, CREATE_SUSPENDED, NULL);
  if(hthWndKlassen){ SetThreadPriority(hthWndKlassen, THREAD_PRIORITY_IDLE); ResumeThread(hthWndKlassen); }
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePagPrg_Ende(void)
{
  DestroyAcceleratorTable(hAccelerator);
  DeleteTimerQueueEx(htqTimerQueue, INVALID_HANDLE_VALUE);
  UnregisterClass(pcRePag_GrafikGruppe, hInstance);

  EnterCriticalSection(&thstWndKlassen.csWndKlasse);
  thstWndKlassen.bAbbruch = true;
  LeaveCriticalSection(&thstWndKlassen.csWndKlasse);

  SetEvent(heWndKlassen);
  Sleep(500);
  CloseHandle(hthWndKlassen);

  VMFreiV(vthlDialoge); VMFreiV(vthlThreadId);
  FreeVirtualMem(vmDialog);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
VMEMORY __vectorcall RePag::GUI::VMDialog(void)
{
  return vmDialog;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
HINSTANCE __vectorcall RePag::GUI::Hinstance(void)
{
  return hInstance;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
HACCEL __vectorcall RePag::GUI::Accelerator(void)
{
  return hAccelerator;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
HANDLE __vectorcall RePag::GUI::TimerQueue(void)
{
  return htqTimerQueue;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::Cursorform(const char* pcCursor)
{
  (pcCursor == NULL ? SetCursor(NULL) : SetCursor(LoadCursor(hInstance, pcCursor)));
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall RePag::GUI::EinfugenKurzTasten(const ACCEL* pacTasten, int iAnzahl)
{
  int iKurzTasten = CopyAcceleratorTable(hAccelerator, NULL, 0);
  ACCEL* vacTasten = (ACCEL*)malloc(sizeof(ACCEL) * (iKurzTasten + iAnzahl));
  CopyAcceleratorTable(hAccelerator, vacTasten, iKurzTasten);
  DestroyAcceleratorTable(hAccelerator);
  MemCopy(&vacTasten[iKurzTasten], pacTasten, sizeof(ACCEL) * iAnzahl);
  hAccelerator = CreateAcceleratorTable(vacTasten, iKurzTasten + iAnzahl);
  free(vacTasten);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall LoschWndKlasse(ATOM atWndKlasse)
{
  ATOM* patWndKlasse = (ATOM*)VMBlock(2); MemCopy(patWndKlasse, &atWndKlasse, 2);
  vthlWndKlassen->ThToEnd(patWndKlasse);
  SetEvent(heWndKlassen);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
DWORD WINAPI thLoschWndKlassen(void* pvParam)
{
  void* pvIterator;
  TryEnterCriticalSection(&thstWndKlassen.csWndKlasse);
  while(!((STTHWndKlassen*)pvParam)->bAbbruch){
    LeaveCriticalSection(&thstWndKlassen.csWndKlasse);
    pvIterator = vthlWndKlassen->ThIteratorToBegin_Lock();
    while(pvIterator){
      if(UnregisterClass(MAKEINTATOM(*((ATOM*)vthlWndKlassen->Element(pvIterator))), hInstance)) vthlWndKlassen->DeleteFirstElement(pvIterator, true);
    }
    vthlWndKlassen->ThIteratorEnd();
    WaitForSingleObject(heWndKlassen, INFINITE);
    TryEnterCriticalSection(&thstWndKlassen.csWndKlasse);
  }

  LeaveCriticalSection(&thstWndKlassen.csWndKlasse);
  DeleteCriticalSection(&thstWndKlassen.csWndKlasse);
  CloseHandle(heWndKlassen);
  vthlWndKlassen->ThDeleteList(false);
  VMFreiV(vthlWndKlassen);
  return NULL;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
