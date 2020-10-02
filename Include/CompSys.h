// up Fileversion 2.2.5.0
#ifndef CompSysH
#define CompSysH
#include <Windows.h>
#include <immintrin.h>
//-------------------------------------------------------------------------------------------------------------------------------------------
namespace RePag
{
  namespace System
  {
    constexpr BYTE EX_VM_KEINSPEICHER = 90;
    using VMEMORY = const void*;
    using VMBLOCK = char*;
    using VMBLOCKM = VMBLOCK;
    using VMBLOCKA = unsigned char*;
    using EXCEPTION = unsigned char;
    using BIT128 = unsigned char[16];
    using BIT256 = unsigned char[32];
    using PBIT128 = BIT128*;
    using PBIT256 = BIT256*;
    using BIT128A = __declspec(align(16)) BIT128;
    using BIT256A = __declspec(align(32)) BIT256;
    //-------------------------------------------------------------------------------------------------------------------------------------------
    __declspec(dllimport) VMEMORY __vectorcall InitVirtualMem(_In_ bool bAuslagern, _In_z_ const char* pcMemoryName);
    __declspec(dllimport) void __vectorcall FreeVirtualMem(_In_ VMEMORY vmMemory);
    __declspec(dllimport) VMBLOCK __vectorcall MemoryNamesList(_Out_ VMBLOCK& vbName);
    __declspec(dllimport) VMBLOCK __vectorcall VMBlock(_In_ VMEMORY vmMemory, _In_ unsigned long ulBytes);
    __declspec(dllimport) VMBLOCK __vectorcall VMBlock(_In_ unsigned long ulBytes);
    __declspec(dllimport) VMBLOCK __vectorcall VMBlockS(_In_ VMEMORY vmMemory, _In_ unsigned long ulBytes);
    __declspec(dllimport) VMBLOCK __vectorcall VMBlockS(_In_ unsigned long ulBytes);
    __declspec(dllimport) VMBLOCKA __vectorcall VMBlockA(unsigned long ulBytes, unsigned char ucAlign);
    __declspec(dllimport) VMBLOCKA __vectorcall VMBlockA(VMEMORY vmMemory, unsigned long ulBytes, unsigned char ucAlign);
    __declspec(dllimport) void __vectorcall VMFrei(_In_ VMEMORY vmMemory, _In_ void* pvAddress);
    __declspec(dllimport) void __vectorcall VMFrei(_In_ void* pvAddress);
    __declspec(dllimport) void __vectorcall VMFreiS(_In_ VMEMORY vmMemory, _In_ void* pvAddress);
    __declspec(dllimport) void __vectorcall VMFreiS(_In_ void* pvAddress);
    __declspec(dllimport) VMBLOCK __vectorcall VMOccupancy(_In_z_ const char* pcMemoryName, _Out_ VMBLOCK& vbOccupancy);
    __declspec(dllimport) VMBLOCK __vectorcall VMOccupancyPage(_In_z_ const char* pccMemoryName, _Out_ VMBLOCK& vbOccupancy);
    __declspec(dllimport) VMBLOCK __vectorcall VMOccupancyCore(_In_z_ const char* pccMemoryName, _Out_ VMBLOCK& vbOccupancy);
    __declspec(dllimport) void* __vectorcall MemCopy(_In_z_ void* pvDestination, _In_z_ const void* pvSource, _In_ unsigned long ulBytes);
    __declspec(dllimport) void* __vectorcall ZeroMem(_In_ void* vbMemory, _In_ unsigned long ulBytes);
    //-------------------------------------------------------------------------------------------------------------------------------------------
    template <typename XTyp>
    XTyp* __vectorcall VMBlock(XTyp* xTyp)
    {
      return (XTyp*)VMBlock(sizeof(XTyp));
    }
    //-------------------------------------------------------------------------------------------------------------------------------------------
    template <typename XTyp>
    XTyp* __vectorcall VMBlock(VMEMORY vmMemory, XTyp* xTyp)
    {
      return (XTyp*)VMBlock(vmMemory, sizeof(XTyp));
    }
    //-------------------------------------------------------------------------------------------------------------------------------------------
    template <typename XTyp>
    XTyp* __vectorcall VMBlockS(XTyp* xTyp)
    {
      return (XTyp*)VMBlockS(sizeof(XTyp));
    }
    //-------------------------------------------------------------------------------------------------------------------------------------------
    template <typename XTyp>
    XTyp* __vectorcall VMBlockS(VMEMORY vmMemory, XTyp* xTyp)
    {
      return (XTyp*)VMBlockS(vmMemory, sizeof(XTyp));
    }
    //-------------------------------------------------------------------------------------------------------------------------------------------
    template <typename XTyp>
    void __vectorcall VMFreiV(XTyp xTyp)
    {
      VMFrei(xTyp->COFreiV(), xTyp);
    }
    //-------------------------------------------------------------------------------------------------------------------------------------------
    template <typename XTyp>
    void __vectorcall VMFreiVS(XTyp xTyp)
    {
      VMFreiS(xTyp->COFreiV(), xTyp);
    }
    //-------------------------------------------------------------------------------------------------------------------------------------------
    class __declspec(dllimport) COBlock
    {
      friend DWORD WINAPI thBlockSuche(_In_ void* pvParam);

