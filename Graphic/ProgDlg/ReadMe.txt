				mov ecx, 100;
		xor rbx, rbx
	Schleife:
		inc rbx
		mov eax, dword ptr STDIBSection_dsBmih_biWidth[rsp + rbx]
		loop Schleife