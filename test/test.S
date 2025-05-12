; This is a test assembly file.
; Used to assess the assembler's capabilities.

mov r0, r1
rd32 r1, [sp]   ; load from stack
swp r1, r2
add r0, r1, r2
