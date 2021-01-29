// up Fileversion 2.3.7.0
#ifndef ADTH
#define ADTH
#ifndef CompSysH
#include <windows.h>
#include <immintrin.h>
    using VMEMORY = const void*;
    using VMBLOCK = char*;
    using BIT128 = unsigned char[16];
    using BIT256 = unsigned char[32];
    using PBIT128 = BIT128*;
    using PBIT256 = BIT256*;
    using BIT128A = __declspec(align(16)) BIT128;
    using BIT256A = __declspec(align(32)) BIT256;
#endif

namespace RePag
{
  namespace System
  {
    //---------------------------------------------------------------------------
    class __declspec(dllimport) COComma4
    {
    private:
      struct STComma4
      {
        long lPreComma;
        long lPreComma_A;
        short sAftercomma;
        short sAfterComma_A;
      };
      char c12Comma4[12];
      VMEMORY vmMemory;

    public:
      __thiscall COComma4(void);
      __thiscall COComma4(_In_ const double dNumber);
      __thiscall COComma4(_In_ const int iNumber);
      __thiscall COComma4(_In_ const COComma4& k4Number);
      __thiscall COComma4(_In_ const __m128i m128iNumber); // use only Doubleword
      __thiscall COComma4(_In_ const __m128d m128dNumber); // Note range of values
      VMEMORY __vectorcall COFreiV(void);
      void __vectorcall operator =(_In_ const double dNumber);
      void __vectorcall operator =(_In_ const int iNumber);
      void __vectorcall operator =(_In_ const COComma4& k4Number);
      void __vectorcall operator =(_In_ const __m128i m128iNumber); // use only Doubleword
      void __vectorcall operator =(_In_ const __m128d m128dNumber); // Note range of values
      void __vectorcall operator +=(_In_ const COComma4& k4Number);
      COComma4& __vectorcall operator +(_In_ const COComma4& k4Number);
      void __vectorcall operator -=(_In_ const COComma4& k4Number);
      COComma4& __vectorcall operator -(_In_ const COComma4& k4Number);
      void __vectorcall operator *=(_In_ const COComma4& k4Number);
      COComma4& __vectorcall operator *(_In_ const COComma4& k4Number);
      void __vectorcall operator /=(_In_ const COComma4& k4Number);
      COComma4& __vectorcall operator /(_In_ const COComma4& k4Number);
      bool __vectorcall operator <(_In_ const COComma4& k4Number);
      bool __vectorcall operator >(_In_ const COComma4& k4Number);
      bool __vectorcall operator <=(_In_ const COComma4& k4Number);
      bool __vectorcall operator >=(_In_ const COComma4& k4Number);
      bool __vectorcall operator ==(_In_ const COComma4& k4Number);
      bool __vectorcall operator !=(_In_ const COComma4& k4Number);
      void __vectorcall operator ++(void);
      void __vectorcall operator ++(_In_ int i);
      void __vectorcall operator --(void);
      void __vectorcall operator --(_In_ int i);
      char __vectorcall Compare(_In_ const COComma4* pk4Number);
      void __vectorcall Write(_In_reads_(6) const char c6Number[6]);
      void __vectorcall Read(_Out_writes_(6) const char c6Number[6]);
      long __vectorcall PreComma(void);
      short __vectorcall AfterComma(void);
      float __vectorcall FLOAT(void);
      double __vectorcall DOUBLE(void);
      __m128d __vectorcall M128D(void);
      void __vectorcall SetZero(void);
      COComma4* __vectorcall Round(_In_ const unsigned char ucStellen);
      void __vectorcall PresignChange(void);
      COComma4* __vectorcall pi(void);
      COComma4* __vectorcall pi_10e8(void);
      double __vectorcall sin(void);
      double __vectorcall cos(void);
      double __vectorcall tan(void);
      double __vectorcall Squareroot(void);

    };
    __declspec(dllimport) COComma4* __vectorcall COComma4V(void);
    __declspec(dllimport) COComma4* __vectorcall COComma4V(_In_ const VMEMORY vmMemory);
    __declspec(dllimport) COComma4* __vectorcall COComma4V(_In_ const double dNumber);
    __declspec(dllimport) COComma4* __vectorcall COComma4V(_In_ const VMEMORY vmMemory, _In_ const double dNumber);
    __declspec(dllimport) COComma4* __vectorcall COComma4V(_In_ const int iNumber);
    __declspec(dllimport) COComma4* __vectorcall COComma4V(_In_ const VMEMORY vmMemory, _In_ const int iNumber);
    __declspec(dllimport) COComma4* __vectorcall COComma4V(_In_ const COComma4* pk4Number);
    __declspec(dllimport) COComma4* __vectorcall COComma4V(_In_ const VMEMORY vmMemory, _In_ const COComma4* pk4Number);
    __declspec(dllimport) COComma4* __vectorcall COComma4V(_In_ const __m128i m128iNumber);                                // Note range of values
    __declspec(dllimport) COComma4* __vectorcall COComma4V(_In_ const VMEMORY vmMemory, _In_ const __m128i m128iNumber);   // Note range of values
    __declspec(dllimport) COComma4* __vectorcall COComma4V(_In_ const __m128d m128dNumber);                                // Note range of values
    __declspec(dllimport) COComma4* __vectorcall COComma4V(_In_ const VMEMORY vmMemory, _In_ const __m128d m128dNumber);   // Note range of values
    //---------------------------------------------------------------------------
    class __declspec(dllimport) COComma4_80
    {
    private:
      struct STComma4_80
      {
        long long llPreComma;
        long long llPreComma_A;
        short sAfterComma;
        short sAfterComma_A;
      };
      char c20Comma4_80[20];
      VMEMORY vmMemory;

