global getTimeRTC

section .text
getTimeRTC:
    ; [rdi] = *h, [rsi] = *m, [rdx]=*s

    ;horas
    mov al, 4
    out 70h, al
    in al, 71h
    mov [rdi], al

    ;minutos
    mov al, 2
    out 70h, al
    in al, 71h
    mov [rsi], al

    ;segundos
    mov al, 0
    out 70h, al
    in al, 71h
    mov [rdx], al

    ret