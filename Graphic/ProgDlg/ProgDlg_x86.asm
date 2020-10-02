;/****************************************************************************
;  ProgDlg_x86.asm
;  For more information see https://github.com/RePag-net/Graphic-User-Interface
;****************************************************************************/
;
;/****************************************************************************
;  The MIT License(MIT)
;
;  Copyright(c) 2020 René Pagel
;
;  Permission is hereby granted, free of charge, to any person obtaining a copy
;  of this softwareand associated documentation files(the "Software"), to deal
;  in the Software without restriction, including without limitation the rights
;  to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
;  copies of the Software, and to permit persons to whom the Software is
;  furnished to do so, subject to the following conditions :
;
;  The above copyright noticeand this permission notice shall be included in all
;  copies or substantial portions of the Software.
;
;  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
;  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
;  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
;  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
;  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
;  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
;  SOFTWARE.
;******************************************************************************/

.686P
.XMM
INCLUDE listing.inc
.MODEL FLAT
INCLUDELIB LIBCMTD
INCLUDELIB OLDNAMES

EXTRN	__imp__GetObjectA@12:PROC

PUBLIC dwEAX_CPUID_EAX_1
PUBLIC dwEBX_CPUID_EAX_1
PUBLIC dwECX_CPUID_EAX_1
PUBLIC dwEDX_CPUID_EAX_1

PUBLIC _SSE3
PUBLIC _AES
PUBLIC _AVX
PUBLIC _RDRAND
PUBLIC _MMX
PUBLIC _SSE
PUBLIC _SSE2

.DATA?
dwEineSpeicherSeite DWORD ?
dwDoppelSpeicherSeite DWORD ?
dbIntel DB ?

dwEAX_CPUID_EAX_1 DWORD ?
dwEBX_CPUID_EAX_1 DWORD ?
dwECX_CPUID_EAX_1 DWORD ?
dwEDX_CPUID_EAX_1 DWORD ?

dwEBX_CPUID_EAX_7 DWORD ?
dwECX_CPUID_EAX_7 DWORD ?

.DATA
;---ECX----
_SSE3 equ 0
_AES equ 25
_AVX equ 28
_RDRAND equ 30
;---EDX----
_MMX equ 23
_SSE equ 25
_SSE2 equ 26
;---EBX---
_AVX2 equ 5
_RDSEED equ 18
_SHA equ 29

CS_Prozessor SEGMENT PARA PRIVATE FLAT EXECUTE
;----------------------------------------------------------------------------
a_stSystemInfo_dwPageSize = 4
?CPUID@@YQXAAU_SYSTEM_INFO@@@Z PROC PUBLIC
		push ebx
    push esi
    mov esi, ecx

		mov dword ptr [dbIntel], 00h
		xor eax, eax
		cpuid
		cmp ebx, 756e6547h
		jne Speicher
		cmp ecx, 6c65746eh
		jne Speicher
		cmp edx, 49656e69h
		jne Speicher
		mov dword ptr [dbIntel], 01h

	Speicher:
    mov ecx, dword ptr a_stSystemInfo_dwPageSize[esi]
		mov dword ptr [dwEineSpeicherSeite], ecx
		imul ecx, 02h
		mov dword ptr [dwDoppelSpeicherSeite], ecx

		mov eax, 01h
		cpuid
		mov dword ptr [dwEAX_CPUID_EAX_1], eax
		mov dword ptr [dwEBX_CPUID_EAX_1], ebx
		mov dword ptr [dwECX_CPUID_EAX_1], ecx
		mov dword ptr [dwEDX_CPUID_EAX_1], edx

    mov eax, 07h
    xor ecx, ecx
		cpuid
		mov dword ptr [dwEBX_CPUID_EAX_7], ebx
		mov dword ptr [dwECX_CPUID_EAX_7], ecx

		pop	ebx
    pop esi
		ret 0
