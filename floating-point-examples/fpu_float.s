.intel_syntax noprefix
.data
    double_1: .double 2.0

.text
.global _start

_start:
fldpi
fmul qword ptr [double_1]
fsin
fist dword ptr [rsp]

mov eax, 60
mov edi, dword ptr [rsp]
syscall