    public:
      __thiscall COComma4_80(void);
      __thiscall COComma4_80(_In_ const double dNumber);
      __thiscall COComma4_80(_In_ const int iNumber);
      __thiscall COComma4_80(_In_ const long long llNumber);
      __thiscall COComma4_80(_In_ const COComma4& k4Number);
      __thiscall COComma4_80(_In_ const COComma4_80& k4_80Number);
      __thiscall COComma4_80(_In_ const __m128i m128iNumber); // use only Quadword
      __thiscall COComma4_80(_In_ const __m128d m128dNumber); // Note range of values
      VMEMORY __vectorcall COFreiV(void);
      void __vectorcall operator =(_In_ const double dNumber);
      void __vectorcall operator =(_In_ const int iNumber);
      void __vectorcall operator =(_In_ const long long llNumber);
      void __vectorcall operator =(_In_ const COComma4& k4Number);
      void __vectorcall operator =(_In_ const COComma4_80& k4_80Number);
      void __vectorcall operator =(_In_ const __m128i m128iNumber); // use only Quadword
      void __vectorcall operator =(_In_ const __m128d m128dNumber); // Note range of values
      void __vectorcall operator +=(_In_ const COComma4& k4Number);
      void __vectorcall operator +=(_In_ const COComma4_80& k4_80Number);
      COComma4_80& __vectorcall operator +(_In_ const COComma4& k4Number);
      COComma4_80& __vectorcall operator +(_In_ const COComma4_80& k4_80Number);
      void __vectorcall operator -=(_In_ const COComma4& k4Number);
      void __vectorcall operator -=(_In_ const COComma4_80& k4_80Number);
      COComma4_80& __vectorcall operator -(_In_ const COComma4& k4Number);
      COComma4_80& __vectorcall operator -(_In_ const COComma4_80& k4_80Number);
      void __vectorcall operator *=(_In_ const COComma4& k4Number);
      void __vectorcall operator *=(_In_ const COComma4_80& k4_80Number);
      COComma4_80& __vectorcall operator *(_In_ const COComma4& k4Number);
      COComma4_80& __vectorcall operator *(_In_ const COComma4_80& k4_80Number);
      void __vectorcall operator /=(_In_ const COComma4& k4Number);
      void __vectorcall operator /=(_In_ const COComma4_80& k4_80Number);
      COComma4_80& __vectorcall operator /(_In_ const COComma4& k4Number);
      COComma4_80& __vectorcall operator /(_In_ const COComma4_80& k4_80Number);
      bool __vectorcall operator <(_In_ const COComma4& k4Number);
      bool __vectorcall operator <(_In_ const COComma4_80& k4_80Number);
      bool __vectorcall operator >(_In_ const COComma4& k4Number);
      bool __vectorcall operator >(_In_ const COComma4_80& k4_80Number);
      bool __vectorcall operator <=(_In_ const COComma4& k4Number);
      bool __vectorcall operator <=(_In_ const COComma4_80& k4_80Number);
      bool __vectorcall operator >=(_In_ const COComma4& k4Number);
      bool __vectorcall operator >=(_In_ const COComma4_80& k4_80Number);
      bool __vectorcall operator ==(_In_ const COComma4& k4Number);
      bool __vectorcall operator ==(_In_ const COComma4_80& k4_80Number);
      bool __vectorcall operator !=(_In_ const COComma4& k4Number);
      bool __vectorcall operator !=(_In_ const COComma4_80& k4_80Number);
      void __vectorcall operator ++(void);
      void __vectorcall operator ++(_In_ int i);
      void __vectorcall operator --(void);
      void __vectorcall operator --(_In_ int i);
      char __vectorcall Compare(_In_ const COComma4* pk4Number);
      char __vectorcall Compare(_In_ const COComma4_80* pk4_80Number);
      void __vectorcall Write(_Out_writes_(10) const char c10Number[10]);
      void __vectorcall Read(_In_reads_(10) const char c10Number[10]);
      long long __vectorcall PreComma(void);
      short __vectorcall AfterComma(void);
      float __vectorcall FLOAT(void);
      double __vectorcall DOUBLE(void);
      __m128d __vectorcall M128D(void);
      void __vectorcall SetZero(void);
      COComma4_80* __vectorcall Round(_In_ const unsigned char ucStellen);
      void __vectorcall PresignChange(void);
      COComma4_80* __vectorcall pi(void);
      COComma4_80* __vectorcall pi_10e18(void);
      double __vectorcall sin(void);
      double __vectorcall cos(void);
      double __vectorcall tan(void);
      double __vectorcall Squareroot(void);

    };
    //---------------------------------------------------------------------------
    __declspec(dllimport) COComma4_80* __vectorcall COComma4_80V(void);
    __declspec(dllimport) COComma4_80* __vectorcall COComma4_80V(_In_ const VMEMORY vmMemory);
    __declspec(dllimport) COComma4_80* __vectorcall COComma4_80V(_In_ const double dNumber);
    __declspec(dllimport) COComma4_80* __vectorcall COComma4_80V(_In_ const VMEMORY vmMemory, _In_ const double dNumber);
    __declspec(dllimport) COComma4_80* __vectorcall COComma4_80V(_In_ const int iNumber);
    __declspec(dllimport) COComma4_80* __vectorcall COComma4_80V(_In_ const VMEMORY vmMemory, _In_ const int iNumber);
    __declspec(dllimport) COComma4_80* __vectorcall COComma4_80V(_In_ const long long llNumber);
    __declspec(dllimport) COComma4_80* __vectorcall COComma4_80V(_In_ const VMEMORY vmMemory, _In_ const long long llNumber);
    __declspec(dllimport) COComma4_80* __vectorcall COComma4_80V(_In_ const COComma4* pk4Number);
    __declspec(dllimport) COComma4_80* __vectorcall COComma4_80V(_In_ const VMEMORY vmMemory, _In_ COComma4* pk4Number);
    __declspec(dllimport) COComma4_80* __vectorcall COComma4_80V(_In_ const COComma4_80* pk4_80Number);
    __declspec(dllimport) COComma4_80* __vectorcall COComma4_80V(_In_ const VMEMORY vmMemory, _In_ const COComma4_80* pk4_80Number);
    __declspec(dllimport) COComma4_80* __vectorcall COComma4_80V(_In_ const __m128i m128iNumber);                                // use only Quadword
    __declspec(dllimport) COComma4_80* __vectorcall COComma4_80V(_In_ const VMEMORY vmMemory, _In_ const __m128i m128iNumber);   // use only Quadword
    __declspec(dllimport) COComma4_80* __vectorcall COComma4_80V(_In_ const __m128d m128dNumber);                                // Note range of values
    __declspec(dllimport) COComma4_80* __vectorcall COComma4_80V(_In_ const VMEMORY vmMemory, _In_ const __m128d m128dNumber);   // Note range of values
    //---------------------------------------------------------------------------
    class __declspec(dllimport) COStringA
    {
    private:
      struct STStringA
      {
        VMBLOCK vbInhalt;
        VMBLOCK vbInhalt_A;
        unsigned long ulLange;
        unsigned long ulLange_A;
      };
#ifndef _64bit
      char c16StringA[16];
#else
      char c16StringA[24];
#endif
      VMEMORY vmMemory;