?CPUID@@YQXAAU_SYSTEM_INFO@@@Z ENDP
;----------------------------------------------------------------------------
CS_Prozessor ENDS
;----------------------------------------------------------------------------
CS_Grafic SEGMENT PARA PRIVATE FLAT EXECUTE
;----------------------------------------------------------------------------
c_hbmBild = 20
c_stHintergrundfarbe = 60
s_stDIBSection = 0
s_stDIBSection_dsBm_bmBits = 20
s_stDIBSection_dsBmih_biWidth = 28
s_stDIBSection_dsBmih_biHeight = 32
s_stDIBSection_dsBmih_biBitCount = 38
a_rcZeichnen_left = 0
a_rcZeichnen_top = 4
a_rcZeichnen_right = 8
a_rcZeichnen_bottom = 12
?Hintergrund@COElement@GUI@RePag@@AAQXAAUtagRECT@@@Z PROC PUBLIC
    push ebp
    push ebx
    push edi
    sub esp, 84
    mov ebp, ecx
    mov edi, edx

    lea eax, dword ptr s_stDIBSection[esp]
    push eax
    push 84
    mov eax, dword ptr c_hbmBild[ebp]
    push eax
    call dword ptr __imp__GetObjectA@12

    mov ecx, dword ptr c_stHintergrundfarbe[ebp]
    mov ebx, ecx
		shl ecx, 08h
		shr ebx, 08h
		mov cl, bh
		mov dx, bx
		mov ebx, ecx
		shl ebx, 08h
		mov bl, dl

		movd mm0, ebx
		movd mm1, ecx
		punpckldq mm0, mm1
		movq mm1, mm0
		psrlq mm1, 10h
		punpckldq mm1, mm0

		movq2dq xmm0, mm0
		movq2dq xmm1, mm1
		movlhps xmm0, xmm1

  DIBSection:
		mov edx, dword ptr s_stDIBSection_dsBmih_biHeight[esp]
    mov ebx, edx
		sub edx, dword ptr a_rcZeichnen_top[edi]
		movd mm1, edx
		sub ebx, dword ptr a_rcZeichnen_bottom[edi]
		mov edx, dword ptr a_rcZeichnen_right[edi]
		mov eax, dword ptr a_rcZeichnen_left[edi]
		sub edx, eax
    imul edx, 03h
		movd mm2, edx
		imul eax, 03h
		add eax, dword ptr s_stDIBSection_dsBm_bmBits[esp]
		movd mm4, eax
		movzx ecx, word ptr s_stDIBSection_dsBmih_biBitCount[esp]
		imul ecx, dword ptr s_stDIBSection_dsBmih_biWidth[esp]
		add ecx, 1Fh 
		and ecx, 0FFFFFFE0h 
		sar ecx, 03h
		movd mm3, ecx

	ZeilenAnfang:
		movd ecx, mm3
		imul ecx, ebx 
		movd eax, mm4
		add eax, ecx

		movd edx, mm2

	Register_16:
		cmp edx, 10h
		jl Register_8
		movdqu xmmword ptr [eax], xmm0
		add eax, 0fh
		sub edx, 0fh
		jmp Register_16

	Register_8:
		cmp edx, 08h
		jl Register_4
		movq mmword ptr [eax], mm0
		add eax, 06h
		sub edx, 06h
		jmp Register_8

	Register_4:
		cmp edx, 04h
		jl Register_2
		movd dword ptr [eax], mm0
		add eax, 03h
		sub edx, 03h
		jmp Register_4

	Register_2:
		cmp edx, 03h
		jl ZeilenEnde
		movd ecx, mm0
		mov word ptr [eax], cx
		add eax, 02h
		shr ecx, 08h
		mov byte ptr [eax], ch

	ZeilenEnde:
		movd ecx, mm1
		inc ebx
		cmp ebx, ecx
		jl ZeilenAnfang

		emms
    add esp, 84
    pop edi
    pop ebx
    pop ebp
    ret 0
