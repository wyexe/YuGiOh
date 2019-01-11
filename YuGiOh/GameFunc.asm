.CODE

HookPullCard PROC
; ESI == 0 => ◊‘º∫  ESI == 1 => µ–»À
CMP ESI, 1
JE lb_EnemyRound
;movabs rax, 0x1410d7fc4

;
MOV RAX, 1410d7fc4h
MOVZX ECX, WORD PTR [RAX]
CMP CX, 1;
JNE lb_EnemyRound;

MOV RCX, 2;
MOV WORD PTR [RAX], CX


lb_EnemyRound:
; Restore
MOV RCX, 1410d7fc4h
MOVZX ECX, WORD PTR [RCX]
CMP CX, 0;
JE lb_Exit;


; Back
MOV RAX, 14049BD0Ch
JMP RAX;



lb_Exit:;
MOV RAX, 14049bac5h;
JMP RAX;


HookPullCard ENDP
END