    private:
      VMBLOCK vbBlock;
      unsigned long ulBytes;
      HANDLE hThread;

    public:
      void __vectorcall COBlockV(_In_ VMBLOCK vbBlockA, _In_ unsigned long ulBytesA);
      VMEMORY __vectorcall COFreiV(void);
      void __vectorcall Start(void);
      void (CALLBACK* pfnPosition)(unsigned long);

    };
    __declspec(dllimport) COBlock* __vectorcall COBlockV(_In_ VMBLOCK vbBlock, _In_ unsigned long ulBytes);
    //-------------------------------------------------------------------------------------------------------------------------------------------
    __declspec(dllimport) bool __vectorcall Is_SSE2(void);
    __declspec(dllimport) void __vectorcall DeleteEvent(_In_ HANDLE hEvent);
    __declspec(dllimport) void __vectorcall ThreadTerminate(_In_ HANDLE hThread);
    __declspec(dllimport) unsigned int __vectorcall ZufallZahl(_In_ unsigned long ulRangeMin, _In_ unsigned long ulRangeMax);
    __declspec(dllimport) LONG __vectorcall CheckSignaturDatei(_In_z_ LPCSTR pcDateiname);  // Return value https://msdn.microsoft.com/EN-US/library/windows/desktop/aa388208(v=vs.85).aspx#
    __declspec(dllimport) LONG __vectorcall CheckSignaturModule(_In_ HMODULE hModule);      // Return value https://msdn.microsoft.com/EN-US/library/windows/desktop/aa388208(v=vs.85).aspx#
    __declspec(dllimport) bool __vectorcall InstallRePagRootCertificate(void);
    __declspec(dllexport) BYTE __vectorcall SetTimeBSTPerfectBalance(_In_reads_z_(6) const char cTime[6]); // Format 24h examples "18:30" or "07:15"
    __declspec(dllexport) BYTE __vectorcall DeleteTimeBSTPerfectBalance(void);
    __declspec(dllexport) BYTE __vectorcall ImmediatelyBSTPerfectBalance(void);
    __declspec(dllexport) BYTE __vectorcall SetTimeStartOptimize(_In_reads_z_(6) const char cTime[6]); // Format 24h examples "18:30" or "07:15"
    __declspec(dllexport) BYTE __vectorcall DeleteTimeStartOptimize(void);
    __declspec(dllexport) BYTE __vectorcall ImmediatelyStartOptimize(void);
  //-------------------------------------------------------------------------------------------------------------------------------------------

    constexpr BYTE SHA_128 = 1;
    constexpr BYTE SHA_256 = 2;
    constexpr BYTE SHA_384 = 3;
    constexpr BYTE SHA_512 = 4;

