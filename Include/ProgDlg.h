// up Fileversion 3.2.3.1
#ifndef ProgDlgH
#define ProgDlgH
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
  namespace GUI
  {
    //---------------------------------------------------------------------------------------------------------------------------------------
    constexpr BYTE DLG_LINKS = 1;
    constexpr BYTE DLG_RECHTS = 2;
    constexpr BYTE DLG_OBEN = 4;
    constexpr BYTE DLG_UNTEN = 8;
    constexpr BYTE DLG_MITTEVERTICAL = 16;
    constexpr BYTE DLG_MITTEHORIZONTAL = 32;
    constexpr BYTE DLG_MINIMAL = 64;
    constexpr BYTE DLG_MAXIMAL = 128;
    constexpr USHORT IDE_DLG_SICHTBAR = 0xFFFF;

    constexpr BYTE HGE_KEINHINTERGRUND = 0;
    constexpr BYTE HGE_HINTERGRUND = 1;
    constexpr BYTE HGE_BELEUCHTUNG_H = 2;
    constexpr BYTE HGE_BELEUCHTUNG_V = 4;
    constexpr BYTE HGE_RAND_H = 8;
    constexpr BYTE HGE_RAND_V = 16;
    //---------------------------------------------------------------------------------------------------------------------------------------
    typedef struct STRGBFarbe
    {
      unsigned char ucRot;
      unsigned char ucBlau;
      unsigned char ucGrun;
    } STRGBFarbe;
    //---------------------------------------------------------------------------------------------------------------------------------------
    class __declspec(dllexport) COElement
    {
    private:
      struct STThread
      {
        HANDLE heElement[2];
        DWORD dwHaupt;
        DWORD dwAktiv;
        unsigned char ucFunktion;
        unsigned char ucWarten;
      };
      STThread stThread;
      HBITMAP hbmBild;
      bool bHintergrund;
      unsigned char ucHintergrundeffekt;
      STRGBFarbe stHintergrundeffektfarbe;
      void __vectorcall Hintergrund(RECT& rcZeichnen);
      void __vectorcall Hintergrund_Effekt(RECT* prcZeichnen);
      void __vectorcall Effekt_Beleuchtung_Horizontal(void);
      void __vectorcall Effekt_Beleuchtung_Vertical(void);
      void __vectorcall Effekt_Rand_Horizontal(void);
      void __vectorcall Effekt_Rand_Vertical(void);

    protected:
      VMEMORY vmSpeicher;
      HWND hWndElement;
      HANDLE htEffekt_Timer;
      long lHohe;
      long lBreite;
      POINT ptPosition;
      STRGBFarbe stHintergrundfarbe;
      RECT rcEffektrand;
      unsigned char ucRahmenbreite;
      void __vectorcall WM_Size_Element(HWND hWnd, LPARAM lParam);
      void __vectorcall WM_Paint_Hintergrund(PAINTSTRUCT& stPaint);
      inline void __vectorcall ThreadSicher_Anfang(void);
      inline void __vectorcall ThreadSicher_Ende(void);
      void __vectorcall COElementV(VMEMORY vmSpeicherA);

    public:
      VMEMORY __vectorcall COFreiV(void);
      void(__vectorcall* pfnWM_Size)(COElement*, WPARAM, LPARAM);
      HWND __vectorcall HWND_Element(void);
      HWND __vectorcall HWND_Haupt(void);
      long& __vectorcall Breite(long& lBreiteA);
      long& __vectorcall Hohe(long& lHoheA);
      long& __vectorcall Pos_X(long& lPos_x);
      long& __vectorcall Pos_Y(long& lPos_y);
      POINT& __vectorcall Position(POINT& ptPositionA);
      RECT& __vectorcall Fenster(RECT& rcFenster);
      void __vectorcall NeueFensterPosition(long lPos_x, long lPos_y);
      void __vectorcall NeueFensterPosition(POINT& ptPositionA);
      void __vectorcall NeuesFenster(long lHoheA, long lBreiteA, long lPos_x, long lPos_y);
      void __vectorcall NeueFensterGrosse(long lHoheA, long lBreiteA);
      void __vectorcall NeueFensterHohe(long lHoheA);
      void __vectorcall NeueFensterBreite(long lBreiteA);
      void __vectorcall AndernFensterGrosse(long lHoheA, long lBreiteA);
      void __vectorcall AndernFensterPosition(long lPos_x, long lPos_y);
      void __vectorcall Hintergrundfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
      void __vectorcall Hintergrundeffekt(unsigned char ucEffekt);
      void __vectorcall Hintergrundeffektfarbe(unsigned char ucRot, unsigned char ucGrun, unsigned char ucBlau);
      void __vectorcall Hintergrundeffektrand(long lOben, long lUnten, long lLinks, long lRechts);
      void __vectorcall Effekt_Timer(bool bStarten, unsigned long ulInterval_msek, void CALLBACK fnEffekt_Timer(COElement*, bool bTimerOrWaitFired));
      void __vectorcall UpdateFenster(RECT* prcFenster, bool bUpdateHintergrund, bool bUpdateHintergrundeffekt);
      void __vectorcall SetzHintergrund(bool bMitHintergrund);
      void __vectorcall SetzVerfugbar(bool bVerfugbar);
      bool __vectorcall Verfugbar(void);
      void __vectorcall Loschen(void);

    };
    //---------------------------------------------------------------------------------------------------------------------------------------
    class __declspec(dllexport) CODialog : public COElement
    {
      friend LRESULT CALLBACK WndProc_Dialog(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);
      friend DWORD WINAPI thWM_Command_Dialog(void* pvParam);

    private:
      struct STFensterBau
      {
        COStringA asName;
        VMBLOCK vbTitel;
        DWORD dwFensterStil;
        DWORD dwErweitertStil;
        WNDCLASSEX wndKlasse;
      };
      STFensterBau* vstFensterBau;
      long lRuckgabe;
      bool bModal;
      DWORD dwThreadID;
      void __vectorcall WM_Move_Dialog(void);
      void __vectorcall WM_Paint_Dialog(void);
      void __vectorcall WM_Command_Dialog(unsigned int uiMessage, WPARAM wParam, LPARAM lParam);
      LRESULT(CALLBACK* pfnWndProc_DLGDialog)(HWND, unsigned int, WPARAM, LPARAM);

    protected:
      void __vectorcall FensterStil(DWORD dwFensterStilA);
      void __vectorcall ErweitertStil(DWORD dwErweitertStilA);
      void __vectorcall FensterTitel(const char* pcFensterTitel);
      void __vectorcall Icon(long lIcon);
      void __vectorcall IconSm(long lIconSm);
      void __vectorcall Menu(long lMenu);

    public:
      void __vectorcall CODialogV(const char* pcFensterName, bool bModalA, LRESULT CALLBACK WndProc_DLG(HWND, unsigned int, WPARAM, LPARAM));
      void __vectorcall CODialogV(VMEMORY vmSpeicher, const char* pcFensterName, bool bModalA, LRESULT CALLBACK WndProc_DLG(HWND, unsigned int, WPARAM, LPARAM));
      VMEMORY __vectorcall COFreiV(void);
      LRESULT(CALLBACK* pfnWndProc_Dialog)(HWND, unsigned int, WPARAM, LPARAM);
      void(__vectorcall* pfnWM_Paint)(CODialog*, PAINTSTRUCT&);
      void(__vectorcall* pfnWM_Move)(CODialog*, LPARAM);
      void (__vectorcall* pfnWM_Command)(HWND, unsigned int, WPARAM, LPARAM);
      void __vectorcall ErstellFenster(HWND hWndHaupt, long lHoheA, long lBreiteA, long lPos_x, long lPos_y);
      long __vectorcall SetzSichtbar(bool bSichtbar, unsigned char ucAusrichtung, long lRuckgabeA);
      void __vectorcall Schliessen(void);
      LOGFONT lfSchrift;

    };
    //---------------------------------------------------------------------------------------------------------------------------------------
    class __declspec(dllimport) COGrafik : public COElement
    {
    private:
      struct STFensterBau
      {
        VMBLOCK vbKlassenName;
        COStringA asFensterName;
        DWORD dwFensterStil;
        DWORD dwErweitertStil;
        unsigned int uiIDElement;
      };
      STFensterBau* vstFensterBau;

    protected:
      void __vectorcall COGrafikV(VMEMORY vmSpeicher, const char* pcKlassenName, const char* pcFensterName, unsigned int uiIDElementA);

    public:
      void __vectorcall FensterStil(DWORD dwFensterStilA);
      void __vectorcall ErweitertStil(DWORD dwErweitertStilA);
      void __vectorcall ErstellFenster(HWND hWndHaupt, long lHoheA, long lBreiteA, long lPos_x, long lPos_y);
      unsigned int __vectorcall IDElement(void);
      void __vectorcall SetzSichtbar(bool bSichtbar);
      bool __vectorcall Sichtbar(void);
    };
    //---------------------------------------------------------------------------------------------------------------------------------------
    class __declspec(dllimport) COGrafikGruppe : public COGrafik
    {
      friend LRESULT CALLBACK WndProc_GrafikGruppe(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam);
      friend DWORD WINAPI thWM_Command_GrafikGruppe(void* pvParam);

    private:
      void __vectorcall WM_Paint(void);
      void __vectorcall WM_Command_GrafikGruppe(unsigned int uiMessage, WPARAM wParam, LPARAM lParam);

    protected:

    public:
      void __vectorcall COGrafikGruppeV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElementA);
      void __vectorcall ErstellFensterInRahmen(COGrafik* pGrafik);
      void __vectorcall ErstellFensterInGruppe(COGrafik* pGrafik, long lHoheA, long lBreiteA, long lPos_x, long lPos_y);
      void(__vectorcall* pfnWM_Paint)(COGrafikGruppe*, PAINTSTRUCT&);
      void(__vectorcall* pfnWM_Command)(HWND, unsigned int, WPARAM, LPARAM);
      void __vectorcall SetzSichtbar(bool bSichtbar);
    };
    __declspec(dllimport) COGrafikGruppe* __vectorcall COGrafikGruppeV(const char* pcFensterName, unsigned int uiIDElement);
    __declspec(dllimport) COGrafikGruppe* __vectorcall COGrafikGruppeV(VMEMORY vmSpeicher, const char* pcFensterName, unsigned int uiIDElement);
    //---------------------------------------------------------------------------------------------------------------------------------------
    __declspec(dllimport) VMEMORY __vectorcall VMDialog(void);
    __declspec(dllimport) HINSTANCE __vectorcall Hinstance(void);
    __declspec(dllimport) HACCEL __vectorcall Accelerator(void);
    __declspec(dllimport) HANDLE __vectorcall TimerQueue(void);
    __declspec(dllimport) void __vectorcall Cursorform(const char* pcCursor);
    __declspec(dllimport) void __vectorcall EinfugenKurzTasten(const ACCEL* pacTasten, int iAnzahl);
    //---------------------------------------------------------------------------------------------------------------------------------------
  }
}
#endif