?Hintergrund@COElement@GUI@RePag@@AAQXAAUtagRECT@@@Z ENDP
;----------------------------------------------------------------------------
c_rcEffektrand_links = 64 
c_rcEffektrand_oben = 68
c_rcEffektrand_rechts = 72
c_rcEffektrand_unten = 76
c_stHintergrundeffektfarbe = 26
?Effekt_Rand_Horizontal@COElement@GUI@RePag@@AAQXXZ PROC PUBLIC
    push ebp
    push ebx
    push edi
    sub esp, 84
    mov ebp, ecx
    mov edi, edx

    lea eax, dword ptr s_stDIBSection[esp]
    push eax
    push 84
    mov eax, dword ptr c_hbmBild[ebp]
    push eax
    call dword ptr __imp__GetObjectA@12

    mov ecx, dword ptr c_stHintergrundeffektfarbe[ebp]
    mov ebx, ecx
		shl ecx, 08h
		shr ebx, 08h
		mov cl, bh
		mov dx, bx
		mov ebx, ecx
		shl ebx, 08h
		mov bl, dl

		movd mm0, ebx
		movd mm1, ecx
		punpckldq mm0, mm1
		movq mm1, mm0
		psrlq mm1, 10h
		punpckldq mm1, mm0

		movq2dq xmm0, mm0
		movq2dq xmm1, mm1
		movlhps xmm0, xmm1

    mov edx, dword ptr s_stDIBSection_dsBmih_biWidth[esp]
		imul edx, 03h
		movd mm2, edx
		mov eax, dword ptr s_stDIBSection_dsBm_bmBits[esp]
		movd mm4, eax
		movzx ecx, word ptr s_stDIBSection_dsBmih_biBitCount[esp] 
		imul ecx, dword ptr s_stDIBSection_dsBmih_biWidth[esp] 
		add ecx, 1Fh 
		and ecx, 0FFFFFFE0h 
		sar ecx, 03h
		movd mm3, ecx

    mov edx, dword ptr c_rcEffektrand_unten[ebp]
		cmp edx, 0h
		je RandOben
		dec edx

	RandOben:
		movd mm7, edx
		mov ebx, dword ptr s_stDIBSection_dsBmih_biHeight[esp]
		dec ebx
		mov ecx, ebx
    sub ecx, dword ptr c_rcEffektrand_oben[ebp]
		movd mm6, ecx
		cmp ebx, ecx
		je ZeilenEnde
		cmp ecx, edx
		jge ZeilenAnfang
		movq mm6, mm7

	ZeilenAnfang:
		movd ecx, mm3
		imul ecx, ebx 
		movd eax, mm4
		add eax, ecx
		movd edx, mm2

	Register_16:
		cmp edx, 10h
		jl Register_8
		movups xmmword ptr [eax], xmm0
		add eax, 0fh
		sub edx, 0fh
		jmp Register_16

	Register_8:
		cmp edx, 08h
		jl Register_4
		movq mmword ptr [eax], mm0
		add eax, 06h
		sub edx, 06h
		jmp Register_8

	Register_4:
		cmp edx, 04h
		jl Register_2
		movd dword ptr [eax], mm0
		add eax, 03h
		sub edx, 03h
		jmp Register_4

	Register_2:
		cmp edx, 03h
		jl ZeilenEnde
		movd ecx, mm0
		mov word ptr [eax], cx
		add eax, 02h
		shr ecx, 08h
		mov byte ptr [eax], ch

	ZeilenEnde:
		movd ecx, mm6
		dec ebx
		cmp ebx, ecx
		jge ZeilenAnfang
		cmp ebx, 0h
		jl Ende
		movd ebx, mm7
		cmp ebx, 0h
		je Ende

		mov ebx, 0h
		movd mm6, ebx
    movd ebx, mm7
		jmp ZeilenAnfang

	Ende:
		emms
    add esp, 84
    pop edi
    pop ebx
    pop ebp
    ret 0