    public:
      __thiscall COStringA(void);
      __thiscall COStringA(_In_ VMEMORY vmSpeicher);
      __thiscall COStringA(_In_z_ const char* pcString);
      __thiscall COStringA(_In_ VMEMORY vmSpeicher, _In_z_ const char* pcString);
      __thiscall ~COStringA(void);
      VMEMORY __vectorcall COFreiV(void);
      void __vectorcall operator =(_In_z_ const char* pcString);
      void __vectorcall operator =(_In_ const COStringA& asString);
      void __vectorcall operator +=(_In_z_ const char* pcString);
      void __vectorcall operator +=(_In_ const COStringA& asString);
      COStringA& __vectorcall operator +(_In_z_ const char* pcString);
      COStringA& __vectorcall operator +(_In_ const COStringA& asString);
      bool __vectorcall operator ==(_In_z_ const char* pcString);
      bool __vectorcall operator ==(_In_ const COStringA& asString);
      bool __vectorcall operator !=(_In_z_ const char* pcString);
      bool __vectorcall operator !=(_In_ const COStringA& asString);
      bool __vectorcall operator <(_In_z_ const char* pcString);
      bool __vectorcall operator <(_In_ const COStringA& asString);
      bool __vectorcall operator >(_In_z_ const char* pcString);
      bool __vectorcall operator >(_In_ const COStringA& asString);
      char& __vectorcall operator [](_In_ const unsigned long ulIndex);
      bool __vectorcall Contain(_In_z_ const char* pcString);
      bool __vectorcall Contain(_In_ const COStringA& asString);
      bool __vectorcall ContainLeft(_In_z_ const char* pcString);
      bool __vectorcall ContainLeft(_In_ const COStringA& asString);
      bool __vectorcall ContainRight(_In_z_ const char* pcString);
      bool __vectorcall ContainRight(_In_ const COStringA& asString);
      unsigned long __vectorcall SubString(_In_ VMBLOCK& vbString, _In_ const unsigned long ulFrom, _In_ const unsigned long ulTo);
      COStringA* __vectorcall SubString(_In_ const COStringA* pasString, _In_ const unsigned long ulFrom, _In_ const unsigned long ulTo);
      COStringA* __vectorcall Insert(_In_z_ const char* pcString, _In_ const unsigned long ulPosition);
      COStringA* __vectorcall Insert(_In_ const COStringA* pasString, _In_ const unsigned long ulPosition);
      COStringA* __vectorcall Delete(_In_ const unsigned long ulPosition, _In_ const unsigned long ulNumber);
      unsigned long __vectorcall SearchCharacters(_In_z_ const char* pcCharacters);
      unsigned long __vectorcall SearchCharacters(_In_z_ const char* pcCharacters, _In_ const unsigned long ulFrom, _In_ const unsigned long ulTo);
      void __vectorcall ShortRight(_In_ const unsigned long ulStrLength);
      void __vectorcall ShortLeft(_In_ const unsigned long ulStrLength);
      void __vectorcall ShortRightOne(void);
      char& __vectorcall CHAR(_Out_ char& cNumber);
      unsigned char& __vectorcall BYTE(_Out_ unsigned char& ucNumber);
      short& __vectorcall SHORT(_Out_ short& sNumber);
      unsigned short& __vectorcall USHORT(_Out_ unsigned short& usNumber);
      long& __vectorcall LONG(_Out_ long& lNumber);
      unsigned long& __vectorcall ULONG(_Out_ unsigned long& ulNumber);
      long long& __vectorcall LONGLONG(_Out_ long long& llNumber);
      float& __vectorcall FLOAT(_Out_ float& fNumber);
      double& __vectorcall DOUBLE(_Out_ double& dNumber);
      COComma4* __vectorcall COMMA4(_Out_ COComma4* pk4Number);
      COComma4_80* __vectorcall COMMA4_80(_Out_ COComma4_80* pk4gNumber);
      BIT128& __vectorcall BIT128fromGUID(BIT128& bit128Zahl);
      bool __vectorcall IsIntegralNumber(void);
      bool __vectorcall IsFloatingPointNumber(void);
      void __vectorcall Uppercase(void);
      unsigned long __vectorcall Length(void);
      void __vectorcall SetLength(_In_ const unsigned long ulStrLength);
      char* __vectorcall c_Str(void);

    };
    __declspec(dllimport) COStringA* __vectorcall COStringAV(void);
    __declspec(dllimport) COStringA* __vectorcall COStringAV(_In_ const VMEMORY vmMemory);
    __declspec(dllimport) COStringA* __vectorcall COStringAV(_In_z_ const char* pcString);
    __declspec(dllimport) COStringA* __vectorcall COStringAV(_In_ const VMEMORY vmMemory, _In_z_ const char* pcString);
    __declspec(dllimport) COStringA* __vectorcall COStringAV(_In_ const COStringA* pasString);
    __declspec(dllimport) COStringA* __vectorcall COStringAV(_In_ const VMEMORY vmMemory, _In_ const COStringA* pasString);
    __declspec(dllimport) COStringA* __vectorcall COStringAV(_In_ const unsigned long ulStrLength);
    __declspec(dllimport) COStringA* __vectorcall COStringAV(_In_ const VMEMORY vmMemory, _In_ const unsigned long ulStrLength);
    __declspec(dllimport) COStringA* __vectorcall COStringAV_A(_In_ const char* pcString);
    //---------------------------------------------------------------------------
    class __declspec(dllimport) COList
    {
    private:
      struct STKnoten
      {
        STKnoten* pstNachster;
        void* pvDaten;
      };
      STKnoten* pstErster;
      STKnoten* pstLetzter;
      CRITICAL_SECTION csIterator;
      ULONG ulAnzahl;
      bool bThread;
      VMEMORY vmMemory;

