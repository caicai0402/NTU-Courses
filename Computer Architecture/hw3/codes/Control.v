module Control
(
    Op_i,
    ALUOp_o,
    ALUSrc_o,
    RegWrite_o
);

// Ports
input   [6:0]       Op_i;
output  [1:0]       ALUOp_o;
output              ALUSrc_o;
output              RegWrite_o;

assign ALUOp_o = (Op_i == (7'b0010011)? 2'b01 : 2'b00);
assign ALUSrc_o = (Op_i == (7'b0010011)? 1 : 0);
assign RegWrite_o = 1;

endmodule
