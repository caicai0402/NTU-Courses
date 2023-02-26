module ALU
(
    data1_i,
    data2_i,
    ALUCtrl_i,
    data_o,
);

// Ports
input   [31:0]      data1_i;
input   [31:0]      data2_i;
input   [2:0]       ALUCtrl_i;
output  [31:0]      data_o;

`define   ADD   3'b000
`define   SUB   3'b001
`define   MUL   3'b010
`define   AND   3'b011
`define   XOR   3'b100
`define   SLL   3'b101
`define   SRAI  3'b110

assign data_o = ((ALUCtrl_i == `ADD)? data1_i + data2_i : 
                (ALUCtrl_i == `SUB)? data1_i - data2_i :
                (ALUCtrl_i == `MUL)? data1_i * data2_i :
                (ALUCtrl_i == `AND)? data1_i & data2_i :
                (ALUCtrl_i == `XOR)? data1_i ^ data2_i :
                (ALUCtrl_i == `SLL)? data1_i << data2_i :                
                $signed(data1_i) >>> data2_i[4:0]);

endmodule