    public:
      __thiscall COList(_In_ bool bThreadSafe);
      __thiscall COList(_In_ bool bThreadSafe, _In_ unsigned long ulSpinCount);
      __thiscall COList(_In_ VMEMORY vmMemory, _In_ bool bThreadSafe);
      __thiscall COList(_In_ VMEMORY vmMemory, _In_ bool bThreadSafe, _In_ unsigned long ulSpinCount);
      __thiscall ~COList(void);
      VMEMORY __vectorcall COFreiV(void);
      void* __vectorcall ToBegin(_In_ void* pvData);
      void* __vectorcall ToBeginS(_In_ void* pvData);
      void* __vectorcall ToEnd(_In_ void* pvData);
      void* __vectorcall ToEndS(_In_ void* pvData);
      void* __vectorcall Element(_In_ ULONG ulIndex);
      void* __vectorcall Element(_In_ void* pstNode);
      void __vectorcall NextElement(_In_ void*& pstNode);
      void __vectorcall NextElement(_In_ void*& pstCurrentlyNode, _In_ void*& pstPrerorNode);
      void* __vectorcall Insert(_In_ void*& pstCurrentlyNode, _In_ void*& pstPrerorNode, _In_ void* pvData);
      void* __vectorcall InsertS(_In_ void*& pstCurrentlyNode, _In_ void*& pstPrerorNode, _In_ void* pvData);
      void __vectorcall DeleteElement(_In_ ULONG ulIndex, _In_ bool bDeleteData);
      void __vectorcall DeleteElementS(_In_ ULONG ulIndex, _In_ bool bDatenLoschen);
      void __vectorcall DeleteElement(_In_ void*& pstNode, _In_ void*& pstDelete, _In_ bool bDeleteData);
      void __vectorcall DeleteElementS(_In_ void*& pstNode, _In_ void*& pstDelete, _In_ bool bDeleteData);
      void __vectorcall DeleteFirstElement(_In_ void*& pstNode, _In_ bool bDeleteData);
      void __vectorcall DeleteFirstElementS(_In_ void*& pstKnoten, _In_ bool bDatenLoschen);
      void __vectorcall DeleteList(_In_ bool bDeleteData);
      void __vectorcall DeleteListS(_In_ bool bDeleteData);
      void* __vectorcall ThToBegin(_In_ void* pvData);
      void* __vectorcall ThToBeginS(_In_ void* pvData);
      void* __vectorcall ThToEnd(_In_ void* pvData);
      void* __vectorcall ThToEndS(_In_ void* pvData);
      void* __vectorcall ThElement(unsigned long ulIndex);
      void __vectorcall ThDeleteElement(_In_ unsigned long ulIndex, _In_ bool bDeleteData);
      void __vectorcall ThDeleteElementS(_In_ unsigned long ulIndex, _In_ bool bDeleteData);
      void __vectorcall ThDeleteList(_In_ bool bDeleteData);
      void __vectorcall ThDeleteListS(_In_ bool bDeleteData);
      ULONG __vectorcall Number(void);
      ULONG& __vectorcall ThNumber(_In_ unsigned long& ulNumber);
      void* __vectorcall IteratorToBegin(void);
      void* __vectorcall ThIteratorToBegin(void);
      void* __vectorcall ThIteratorToBegin_Lock(void);
      void __vectorcall ThIteratorEnd(void);

    };
    __declspec(dllimport) COList* __vectorcall COListV(_In_ bool bThreadSafe);
    __declspec(dllimport) COList* __vectorcall COListV(_In_ bool bThreadSafe, _In_ unsigned long ulSpinCount);
    __declspec(dllimport) COList* __vectorcall COListV(_In_ VMEMORY vmMemory, _In_ bool bThreadSafe);
    __declspec(dllimport) COList* __vectorcall COListV(_In_ VMEMORY vmMemory, _In_ bool bThreadSafe, _In_ unsigned long ulSpinCount);
    //---------------------------------------------------------------------------
    typedef struct STTime
    {
      USHORT usJahr;
      BYTE ucMonat;
      ULONG ulTag;
      BYTE ucStunde;
      BYTE ucMinute;
      BYTE ucSekunde;
      USHORT usMillisekunde;
    } STTime;
    //---------------------------------------------------------------------------
    /*
    Datumformat:
      d    -> Tag ohne führende Null
      dd   -> Tag mit führende Null
      ddd  -> Wochentag zwei Buchstaben
      dddd -> Wochentag ganzes Wort
      M    -> Monat ohne führende Null
      MM   -> Monat mit führende Null
      MMM  -> Monat drei Buchstaben
      MMMM -> Monat ganzes Wort
      y    -> Jahr 2-stellig ohne führende Null
      yy   -> Jahr 2-stellig mit führende Null
      yyyy -> Jahr 4-stellig
      ''   -> Selbstdarstellung

    Zeitformat:
      h    -> 12-Stunden-Uhr ohne führende Null
      hh   -> 12-Stunden-Uhr mit führende Null
      H    -> 24-Stunden-Uhr ohne führende Null
      HH   -> 24-Stunden-Uhr mit führende Null
      m    -> Minuten ohne führende Null
      mm   -> Minuten mit führende Null
      s    -> Sekunden ohne führende Null
      ss   -> Sekunden mit führende Null
      t    -> Zeitmarke A oder P
      tt   -> Zeitmarke AM oder PM
      ''   -> Selbstdarstellung
    */
    class __declspec(dllimport) COTime
    {
    private:
      struct STFZeit
      {
        FILETIME FZeit;
        FILETIME FZeit_A;
      };
      char c16FZeit[16];
      VMEMORY vmMemory;

