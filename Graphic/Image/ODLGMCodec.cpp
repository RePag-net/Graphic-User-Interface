/****************************************************************************
  ODLGMCodec.cpp
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
#include "ODLGMCodec.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
CODLGMCodec* dlgmCodec;
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall fnCodec_WM_Paint(RePag::GUI::CODialog* pDialog, PAINTSTRUCT& stPaint)
{
 HFONT hFont = CreateFontIndirect(&pDialog->lfSchrift);	
 SelectObject(stPaint.hdc, hFont);
 SetBkMode(stPaint.hdc, TRANSPARENT); 
 SetTextColor(stPaint.hdc, RGB(230, 230, 230));
 TextOut(stPaint.hdc, 10, 10, "Auf diesem Computer installierte", 32);
 TextOut(stPaint.hdc, 10, 25, "Encoder", 7);
 TextOut(stPaint.hdc, 10, 155, "Decoder", 7);
 DeleteObject(hFont);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall fnCodec_WM_Command(HWND hWndDlg, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
 switch(LOWORD(wParam)){
     case IDE_CODEC_KSCHLIESSEN    : dlgmCodec->SetzSichtbar(false, NULL, IDE_CODEC_KSCHLIESSEN);
													           break;
										      default  : DefWindowProc(hWndDlg, uiMessage, wParam, lParam);
 }
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void __vectorcall CODLGMCodec::CODLGMCodecV(void)
{
 CODialogV("DLGMCodec", true, WndProc_DLGMCodec); FensterStil(WS_POPUP | WS_CAPTION); ErweitertStil(WS_EX_TOOLWINDOW | WS_EX_TOPMOST); FensterTitel("(De)Encoder");
 stHintergrundfarbe.ucRot = 80; stHintergrundfarbe.ucGrun = 80; stHintergrundfarbe.ucBlau = 80;
 pfnWM_Paint = fnCodec_WM_Paint;
 pfnWM_Command = fnCodec_WM_Command;

 UINT uiAnzahl, uiGrosse; ImageCodecInfo* pImageCodecInfo; COStringA* vasDeEncoder = COStringAV(); int iGrosse; VMBLOCK vbAnsiString;
 GetImageEncodersSize(&uiAnzahl, &uiGrosse);
 pImageCodecInfo = (ImageCodecInfo*)(VMBlock(uiGrosse));
 GetImageEncoders(uiAnzahl, uiGrosse, pImageCodecInfo);
 for(UINT uiCodec = 0; uiCodec < uiAnzahl; ++uiCodec){ 
 	 iGrosse = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, pImageCodecInfo[uiCodec].MimeType, -1, nullptr, 0, nullptr, nullptr); vbAnsiString = VMBlock(iGrosse);
   WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, pImageCodecInfo[uiCodec].MimeType, -1, vbAnsiString, iGrosse, nullptr, nullptr);
	 *vasDeEncoder += vbAnsiString; *vasDeEncoder += " - "; VMFrei(vbAnsiString);
	 iGrosse = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, pImageCodecInfo[uiCodec].FilenameExtension, -1, nullptr, 0, nullptr, nullptr); vbAnsiString = VMBlock(iGrosse);
   WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, pImageCodecInfo[uiCodec].FilenameExtension, -1, vbAnsiString, iGrosse, nullptr, nullptr);
	 *vasDeEncoder += vbAnsiString; *vasDeEncoder += "\n"; VMFrei(vbAnsiString);
 }
 VMFrei(pImageCodecInfo);
 tbEncoder = COTextBoxV("tbEncoder_DLGMCodec", IDE_TBENCODER);
 tbEncoder->Hintergrundfarbe(100, 100, 100);
 tbEncoder->Schriftfarbe(255, 255, 255);
 tbEncoder->Schriftart(lfSchrift);
 tbEncoder->Text(vasDeEncoder->c_Str());
 *vasDeEncoder = NULL;

 GetImageDecodersSize(&uiAnzahl, &uiGrosse);
 pImageCodecInfo = (ImageCodecInfo*)(VMBlock(uiGrosse));
 GetImageDecoders(uiAnzahl, uiGrosse, pImageCodecInfo);
 for(UINT uiCodec = 0; uiCodec < uiAnzahl; ++uiCodec){ 
 	 iGrosse = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, pImageCodecInfo[uiCodec].MimeType, -1, nullptr, 0, nullptr, nullptr); vbAnsiString = VMBlock(iGrosse);
   WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, pImageCodecInfo[uiCodec].MimeType, -1, vbAnsiString, iGrosse, nullptr, nullptr);
	 *vasDeEncoder += vbAnsiString; *vasDeEncoder += " - "; VMFrei(vbAnsiString);
	 iGrosse = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, pImageCodecInfo[uiCodec].FilenameExtension, -1, nullptr, 0, nullptr, nullptr); vbAnsiString = VMBlock(iGrosse);
   WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, pImageCodecInfo[uiCodec].FilenameExtension, -1, vbAnsiString, iGrosse, nullptr, nullptr);
	 *vasDeEncoder += vbAnsiString; *vasDeEncoder += "\n"; VMFrei(vbAnsiString);
 }
 VMFrei(pImageCodecInfo);
 tbDecoder = COTextBoxV("tbDecoder_DLGMCodec", IDE_TBDECODER);
 tbDecoder->Hintergrundfarbe(100, 100, 100);
 tbDecoder->Schriftfarbe(255, 255, 255);
 tbDecoder->Schriftart(lfSchrift);
 tbDecoder->Text(vasDeEncoder->c_Str());

 kSchliessen = COKnopfV("kSchliessen_DLGMCodec", IDE_CODEC_KSCHLIESSEN); kSchliessen->Beschriftung("Schliessen");
}
//-------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc_DLGMCodec(HWND hWndDlg, UINT uiMessage, WPARAM wParam, LPARAM lParam)
{ 
 switch(uiMessage){
		case WM_CREATE      : dlgmCodec->kSchliessen->ErstellFenster(hWndDlg, 25, 85, 175, 285);
			                    dlgmCodec->tbEncoder->ErstellFenster(hWndDlg, 98, 255, 10, 45);
                          dlgmCodec->tbDecoder->ErstellFenster(hWndDlg, 98, 255, 10, 175);
												  return NULL;
	case WM_NCDESTROY     : VMFreiV(dlgmCodec);
												  return NULL;
 }
 return CallWindowProc(dlgmCodec->pfnWndProc_Dialog, hWndDlg, uiMessage, wParam, lParam); 
}
//-------------------------------------------------------------------------------------------------------------------------------------------