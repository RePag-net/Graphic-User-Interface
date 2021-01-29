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
  wndKlasse.cbWndExtra = 16;
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