?Effekt_Rand_Horizontal@COElement@GUI@RePag@@AAQXXZ ENDP
;----------------------------------------------------------------------------
?Effekt_Rand_Vertical@COElement@GUI@RePag@@AAQXXZ PROC PUBLIC
    push ebp
    push ebx
    push edi
    sub esp, 84
    mov ebp, ecx
    mov edi, edx

    lea eax, dword ptr s_stDIBSection[esp]
    push eax
    push 84
    mov eax, dword ptr c_hbmBild[ebp]
    push eax
    call dword ptr __imp__GetObjectA@12

    mov ecx, dword ptr c_stHintergrundeffektfarbe[ebp]
		mov ebx, ecx
		shl ecx, 08h
		shr ebx, 08h
		mov cl, bh
		mov dx, bx
		mov ebx, ecx
		shl ebx, 08h
		mov bl, dl

		movd mm0, ebx
		movd mm1, ecx
		punpckldq mm0, mm1
		movq mm1, mm0
		psrlq mm1, 10h
		punpckldq mm1, mm0

		movq2dq xmm0, mm0
		movq2dq xmm1, mm1
		movlhps xmm0, xmm1

		mov edx, dword ptr s_stDIBSection_dsBmih_biHeight[esp]
		movd mm1, edx
		mov ecx, dword ptr s_stDIBSection_dsBmih_biWidth[esp]
		mov edx, dword ptr c_rcEffektrand_links[ebp]
		sub ecx, edx
		imul edx, 03h
		movd mm5, edx
		mov edx, dword ptr c_rcEffektrand_rechts[ebp]
		movd mm6, edx
		sub ecx, edx
		imul ecx, 03h
		movd mm2, ecx
		movzx ecx, word ptr s_stDIBSection_dsBmih_biBitCount[esp] 
		imul ecx, dword ptr s_stDIBSection_dsBmih_biWidth[esp] 
		add ecx, 1Fh 
		and ecx, 0FFFFFFE0h 
		sar ecx, 03h
		movd mm3, ecx
		mov eax, dword ptr s_stDIBSection_dsBm_bmBits[esp]
		movd mm4, eax
		add eax, ecx
		xor ebx, ebx

	ZeilenAnfang:
		movd ecx, mm3
		imul ecx, ebx 
		movd eax, mm4
		add eax, ecx

		movd edx, mm5
		cmp edx, 0h
		je Rechts

	Register_16:
		cmp edx, 10h
		jl Register_8
		movups xmmword ptr [eax], xmm0
		add eax, 0fh
		sub edx, 0fh
		jmp Register_16

	Register_8:
		cmp edx, 08h
		jl Register_4
		movq mmword ptr [eax], mm0
		add eax, 06h
		sub edx, 06h
		jmp Register_8

	Register_4:
		cmp edx, 04h
		jl Register_2
		movd dword ptr [eax], mm0
		add eax, 03h
		sub edx, 03h
		jmp Register_4

	Register_2:
		cmp edx, 03h
		jl ZeilenEnde

	Rechts:
		movd ecx, mm0
		mov word ptr [eax], cx
		add eax, 02h
		shr ecx, 08h
		mov byte ptr [eax], ch

		movd ecx, mm2
		add eax, ecx
		inc eax

		movd edx, mm6
		imul edx, 03h

	Register_16A:
		cmp edx, 10h
		jl Register_8A
		movups xmmword ptr [eax], xmm0
		add eax, 0fh
		sub edx, 0fh
		jmp Register_16A

	Register_8A:
		cmp edx, 08h
		jl Register_4A
		movq mmword ptr [eax], mm0
		add eax, 06h
		sub edx, 06h
		jmp Register_8A

	Register_4A:
		cmp edx, 04h
		jl Register_2A
		movd dword ptr [eax], mm0
		add eax, 03h
		sub edx, 03h
		jmp Register_4A

	Register_2A:
		cmp edx, 03h
		jl ZeilenEnde
		movd ecx, mm0
		mov word ptr [eax], cx
		add eax, 02h
		shr ecx, 08h
		mov byte ptr [eax], ch

	ZeilenEnde:
		movd ecx, mm1
		inc ebx
		cmp ebx, ecx
		jl ZeilenAnfang

		emms
    add esp, 84
    pop edi
    pop ebx
    pop ebp
    ret 0
