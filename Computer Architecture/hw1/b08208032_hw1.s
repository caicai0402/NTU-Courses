.globl __start

.rodata
    division_by_zero: .string "division by zero"

.text
__start:
    # Read first operand
    li a0, 5
    ecall
    mv s0, a0
    # Read operation
    li a0, 5
    ecall
    mv s1, a0
    # Read second operand
    li a0, 5
    ecall
    mv s2, a0

###################################
#  TODO: Develop your calculator  #
#                                 #
###################################
    addi t1, x0, 0
    beq s1, t1, myadd  # add 0
    addi t1, t1, 1
    beq s1, t1, mysub  # sub 1
    addi t1, t1, 1
    beq s1, t1, mymul  # mul 2
    addi t1, t1, 1
    beq s1, t1, mydiv  # div 3
    addi t1, t1, 1
    beq s1, t1, mymin  # min 4
    addi t1, t1, 1
    beq s1, t1, mypow  # pow 5
    addi t1, t1, 1
    beq s1, t1, myfac  # fac 6
    
myadd:
    add s3, s0, s2
    jal x0, output
    
mysub:
    sub s3, s0, s2
    jal x0, output
    
mymul:
    mul s3, s0, s2
    jal x0, output
    
mydiv:
    beq s2, x0, division_by_zero_except
    div s3, s0, s2
    jal x0, output
    
mymin:
    mv s3, s0
    bge s2, s0, output
    mv s3, s2
    jal x0, output
    
mypow:
    li s3, 1
    li t1, 0
    loop_in_mypow:
        beq t1, s2, output
        mul s3, s3, s0
        addi t1, t1, 1
        jal x0, loop_in_mypow
    
myfac:
    li s3, 1
    beq s0, x0, output
    li t1, 1
    loop_in_myfac:
        mul s3, s3, t1
        beq t1, s0, output
	addi t1, t1, 1
        jal x0, loop_in_myfac

division_by_zero_except:
    li a0, 4
    la a1, division_by_zero
    ecall
    jal zero, exit
    
output:
    # Output the result
    li a0, 1
    mv a1, s3
    ecall
    jal x0 exit

exit:
    # Exit program(necessary)
    li a0, 10
    ecall