    public:
      __thiscall COTime(void);
      __thiscall COTime(_In_z_ const char* pcString);
      __thiscall COTime(_In_ const COTime* ptTime);
      __thiscall COTime(_In_ const STTime& stTime);
      __thiscall COTime(_In_ const SYSTEMTIME& stSystemTime);
      VMEMORY __vectorcall COFreiV(void);
      COTime* __vectorcall Now(void);
      COTime* __vectorcall Today(void);
      COTime* __vectorcall Tommorow(void);
      bool __vectorcall IsZero(void);
      char* __vectorcall CHARDate(_In_z_ char* pc11Date);
      char* __vectorcall CHARTime(_In_z_ char* pc9Time);
      char* __vectorcall CHARDateTime(_In_z_ char* pc20DateTime);
      COStringA* __vectorcall StrDate(_In_ COStringA* pasDate);
      COStringA* __vectorcall StrTime(_In_ COStringA* pasTime);
      COStringA* __vectorcall StrDateTime(_In_ COStringA* pasDateTime);
      COStringA* __vectorcall StrDateFormat(_In_ COStringA* pasDate, _In_z_ const char* pcFormat);
      COStringA* __vectorcall StrTimeFormat(_In_ COStringA* pasTime, _In_z_ const char* pcFormat);
      COStringA* __vectorcall StrDateTimeFormat(_In_ COStringA* pasDateTime, _In_z_ const char* pcFormat_Date, _In_z_ const char* pcFormat_Time, _In_ bool bOrding_DateTime);
      VMBLOCK __vectorcall VMBLOCKDateFormat(_In_ VMBLOCK& vbDatum, _In_z_ const char* pcFormat);
      VMBLOCK __vectorcall VMBLOCKTimeFormat(_In_ VMBLOCK& vbZeit, _In_z_ const char* pcFormat);
      VMBLOCK __vectorcall VMBLOCKDateTimeFormat(_In_ VMBLOCK& vbDateTime, _In_z_ const char* pcFormat_Date, _In_z_ const char* pcFormat_Time, _In_ bool bOrding_DateTime);
      void __vectorcall operator =(_In_z_ const char* pcString);
      void __vectorcall operator =(_In_ const COTime& tTime);
      void __vectorcall operator =(_In_ const STTime& stTime);
      void __vectorcall operator =(_In_ const SYSTEMTIME& stSystemTime);
      bool __vectorcall operator <(_In_ const COTime& tTime);
      bool __vectorcall operator >(_In_ const COTime& tTime);
      bool __vectorcall operator <=(_In_ const COTime& tTime);
      bool __vectorcall operator >=(_In_ const COTime& tTime);
      bool __vectorcall operator ==(_In_ const COTime& tTime);
      bool __vectorcall operator !=(_In_ const COTime& tTime);
      void __vectorcall operator +=(_In_ const STTime& tTime);
      void __vectorcall operator +=(_In_ const long long llDiffSeconds);
      void __vectorcall operator -=(_In_ const STTime& stTime);
      void __vectorcall operator -=(_In_ const long long llDiffSeconds);
      COTime& __vectorcall operator +(_In_ const STTime& stTime);
      COTime& __vectorcall operator +(_In_ const long long llDiffSeconds);
      COTime& __vectorcall operator -(_In_ const STTime& stTime);
      COTime& __vectorcall operator -(_In_ const long long llDiffSeconds);
      void __vectorcall DifferenceTime(_In_ const COTime* pzZeit, _In_ STTime& stTime);
      void __vectorcall Read(_In_z_ char* pcContent);
      void __vectorcall Write(_In_z_ const char* pcContent);
      FILETIME __vectorcall FileTime(void);
      SYSTEMTIME __vectorcall SystemTime(void);

    };
    __declspec(dllimport) COTime* __vectorcall COTimeV(void);
    __declspec(dllimport) COTime* __vectorcall COTimeV(_In_ VMEMORY vmMemory);
    __declspec(dllimport) COTime* __vectorcall COTimeV(_In_z_ const char* pcString);
    __declspec(dllimport) COTime* __vectorcall COTimeV(_In_ VMEMORY vmMemory, _In_z_ const char* pcString);
    __declspec(dllimport) COTime* __vectorcall COTimeV(_In_ const COTime* ptTime);
    __declspec(dllimport) COTime* __vectorcall COTimeV(_In_ VMEMORY vmMemory, _In_ const COTime* ptTime);
    __declspec(dllimport) COTime* __vectorcall COTimeV(_In_ const STTime& stTime);
    __declspec(dllimport) COTime* __vectorcall COTimeV(_In_ VMEMORY vmMemory, _In_ const STTime& stTime);
    __declspec(dllimport) COTime* __vectorcall COTimeV(_In_ const SYSTEMTIME& stSystemTime);
    __declspec(dllimport) COTime* __vectorcall COTimeV(_In_ VMEMORY vmMemory, _In_ const SYSTEMTIME& stSystemTime);
    //---------------------------------------------------------------------------
    class __declspec(dllimport) COStream
    {
    private:
      struct STElement
      {
        ULONG ulByte;
        VMBLOCK vbDaten;
      };
      COList lsDaten;
      CRITICAL_SECTION csStream;
      ULONG ulBytes;
      ULONG ulPosition;
      bool bThread;
      BYTE ucInfo;
      VMEMORY vmSpeicher;

