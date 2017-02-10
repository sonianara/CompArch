.data
	newline: .asciiz "\n"

.macro exit
   	li      $v0, 10
   	syscall
.end_macro

.macro print_int (%a)
   	li      $v0, 1
   	move    $a0, %a
   	syscall
.end_macro

.macro print_newline
	addi $v0, $zero, 4
	la $a0, newline
	syscall
.end_macro
    

.text
main: 

    li      $a0, 4
   	li      $a1, 16
   	li 	    $a2, 1

    jal     dars                # Call dars function
    
    move 	$s1, $v0
    move 	$s2, $v1
    print_int ($s1)
   	print_newline
   	
    print_int ($s2)
    print_newline
    	
    	
    li	    $a0, 8
   	li      $a1, 32
   	li 	    $a2, 1
    
    jal     dars                # Call dars function
   
   	move 	$s1, $v0
   	move 	$s2, $v1
    
   	print_int ($s1)
   	print_newline
    	
   	print_int ($s2)
   	print_newline
    	
    	
   	exit



# dars: "Double Arithmetic Right Shift" (part 5)
dars:
   	# $a0: Top 32 bits of the double
   	# $a1: Low 32 bits of the double
   	# $a2: Nummber of bits to shift
   	
   	addi    $t2, $zero, 32      # Load value 32 to register T2
   	sub     $t1, $t2, $a2       # T1 = 32 - number of bits to shift
  
   	sllv    $t0, $a0, $t1       # T0 = top 32 bits << (32 - # of bits to shift)
   
   	srlv    $a1, $a1, $a2       # Right shift $a1
   	or      $v1, $a1, $t0

   	srlv    $v0, $a0, $a2       # Right shift $a0
   
   	jr      $ra


.end



