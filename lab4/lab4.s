.data 


ascii: .word '0','1','2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
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
syscall				#Load input number into $v0

add $a0, $v0, $zero #Load input number into $a0
la $a1, buff

bintohex:
li $s2, 5
la $t5, ascii 		#loading address of ascii array
la $s1, intspace
sw $a0, 0($s1)


addi $t7, 0		#start counter at 0
lb $t0, 0($s1)		 #load byte value into $t0
andi $t3, $t0, 240 	#masking the upper 4 bits with 11110000
andi $t4, $t0, 15 	#masking the lower 4 bits with 00001111

			#upper 4 bits
add $t1, $t3, $t5	#find address of character
lb $t2, 0($t1)		#loading character into $t1
sb $t2, 0($a1)
add $a1, $a1, 1		#increment counter

			#lower 4 bits 
add $t1, $t4, $t5	#find address of character
lb $t2, 0($t1)		#loading character into $t1
sb $t2, 0($a1)
add $a1, $a1, 1		#increment counter
add $t7, 1		#decrement overall counter
blt $t7, $t2, bintohex

li $v0, 4 		#to print string
add $a0, $a1, $zero
syscall

li $v0, 10
syscall

.end




