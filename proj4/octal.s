OCTAL:

lui $r0 0
lui $r1 0
lui $r2 0
sw $r3 1($r0)
lw $r0 0($r0)

addi $r3 $r3 1

andi $r1 $r0 7
addi $r2 $r1 0

andi $r1 $r0 56
sllv $r1 $r1 $r3
add $r2 $r2 $r1

andi $r1 $r1 0
addi $r1 $r1 112
sllv $r1 $r1 $r3
sllv $r1 $r1 $r3
and $r1 $r0 $r1
sllv $r1 $r1 $r3
sllv $r1 $r1 $r3
add $r2 $r2 $r1

disp $r2 0
lw $r3 1($r0)
jr $r3
