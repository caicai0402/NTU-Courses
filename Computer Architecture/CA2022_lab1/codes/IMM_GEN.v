module IMM_GEN
(
    data_i,
    data_o
);

// Ports
input   [31:0]      data_i;
output  [31:0]      data_o;

assign data_o = (data_i[6:0] == (7'b0010011) && data_i[14:12] == (3'b000))? {{20{data_i[31]}}, data_i[31:20]} : // addi
                (data_i[6:0] == (7'b0010011) && data_i[14:12] == (3'b101))? {{27{data_i[24]}}, data_i[24:20]} : // srai
                (data_i[6:0] == (7'b0000011))? {{20{data_i[31]}}, data_i[31:20]} :                              // lw
                (data_i[6:0] == (7'b0100011))? {{20{data_i[31]}}, data_i[31:25], data_i[11:7]} :                // sw
                {{20{data_i[31]}}, data_i[7], data_i[30:25], data_i[11:8], 1'b0};                               // beq

endmodule
