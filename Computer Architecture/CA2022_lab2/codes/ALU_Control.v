module ALU_Control
(
    funct_i,
    ALUOp_i,
    ALUCtrl_o
);

input   [9:0]    funct_i;
input   [1:0]    ALUOp_i;
output  [2:0]    ALUCtrl_o;

`define   ADD   3'b000
`define   SUB   3'b001
`define   MUL   3'b010
`define   AND   3'b011
`define   XOR   3'b100
`define   SLL   3'b101
`define   SRAI  3'b110

assign ALUCtrl_o = (ALUOp_i == 2'b00 ? (funct_i == 10'b0000000000 ? `ADD :
                                        funct_i == 10'b0100000000 ? `SUB :
                                        funct_i == 10'b0000001000 ? `MUL :
                                        funct_i == 10'b0000000111 ? `AND :
                                        funct_i == 10'b0000000100 ? `XOR :
                                        `SLL) :
                    ALUOp_i == 2'b01 ? (funct_i[2:0] == 3'b000 ? `ADD : `SRAI) :
                    ALUOp_i == 2'b10 ? `ADD : `SUB
                    );
endmodule