    public:
      __thiscall COStream(_In_ bool bThreadSafe);
      __thiscall COStream(_In_ bool bThreadSafe, _In_ unsigned long ulSpinCount);
      __thiscall COStream(_In_ VMEMORY vmMemeory, _In_ bool bThreadSafe);
      __thiscall COStream(_In_ VMEMORY vmMemeory, _In_ bool bThreadSafe, _In_ unsigned long ulSpinCount);
      __thiscall ~COStream(void);
      VMEMORY __vectorcall COFreiV(void);
      void* __vectorcall Read(_In_ void* pvData, _In_ ULONG ulByte);
      void* __vectorcall Read(_In_ void* pvData, _In_ ULONG ulByte, _In_ ULONG& ulReadPosition);
      void __vectorcall Write(_In_ void* pvDaten, _In_ ULONG ulByte);
      void __vectorcall WriteS(_In_ void* pvData, _In_ ULONG ulByte);
      void __vectorcall Write(_In_ void* pvData, _In_ ULONG ulByte, _In_ ULONG& ulWritePosition);
      void __vectorcall WriteS(_In_ void* pvData, _In_ ULONG ulByte, _In_ ULONG& ulWritePosition);
      VMBLOCK __vectorcall Data(_In_ VMBLOCK& pvData);
      void __vectorcall Delete(void);
      bool __vectorcall End(void);
      DWORD __vectorcall ReadFile(_In_ HANDLE hFile, _In_ bool bAsynchronous);
      DWORD __vectorcall ReadFile(_In_ HANDLE hFile, _In_ LPOVERLAPPED pOverlapped, _In_ BOOL BWait);
      DWORD __vectorcall WriteFile(_In_ HANDLE hFile, _In_ bool bAsynchronous);
      DWORD __vectorcall WriteFile(_In_ HANDLE hFile, _In_ LPOVERLAPPED pOverlapped, _In_ BOOL BWait);
      COStringA* __vectorcall ReadStringA(_In_ COStringA* pasString, _In_ unsigned char ucStringtyp);
      void __vectorcall WriteStringA(_In_ COStringA* pasString, _In_ unsigned char ucStringtyp);
      char* __vectorcall ReadCHAR(_Inout_z_ VMBLOCK& pcString, _In_ unsigned char ucStringtyp);
      void __vectorcall WriteCHAR(_In_z_ char* pcString, _In_ unsigned char ucStringtyp);
      COTime* __vectorcall ReadTime(_In_ COTime* ptTime);
      void __vectorcall WriteTime(_In_ COTime* ptTime);
      COComma4* __vectorcall ReadComma4(_In_ COComma4* pk4Number);
      void __vectorcall WriteComma4(_In_ COComma4* pk4Number);
      COComma4_80* __vectorcall ReadComma4_80(_In_ COComma4_80* pk4Number_80);
      void __vectorcall WriteComma4_80(_In_ COComma4_80* pk4Number_80);
      ULONG __vectorcall Bytes(void);
      ULONG __vectorcall Position(void);
      ULONG __vectorcall SetPosition(_In_ long lDistance, _In_ char cFromWhere);
      void* __vectorcall ThRead(_In_ void* pvData, _In_ ULONG ulByte);
      void* __vectorcall ThRead(_In_ void* pvData, _In_ ULONG ulByte, _In_ ULONG& ulReadPosition);
      void __vectorcall ThWrite(_In_ void* pvData, _In_ ULONG ulByte);
      void __vectorcall ThWriteS(_In_ void* pvData, _In_ ULONG ulByte);
      void __vectorcall ThWrite(_In_ void* pvData, _In_ ULONG ulByte, _In_ ULONG& ulWritePosition);
      void __vectorcall ThWriteS(_In_ void* pvData, _In_ ULONG ulByte, _In_ ULONG& ulWritePosition);
      DWORD __vectorcall ThReadFile(_In_ HANDLE hFile, _In_ bool bAsynchronous);
      DWORD __vectorcall ThReadFile(_In_ HANDLE hFile, _In_ LPOVERLAPPED pOverlapped, _In_ BOOL BWait);
      DWORD __vectorcall ThWriteFile(_In_ HANDLE hFile, _In_ bool bAsynchronous);
      DWORD __vectorcall ThWriteFile(_In_ HANDLE hFile, _In_ LPOVERLAPPED pOverlapped, _In_ BOOL BWait);
      COStringA* __vectorcall ThReadStringA(_In_ COStringA* pasString, _In_ unsigned char ucStringtyp);
      void __vectorcall ThWriteStringA(_In_ COStringA* pasString, _In_ unsigned char ucStringtyp);
      char* __vectorcall ThReadCHAR(_Inout_z_ VMBLOCK& vbString, _In_ unsigned char ucStringtyp);
      void __vectorcall ThWriteCHAR(_In_z_ char* pcString, _In_ unsigned char ucStringtyp);
      COTime* __vectorcall ThReadTime(_In_ COTime* ptTime);
      void __vectorcall ThWriteTime(_In_ COTime* ptTime);
      COComma4* __vectorcall ThReadComma4(_In_ COComma4* pk4Number);
      void __vectorcall ThWriteComma4(_In_ COComma4* pk4Number);
      COComma4_80* __vectorcall ThReadComma4_80(_In_ COComma4_80* pk4Number_80);
      void __vectorcall ThWriteComma4_80(_In_ COComma4_80* pk4Number_80);
      ULONG* __vectorcall ThBytes(_In_ ULONG& ulBytes);
      ULONG* __vectorcall ThPosition(_In_ ULONG& ulPosition);
      ULONG __vectorcall ThSetPosition(_In_ long lDistance, _In_ char cFromWhere);
      VMBLOCK __vectorcall ThData(_In_ VMBLOCK& vbData);
      void __vectorcall ThDelete(void);
      bool __vectorcall ThEnd(void);
      BYTE __vectorcall GetLastError(void);
      BYTE* __vectorcall ThGetLastError(_In_ BYTE& ucError);

    };
    __declspec(dllimport) COStream* __vectorcall COStreamV(bool bThreadSafe);
    __declspec(dllimport) COStream* __vectorcall COStreamV(VMEMORY vmMemory, bool bThreadSafe);
    __declspec(dllimport) COStream* __vectorcall COStreamV(bool bThreadSafe, unsigned long ulSpinCount);
    __declspec(dllimport) COStream* __vectorcall COStreamV(VMEMORY vmMemory, bool bThreadSafe, unsigned long ulSpinCount);
    //---------------------------------------------------------------------------
    constexpr CHAR STM_POS_ANFANG = -1;
    constexpr CHAR STM_POS_AKTUELL = 0;
    constexpr CHAR STM_POS_ENDE = 1;
    constexpr BYTE EX_STM_DATEIFEHLER = 88;
    constexpr BYTE STM_POSITIONSFEHLER = 89;
    //---------------------------------------------------------------------------
    __declspec(dllimport) unsigned long __vectorcall StrLength(_In_z_ const char* pcString);
    __declspec(dllimport) unsigned long __vectorcall CharactersPosition(_In_z_ const char* pcString, _In_ char cCharacters, _In_z_ bool bFromLefttoRight);
    __declspec(dllimport) unsigned long __vectorcall CharactersPosition(_In_z_ const char* pcString, _In_ unsigned long ulLength, _In_ char cChararcters, _In_ bool bFromLeftToRight);
    __declspec(dllimport) char __vectorcall StrCompare(_In_z_ const char* pcRefString, _In_z_ const char* pcCmpString);
    __declspec(dllimport) char __vectorcall StrCompare(_In_z_ const char* pcRefString, _In_ unsigned long ulRefLength, _In_z_ const char* pcVglString, _In_ unsigned long ulCmpLength);
    __declspec(dllimport) bool __vectorcall StrContain(_In_z_ const char* pcRefString, _In_z_ const char* pcCmpString);
    __declspec(dllimport) bool __vectorcall StrContain(_In_z_ const char* pcRefString, _In_ unsigned long ulRefLength, _In_z_ const char* pcCmpString, _In_ unsigned long ulVglLength);
    __declspec(dllimport) bool __vectorcall StrContainLeft(_In_z_ const char* pcRefString, _In_z_ const char* pcCmpString);
    __declspec(dllimport) bool __vectorcall StrContainLeft(_In_z_ const char* pcRefString, _In_ unsigned long ulRefLength, _In_z_ const char* pcCmpString, _In_ unsigned long ulCmpLength);
    __declspec(dllimport) bool __vectorcall StrContainRight(_In_z_ const char* pcRefString, _In_z_ const char* pcCmpString);
    __declspec(dllimport) bool __vectorcall StrContainRight(_In_z_ const char* pcRefString, _In_ unsigned long ulRefLength, _In_z_ const char* pcCmpString, _In_ unsigned long ulCmpLength);
    __declspec(dllimport) char __vectorcall BIT128Compare(_In_ const BIT128 bit128Value_1, _In_ const BIT128 bit128Value_2);
    //---------------------------------------------------------------------------------------------------------------------------------------
    __declspec(dllimport) void __vectorcall StringAtoList(COList* plNumberList, COStringA& asNumberChain);
    __declspec(dllimport) COStringA* __vectorcall ListtoStringA(COList* plNumberList, COStringA* pasNumber);
    __declspec(dllimport) char* __vectorcall Comma4toCHAR(_Out_writes_z_(20) char pc20Number[20], _In_ COComma4* pk4Number, _In_ unsigned char ucPositions);
    __declspec(dllimport) COStringA* __vectorcall Comma4toStringA(_In_ COStringA* pasString, _In_ COComma4* pk4Number, _In_ unsigned char ucPositions);
    __declspec(dllimport) char* __vectorcall Comma4_80toCHAR(_Out_writes_z_(32) char pc32Number[32], _In_ COComma4_80* pk4_80Number, _In_ unsigned char ucPositions);
    __declspec(dllimport) COStringA* __vectorcall Comma4_80toStringA(_In_ COStringA* pasString, _In_ COComma4_80* pk4_80Number, _In_ unsigned char ucPositions);
    __declspec(dllimport) char* __vectorcall LONGtoCHAR(_Out_writes_z_(12) char pc12Number[12], _In_ long lNumber);
    __declspec(dllimport) char* __vectorcall LONGLONGtoCHAR(_Out_writes_z_(21) char pc21Number[21], _In_ long long llNumber);
    __declspec(dllimport) char* __vectorcall ULONGtoCHAR(_Out_writes_z_(11) char pc11Number[11], _In_ unsigned long ulNumber);
    __declspec(dllimport) char* __vectorcall ULONGtoHexCHAR(_Out_writes_z_(10) char pc10HexNumber[10], _In_ unsigned long ulNumber);
    __declspec(dllimport) char* __vectorcall ULONGtoHexCHAR_64(_Out_writes_z_(18) char pc18HexNumber[18], _In_ unsigned long long ulNumber);
    __declspec(dllimport) char* __vectorcall DOUBLE_B10toCHAR(_Out_writes_z_(28) char pc28Number[28], _In_ double dNumber, _In_ unsigned char ucPositions); // ucPosition = 15 - rounds from 16 to 15 positions not technically possible
    __declspec(dllimport) char* __vectorcall FLOAT_B10toCHAR(_Out_writes_z_(20) char pc20Number[20], _In_ float fNumber, _In_ unsigned char ucPositions);   // ucPosition = 7 -  rounds from 8 to 7 positions not technically possible
    __declspec(dllimport) char* __vectorcall BIT128toGUID(_Out_writes_z_(37) char pc37GUID[37], _In_ const BIT128 bit128Value);
    //---------------------------------------------------------------------------------------------------------------------------------------
  }
}
#endif