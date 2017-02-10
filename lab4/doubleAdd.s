.data 

prompt1: .asciiz "Enter a high: "
prompt2: .asciiz "Enter a low: "
prompt3: .asciiz "Enter b high: "
prompt4: .asciiz "Enter b low: "

.text

main: 

li $v0, 4		#print message
la $a0, prompt1
syscall
li $v0, 5		#read number
syscall
add $a0, $v0, $zero

li $v0, 4
la $a0, prompt2
syscall 
li $v0, 5
syscall
add $a1, $v0, $zero

li $v0, 4
la $a0, prompt3
syscall 
li $v0, 5
syscall
add $a2, $v0, $zero

li $v0, 4
la $a0, prompt4
syscall 
li $v0, 5
syscall
add $a3, $v0, $zero


double_add:
	or 	$t0, $t0, $a0	#set lower bits
	or 	$t1, $t1, $a1
	or 	$t2, $t2, $a2	#set higher bits 
	or 	$t3, $t3, $a3

	addi $t5, 1  		#sign 
	add $t6, $t6, 0
	add $t7, $t7, 0

	add 	$t6, $t0, $t1		#add lower bits and store in $t6

	slt		$t7, $t6, $t0			#set $v0 to 1 if $v1 < $t0, 0 otherwise 
	beq 	$t5, $v0, skip 		#skip second check if carry already set in case of weirdness
	slt 	$t7, $t6, $t1
	
	skip: 
	add 	$t7, $t7, $t2		#add high bits to carry 
	add 	$t7, $t7, $t3

	li $v0, 4 		#to print string
	add $a0, $a1, $zero
	syscall

	li $v0, 10  
	syscall        

.end




