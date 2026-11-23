.include "xc.inc"
.text ;BP (put the following data in ROM(program memory))
; This is a library, thus it can *not* contain a _main function: the C file will
; define main(). However, we will need a .global statement to make available ASM
; functions to C code.
; All functions utilized outside of this file will need to have a leading
; underscore (_) and be included in a comment delimited list below.
.global _delay1ms, _delay1us
_delay1ms:
repeat #15993
nop
return
    
_delay1us:
repeat #3
nop
return





