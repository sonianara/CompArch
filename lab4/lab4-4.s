.data 

prompt1: .asciiz "Enter a high: "
prompt2: .asciiz "Enter a low: "
prompt3: .asciiz "Enter b high: "
prompt4: .asciiz "Enter b low: "
prompt5: .asciiz "Lower bit value: "
prompt6: .asciiz "Upper bit value: "

.text

main: 

li $v0, 4		#print message				
la $a0, prompt1
syscall
li $v0, 5		#read number
syscall
add $t0, $v0, $zero

li $v0, 4
la $a0, prompt2
syscall 
li $v0, 5
syscall
add $t1, $v0, $zero

li $v0, 4
la $a0, prompt3
syscall 
li $v0, 5
syscall
add $t2, $v0, $zero

li $v0, 4
la $a0, prompt4
syscall 
li $v0, 5
syscall
add $t3, $v0, $zero


double_add:

	addi $t5, 1  		#sign 

	add 	$t6, $t1, $t3		#add lower bits and store in $t6

	slt		$t7, $t6, $t1			#set $t7 to 1 if $t7 < $t6, 0 otherwise 
	beq 	$t5, $t7, skip 			#skip second check if carry already set 
	slt 	$t7, $t6, $t3
	
	skip: 
	add 	$t7, $t7, $t0		#add high bits to carry 
	add 	$t7, $t7, $t2

	li $v0, 4
	la $a0, prompt6
	syscall
	li $v0, 1 		#to print string
	add $a0, $t7, $zero
	syscall

	li $v0, 4
	la $a0, prompt5
	syscall
	li $v0, 1
	add $a0, $t6, $zero
	syscall

	li $v0, 10  
	syscall        

.end