    __declspec(dllimport) USHORT __vectorcall Hash_SHA(_In_ BYTE ucAlgID, _In_ VMBLOCK vbHashDaten, _In_ ULONG ulBytes_HashDaten, _Out_ VMBLOCK& vbHash);
    __declspec(dllimport) char __vectorcall Is_RNGenerator(void);  // Return value -1 = RDRAND; 1 = RDSEED;
    __declspec(dllimport) bool __vectorcall Is_AES(void);
    __declspec(dllimport) BYTE __vectorcall CoreCompleteReset(void);
    __declspec(dllimport) BYTE __vectorcall AddAESKey(_In_reads_z_(254) const char pcKeyName[254], _In_reads_z_(254) const char pcPasswort[254]);
    __declspec(dllimport) BYTE __vectorcall DeleteAESKey(_In_reads_z_(254) const char pcKeyName[254], _In_reads_z_(254) const char pcPasswort[254]);
    __declspec(dllimport) BYTE __vectorcall ChangeKeyname(_In_reads_z_(254) const char pcKeyName_Old[254], _In_reads_z_(254) const char pcKeyName_New[254], _In_reads_z_(254) const char pcPasswort[254]);
    __declspec(dllimport) BYTE __vectorcall ChangeKeypassword(_In_reads_z_(254) const char pcKeyName[254], _In_reads_z_(254) const char pcPasswort_Old[254], _In_reads_z_(254) const char pcPasswort_New[254]);
    __declspec(dllimport) BYTE __vectorcall TransferKeyToComputer(_In_reads_z_(254) const char pcKeyName[254], _In_reads_z_(254) const char pcPassword[254], _In_z_ const char* pcIPNumber_IV4, _In_z_ const char* pcIPNumber_IV6);
    __declspec(dllimport) BYTE __vectorcall TransferKeyFromComputer(_In_reads_z_(254) const char pcKeyName[254], _In_reads_z_(254) const char pcPassword[254], _In_z_ const char* pcIPNumber_IV4, _In_z_ const char* pcIPNumber_IV6); // either IPV4 or IPV6 other NULL
    __declspec(dllimport) BYTE __vectorcall TransferKeyToComputer(_In_reads_z_(254) const char pcKeyName[254], _In_reads_z_(254) const char pcPassword[254], _In_z_ const char* pcHost_Node_Name);
    __declspec(dllimport) BYTE __vectorcall TransferKeyFromComputer(_In_reads_z_(254) const char pcKeyName[254], _In_reads_z_(254) const char pcPassword[254], _In_z_ const char* pcHost_Node_Name);
    __declspec(dllimport) bool __vectorcall AES256_Encrypt(_In_reads_z_(254) const char pcKeyName[254], _In_reads_z_(254) const char pcPasswort[254], _In_ VMBLOCK vbPlainText, _In_ VMBLOCK vbChipperText, _In_ unsigned long ulBytes);
    __declspec(dllimport) bool __vectorcall AES256_Decrypt(_In_reads_z_(254) const char pcKeyName[254], _In_reads_z_(254) const char pcPasswort[254], _In_ VMBLOCK vbChipperText, _In_ VMBLOCK vbDecryptText, _In_ unsigned long ulBytes);
    //-------------------------------------------------------------------------------------------------------------------------------------------
  }
}

#ifndef ADTH
#include "ADT.h"
namespace RePag
{
  namespace System
  {
    //-------------------------------------------------------------------------------------------------------------------------------------------
    class __declspec(dllimport) COVMBlockShared
    {
    private:
      VMEMORY vmcMemory;
      COStringA* vasName;
      VMBLOCKM vbmBlock;
      unsigned long ulBytes_VMBlock;
      HANDLE heSchreiben[3];
      HANDLE heLesen[3];
      CRITICAL_SECTION csVMBlock;

    public:
      bool __vectorcall COVMBlockSharedV(_In_z_ const char* pcName, _In_ unsigned long ulBytesA);
      bool __vectorcall COVMBlockSharedV(_In_ VMEMORY vmcMemory, _In_z_ const char* pcName, _In_ unsigned long ulBytesA);
      VMEMORY __vectorcall COFreiV(void);
      void __vectorcall Schreiben(_In_ void* pvDaten, _In_ unsigned long ulBytes, _Out_ unsigned long& ulPosition);
      void* __vectorcall Lesen(_Out_ VMBLOCK& vbvDaten, _In_ unsigned long ulBytes, _Out_ unsigned long& ulPosition);
      void* __vectorcall Lesen(_In_ VMEMORY vmcMemory, _Out_ VMBLOCK& vbvDaten, _In_ unsigned long ulBytes, _Out_ unsigned long& ulPosition);

    };
    //-------------------------------------------------------------------------------------------------------------------------------------------
    __declspec(dllimport) COVMBlockShared* __vectorcall COVMBlockSharedV(_In_z_ const char* pcName, _In_ unsigned long ulBytes);
    __declspec(dllimport) COVMBlockShared* __vectorcall COVMBlockSharedV(_In_ VMEMORY vmcMemory, _In_z_ const char* pcName, _In_ unsigned long ulBytes);
    //-------------------------------------------------------------------------------------------------------------------------------------------

    __declspec(dllimport) COStringA* __vectorcall Datei_Info(_In_ COStringA* pasFile);
    __declspec(dllimport) BYTE __vectorcall Update_Info(_In_ COStream* psmFile_Version); // Return value 0 -> Update available
    __declspec(dllimport) BYTE __vectorcall ProzessPlatze(_In_ COStream* psmProzesse);
    //-------------------------------------------------------------------------------------------------------------------------------------------
  }
}
#endif

#endif