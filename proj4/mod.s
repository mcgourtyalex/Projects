MOD:

lui $r0 0
lui $r1 0
lui $r2 0
sw $r3 3($r0)
lui $r3 0

lw $r0 1($r3)
slt $r1 $r0 $r3
bne $r1 $r3 negative

continue:
lw $r1 0($r3)

subtracts:
sub $r1 $r1 $r0
slt $r2 $r1 $r3
bne $r2 $r3 done
j subtracts

negative:
sub $r0 $r3 $r0
j continue

done:
add $r1 $r0 $r1
sw $r1 2($r3)
lui $r0 0
lw $r3 3($r0)
jr $r3