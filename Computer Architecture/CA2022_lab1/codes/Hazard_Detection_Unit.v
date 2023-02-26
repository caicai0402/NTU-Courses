module Hazard_Detection_Unit
(
    ID_RS1addr_i,
    ID_RS2addr_i,
    EX_RDaddr_i, 
    EX_MemRead_i,
    PCWrite_o,
    Stall_o,
    NoOp_o
);

// Ports
input   [4:0]       ID_RS1addr_i;
input   [4:0]       ID_RS2addr_i;
input   [4:0]       EX_RDaddr_i;
input               EX_MemRead_i;
output              PCWrite_o;
output              Stall_o;
output              NoOp_o;

assign PCWrite_o = (EX_MemRead_i && (ID_RS1addr_i == EX_RDaddr_i || ID_RS2addr_i == EX_RDaddr_i))? 0 : 1;
assign Stall_o = (EX_MemRead_i && (ID_RS1addr_i == EX_RDaddr_i || ID_RS2addr_i == EX_RDaddr_i))? 1 : 0;
assign NoOp_o = (EX_MemRead_i && (ID_RS1addr_i == EX_RDaddr_i || ID_RS2addr_i == EX_RDaddr_i))? 1 : 0;

endmodule
