#	diagnostics.s - Diagnostics for MIPS CPU simulation

#	CPE 315 MIPS Simulation Test Suite (Version: 20170220)
#	This can be used in your development of Labs 5-7 for testing
#	of the various MIPS instruction types.

#	First, define some constant locations for lw and lb instructions

#	To generate the loadable binary, use mips_asm diagnostics.s

# 	Use your "single-step" feature to check the registers after each instruction
.data

test55:	.word	0x55555555
testAA:	.word	0xAAAAAAAA

.text

main:			# Main entry point for program

	nop				# assume your nop works!

#	Test R type instructions, starting with add

	ori	$t1, $zero, 0x0001	# let's add 1 and 2
	ori	$t2, $zero, 0x0002	# constant 2
	add	$t0, $t1, $t2		# results to $t0
	nop				# CHK: $t0 should be 0x00000003

#	Now try sign-extension on add

	ori	$t1, $zero, 0x000A	# let's add 10 and -1
	addi	$t2, $zero, -1		# constant -1, sign extended
	add	$t0, $t1, $t2		# $t2 should be 0xFFFFFFFF
	nop				# CHK: $t0 should be 0x00000009

#	Now try subtract

	ori	$t1, $zero, 256		# let's compute 256 - 2
	ori	$t2, $zero, 2		# constant 2
	sub	$t0, $t1, $t2		# results to $t0
	nop				# CHK: $t0 should be 0x000000FE

#	Now put a result in another register, $s0, NOT sign extended

	ori	$s0, $zero, 0xFFFF	# $s0 gets sign-extended 0xFFFF
	nop				# CHK: $s0 should be 0x0000FFFF

#	Do left shift by 1

	ori	$t1, $zero, 0x0004	# Load test value
	sll	$t0, $t1, 1		# shift $t1 left 1, result to $t0
	nop				# CHK: $t0 should be 0x00000008

#	Do shift 1 bit right logical

	srl	$t0, $t1, 1		# $t0 gets $t1 shifted right 1
	nop				# CHK: $t0 should be 0x00000002

#	Now test signed shift right, logical

	addi	$t1, $zero, -8		# $t1 should be 0xFFFFFFF8		
	srl	$t0, $t1, 2		# $t0 gets $t1 right shifted 2
	nop				# CHK: $t0 should be 0x3FFFFFFE

#	Now test signed shift right, arithmetic

	addi	$t1, $zero, -8		# $t1 should be 0xFFFFFFF8		
	sra	$t0, $t1, 2		# $t0 gets $t1 right shifted 2
	nop				# CHK: $t0 should be 0xFFFFFFFE

#	Now try variable shifts, starting with srlv

	ori	$t1, $zero, 0x0100	# $t1 gets 0x00000100
	ori	$t2, $zero, 5		# number of bits to shift
	srlv	$t0, $t1, $t2		# $t0 gets $t1 >> 5
	nop				# CHK: $t0 should be 0x00000008

#	Now try variable shift arithmetic

	addi	$t1, $zero, -32		# $t1 gets 0xFFFFFFE0
	ori	$t2, $zero, 3		# number of bits to shift
	srav	$t0, $t1, $t2		# $t0 gets $t1 >> 3
	nop				# CHK: $t0 should be 0xFFFFFFFC

#	Now try slt and sltu

	ori	$t1, $zero, 0x20	# $t1 is 0x20
	ori	$t2, $zero, 0x30	# $t2 is 0x30
	slt	$t0, $t1, $t2		# $t0 1 if $t1 < $t2
	nop				# CHK: $t0 should be 0x00000001

	ori	$t1, $zero, 0x30	# $t1 is 0x20
	ori	$t2, $zero, 0x20	# $t2 is 0x30
	slt	$t0, $t1, $t2		# $t0 1 if $t1 < $t2
	nop				# CHK: $t0 should be 0x00000000

	slti	$t0, $t1, 0x0040	# compare to literal value
	nop				# CHK: $t0 should be 1

#	Test branch instructions

	beq	$zero, $zero, next	# should always branch
	nop
	nop
next:	nop				# CHK: this is where we land !

#	Here's a better test: compare $t1 and $t2

	ori	$t1, $zero, 1
	ori	$t2, $zero, 2		# ok, they're not equal
	beq	$t1, $t2, next2		# should NOT take the branch
	nop
	nop
next2:	nop				# CHK: should get here by stepping down

#	Here's another test: basic loop, where the branch has a negative offset

	ori	$t1, $zero, 1
	ori	$t2, $zero, 4		# ok, they're not equal
next2a:	nop				# misc.
	nop				# doing stuff ....
	addi	$t2, $t2, -1		# decrement $t2
	bne	$t1, $t2, next2a	# should take the branch 3x
	nop				# CHK: $t2 should be 0x00000001

next3:	nop				# should get here by stepping down

#	Let's load words

	lw	$t0, test55		# loads from test55($zero)
	nop				# CHK: should be 0x55555555

	lw	$t0, testAA
	nop				# CHK: $t0 is: 0xAAAAAAAA

	lb	$t0, testAA
	nop				# CHK: $t0 is: 0xFFFFFFAA

	lbu	$t0, testAA
	nop				# CHK: $t0 is 0x000000AA

#	ok, now the jump, jal, and jr instructions

	j	next4			# should go down to next4
	nop
	nop				# skip over these
	nop
next4:	nop				# CHK: ok, we landed here.

	xor	$t0, $t0, $t0		# clear $t0
	jal	next5			# go to next5, then return to the following:
	nop				# $t0 should be a 0x00003333

	ori	$v0, $zero, 10		# code for syscall HALT
	syscall				# We're DONE !!!

#	Function: next5

next5:	ori	$t0, $zero, 0x3333	# CHK: final test: result to $t0
	jr	$ra			# Return from function

	.end	main			# entry point
