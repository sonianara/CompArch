.data
error: .asciiz "Error: A zero field was nonzero"

.text

main:

li $a0, 0x6608C000
jal bit_fields
add $a0, $v0, $zero
li $v0, 1
syscall

li $v0, 10
syscall

bit_fields:

add $t0, $a0, $zero       # Load the argument into $

# fff, saved in $t1
add $t1, $t0, $zero
srl $t1, $t1, 29
sll $t1, $t1, 4

# 0 0, saved in $t2
add $t2, $t0, $zero
sll $t2, $t2, 3
srl $t2, $t2, 30

# nn, saved in $t3
add $t3, $t0, $zero
sll $t3, $t3, 5
srl $t3, $t3, 30

# 0 0000, saved $t4
add $t4, $t0, $zero
sll $t4, $t4, 7
srl $t4, $t4, 27

# x
add $t5, $t0, $zero # x# x# x# x
sll $t5, $t5, 12
srl $t5, $t5, 31
sll $t5, $t5, 8

# 000, saved in $t6
add $t6, $t0, $zero
sll $t6, $t6, 13
srl $t6, $t6, 29

# yyyy
add $t7, $t0, $zero
sll $t7, $t7, 16
srl $t7, $t7, 28
sll $t7, $t7, 12

add $s0, $t2, $zero
add $s0, $s0, $t4
add $s0, $s0, $t6

beq $s0, $zero, zeros_correct

li $v0, 4
la $a0, error
syscall

zeros_correct:

or $s1, $t1, $zero
or $s1, $s1, $t3
or $s1, $s1, $t5
or $s1, $s1, $t7

add $v0, $s1, $zero

jr $ra

.end
