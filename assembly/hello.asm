.include "/sdcard/digital-design/assembly/setup/m328Pdef/m328Pdef.inc"

; Define registers
.def temp = r16
.def A = r17
.def B = r18
.def C = r19
.def D = r20
.def result = r21
.def temp1 = r22

; Initialize DDRD for output
ldi temp, 0b01000000 ; PD6 as output, PD5, PD4, PD3, PD2 as input
out DDRD, temp
ldi temp, 0b00111100 ; Enable pull-up resistors for PD5, PD4, PD3, PD2
out PORTD, temp

start:
; Set values of A, B, C, D
 in temp, PIND
    andi temp, (1<<PD2)
    breq clearA
    ldi A, 1
    rjmp continueA
clearA:
    clr A
continueA:

    in temp, PIND
    andi temp, (1<<PD3)
    breq clearB
    ldi B, 1
    rjmp continueB
clearB:
    clr B
continueB:

    in temp, PIND
    andi temp, (1<<PD4)
    breq clearC
    ldi C, 1
    rjmp continueC
clearC:
    clr C
continueC:

    in temp, PIND
    andi temp, (1<<PD5)
    breq clearD
    ldi D, 1
    rjmp continueD
clearD:
    clr D
continueD:

; Calculate ABC
mov result, A
and result, B
and result, C

; Calculate BD'
mov temp, B
mov temp1, D
com temp1
and temp,temp1
or result, temp

; Output result
sbrs result, 0
cbi PORTD, 6
sbrc result, 0
sbi PORTD, 6

rjmp start
