module Control
(
    Op_i,
    NoOp_i,
    RegWrite_o,
    MemtoReg_o,
    MemRead_o,
    MemWrite_o,
    ALUOp_o,
    ALUSrc_o,
    Branch_o
);

input   [6:0]    Op_i;
input            NoOp_i;
output           RegWrite_o;
output           MemtoReg_o;
output           MemRead_o;
output           MemWrite_o;
output  [1:0]    ALUOp_o;
output           ALUSrc_o;
output           Branch_o;

assign RegWrite_o = (NoOp_i ? 0 : ((Op_i == 7'b0110011 || Op_i == 7'b0010011 || Op_i == 7'b0000011)? 1 : 0));
assign MemtoReg_o = (NoOp_i ? 0 : (Op_i == 7'b0000011 ? 1 : 0));
assign MemRead_o = (NoOp_i ? 0 : (Op_i == 7'b0000011 ? 1 : 0));
assign MemWrite_o = (NoOp_i ? 0 : (Op_i == 7'b0100011 ? 1 : 0));
assign ALUOp_o = ((NoOp_i || Op_i == 7'b0110011)? 2'b00 : (Op_i == 7'b0010011 ? 2'b01 : ((Op_i == 7'b0000011 || Op_i == 7'b0100011) ? 2'b10 : 2'b11)));
assign ALUSrc_o = (NoOp_i ? 0 : ((Op_i == 7'b0010011 || Op_i == 7'b0000011 || Op_i == 7'b0100011)? 1 : 0));
assign Branch_o = (NoOp_i ? 0 : (Op_i == 7'b1100011 ? 1 : 0));

endmodule