.global __start

.data
space: .string " "

.text
__start:

main:
    jal ra, input
    beq s0, x0, exit    
    
    addi s1, s0, 0  # s1 = s0 = input
    li s4, 4        # s4 = 4
    mul t0, s1, s4  # t0 = s1 * s4
    sub sp, sp, t0  # sp = sp - t0
    addi s2, sp, 0  # s2 = sp
    li t1, 0
    
    Loop:
        jal ra, input 
        add t2, t1, s2    # t2 = t1 + sp
        sw s0, 0(t2)      # store input to t2
        addi t1, t1, 4    # t1 = t1 + 4
        bne t0, t1, Loop  # check if t0 == t1

    li a0, 0
    addi sp, sp, -4
    jal ra, inorder
    jal x0, exit

fun:
    jalr x0, 0(ra)

inorder:
    bge a0, s1, fun
    
    sw ra, 0(sp)
    addi sp, sp, -8
    
    sw a0, 4(sp)
    add a0, a0, a0
    addi a0, a0, 1
    jal ra, inorder
    
    lw a0, 4(sp)
    mul t0, a0, s4
    add t0, t0, s2
    lw s0, 0(t0)
    mv t1, a0        # output will use a0
    jal ra, output   
    mv a0, t1
        
    add a0, a0, a0
    addi a0, a0, 2
    jal ra, inorder
    
    addi sp, sp, 8
    lw ra, 0(sp)
    jalr x0, 0(ra)

input:
    li a0, 5
    ecall
    mv s0, a0
    jalr x0, 0(ra)
    
output:
    li a0, 1
    mv a1, s0
    ecall
    
    li a0, 4
    la a1, space   # print space
    ecall
    
    jalr x0, 0(ra)
    
exit:
    li a0, 10
    ecall