?Effekt_Rand_Vertical@COElement@GUI@RePag@@AAQXXZ ENDP
;----------------------------------------------------------------------------
?Effekt_Beleuchtung_Horizontal@COElement@GUI@RePag@@AAQXXZ PROC PUBLIC
    push ebp
    push ebx
    push edi
    sub esp, 84
    mov ebp, ecx
    mov edi, edx

    lea eax, dword ptr s_stDIBSection[esp]
    push eax
    push 84
    mov eax, dword ptr c_hbmBild[ebp]
    push eax
    call dword ptr __imp__GetObjectA@12

		mov ecx, dword ptr c_stHintergrundeffektfarbe[ebp]
		mov ebx, dword ptr c_rcEffektrand_oben[ebp]
		cmp ebx, 0h
		je Schritt
      
		mov ebx, ecx
		shl ecx, 08h
		shr ebx, 08h
		mov cl, bh
		mov dx, bx
		mov ebx, ecx
		shl ebx, 08h
		mov bl, dl

		movd mm0, ebx
		movd mm1, ecx
		punpckldq mm0, mm1
		movq mm1, mm0
		psrlq mm1, 10h
		punpckldq mm1, mm0

		movq2dq xmm0, mm0
		movq2dq xmm1, mm1
		movlhps xmm0, xmm1

	Schritt:
		mov ecx, 03030303h
		movd mm5, ecx
		movd mm6, ecx
		punpckldq mm5, mm6

    mov edx, dword ptr s_stDIBSection_dsBmih_biWidth[esp]
		imul edx, 03h
		movd mm2, edx
		mov eax, dword ptr s_stDIBSection_dsBm_bmBits[esp]
		movd mm4, eax
		movzx ecx, word ptr s_stDIBSection_dsBmih_biBitCount[esp] 
		imul ecx, dword ptr s_stDIBSection_dsBmih_biWidth[esp] 
		add ecx, 1Fh 
		and ecx, 0FFFFFFE0h 
		sar ecx, 03h
		movd mm3, ecx

    mov edx, dword ptr c_rcEffektrand_unten[ebp]
		cmp edx, 0h
		je RandOben
		dec edx

	RandOben:
		movd mm7, edx
		mov ebx, dword ptr s_stDIBSection_dsBmih_biHeight[esp]
		dec ebx
		mov ecx, ebx
    sub ecx, dword ptr c_rcEffektrand_oben[ebp]
		movd mm6, ecx
		cmp ebx, ecx
		je ZeilenEnde
		cmp ecx, edx
		jge ZeilenAnfang
		movq mm6, mm7

	ZeilenAnfang:
		movd ecx, mm3
		imul ecx, ebx 
		movd eax, mm4
		add eax, ecx
		movd edx, mm2

		movd ecx, mm7
		cmp ebx, ecx
		jl Unten
		je Register_16
		psubb mm0, mm5
		psubb mm1, mm5
		movq2dq xmm0, mm0
		movq2dq xmm1, mm1
		movlhps xmm0, xmm1
		jmp Register_16

	Unten:
		paddb mm0, mm5
		paddb mm1, mm5
		movq2dq xmm0, mm0
		movq2dq xmm1, mm1
		movlhps xmm0, xmm1

	Register_16:
		cmp edx, 10h
		jl Register_8
		movups xmmword ptr [eax], xmm0
		add eax, 0fh
		sub edx, 0fh
		jmp Register_16

	Register_8:
		cmp edx, 08h
		jl Register_4
		movq mmword ptr [eax], mm0
		add eax, 06h
		sub edx, 06h
		jmp Register_8

	Register_4:
		cmp edx, 04h
		jl Register_2
		movd dword ptr [eax], mm0
		add eax, 03h
		sub edx, 03h
		jmp Register_4

	Register_2:
		cmp edx, 03h
		jl ZeilenEnde
		movd ecx, mm0
		mov word ptr [eax], cx
		add eax, 02h
		shr ecx, 08h
		mov byte ptr [eax], ch

	ZeilenEnde:
		movd ecx, mm6
		dec ebx
		cmp ebx, ecx
		jge ZeilenAnfang
		cmp ebx, 0h
		jl Ende
		movd ebx, mm7
		cmp ebx, 0h
		je Ende

		mov ecx, dword ptr c_stHintergrundeffektfarbe[ebp]
		mov ebx, dword ptr c_rcEffektrand_unten[ebp]
		imul ebx, 030303h
		sub ecx, ebx
      
		mov ebx, ecx
		shl ecx, 08h
		shr ebx, 08h
		mov cl, bh
		mov dx, bx
		mov ebx, ecx
		shl ebx, 08h
		mov bl, dl

		movd mm0, ebx
		movd mm1, ecx
		punpckldq mm0, mm1
		movq mm1, mm0
		psrlq mm1, 10h
		punpckldq mm1, mm0

		movq2dq xmm0, mm0
		movq2dq xmm1, mm1
		movlhps xmm0, xmm1

		mov ebx, 0h
		movd mm6, ebx
    movd ebx, mm7
		jmp ZeilenAnfang

	Ende:
		emms
    add esp, 84
    pop edi
    pop ebx
    pop ebp
    ret 0
?Effekt_Beleuchtung_Horizontal@COElement@GUI@RePag@@AAQXXZ ENDP
;----------------------------------------------------------------------------
CS_Grafic ENDS
END
