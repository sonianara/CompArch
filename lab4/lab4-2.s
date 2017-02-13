.data 


ascii2: .byte '0','1','2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
prompt: .asciiz "Binary: "
buff: .space 10
intspace: 
	.align 2 
	.space 4
.text

main:
#call bintohex  
li $v0, 4
la $a0, prompt
syscall

li $v0, 5
syscall					#Load input number into $v0
add $a0, $v0, $zero 	#Load input number into $a0

li $t7, 8
la $a1, buff

bintohex:

	srl $t6, $a0, 28
	sll $a0, $a0, 4
	la $t5, ascii2
	add $t5, $t5, $t6
	lb $t6, 0($t5)
	sb $t6, 0($a1)
	addi $a1, $a1, 1
	addi $t7, $t7, -1
	bne $t7, $zero, bintohex
	sb $zero, 0($a1) 

la $a0, buff
li $v0, 4 		#to print string
syscall

li $v0, 10
syscall

.end




