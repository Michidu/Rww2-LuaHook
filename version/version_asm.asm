.code
extern m_procs:QWORD
GetFileVersionInfoA_wrapper proc
	jmp m_procs[0*8]
GetFileVersionInfoA_wrapper endp
GetFileVersionInfoByHandle_wrapper proc
	jmp m_procs[1*8]
GetFileVersionInfoByHandle_wrapper endp
GetFileVersionInfoExA_wrapper proc
	jmp m_procs[2*8]
GetFileVersionInfoExA_wrapper endp
GetFileVersionInfoExW_wrapper proc
	jmp m_procs[3*8]
GetFileVersionInfoExW_wrapper endp
GetFileVersionInfoSizeA_wrapper proc
	jmp m_procs[4*8]
GetFileVersionInfoSizeA_wrapper endp
GetFileVersionInfoSizeExA_wrapper proc
	jmp m_procs[5*8]
GetFileVersionInfoSizeExA_wrapper endp
GetFileVersionInfoSizeExW_wrapper proc
	jmp m_procs[6*8]
GetFileVersionInfoSizeExW_wrapper endp
GetFileVersionInfoSizeW_wrapper proc
	jmp m_procs[7*8]
GetFileVersionInfoSizeW_wrapper endp
GetFileVersionInfoW_wrapper proc
	jmp m_procs[8*8]
GetFileVersionInfoW_wrapper endp
VerFindFileA_wrapper proc
	jmp m_procs[9*8]
VerFindFileA_wrapper endp
VerFindFileW_wrapper proc
	jmp m_procs[10*8]
VerFindFileW_wrapper endp
VerInstallFileA_wrapper proc
	jmp m_procs[11*8]
VerInstallFileA_wrapper endp
VerInstallFileW_wrapper proc
	jmp m_procs[12*8]
VerInstallFileW_wrapper endp
VerLanguageNameA_wrapper proc
	jmp m_procs[13*8]
VerLanguageNameA_wrapper endp
VerLanguageNameW_wrapper proc
	jmp m_procs[14*8]
VerLanguageNameW_wrapper endp
VerQueryValueA_wrapper proc
	jmp m_procs[15*8]
VerQueryValueA_wrapper endp
VerQueryValueW_wrapper proc
	jmp m_procs[16*8]
VerQueryValueW_wrapper endp
end
