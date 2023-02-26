module Forwarding_Unit
(
    EX_RS1addr_i,
    EX_RS2addr_i,
    MEM_RegWrite_i,
    MEM_RDaddr_i,
    WB_RegWrite_i,
    WB_RDaddr_i,
    ForwardA_o,
    ForwardB_o
);

// Ports
input   [4:0]       EX_RS1addr_i;
input   [4:0]       EX_RS2addr_i;
input               MEM_RegWrite_i;
input   [4:0]       MEM_RDaddr_i;
input               WB_RegWrite_i;
input   [4:0]       WB_RDaddr_i;
output              PCWrite_o;
output  [1:0]       ForwardA_o;
output  [1:0]       ForwardB_o;


assign ForwardA_o = (MEM_RegWrite_i && MEM_RDaddr_i != 0 && MEM_RDaddr_i == EX_RS1addr_i)? 2'b10 :
                  (WB_RegWrite_i && WB_RDaddr_i != 0 && !(MEM_RegWrite_i && MEM_RDaddr_i != 0 && MEM_RDaddr_i == EX_RS1addr_i) && WB_RDaddr_i == EX_RS1addr_i)? 2'b01 :
                  2'b00;

assign ForwardB_o = (MEM_RegWrite_i && MEM_RDaddr_i != 0 && MEM_RDaddr_i == EX_RS2addr_i)? 2'b10 :
                  (WB_RegWrite_i && WB_RDaddr_i != 0 && !(MEM_RegWrite_i && MEM_RDaddr_i != 0 && MEM_RDaddr_i == EX_RS2addr_i) && WB_RDaddr_i == EX_RS2addr_i)? 2'b01 :
                  2'b00;

endmodule
