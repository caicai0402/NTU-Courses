.globl __start

.text
__start:       
 
main:
    jal ra, input
    addi a0, s0, 0
    addi s1, x0, 1
    jal ra, Fibonacci
    addi s0, a0, 0
    jal ra, output
    jal x0, exit
    
fun:
    jalr x0, 0(ra)
    
Fibonacci:
    ble a0, s1, fun  
    
    sw ra, 0(sp)
    addi sp sp -12  # 0: address, 4: f(n-1), 8: f(n-2)
    sw a0, 8(sp)
    
    addi a0, a0, -1
    jal ra Fibonacci
    sw a0, 4(sp)
    
    lw a0, 8(sp)
    addi a0, a0, -2
    jal ra Fibonacci
    
    lw t0, 4(sp)
    add a0, a0, t0
    
    addi sp, sp, 12
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
    jalr x0, 0(ra)

exit:
    li a0, 10
    ecall