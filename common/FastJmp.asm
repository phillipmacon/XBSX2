; XBSX2 - PS2 Emulator for Xbox Consoles
;
; XBSX2 is free software: you can redistribute it and/or modify it under the terms
; of the GNU Lesser General Public License as published by the Free Software Found-
; ation, either version 3 of the License, or (at your option) any later version.
;
; XBSX2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
; PURPOSE.  See the GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License along with XBSX2.
; If not, see <http://www.gnu.org/licenses/>.

; -----------------------------------------
; 64-bit X86
; -----------------------------------------
_TEXT         SEGMENT

PUBLIC fastjmp_set
PUBLIC fastjmp_jmp

; void fastjmp_set(fastjmp_buf*)
fastjmp_set   PROC
  mov rax, qword ptr [rsp]
  mov rdx, rsp                              ; fixup stack pointer, so it doesn't include the call to fastjmp_set
  add rdx, 8
  mov qword ptr [rcx], rax                  ; actually rip
  mov qword ptr [rcx + 8], rbx
  mov qword ptr [rcx + 16], rdx             ; actually rsp
  mov qword ptr [rcx + 24], rbp
  mov qword ptr [rcx + 32], rsi
  mov qword ptr [rcx + 40], rdi
  mov qword ptr [rcx + 48], r12
  mov qword ptr [rcx + 56], r13
  mov qword ptr [rcx + 64], r14
  mov qword ptr [rcx + 72], r15
  movaps xmmword ptr [rcx + 80], xmm6
  movaps xmmword ptr [rcx + 96], xmm7
  movaps xmmword ptr [rcx + 112], xmm8
  add rcx, 112                              ; split to two batches to fit displacement in a single byte
  movaps xmmword ptr [rcx + 16], xmm9
  movaps xmmword ptr [rcx + 32], xmm10
  movaps xmmword ptr [rcx + 48], xmm11
  movaps xmmword ptr [rcx + 64], xmm12
  movaps xmmword ptr [rcx + 80], xmm13
  movaps xmmword ptr [rcx + 96], xmm14
  movaps xmmword ptr [rcx + 112], xmm15
  xor eax, eax
  ret
fastjmp_set   ENDP

; void fastjmp_jmp(fastjmp_buf*, int)
fastjmp_jmp   PROC
  mov eax, edx                              ; return code
  mov rdx, qword ptr [rcx + 0]              ; actually rip
  mov rbx, qword ptr [rcx + 8]
  mov rsp, qword ptr [rcx + 16]
  mov rbp, qword ptr [rcx + 24]
  mov rsi, qword ptr [rcx + 32]
  mov rdi, qword ptr [rcx + 40]
  mov r12, qword ptr [rcx + 48]
  mov r13, qword ptr [rcx + 56]
  mov r14, qword ptr [rcx + 64]
  mov r15, qword ptr [rcx + 72]
  movaps xmm6, xmmword ptr [rcx + 80]
  movaps xmm7, xmmword ptr [rcx + 96]
  movaps xmm8, xmmword ptr [rcx + 112]
  add rcx, 112                              ; split to two batches to fit displacement in a single byte
  movaps xmm9, xmmword ptr [rcx + 16]
  movaps xmm10, xmmword ptr [rcx + 32]
  movaps xmm11, xmmword ptr [rcx + 48]
  movaps xmm12, xmmword ptr [rcx + 64]
  movaps xmm13, xmmword ptr [rcx + 80]
  movaps xmm14, xmmword ptr [rcx + 96]
  movaps xmm15, xmmword ptr [rcx + 112]
  jmp rdx
fastjmp_jmp   ENDP

_TEXT         ENDS

END
