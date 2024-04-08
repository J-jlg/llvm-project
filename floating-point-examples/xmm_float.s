.intel_syntax noprefix
.data
    double1: .double 200.5
    double2: .double 100.00
    float1: .float 25.0

.text
.global _start

_start:
movsd xmm0, qword ptr [double1]
addsd xmm0, qword ptr [double2]
cvtsd2si rax, xmm0
movss xmm1, dword ptr [float1]
sqrtss xmm1, xmm1
cvtss2si rbx, xmm1
cvtsd2ss xmm0, xmm0
ucomiss xmm0, xmm1
ja continue
mov rdi, 0
jmp exit
continue:
mov rdi, 1

exit:
mov eax, 60
syscall
